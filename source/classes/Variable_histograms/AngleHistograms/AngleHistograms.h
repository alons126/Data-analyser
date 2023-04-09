//
// Created by alons on 07/04/2023.
//

#ifndef ANGLEHISTOGRAMS_H
#define ANGLEHISTOGRAMS_H

#include "../../hPlots/hPlot1D.cpp"

class AngleHistograms {
protected:
    hPlot1D hTheta_e_All_Int_2p_FD = hPlot1D("All Int.", "", "FD", "#theta_{e} of Outgoing Electron", 0, 50);
    hPlot1D hPhi_e_All_Int_2p_FD = hPlot1D("All Int.", "", "FD", "#phi_{e} of Outgoing Electron", -200, 200);
public:
    AngleHistograms();
};


#endif //ANGLEHISTOGRAMS_H
