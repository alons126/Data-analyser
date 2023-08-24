
#include "ExperimentParameters.h"

// ConfigureSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

std::string ExperimentParameters::ConfigureSampleName(const std::string &AnalyseFilePath, const std::string &AnalyseFileSample) {
    std::string sName = "unknown_sample_598636MeV"; // to set beamE = 5.98636 by default;

    if (AnalyseFilePath == "mnt/d/e4nu/hipo_data_files") { // Local samples
        LocalSample = true;

        //<editor-fold desc="Local samples">
        if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10") {
            BeamAt6GeV = SimulationSample = true;
            sName = "C12_simulation_6GeV_T5_first_10";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100") {
            BeamAt6GeV = SimulationSample = true;
            sName = "C12_simulation_6GeV_T5_first_100";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250") {
            BeamAt6GeV = SimulationSample = true;
            sName = "C12_simulation_6GeV_T5_first_250";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5") {
            BeamAt6GeV = SimulationSample = true;
            sName = "C12_simulation_6GeV_T5";
        } else if (AnalyseFileSample == "C12_simulation_G18_2GeV") {
            BeamAt2GeV = SimulationSample = true;
            sName = "C12_simulation_G18_2GeV";
        } else if (AnalyseFileSample == "C12_G18_02a_00_000_Q204_598636MeV") {
            BeamAt6GeV = SimulationSample = true;
            sName = "C12_simulation_G18_Q204_6GeV";
        }
        //</editor-fold>

    } else { // ifarm samples

        //<editor-fold desc="ifarm samples">
        if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/C12_G18_10a_02_11b_207052MeV") { // C12, simulation, 2GeV, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt2GeV = SimulationSample = true;
                sName = "C12_simulation_G18_2GeV";
            }
        } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/Ar40_G18_10a_02_11b_207052MeV") { // Ar40, simulation, 2GeV, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt2GeV = SimulationSample = true;
                sName = "Ar40_simulation_G18_2GeV";
            }
        } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/Ca48_G18_10a_02_11b_207052MeV") { // Ca48, simulation, 2GeV, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt2GeV = SimulationSample = true;
                sName = "Ca48_simulation_G18_2GeV";
            }
        } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/598636MeV_Q2_0_5_test_5") { // C12, simulation, 6GeV, Q205, T5, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt6GeV = SimulationSample = true;
                sName = "C12_simulation_6GeV_T5";
            }
        } else if (AnalyseFilePath == "volatile/clas12/asportes/simulationFiles/C12_G18_02a_00_000_Q208_598636MeV") { // C12, simulation, 6GeV, Q208, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt6GeV = SimulationSample = true;
                sName = "C12_simulation_G18_Q208_6GeV";
            }
        } else if (AnalyseFilePath == "volatile/clas12/asportes/simulationFiles/C12_G18_02a_00_000_Q204_598636MeV") { // C12, simulation, 6GeV, Q204, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt6GeV = SimulationSample = true;
                sName = "C12_simulation_G18_Q204_6GeV";
            }
        } else if (AnalyseFilePath ==
                   "lustre19/expphy/volatile/clas12/asportes/simulationFiles/Ca48_G18_10a_02_11b_Q205_598636MeV") { // Ca48, simulation, 6GeV, Q205, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt6GeV = SimulationSample = true;
                sName = "Ca48_simulation_G18_10a_02_11b_6GeV";
            }
        } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/H1_G18_10a_02_11b_Q205_598636MeV") { // H1, simulation, 6GeV, Q205, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt6GeV = SimulationSample = true;
                sName = "H1_simulation_G18_10a_02_11b_6GeV";
            }
        } else if (AnalyseFilePath == "volatile/clas12/users/esteejus/Simulation_sigmaCM") { // C, simulation, 6GeV, ifarm
            if (AnalyseFileSample == "reconhipo") {
                BeamAt6GeV = SimulationSample = true;
                sName = "C_simulation_Justin_6GeV";
            }
        } else if (AnalyseFilePath == "volatile/clas12/rg-m/48Ca/dst/recon") { // Ca48, data, 6GeV, ifarm
            if (AnalyseFileSample == "015832") {
                BeamAt6GeV = DataSample = true;
                sName = "Ca48_data_6GeV_run_015832";
            } else if (AnalyseFileSample == "015843") {
                BeamAt6GeV = DataSample = true;
                sName = "Ca48_data_6GeV_run_015843";
            } else if (AnalyseFileSample == "015852") {
                BeamAt6GeV = DataSample = true;
                sName = "Ca48_data_6GeV_run_015852";
            } else if (AnalyseFileSample == "015854") {
                BeamAt6GeV = DataSample = true;
                sName = "Ca48_data_6GeV_run_015854";
            }
        } else if (AnalyseFilePath == "cache/hallb/scratch/rg-m/LH2/8.6.0") { // LH2, data, 6GeV, ifarm
            if (AnalyseFileSample == "015032") {
                BeamAt6GeV = DataSample = true;
                sName = "LH2_data_6GeV_run_015032";
            }
        } else if (AnalyseFilePath == "cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon") { // Cx4, data, 6GeV, ifarm
            if (AnalyseFileSample == "015186") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015186";
            } else if (AnalyseFileSample == "015187") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015187";
            } else if (AnalyseFileSample == "015188") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015188";
            } else if (AnalyseFileSample == "015189") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015189";
            } else if (AnalyseFileSample == "015190") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015190";
            } else if (AnalyseFileSample == "015191") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015191";
            } else if (AnalyseFileSample == "015192") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015192";
            } else if (AnalyseFileSample == "015193") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015193";
            } else if (AnalyseFileSample == "015194") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015194";
            } else if (AnalyseFileSample == "015195") {
                BeamAt6GeV = DataSample = true;
                sName = "C12x4_data_6GeV_run_015195";
            }
        }
        //</editor-fold>

    }

    //<editor-fold desc="Safty checks">
    if (!BeamAt2GeV && !BeamAt4GeV && !BeamAt6GeV) { cout << "\n\nConfigureSampleName: beam energy unknown! Exiting...\n", exit(0); }

    if (BeamAt2GeV && BeamAt4GeV && BeamAt6GeV) { cout << "\n\nConfigureSampleName: beam energy configuration error! Exiting...\n", exit(0); }

    if (SimulationSample && DataSample) { cout << "\n\nConfigureSampleName: sample type unknown! Exiting...\n", exit(0); }

    if (!SimulationSample && !DataSample) { cout << "\n\nConfigureSampleName: sample type configuration error! Exiting...\n", exit(0); }
    //</editor-fold>

    ConfigureVaringSampleName(sName);

    return sName;
}

// ConfigureVaringSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

void ExperimentParameters::ConfigureVaringSampleName(const string &sn) {
    if (findSubstring(sn, "sim")) { // Sample is simulation
        VaringSampleName = SampleName;
    } else if (findSubstring(sn, "data")) { // Sample is data
        if (findSubstring(sn, "C12") && BeamAt6GeV) {
            VaringSampleName = "C12_simulation_G18_Q204_6GeV";
        } else {
            cout << "\n\n\nExperimentParameters::GetVaringSampleName: no corresponding simulation sample! Exiting...", exit(0);
        }
    } else {
        cout << "\n\n\nExperimentParameters::ConfigureVaringSampleName: sample can't be configured! Exiting...", exit(0);
    }
}

// ConfigureBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

double ExperimentParameters::ConfigureBeanEnergy(const string &sn) {
    double be;

    if (findSubstring(sn, "598636MeV") || findSubstring(sn, "598636mev") || findSubstring(sn, "598636")
        || findSubstring(sn, "6GeV") || findSubstring(sn, "6Gev") || findSubstring(sn, "6gev")) {
        be = 5.98636;
    } else if (findSubstring(sn, "402962MeV") || findSubstring(sn, "402962mev") || findSubstring(sn, "402962")
               || findSubstring(sn, "4GeV") || findSubstring(sn, "4Gev") || findSubstring(sn, "4gev")) {
        be = 4.02962;
    } else if (findSubstring(sn, "207052MeV") || findSubstring(sn, "207052mev") || findSubstring(sn, "207052")
               || findSubstring(sn, "2GeV") || findSubstring(sn, "2Gev") || findSubstring(sn, "2gev")) {
        be = 2.07052;
    }

    return be;
}

// GetBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

double ExperimentParameters::GetBeanEnergy() { return BeanEnergy; }

// ExperimentParameters function -----------------------------------------------------------------------------------------------------------------------------------------

ExperimentParameters::ExperimentParameters(const string &AnalyseFilePath, const string &AnalyseFileSample) {
    SampleName = ConfigureSampleName(AnalyseFilePath, AnalyseFileSample);
    BeanEnergy = ConfigureBeanEnergy(SampleName);

    if (SampleName.find("H1") <= SampleName[SampleName.size() - 1]) {
        TargetElement = "H1";
        TargetElementPDG = 1000010010;
        TotalBaryonNumber_A = 1;
        TotalChargeNumber_Z = 1;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if ((SampleName.find("c12") <= SampleName[SampleName.size() - 1]) || (SampleName.find("C12") <= SampleName[SampleName.size() - 1]) ||
               (SampleName.find("12c") <= SampleName[SampleName.size() - 1]) || (SampleName.find("12C") <= SampleName[SampleName.size() - 1]) ||
               (SampleName.find("_c_") <= SampleName[SampleName.size() - 1]) || (SampleName.find("_C_") <= SampleName[SampleName.size() - 1])) {
        TargetElement = "C12";
        TargetElementPDG = 1000060120;
        TotalBaryonNumber_A = 12;
        TotalChargeNumber_Z = 6;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if (SampleName.find("Ca48") <= SampleName[SampleName.size() - 1]) {
        TargetElement = "Ca48";
        TargetElementPDG = 1000200480;
        TotalBaryonNumber_A = 48;
        TotalChargeNumber_Z = 20;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if (SampleName.find("Ar40") <= SampleName[SampleName.size() - 1]) {
        TargetElement = "Ar40";
        TargetElementPDG = 1000180400;
        TotalBaryonNumber_A = 40;
        TotalChargeNumber_Z = 18;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else {
        TargetElement = "UNKOWN";
        TargetElementPDG = -9999;
        TotalBaryonNumber_A = TotalChargeNumber_Z = StrangeQuarksNumber_L = IsomerNumber_I = 0;
    }
}