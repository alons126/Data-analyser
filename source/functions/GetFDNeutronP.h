//
// Created by alons on 28/04/2023.
//

#ifndef GETFDNEUTRONP_H
#define GETFDNEUTRONP_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../constants.h"

double GetFDNeutronP(region_part_ptr &Neutron, bool apply_neutron_Beta_Fit) {
    double Momentum;

    if (apply_neutron_Beta_Fit) {

        //<editor-fold desc="Get for ver. neutrons & calculate for 'photons'">
        int ParticlePDG = Neutron->par()->getPid();

        if (ParticlePDG == 2112) {
            /* Momentum for neutrons - get from file. */
            Momentum = Neutron->getP();

//        //<editor-fold desc="how neutron momentum is calculated in clas12">
//        TVector3 P_n;
//        P_n.SetMagThetaPhi(Neutron->getP(), Neutron->getTheta(), Neutron->getPhi());
//        double E_n = sqrt(m_n * m_n + P_n.Mag2());
//        double Path_n = Neutron->getPath();
//
//        /* NOTE: Beta = Beta_from_Momentum */
//        double Beta_n = Neutron->par()->getBeta();
//        double Beta_n_from_Momentum = P_n.Mag() / E_n;
//
//        /* NOTE: Time_from_Momentum = Time_from_Beta_n */
//        double Time_from_Momentum = Path_n / (c * Beta_n_from_Momentum);
//        double Time_from_Beta_n = Path_n / (c * Beta_n);
//
//        double Velocity_n = Path_n / Time_from_Beta_n;
//        double Gamma_n = 1 / sqrt(1 - (Velocity_n * Velocity_n) / (c * c));
//        double Momentum_n = m_n * Beta_n * Gamma_n; // how neutron momentum is calculated in clas12
//        //</editor-fold>

        } else if (ParticlePDG == 22) {
            /* Momentum for "photons" - calculate. */

            /* NOTE: all of these give the same path:
             * Taking dv_nhit = (Neutron->cal(detlayer)->getX() - Neutron->par()->getVx(), X <-> Y,  X <-> Z) and dv_nhit.Mag().
             * Neutron->cal(detlayer)->getPath().
             * Neutron->getPath(). */

            /* NOTE: Neutron->cal(detlayer)->getTime() = Neutron->getTime() */

            double Path_ph = Neutron->getPath();
            double Time_ph = Neutron->getTime(); // bad
//        double Velocity = Path_ph / Time_ph;
            double Beta_ph = Neutron->par()->getBeta();
            double Time_ph_from_Beta_ph = Path_ph / (c * Beta_ph);
            double Velocity_ph = Path_ph / Time_ph_from_Beta_ph;
//        double Gamma_ph = 1 / sqrt(1 - (Velocity_ph * Velocity_ph) / (c * c));
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));

            Momentum = m_n * Beta_ph * Gamma_ph;
        }
        //</editor-fold>

    } else if (!apply_neutron_Beta_Fit) {

        //<editor-fold desc="Get momentum for both neutrons and photons from file">
        Momentum = Neutron->par()->getP();
        //</editor-fold>

    }

    return Momentum;
}

////<editor-fold desc="Old">
//double GetFDNeutronP(region_part_ptr &Neutron) {
//    double Momentum;
//
//    //<editor-fold desc="Get momentum for both neutrons and photons from file">
//    Momentum = Neutron->par()->getP();
//    //</editor-fold>
//
////    //<editor-fold desc="Calculate for both neutrons and photons">
////    bool PCALhit = (Neutron->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
////    bool ECINhit = (Neutron->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
////    bool ECOUThit = (Neutron->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit
////
////    if (PCALhit) { cout << "\n\nGetFDNeutronP: neutron hit in PCAL. Exiting...\n\n", exit(EXIT_FAILURE); }
////
////    auto detlayer = ECINhit ? clas12::ECIN : clas12::ECOUT; // find first layer of hit
////
////    double path = Neutron->cal(detlayer)->getPath();
////    double time = Neutron->cal(detlayer)->getTime();
////    double velocity = path / time;
////    double beta = velocity / c;
////    double gamma = 1 / sqrt(1 - (beta * beta));
////
////    Momentum = m_n*velocity*gamma;
//////    Momentum = m_n*beta*gamma;
////
//////    cout << "\n\n\nMomentum:\t" << Momentum << "\n";
//////    cout << "\n\n\nTEST TEST TEST TEST TEST TEST\n\n\n";
////    //</editor-fold>
//
//    //<editor-fold desc="Get for ver. neutrons & calculate for 'photons'">
//    int ParticlePDG = Neutron->par()->getPid();
//
//    if (ParticlePDG == 2112) {
//        /* Momentum for neutrons - get from file. */
//        Momentum = Neutron->getP();
//
////        //<editor-fold desc="how neutron momentum is calculated in clas12">
////        TVector3 P_n;
////        P_n.SetMagThetaPhi(Neutron->getP(), Neutron->getTheta(), Neutron->getPhi());
////        double E_n = sqrt(m_n * m_n + P_n.Mag2());
////        double Path_n = Neutron->getPath();
////
////        /* NOTE: Beta = Beta_from_Momentum */
////        double Beta_n = Neutron->par()->getBeta();
////        double Beta_n_from_Momentum = P_n.Mag() / E_n;
////
////        /* NOTE: Time_from_Momentum = Time_from_Beta_n */
////        double Time_from_Momentum = Path_n / (c * Beta_n_from_Momentum);
////        double Time_from_Beta_n = Path_n / (c * Beta_n);
////
////        double Velocity_n = Path_n / Time_from_Beta_n;
////        double Gamma_n = 1 / sqrt(1 - (Velocity_n * Velocity_n) / (c * c));
////        double Momentum_n = m_n * Beta_n * Gamma_n; // how neutron momentum is calculated in clas12
////        //</editor-fold>
//
//    } else if (ParticlePDG == 22) {
//        /* Momentum for "photons" - calculate. */
//
//        /* NOTE: all of these give the same path:
//         * Taking dv_nhit = (Neutron->cal(detlayer)->getX() - Neutron->par()->getVx(), X <-> Y,  X <-> Z) and dv_nhit.Mag().
//         * Neutron->cal(detlayer)->getPath().
//         * Neutron->getPath(). */
//
//        /* NOTE: Neutron->cal(detlayer)->getTime() = Neutron->getTime() */
//
//        double Path_ph = Neutron->getPath();
//        double Time_ph = Neutron->getTime(); // bad
////        double Velocity = Path_ph / Time_ph;
//        double Beta_ph = Neutron->par()->getBeta();
//        double Time_ph_from_Beta_ph = Path_ph / (c * Beta_ph);
//        double Velocity_ph = Path_ph / Time_ph_from_Beta_ph;
////        double Gamma_ph = 1 / sqrt(1 - (Velocity_ph * Velocity_ph) / (c * c));
//        double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
//
//        Momentum = m_n * Beta_ph * Gamma_ph;
//    }
//    //</editor-fold>
//
//    return Momentum;
//}
////</editor-fold>

#endif //GETFDNEUTRONP_H
