//
// Created by alons on 04/06/2023.
//

#ifndef CALCDPHI_H
#define CALCDPHI_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../../constants.h"

using namespace std;

double CalcdPhi(double dPhiTemp) {
    double dPhi;

    if (dPhiTemp > 180.0) {
        dPhi = dPhiTemp - 360.0;
    } else if (dPhiTemp < -180.0) {
        dPhi = dPhiTemp + 360.0;
    } else {
        dPhi = dPhiTemp;
    }

    return dPhi;
}

double CalcdPhi(region_part_ptr proton1, region_part_ptr proton2) {
    double dPhi, PhiFD, PhiCD;

    if (proton1->getRegion() == proton2->getRegion()) {
        cout << "\n\nCalcdPhi: protons are in the same region (" << proton1->getRegion() << " & " << proton2->getRegion() << ")! Exiting...\n", exit(0);
    } else if (proton1->getRegion() == FD && proton2->getRegion() == CD) {
        PhiFD = proton1->getPhi() * 180.0 / pi, PhiCD = proton2->getPhi() * 180.0 / pi;
    } else if (proton1->getRegion() == CD && proton2->getRegion() == FD) {
        PhiFD = proton2->getPhi() * 180.0 / pi, PhiCD = proton1->getPhi() * 180.0 / pi;
    }

    dPhi = CalcdPhi(PhiFD - PhiCD);

    return dPhi;
}

#endif //CALCDPHI_H
