//
// Created by alons on 15/05/2023.
//

#ifndef GETPI0MOMTH_H
#define GETPI0MOMTH_H

#include <math.h>

#include "../classes/DSCuts/DSCuts.h"
#include "../constants.h"

double GetPi0MomTh(DSCuts &ph_mom_th) {
    if (fabs(ph_mom_th.GetLowerCut()) == 9999) {
        return -9999;
    } else {
        double Photons_momentum_th = ph_mom_th.GetLowerCut();
        double Pi0MomTh = sqrt((2 * Photons_momentum_th) * (2 * Photons_momentum_th) - m_pizero * m_pizero);

        return Pi0MomTh;
    }
}

#endif //GETPI0MOMTH_H
