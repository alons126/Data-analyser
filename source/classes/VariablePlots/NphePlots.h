//
// Created by alons on 10/04/2023.
//

#ifndef NPHEPLOTS_H
#define NPHEPLOTS_H

#include "../hPlots/hPlot1D.cpp"

class NphePlots {
private:
    THStack *sNphe_FD;
//    TH1D *hNphe_BC_FD, *hNphe_AC_FD;
//    string hNphe_BC_FD_Dir, hNphe_AC_FD_Dir;

    hPlot1D hNphe_BC_FD, hNphe_AC_FD;

public:
    NphePlots(bool aCuts, std::string fState);
};


#endif //NPHEPLOTS_H
