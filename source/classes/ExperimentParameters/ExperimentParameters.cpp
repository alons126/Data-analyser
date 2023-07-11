
#include "ExperimentParameters.h"

// ConfigureSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

std::string ExperimentParameters::ConfigureSampleName(std::string AnalyseFilePath, std::string AnalyseFileSample) {
    std::string sName;

    if (AnalyseFilePath == "mnt/d/e4nu/hipo_data_files") { // C12, simulation, 6GeV, Q205, T5, local
        if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10") {
            BeamAt6GeV = true;
            SimulationSample = true;
            sName = "C12_simulation_6GeV_T5_first_10";
//            sName = "C12_simulation_Q205_598636MeV_T5_first_10";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100") {
            BeamAt6GeV = true;
            SimulationSample = true;
            sName = "C12_simulation_6GeV_T5_first_100";
//            sName = "C12_simulation_Q205_598636MeV_T5_first_100";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250") {
            BeamAt6GeV = true;
            SimulationSample = true;
            sName = "C12_simulation_6GeV_T5_first_250";
//            sName = "C12_simulation_Q205_598636MeV_T5_first_250";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5") {
            BeamAt6GeV = true;
            SimulationSample = true;
            sName = "C12_simulation_6GeV_T5";
//            sName = "C12_simulation_Q205_598636MeV_T5";
        } else if (AnalyseFileSample == "C12_simulation_G18_2GeV") {
            BeamAt2GeV = true;
            SimulationSample = true;
            sName = "C12_simulation_G18_2GeV";
//            sName = "C12_simulation_G18_2GeV_local";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/598636MeV_Q2_0_5_test_5") { // C12, simulation, 6GeV, Q205, T5, ifarm
        if (AnalyseFileSample == "reconhipo") {
            BeamAt6GeV = true;
            SimulationSample = true;
            sName = "C12_simulation_6GeV_T5";
//            sName = "C12_simulation_Q205_598636MeV_T5";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/C12_G18_10a_02_11b_207052MeV") { // C12, simulation, 2GeV, ifarm
        if (AnalyseFileSample == "reconhipo") {
            SimulationSample = true;
            BeamAt2GeV = true;
            sName = "C12_simulation_G18_2GeV";
//            sName = "C12_G18_10a_02_11b_207052MeV";
        }
    } else if (AnalyseFilePath == "volatile/clas12/users/esteejus/Simulation_sigmaCM") { // C, simulation, 6GeV, ifarm
        if (AnalyseFileSample == "reconhipo") {
            BeamAt6GeV = true;
            SimulationSample = true;
            sName = "C_simulation_Justin_6GeV";
//            sName = "C_simulation_Justin_598636MeV";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/Ca48_G18_10a_02_11b_Q205_598636MeV") { // Ca48, simulation, 6GeV, Q205, ifarm
        if (AnalyseFileSample == "reconhipo") {
            BeamAt6GeV = true;
            SimulationSample = true;
            sName = "Ca48_simulation_G18_10a_02_11b_6GeV";
//            sName = "Ca48_simulation_G18_10a_02_11b_Q205_598636MeV";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/H1_G18_10a_02_11b_Q205_598636MeV") { // H1, simulation, 6GeV, Q205, ifarm
        if (AnalyseFileSample == "reconhipo") {
            BeamAt6GeV = true;
            SimulationSample = true;
            sName = "H1_simulation_G18_10a_02_11b_6GeV";
        }
    } else if (AnalyseFilePath == "volatile/clas12/rg-m/48Ca/dst/recon") { // Ca48, data, 6GeV, ifarm
        if (AnalyseFileSample == "015832") {
            BeamAt6GeV = true;
            DataSample = true;
            sName = "Ca48_data_6GeV_run_015832";
//            sName = "Ca48_data_598636MeV_run_015832";
        } else if (AnalyseFileSample == "015843") {
            BeamAt6GeV = true;
            DataSample = true;
            sName = "Ca48_data_6GeV_run_015843";
//            sName = "Ca48_data_598636MeV_run_015843";
        } else if (AnalyseFileSample == "015852") {
            BeamAt6GeV = true;
            DataSample = true;
            sName = "Ca48_data_6GeV_run_015843";
//            sName = "Ca48_data_598636MeV_run_015843";
        } else if (AnalyseFileSample == "015854") {
            BeamAt6GeV = true;
            DataSample = true;
            sName = "Ca48_data_6GeV_run_015854";
//            sName = "Ca48_data_598636MeV_run_015854";
        }
    } else if (AnalyseFilePath == "cache/hallb/scratch/rg-m/LH2/8.6.0") { // LH2, data, 6GeV, ifarm
        if (AnalyseFileSample == "015032") {
            BeamAt6GeV = true;
            DataSample = true;
            sName = "LH2_data_6GeV_run_015032";
//            sName = "LH2_data_598636MeV_run_015032";
        }
    } else {
        sName = "unknown_sample_598636MeV"; // to set beamE = 5.98636 by default
    }

    if (!BeamAt2GeV && !BeamAt4GeV && !BeamAt6GeV) { cout << "\n\nBeam energy unknown!\n"; }

    return sName;
}

// ConfigureBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

double ExperimentParameters::ConfigureBeanEnergy(string sn) {
    double be;

    if ((findSubstring(sn, "598636MeV") == true) || (findSubstring(sn, "598636mev") == true)
        || (findSubstring(sn, "598636") == true) || (findSubstring(sn, "6GeV") == true)) {
//        BeamAt6GeV = true;
        be = 5.98636;
    } else if ((findSubstring(sn, "402962MeV") == true) || (findSubstring(sn, "402962mev") == true)
               || (findSubstring(sn, "402962") == true) || (findSubstring(sn, "4GeV") == true)) {
//        BeamAt4GeV = true;
        be = 4.02962;
    } else if ((findSubstring(sn, "207052MeV") == true) || (findSubstring(sn, "207052mev") == true)
               || (findSubstring(sn, "207052") == true) || (findSubstring(sn, "2GeV") == true)) {
//        BeamAt2GeV = true;
        be = 2.07052;
    }

    return be;
}

// GetBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

double ExperimentParameters::GetBeanEnergy() { return BeanEnergy; }

// ExperimentParameters function -----------------------------------------------------------------------------------------------------------------------------------------

ExperimentParameters::ExperimentParameters(string AnalyseFilePath, string AnalyseFileSample) {
    SampleName = ConfigureSampleName(AnalyseFilePath, AnalyseFileSample);
    BeanEnergy = ConfigureBeanEnergy(SampleName);

    if ((SampleName.find("c12") <= SampleName[SampleName.size() - 1]) || (SampleName.find("C12") <= SampleName[SampleName.size() - 1]) ||
        (SampleName.find("12c") <= SampleName[SampleName.size() - 1]) || (SampleName.find("12C") <= SampleName[SampleName.size() - 1]) ||
        (SampleName.find("_c_") <= SampleName[SampleName.size() - 1]) || (SampleName.find("_C_") <= SampleName[SampleName.size() - 1])) {
        TargetElement = "C12";
        TargetElementPDG = 1000060120;
    } else if (SampleName.find("Ca48") <= SampleName[SampleName.size() - 1]) {
        TargetElement = "Ca48";
        TargetElementPDG = 1000200480;
    } else {
        TargetElement = "UNKOWN";
        TargetElementPDG = -9999;
    }
}