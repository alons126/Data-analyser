//
// Created by alons on 13/06/2023.
//

#include "TLCuts.h"

TLCuts::TLCuts(const string &SampleName, bool calculate_truth_level, bool Enable_photons_FD, bool apply_nucleon_cuts,
               TH2D *Electron_AMap, TH2D *Proton_AMap, TH2D *Neutron_AMap,
               const DSCuts &ThetaFD, double Theta_uboundary_FD, double Theta_lboundary_FD,
               const DSCuts &ThetaCD, double Theta_uboundary_CD, double Theta_lboundary_CD,
               double Phi_lboundary, double Phi_uboundary,
               const DSCuts &TL_e_mom_cuts, const DSCuts &TL_n_mom_cuts, const DSCuts &TL_p_mom_cuts,
               const DSCuts &TL_pip_mom_cuts, const DSCuts &TL_pim_mom_cuts, const DSCuts &TL_pi0_mom_cuts, const DSCuts &TL_ph_mom_cuts,
               const std::unique_ptr<clas12::clas12reader> &c12) {
    if (calculate_truth_level && findSubstring(SampleName, "sim") && apply_nucleon_cuts) { // run only for CLAS12 simulation & AFTER beta fit
        auto mcpbank = c12->mcparts();
        const Int_t Ngen = mcpbank->getRows();

        //<editor-fold desc="Particle counting">
        for (Int_t i = 0; i < Ngen; i++) {
            mcpbank->setEntry(i);

            int particlePDGtmp = mcpbank->getPid();

            double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            int BinX = GetBinFromAng(Particle_TL_Phi, Neutron_AMap->GetNbinsX(), Phi_lboundary, Phi_uboundary, false, "Phi");
            int BinY = GetBinFromAng(Particle_TL_Theta, Neutron_AMap->GetNbinsY(), Theta_lboundary_FD, Theta_uboundary_FD, false, "Theta");
            int BinX_e = GetBinFromAng(Particle_TL_Phi, Electron_AMap->GetNbinsX(), Phi_lboundary, Phi_uboundary, false, "Phi");
            int BinY_e = GetBinFromAng(Particle_TL_Theta, Electron_AMap->GetNbinsY(), Theta_lboundary_FD, Theta_uboundary_FD, false, "Theta");
            int BinX_p = GetBinFromAng(Particle_TL_Phi, Proton_AMap->GetNbinsX(), Phi_lboundary, Phi_uboundary, false, "Phi");
            int BinY_p = GetBinFromAng(Particle_TL_Theta, Proton_AMap->GetNbinsY(), Theta_lboundary_FD, Theta_uboundary_FD, false, "Theta");

            bool inSomeSector = (Neutron_AMap->GetBinContent(BinX, BinY) != 0);
            bool e_inSomeSector = (Electron_AMap->GetBinContent(BinX_e, BinY_e) != 0);
            bool p_inSomeSector = (Proton_AMap->GetBinContent(BinX_p, BinY_p) != 0);
            bool n_inSomeSector = (Proton_AMap->GetBinContent(BinX_p, BinY_p) != 0);

            bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCutConst()) && (Particle_TL_Theta <= ThetaFD.GetUpperCutConst()));
            bool e_inFD = (e_inSomeSector && (Particle_TL_Theta >= ThetaFD.GetLowerCutConst()) && (Particle_TL_Theta <= ThetaFD.GetUpperCutConst()));
            bool p_inFD = (p_inSomeSector && (Particle_TL_Theta >= ThetaFD.GetLowerCutConst()) && (Particle_TL_Theta <= ThetaFD.GetUpperCutConst()));
            bool n_inFD = (n_inSomeSector && (Particle_TL_Theta >= ThetaFD.GetLowerCutConst()) && (Particle_TL_Theta <= ThetaFD.GetUpperCutConst()));
            bool inCD = ((Particle_TL_Theta > ThetaCD.GetLowerCutConst()) && (Particle_TL_Theta <= ThetaCD.GetUpperCutConst()));

            if (particlePDGtmp == 11) {
                if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCutConst()) &&
                    (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCutConst())) { TL_Electron_mom_ind.push_back(i); }

                TL_Electron_ind.push_back(i);

                if (e_inFD) {
                    if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCutConst()) &&
                        (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCutConst())) { TL_ElectronFD_mom_ind.push_back(i); }

                    TL_ElectronFD_ind.push_back(i);
                }
            } else if (particlePDGtmp == 2112) {
                if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCutConst()) &&
                    (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCutConst())) { TL_Neutrons_mom_ind.push_back(i); }

                TL_Neutrons_ind.push_back(i);

                if (n_inFD) {
//                    if (inFD) {
                    if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCutConst()) &&
                        (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCutConst())) { TL_NeutronsFD_mom_ind.push_back(i); }

                    TL_IDed_neutrons_FD.push_back(i);
                }
            } else if (particlePDGtmp == 2212) {
                if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCutConst()) &&
                    (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCutConst())) { TL_Protons_mom_ind.push_back(i); }

                TL_Protons_ind.push_back(i);

                if (p_inFD) {
//                    if (inFD) {
                    if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCutConst()) &&
                        (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCutConst())) { TL_ProtonsFD_mom_ind.push_back(i); }

                    TL_ProtonsFD_ind.push_back(i);
                } else if (inCD) {
                    if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCutConst()) &&
                        (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCutConst())) { TL_ProtonsCD_mom_ind.push_back(i); }

                    TL_ProtonsCD_ind.push_back(i);
                }
            } else if (particlePDGtmp == 211) {
                if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCutConst()) &&
                    (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCutConst())) { TL_piplus_mom_ind.push_back(i); }

                TL_piplus_ind.push_back(i);
            } else if (particlePDGtmp == -211) {
                if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCutConst()) &&
                    (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCutConst())) { TL_piminus_mom_ind.push_back(i); }

                TL_piminus_ind.push_back(i);
            } else if (particlePDGtmp == 111) {
                if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCutConst()) &&
                    (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCutConst())) { TL_pizero_mom_ind.push_back(i); }

                TL_pizero_ind.push_back(i);

                if (inFD) {
                    if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCutConst()) &&
                        (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCutConst())) { TL_pi0FD_mom_ind.push_back(i); }

                    TL_pi0FD_ind.push_back(i);
                }
            } else if (particlePDGtmp == 22) {
                if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCutConst()) &&
                    (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCutConst())) { TL_Photons_mom_ind.push_back(i); }

                TL_Photons_ind.push_back(i);

                if (inFD) {
                    if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCutConst()) &&
                        (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCutConst())) { TL_PhotonsFD_mom_ind.push_back(i); }

                    TL_PhotonsFD_ind.push_back(i);
                }
            } else {
                TL_OtherPart_ind.push_back(i);
            }
        }
        //</editor-fold>

        //<editor-fold desc="Selection setup">
        /* Setting up basic TL event selection */
        no_TL_cPions = (TL_piplus_mom_ind.size() == 0 && TL_piminus_mom_ind.size() == 0);                  // No id. cPions above momentum threshold
        no_TL_OtherPart = (TL_OtherPart_ind.size() == 0);                                                  // No other part. above momentum threshold
        no_TL_FDpi0 = (Enable_photons_FD || (TL_pi0FD_mom_ind.size() == 0));                               // No id. pi0 in the FD above momentum threshold
        no_TL_FDPhotons = (Enable_photons_FD || (TL_PhotonsFD_mom_ind.size() == 0));                       // No id. photons in the FD above momentum threshold
        TL_Event_Selection_1e_cut = (TL_Electron_mom_ind.size() == 1 &&
                                          TL_ElectronFD_mom_ind.size() == 1);                                   // One id. electron above momentum threshold
//            TL_Event_Selection_1e_cut = (TL_Electron_mom_ind.size() == 1);                                   // One id. electron above momentum threshold
        TL_Basic_ES = (TL_Event_Selection_1e_cut && no_TL_cPions && no_TL_OtherPart && no_TL_FDpi0 && no_TL_FDPhotons);

        /* Setting up 1p TL event selection */
        one_FDproton_1p = ((TL_Protons_mom_ind.size() == 1) && (TL_ProtonsFD_mom_ind.size() == 1));

        /* Setting up 1n TL event selection */
        one_FDNeutron_1n = (TL_NeutronsFD_mom_ind.size() == 1);
        no_protons_1n = (TL_Protons_mom_ind.size() == 0);

        /* Setting up pFDpCD TL event selection */
//            two_protons_pFDpCD = (TL_Protons_mom_ind.size() == 2);
        one_FDproton_pFDpCD = (TL_ProtonsFD_mom_ind.size() == 1);
        one_CDproton_pFDpCD = (TL_ProtonsCD_mom_ind.size() == 1);

        /* Setting up nFDpCD TL event selection */
        one_FDNeutron_nFDpCD = (TL_NeutronsFD_mom_ind.size() == 1);
        one_proton_nFDpCD = (TL_Protons_mom_ind.size() == 1);
        no_FDproton_nFDpCD = (TL_ProtonsFD_mom_ind.size() == 0);
        one_CDproton_nFDpCD = (TL_ProtonsCD_mom_ind.size() == 1);

        // 1p = one id. FD proton:
        TL_Event_Selection_1p = (TL_Basic_ES && one_FDproton_1p);

        // 1n = one id. FD neutron & no id. protons:
        TL_Event_Selection_1n = (TL_Basic_ES && one_FDNeutron_1n && no_protons_1n);

        // pFDpCD = One id. FD proton & one id. CD proton:
        TL_Event_Selection_pFDpCD = (TL_Basic_ES && one_FDproton_pFDpCD && one_CDproton_pFDpCD);
//            TL_Event_Selection_pFDpCD = (TL_Basic_ES && two_protons_pFDpCD && one_FDproton_pFDpCD && one_CDproton_pFDpCD);

        // nFDpCD = One id. FD neutron & one id. CD proton:
        TL_Event_Selection_nFDpCD = (TL_Basic_ES && one_FDNeutron_nFDpCD && one_proton_nFDpCD && no_FDproton_nFDpCD && one_CDproton_nFDpCD);
        //</editor-fold>

    }
}
