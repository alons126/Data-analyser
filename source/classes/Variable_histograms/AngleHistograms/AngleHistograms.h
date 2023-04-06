//
// Created by alons on 06/04/2023.
//

#ifndef ANGLEHISTOGRAMS_H
#define ANGLEHISTOGRAMS_H

#include "../../hPlots/hPlot1D.cpp"

#endif //CA48_G18_10A_02_11B_207052MEV_T1_60206802_FARM140233_OUT_ANGLEHISTOGRAMS_H

class AngleHistograms : protected hPlot1D {
protected:
    hPlot1D hTheta_e_All_Int_2p_FD = hPlot1D("All Int.", "", "FD", "#theta_{e} of Outgoing Electron", 0, 50);
    hPlot1D hPhi_e_All_Int_2p_FD = hPlot1D("All Int.", "", "FD", "#phi_{e} of Outgoing Electron", -200, 200);
public:
    AngleHistograms();
};