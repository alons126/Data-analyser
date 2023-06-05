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

void SettingSaveNames(string &SampleName, string &Type, string &Particle, string &SaveDir, string &TestSaveDir, string &PlotsT,
                      string &Numerator_Clone_SaveName, string &Numerator_Clone_test_SaveName, string &Numerator_Clone_test_rebined_SaveName,
                      string &Denominator_Clone_SaveName, string &Denominator_Clone_test_SaveName, string &Denominator_Clone_test_rebined_SaveName,
                      string &sNameFlag, string &PlotSaveName) {
    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    if (findSubstring(PlotsT,"FSRatio")) { // for SFRation plots
        Numerator_Clone_SaveName = SaveDir + sNameFlag + "_" + Particle + "_" + Type + "_nFDpCD_Clone.png";
        Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Particle + "_" + Type + "_nFDpCD_Clone_test.png";
        Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Particle + "_" + Type + "_nFDpCD_Clone_test_rebined.png";

        Denominator_Clone_SaveName = SaveDir + sNameFlag + "_" + Particle + "_" + Type + "_pFDpCD_Clone.png";
        Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Particle + "_" + Type + "_pFDpCD_Clone_test.png";
        Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Particle + "_" + Type + "_pFDpCD_Clone_test_rebined.png";

        PlotSaveName = SaveDir + sNameFlag + "_" + Particle + "_" + Type + "_" + PlotsT + ".png";
    } else { // for efficiency and acceptance correction plots
        Numerator_Clone_SaveName = SaveDir + sNameFlag + "_" + Particle + "_" + Type + "_Rec_Clone.png";
        Numerator_Clone_test_SaveName = TestSaveDir + sNameFlag + "01a_" + Particle + "_" + Type + "_Rec_Clone_test.png";
        Numerator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "01b_" + Particle + "_" + Type + "_Rec_Clone_test_rebined.png";

        Denominator_Clone_SaveName = SaveDir + sNameFlag + "_" + Particle + "_" + Type + "_Truth_Clone.png";
        Denominator_Clone_test_SaveName = TestSaveDir + sNameFlag + "02a_" + Particle + "_" + Type + "_Truth_Clone_test.png";
        Denominator_Clone_test_rebined_SaveName = TestSaveDir + sNameFlag + "02b_" + Particle + "_" + Type + "_Truth_Clone_test_rebined.png";

        PlotSaveName = SaveDir + sNameFlag + "_" + Particle + "_" + Type + "_" + PlotsT + ".png";
    }
}

#endif //SETTINGSAVENAMES_H
