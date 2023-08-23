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

string SetYAxisTitle(const string &PlotT, const string &FinalState, const string &Numerator, const string &Denominator, const string &Title = "") {
    string YAxisTitle;

    string xLabelNum = Numerator.substr(0, Numerator.find_last_of('[') - 1);
    string xLabelDem = Denominator.substr(0, Denominator.find_last_of('[') - 1);

    if (PlotT == "efficiency" || PlotT == "Efficiency" || PlotT == "eff" || PlotT == "Eff") {
        YAxisTitle = "#epsilon_{eff} = " + xLabelNum + "^{rec}" + "/" + xLabelDem;
    } else if (PlotT == "acceptance correction" || PlotT == "Acceptance Correction" || PlotT == "acorrection" || PlotT == "ACorrection" ||
               PlotT == "acorr" || PlotT == "ACorr") {
        YAxisTitle = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabelNum + "/" + xLabelDem + "^{rec}";
    } else if (PlotT == "FSRatio") {
        if (!findSubstring(Title, "vs") && !findSubstring(Title, "vs.") && !findSubstring(Title, "VS") && !findSubstring(Title, "VS.")) {
            if (FinalState == "1p" || FinalState == "1n") {
                YAxisTitle = "1nFD/1pFD";
            } else if (FinalState == "pFDpCD" || FinalState == "nFDpCD") {
                YAxisTitle = "nFDpCD/pFDpCD";
            }
        } else {
            if (findSubstring(Title, "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|")) {
                YAxisTitle = "|#vec{P}_{rel}| = |#vec{P}_{nucL} - #vec{P}_{nucR}|/2 [GeV/c]";
            } else if (findSubstring(Title, "P_{tot}^{#mu} vs. P_{rel}^{#mu}")) {
                YAxisTitle = "P_{rel}^{#mu} = (P_{pL}^{#mu} - P_{pR}^{#mu})/2 [GeV/c]";
            } else if (findSubstring(Title, "P_{pL} vs. P_{pR}") || findSubstring(Title, "P_{nL} vs. P_{nR}")) {
                YAxisTitle = "P_{nucR} [GeV/c]";
            } else if (findSubstring(Title, "P_{pFD} vs. P_{pCD}") || findSubstring(Title, "P_{nFD} vs. P_{pCD}")) {
                YAxisTitle = "P_{nucCD} [GeV/c]";
            } else if (findSubstring(Title, "#theta_{e} vs. P_{e}")) {
                YAxisTitle = "#theta_{e} [Deg]";
            } else if (findSubstring(Title, "#theta_{e} vs. W")) {
                YAxisTitle = "#theta_{e} [Deg]";
            } else if (findSubstring(Title, "#phi_{e} vs. P_{e}")) {
                YAxisTitle = "#phi_{e} [Deg]";
            } else if (findSubstring(Title, "#phi_{e} vs. W")) {
                YAxisTitle = "#phi_{e} [Deg]";
            } else if (findSubstring(Title, "#theta_{e} vs. #phi_{e}")) {
                YAxisTitle = "#theta_{e} [Deg]";
            } else if (findSubstring(Title, "#theta_{pFD} vs. #phi_{pFD}") || findSubstring(Title, "#theta_{nFD} vs. #phi_{nFD}")) {
                YAxisTitle = "#theta_{nucFD} [Deg]";
            } else if (findSubstring(Title, "#theta_{pCD} vs. #phi_{pCD}")) {
                YAxisTitle = "#theta_{nucCD} [Deg]";
            } else if (findSubstring(Title, "#theta_{pFD} vs. W") || findSubstring(Title, "#theta_{nFD} vs. W")) {
                YAxisTitle = "#theta_{nucFD} [Deg]";
            } else if (findSubstring(Title, "#theta_{pFD} vs. P_{pFD}") || findSubstring(Title, "#theta_{nFD} vs. P_{nFD}")) {
                YAxisTitle = "#theta_{nucFD} [Deg]";
            } else if (findSubstring(Title, "#theta_{pCD} vs. W") || findSubstring(Title, "#theta_{nCD} vs. W")) {
                YAxisTitle = "#theta_{nucCD} [Deg]";
            } else if (findSubstring(Title, "#theta_{pCD} vs. P_{pCD}") || findSubstring(Title, "#theta_{nCD} vs. P_{nCD}")) {
                YAxisTitle = "#theta_{nucCD} [Deg]";
            } else if (findSubstring(Title, "#phi_{pFD} vs. W") || findSubstring(Title, "#phi_{nFD} vs. W")) {
                YAxisTitle = "#phi_{nucFD} [Deg]";
            } else if (findSubstring(Title, "#phi_{pFD} vs. P_{pFD}") || findSubstring(Title, "#phi_{nFD} vs. P_{nFD}")) {
                YAxisTitle = "#phi_{nucFD} [Deg]";
            } else if (findSubstring(Title, "#phi_{pCD} vs. W") || findSubstring(Title, "#phi_{nCD} vs. W")) {
                YAxisTitle = "#phi_{nucCD} [Deg]";
            } else if (findSubstring(Title, "#phi_{pCD} vs. P_{pCD}") || findSubstring(Title, "#phi_{nCD} vs. P_{nCD}")) {
                YAxisTitle = "#phi_{nucFD} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W")) {
                YAxisTitle = "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{q},#vec{P}_{tot}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{tot}} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{q},#vec{P}_{pL}} vs. W") ||
            findSubstring(Title, "#theta_{#vec{q},#vec{P}_{nL}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucL}} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{q},#vec{P}_{pR}} vs. W") ||
            findSubstring(Title, "#theta_{#vec{q},#vec{P}_{nR}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucR}} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{q},#vec{P}_{pFD}} vs. W") ||
            findSubstring(Title, "#theta_{#vec{q},#vec{P}_{nFD}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucFD}} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{q},#vec{P}_{pCD}} vs. W") ||
            findSubstring(Title, "#theta_{#vec{q},#vec{P}_{nCD}} vs. W")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucCD}} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}|") ||
                       findSubstring(Title, "#theta_{#vec{q},#vec{P}_{nL}} vs. r_{nL}=|#vec{P}_{nL}|/|#vec{q}|")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucL}} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}}") ||
                       findSubstring(Title, "#theta_{#vec{q},#vec{P}_{nL}} vs. #theta_{#vec{q},#vec{P}_{nR}}")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucR}} [Deg]";
            } else if (findSubstring(Title, "#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}") ||
                       findSubstring(Title, "#theta_{#vec{q},#vec{P}_{nFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}")) {
                YAxisTitle = "#theta_{#vec{q},#vec{P}_{nucCD}} [Deg]";
            } else if (findSubstring(Title, "#theta_{pFD,pCD} vs. W") || findSubstring(Title, "#theta_{nFD,pCD} vs. W")) {
                YAxisTitle = "#theta_{nucFD,nucCD} [Deg]";
            } else if (findSubstring(Title, "Q^{2} vs. W")) {
                YAxisTitle = "Q^{2} [GeV^{2}/c^{2}]";
            } else if (findSubstring(Title, "E_{e} vs. #theta_{e}")) {
                YAxisTitle = "E_{e} [GeV]";
            } else if (findSubstring(Title, "E_{cal} vs. #delta#alpha_{T,L}")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (findSubstring(Title, "E_{cal} vs. #delta#alpha_{T,tot}")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (findSubstring(Title, "E_{cal} vs. #deltaP_{T,L}")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (findSubstring(Title, "E_{cal} vs. #deltaP_{T,tot}")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (findSubstring(Title, "E_{cal} vs. W")) {
                YAxisTitle = "E_{cal} [GeV]";
            } else if (findSubstring(Title, "#deltaP_{T,L} vs. #delta#alpha_{T,L}")) {
                YAxisTitle = "#deltaP_{T,L} [GeV/c]";
            } else if (findSubstring(Title, "#deltaP_{T,tot} vs. #delta#alpha_{T,tot}")) {
                YAxisTitle = "#deltaP_{T,tot} [GeV/c]";
            } else if (findSubstring(Title, "#deltaP_{T,L} vs. W")) {
                YAxisTitle = "#deltaP_{T,L} [GeV/c]";
            } else if (findSubstring(Title, "#deltaP_{T,tot} vs. W")) {
                YAxisTitle = "#deltaP_{T,tot} [GeV/c]";
            } else if (findSubstring(Title, "#delta#alpha_{T,L} vs. W")) {
                YAxisTitle = "#delta#alpha_{T,L} [Deg]";
            } else if (findSubstring(Title, "#delta#alpha_{T,tot} vs. W")) {
                YAxisTitle = "#delta#alpha_{T,tot} [Deg]";
            }
        }
    }

    return YAxisTitle;
}

#endif //SETYAXISTITLE_H
