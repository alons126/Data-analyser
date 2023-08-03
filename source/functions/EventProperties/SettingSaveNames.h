//
// Created by alons on 05/06/2023.
//

#ifndef SETTINGSAVENAMES_H
#define SETTINGSAVENAMES_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "../GeneralFunctions.h"
//#include "../findSubstring.h"

using namespace std;

void SettingSaveNames(const string &SampleName, const string &Type, const string &Particle, const string &SaveDir, const string &TestSaveDir, const string &PlotsT,
                      string &Numerator_Clone_SaveName, string &Numerator_Clone_test_SaveName, string &Numerator_Clone_test_rebined_SaveName,
                      string &Denominator_Clone_SaveName, string &Denominator_Clone_test_SaveName, string &Denominator_Clone_test_rebined_SaveName,
                      string &sNameFlag, string &PlotSaveName, const string &DRegion = "") {
    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    if (findSubstring(PlotsT, "FSRatio")) { // for SFRation plots
        if (Type == "W" || Type == "Q2" || Type == "E_e" || Type == "omega" || Type == "Ecal" || Type == "deltaP_T_tot" || Type == "deltaP_T_L" ||
            Type == "deltaAlpha_T_tot" || Type == "deltaAlpha_T_L" || Type == "deltaPhi_T_tot" || Type == "deltaPhi_T_L" ||
            Type == "Opening_ang_P_e_P_tot" || Type == "Opening_ang_q_P_tot" || Type == "Opening_ang_q_P_nucFD" || Type == "Opening_ang_q_P_nucCD" ||
            Type == "Opening_ang_q_P_nucL" || Type == "Opening_ang_q_P_nucR" || Type == "Opening_ang_P_nucFD_P_nucCD") {
            Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_" + Type + "_nFDpCD_Clone.png";
            Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Type + "_nFDpCD_Clone_test.png";
            Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Type + "_nFDpCD_Clone_test_rebined.png";

            Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_" + Type + "_pFDpCD_Clone.png";
            Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Type + "_pFDpCD_Clone_test.png";
            Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Type + "_pFDpCD_Clone_test_rebined.png";

            PlotSaveName = SaveDir + sNameFlag + "03_" + Type + "_ratio.png";
        } else {
            if (Particle == "Electron") {
                Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_" + Particle + "_" + Type + "_nFDpCD_Clone.png";
                Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Particle + "_" + Type + "_nFDpCD_Clone_test.png";
                Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Particle + "_" + Type + "_nFDpCD_Clone_test_rebined.png";

                Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_" + Particle + "_" + Type + "_pFDpCD_Clone.png";
                Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Particle + "_" + Type + "_pFDpCD_Clone_test.png";
                Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Particle + "_" + Type + "_pFDpCD_Clone_test_rebined.png";

                PlotSaveName = SaveDir + sNameFlag + "03_" + Particle + "_" + Type + "_ratio.png";
            } else {
                if (DRegion == "FD") {
                    Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_FD_Neutron_" + Type + "_nFDpCD_Clone.png";
                    Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_FD_Neutron_" + Type + "_nFDpCD_Clone_test.png";
                    Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_FD_Neutron_" + Type + "_nFDpCD_Clone_test_rebined.png";

                    Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_FD_Proton_" + Type + "_pFDpCD_Clone.png";
                    Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_FD_Proton_" + Type + "_pFDpCD_Clone_test.png";
                    Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_FD_Proton_" + Type + "_pFDpCD_Clone_test_rebined.png";

                    PlotSaveName = SaveDir + sNameFlag + "03_FD_Nucleon_" + Type + "_ratio.png";
                } else if (DRegion == "CD") {
                    Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_CD_Proton_" + Type + "_nFDpCD_Clone.png";
                    Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_CD_Proton_" + Type + "_nFDpCD_Clone_test.png";
                    Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_CD_Proton_" + Type + "_nFDpCD_Clone_test_rebined.png";

                    Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_CD_Proton_" + Type + "_pFDpCD_Clone.png";
                    Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_CD_Proton_" + Type + "_pFDpCD_Clone_test.png";
                    Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_CD_Proton_" + Type + "_pFDpCD_Clone_test_rebined.png";

                    PlotSaveName = SaveDir + sNameFlag + "03_CD_Nucleon_" + Type + "_ratio.png";
                }
            }
        }
    } else { // for efficiency and acceptance correction plots
        Numerator_Clone_SaveName = SaveDir + sNameFlag + "01_" + Particle + "_" + Type + "_Rec_Clone.png";
        Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Particle + "_" + Type + "_Rec_Clone_test.png";
        Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Particle + "_" + Type + "_Rec_Clone_test_rebined.png";

        Denominator_Clone_SaveName = SaveDir + sNameFlag + "02_" + Particle + "_" + Type + "_Truth_Clone.png";
        Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Particle + "_" + Type + "_Truth_Clone_test.png";
        Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Particle + "_" + Type + "_Truth_Clone_test_rebined.png";

        PlotSaveName = SaveDir + sNameFlag + "03_" + Particle + "_" + Type + "_" + PlotsT + ".png";
    }
}

#endif //SETTINGSAVENAMES_H
