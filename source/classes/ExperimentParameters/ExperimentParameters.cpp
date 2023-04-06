
#include "ExperimentParameters.h"

std::string ExperimentParameters::ConfigureSampleName(std::string AnalyseFilePath, std::string AnalyseFileSample) {
    std::string sName;

    if (AnalyseFilePath == "mnt/d/e4nu/hipo_data_files") { // C12, simulation, 6GeV, Q205, T5, local
        if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10") {
            sName = "C12_simulation_6GeV_T5_first_10";
//            sName = "C12_simulation_Q205_598636MeV_T5_first_10";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100") {
            sName = "C12_simulation_6GeV_T5_first_100";
//            sName = "C12_simulation_Q205_598636MeV_T5_first_100";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5") {
            sName = "C12_simulation_6GeV_T5";
//            sName = "C12_simulation_Q205_598636MeV_T5";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/598636MeV_Q2_0_5_test_5") { // C12, simulation, 6GeV, Q205, T5, ifarm
        if (AnalyseFileSample == "reconhipo") {
            sName = "C12_simulation_6GeV_T5";
//            sName = "C12_simulation_Q205_598636MeV_T5";
        }
    } else if (AnalyseFilePath == "volatile/clas12/users/esteejus/Simulation_sigmaCM") { // C, simulation, 6GeV, ifarm
        if (AnalyseFileSample == "reconhipo") {
            sName = "C_simulation_Justin_6GeV";
//            sName = "C_simulation_Justin_598636MeV";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/Ca48_G18_10a_02_11b_Q205_598636MeV") { // Ca48, simulation, 6GeV, Q205, ifarm
        if (AnalyseFileSample == "reconhipo") {
            sName = "Ca48_simulation_G18_10a_02_11b_6GeV";
//            sName = "Ca48_simulation_G18_10a_02_11b_Q205_598636MeV";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/H1_G18_10a_02_11b_Q205_598636MeV") { // H1, simulation, 6GeV, Q205, ifarm
        if (AnalyseFileSample == "reconhipo") {
            sName = "H1_simulation_G18_10a_02_11b_6GeV";
        }
    } else if (AnalyseFilePath == "volatile/clas12/rg-m/48Ca/dst/recon") { // Ca48, data, 6GeV, ifarm
        if (AnalyseFileSample == "015832") {
            sName = "Ca48_data_6GeV_run_015832";
//            sName = "Ca48_data_598636MeV_run_015832";
        } else if (AnalyseFileSample == "015843") {
            sName = "Ca48_data_6GeV_run_015843";
//            sName = "Ca48_data_598636MeV_run_015843";
        } else if (AnalyseFileSample == "015852") {
            sName = "Ca48_data_6GeV_run_015843";
//            sName = "Ca48_data_598636MeV_run_015843";
        } else if (AnalyseFileSample == "015854") {
            sName = "Ca48_data_6GeV_run_015854";
//            sName = "Ca48_data_598636MeV_run_015854";
        }
    } else if (AnalyseFilePath == "cache/hallb/scratch/rg-m/LH2/8.6.0") { // LH2, data, 6GeV, ifarm
        if (AnalyseFileSample == "015032") {
            sName = "LH2_data_6GeV_run_015032";
//            sName = "LH2_data_598636MeV_run_015032";
        }
    } else {
        sName = "unknown_sample_598636MeV"; // to set beamE = 5.98636 by default
    }

    return sName;
}

double ExperimentParameters::ConfigureBeanEnergy(string sn) {
    double be;

    if ((findSubstring(sn, "598636MeV") == true) || (findSubstring(sn, "598636mev") == true)
        || (findSubstring(sn, "598636") == true) || (findSubstring(sn, "6GeV") == true)) {
        be = 5.98636;
    } else if ((findSubstring(sn, "402962MeV") == true) || (findSubstring(sn, "402962mev") == true)
               || (findSubstring(sn, "402962") == true) || (findSubstring(sn, "4GeV") == true)) {
        be = 4.02962;
    } else if ((findSubstring(sn, "207052MeV") == true) || (findSubstring(sn, "207052mev") == true)
               || (findSubstring(sn, "207052") == true) || (findSubstring(sn, "2GeV") == true)) {
        be = 2.07052;
    }

    return be;
}

double ExperimentParameters::GetBeanEnergy() { return BeanEnergy; }

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