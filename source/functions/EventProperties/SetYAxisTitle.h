//
// Created by alons on 05/06/2023.
//

#ifndef SETYAXISTITLE_H
#define SETYAXISTITLE_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "GetParticleName.h"
#include "GetParticleNameShort.h"
#include "../GeneralFunctions.h"
//#include "../findSubstring.h"

using namespace std;

string SetYAxisTitle(const string& PlotT, const string &Numerator, const string &Denominator) {
    string YAxisTitle;

    string xLabelNum = Numerator.substr(0, Numerator.find_last_of('[') - 1);
    string xLabelDem = Denominator.substr(0, Denominator.find_last_of('[') - 1);

    if (PlotT == "efficiency" || PlotT == "Efficiency" || PlotT == "eff" || PlotT == "Eff") {
        YAxisTitle = "#epsilon_{eff} = " + xLabelNum + "^{rec}" + "/" + xLabelDem;
    } else if (PlotT == "acceptance correction" || PlotT == "Acceptance Correction" || PlotT == "acorrection" || PlotT == "ACorrection" ||
               PlotT == "acorr" || PlotT == "ACorr") {
        YAxisTitle = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabelNum + "/" + xLabelDem + "^{rec}";
    } else if (PlotT == "FSRatio") {
        YAxisTitle = "nFDpCD/pFDpCD";
    }

    return YAxisTitle;
}

#endif //SETYAXISTITLE_H
