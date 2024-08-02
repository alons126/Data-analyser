//
// Created by alons on 23/12/2023.
//

#ifndef CUSTOMPLOTSDIRECTORY_H
#define CUSTOMPLOTSDIRECTORY_H

using namespace std;

class CustomPlotsDirectory {
private:
    string Plots_path;
    string Plots_log_save_Directory;

    string Stage0_prefix = "_S00_NO_CUTS"; // Stage 0 - no cuts
    string Stage1_prefix = "_S01ACwoChi2"; // Stage 1 - with cuts except PID (chi2) cuts
    string Stage2_prefix = "_S02AC";       // Stage 2 - set nucleon cuts (neutron beta fit & proton double detection cuts)
    string Stage3_prefix = "_S03AC";       // Stage 3 - other runs

    /* Save plots to custom-named folders, to allow multi-sample runs at once. */
    bool Custom_cuts_naming = true;
    string Clas12ana_particles_status = "", Preselection_Cuts_Status = "", Electron_Quality_Cuts_Status = "";

    string Nucleon_Cuts_Status = "", FD_photons_Status = "", PSmearing_Status = "", FiducialCuts_Status = "";
    string KinCuts_Status = "", KinWei_Status = "", Additional_Status = "", Efficiency_Status = "";

    string Added_PreStatuses, Added_Statuses;

    string Global_Run_Status = "v3_Redo4exam";

public:

// Default constructor ---------------------------------------------------------------------------------------------------------------------------------------------

    CustomPlotsDirectory() = default;

// ConfigureStatuses function ------------------------------------------------------------------------------------------------------------------------------------

    void ConfigureStatuses(const bool &apply_cuts, const bool &clas12ana_particles, const bool &only_preselection_cuts, const bool &apply_chi2_cuts_1e_cut,
                           const bool &only_electron_quality_cuts, const bool &apply_nucleon_cuts, const bool &Enable_FD_photons, const bool &apply_nucleon_SmearAndCorr,
                           const bool &apply_kinematical_cuts, const bool &apply_kinematical_weights, const bool &apply_fiducial_cuts, const bool &Generate_AMaps,
                           const bool &plot_and_fit_MomRes, const bool &VaryingDelta, const bool &Calculate_momResS2, const bool &Run_in_momResS2, const bool &nRes_test,
                           const bool &Rec_wTL_ES, const bool &ZoomIn_On_mom_th_plots);

// SetPaths function -----------------------------------------------------------------------------------------------------------------------------------------------

    void SetPaths(const string &WorkingDirectory, const string &SampleName, const string &plots_path, const bool &apply_cuts, const bool &apply_chi2_cuts_1e_cut,
                  const bool &apply_nucleon_cuts);

// Set functions ---------------------------------------------------------------------------------------------------------------------------------------------------

    void SetCustomCutsNaming(const bool &custom_cuts_naming) { Custom_cuts_naming = custom_cuts_naming; }

// Get functions ---------------------------------------------------------------------------------------------------------------------------------------------------

    bool GetCustomCutsNaming() { return Custom_cuts_naming; }

    void GetPlotsPath(string &plots_path) { if (Custom_cuts_naming) { plots_path = Plots_path; }}

    void GetPlotsLogSaveDirectory(string &plots_log_save_Directory) { if (Custom_cuts_naming) { plots_log_save_Directory = Plots_log_save_Directory; }}

    string GetPreselection_Cuts_Status() { return Preselection_Cuts_Status; }

    string GetElectron_Quality_Cuts_Status() { return Electron_Quality_Cuts_Status; }

    string GetNucleon_Cuts_Status() { return Nucleon_Cuts_Status; }

    string GetFD_photons_Status() { return FD_photons_Status; }

    string GetPSmearing_Status() { return PSmearing_Status; }

    string GetFiducialCuts_Status() { return FiducialCuts_Status; }

    string GetKinWei_Status() { return KinWei_Status; }

    string GetAdditional_Status() { return Additional_Status; }

    string GetEfficiency_Status() { return Efficiency_Status; }
};


#endif //CUSTOMPLOTSDIRECTORY_H
