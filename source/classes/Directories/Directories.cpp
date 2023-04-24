//
// Created by alons on 09/04/2023.
//

#include "Directories.h"

// Private methods ------------------------------------------------------------------------------------------------------------------------------------------------------

bool Directories::FindSubstring(std::string string1, std::string string2) {
    if (string1.find(string2) != string::npos) {
        return true;
    } else {
        return false;
    }
}

std::string Directories::Find(vector<std::string> Vector, std::string String) {
    for (std::string Element: Vector) {
        if (FindSubstring(Element, String)) {
            return Element;
        }
    }

    return "";
}

// MakeDirectory function -----------------------------------------------------------------------------------------------------------------------------------------------

void Directories::MakeDirectory(bool Create_Directory, std::string Plots_Parent_Folder, std::string Plots_Daughter_Folder, bool Clear_Parent_Folder_content,
                                std::string Parent_Folder) {
    std::string MakeDirectory = "mkdir -p " + Parent_Folder;
    std::string RemoveDirectoryContent = "rm -r " + Parent_Folder + "/" + Plots_Parent_Folder + "/*";

    if (Clear_Parent_Folder_content == true && Create_Directory == true) {
        system(RemoveDirectoryContent.c_str());
        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
    } else if (Clear_Parent_Folder_content == false && Create_Directory == true) {
        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
    }
}

// Constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

Directories::Directories(std::string plots_path) {
    std::string Plots_Folder = plots_path; // Plots_Folder = Parent_Folder
    system(("mkdir -p " + Plots_Folder).c_str()); // clear old stuff in Parent_Folder
    system(("rm -r " + Plots_Folder + "*").c_str()); // clear old stuff in Parent_Folder

    //<editor-fold desc="Cut parameters plots directories">

    //<editor-fold desc="Number of Photo-electrons (Nphe) plots directories">
    for (std::string folders_name: Nphe_Daughter_Folders) {
        MakeDirectory(create_Nphe_Dir, Nphe_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Nphe_Directory_map["Nphe_1e_cut_BC_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "01_1e_cut") + "/"; // before cuts - i.e. plots before Nphe cuts
    Nphe_Directory_map["Nphe_1e_cut_AC_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "01_1e_cut") + "/"; // after cuts - i.e. plots after Nphe cuts
    Nphe_Directory_map["Nphe_1p_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "02_1p") + "/"; // 1p - i.e. plots after Nphe and all other cuts
    Nphe_Directory_map["Nphe_1n_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "04_1n") + "/"; // 1n - i.e. plots after Nphe and all other cuts
    Nphe_Directory_map["Nphe_2p_Directory"] =
            Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Find(Nphe_Daughter_Folders, "03_2p") + "/"; // 2p - i.e. plots after Nphe and all other cuts
    //</editor-fold>

    //<editor-fold desc="Chi2 plots directories">
    for (std::string folders_name: Chi2_Daughter_Folders) {
        MakeDirectory(create_chi2_Dir, Chi2_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Chi2_Directory_map["Chi2_All_e_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "01_All_e") + "/";
    Chi2_Directory_map["Chi2_1e_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "02_1e_cut") + "/";
    Chi2_Directory_map["Chi2_1p_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "03_1p") + "/";
    Chi2_Directory_map["Chi2_1n_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "06_1n") + "/";
    Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "04_1e2p_BC-AC_cut_tests") + "/";
    Chi2_Directory_map["Chi2_2p_Directory"] = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Find(Chi2_Daughter_Folders, "05_2p") + "/";
    //</editor-fold>

    //<editor-fold desc="Vertex plots directories">
    for (std::string folders_name: Vertex_Daughter_Folders) {
        MakeDirectory(create_Vertex_Dir, Vertex_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Vertex_Directory_map["Vertex_dV_1p_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                     Find(Vertex_Daughter_Folders, "02_dV_plots/01_1p") + "/";
    Vertex_Directory_map["Vertex_dV_1n_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                     Find(Vertex_Daughter_Folders, "02_dV_plots/04_1n") + "/";
    Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                          Find(Vertex_Daughter_Folders, "02_dV_plots/02_1e2p") + "/";
    Vertex_Directory_map["Vertex_1e2p_dV_AC_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                          Find(Vertex_Daughter_Folders, "02_dV_plots/02_1e2p") + "/";
    Vertex_Directory_map["Vertex_dV_2p_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                     Find(Vertex_Daughter_Folders, "02_dV_plots/03_2p") + "/";
    Vertex_Directory_map["Vertex_Electron_1e_Vx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                              Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/01_Electrons") + "/";
    Vertex_Directory_map["Vertex_Electron_1e_Vy_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                              Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/01_Electrons") + "/";
    Vertex_Directory_map["Vertex_Electron_1e_Vz_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                              Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/01_Electrons") + "/";
    Vertex_Directory_map["Vertex_Proton_1e_Vx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/02_Protons") + "/";
    Vertex_Directory_map["Vertex_Proton_1e_Vy_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/02_Protons") + "/";
    Vertex_Directory_map["Vertex_Proton_1e_Vz_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/02_Protons") + "/";
    Vertex_Directory_map["Vertex_Kplus_1e_Vx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                           Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/03_Kplus") + "/";
    Vertex_Directory_map["Vertex_Kplus_1e_Vy_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                           Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/03_Kplus") + "/";
    Vertex_Directory_map["Vertex_Kplus_1e_Vz_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                           Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/03_Kplus") + "/";
    Vertex_Directory_map["Vertex_Kminus_1e_Vx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/04_Kminus") + "/";
    Vertex_Directory_map["Vertex_Kminus_1e_Vy_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/04_Kminus") + "/";
    Vertex_Directory_map["Vertex_Kminus_1e_Vz_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/04_Kminus") + "/";
    Vertex_Directory_map["Vertex_piplus_1e_Vx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/05_Piplus") + "/";
    Vertex_Directory_map["Vertex_piplus_1e_Vy_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/05_Piplus") + "/";
    Vertex_Directory_map["Vertex_piplus_1e_Vz_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                            Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/05_Piplus") + "/";
    Vertex_Directory_map["Vertex_piminus_1e_Vx_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                             Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/06_Piminus") + "/";
    Vertex_Directory_map["Vertex_piminus_1e_Vy_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                             Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/06_Piminus") + "/";
    Vertex_Directory_map["Vertex_piminus_1e_Vz_Directory"] = Plots_Folder + "/" + Vertex_Parent_Directory + "/" +
                                                             Find(Vertex_Daughter_Folders, "01_Vertex_components_plots/01_1e_cut/06_Piminus") + "/";
    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) plots directories">
    for (std::string folders_name: SF_Daughter_Folders) {
        MakeDirectory(create_SF_Dir, SF_Parent_Directory, folders_name, false, Plots_Folder);
    }

    SF_Directory_map["SF_plots_1e_cut_BC_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                       Find(SF_Daughter_Folders, "01_1e_cut/SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_plots_1e_cut_BC_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                              Find(SF_Daughter_Folders, "01_1e_cut/SF_VS_P_e_plots") + "/";
    SF_Directory_map["SF_plots_1e_cut_AC_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                       Find(SF_Daughter_Folders, "01_1e_cut/SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_plots_1e_cut_AC_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                              Find(SF_Daughter_Folders, "01_1e_cut/SF_VS_P_e_plots") + "/";
    SF_Directory_map["SF_plots_1p_cuts_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                     Find(SF_Daughter_Folders, "02_1p/SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_plots_1p_cuts_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                            Find(SF_Daughter_Folders, "02_1p/SF_VS_P_e_plots") + "/";
    SF_Directory_map["SF_plots_1n_cuts_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                     Find(SF_Daughter_Folders, "04_1n/SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_plots_1n_cuts_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                            Find(SF_Daughter_Folders, "04_1n/SF_VS_P_e_plots") + "/";
    SF_Directory_map["SF_plots_2p_cuts_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                     Find(SF_Daughter_Folders, "03_2p/SF_plots") + "/";
    SF_Directory_map["SF_VS_P_e_plots_2p_cuts_Directory"] = Plots_Folder + "/" + SF_Parent_Directory + "/" +
                                                            Find(SF_Daughter_Folders, "03_2p/SF_VS_P_e_plots") + "/";
    //</editor-fold>

    //<editor-fold desc="Edge cuts histograms plots directories">
    for (std::string folders_name: fiducial_Daughter_Folders) {
        MakeDirectory(create_fiducial_Dir, fiducial_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                                    Find(fiducial_Daughter_Folders, "01_1e_cut/PCAL") + "/";
    Fiducial_Directory_map["fiducial_plots_1e_AC_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                                    Find(fiducial_Daughter_Folders, "01_1e_cut/PCAL") + "/";
    Fiducial_Directory_map["fiducial_plots_1p_cuts_histograms_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                                                 Find(fiducial_Daughter_Folders, "02_1p/PCAL") + "/";
    Fiducial_Directory_map["fiducial_plots_1n_cuts_histograms_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                                                 Find(fiducial_Daughter_Folders, "04_1n/PCAL") + "/";
    Fiducial_Directory_map["fiducial_plots_2p_cuts_histograms_PCAL_Directory"] = Plots_Folder + "/" + fiducial_Parent_Directory + "/" +
                                                                                 Find(fiducial_Daughter_Folders, "03_2p/PCAL") + "/";
    //</editor-fold>

    //<editor-fold desc="Momentum plots directories">
    for (std::string folders_name: Momentum_Daughter_Folders) {
        MakeDirectory(create_Momentum_Dir, Momentum_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Momentum_Directory_map["Momentum_1e_cut_tests_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                                Find(Momentum_Daughter_Folders, "01_1e_cut") + "/";
    Momentum_Directory_map["Momentum_1p_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                      Find(Momentum_Daughter_Folders, "02_1p") + "/";
    Momentum_Directory_map["Momentum_1n_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                      Find(Momentum_Daughter_Folders, "05_1n") + "/";
    Momentum_Directory_map["Momentum_1e2p_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                        Find(Momentum_Daughter_Folders, "03_1e2p") + "/";
    Momentum_Directory_map["Momentum_2p_Directory"] = Plots_Folder + "/" + Momentum_Parent_Directory + "/" +
                                                      Find(Momentum_Daughter_Folders, "04_2p") + "/";
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. p plots directories">
    for (std::string folders_name: Beta_VS_P_Daughter_Folders) {
        MakeDirectory(create_Beta_vs_P_Dir, Beta_VS_P_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Beta_VS_P_Directory_map["Beta_VS_P_All_e_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                           Find(Beta_VS_P_Daughter_Folders, "01_All_e") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_by_charge_All_e_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                                     Find(Beta_VS_P_Daughter_Folders, "01_All_e/By_charge") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_1e_cut_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                            Find(Beta_VS_P_Daughter_Folders, "02_1e_cut") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                                      Find(Beta_VS_P_Daughter_Folders, "02_1e_cut/By_charge") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_1p_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                        Find(Beta_VS_P_Daughter_Folders, "04_1p") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_by_charge_1p_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                                  Find(Beta_VS_P_Daughter_Folders, "04_1p/By_charge") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_1n_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                        Find(Beta_VS_P_Daughter_Folders, "06_1n") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_by_charge_1n_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                                  Find(Beta_VS_P_Daughter_Folders, "06_1n/By_charge") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_2p_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                        Find(Beta_VS_P_Daughter_Folders, "05_2p") + "/";
    Beta_VS_P_Directory_map["Beta_VS_P_by_charge_2p_Directory"] = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" +
                                                                  Find(Beta_VS_P_Daughter_Folders, "05_2p/By_charge") + "/";
    //</editor-fold>

    //<editor-fold desc="Angle plots directories">
    for (std::string folders_name: Angle_Daughter_Folders) {
        MakeDirectory(create_Angle_Dir, Angle_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Angle_Directory_map["Theta_e_All_e_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                     Find(Angle_Daughter_Folders, "01_All_e/01_Theta_e_All_e_plots") + "/";
    Angle_Directory_map["Phi_e_All_e_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                   Find(Angle_Daughter_Folders, "01_All_e/02_Phi_e_All_e_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_All_e_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                              Find(Angle_Daughter_Folders, "01_All_e/03_Theta_e_VS_Phi_e_All_e_plots") + "/";
    Angle_Directory_map["Theta_e_1e_cut_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "02_1e_cut/01_Theta_e_1e_cut_plots") + "/";
    Angle_Directory_map["Phi_e_1e_cut_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                    Find(Angle_Daughter_Folders, "02_1e_cut/02_Phi_e_1e_cut_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_1e_cut_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                               Find(Angle_Daughter_Folders, "02_1e_cut/03_Theta_e_VS_Phi_e_1e_cut_plots") + "/";

    Angle_Directory_map["Theta_e_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "03_1p/01_Theta_e_1p_plots") + "/";
    Angle_Directory_map["Phi_e_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                Find(Angle_Daughter_Folders, "03_1p/02_Phi_e_1p_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_1p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "03_1p/03_Theta_e_VS_Phi_e_1p_plots") + "/";
    Angle_Directory_map["Opening_angle_Directory_1p"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "03_1p/04_Opening_angles_1p_plots") + "/";
    Angle_Directory_map["Theta_p_Directory_1p"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "03_1p/05_Theta_p_1p") + "/";
    Angle_Directory_map["Phi_p_Directory_1p"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "03_1p/06_Phi_p_1p") + "/";

    Angle_Directory_map["Theta_e_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "08_1n/01_Theta_e_1n_plots") + "/";
    Angle_Directory_map["Phi_e_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                Find(Angle_Daughter_Folders, "08_1n/02_Phi_e_1n_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_1n_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "08_1n/03_Theta_e_VS_Phi_e_1n_plots") + "/";
    Angle_Directory_map["Opening_angle_Directory_1n"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                        Find(Angle_Daughter_Folders, "08_1n/04_Opening_angles_1n_plots") + "/";
    Angle_Directory_map["Theta_n_Directory_1n"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "08_1n/05_Theta_n_1n") + "/";
    Angle_Directory_map["Phi_n_Directory_1n"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "08_1n/06_Phi_n_1n") + "/";

    Angle_Directory_map["Theta_e_1e2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                    Find(Angle_Daughter_Folders, "05_1e2p/01_Theta_e_1e2p_plots") + "/";
    Angle_Directory_map["Phi_e_1e2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "05_1e2p/02_Phi_e_1e2p_plots") + "/";
    Angle_Directory_map["Theta_e_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                  Find(Angle_Daughter_Folders, "06_2p/01_Theta_e_2p_plots") + "/";
    Angle_Directory_map["Phi_e_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                Find(Angle_Daughter_Folders, "06_2p/02_Phi_e_2p_plots") + "/";
    Angle_Directory_map["Theta_e_VS_Phi_e_2p_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                           Find(Angle_Daughter_Folders, "06_2p/03_Theta_e_VS_Phi_e_2p_plots") + "/";
    Angle_Directory_map["Opening_angle_Directory_2p"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                     Find(Angle_Daughter_Folders, "06_2p/04_Opening_angles_2p_plots") + "/";

    Angle_Directory_map["Theta_p1_p2_by_interaction_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                                  Find(Angle_Daughter_Folders, "06_2p/04_Opening_angles_2p_plots/01_Theta_p1_p2_by_interaction") + "/";
    Angle_Directory_map["Phi_Proton_1e2pXy_Directory"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                         Find(Angle_Daughter_Folders, "07_1e2pXy/04_Phi_Proton_1e2pXy_plots") + "/";
    Angle_Directory_map["Ghost_tracks_handling_2p"] = Plots_Folder + "/" + Angle_Parent_Directory + "/" +
                                                      Find(Angle_Daughter_Folders, "06_2p/05_Ghost_tracks_handling_2p_plots") + "/";
    //TODO: reorganize properly

    //</editor-fold>

    //<editor-fold desc="Q2 plots directories">
    for (std::string folders_name: Q2_Daughter_Folders) {
        MakeDirectory(create_Q2_Dir, Q2_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Q2_Directory_map["Q2_All_e_Directory"] = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Find(Q2_Daughter_Folders, "01_All_e") + "/";
    Q2_Directory_map["Q2_1e_cut_Directory"] = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Find(Q2_Daughter_Folders, "02_1e_cut") + "/";
    Q2_Directory_map["Q2_1p_Directory"] = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Find(Q2_Daughter_Folders, "03_1p") + "/";
    Q2_Directory_map["Q2_1n_Directory"] = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Find(Q2_Daughter_Folders, "06_1n") + "/";
    Q2_Directory_map["Q2_1e2p_Directory"] = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Find(Q2_Daughter_Folders, "04_1e2p") + "/";
    Q2_Directory_map["Q2_2p_Directory"] = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Find(Q2_Daughter_Folders, "05_2p") + "/";
    //</editor-fold>

    //<editor-fold desc="E_e plots directories">
    for (std::string folders_name: E_e_Daughter_Folders) {
        MakeDirectory(create_E_e_Dir, E_e_Parent_Directory, folders_name, false, Plots_Folder);
    }

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
                                                          Find(E_e_Daughter_Folders, "04_1n") + "/";
    E_e_Directory_map["E_e_All_Int_1n_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                    Find(E_e_Daughter_Folders, "04_1n/E_e_plots_1n") + "/";
    E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                               Find(E_e_Daughter_Folders, "04_1n/E_e_VS_Theta_e_1n") + "/";
    E_e_Directory_map["E_e_15_All_Int_1n_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                       Find(E_e_Daughter_Folders, "04_1n/E_e_plots_1n/Around_15_deg_1n") + "/";
    E_e_Directory_map["E_e_All_Int_2p_stack_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                          Find(E_e_Daughter_Folders, "03_2p") + "/";
    E_e_Directory_map["E_e_All_Int_2p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                    Find(E_e_Daughter_Folders, "03_2p/E_e_plots_2p") + "/";
    E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                               Find(E_e_Daughter_Folders, "03_2p/E_e_VS_Theta_e_2p") + "/";
    E_e_Directory_map["E_e_15_All_Int_2p_Directory"] = Plots_Folder + "/" + E_e_Parent_Directory + "/" +
                                                       Find(E_e_Daughter_Folders, "03_2p/E_e_plots_2p/Around_15_deg_2p") + "/";
    //</editor-fold>

    //<editor-fold desc="ETrans plots directories">
    for (std::string folders_name: ETrans_Daughter_Folders) {
        MakeDirectory(create_ETrans_Dir, ETrans_Parent_Directory, folders_name, false, Plots_Folder);
    }

    ETrans_Directory_map["ETrans_15_stack_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                           Find(ETrans_Daughter_Folders, "02_2p/02_Around_15_Deg_2p") + "/";
    ETrans_Directory_map["ETrans_All_Int_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "02_2p/02_Around_15_Deg_2p/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "02_2p/02_Around_15_Deg_2p/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "02_2p/02_Around_15_Deg_2p/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "02_2p/02_Around_15_Deg_2p/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_15_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "02_2p/02_Around_15_Deg_2p/04_DIS_only") + "/";
    ETrans_Directory_map["ETrans_All_Ang_stack_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                Find(ETrans_Daughter_Folders, "02_2p/01_Every_theta_e_2p") + "/";
    ETrans_Directory_map["ETrans_All_Int_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "02_2p/01_Every_theta_e_2p/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "02_2p/01_Every_theta_e_2p/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "02_2p/01_Every_theta_e_2p/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "02_2p/01_Every_theta_e_2p/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_All_Ang_2p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "02_2p/01_Every_theta_e_2p/04_DIS_only") + "/";

    ETrans_Directory_map["ETrans_15_stack_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                           Find(ETrans_Daughter_Folders, "01_1p/02_Around_15_Deg_1p") + "/";
    ETrans_Directory_map["ETrans_All_Int_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                             Find(ETrans_Daughter_Folders, "01_1p/02_Around_15_Deg_1p/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "01_1p/02_Around_15_Deg_1p/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "01_1p/02_Around_15_Deg_1p/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "01_1p/02_Around_15_Deg_1p/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_15_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                         Find(ETrans_Daughter_Folders, "01_1p/02_Around_15_Deg_1p/04_DIS_only") + "/";
    ETrans_Directory_map["ETrans_All_Ang_stack_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                Find(ETrans_Daughter_Folders, "01_1p/01_Every_theta_e_1p") + "/";
    ETrans_Directory_map["ETrans_All_Int_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                                  Find(ETrans_Daughter_Folders, "01_1p/01_Every_theta_e_1p/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "01_1p/01_Every_theta_e_1p/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "01_1p/01_Every_theta_e_1p/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "01_1p/01_Every_theta_e_1p/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_All_Ang_1p_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "01_1p/01_Every_theta_e_1p/04_DIS_only") + "/";

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
                                                                  Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/00_All_interactions") + "/";
    ETrans_Directory_map["ETrans_QEL_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/01_QEL_only") + "/";
    ETrans_Directory_map["ETrans_MEC_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/02_MEC_only") + "/";
    ETrans_Directory_map["ETrans_RES_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/03_RES_only") + "/";
    ETrans_Directory_map["ETrans_DIS_All_Ang_1n_Directory"] = Plots_Folder + "/" + ETrans_Parent_Directory + "/" +
                                                              Find(ETrans_Daughter_Folders, "03_1n/01_Every_theta_e_1n/04_DIS_only") + "/";
    //</editor-fold>

    //<editor-fold desc="Ecal plots directories">
    for (std::string folders_name: Ecal_Daughter_Folders) {
        MakeDirectory(create_Ecal_Dir, Ecal_Parent_Directory, folders_name, false, Plots_Folder);
    }

    Ecal_Directory_map["Ecal_stack_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                    Find(Ecal_Daughter_Folders, "02_2p") + "/";
    Ecal_Directory_map["Ecal_All_Int_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "02_2p/01_Ecal_rec_by_reaction_2p/00_All_interactions") + "/";
    Ecal_Directory_map["Ecal_QEL_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "02_2p/01_Ecal_rec_by_reaction_2p/01_QEL_only") + "/";
    Ecal_Directory_map["Ecal_MEC_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "02_2p/01_Ecal_rec_by_reaction_2p/02_MEC_only") + "/";
    Ecal_Directory_map["Ecal_RES_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "02_2p/01_Ecal_rec_by_reaction_2p/03_RES_only") + "/";
    Ecal_Directory_map["Ecal_DIS_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "02_2p/01_Ecal_rec_by_reaction_2p/04_DIS_only") + "/";
    Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                                          Find(Ecal_Daughter_Folders, "02_2p/02_Ecal_rec_vs_transverse_variables_2p") + "/";
    Ecal_Directory_map["Chi2_Ecal_test_2p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                        Find(Ecal_Daughter_Folders, "02_2p/03_Chi2_for_Ecal_above_Ebeam_2p") + "/";
    Ecal_Directory_map["Ecal_stack_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                    Find(Ecal_Daughter_Folders, "01_1p") + "/";
    Ecal_Directory_map["Ecal_All_Int_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                      Find(Ecal_Daughter_Folders, "01_1p/01_Ecal_rec_by_reaction_1p/00_All_interactions") + "/";
    Ecal_Directory_map["Ecal_QEL_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "01_1p/01_Ecal_rec_by_reaction_1p/01_QEL_only") + "/";
    Ecal_Directory_map["Ecal_MEC_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "01_1p/01_Ecal_rec_by_reaction_1p/02_MEC_only") + "/";
    Ecal_Directory_map["Ecal_RES_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "01_1p/01_Ecal_rec_by_reaction_1p/03_RES_only") + "/";
    Ecal_Directory_map["Ecal_DIS_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                  Find(Ecal_Daughter_Folders, "01_1p/01_Ecal_rec_by_reaction_1p/04_DIS_only") + "/";
    Ecal_Directory_map["Ecal_rec_vs_transverse_variables_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                                          Find(Ecal_Daughter_Folders, "01_1p/02_Ecal_rec_vs_transverse_variables_1p") + "/";
    Ecal_Directory_map["Chi2_Ecal_test_1p_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                        Find(Ecal_Daughter_Folders, "01_1p/03_Chi2_for_Ecal_above_Ebeam_1p") + "/";

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
    Ecal_Directory_map["Ecal_rec_vs_transverse_variables_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                                          Find(Ecal_Daughter_Folders, "03_1n/02_Ecal_rec_vs_transverse_variables_1n") + "/";
    Ecal_Directory_map["Chi2_Ecal_test_1n_Directory"] = Plots_Folder + "/" + Ecal_Parent_Directory + "/" +
                                                        Find(Ecal_Daughter_Folders, "03_1n/03_Chi2_for_Ecal_above_Ebeam_1n") + "/";
    //</editor-fold>

    //<editor-fold desc="Transverse variables plots directories">
    for (std::string folders_name: TVariables_Daughter_Folders) {
        MakeDirectory(create_TVariables_Dir, TVariables_Parent_Directory, folders_name, false, Plots_Folder);
    }

    TVariables_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                                Find(Ecal_Daughter_Folders, "02_2p") + "/";
    TVariables_Directory_map["dP_T_2p_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                    Find(TVariables_Daughter_Folders, "02_2p/01_dP_T_2p") + "/";
    TVariables_Directory_map["dAlpha_T_2p_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                        Find(TVariables_Daughter_Folders, "02_2p/02_dAlpha_T_2p") + "/";
    TVariables_Directory_map["dPhi_T_2p_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                      Find(TVariables_Daughter_Folders, "02_2p/03_dPhi_T_2p") + "/";
    TVariables_Directory_map["dP_T_vs_dAlpha_T_1p_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                                Find(Ecal_Daughter_Folders, "01_1p") + "/";
    TVariables_Directory_map["dP_T_1p_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                    Find(TVariables_Daughter_Folders, "01_1p/01_dP_T_1p") + "/";
    TVariables_Directory_map["dAlpha_T_1p_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                        Find(TVariables_Daughter_Folders, "01_1p/02_dAlpha_T_1p") + "/";
    TVariables_Directory_map["dPhi_T_1p_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                      Find(TVariables_Daughter_Folders, "01_1p/03_dPhi_T_1p") + "/";
    TVariables_Directory_map["dP_T_vs_dAlpha_T_1n_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                                Find(Ecal_Daughter_Folders, "03_1n") + "/";
    TVariables_Directory_map["dP_T_1n_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                    Find(TVariables_Daughter_Folders, "03_1n/01_dP_T_1n") + "/";
    TVariables_Directory_map["dAlpha_T_1n_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                        Find(TVariables_Daughter_Folders, "03_1n/02_dAlpha_T_1n") + "/";
    TVariables_Directory_map["dPhi_T_1n_Directory"] = Plots_Folder + "/" + TVariables_Parent_Directory + "/" +
                                                      Find(TVariables_Daughter_Folders, "03_1n/03_dPhi_T_1n") + "/";
    //</editor-fold>

    //<editor-fold desc="ToF plots directories">
    for (std::string folders_name: ToF_Daughter_Folders) {
        MakeDirectory(create_ToF_Dir, ToF_Parent_Directory, folders_name, false, Plots_Folder);
    }

    ToF_Directory_map["Neutron_vs_cParticles_hits_1e_cut"] = Plots_Folder + "/" + ToF_Parent_Directory + "/" +
                                                             Find(ToF_Daughter_Folders, "01_1e_cut/01_Neutron_vs_cParticles_hits_FTOF_1e_cut") + "/";
    ToF_Directory_map["Neutron_vs_cParticles_hits_1n1p"] = Plots_Folder + "/" + ToF_Parent_Directory + "/" +
                                                           Find(ToF_Daughter_Folders, "02_1n1p/01_Neutron_vs_cParticles_hits_FTOF_1n1p") + "/";
    //</editor-fold>

}