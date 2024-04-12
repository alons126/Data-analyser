//
// Created by alons on 09/04/2023.
//

#include "Directories.h"

// Constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Constructor">
Directories::Directories(const string &plots_path, const bool Clear_Old_Directories) {
    ClearOldDirectories = Clear_Old_Directories;

    string Plots_Folder = plots_path; // Plots_Folder = Parent_Folder
    system(("mkdir -p " + Plots_Folder).c_str()); // clear old stuff in Parent_Folder

    if (ClearOldDirectories) {
        system(("rm -r " + Plots_Folder + "/*").c_str()); // clear old stuff in Parent_Folder
    }

    //<editor-fold desc="Cut parameters plots directories">

    //<editor-fold desc="Number of Photo-electrons (Nphe) plots directories">
    for (string folders_name: Nphe_Daughter_Folders) {
        MakeDirectory(create_Nphe_Dir, Nphe_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Nphe_Directory_map["Nphe_1e_cut_BC_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "01_1e_cut") +
            "/"; // before cuts - i.e. plots before Nphe cuts
    Nphe_Directory_map["Nphe_1e_cut_AC_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "01_1e_cut") +
            "/"; // after cuts - i.e. plots after Nphe cuts

    Nphe_Directory_map["Nphe_1p_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "02_1p") +
            "/"; // 1p - i.e. plots after Nphe and all other cuts

    Nphe_Directory_map["Nphe_1n_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "03_1n") +
            "/"; // 1n - i.e. plots after Nphe and all other cuts

    Nphe_Directory_map["Nphe_2p_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "04_2p") +
            "/"; // 2p - i.e. plots after Nphe and all other cuts

    Nphe_Directory_map["Nphe_pFDpCD_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "05_pFDpCD") +
            "/"; // pFDpCD - i.e. plots after Nphe and all other cuts

    Nphe_Directory_map["Nphe_nFDpCD_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "06_nFDpCD") +
            "/"; // nFDpCD - i.e. plots after Nphe and all other cuts
    //</editor-fold>

    //<editor-fold desc="Chi2 plots directories">
    for (string folders_name: Chi2_Daughter_Folders) {
        MakeDirectory(create_chi2_Dir, Chi2_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Chi2_Directory_map["Chi2_All_e_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "00_All_e") + "/";

    Chi2_Directory_map["Chi2_1e_cut_Directory"] =
            Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "01_1e_cut") + "/";

    Chi2_Directory_map["Chi2_1p_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "02_1p") + "/";

    Chi2_Directory_map["Chi2_1n_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "03_1n") + "/";

    //    Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "04_1e2p_BC-AC_cut_tests") + "/";

    Chi2_Directory_map["Chi2_2p_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "04_2p") + "/";

    Chi2_Directory_map["Chi2_pFDpCD_Directory"] =
            Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "05_pFDpCD") + "/";

    Chi2_Directory_map["Chi2_nFDpCD_Directory"] =
            Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "06_nFDpCD") + "/";
    //</editor-fold>

    //<editor-fold desc="Vertex plots directories">
    for (string folders_name: Vertex_Daughter_Folders) {
        MakeDirectory(create_Vertex_Dir, Vertex_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Vertex_Directory_map["Vertex_Electron_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                   Find(Vertex_Daughter_Folders,
                                                                        "01_1e_cut/01_cParticles_Vtx_comp_plots/01_Electrons") + "/";
    Vertex_Directory_map["Vertex_Proton_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                 Find(Vertex_Daughter_Folders,
                                                                      "01_1e_cut/01_cParticles_Vtx_comp_plots/02_Protons") + "/";
    Vertex_Directory_map["Vertex_Kplus_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                Find(Vertex_Daughter_Folders, "01_1e_cut/01_cParticles_Vtx_comp_plots/03_Kplus") +
                                                                "/";
    Vertex_Directory_map["Vertex_Kminus_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                 Find(Vertex_Daughter_Folders,
                                                                      "01_1e_cut/01_cParticles_Vtx_comp_plots/04_Kminus") + "/";
    Vertex_Directory_map["Vertex_piplus_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                 Find(Vertex_Daughter_Folders,
                                                                      "01_1e_cut/01_cParticles_Vtx_comp_plots/05_Piplus") + "/";
    Vertex_Directory_map["Vertex_piminus_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                  Find(Vertex_Daughter_Folders,
                                                                       "01_1e_cut/01_cParticles_Vtx_comp_plots/06_Piminus") + "/";
    Vertex_Directory_map["Vertex_deuteron_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                   Find(Vertex_Daughter_Folders,
                                                                        "01_1e_cut/01_cParticles_Vtx_comp_plots/07_Deuteron") + "/";

    Vertex_Directory_map["Vertex_corr_Proton_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                      Find(Vertex_Daughter_Folders,
                                                                           "01_1e_cut/02_cParticles_dVtx_plots/01_Protons") + "/";
    Vertex_Directory_map["Vertex_corr_Kplus_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                     Find(Vertex_Daughter_Folders, "01_1e_cut/02_cParticles_dVtx_plots/02_Kplus") +
                                                                     "/";
    Vertex_Directory_map["Vertex_corr_Kminus_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                      Find(Vertex_Daughter_Folders,
                                                                           "01_1e_cut/02_cParticles_dVtx_plots/03_Kminus") + "/";
    Vertex_Directory_map["Vertex_corr_piplus_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                      Find(Vertex_Daughter_Folders,
                                                                           "01_1e_cut/02_cParticles_dVtx_plots/04_Piplus") + "/";
    Vertex_Directory_map["Vertex_corr_piminus_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                       Find(Vertex_Daughter_Folders,
                                                                            "01_1e_cut/02_cParticles_dVtx_plots/05_Piminus") + "/";
    Vertex_Directory_map["Vertex_corr_deuteron_1e_cut_Vtx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                                        Find(Vertex_Daughter_Folders,
                                                                             "01_1e_cut/02_cParticles_dVtx_plots/06_Deuteron") + "/";

    Vertex_Directory_map["Vertex_dV_1p_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                     Find(Vertex_Daughter_Folders, "02_1p/01_dV_plots") + "/";

    Vertex_Directory_map["Vertex_dV_1n_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                     Find(Vertex_Daughter_Folders, "03_1n/01_dV_plots") + "/";

    //    Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
//                                                          Find(Vertex_Daughter_Folders, "01_dV_plots/02_1e2p") + "/";

//    Vertex_Directory_map["Vertex_1e2p_dV_AC_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
//                                                          Find(Vertex_Daughter_Folders, "01_dV_plots/02_1e2p") + "/";

    Vertex_Directory_map["Vertex_dV_2p_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                     Find(Vertex_Daughter_Folders, "04_2p/01_dV_plots") + "/";

    Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                         Find(Vertex_Daughter_Folders, "05_pFDpCD/01_dV_plots") + "/";

    Vertex_Directory_map["Vertex_dV_nFDpCD_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                         Find(Vertex_Daughter_Folders, "06_nFDpCD/01_dV_plots") + "/";
    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) plots directories">
    for (string folders_name: SF_Daughter_Folders) {
        MakeDirectory(create_SF_Dir, SF_Parent_Directory, folders_name, false, Plots_Folder);
    }

    SF_Directory_map["SF_1e_cut_BC_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                 Find(SF_Daughter_Folders, "01_1e_cut/01_SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_1e_cut_BC_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                        Find(SF_Daughter_Folders, "01_1e_cut/02_SF_VS_P_e_plots") + "/";
    SF_Directory_map["SF_1e_cut_AC_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                 Find(SF_Daughter_Folders, "01_1e_cut/01_SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_1e_cut_AC_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                        Find(SF_Daughter_Folders, "01_1e_cut/02_SF_VS_P_e_plots") + "/";

    SF_Directory_map["SF_1p_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                          Find(SF_Daughter_Folders, "02_1p/01_SF_plots") + "/";

    SF_Directory_map["SF_VS_P_e_1p_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                 Find(SF_Daughter_Folders, "02_1p/02_SF_VS_P_e_plots") + "/";

    SF_Directory_map["SF_1n_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                          Find(SF_Daughter_Folders, "03_1n/01_SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_1n_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                 Find(SF_Daughter_Folders, "03_1n/02_SF_VS_P_e_plots") + "/";

    SF_Directory_map["SF_2p_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                          Find(SF_Daughter_Folders, "04_2p/01_SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_2p_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                 Find(SF_Daughter_Folders, "04_2p/02_SF_VS_P_e_plots") + "/";

    SF_Directory_map["SF_pFDpCD_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                              Find(SF_Daughter_Folders, "05_pFDpCD/01_SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_pFDpCD_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                     Find(SF_Daughter_Folders, "05_pFDpCD/02_SF_VS_P_e_plots") + "/";

    SF_Directory_map["SF_nFDpCD_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                              Find(SF_Daughter_Folders, "06_nFDpCD/01_SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_nFDpCD_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                     Find(SF_Daughter_Folders, "06_nFDpCD/02_SF_VS_P_e_plots") + "/";
    //</editor-fold>

    //<editor-fold desc="Edge cuts histograms plots directories">
    for (string folders_name: fiducial_Daughter_Folders) {
        MakeDirectory(create_fiducial_Dir, fiducial_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Fiducial_Directory_map["Edge_1e_BC_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                          Find(fiducial_Daughter_Folders, "01_1e_cut/01_PCAL") + "/";
    Fiducial_Directory_map["Edge_1e_BC_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                          Find(fiducial_Daughter_Folders, "01_1e_cut/01_PCAL") + "/";

    Fiducial_Directory_map["Edge_1p_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                       Find(fiducial_Daughter_Folders, "02_1p/01_PCAL") + "/";

    Fiducial_Directory_map["Edge_1n_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                       Find(fiducial_Daughter_Folders, "03_1n/01_PCAL") + "/";

    Fiducial_Directory_map["Edge_2p_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                       Find(fiducial_Daughter_Folders, "04_2p/01_PCAL") + "/";

    Fiducial_Directory_map["Edge_pFDpCD_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                           Find(fiducial_Daughter_Folders, "05_pFDpCD/01_PCAL") + "/";

    Fiducial_Directory_map["Edge_nFDpCD_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                           Find(fiducial_Daughter_Folders, "06_nFDpCD/01_PCAL") + "/";
    //</editor-fold>

    //<editor-fold desc="Momentum plots directories">
    for (string folders_name: Momentum_Daughter_Folders) {
        MakeDirectory(create_Momentum_Dir, Momentum_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Momentum_Directory_map["Momentum_1e_cut_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                          Find(Momentum_Daughter_Folders, "01_1e_cut") + "/";
    Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                                  Find(Momentum_Daughter_Folders, "01_1e_cut/01_Momentum_th_plots_1e_cut") + "/";

    Momentum_Directory_map["Momentum_1p_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                      Find(Momentum_Daughter_Folders, "02_1p") + "/";
    Momentum_Directory_map["Analysis_plots_momentum_1p_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                                     Find(Momentum_Daughter_Folders, "02_1p/Analysis_plots_1p") + "/";

    Momentum_Directory_map["Momentum_1n_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                      Find(Momentum_Daughter_Folders, "03_1n") + "/";
    Momentum_Directory_map["Analysis_plots_momentum_1n_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                                     Find(Momentum_Daughter_Folders, "03_1n/Analysis_plots_1n") + "/";

//    Momentum_Directory_map["Momentum_1e2p_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
    Find(Momentum_Daughter_Folders, "03_1e2p") + "/";

    Momentum_Directory_map["Momentum_2p_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                      Find(Momentum_Daughter_Folders, "04_2p") + "/";

    Momentum_Directory_map["Momentum_pFDpCD_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                          Find(Momentum_Daughter_Folders, "05_pFDpCD") + "/";
    Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                                         Find(Momentum_Daughter_Folders, "05_pFDpCD/Analysis_plots_pFDpCD") + "/";

    Momentum_Directory_map["Momentum_nFDpCD_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                          Find(Momentum_Daughter_Folders, "06_nFDpCD") + "/";
    Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                                         Find(Momentum_Daughter_Folders, "06_nFDpCD/Analysis_plots_nFDpCD") + "/";
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta plots directories">
    for (string folders_name: Beta_Daughter_Folders) {
        MakeDirectory(create_Beta_Dir, Beta_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Beta_Directory_map["Beta_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                              Find(Beta_Daughter_Folders, "01_Beta_fit_plots/03_1n") + "/";

    Beta_Directory_map["Neutron_beta_from_ph_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                              Find(Beta_Daughter_Folders,
                                                                   "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n") + "/";
    Beta_Directory_map["Neutron_beta_from_ph_01_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                 Find(Beta_Daughter_Folders,
                                                                      "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/01_fPhotons_All_plots_1n") +
                                                                 "/";
    Beta_Directory_map["Neutron_beta_from_ph_02_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                 Find(Beta_Daughter_Folders,
                                                                      "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/02_fPhotons_noPCAL_hit_plots_1n") +
                                                                 "/";
    Beta_Directory_map["Neutron_beta_from_ph_03_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                 Find(Beta_Daughter_Folders,
                                                                      "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/03_fPhotons_noPCAL_yesECIN_plots_1n") +
                                                                 "/";
    Beta_Directory_map["Neutron_beta_from_ph_04_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                 Find(Beta_Daughter_Folders,
                                                                      "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_1n") +
                                                                 "/";
//    Beta_Directory_map["Neutron_beta_from_ph_01_S1_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/01_Sector_1/01_fPhotons_All_plots_S1_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_02_S1_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/01_Sector_1/02_fPhotons_noPCAL_hit_plots_S1_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_03_S1_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/01_Sector_1/03_fPhotons_noPCAL_yesECIN_plots_S1_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_04_S1_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/01_Sector_1/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_S1_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_01_S2_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/02_Sector_2/01_fPhotons_All_plots_S2_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_02_S2_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/02_Sector_2/02_fPhotons_noPCAL_hit_plots_S2_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_03_S2_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/02_Sector_2/03_fPhotons_noPCAL_yesECIN_plots_S2_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_04_S2_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/02_Sector_2/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_S2_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_01_S3_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/03_Sector_3/01_fPhotons_All_plots_S3_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_02_S3_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/03_Sector_3/02_fPhotons_noPCAL_hit_plots_S3_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_03_S3_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/03_Sector_3/03_fPhotons_noPCAL_yesECIN_plots_S3_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_04_S3_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/03_Sector_3/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_S3_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_01_S4_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/04_Sector_4/01_fPhotons_All_plots_S4_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_02_S4_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/04_Sector_4/02_fPhotons_noPCAL_hit_plots_S4_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_03_S4_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/04_Sector_4/03_fPhotons_noPCAL_yesECIN_plots_S4_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_04_S4_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/04_Sector_4/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_S4_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_01_S5_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/05_Sector_5/01_fPhotons_All_plots_S5_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_02_S5_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/05_Sector_5/02_fPhotons_noPCAL_hit_plots_S5_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_03_S5_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/05_Sector_5/03_fPhotons_noPCAL_yesECIN_plots_S5_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_04_S5_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/05_Sector_5/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_S5_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_01_S6_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/06_Sector_6/01_fPhotons_All_plots_S6_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_02_S6_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/06_Sector_6/02_fPhotons_noPCAL_hit_plots_S6_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_03_S6_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/06_Sector_6/03_fPhotons_noPCAL_yesECIN_plots_S6_1n") +
//                                                                    "/";
//    Beta_Directory_map["Neutron_beta_from_ph_04_S6_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                                    Find(Beta_Daughter_Folders,
//                                                                         "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/05_Beta_fit_by_sector_1n/06_Sector_6/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_S6_1n") +
//                                                                    "/";

//    Beta_Directory_map["All_FD_neutrals_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                         Find(Beta_Daughter_Folders, "01_Beta_fit_plots/03_1n/02_All_FD_neutrals_beta_plots_1n") + "/";
//    Beta_Directory_map["All_FD_neutrals_01_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                            Find(Beta_Daughter_Folders,
//                                                                 "01_Beta_fit_plots/03_1n/02_All_FD_neutrals_beta_plots_1n/01_All_FD_neutrals_All_plots_1n") + "/";
//    Beta_Directory_map["All_FD_neutrals_02_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                            Find(Beta_Daughter_Folders,
//                                                                 "01_Beta_fit_plots/03_1n/02_All_FD_neutrals_beta_plots_1n/02_All_FD_neutrals_noPCAL_hit_plots_1n") + "/";
//    Beta_Directory_map["All_FD_neutrals_03_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                            Find(Beta_Daughter_Folders,
//                                                                 "01_Beta_fit_plots/03_1n/02_All_FD_neutrals_beta_plots_1n/03_All_FD_neutrals_noPCAL_yesECIN_plots_1n") +
//                                                            "/";
//    Beta_Directory_map["All_FD_neutrals_04_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                            Find(Beta_Daughter_Folders,
//                                                                 "01_Beta_fit_plots/03_1n/02_All_FD_neutrals_beta_plots_1n/04_All_FD_neutrals_noPCAL_noECIN_yesECOUT_plots_1n") +
//                                                            "/";
//
//    Beta_Directory_map["FD_neutrals_noPDG0_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                            Find(Beta_Daughter_Folders, "01_Beta_fit_plots/03_1n/03_FD_neutrals_beta_noPDG0_plots_1n") + "/";
//    Beta_Directory_map["FD_neutrals_noPDG0_01_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                               Find(Beta_Daughter_Folders,
//                                                                    "01_Beta_fit_plots/03_1n/03_FD_neutrals_beta_noPDG0_plots_1n/01_FD_neutrals_noPDG0_All_plots_1n") +
//                                                               "/";
//    Beta_Directory_map["FD_neutrals_noPDG0_02_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                               Find(Beta_Daughter_Folders,
//                                                                    "01_Beta_fit_plots/03_1n/03_FD_neutrals_beta_noPDG0_plots_1n/02_FD_neutrals_noPDG0_noPCAL_hit_plots_1n") +
//                                                               "/";
//    Beta_Directory_map["FD_neutrals_noPDG0_03_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                               Find(Beta_Daughter_Folders,
//                                                                    "01_Beta_fit_plots/03_1n/03_FD_neutrals_beta_noPDG0_plots_1n/03_FD_neutrals_noPDG0_noPCAL_yesECIN_plots_1n") +
//                                                               "/";
//    Beta_Directory_map["FD_neutrals_noPDG0_04_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
//                                                               Find(Beta_Daughter_Folders,
//                                                                    "01_Beta_fit_plots/03_1n/03_FD_neutrals_beta_noPDG0_plots_1n/04_FD_neutrals_noPDG0_noPCAL_noECIN_yesECOUT_plots_1n") +
//                                                               "/";

    Beta_Directory_map["Beta_vs_other_variables_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                 Find(Beta_Daughter_Folders,
                                                                      "01_Beta_fit_plots/03_1n/04_Beta_vs_other_variables_plots_1n") +
                                                                 "/";

    Beta_Directory_map["Beta_VS_P_All_e_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                      Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/00_All_e") + "/";
    Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/00_All_e/By_charge") + "/";

    Beta_Directory_map["Beta_VS_P_1e_cut_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                       Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/01_1e_cut") + "/";
    Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                 Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/01_1e_cut/By_charge") + "/";

    Beta_Directory_map["Beta_VS_P_1p_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                   Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/02_1p") + "/";
    Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                             Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/02_1p/By_charge") + "/";

    Beta_Directory_map["Beta_VS_P_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                   Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/03_1n") + "/";
    Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                             Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/03_1n/By_charge") + "/";

    Beta_Directory_map["Beta_VS_P_2p_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                   Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/04_2p") + "/";
    Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                             Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/04_2p/By_charge") + "/";

    Beta_Directory_map["Beta_VS_P_pFDpCD_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                       Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/05_pFDpCD") + "/";
    Beta_Directory_map["Beta_VS_P_by_charge_pFDpCD_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                 Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/05_pFDpCD/By_charge") + "/";

    Beta_Directory_map["Beta_nFDpCD_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                  Find(Beta_Daughter_Folders, "01_Beta_fit_plots/06_nFDpCD") + "/";
    Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                       Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/06_nFDpCD") + "/";
    Beta_Directory_map["Beta_VS_P_by_charge_nFDpCD_Directory"] = Plots_Folder + "/" + Beta_Parent_Directory + "/" +
                                                                 Find(Beta_Daughter_Folders, "02_Beta_VS_P_plots/06_nFDpCD/By_charge") + "/";
    //</editor-fold>

    //<editor-fold desc="W plots directories">
    for (string folders_name: W_Daughter_Folders) {
        MakeDirectory(create_W_Dir, W_Parent_Directory, folders_name, false, Plots_Folder);
    }

    W_Directory_map["W_1e_cut_Directory"] = Plots_Folder + "/" + W_Parent_Directory + "/" + Find(W_Daughter_Folders, "01_1e_cut") + "/";
    W_Directory_map["W_1p_Directory"] = Plots_Folder + "/" + W_Parent_Directory + "/" + Find(W_Daughter_Folders, "02_1p") + "/";
    W_Directory_map["W_1n_Directory"] = Plots_Folder + "/" + W_Parent_Directory + "/" + Find(W_Daughter_Folders, "03_1n") + "/";
    W_Directory_map["W_2p_Directory"] = Plots_Folder + "/" + W_Parent_Directory + "/" + Find(W_Daughter_Folders, "04_2p") + "/";
    W_Directory_map["W_pFDpCD_Directory"] = Plots_Folder + "/" + W_Parent_Directory + "/" + Find(W_Daughter_Folders, "05_pFDpCD") + "/";
    W_Directory_map["W_nFDpCD_Directory"] = Plots_Folder + "/" + W_Parent_Directory + "/" + Find(W_Daughter_Folders, "06_nFDpCD") + "/";
    //</editor-fold>

    //<editor-fold desc="Angle plots directories">
    for (string folders_name: Angle_Daughter_Folders) {
        MakeDirectory(create_Angle_Dir, Angle_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Angle_Directory_map["Theta_e_All_e_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                     Find(Angle_Daughter_Folders, "00_All_e/01_Theta_e_All_e_plots") + "/";
    Angle_Directory_map["Phi_e_All_e_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                   Find(Angle_Daughter_Folders, "00_All_e/02_Phi_e_All_e_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_All_e_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                              Find(Angle_Daughter_Folders, "00_All_e/03_Theta_e_VS_Phi_e_All_e_plots") + "/";
    Angle_Directory_map["Theta_neut_VS_Phi_neut_All_e_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                    Find(Angle_Daughter_Folders,
                                                                         "00_All_e/03_Theta_neut_VS_Phi_neut_All_e_plots") + "/";

    Angle_Directory_map["Theta_e_1e_cut_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "01_1e_cut/01_Theta_e_1e_cut_plots") + "/";
    Angle_Directory_map["Phi_e_1e_cut_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                    Find(Angle_Daughter_Folders, "01_1e_cut/02_Phi_e_1e_cut_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_1e_cut_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                               Find(Angle_Daughter_Folders, "01_1e_cut/03_Theta_e_VS_Phi_e_1e_cut_plots") + "/";
    Angle_Directory_map["Neutron_veto_1e_cut_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "01_1e_cut/04_Neutron_veto_1e_cut_plots") + "/";

    Angle_Directory_map["Theta_e_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "02_1p/01_Theta_e_1p_plots") + "/";
    Angle_Directory_map["Phi_e_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                Find(Angle_Daughter_Folders, "02_1p/02_Phi_e_1p_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "02_1p/03_Theta_e_VS_Phi_e_1p_plots") + "/";
    Angle_Directory_map["Theta_p_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "02_1p/04_Theta_p_1p") + "/";
    Angle_Directory_map["Phi_p_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                Find(Angle_Daughter_Folders, "02_1p/05_Phi_p_1p") + "/";
    Angle_Directory_map["Theta_p_VS_Phi_p_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "02_1p/06_Theta_p_VS_Phi_p_1p_plots") + "/";
    Angle_Directory_map["Opening_angle_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "02_1p/07_Opening_angles_1p_plots") + "/";

    Angle_Directory_map["Theta_e_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "03_1n/01_Theta_e_1n_plots") + "/";
    Angle_Directory_map["Phi_e_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                Find(Angle_Daughter_Folders, "03_1n/02_Phi_e_1n_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "03_1n/03_Theta_e_VS_Phi_e_1n_plots") + "/";
    Angle_Directory_map["Theta_n_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "03_1n/04_Theta_n_1n") + "/";
    Angle_Directory_map["Phi_n_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                Find(Angle_Daughter_Folders, "03_1n/05_Phi_n_1n") + "/";
    Angle_Directory_map["Theta_n_VS_Phi_n_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "03_1n/06_Theta_n_VS_Phi_n_1n_plots") + "/";
    Angle_Directory_map["Opening_angle_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "03_1n/07_Opening_angles_1n_plots") + "/";
    Angle_Directory_map["Neutron_veto_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                       Find(Angle_Daughter_Folders, "03_1n/08_Neutron_veto_1n_plots") + "/";

//    Angle_Directory_map["Theta_e_1e2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
//                                                    Find(Angle_Daughter_Folders, "05_1e2p/01_Theta_e_1e2p_plots") + "/";
//    Angle_Directory_map["Phi_e_1e2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
//                                                  Find(Angle_Daughter_Folders, "05_1e2p/02_Phi_e_1e2p_plots") + "/";
    Angle_Directory_map["Theta_e_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "04_2p/01_Theta_e_2p_plots") + "/";
    Angle_Directory_map["Phi_e_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                Find(Angle_Daughter_Folders, "04_2p/02_Phi_e_2p_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "04_2p/03_Theta_e_VS_Phi_e_2p_plots") + "/";
    Angle_Directory_map["Opening_angle_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "04_2p/04_Opening_angles_2p_plots") + "/";
    Angle_Directory_map["Opening_angle_by_interaction_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                       Find(Angle_Daughter_Folders,
                                                                            "04_2p/04_Opening_angles_2p_plots/00_Theta_p1_p2_by_interaction") +
                                                                       "/";
    Angle_Directory_map["Ghost_tracks_handling_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                Find(Angle_Daughter_Folders, "04_2p/05_Ghost_tracks_handling_2p_plots") + "/";
    Angle_Directory_map["CToF_hits_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                    Find(Angle_Daughter_Folders, "04_2p/05_Ghost_tracks_handling_2p_plots/01_CFoF_hits_2p_plots") +
                                                    "/";
    Angle_Directory_map["CToF_hits_monitoring_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                               Find(Angle_Daughter_Folders,
                                                                    "04_2p/05_Ghost_tracks_handling_2p_plots/01_CFoF_hits_2p_plots/00_CFoF_hits_monitoring_2p_plots") +
                                                               "/";
    Angle_Directory_map["Double_detection_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders,
                                                                "04_2p/05_Ghost_tracks_handling_2p_plots/02_Double_detection_2p_plots") + "/";
    Angle_Directory_map["Double_detection_monitoring_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                      Find(Angle_Daughter_Folders,
                                                                           "04_2p/05_Ghost_tracks_handling_2p_plots/02_Double_detection_2p_plots/00_Double_detection_monitoring_2p_plots") +
                                                                      "/";

    Angle_Directory_map["Theta_e_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "05_pFDpCD/01_Theta_e_pFDpCD_plots") + "/";
    Angle_Directory_map["Phi_e_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                    Find(Angle_Daughter_Folders, "05_pFDpCD/02_Phi_e_pFDpCD_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                               Find(Angle_Daughter_Folders, "05_pFDpCD/03_Theta_e_VS_Phi_e_pFDpCD_plots") + "/";
    Angle_Directory_map["Theta_pFD_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "05_pFDpCD/04_Theta_pFD_pFDpCD_plots") + "/";
    Angle_Directory_map["Phi_pFD_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "05_pFDpCD/05_Phi_pFD_pFDpCD_plots") + "/";
    Angle_Directory_map["Theta_pFD_VS_Phi_pFD_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders, "05_pFDpCD/06_Theta_pFD_VS_Phi_pFD_pFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Theta_pCD_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "05_pFDpCD/07_Theta_pCD_pFDpCD_plots") + "/";
    Angle_Directory_map["Phi_pCD_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "05_pFDpCD/08_Phi_pCD_pFDpCD_plots") + "/";
    Angle_Directory_map["Theta_pCD_VS_Phi_pCD_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders, "05_pFDpCD/09_Theta_pCD_VS_Phi_pCD_pFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Theta_tot_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "05_pFDpCD/10_Theta_tot_pFDpCD_plots") + "/";
    Angle_Directory_map["Phi_tot_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "05_pFDpCD/11_Phi_tot_pFDpCD_plots") + "/";
    Angle_Directory_map["Theta_tot_VS_Phi_tot_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders, "05_pFDpCD/12_Theta_tot_VS_Phi_tot_pFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Theta_rel_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "05_pFDpCD/13_Theta_rel_pFDpCD_plots") + "/";
    Angle_Directory_map["Phi_rel_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "05_pFDpCD/14_Phi_rel_pFDpCD_plots") + "/";
    Angle_Directory_map["Theta_rel_VS_Phi_rel_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders, "05_pFDpCD/15_Theta_rel_VS_Phi_rel_pFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Opening_angles_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                             Find(Angle_Daughter_Folders, "05_pFDpCD/16_Opening_angles_pFDpCD_plots") + "/";
    Angle_Directory_map["Opening_angles_by_interaction_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                            Find(Angle_Daughter_Folders,
                                                                                 "05_pFDpCD/16_Opening_angles_pFDpCD_plots/00_Theta_pFD_pCD_by_interaction") +
                                                                            "/";
    Angle_Directory_map["Ghost_tracks_handling_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                    Find(Angle_Daughter_Folders,
                                                                         "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots") + "/";
    Angle_Directory_map["CToF_hits_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders,
                                                             "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/01_CFoF_hits_pFDpCD_plots") + "/";
    Angle_Directory_map["CToF_hits_monitoring_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders,
                                                                        "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/01_CFoF_hits_pFDpCD_plots/00_CFoF_hits_Monitoring_pFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Double_detection_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                               Find(Angle_Daughter_Folders,
                                                                    "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/02_Double_detection_pFDpCD_plots") +
                                                               "/";
    Angle_Directory_map["Double_detection_monitoring_pFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                          Find(Angle_Daughter_Folders,
                                                                               "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/02_Double_detection_pFDpCD_plots/00_Double_detection_monitoring_pFDpCD_plots") +
                                                                          "/";

    Angle_Directory_map["Theta_e_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "06_nFDpCD/01_Theta_e_nFDpCD_plots") + "/";
    Angle_Directory_map["Phi_e_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                    Find(Angle_Daughter_Folders, "06_nFDpCD/02_Phi_e_nFDpCD_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                               Find(Angle_Daughter_Folders, "06_nFDpCD/03_Theta_e_VS_Phi_e_nFDpCD_plots") + "/";
    Angle_Directory_map["Theta_nFD_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "06_nFDpCD/04_Theta_nFD_nFDpCD_plots") + "/";
    Angle_Directory_map["Phi_nFD_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "06_nFDpCD/05_Phi_nFD_nFDpCD_plots") + "/";
    Angle_Directory_map["Theta_nFD_VS_Phi_nFD_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders, "06_nFDpCD/06_Theta_nFD_VS_Phi_nFD_nFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Theta_pCD_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "06_nFDpCD/07_Theta_pCD_nFDpCD_plots") + "/";
    Angle_Directory_map["Phi_pCD_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "06_nFDpCD/08_Phi_pCD_nFDpCD_plots") + "/";
    Angle_Directory_map["Theta_pCD_VS_Phi_pCD_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders, "06_nFDpCD/09_Theta_pCD_VS_Phi_pCD_nFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Theta_tot_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "06_nFDpCD/10_Theta_tot_nFDpCD_plots") + "/";
    Angle_Directory_map["Phi_tot_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "06_nFDpCD/11_Phi_tot_nFDpCD_plots") + "/";
    Angle_Directory_map["Theta_tot_VS_Phi_tot_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders, "06_nFDpCD/12_Theta_tot_VS_Phi_tot_nFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Theta_rel_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "06_nFDpCD/13_Theta_rel_nFDpCD_plots") + "/";
    Angle_Directory_map["Phi_rel_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "06_nFDpCD/14_Phi_rel_nFDpCD_plots") + "/";
    Angle_Directory_map["Theta_rel_VS_Phi_rel_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                   Find(Angle_Daughter_Folders, "06_nFDpCD/15_Theta_rel_VS_Phi_rel_nFDpCD_plots") +
                                                                   "/";
    Angle_Directory_map["Opening_angles_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                             Find(Angle_Daughter_Folders, "06_nFDpCD/16_Opening_angles_nFDpCD_plots") + "/";
    Angle_Directory_map["Opening_angles_by_interaction_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                            Find(Angle_Daughter_Folders,
                                                                                 "06_nFDpCD/16_Opening_angles_nFDpCD_plots/00_Theta_nFD_pCD_by_interaction") +
                                                                            "/";
    Angle_Directory_map["Double_detection_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                               Find(Angle_Daughter_Folders,
                                                                    "06_nFDpCD/17_Ghost_tracks_handling_nFDpCD_plots/02_Double_detection_nFDpCD_plots") +
                                                               "/";
    Angle_Directory_map["Double_detection_monitoring_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                          Find(Angle_Daughter_Folders,
                                                                               "06_nFDpCD/17_Ghost_tracks_handling_nFDpCD_plots/02_Double_detection_nFDpCD_plots/00_Double_detection_monitoring_nFDpCD_plots") +
                                                                          "/";
    Angle_Directory_map["Neutron_veto_nFDpCD_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "06_nFDpCD/18_Neutron_veto_nFDpCD_plots") + "/";

    Angle_Directory_map["Phi_Proton_1e2pXy_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                         Find(Angle_Daughter_Folders, "07_1e2pXy/04_Phi_Proton_1e2pXy_plots") + "/";
    //</editor-fold>

    //<editor-fold desc="Momentum transfer directories">
    for (string folders_name: Momentum_transfer_Daughter_Folders) {
        MakeDirectory(create_Momentum_transfer_Dir, Momentum_transfer_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Momentum_transfer_Directory_map["Momentum_transfer_All_e_Directory"] =
            Plots_Folder + "/" + Momentum_transfer_Parent_Directory + "/" + Find(Momentum_transfer_Daughter_Folders, "00_All_e") + "/";

    Momentum_transfer_Directory_map["Momentum_transfer_1e_cut_Directory"] =
            Plots_Folder + "/" + Momentum_transfer_Parent_Directory + "/" + Find(Momentum_transfer_Daughter_Folders, "01_1e_cut") + "/";

    Momentum_transfer_Directory_map["Momentum_transfer_1p_Directory"] =
            Plots_Folder + "/" + Momentum_transfer_Parent_Directory + "/" + Find(Momentum_transfer_Daughter_Folders, "02_1p") + "/";

    Momentum_transfer_Directory_map["Momentum_transfer_1n_Directory"] =
            Plots_Folder + "/" + Momentum_transfer_Parent_Directory + "/" + Find(Momentum_transfer_Daughter_Folders, "03_1n") + "/";

    Momentum_transfer_Directory_map["Momentum_transfer_2p_Directory"] =
            Plots_Folder + "/" + Momentum_transfer_Parent_Directory + "/" + Find(Momentum_transfer_Daughter_Folders, "04_2p") + "/";

    Momentum_transfer_Directory_map["Momentum_transfer_pFDpCD_Directory"] =
            Plots_Folder + "/" + Momentum_transfer_Parent_Directory + "/" + Find(Momentum_transfer_Daughter_Folders, "05_pFDpCD") + "/";

    Momentum_transfer_Directory_map["Momentum_transfer_nFDpCD_Directory"] =
            Plots_Folder + "/" + Momentum_transfer_Parent_Directory + "/" + Find(Momentum_transfer_Daughter_Folders, "06_nFDpCD") + "/";
    //</editor-fold>

    //<editor-fold desc="E_e plots directories">
    for (string folders_name: E_e_Daughter_Folders) {
        MakeDirectory(create_E_e_Dir, E_e_Parent_Directory, folders_name, false, Plots_Folder);
    }

    E_e_Directory_map["E_e_All_Int_1e_cut_stack_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                              Find(E_e_Daughter_Folders, "01_1e_cut") + "/";
    E_e_Directory_map["E_e_All_Int_1e_cut_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                        Find(E_e_Daughter_Folders, "01_1e_cut/E_e_plots_1e_cut") + "/";
    E_e_Directory_map["E_e_VS_Theta_e_All_Int_1e_cut_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                                   Find(E_e_Daughter_Folders, "01_1e_cut/E_e_VS_Theta_e_1e_cut") + "/";

    E_e_Directory_map["E_e_All_Int_1p_stack_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                          Find(E_e_Daughter_Folders, "02_1p") + "/";
    E_e_Directory_map["E_e_All_Int_1p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                    Find(E_e_Daughter_Folders, "02_1p/E_e_plots_1p") + "/";
    E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                               Find(E_e_Daughter_Folders, "02_1p/E_e_VS_Theta_e_1p") + "/";
    E_e_Directory_map["E_e_15_All_Int_1p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                       Find(E_e_Daughter_Folders, "02_1p/E_e_plots_1p/Around_15_deg_1p") + "/";

    E_e_Directory_map["E_e_All_Int_1n_stack_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                          Find(E_e_Daughter_Folders, "03_1n") + "/";
    E_e_Directory_map["E_e_All_Int_1n_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                    Find(E_e_Daughter_Folders, "03_1n/E_e_plots_1n") + "/";
    E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                               Find(E_e_Daughter_Folders, "03_1n/E_e_VS_Theta_e_1n") + "/";
    E_e_Directory_map["E_e_15_All_Int_1n_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                       Find(E_e_Daughter_Folders, "03_1n/E_e_plots_1n/Around_15_deg_1n") + "/";

    E_e_Directory_map["E_e_All_Int_2p_stack_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                          Find(E_e_Daughter_Folders, "04_2p") + "/";
    E_e_Directory_map["E_e_All_Int_2p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                    Find(E_e_Daughter_Folders, "04_2p/E_e_plots_2p") + "/";
    E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                               Find(E_e_Daughter_Folders, "04_2p/E_e_VS_Theta_e_2p") + "/";
    E_e_Directory_map["E_e_15_All_Int_2p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                       Find(E_e_Daughter_Folders, "04_2p/E_e_plots_2p/Around_15_deg_2p") + "/";

    E_e_Directory_map["E_e_All_Int_pFDpCD_stack_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                              Find(E_e_Daughter_Folders, "05_pFDpCD") + "/";
    E_e_Directory_map["E_e_All_Int_pFDpCD_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                        Find(E_e_Daughter_Folders, "05_pFDpCD/E_e_plots_pFDpCD") + "/";
    E_e_Directory_map["E_e_VS_Theta_e_All_Int_pFDpCD_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                                   Find(E_e_Daughter_Folders, "05_pFDpCD/E_e_VS_Theta_e_pFDpCD") + "/";
    E_e_Directory_map["E_e_15_All_Int_pFDpCD_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                           Find(E_e_Daughter_Folders, "05_pFDpCD/E_e_plots_pFDpCD/Around_15_deg_pFDpCD") + "/";

    E_e_Directory_map["E_e_All_Int_nFDpCD_stack_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                              Find(E_e_Daughter_Folders, "06_nFDpCD") + "/";
    E_e_Directory_map["E_e_All_Int_nFDpCD_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                        Find(E_e_Daughter_Folders, "06_nFDpCD/E_e_plots_nFDpCD") + "/";
    E_e_Directory_map["E_e_VS_Theta_e_All_Int_nFDpCD_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                                   Find(E_e_Daughter_Folders, "06_nFDpCD/E_e_VS_Theta_e_nFDpCD") + "/";
    E_e_Directory_map["E_e_15_All_Int_nFDpCD_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                           Find(E_e_Daughter_Folders, "06_nFDpCD/E_e_plots_nFDpCD/Around_15_deg_nFDpCD") + "/";
    //</editor-fold>

    //<editor-fold desc="ETrans plots directories">
    for (string folders_name: ETrans_Daughter_Folders) {
        MakeDirectory(create_ETrans_Dir, ETrans_Parent_Directory, folders_name, false, Plots_Folder);
    }

    ETrans_Directory_map["ETrans_15_stack_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                           Find(ETrans_Daughter_Folders, "02_1p/02_Around_15_Deg_1p") + "/";
    ETrans_Directory_map["ETrans_All_Int_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "02_1p/02_Around_15_Deg_1p/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "02_1p/02_Around_15_Deg_1p/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "02_1p/02_Around_15_Deg_1p/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "02_1p/02_Around_15_Deg_1p/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "02_1p/02_Around_15_Deg_1p/04_DIS_only") + "/";
    ETrans_Directory_map["ETrans_All_Ang_stack_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                Find(ETrans_Daughter_Folders, "02_1p/01_Every_theta_e_1p") + "/";
    ETrans_Directory_map["ETrans_All_Int_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "02_1p/01_Every_theta_e_1p/00_All_interactions") +
                                                                  "/";
    ETrans_Directory_map["ETrans_QEL_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "02_1p/01_Every_theta_e_1p/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "02_1p/01_Every_theta_e_1p/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "02_1p/01_Every_theta_e_1p/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "02_1p/01_Every_theta_e_1p/04_DIS_only") + "/";

    ETrans_Directory_map["ETrans_15_stack_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                           Find(ETrans_Daughter_Folders, "03_1n/02_Around_15_Deg_1n") + "/";
    ETrans_Directory_map["ETrans_All_Int_15_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "03_1n/02_Around_15_Deg_1n/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_15_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "03_1n/02_Around_15_Deg_1n/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_15_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "03_1n/02_Around_15_Deg_1n/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_15_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "03_1n/02_Around_15_Deg_1n/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_15_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "03_1n/02_Around_15_Deg_1n/04_DIS_only") + "/";
    ETrans_Directory_map["ETrans_All_Ang_stack_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n") + "/";
    ETrans_Directory_map["ETrans_All_Int_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/00_All_interactions") +
                                                                  "/";
    ETrans_Directory_map["ETrans_QEL_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/04_DIS_only") + "/";

    ETrans_Directory_map["ETrans_15_stack_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                           Find(ETrans_Daughter_Folders, "04_2p/02_Around_15_Deg_2p") + "/";
    ETrans_Directory_map["ETrans_All_Int_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "04_2p/02_Around_15_Deg_2p/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "04_2p/02_Around_15_Deg_2p/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "04_2p/02_Around_15_Deg_2p/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "04_2p/02_Around_15_Deg_2p/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "04_2p/02_Around_15_Deg_2p/04_DIS_only") + "/";
    ETrans_Directory_map["ETrans_All_Ang_stack_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                Find(ETrans_Daughter_Folders, "04_2p/01_Every_theta_e_2p") + "/";
    ETrans_Directory_map["ETrans_All_Int_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "04_2p/01_Every_theta_e_2p/00_All_interactions") +
                                                                  "/";
    ETrans_Directory_map["ETrans_QEL_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "04_2p/01_Every_theta_e_2p/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "04_2p/01_Every_theta_e_2p/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "04_2p/01_Every_theta_e_2p/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "04_2p/01_Every_theta_e_2p/04_DIS_only") + "/";

    ETrans_Directory_map["ETrans_15_stack_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                               Find(ETrans_Daughter_Folders, "05_pFDpCD/02_Around_15_Deg_pFDpCD") + "/";
    ETrans_Directory_map["ETrans_All_Int_15_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                 Find(ETrans_Daughter_Folders,
                                                                      "05_pFDpCD/02_Around_15_Deg_pFDpCD/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_15_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "05_pFDpCD/02_Around_15_Deg_pFDpCD/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_15_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "05_pFDpCD/02_Around_15_Deg_pFDpCD/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_15_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "05_pFDpCD/02_Around_15_Deg_pFDpCD/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_15_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "05_pFDpCD/02_Around_15_Deg_pFDpCD/04_DIS_only") + "/";
    ETrans_Directory_map["ETrans_All_Ang_stack_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                    Find(ETrans_Daughter_Folders, "05_pFDpCD/01_Every_theta_e_pFDpCD") + "/";
    ETrans_Directory_map["ETrans_All_Int_All_Ang_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                      Find(ETrans_Daughter_Folders,
                                                                           "05_pFDpCD/01_Every_theta_e_pFDpCD/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_All_Ang_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "05_pFDpCD/01_Every_theta_e_pFDpCD/01_QEL_only") +
                                                                  "/";
    ETrans_Directory_map["ETrans_MEC_All_Ang_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "05_pFDpCD/01_Every_theta_e_pFDpCD/02_MEC_only") +
                                                                  "/";
    ETrans_Directory_map["ETrans_RES_All_Ang_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "05_pFDpCD/01_Every_theta_e_pFDpCD/03_RES_only") +
                                                                  "/";
    ETrans_Directory_map["ETrans_DIS_All_Ang_pFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "05_pFDpCD/01_Every_theta_e_pFDpCD/04_DIS_only") +
                                                                  "/";

    ETrans_Directory_map["ETrans_15_stack_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                               Find(ETrans_Daughter_Folders, "06_nFDpCD/02_Around_15_Deg_nFDpCD") + "/";
    ETrans_Directory_map["ETrans_All_Int_15_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                 Find(ETrans_Daughter_Folders,
                                                                      "06_nFDpCD/02_Around_15_Deg_nFDpCD/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_15_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "06_nFDpCD/02_Around_15_Deg_nFDpCD/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_15_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "06_nFDpCD/02_Around_15_Deg_nFDpCD/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_15_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "06_nFDpCD/02_Around_15_Deg_nFDpCD/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_15_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "06_nFDpCD/02_Around_15_Deg_nFDpCD/04_DIS_only") + "/";
    ETrans_Directory_map["ETrans_All_Ang_stack_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                    Find(ETrans_Daughter_Folders, "06_nFDpCD/01_Every_theta_e_nFDpCD") + "/";
    ETrans_Directory_map["ETrans_All_Int_All_Ang_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                      Find(ETrans_Daughter_Folders,
                                                                           "06_nFDpCD/01_Every_theta_e_nFDpCD/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_All_Ang_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "06_nFDpCD/01_Every_theta_e_nFDpCD/01_QEL_only") +
                                                                  "/";
    ETrans_Directory_map["ETrans_MEC_All_Ang_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "06_nFDpCD/01_Every_theta_e_nFDpCD/02_MEC_only") +
                                                                  "/";
    ETrans_Directory_map["ETrans_RES_All_Ang_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "06_nFDpCD/01_Every_theta_e_nFDpCD/03_RES_only") +
                                                                  "/";
    ETrans_Directory_map["ETrans_DIS_All_Ang_nFDpCD_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "06_nFDpCD/01_Every_theta_e_nFDpCD/04_DIS_only") +
                                                                  "/";
    //</editor-fold>

    //<editor-fold desc="Ecal plots directories">
    for (string folders_name: Ecal_Daughter_Folders) {
        MakeDirectory(create_Ecal_Dir, Ecal_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Ecal_Directory_map["Ecal_stack_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                    Find(Ecal_Daughter_Folders, "02_1p") + "/";
    Ecal_Directory_map["Ecal_All_Int_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "02_1p/01_Ecal_rec_by_reaction_1p/00_All_interactions") + "/";
    Ecal_Directory_map["Ecal_QEL_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "02_1p/01_Ecal_rec_by_reaction_1p/01_QEL_only") + "/";
    Ecal_Directory_map["Ecal_MEC_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "02_1p/01_Ecal_rec_by_reaction_1p/02_MEC_only") + "/";
    Ecal_Directory_map["Ecal_RES_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "02_1p/01_Ecal_rec_by_reaction_1p/03_RES_only") + "/";
    Ecal_Directory_map["Ecal_DIS_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "02_1p/01_Ecal_rec_by_reaction_1p/04_DIS_only") + "/";
    Ecal_Directory_map["Ecal_rec_vs_Mom_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "02_1p/02_Ecal_rec_vs_Mom_1p") + "/";
    Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "02_1p/03_Ecal_rec_vs_Ang_1p") + "/";
    Ecal_Directory_map["Ecal_rec_vs_TKI_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "02_1p/04_Ecal_rec_vs_TKI_1p") + "/";
    Ecal_Directory_map["Chi2_Ecal_test_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                        Find(Ecal_Daughter_Folders, "02_1p/05_Chi2_for_Ecal_above_Ebeam_1p") + "/";

    Ecal_Directory_map["Ecal_stack_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                    Find(Ecal_Daughter_Folders, "03_1n") + "/";
    Ecal_Directory_map["Ecal_All_Int_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "03_1n/01_Ecal_rec_by_reaction_1n/00_All_interactions") + "/";
    Ecal_Directory_map["Ecal_QEL_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "03_1n/01_Ecal_rec_by_reaction_1n/01_QEL_only") + "/";
    Ecal_Directory_map["Ecal_MEC_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "03_1n/01_Ecal_rec_by_reaction_1n/02_MEC_only") + "/";
    Ecal_Directory_map["Ecal_RES_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "03_1n/01_Ecal_rec_by_reaction_1n/03_RES_only") + "/";
    Ecal_Directory_map["Ecal_DIS_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "03_1n/01_Ecal_rec_by_reaction_1n/04_DIS_only") + "/";
    Ecal_Directory_map["Ecal_rec_vs_Mom_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "03_1n/02_Ecal_rec_vs_Mom_1n") + "/";
    Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "03_1n/03_Ecal_rec_vs_Ang_1n") + "/";
    Ecal_Directory_map["Ecal_rec_vs_TKI_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "03_1n/04_Ecal_rec_vs_TKI_1n") + "/";
    Ecal_Directory_map["Chi2_Ecal_test_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                        Find(Ecal_Daughter_Folders, "03_1n/05_Chi2_for_Ecal_above_Ebeam_1n") + "/";

    Ecal_Directory_map["Ecal_stack_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                    Find(Ecal_Daughter_Folders, "04_2p") + "/";
    Ecal_Directory_map["Ecal_All_Int_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "04_2p/01_Ecal_rec_by_reaction_2p/00_All_interactions") + "/";
    Ecal_Directory_map["Ecal_QEL_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "04_2p/01_Ecal_rec_by_reaction_2p/01_QEL_only") + "/";
    Ecal_Directory_map["Ecal_MEC_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "04_2p/01_Ecal_rec_by_reaction_2p/02_MEC_only") + "/";
    Ecal_Directory_map["Ecal_RES_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "04_2p/01_Ecal_rec_by_reaction_2p/03_RES_only") + "/";
    Ecal_Directory_map["Ecal_DIS_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "04_2p/01_Ecal_rec_by_reaction_2p/04_DIS_only") + "/";
    Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "04_2p/02_Ecal_rec_vs_TKI_2p") + "/";
    Ecal_Directory_map["Chi2_Ecal_test_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                        Find(Ecal_Daughter_Folders, "04_2p/03_Chi2_for_Ecal_above_Ebeam_2p") + "/";

    Ecal_Directory_map["Ecal_stack_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                        Find(Ecal_Daughter_Folders, "05_pFDpCD") + "/";
    Ecal_Directory_map["Ecal_All_Int_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                          Find(Ecal_Daughter_Folders,
                                                               "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/00_All_interactions") + "/";
    Ecal_Directory_map["Ecal_QEL_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/01_QEL_only") + "/";
    Ecal_Directory_map["Ecal_MEC_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/02_MEC_only") + "/";
    Ecal_Directory_map["Ecal_RES_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/03_RES_only") + "/";
    Ecal_Directory_map["Ecal_DIS_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/04_DIS_only") + "/";
    Ecal_Directory_map["Ecal_rec_vs_Mom_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                             Find(Ecal_Daughter_Folders, "05_pFDpCD/02_Ecal_rec_vs_Mom_pFDpCD") + "/";
    Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                             Find(Ecal_Daughter_Folders, "05_pFDpCD/03_Ecal_rec_vs_Ang_pFDpCD") + "/";
    Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                             Find(Ecal_Daughter_Folders, "05_pFDpCD/04_Ecal_rec_vs_TKI_pFDpCD") + "/";
    Ecal_Directory_map["Chi2_Ecal_test_pFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                            Find(Ecal_Daughter_Folders, "05_pFDpCD/05_Chi2_for_Ecal_above_Ebeam_pFDpCD") + "/";

    Ecal_Directory_map["Ecal_stack_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                        Find(Ecal_Daughter_Folders, "06_nFDpCD") + "/";
    Ecal_Directory_map["Ecal_All_Int_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                          Find(Ecal_Daughter_Folders,
                                                               "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/00_All_interactions") + "/";
    Ecal_Directory_map["Ecal_QEL_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/01_QEL_only") + "/";
    Ecal_Directory_map["Ecal_MEC_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/02_MEC_only") + "/";
    Ecal_Directory_map["Ecal_RES_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/03_RES_only") + "/";
    Ecal_Directory_map["Ecal_DIS_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/04_DIS_only") + "/";
    Ecal_Directory_map["Ecal_rec_vs_Mom_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                             Find(Ecal_Daughter_Folders, "06_nFDpCD/02_Ecal_rec_vs_Mom_nFDpCD") + "/";
    Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                             Find(Ecal_Daughter_Folders, "06_nFDpCD/03_Ecal_rec_vs_Ang_nFDpCD") + "/";
    Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                             Find(Ecal_Daughter_Folders, "06_nFDpCD/04_Ecal_rec_vs_TKI_nFDpCD") + "/";
    Ecal_Directory_map["Chi2_Ecal_test_nFDpCD_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                            Find(Ecal_Daughter_Folders, "06_nFDpCD/05_Chi2_for_Ecal_above_Ebeam_nFDpCD") + "/";
    //</editor-fold>

    //<editor-fold desc="TKI plots directories">
    for (string folders_name: TKI_Daughter_Folders) {
        MakeDirectory(create_TKI_Dir, TKI_Parent_Directory, folders_name, false, Plots_Folder);
    }

    TKI_Directory_map["dP_T_vs_dAlpha_T_1p_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "02_1p") + "/";
    TKI_Directory_map["dP_T_1p_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                             Find(TKI_Daughter_Folders, "02_1p/01_dP_T_1p") + "/";
    TKI_Directory_map["dAlpha_T_1p_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                 Find(TKI_Daughter_Folders, "02_1p/02_dAlpha_T_1p") + "/";
    TKI_Directory_map["dPhi_T_1p_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                               Find(TKI_Daughter_Folders, "02_1p/03_dPhi_T_1p") + "/";

    TKI_Directory_map["dP_T_vs_dAlpha_T_1n_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "03_1n") + "/";
    TKI_Directory_map["dP_T_1n_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                             Find(TKI_Daughter_Folders, "03_1n/01_dP_T_1n") + "/";
    TKI_Directory_map["dAlpha_T_1n_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                 Find(TKI_Daughter_Folders, "03_1n/02_dAlpha_T_1n") + "/";
    TKI_Directory_map["dPhi_T_1n_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                               Find(TKI_Daughter_Folders, "03_1n/03_dPhi_T_1n") + "/";

    TKI_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                         Find(Ecal_Daughter_Folders, "04_2p") + "/";
    TKI_Directory_map["dP_T_2p_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                             Find(TKI_Daughter_Folders, "04_2p/01_dP_T_2p") + "/";
    TKI_Directory_map["dAlpha_T_2p_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                 Find(TKI_Daughter_Folders, "04_2p/02_dAlpha_T_2p") + "/";
    TKI_Directory_map["dPhi_T_2p_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                               Find(TKI_Daughter_Folders, "04_2p/03_dPhi_T_2p") + "/";

    TKI_Directory_map["dP_T_vs_dAlpha_T_pFDpCD_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                             Find(Ecal_Daughter_Folders, "05_pFDpCD") + "/";
    TKI_Directory_map["dP_T_pFDpCD_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                 Find(TKI_Daughter_Folders, "05_pFDpCD/01_dP_T_pFDpCD") + "/";
    TKI_Directory_map["dAlpha_T_pFDpCD_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                     Find(TKI_Daughter_Folders, "05_pFDpCD/02_dAlpha_T_pFDpCD") + "/";
    TKI_Directory_map["dPhi_T_pFDpCD_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                   Find(TKI_Daughter_Folders, "05_pFDpCD/03_dPhi_T_pFDpCD") + "/";

    TKI_Directory_map["dP_T_vs_dAlpha_T_nFDpCD_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                             Find(Ecal_Daughter_Folders, "06_nFDpCD") + "/";
    TKI_Directory_map["dP_T_nFDpCD_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                 Find(TKI_Daughter_Folders, "06_nFDpCD/01_dP_T_nFDpCD") + "/";
    TKI_Directory_map["dAlpha_T_nFDpCD_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                     Find(TKI_Daughter_Folders, "06_nFDpCD/02_dAlpha_T_nFDpCD") + "/";
    TKI_Directory_map["dPhi_T_nFDpCD_Directory"] = Plots_Folder + "/" + TKI_Parent_Directory + "/" +
                                                   Find(TKI_Daughter_Folders, "06_nFDpCD/03_dPhi_T_nFDpCD") + "/";
    //</editor-fold>

    //<editor-fold desc="ToF plots directories">
    for (string folders_name: ToF_Daughter_Folders) {
        MakeDirectory(create_ToF_Dir, ToF_Parent_Directory, folders_name, false, Plots_Folder);
    }

    ToF_Directory_map["Neutron_vs_cParticles_hits_1e_cut"] = Plots_Folder + "/" + ToF_Parent_Directory + "/" +
                                                             Find(ToF_Daughter_Folders, "01_1e_cut/01_Neutron_vs_cParticles_hits_FTOF_1e_cut") +
                                                             "/";
    ToF_Directory_map["Neutron_vs_cParticles_hits_nFDpCD"] = Plots_Folder + "/" + ToF_Parent_Directory + "/" +
                                                             Find(ToF_Daughter_Folders, "06_nFDpCD/01_Neutron_vs_cParticles_hits_FTOF_nFDpCD") +
                                                             "/";
    //</editor-fold>

    //<editor-fold desc="Efficiency & Acceptance Correction plots directories">
    for (string folders_name: Eff_and_ACorr_Daughter_Folders) {
        MakeDirectory(create_Eff_and_ACorr_Dir, Eff_and_ACorr_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Eff_and_ACorr_Directory_map["Eff_and_ACorr_1e_cut_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/"
                                                                    + Find(Eff_and_ACorr_Daughter_Folders, "01_1e_cut") + "/";
    Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                        Find(Eff_and_ACorr_Daughter_Folders,
                                                                             "01_1e_cut/01_Momentum_Eff_and_ACorr_plots_1e_cut") + "/";
    Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                          Find(Eff_and_ACorr_Daughter_Folders,
                                                                               "01_1e_cut/02_Theta_Eff_and_ACorr_plots_1e_cut") + "/";
    Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                        Find(Eff_and_ACorr_Daughter_Folders,
                                                                             "01_1e_cut/03_Phi_Eff_and_ACorr_plots_1e_cut") + "/";
    Eff_and_ACorr_Directory_map["TL_Acceptance_Maps_1e_cut_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                         Find(Eff_and_ACorr_Daughter_Folders, "01_1e_cut/04_Hit_maps") + "/";
    Eff_and_ACorr_Directory_map["Loaded_reco_ref_Acceptance_Maps_1e_cut_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                                      Find(Eff_and_ACorr_Daughter_Folders,
                                                                                           "01_1e_cut/05_Loaded_reco_ref_hit_maps") + "/";
    Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                     Find(Eff_and_ACorr_Daughter_Folders,
                                                                          "01_1e_cut/05_Eff_plots_for_Momentum_th") + "/";

    Eff_and_ACorr_Directory_map["Eff_and_ACorr_1p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/"
                                                                + Find(Eff_and_ACorr_Daughter_Folders, "02_1p") + "/";
    Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                    Find(Eff_and_ACorr_Daughter_Folders,
                                                                         "02_1p/01_Momentum_Eff_and_ACorr_plots_1p") + "/";
    Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                      Find(Eff_and_ACorr_Daughter_Folders,
                                                                           "02_1p/02_Theta_Eff_and_ACorr_plots_1p") + "/";
    Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                    Find(Eff_and_ACorr_Daughter_Folders, "02_1p/03_Phi_Eff_and_ACorr_plots_1p") +
                                                                    "/";
    Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                    Find(Eff_and_ACorr_Daughter_Folders, "02_1p/04_Proton_FD_Hit_map_1p") + "/";

    Eff_and_ACorr_Directory_map["Eff_and_ACorr_1n_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/"
                                                                + Find(Eff_and_ACorr_Daughter_Folders, "03_1n") + "/";
    Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                    Find(Eff_and_ACorr_Daughter_Folders,
                                                                         "03_1n/01_Momentum_Eff_and_ACorr_plots_1n") + "/";
    Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                      Find(Eff_and_ACorr_Daughter_Folders,
                                                                           "03_1n/02_Theta_Eff_and_ACorr_plots_1n") + "/";
    Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                    Find(Eff_and_ACorr_Daughter_Folders, "03_1n/03_Phi_Eff_and_ACorr_plots_1n") +
                                                                    "/";
    Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                     Find(Eff_and_ACorr_Daughter_Folders, "03_1n/04_Neutron_FD_Hit_map_1n") + "/";

    Eff_and_ACorr_Directory_map["Eff_and_ACorr_2p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/"
                                                                + Find(Eff_and_ACorr_Daughter_Folders, "04_2p") + "/";
    Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_2p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                    Find(Eff_and_ACorr_Daughter_Folders,
                                                                         "04_2p/01_Momentum_Eff_and_ACorr_plots_2p") + "/";
    Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_2p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                      Find(Eff_and_ACorr_Daughter_Folders,
                                                                           "04_2p/02_Theta_Eff_and_ACorr_plots_2p") + "/";
    Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_2p_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                    Find(Eff_and_ACorr_Daughter_Folders, "04_2p/03_Phi_Eff_and_ACorr_plots_2p") +
                                                                    "/";

    Eff_and_ACorr_Directory_map["Eff_and_ACorr_pFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/"
                                                                    + Find(Eff_and_ACorr_Daughter_Folders, "05_pFDpCD") + "/";
    Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                        Find(Eff_and_ACorr_Daughter_Folders,
                                                                             "05_pFDpCD/01_Momentum_Eff_and_ACorr_plots_pFDpCD") + "/";
    Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                          Find(Eff_and_ACorr_Daughter_Folders,
                                                                               "05_pFDpCD/02_Theta_Eff_and_ACorr_plots_pFDpCD") + "/";
    Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                        Find(Eff_and_ACorr_Daughter_Folders,
                                                                             "05_pFDpCD/03_Phi_Eff_and_ACorr_plots_pFDpCD") + "/";
    Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                        Find(Eff_and_ACorr_Daughter_Folders,
                                                                             "05_pFDpCD/04_Proton_FD_Hit_map_pFDpCD") + "/";

    Eff_and_ACorr_Directory_map["Eff_and_ACorr_nFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/"
                                                                    + Find(Eff_and_ACorr_Daughter_Folders, "06_nFDpCD") + "/";
    Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                        Find(Eff_and_ACorr_Daughter_Folders,
                                                                             "06_nFDpCD/01_Momentum_Eff_and_ACorr_plots_nFDpCD") + "/";
    Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                          Find(Eff_and_ACorr_Daughter_Folders,
                                                                               "06_nFDpCD/02_Theta_Eff_and_ACorr_plots_nFDpCD") + "/";
    Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                        Find(Eff_and_ACorr_Daughter_Folders,
                                                                             "06_nFDpCD/03_Phi_Eff_and_ACorr_plots_nFDpCD") + "/";
    Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"] = Plots_Folder + "/" + Eff_and_ACorr_Parent_Directory + "/" +
                                                                         Find(Eff_and_ACorr_Daughter_Folders,
                                                                              "06_nFDpCD/04_Neutron_FD_Hit_map_nFDpCD") + "/";
    //</editor-fold>

    //<editor-fold desc="Acceptance maps directories">
    for (string folders_name: Hit_Maps_Daughter_Folders) {
        MakeDirectory(create_Acceptance_Maps_Dir, Hit_Maps_Parent_Directory, folders_name, false, Plots_Folder);
    }

    AMaps_Directory_map["AMaps_1e_cut_Directory"] = Plots_Folder + "/" + Hit_Maps_Parent_Directory + "/"
                                                    + Find(Hit_Maps_Daughter_Folders, "01_AMaps_1e_cut") + "/";
    AMaps_Directory_map["AMaps_BC_1e_cut_Directory"] = Plots_Folder + "/" + Hit_Maps_Parent_Directory + "/"
                                                       + Find(Hit_Maps_Daughter_Folders, "01_AMaps_1e_cut/00a_AMaps_BC_from_file") + "/";
    AMaps_Directory_map["WMaps_1e_cut_Directory"] = Plots_Folder + "/" + Hit_Maps_Parent_Directory + "/"
                                                    + Find(Hit_Maps_Daughter_Folders, "02_WMaps_1e_cut") + "/";
    AMaps_Directory_map["WMaps_BC_1e_cut_Directory"] = Plots_Folder + "/" + Hit_Maps_Parent_Directory + "/"
                                                       + Find(Hit_Maps_Daughter_Folders, "02_WMaps_1e_cut/00a_WMaps_BC_from_file") + "/";
    //</editor-fold>

    //<editor-fold desc="Resolution plots directories">
    for (string folders_name: Resolution_Daughter_Folders) {
        MakeDirectory(create_Resolution_Dir, Resolution_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Resolution_Directory_map["Resolution_1p_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                          Find(Resolution_Daughter_Folders, "02_1p") + "/";
    Resolution_Directory_map["pRes_plots_1p_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                          Find(Resolution_Daughter_Folders, "02_1p/01_Proton_resolution_plots_1p") + "/";
    Resolution_Directory_map["Monitoring_1p_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                          Find(Resolution_Daughter_Folders, "02_1p/02_Match_monitoring_plots_1p") + "/";
    Resolution_Directory_map["Match_multi_1p_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                           Find(Resolution_Daughter_Folders, "02_1p/03_Match_multiplicity_plots_1p") + "/";
    Resolution_Directory_map["Basic_var_1p_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                         Find(Resolution_Daughter_Folders, "02_1p/04_Fit_justification_plots_1p/01_Basic_variables_1p") + "/";
    Resolution_Directory_map["Corr_just_1p_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                         Find(Resolution_Daughter_Folders, "02_1p/04_Fit_justification_plots_1p/02_Correction_1p") + "/";
    Resolution_Directory_map["Corr_Smear_1p_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                          Find(Resolution_Daughter_Folders, "02_1p/04_Fit_justification_plots_1p/03_Smearing_1p") + "/";

    Resolution_Directory_map["Resolution_1n_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                          Find(Resolution_Daughter_Folders, "03_1n") + "/";
    Resolution_Directory_map["nRes_plots_1n_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                          Find(Resolution_Daughter_Folders, "03_1n/01_Neutron_resolution_plots_1n") + "/";
    Resolution_Directory_map["Monitoring_1n_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                          Find(Resolution_Daughter_Folders, "03_1n/02_Match_monitoring_plots_1n") + "/";
    Resolution_Directory_map["Match_multi_1n_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                           Find(Resolution_Daughter_Folders, "03_1n/03_Match_multiplicity_plots_1n") + "/";
    Resolution_Directory_map["Basic_var_1n_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                         Find(Resolution_Daughter_Folders, "03_1n/04_Fit_justification_plots_1n/01_Basic_variables_1n") + "/";
    Resolution_Directory_map["Corr_just_1n_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                         Find(Resolution_Daughter_Folders, "03_1n/04_Fit_justification_plots_1n/02_Correction_1n") + "/";
    Resolution_Directory_map["Smear_just_1n_Directory"] = Plots_Folder + "/" + Resolution_Parent_Directory + "/" +
                                                          Find(Resolution_Daughter_Folders, "03_1n/04_Fit_justification_plots_1n/03_Smearing_1n") + "/";
    //</editor-fold>

    //<editor-fold desc="Multiplicity plots directories">
    for (string folders_name: Multiplicity_Daughter_Folders) {
        MakeDirectory(create_Multiplicity_Dir, Multiplicity_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"] = Plots_Folder + "/" + Multiplicity_Parent_Directory + "/" +
                                                                  Find(Multiplicity_Daughter_Folders, "01_1e_cut") + "/";

    Multiplicity_Directory_map["Multiplicity_1p_Directory"] = Plots_Folder + "/" + Multiplicity_Parent_Directory + "/" +
                                                              Find(Multiplicity_Daughter_Folders, "02_1p") + "/";

    Multiplicity_Directory_map["Multiplicity_1n_Directory"] = Plots_Folder + "/" + Multiplicity_Parent_Directory + "/" +
                                                              Find(Multiplicity_Daughter_Folders, "03_1n") + "/";

    Multiplicity_Directory_map["Multiplicity_pFDpCD_Directory"] = Plots_Folder + "/" + Multiplicity_Parent_Directory + "/" +
                                                                  Find(Multiplicity_Daughter_Folders, "05_pFDpCD") + "/";

    Multiplicity_Directory_map["Multiplicity_nFDpCD_Directory"] = Plots_Folder + "/" + Multiplicity_Parent_Directory + "/" +
                                                                  Find(Multiplicity_Daughter_Folders, "06_nFDpCD") + "/";
    //</editor-fold>

}
//</editor-fold>

// Private methods ------------------------------------------------------------------------------------------------------------------------------------------------------

bool Directories::FindSubstring(string string1, string string2) {
    if (string1.find(string2) != string::npos) {
        return true;
    } else {
        return false;
    }
}

string Directories::Find(vector <string> Vector, string String) {
    for (string Element: Vector) {
        if (FindSubstring(Element, String)) {
            return Element;
        }
    }

    return "";
}

// MakeDirectory function -----------------------------------------------------------------------------------------------------------------------------------------------

void Directories::MakeDirectory(const bool &Create_Directory, const string &Plots_Parent_Folder, const string &Plots_Daughter_Folder, const bool &Clear_Parent_Folder_content,
                                const string &Parent_Folder) {
    string MakeDirectory = "mkdir -p " + Parent_Folder;
    string RemoveDirectoryContent = "rm -r " + Parent_Folder + "/" + Plots_Parent_Folder + "/*";

    if (Create_Directory) {
        if (Clear_Parent_Folder_content) { system(RemoveDirectoryContent.c_str()); }

        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
    }

//    if (Clear_Parent_Folder_content == true && Create_Directory == true) {
//        system(RemoveDirectoryContent.c_str());
//        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
//    } else if (Clear_Parent_Folder_content == false && Create_Directory == true) {
//        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
//    }
}
