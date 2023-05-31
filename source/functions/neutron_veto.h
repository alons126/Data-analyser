#ifndef Neutron_veto_HH
#define Neutron_veto_HH
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <vector>
#include <typeinfo>

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>

#include "clas12reader.h"
#include "HipoChain.h"

#include "TF1.h"
#include "TCanvas.h"

bool isTrackHitNear(int sdiff, int ldiff) {
    if ((ldiff == -2) && (sdiff >= -1) && (sdiff <= 0)) { return true; }
    if ((ldiff == -1) && (sdiff >= -1) && (sdiff <= 2)) { return true; }
    if ((ldiff == 0) && (sdiff >= -1) && (sdiff <= 2)) { return true; }
    if ((ldiff == 1) && (sdiff >= -1) && (sdiff <= 2)) { return true; }
    if ((ldiff == 2) && (sdiff >= -1) && (sdiff <= 2)) { return true; }
    if ((ldiff == 3) && (sdiff >= -1) && (sdiff <= 2)) { return true; }
    return false;
}

bool isAllCNDHitNear(int sdiff, int ldiff) {
    if ((ldiff == -1) && (sdiff == -2)) { return true; }
    if ((ldiff == -1) && (sdiff == -1)) { return true; }
    if ((ldiff == -1) && (sdiff == 1)) { return true; }
    //if((ldiff==-1) && (sdiff== 2)){return true;}

    if ((ldiff == 0) && (sdiff == -4)) { return true; }
    if ((ldiff == 0) && (sdiff == -3)) { return true; }
    if ((ldiff == 0) && (sdiff == -2)) { return true; }
    if ((ldiff == 0) && (sdiff == -1)) { return true; }
    if ((ldiff == 0) && (sdiff == 1)) { return true; }
    if ((ldiff == 0) && (sdiff == 2)) { return true; }
    if ((ldiff == 0) && (sdiff == 3)) { return true; }

    if ((ldiff == 1) && (sdiff == -2)) { return true; }
    if ((ldiff == 1) && (sdiff == -1)) { return true; }
    if ((ldiff == 1) && (sdiff == 1)) { return true; }
    //if((ldiff== 1) && (sdiff== 2)){return true;}

    return false;
}

bool isAllCTOFHitNear(int sdiff, int ldiff) {
    if ((ldiff == 1) && (sdiff == -3)) { return true; }
    if ((ldiff == 1) && (sdiff == -2)) { return true; }
    if ((ldiff == 1) && (sdiff == -1)) { return true; }
    if ((ldiff == 1) && (sdiff == 1)) { return true; }
    if ((ldiff == 1) && (sdiff == 2)) { return true; }
    if ((ldiff == 1) && (sdiff == 3)) { return true; }

    if ((ldiff == 2) && (sdiff == -3)) { return true; }
    if ((ldiff == 2) && (sdiff == -2)) { return true; }
    if ((ldiff == 2) && (sdiff == -1)) { return true; }
    if ((ldiff == 2) && (sdiff == 0)) { return true; }
    if ((ldiff == 2) && (sdiff == 1)) { return true; }
    if ((ldiff == 2) && (sdiff == 2)) { return true; }
    if ((ldiff == 2) && (sdiff == 3)) { return true; }

    if ((ldiff == 3) && (sdiff == -3)) { return true; }
    if ((ldiff == 3) && (sdiff == -2)) { return true; }
    if ((ldiff == 3) && (sdiff == -1)) { return true; }
    if ((ldiff == 3) && (sdiff == 0)) { return true; }
    if ((ldiff == 3) && (sdiff == 1)) { return true; }
    if ((ldiff == 3) && (sdiff == 2)) { return true; }
    if ((ldiff == 3) && (sdiff == 3)) { return true; }

    return false;
}

TVector3 NeutronMomentum(const std::unique_ptr<clas12::clas12reader> &c12, int index) {
    auto allParticles = c12->getDetParticles();

    //No determine the layer of the neutral hit
    bool C1 = (allParticles[index]->sci(clas12::CND1)->getDetector() == 3);
    bool C2 = (allParticles[index]->sci(clas12::CND2)->getDetector() == 3);
    bool C3 = (allParticles[index]->sci(clas12::CND3)->getDetector() == 3);
    auto detlayer = C1 ? clas12::CND1 : C2 ? clas12::CND2 : clas12::CND3;

    //The momentum might be wrong because
    //because it assumes a mass based on
    //the beta. So we recalculate based
    //only on the neutron momentum.
    double beta = allParticles[index]->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double mom = gamma * beta * mN;

    //Now grab the hit positon to give you
    //the direction and Z. You can now get
    //the three momentum
    TVector3 v_nvtx(allParticles[index]->par()->getVx(), allParticles[index]->par()->getVy(), allParticles[index]->par()->getVz());
    TVector3 v_nhit(allParticles[index]->sci(detlayer)->getX(), allParticles[index]->sci(detlayer)->getY(), allParticles[index]->sci(detlayer)->getZ());
    TVector3 v_npath = v_nhit - v_nvtx;
    TVector3 p_n;
    p_n.SetMagThetaPhi(mom, v_npath.Theta(), v_npath.Phi());

    return p_n;
}

bool NeutronCD_CutVeto(const std::unique_ptr<clas12::clas12reader> &c12, int index, int cnd_hits, int cnd_hit_layer, int cnd_hit_sector, int cnd_hit_energy,
                       int ctof_hits, int ctof_hit_component, int ctof_hit_energy) {
    auto allParticles = c12->getDetParticles();
    //Only get the zero charge parricles
    if (allParticles[index]->par()->getCharge() != 0) { return false; }

    //No determine the layer of the neutral hit
    bool CT = (allParticles[index]->sci(clas12::CTOF)->getDetector() == 4);
    bool C1 = (allParticles[index]->sci(clas12::CND1)->getDetector() == 3);
    bool C2 = (allParticles[index]->sci(clas12::CND2)->getDetector() == 3);
    bool C3 = (allParticles[index]->sci(clas12::CND3)->getDetector() == 3);
    int detINTlayer = C1 ? 1 : C2 ? 2 : 3;
    auto detlayer = C1 ? clas12::CND1 : C2 ? clas12::CND2 : clas12::CND3;
    if (!(C1 || C2 || C3)) { return false; }

    //The momentum might be wrong because
    //because it assumes a mass based on
    //the beta. So we recalculate based
    //only on the neutron momentum.
    double beta = allParticles[index]->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double mom = gamma * beta * mN;
    double ToF = allParticles[index]->getTime() - c12->event()->getStartTime();

    //Get energy deposition in all layers
    double edep = allParticles[index]->sci(clas12::CND1)->getEnergy() + allParticles[index]->sci(clas12::CND2)->getEnergy() +
                  allParticles[index]->sci(clas12::CND3)->getEnergy();

    //Now grab the hit positon to give you
    //the direction and Z. You can now get
    //the three momentum
    int n_sector = allParticles[index]->sci(detlayer)->getSector();
    TVector3 v_nvtx(allParticles[index]->par()->getVx(), allParticles[index]->par()->getVy(), allParticles[index]->par()->getVz());
    TVector3 v_nhit(allParticles[index]->sci(detlayer)->getX(), allParticles[index]->sci(detlayer)->getY(), allParticles[index]->sci(detlayer)->getZ());
    TVector3 v_npath = v_nhit - v_nvtx;
    double path = v_npath.Mag() / 100;
    TVector3 p_n;
    p_n.SetMagThetaPhi(mom, v_npath.Theta(), v_npath.Phi());

    //Now we can apply some cuts
    //Some events have a wrong beta
    if (beta - (path * 100) / (ToF * c) < -0.01) { return false; }
    if (beta - (path * 100) / (ToF * c) > 0.01) { return false; }
    //The hit should be inside the CND
    if (v_nhit.Z() > 45) { return false; }
    if (v_nhit.Z() < -40) { return false; }
    //No Neutrons arrive after 20s (almost none after 10ns)
    if (ToF < 0) { return false; }
    if (ToF > 20) { return false; }


    //Now we want to veto events where a charged
    //track went nearby.
    bool TrackHitVeto = false;
    for (int k = 0; k < allParticles.size(); k++) {
        //Skip the forward detector electron
        //Skip this particle
        //Only charged particles
        //Only central detector particles
        if (k == 0) { continue; }
        if (k == index) { continue; }
        if (allParticles[k]->par()->getCharge() <= 0) { continue; }
        if (allParticles[k]->sci(clas12::CTOF)->getDetector() == 0) { continue; }
        //Put all hits into the CND sector
        //The CTOF Sectors map to CND sectors by:
        //CND_Sector = CTOF_Component+1/2 rounded down
        int vetoSectorbyLayer[4] = {(allParticles[k]->sci(clas12::CTOF)->getComponent() + 1) / 2, allParticles[k]->sci(clas12::CND1)->getSector(),
                                    allParticles[k]->sci(clas12::CND2)->getSector(), allParticles[k]->sci(clas12::CND3)->getSector()};
        for (int k = 0; k < 4; k++) {
            //Continue if the hit did not trigger this layer
            if (vetoSectorbyLayer[k] == 0) { continue; }
            //Get the difference in sectors by wrapping the ends
            int sdiff = n_sector - vetoSectorbyLayer[k];
            if (sdiff <= -12) { sdiff += 24; }
            else if (sdiff > 12) { sdiff -= 24; }
            //Layer difference is trivial
            int ldiff = detINTlayer - k;
            //If something is close then veto
            if (isTrackHitNear(sdiff, ldiff)) { TrackHitVeto = true; }
        }
    }
    if (TrackHitVeto) { return false; }

    //Now we want to loop over all hits in the CND.
    //We will need to access this bank directly.
    bool AllCNDHitVeto = false;
    for (int row = 0; row < c12->getBank(cnd_hits)->getRows(); row++) {
        //Grab the sector, layer, and energy deposition
        int hit_sector = c12->getBank(cnd_hits)->getInt(cnd_hit_sector, row);
        int hit_layer = c12->getBank(cnd_hits)->getInt(cnd_hit_layer, row);
        double hit_edep = c12->getBank(cnd_hits)->getFloat(cnd_hit_energy, row);
        //Apply a minimum energy depostion
        //to the hits we will veto
        if (hit_edep < 5) { continue; }
        //Get the difference in sectors by wrapping the ends
        int sdiff = n_sector - hit_sector;
        if (sdiff <= -12) { sdiff += 24; }
        else if (sdiff > 12) { sdiff -= 24; }
        //Layer difference is trivial
        int ldiff = detINTlayer - hit_layer;
        //Don't include the hit itself
        if ((ldiff == 0) && (sdiff == 0)) { continue; }
        if (isAllCNDHitNear(sdiff, ldiff)) { AllCNDHitVeto = true; };
    }
    if (AllCNDHitVeto) { return false; }


    //Now we want to loop over all hits in the CTOF.
    //We will need to access this bank directly.
    bool AllCTOFHitVeto = false;
    for (int row = 0; row < c12->getBank(ctof_hits)->getRows(); row++) {
        //Grab the sector from component and energy deposition
        int hit_sector = (c12->getBank(ctof_hits)->getInt(ctof_hit_component, row) + 1) / 2;
        double hit_edep = c12->getBank(ctof_hits)->getFloat(ctof_hit_energy, row);
        //Apply a minimum energy depostion
        //to the hits we will veto
        if (hit_edep < 5) { continue; }
        //Get the difference in sectors by wrapping the ends
        int sdiff = n_sector - hit_sector;
        if (sdiff <= -12) { sdiff += 24; }
        else if (sdiff > 12) { sdiff -= 24; }
        //Layer difference is trivial
        int ldiff = detINTlayer;
        //Don't include the hit itself
        if ((ldiff == 0) && (sdiff == 0)) { continue; }
        if (isAllCTOFHitNear(sdiff, ldiff)) { AllCTOFHitVeto = true; };
    }
    if (AllCTOFHitVeto) { return false; }

    return true;
}

bool NeutronECAL_CutVeto(const std::unique_ptr<clas12::clas12reader> &c12, double Ebeam, int index) {
    TVector3 p_b(0, 0, Ebeam);

    auto electrons = c12->getByID(11);
    TVector3 p_e;
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 p_q = p_b - p_e;

    auto allParticles = c12->getDetParticles();
    if (allParticles[index]->par()->getCharge() != 0) { return false; }
    //Check which layers of the ECAL have been hit
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, allParticles[index]->getTheta(), allParticles[index]->getPhi());

    bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT;

    TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());
    double beta = allParticles[index]->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = p_n_Angles.Theta() * 180 / M_PI;
    double theta_q = p_q.Theta() * 180 / M_PI;
    double theta_nq = p_n_Angles.Angle(p_q) * 180 / M_PI;
    //cout<<beta<<endl;
    if (beta < 0) { return false; }
    if (beta > 1.1) { return false; }
    //if(theta_nq>25){return false;}
    //if(theta_q>40){return false;}
    if (theta_n < 1) { return false; }
    if (!(IC || OC)) { return false; }
    if (PC) { return false; }

    //Now let's put a charge particle veto
    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; }
        TVector3 v_chit;
        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nhit - v_chit;
            if (v_dist.Mag() < 100) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nhit - v_chit;
            if (v_dist.Mag() < 100) { Veto = true; }
        } else {
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nhit - v_chit;
            if (v_dist.Mag() < 100) { Veto = true; }
        }
    }

    if (Veto) { return false; }

    return true;
}


#endif

