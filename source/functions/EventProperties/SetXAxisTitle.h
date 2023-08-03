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

    if (findSubstring(RecTitle, "momentum")) {
        XAxisTitle = "Momentum [GeV/c]";
    } else if (findSubstring(RecTitle, "W")) {
        XAxisTitle = "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV]";
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
        XAxisTitle = "#theta_{#vec{q},#vec{P}_{nuc,L}} [Deg]";
    } else if (findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{pR}}") || findSubstring(RecTitle, "#theta_{#vec{q},#vec{P}_{nR}}")) {
        XAxisTitle = "#theta_{#vec{q},#vec{P}_{nuc,R}} [Deg]";
    } else if (findSubstring(RecTitle, "#theta_{pFD,pCD}") || findSubstring(RecTitle, "#theta_{nFD,pCD}")) {
        XAxisTitle = "#theta_{nucFD,nucCD} [Deg]";
    }

    return XAxisTitle;
}

#endif //SETXAXISTITLE_H
