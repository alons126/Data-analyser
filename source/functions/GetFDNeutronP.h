//
// Created by alons on 28/04/2023.
//

#ifndef DATA_ANALYSER_GETFDNEUTRONP_H
#define DATA_ANALYSER_GETFDNEUTRONP_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../constants.h"

double GetFDNeutronP(region_part_ptr &Neutron) {
    double Momentum;

    int ParticlePDG = Neutron->par()->getPid();

    if (ParticlePDG == 2112) {
        /* Momentum for neutrons - get from file. */
        Momentum = Neutron->getP();

        TVector3 P_n;
        P_n.SetMagThetaPhi(Neutron->getP(), Neutron->getTheta(), Neutron->getPhi());
        double E_n = sqrt(m_n * m_n + P_n.Mag2());

        double Path = Neutron->getPath();
        double Time = Neutron->getTime(); // bad
        double Velocity = Path / Time;

        /* NOTE: Beta = Beta_from_Momentum */
        double Beta = Neutron->par()->getBeta();
        double Beta_from_Momentum = P_n.Mag() / E_n;
        double Beta_from_Velocity = Velocity / c; // bad

        /* NOTE: Time_from_Momentum = Time_from_Beta */
        double Time_from_Momentum = Path / (c * Beta_from_Momentum);
        double Time_from_Beta = Path / (c * Beta);

        double Velocity1 = Path / Time_from_Beta;
        double Gamma1 = 1 / sqrt(1 - (Velocity1 * Velocity1) / (c * c));
        double Gamma11 = 1 / sqrt(1 - (Velocity1 * Velocity1));
        double Velocity2 = Path / Time;
        double Momentum1 = m_n * Velocity1;
        double Momentum2 = m_n * Velocity2;
        double Momentum3 = m_n * Beta * Gamma1;

//        cout << "\n\n\n=========================================================" << "\n\n";
//        cout << "P_n:\t\t\t" << P_n.Mag() << "\n\n";
//        cout << "Momentum3:\t\t" << Momentum3 << "\n";
////        cout << "Momentum1 / c:\t\t" << Momentum1 / c << "\n";
////        cout << "Momentum1 * Gamma11 / c:\t\t" << Momentum1 * Gamma11 / c << "\n";
////        cout << "Momentum1 * Gamma11:\t\t" << Momentum1 * Gamma11 << "\n";



/*
//        cout << "Momentum2:\t\t" << Momentum2 << "\n";
//        cout << "Beta:\t\t\t" << Beta << "\n";
////        cout << "Beta_from_Momentum:\t" << Beta_from_Momentum << "\n";
////        cout << "Beta_from_Velocity:\t" << Beta_from_Velocity << "\n";
//        cout << "Path:\t\t\t" << Path << "\n";
//        cout << "Time:\t\t\t" << Time << "\n";
//        cout << "Velocity:\t\t" << Velocity << "\n";
////        cout << "Time_from_Momentum:\t" << Time_from_Momentum << "\n";
////        cout << "Time_from_Beta:\t\t" << Time_from_Beta << "\n";
*/
/*
        cout << "\n\n\nE_n:\t\t\t" << E_n << "\n";
//        cout << "P_n / m_n:\t\t\t" << P_n.Mag() / m_n << "\n";
//        cout << "P_n * c:\t\t\t" << P_n.Mag() * c << "\n";
        cout << "P_n:\t\t\t" << P_n.Mag() << "\n";
        cout << "Momentum1 / c:\t\t\t" << Momentum1 / c << "\n";
        cout << "Momentum2:\t\t\t" << Momentum2 << "\n";
        cout << "Beta:\t\t\t" << Beta << "\n";
//        cout << "Beta_from_Momentum:\t" << Beta_from_Momentum << "\n";
//        cout << "Beta_from_Velocity:\t" << Beta_from_Velocity << "\n";
        cout << "Path:\t\t\t" << Path << "\n";
        cout << "Time:\t\t\t" << Time << "\n";
        cout << "Velocity:\t\t" << Velocity << "\n";
//        cout << "Time_from_Momentum:\t" << Time_from_Momentum << "\n";
//        cout << "Time_from_Beta:\t\t" << Time_from_Beta << "\n";
*/
/*
        cout << "\n\n\nE_n:\t\t\t" << E_n << "\n";
        cout << "P_n:\t\t\t" << Momentum << "\n";
        cout << "Beta:\t\t\t" << Beta << "\n";
        cout << "Beta_from_Momentum:\t" << Beta_from_Momentum << "\n";
        cout << "Beta_from_Velocity:\t" << Beta_from_Velocity << "\n";
        cout << "Path:\t\t\t" << Path << "\n";
        cout << "Time:\t\t\t" << Time << "\n";
        cout << "Velocity:\t\t" << Velocity << "\n";
        cout << "Time_from_Momentum:\t" << Time_from_Momentum << "\n";
        cout << "Time_from_Beta:\t\t" << Time_from_Beta << "\n";
*/

    } else if (ParticlePDG == 22) {
        /* Momentum for "photons" - calculate. */

        /* NOTE: all of these give the same path:
         * dv_nhit = (Neutron->cal(detlayer)->getX() - Neutron->par()->getVx(), X <-> Y,  X <-> Z)
         * Neutron->cal(detlayer)->getPath()
         * Neutron->getPath() */

        /* NOTE: all of these give the same time:
         * Neutron->cal(detlayer)->getTime()
         * Neutron->getTime() */

//        TVector3 P_n;
//        P_n.SetMagThetaPhi(Neutron->getP(), Neutron->getTheta(), Neutron->getPhi());
//        double E_n = sqrt(m_n * m_n + P_n.Mag2());

        double Path = Neutron->getPath();
        double Time = Neutron->getTime(); // bad
//        double Velocity = Path / Time;

        double Beta = Neutron->par()->getBeta();
//        double Beta_from_Momentum = P_n.Mag() / E_n;
//        double Beta_from_Velocity = Velocity / c; // bad

//        double Time_from_Momentum = Path / (c * Beta_from_Momentum);
        double Time_from_Beta = Path / (c * Beta);

        double Velocity1 = Path / Time_from_Beta;
        double Gamma1 = 1 / sqrt(1 - (Velocity1 * Velocity1) / (c * c));
        double Gamma11 = 1 / sqrt(1 - (Velocity1 * Velocity1));
        double Velocity2 = Path / Time;
        double Momentum1 = m_n * Velocity1;
        double Momentum2 = m_n * Velocity2;
        Momentum = m_n * Beta * Gamma1;

//        cout << "\n\n\n=========================================================" << "\n\n";
////        cout << "P_n:\t\t\t" << P_n.Mag() << "\n\n";
//        cout << "Momentum:\t\t" << Momentum << "\n";
////        cout << "Momentum1 / c:\t\t" << Momentum1 / c << "\n";
////        cout << "Momentum1 * Gamma11 / c:\t\t" << Momentum1 * Gamma11 / c << "\n";
////        cout << "Momentum1 * Gamma11:\t\t" << Momentum1 * Gamma11 << "\n";

//        //<editor-fold desc="old">
//        double Path = Neutron->getPath();
//        double Time = Neutron->getTime();
//        double Velocity = Path / Time;
//        double Beta = Velocity / c;
//        double Gamma = 1 / sqrt(1 - (Beta * Beta));
//////        double Beta_from_Velocity = Velocity / c;
//        double Time_from_Beta = Path / (c * Beta);
////        double Velocity_from_Beta = Path / Time_from_Beta;
//
//        Momentum = m_n * Velocity;
////        Momentum = Path / (Time_from_Beta * Beta);
////        cout << "\n\nTime = " << Time << "\n";
//
//        //        cout << "c = Velocity_from_Beta / Beta = " << Velocity_from_Beta / Beta << "\n";
//////        Momentum = Path / (Time_from_Beta * Beta);
////        cout << "\n\nMomentum = m_n * Velocity = " << m_n * Velocity << "\n";
////        cout << "Gamma = " << Gamma << "\n";
////        cout << "Momentum * Gamma = " << Momentum * Gamma << "\n";
////        cout << "E = sqrt(m_n * m_n + Momentum * Momentum) = " << sqrt(m_n * m_n + Momentum * Momentum) << "\n";
////        cout << "E = sqrt(m_n * m_n + (Momentum * Gamma) * (Momentum * Gamma)) = " << sqrt(m_n * m_n + (Momentum * Gamma) * (Momentum * Gamma)) << "\n";
//////        cout << "c = Velocity_from_Beta / Beta = " << Velocity_from_Beta / Beta << "\n";
///*
//        double Path = Neutron->getPath();
////        double Time = Neutron->cal(detlayer)->getTime();
////        double Velocity = Path / Time;
//        double Beta = Neutron->par()->getBeta();
//        double Gamma = 1 / sqrt(1 - (Beta * Beta));
////        double Beta_from_Velocity = Velocity / c;
//        double Time_from_Beta = Path / (c * Beta);
//        double Velocity_from_Beta = Path / Time_from_Beta;
//
//        Momentum = m_n * Velocity_from_Beta;
////        Momentum = Path / (Time_from_Beta * Beta);
//        cout << "\n\nMomentum = " << Momentum << "\n";
//        cout << "c = Velocity_from_Beta / Beta = " << Velocity_from_Beta / Beta << "\n";
//*/
//
//
////        cout << "\n\nc = " << Path / (Time * Beta) << "\n";
//////        cout << "\n\nc = " << Path / (Time_from_Beta * Beta) << "\n";
////        cout << "Beta_from_Velocity = " << Beta_from_Velocity << "\n";
////        cout << "Beta = " << Beta << "\n";
////        cout << "Beta/Beta_from_Velocity = " << Beta/Beta_from_Velocity << "\n";
////        cout << "Time = " << Time << "\n";
////        cout << "Time_from_Beta = " << Time_from_Beta << "\n";
//
////        Momentum = m_n * (ParticlePath / ParticleTime);
////        Momentum = m_n * PhotonBeta * c / sqrt(1 - PhotonBeta * PhotonBeta);
////        Momentum = m_n * (ParticlePath / Time_from_Beta);
////        Momentum = m_n * (ParticlePath / Time_from_Beta) / 10;
//
//
////        double ParticlePath = Neutron->getPath();
////        double ParticleBeta = Neutron->par()->getBeta();
////        double Time_from_Beta = ParticlePath / (c * ParticleBeta);
////
////        Momentum = m_n * ParticlePath / Time_from_Beta;
//        //</editor-fold>
    }

    return Momentum;
}

#endif //DATA_ANALYSER_GETFDNEUTRONP_H
