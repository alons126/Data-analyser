//
// Created by alons on 05/06/2023.
//

#ifndef SETXAXISTITLE_H
#define SETXAXISTITLE_H

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

string SetXAxisTitle(const string &RecTitle) {
    string XAxisTitle;

    string Particle = GetParticleName(RecTitle);
    string ParticleShort = GetParticleNameShort(RecTitle);

    if (!findSubstring(RecTitle, "vs") && !findSubstring(RecTitle, "vs.") && !findSubstring(RecTitle, "VS") && !findSubstring(RecTitle, "VS.")) {
        if (findSubstring(RecTitle, "momentum")) {
            XAxisTitle = "Momentum [GeV/c]";
        } else if (findSubstring(RecTitle, "W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "Q^{2}")) {
            XAxisTitle = "Q^{2} [GeV^{2}/c^{2}]";
        } else if (findSubstring(RecTitle, "E_{e}")) {
            XAxisTitle = "E_{e} [GeV]";
        } else if (findSubstring(RecTitle, "#omega")) {
            XAxisTitle = "#omega = E_{beam}-E_{e} [GeV]";
        } else if (findSubstring(RecTitle, "E_{cal}")) {
            XAxisTitle = "E_{cal} = E_{e} + T_{nuc,FD} + T_{nuc,CD} [GeV]";
        } else if (findSubstring(RecTitle, "#deltaP_{T,tot}")) {
            XAxisTitle = "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD}| [GeV/c]";
        } else if (findSubstring(RecTitle, "#deltaP_{T,L}")) {
            XAxisTitle = "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}| [GeV/c]";
        } else if (findSubstring(RecTitle, "#delta#alpha_{T,tot}")) {
            XAxisTitle = "#delta#alpha_{T,tot} [Deg]";
        } else if (findSubstring(RecTitle, "#delta#alpha_{T,L}")) {
            XAxisTitle = "#delta#alpha_{T,L} [Deg]";
        } else if (findSubstring(RecTitle, "#delta#phi_{T,tot}")) {
            XAxisTitle = "#delta#phi_{T,tot} [Deg]";
        } else if (findSubstring(RecTitle, "#delta#phi_{T,L}")) {
            XAxisTitle = "#delta#phi_{T,L} [Deg]";
        } else if (findSubstring(RecTitle, "#theta") && !findSubstring(RecTitle, "#theta_{#vec{") &&
                   !(findSubstring(RecTitle, "#theta_{pFD,pCD}") || findSubstring(RecTitle, "#theta_{nFD,pCD}"))) {
            XAxisTitle = "#theta [Deg]";
        } else if (findSubstring(RecTitle, "#phi")) {
            XAxisTitle = "#phi [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{P}_{e},#vec{P}_{tot}}")) {
            XAxisTitle = "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{tot}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{tot}} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pFD}}") || findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nFD}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nuc,FD}} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pCD}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{p,CD}} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pL}}") || findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nL}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nucL}} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pR}}") || findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nR}}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nucR}} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{pFD,pCD}") || findSubstring(RecTitle, "#theta_{nFD,pCD}")) {
            XAxisTitle = "#theta_{nucFD,nucCD} [Deg]";
        }
    } else {
        if (findSubstring(RecTitle, "#vec{P}_{tot} vs. #vec{P}_{rel}")) {
            XAxisTitle = "|#vec{P}_{tot}| = |#vec{P}_{nucL} + #vec{P}_{nucR}| [GeV/c]";
        } else if (findSubstring(RecTitle, "P_{tot}^{#mu} vs. P_{rel}^{#mu}")) {
            XAxisTitle = "P_{tot}^{#mu} = P_{pL}^{#mu} + P_{pR}^{#mu} [GeV/c]";
        } else if (findSubstring(RecTitle, "P_{pL} vs. P_{pR}") || findSubstring(RecTitle, "P_{nL} vs. P_{nR}")) {
            XAxisTitle = "P_{nucL} [GeV/c]";
        } else if (findSubstring(RecTitle, "P_{pFD} vs. P_{pCD}") || findSubstring(RecTitle, "P_{nFD} vs. P_{ncCD}")) {
            XAxisTitle = "P_{nucFD} [GeV/c]";
        } else if (findSubstring(RecTitle, "#theta_{e} vs. P_{e}")) {
            XAxisTitle = "P_{e} [GeV/c]";
        } else if (findSubstring(RecTitle, "#theta_{e} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#phi_{e} vs. P_{e}")) {
            XAxisTitle = "P_{e} [GeV/c]";
        } else if (findSubstring(RecTitle, "#phi_{e} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{e} vs. #phi_{e}")) {
            XAxisTitle = "#phi_{e} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{pFD} vs. #phi_{pFD}") || findSubstring(RecTitle, "#theta_{nFD} vs. #phi_{nFD}")) {
            XAxisTitle = "#phi_{nucFD} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{pCD} vs. #phi_{pCD}")) {
            XAxisTitle = "#phi_{pCD} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{pCD} vs. #phi_{pCD}")) {
            XAxisTitle = "#phi_{pCD} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{pFD} vs. W") ||
                   findSubstring(RecTitle, "#theta_{nFD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{pFD} vs. P_{pFD}") || findSubstring(RecTitle, "#theta_{nFD} vs. P_{nFD}")) {
            XAxisTitle = "P_{nucFD} [GeV/c]";
        } else if (findSubstring(RecTitle, "#theta_{pCD} vs. W") || findSubstring(RecTitle, "#theta_{nCD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{pCD} vs. P_{pCD}") || findSubstring(RecTitle, "#theta_{nCD} vs. P_{nCD}")) {
            XAxisTitle = "P_{pCD} [GeV/c]";
        } else if (findSubstring(RecTitle, "#phi_{pFD} vs. W") || findSubstring(RecTitle, "#phi_{nFD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#phi_{pFD} vs. P_{pFD}") || findSubstring(RecTitle, "#phi_{nFD} vs. P_{nFD}")) {
            XAxisTitle = "P_{nucFD} [GeV/c]";
        } else if (findSubstring(RecTitle, "#phi_{pCD} vs. W") || findSubstring(RecTitle, "#phi_{nCD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#phi_{pCD} vs. P_{pCD}") || findSubstring(RecTitle, "#phi_{nCD} vs. P_{nCD}")) {
            XAxisTitle = "P_{pCD} [GeV/c]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{tot}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pL}} vs. W") || findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nL}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pR}} vs. W") || findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nR}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pFD}} vs. W") || findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nFD}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pCD}} vs. W") || findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nCD}} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}|") ||
                   findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nL}} vs. r_{nL}=|#vec{P}_{nL}|/|#vec{q}|")) {
            XAxisTitle = "r_{nucL}";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}") ||
                   findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nL}} vs. #theta_{#vec{q},#vec{P}_{nR}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nucL}}";
        } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}") ||
                   findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}")) {
            XAxisTitle = "#theta_{#vec{q},#vec{P}_{nucFD}} [Deg]";
        } else if (findSubstring(RecTitle, "#theta_{pFD,pCD} vs. W") || findSubstring(RecTitle, "#theta_{nFD,pCD} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "E_{e} vs. #theta_{e}")) {
            XAxisTitle = "#theta_{e} [Deg]";
        } else if (findSubstring(RecTitle, "E_{cal} vs. #delta#alpha_{T,L}")) {
            XAxisTitle = "#delta#alpha_{T,L} [Deg]";
        } else if (findSubstring(RecTitle, "E_{cal} vs. #delta#alpha_{T,tot}")) {
            XAxisTitle = "#delta#alpha_{T,tot} [Deg]";
        } else if (findSubstring(RecTitle, "E_{cal} vs. #deltaP_{T,L}")) {
            XAxisTitle = "#deltaP_{T,L} [GeV/c]";
        } else if (findSubstring(RecTitle, "E_{cal} vs. #deltaP_{T,tot}")) {
            XAxisTitle = "#deltaP_{T,tot} [GeV/c]";
        } else if (findSubstring(RecTitle, "E_{cal} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#deltaP_{T,L} vs. #delta#alpha_{T,L}")) {
            XAxisTitle = "#delta#alpha_{T,L} [Deg]";
        } else if (findSubstring(RecTitle, "#deltaP_{T,tot} vs. #delta#alpha_{T,tot}")) {
            XAxisTitle = "#delta#alpha_{T,tot} [Deg]";
        } else if (findSubstring(RecTitle, "#deltaP_{T,L} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#deltaP_{T,tot} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#delta#alpha_{T,L} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        } else if (findSubstring(RecTitle, "#delta#alpha_{T,tot} vs. W")) {
            XAxisTitle = "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}}  [GeV]";
        }
    }

    return XAxisTitle;
}

#endif //SETXAXISTITLE_H
