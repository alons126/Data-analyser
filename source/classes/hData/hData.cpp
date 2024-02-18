//
// Created by alons on 09/07/2023.
//

#include "hData.h"

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleName function">
string hData::GetParticleName(const string &Source, const bool &PluralParticles) {
    string ParticleName, FS = GetFS(Source);

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        if (PluralParticles) {
            ParticleName = "Neut";
        } else {
            ParticleName = "Neut";
        }
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        if (PluralParticles) {
            ParticleName = "Electrons";
        } else {
            ParticleName = "Electron";
        }
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        if (PluralParticles) {
            ParticleName = "Protons";
        } else {
            ParticleName = "Proton";
        }
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        if (PluralParticles) {
            ParticleName = "Neutrons";
        } else {
            ParticleName = "Neutron";
        }
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "Piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "Piminus";
    } else if (findSubstring(Source, "K^{+}")) {
        ParticleName = "Kplus";
    } else if (findSubstring(Source, "K^{-}")) {
        ParticleName = "Kminus";
    } else if (findSubstring(Source, "D")) {
        if (PluralParticles) {
            ParticleName = "Deuterons";
        } else {
            ParticleName = "Deuteron";
        }
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        if (PluralParticles) {
            ParticleName = "Photons";
        } else {
            ParticleName = "Photon";
        }
    } else if ((FS == "nFDpCD") || (FS == "pFDpCD")) {
        ParticleName = "Nucleon";
    } else {
        ParticleName = "Unknown";
    }

    return ParticleName;
}
//</editor-fold>

//<editor-fold desc="GetParticleName1 function">
string hData::GetParticleName1(const string &Source, const bool &PluralParticles) {
    string ParticleName, FS = GetFS(Source);

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        if (PluralParticles) {
            ParticleName = "Neut";
        } else {
            ParticleName = "Neut";
        }
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        if (PluralParticles) {
            ParticleName = "Electrons";
        } else {
            ParticleName = "Electron";
        }
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        if (PluralParticles) {
            ParticleName = "Protons";
        } else {
            ParticleName = "Proton";
        }
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        if (PluralParticles) {
            ParticleName = "Neutrons";
        } else {
            ParticleName = "Neutron";
        }
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "Piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "Piminus";
    } else if (findSubstring(Source, "K^{+}")) {
        ParticleName = "Kplus";
    } else if (findSubstring(Source, "K^{-}")) {
        ParticleName = "Kminus";
    } else if (findSubstring(Source, "D")) {
        if (PluralParticles) {
            ParticleName = "Deuterons";
        } else {
            ParticleName = "Deuteron";
        }
    } else {
        ParticleName = "Deuteron";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameFromSubscript function --------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameFromSubscript function">
string hData::GetParticleNameFromSubscript(const string &Source, const bool &PluralParticles) {
    string ParticleName;

    if (findSubstring(Source, "{e}")) {
        if (PluralParticles) {
            ParticleName = "Electrons";
        } else {
            ParticleName = "Electron";
        }
    } else if (findSubstring(Source, "{p}") ||
               findSubstring(Source, "{pFD}") || findSubstring(Source, "{pCD}")) {
        if (PluralParticles) {
            ParticleName = "Protons";
        } else {
            ParticleName = "Proton";
        }
    } else if (findSubstring(Source, "{n}") || findSubstring(Source, "{nFD}")) {
        if (PluralParticles) {
            ParticleName = "Neutrons";
        } else {
            ParticleName = "Neutron";
        }
    } else if (findSubstring(Source, "{#pi^{+}}")) {
        if (PluralParticles) {
            ParticleName = "Positive pions";
        } else {
            ParticleName = "#pi^{+}";
        }
    } else if (findSubstring(Source, "{#pi^{-}}")) {
        if (PluralParticles) {
            ParticleName = "Negative pions";
        } else {
            ParticleName = "#pi^{-}";
        }
    } else if (findSubstring(Source, "{K^{+}}")) {
        if (PluralParticles) {
            ParticleName = "Positive kaons";
        } else {
            ParticleName = "K^{+}";
        }
    } else if (findSubstring(Source, "{K^{-}}")) {
        if (PluralParticles) {
            ParticleName = "Negative kaons";
        } else {
            ParticleName = "K^{-}";
        }
    } else if (findSubstring(Source, "{D}")) {
        if (PluralParticles) {
            ParticleName = "Deuterons";
        } else {
            ParticleName = "Deuteron";
        }
    } else if (findSubstring(Source, "{#gamma}")) {
        if (PluralParticles) {
            ParticleName = "Photons";
        } else {
            ParticleName = "Photon";
        }
    } else {
        ParticleName = "Unknown";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameLC function">
string hData::GetParticleNameLC(const string &Source, const bool &PluralParticles) {
    string ParticleName;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        if (PluralParticles) {
            ParticleName = "neut";
        } else {
            ParticleName = "neut";
        }
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        if (PluralParticles) {
            ParticleName = "electrons";
        } else {
            ParticleName = "electron";
        }
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        if (PluralParticles) {
            ParticleName = "protons";
        } else {
            ParticleName = "proton";
        }
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        if (PluralParticles) {
            ParticleName = "neutrons";
        } else {
            ParticleName = "neutron";
        }
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "piminus";
    } else if (findSubstring(Source, "K^{+}")) {
        ParticleName = "Kplus";
    } else if (findSubstring(Source, "K^{-}")) {
        ParticleName = "Kminus";
    } else if (findSubstring(Source, "deuteron")) {
        if (PluralParticles) {
            ParticleName = "deuterons";
        } else {
            ParticleName = "deuteron";
        }
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        if (PluralParticles) {
            ParticleName = "photons";
        } else {
            ParticleName = "photon";
        }
    } else {
        ParticleName = "unknown";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameLCFromSubscript function --------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameLCFromSubscript function">
string hData::GetParticleNameLCFromSubscript(const string &Source, const bool &PluralParticles) {
    string ParticleName;

    if (findSubstring(Source, "{e}")) {
        if (PluralParticles) {
            ParticleName = "electrons";
        } else {
            ParticleName = "electron";
        }
    } else if (findSubstring(Source, "{p}") ||
               findSubstring(Source, "{pFD}") || findSubstring(Source, "{pCD}")) {
        if (PluralParticles) {
            ParticleName = "protons";
        } else {
            ParticleName = "proton";
        }
    } else if (findSubstring(Source, "{n}") || findSubstring(Source, "{nFD}")) {
        if (PluralParticles) {
            ParticleName = "neutrons";
        } else {
            ParticleName = "neutron";
        }
    } else if (findSubstring(Source, "{#pi^{+}}")) {
        if (PluralParticles) {
            ParticleName = "positive pions";
        } else {
            ParticleName = "#pi^{+}";
        }
    } else if (findSubstring(Source, "{#pi^{-}}")) {
        if (PluralParticles) {
            ParticleName = "negative pions";
        } else {
            ParticleName = "#pi^{-}";
        }
    } else if (findSubstring(Source, "{K^{+}}")) {
        if (PluralParticles) {
            ParticleName = "positive kaons";
        } else {
            ParticleName = "K^{+}";
        }
    } else if (findSubstring(Source, "{K^{-}}")) {
        if (PluralParticles) {
            ParticleName = "negative kaons";
        } else {
            ParticleName = "K^{-}";
        }
    } else if (findSubstring(Source, "{D}")) {
        if (PluralParticles) {
            ParticleName = "deuterons";
        } else {
            ParticleName = "deuteron";
        }
    } else if (findSubstring(Source, "{#gamma}")) {
        if (PluralParticles) {
            ParticleName = "Photons";
        } else {
            ParticleName = "Photon";
        }
    } else {
        ParticleName = "Unknown";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameShort function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameShort function">
string hData::GetParticleNameShort(const string &Source) {
    string ParticleNameShort;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleNameShort = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleNameShort = "e";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleNameShort = "p";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        ParticleNameShort = "n";
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleNameShort = "#pi^{+}";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleNameShort = "#pi^{-}";
    } else if (findSubstring(Source, "K^{+}")) {
        ParticleNameShort = "K^{+}";
    } else if (findSubstring(Source, "K^{-}")) {
        ParticleNameShort = "K^{-}";
    } else if (findSubstring(Source, "D")) {
        ParticleNameShort = "D";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleNameShort = "Photon";
    } else {
        ParticleNameShort = "";
    }

    return ParticleNameShort;
}
//</editor-fold>

// GetParticleNameShortFromSubscript function ---------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameShortFromSubscript function">
string hData::GetParticleNameShortFromSubscript(const string &Source) {
    string ParticleNameShort;

    if (findSubstring(Source, "{e}")) {
        ParticleNameShort = "e";
    } else if (findSubstring(Source, "{p}")) {
        ParticleNameShort = "p";
    } else if (findSubstring(Source, "{n}")) {
        ParticleNameShort = "n";
    } else if (findSubstring(Source, "{#pi^{+}}")) {
        ParticleNameShort = "#pi^{+}";
    } else if (findSubstring(Source, "{#pi^{-}}")) {
        ParticleNameShort = "#pi^{-}";
    } else if (findSubstring(Source, "{K^{+}}")) {
        ParticleNameShort = "K^{+}";
    } else if (findSubstring(Source, "{K^{-}}")) {
        ParticleNameShort = "K^{-}";
    } else if (findSubstring(Source, "{D}")) {
        ParticleNameShort = "D";
    } else if (findSubstring(Source, "{#gamma}")) {
        ParticleNameShort = "#gamma";
    } else {
        ParticleNameShort = "";
    }

    return ParticleNameShort;
}
//</editor-fold>

// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetFS function">
string hData::GetFS(const string &Source) {
    string FinalState;

    if (findSubstring(Source, "1e_cut") || findSubstring(Source, "1e cut") || findSubstring(Source, "1e Cut") ||
        findSubstring(Source, "(e,e')")) {
        FinalState = "1e cut";
    } else if (findSubstring(Source, "1pFD")) {
        FinalState = "1pFD";
    } else if (findSubstring(Source, "1p")) {
        FinalState = "1p";
    } else if (findSubstring(Source, "1nFD")) {
        FinalState = "1n";
    } else if (findSubstring(Source, "1LnFD")) {
        FinalState = "1LnFD";
    } else if (findSubstring(Source, "1n")) {
        FinalState = "1n";
    } else if (findSubstring(Source, "1n1p")) {
        FinalState = "1n1p";
    } else if (findSubstring(Source, "1e2p")) {
        FinalState = "1e2p";
    } else if (findSubstring(Source, "2p")) {
        FinalState = "2p";
    } else if (findSubstring(Source, "pFDpCD")) {
        FinalState = "pFDpCD";
    } else if (findSubstring(Source, "nFDpCD")) {
        FinalState = "nFDpCD";
    }

//    //<editor-fold desc="Safety check">
//    if (FinalState == "") { cout << "hData::GetFS: finale state is not found (Source = " << Source << ")! Exiting...\n\n", exit(0); }
//    //</editor-fold>

    return FinalState;
}
//</editor-fold>

// GetTopology function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetTopology function">
string hData::GetTopology(const string &Source) {
    string Topology;

    if (findSubstring(Source, "1n") || findSubstring(Source, "1p") ||
        findSubstring(Source, "1nFD") || findSubstring(Source, "1pFD")) {
        Topology = "1N";
    } else if (findSubstring(Source, "1n1p") || findSubstring(Source, "2p") ||
               findSubstring(Source, "nFDpCD") || findSubstring(Source, "pFDpCD")) {
        Topology = "2N";
    }

//    //<editor-fold desc="Safety check">
//    if (Topology == "") { cout << "hData::GetTopology: topology is not found (Source = " << Source << ")! Exiting...\n\n", exit(0); }
//    //</editor-fold>

    return Topology;
}
//</editor-fold>

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetType function">
string hData::GetType(const string &Source) {
    string Type;

    if (!findSubstring(Source, "vs") && !findSubstring(Source, "vs.")
        && !findSubstring(Source, "VS") && !findSubstring(Source, "VS.")) {
        if (findSubstring(Source, "momentum") && !findSubstring(Source, "-momentum") &&
            !findSubstring(Source, "Total") && !findSubstring(Source, "Relative") &&
            !findSubstring(Source, "Leading") && !findSubstring(Source, "Recoil")) {
            Type = "momentum";
        } else if (findSubstring(Source, "momentum") && findSubstring(Source, "Leading")) {
            Type = "leading_nuc_momentum";
        } else if (findSubstring(Source, "momentum") && findSubstring(Source, "Recoil")) {
            Type = "recoil_nuc_momentum";
        } else if (findSubstring(Source, "Total") && findSubstring(Source, "3-momentum")) {
            Type = "total_3momentum";
        } else if (findSubstring(Source, "Relative") && findSubstring(Source, "3-momentum")) {
            Type = "relative_3momentum";
        } else if (findSubstring(Source, "Total") && findSubstring(Source, "4-momentum")) {
            Type = "total_4momentum";
        } else if (findSubstring(Source, "Relative") && findSubstring(Source, "4-momentum")) {
            Type = "relative_4momentum";
        } else if (findSubstring(Source, "#vec{P}_{tot}-#vec{q}")) {
            Type = "P_tot_minus_q";
        } else if (findSubstring(Source, "W ")) {
            Type = "W";
        } else if (findSubstring(Source, "Q^{2}")) {
            Type = "Q2";
        } else if (findSubstring(Source, "E_{e}")) {
            Type = "E_e";
        } else if (findSubstring(Source, "#omega")) {
            Type = "omega";
        } else if (findSubstring(Source, "E_{cal}")) {
            Type = "Ecal";
        } else if (findSubstring(Source, "#deltaP_{T,tot}")) {
            Type = "deltaP_T_tot";
        } else if (findSubstring(Source, "#deltaP_{T,L}")) {
            Type = "deltaP_T_L";
        } else if (findSubstring(Source, "#delta#alpha_{T,tot}")) {
            Type = "deltaAlpha_T_tot";
        } else if (findSubstring(Source, "#delta#alpha_{T,L}")) {
            Type = "deltaAlpha_T_L";
        } else if (findSubstring(Source, "#delta#phi_{T,tot}")) {
            Type = "deltaPhi_T_tot";
        } else if (findSubstring(Source, "#delta#phi_{T,L}")) {
            Type = "deltaPhi_T_L";
        } else if (findSubstring(Source, "#theta") &&
                   !findSubstring(Source, "#theta_{#vec{") &&
                   !(findSubstring(Source, "#theta_{pFD,pCD}") || findSubstring(Source, "#theta_{nFD,pCD}")) &&
                   !(findSubstring(Source, "#theta_{tot}") || findSubstring(Source, "#theta_{rel}"))) {
            Type = "theta";
        } else if (findSubstring(Source, "#phi") &&
                   !(findSubstring(Source, "#phi_{tot}") || findSubstring(Source, "#phi_{rel}"))) {
            Type = "phi";
        } else if (findSubstring(Source, "#theta_{tot}")) {
            Type = "theta_tot";
        } else if (findSubstring(Source, "#phi_{tot}")) {
            Type = "phi_tot";
        } else if (findSubstring(Source, "#theta_{rel}")) {
            Type = "theta_rel";
        } else if (findSubstring(Source, "#phi_{rel}")) {
            Type = "phi_rel";
        } else if (findSubstring(Source, "#theta_{#vec{")) {
            if (findSubstring(Source, "#theta_{#vec{P}_{e},#vec{P}_{tot}}")) {
                Type = "Opening_ang_P_e_P_tot";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{tot}}")) {
                Type = "Opening_ang_q_P_tot";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pFD}}") || findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nFD}}")) {
                Type = "Opening_ang_q_P_nucFD";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pCD}}")) {
                Type = "Opening_ang_q_P_nucCD";
            } else if (findSubstring(Source, "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}}") || findSubstring(Source, "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}}")) {
                Type = "Opening_ang_P_nucL_minus_q_nucR";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pL}}") || findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nL}}")) {
                Type = "Opening_ang_q_P_nucL";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pR}}") || findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nR}}")) {
                Type = "Opening_ang_q_P_nucR";
            }
        } else if (findSubstring(Source, "#theta_{pFD,pCD}") || findSubstring(Source, "#theta_{nFD,pCD}")) {
            Type = "Opening_ang_P_nucFD_P_nucCD";
        } else if (findSubstring(Source, "Total nucleon 3-momentum")) {
            Type = "Total_nucleon_3momentum";
        } else if (findSubstring(Source, "Total nucleon 4-momentum")) {
            Type = "Total_nucleon_4momentum";
        }
    } else {

        if (findSubstring(Source, "P_{pL} vs. P_{pR}") || findSubstring(Source, "P_{nL} vs. P_{nR}")) {
            Type = "P_nucL_vs_P_nucR";
        } else if (findSubstring(Source, "P_{pFD} vs. P_{pCD}") || findSubstring(Source, "P_{nFD} vs. P_{pCD}")) {
            Type = "P_nucFD_vs_P_nucCD";

        } else if (findSubstring(Source, "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|")) {
            Type = "P_tot_vs_P_rel_3v";

        } else if (findSubstring(Source, "P_{tot}^{#mu} vs. P_{rel}^{#mu}")) {
            Type = "P_tot_vs_P_rel_4v";

        } else if (findSubstring(Source, "#theta_{e} vs. P_{e}")) {
            Type = "theta_e_vs_P_e";
        } else if (findSubstring(Source, "#theta_{e} vs. W")) {
            Type = "theta_e_vs_W";
        } else if (findSubstring(Source, "#phi_{e} vs. P_{e}")) {
            Type = "phi_e_vs_P_e";
        } else if (findSubstring(Source, "#phi_{e} vs. W")) {
            Type = "phi_e_vs_W";

        } else if (findSubstring(Source, "#theta_{pFD} vs. P_{pFD}") || findSubstring(Source, "#theta_{nFD} vs. P_{nFD}")) {
            Type = "theta_nucFD_vs_P_nucFD";
        } else if (findSubstring(Source, "#theta_{pFD} vs. W") || findSubstring(Source, "#theta_{nFD} vs. W")) {
            Type = "theta_nucFD_vs_W";

        } else if (findSubstring(Source, "#theta_{pCD} vs. P_{pCD}")) {
            Type = "theta_nucCD_vs_P_nucCD";
        } else if (findSubstring(Source, "#theta_{pCD} vs. W")) {
            Type = "theta_nucCD_vs_W";

        } else if (findSubstring(Source, "#phi_{pFD} vs. P_{pFD}") || findSubstring(Source, "#phi_{nFD} vs. P_{nFD}")) {
            Type = "phi_nucFD_vs_P_nucFD";
        } else if (findSubstring(Source, "#phi_{pFD} vs. W") || findSubstring(Source, "#phi_{nFD} vs. W")) {
            Type = "phi_nucFD_vs_W";

        } else if (findSubstring(Source, "#phi_{pCD} vs. P_{pCD}")) {
            Type = "phi_nucCD_vs_P_nucCD";
        } else if (findSubstring(Source, "#phi_{pCD} vs. W")) {
            Type = "phi_nucCD_vs_W";

        } else if (findSubstring(Source, "#theta_{e} vs. #phi_{e}")) {
            Type = "theta_e_vs_phi_e";
        } else if (findSubstring(Source, "#theta_{pFD} vs. #phi_{pFD}") || findSubstring(Source, "#theta_{nFD} vs. #phi_{nFD}")) {
            Type = "theta_nucFD_vs_theta_nucFD";
        } else if (findSubstring(Source, "#theta_{pCD} vs. #phi_{pCD}")) {
            Type = "theta_nucCD_vs_theta_nucCD";

        } else if (findSubstring(Source, "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W")) {
            Type = "theta_P_e_P_tot_vs_W";
        } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{tot}} vs. W")) {
            Type = "theta_q_P_tot_vs_W";
        } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pL}} vs. W") || findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nL}} vs. W")) {
            Type = "theta_q_P_nucL_vs_W";
        } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pR}} vs. W") || findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nR}} vs. W")) {
            Type = "theta_q_P_nucR_vs_W";
        } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pFD}} vs. W") || findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nFD}} vs. W")) {
            Type = "theta_q_P_nucFD_vs_W";
        } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pCD}} vs. W") || findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nCD}} vs. W")) {
            Type = "theta_q_P_nucCD_vs_W";

        } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}|") ||
                   findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nL}} vs. r_{nL}=|#vec{P}_{nL}|/|#vec{q}|")) {
            Type = "theta_q_P_nucCD_vs_r_nucL";
        } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}}") ||
                   findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nL}} vs. #theta_{#vec{q},#vec{P}_{nR}}")) {
            Type = "theta_q_P_nucL_vs_theta_q_P_nucR";
        } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}") ||
                   findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}")) {
            Type = "theta_q_P_nucFD_vs_theta_q_P_nucCD";

        } else if (findSubstring(Source, "#theta_{pFD,pCD} vs. W") || findSubstring(Source, "#theta_{nFD,pCD} vs. W")) {
            Type = "theta_nucFD_nucCD_vs_W";

        } else if (findSubstring(Source, "Q^{2} vs. W")) {
            Type = "Q2_vs_W";

        } else if (findSubstring(Source, "E_{e} vs. #theta_{e}")) {
            Type = "E_e_vs_theta_e";

        } else if (findSubstring(Source, "E_{cal} vs. #delta#alpha_{T,L}")) {
            Type = "E_cal_vs_dalpha_T_L";
        } else if (findSubstring(Source, "E_{cal} vs. #delta#alpha_{T,tot}")) {
            Type = "E_cal_vs_dalpha_T_tot";
        } else if (findSubstring(Source, "E_{cal} vs. #deltaP_{T,L}")) {
            Type = "E_cal_vs_dP_T_L";
        } else if (findSubstring(Source, "E_{cal} vs. #deltaP_{T,tot}")) {
            Type = "E_cal_vs_dP_T_tot";

        } else if (findSubstring(Source, "E_{cal} vs. W")) {
            Type = "E_cal_vs_W";

        } else if (findSubstring(Source, "#deltaP_{T,L} vs. #delta#alpha_{T,L}")) {
            Type = "dP_T_L_vs_dalpha_T_L";
        } else if (findSubstring(Source, "#deltaP_{T,tot} vs. #delta#alpha_{T,tot}")) {
            Type = "dP_T_tot_vs_dalpha_T_tot";
        } else if (findSubstring(Source, "#deltaP_{T,L} vs. W")) {
            Type = "dP_T_L_vs_W";
        } else if (findSubstring(Source, "#deltaP_{T,tot} vs. W")) {
            Type = "dP_T_tot_vs_W";
        } else if (findSubstring(Source, "#delta#alpha_{T,L} vs. W")) {
            Type = "dalpha_T_L_vs_W";
        } else if (findSubstring(Source, "#delta#alpha_{T,tot} vs. W")) {
            Type = "dalpha_T_tot_vs_W";
        }
    }

    return Type;
}
//</editor-fold>

// GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetDRegion function">
string hData::GetDRegion(const string &Source, const bool &ReturnGoingRegion) {
    string DRegion, Type = GetType(Source), Particle = GetParticleName(Source), ParticleLC = GetParticleNameLC(Source);

    if (findSubstring(Source, ", FD)") || findSubstring(Type, "FD " + Particle) ||
        findSubstring(Source, "FD " + ParticleLC)) {
        if (ReturnGoingRegion) {
            DRegion = "forward-going";
        } else {
            DRegion = "FD";
        }
    } else if (findSubstring(Source, ", CD)") || findSubstring(Type, "CD " + Particle) ||
               findSubstring(Source, "CD " + ParticleLC)) {
        if (ReturnGoingRegion) {
            DRegion = "central-going";
        } else {
            DRegion = "CD";
        }
    }

    return DRegion;
}
//</editor-fold>

// GetDRegionExplicit function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetDRegionExplicit function">
string hData::GetDRegionExplicit(const string &Source, const bool &ReturnGoingRegion) {
    string DRegion, Type = GetType(Source), Particle = GetParticleName(Source), ParticleLC = GetParticleNameLC(Source);

    if (findSubstring(Source, "FD")) {
        if (ReturnGoingRegion) {
            DRegion = "forward-going";
        } else {
            DRegion = "FD";
        }
    } else if (findSubstring(Source, "CD")) {
        if (ReturnGoingRegion) {
            DRegion = "central-going";
        } else {
            DRegion = "CD";
        }
    }

    return DRegion;
}
//</editor-fold>

// GetFSRTitle function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetFSRTitle function">
string hData::GetFSRTitle(const string &Source, const string &PlotsT) {
    string FSRTitle;

    string Particle = GetParticleName(Source), ParticleShort = GetParticleNameShort(Source), Type = GetType(Source), DRegion = GetDRegion(Source);

    if (PlotsT == "FSRatio") {
        if (!findSubstring(Source, "vs") && !findSubstring(Source, "vs.") &&
            !findSubstring(Source, "VS") && !findSubstring(Source, "VS.")) {
            if (Type == "W" || Type == "Q2" || Type == "E_e" || Type == "omega" || Type == "Ecal" || Type == "deltaP_T_tot" || Type == "deltaP_T_L" ||
                Type == "deltaAlpha_T_tot" || Type == "deltaAlpha_T_L" || Type == "deltaPhi_T_tot" || Type == "deltaPhi_T_L" ||
                Type == "Opening_ang_P_e_P_tot" || Type == "Opening_ang_q_P_tot" || Type == "Opening_ang_q_P_nucFD" || Type == "Opening_ang_q_P_nucCD" ||
                Type == "Opening_ang_q_P_nucL" || Type == "Opening_ang_q_P_nucR" || Type == "Opening_ang_P_nucFD_P_nucCD") {
                if (Type == "Q2") {
                    FSRTitle = "Q^{2} ratio - ";
                } else if (Type == "E_e") {
                    FSRTitle = "E_{e} ratio - ";
                } else if (Type == "omega") {
                    FSRTitle = "#omega ratio - ";
                } else if (Type == "Ecal") {
                    FSRTitle = "E_{cal} ratio - ";
                } else if (Type == "deltaP_T_tot") {
                    FSRTitle = "#deltaP_{T,tot} ratio - ";
                } else if (Type == "deltaP_T_L") {
                    FSRTitle = "#deltaP_{T,L} ratio - ";
                } else if (Type == "deltaAlpha_T_tot") {
                    FSRTitle = "#delta#alpha_{T,tot} ratio - ";
                } else if (Type == "deltaAlpha_T_L") {
                    FSRTitle = "#delta#alpha_{T,L} ratio - ";
                } else if (Type == "deltaPhi_T_tot") {
                    FSRTitle = "#delta#phi_{T,tot} ratio - ";
                } else if (Type == "deltaPhi_T_L") {
                    FSRTitle = "#delta#phi_{T,L} ratio - ";
                } else if (Type == "Opening_ang_P_e_P_tot") {
                    FSRTitle = "Opening angle between P_{e} and P_{tot} ratio - ";
                } else if (Type == "Opening_ang_q_P_tot") {
                    FSRTitle = "Opening angle between q and P_{tot} ratio - ";
                } else if (Type == "Opening_ang_q_P_nucFD") {
                    FSRTitle = "Opening angle between q and P_{nuc,FD} ratio - ";
                } else if (Type == "Opening_ang_q_P_nucCD") {
                    FSRTitle = "Opening angle between q and P_{nuc,CD} ratio - ";
                } else if (Type == "Opening_ang_q_P_nucL") {
                    FSRTitle = "Opening angle between q and leading nucleon P_{nucL} ratio - ";
                } else if (Type == "Opening_ang_q_P_nucR") {
                    FSRTitle = "Opening angle between q and recoil nucleon P_{nucR} ratio - ";
                } else if (Type == "Opening_ang_P_nucFD_P_nucCD") {
                    FSRTitle = "Opening angle between FD and CD nucleons ratio - ";
                } else {
                    FSRTitle = Type + " ratio - ";
                }
            } else if (Type == "leading_nuc_momentum") {
                FSRTitle = "Leading nucleon momentum ratio- ";
            } else if (Type == "recoil_nuc_momentum") {
                FSRTitle = "Recoil nucleon momentum ratio - ";
            } else if (Type == "total_3momentum") {
                FSRTitle = "Total nucleon 3-momentum ratio - ";
            } else if (Type == "relative_3momentum") {
                FSRTitle = "Relative nucleon 3-momentum ratio - ";
            } else if (Type == "total_4momentum") {
                FSRTitle = "Total nucleon 4-momentum ratio - ";
            } else if (Type == "relative_4momentum") {
                FSRTitle = "Relative nucleon 4-momentum ratio - ";
            } else if (Type == "theta_tot") {
                FSRTitle = "#theta_{tot} of total 3-momentum ratio - ";
            } else if (Type == "phi_tot") {
                FSRTitle = "#phi_{tot} of total 3-momentum ratio - ";
            } else if (Type == "theta_rel") {
                FSRTitle = "#theta_{rel} of relative 3-momentum ratio - ";
            } else if (Type == "phi_rel") {
                FSRTitle = "#phi_{rel} of relative 3-momentum ratio - ";
            } else if (Type == "P_tot_minus_q") {
                FSRTitle = "#vec{P}_{tot}-#vec{q} ratio - ";
            } else if (Type == "Opening_ang_P_nucL_minus_q_nucR") {
                FSRTitle = "#theta_{#vec{P}_{nucL}-#vec{q},#vec{P}_{nucR}} ratio - ";
            } else {
                if (Particle == "Electron") {
                    if (Type == "momentum") {
                        FSRTitle = Particle + " " + Type + " ratio - ";
                    } else {
                        FSRTitle = Particle + " #" + Type + " ratio - ";
                    }
                } else {
                    if (DRegion == "FD") {
                        if (Type == "momentum") {
                            FSRTitle = DRegion + " nucleon " + Type + " ratio - ";
                        } else {
                            FSRTitle = DRegion + " nucleon #" + Type + " ratio - ";
                        }
                    } else if (DRegion == "CD") {
                        if (Type == "momentum") {
                            FSRTitle = DRegion + " proton " + Type + " ratio - ";
                        } else {
                            FSRTitle = DRegion + " proton #" + Type + " ratio - ";
                        }
                    }
                }
            }
        } else {
            if (findSubstring(Source, "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|")) {
                FSRTitle = "|#vec{P}_{tot}| vs. |#vec{P}_{rel}| ratio - ";
            } else if (findSubstring(Source, "P_{tot}^{#mu} vs. P_{rel}^{#mu}")) {
                FSRTitle = "P_{tot}^{#mu} vs. P_{rel}^{#mu} ratio - ";
            } else if (findSubstring(Source, "P_{pL} vs. P_{pR}") || findSubstring(Source, "P_{nL} vs. P_{nR}")) {
                FSRTitle = "P_{nucL} vs. P_{nucR} ratio - ";
            } else if (findSubstring(Source, "P_{pFD} vs. P_{pCD}") || findSubstring(Source, "P_{nFD} vs. P_{pCD}")) {
                FSRTitle = "P_{nucFD} vs. P_{nucCD} ratio - ";
            } else if (findSubstring(Source, "#theta_{e} vs. P_{e}")) {
                FSRTitle = "#theta_{e} vs. P_{e} ratio - ";
            } else if (findSubstring(Source, "#theta_{e} vs. W")) {
                FSRTitle = "#theta_{e} vs. W ratio - ";
            } else if (findSubstring(Source, "#phi_{e} vs. P_{e}")) {
                FSRTitle = "#phi_{e} vs. P_{e} ratio - ";
            } else if (findSubstring(Source, "#phi_{e} vs. W")) {
                FSRTitle = "#phi_{e} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{pFD} vs. P_{pFD}") || findSubstring(Source, "#theta_{nFD} vs. P_{nFD}")) {
                FSRTitle = "#theta_{nucFD} vs. P_{nucFD}";
            } else if (findSubstring(Source, "#theta_{pFD} vs. W") || findSubstring(Source, "#theta_{nFD} vs. W")) {
                FSRTitle = "#theta_{nucFD} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{pCD} vs. P_{pCD}")) {
                FSRTitle = "#theta_{nucCD} vs. P_{nucCD}";
            } else if (findSubstring(Source, "#theta_{pCD} vs. W")) {
                FSRTitle = "#theta_{nucCD} vs. W ratio - ";
            } else if (findSubstring(Source, "#phi_{pFD} vs. P_{pFD}") || findSubstring(Source, "#phi_{nFD} vs. P_{nFD}")) {
                FSRTitle = "#phi_{nucFD} vs. P_{nucFD}";
            } else if (findSubstring(Source, "#phi_{pFD} vs. W") || findSubstring(Source, "#phi_{nFD} vs. W")) {
                FSRTitle = "#phi_{nucFD} vs. W ratio - ";
            } else if (findSubstring(Source, "#phi_{pCD} vs. P_{pCD}")) {
                FSRTitle = "#phi_{nucCD} vs. P_{nucCD}";
            } else if (findSubstring(Source, "#phi_{pCD} vs. W")) {
                FSRTitle = "#phi_{nucCD} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{e} vs. #phi_{e}")) {
                FSRTitle = "#theta_{e} vs. #phi_{e} ratio - ";
            } else if (findSubstring(Source, "#theta_{pFD} vs. #phi_{pFD}") || findSubstring(Source, "#theta_{nFD} vs. #phi_{nFD}")) {
                FSRTitle = "#theta_{nucFD} vs. #phi_{nucFD} ratio - ";
            } else if (findSubstring(Source, "#theta_{pCD} vs. #phi_{pCD}")) {
                FSRTitle = "#theta_{nucCD} vs. #phi_{nucCD} ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W")) {
                FSRTitle = "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{tot}} vs. W")) {
                FSRTitle = "#theta_{#vec{q},#vec{P}_{tot}} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pL}} vs. W") ||
                       findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nL}} vs. W")) {
                FSRTitle = "#theta_{#vec{q},#vec{P}_{nucL}} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pR}} vs. W") ||
                       findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nR}} vs. W")) {
                FSRTitle = "#theta_{#vec{q},#vec{P}_{nucR}} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pFD}} vs. W") ||
                       findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nFD}} vs. W")) {
                FSRTitle = "#theta_{#vec{q},#vec{P}_{nucFD}} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pCD}} vs. W") ||
                       findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nCD}} vs. W")) {
                FSRTitle = "#theta_{#vec{q},#vec{P}_{nucCD}} vs. W ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}|") ||
                       findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nL}} vs. r_{nL}=|#vec{P}_{nL}|/|#vec{q}|")) {
                FSRTitle = "#theta_{#vec{q},#vec{P}_{nucL}} vs. r_{pL}=|#vec{P}_{nucL}|/|#vec{q}| ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}}") ||
                       findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nL}} vs. #theta_{#vec{q},#vec{P}_{nR}}")) {
                FSRTitle = "#theta_{#vec{q},#vec{P}_{nucL}} vs. #theta_{#vec{q},#vec{P}_{nucR}} ratio - ";
            } else if (findSubstring(Source, "#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}") ||
                       findSubstring(Source, "#theta_{#vec{q},#vec{P}_{nFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}}")) {
                FSRTitle = "#theta_{#vec{q},#vec{P}_{nucFD}} vs. #theta_{#vec{q},#vec{P}_{nucCD}} ratio - ";
            } else if (findSubstring(Source, "#theta_{pFD,pCD} vs. W") || findSubstring(Source, "#theta_{nFD,pCD} vs. W")) {
                FSRTitle = "#theta_{nucFD,nucCD} vs. W ratio - ";
            } else if (findSubstring(Source, "Q^{2} vs. W")) {
                FSRTitle = "Q^{2} vs. W ratio - ";
            } else if (findSubstring(Source, "E_{e} vs. #theta_{e}")) {
                FSRTitle = "E_{e} vs. #theta_{e} ratio - ";
            } else if (findSubstring(Source, "E_{cal} vs. #delta#alpha_{T,L}")) {
                FSRTitle = "E_{cal} vs. #delta#alpha_{T,L} ratio - ";
            } else if (findSubstring(Source, "E_{cal} vs. #delta#alpha_{T,tot}")) {
                FSRTitle = "E_{cal} vs. #delta#alpha_{T,tot} ratio - ";
            } else if (findSubstring(Source, "E_{cal} vs. #deltaP_{T,L}")) {
                FSRTitle = "E_{cal} vs. #deltaP_{T,L} ratio - ";
            } else if (findSubstring(Source, "E_{cal} vs. #deltaP_{T,tot}")) {
                FSRTitle = "E_{cal} vs. #deltaP_{T,tot} ratio - ";
            } else if (findSubstring(Source, "E_{cal} vs. W")) {
                FSRTitle = "E_{cal} vs. W ratio - ";
            } else if (findSubstring(Source, "#deltaP_{T,L} vs. #delta#alpha_{T,L}")) {
                FSRTitle = "#deltaP_{T,L} vs. #delta#alpha_{T,L} ratio - ";
            } else if (findSubstring(Source, "#deltaP_{T,tot} vs. #delta#alpha_{T,tot}")) {
                FSRTitle = "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} ratio - ";
            } else if (findSubstring(Source, "#deltaP_{T,L} vs. W")) {
                FSRTitle = "#deltaP_{T,L} vs. W ratio - ";
            } else if (findSubstring(Source, "#deltaP_{T,tot} vs. W")) {
                FSRTitle = "#deltaP_{T,tot} vs. W ratio - ";
            } else if (findSubstring(Source, "#delta#alpha_{T,L} vs. W")) {
                FSRTitle = "#delta#alpha_{T,L} vs. W ratio - ";
            } else if (findSubstring(Source, "#delta#alpha_{T,tot} vs. W")) {
                FSRTitle = "#delta#alpha_{T,tot} vs. W ratio - ";
            }
        }
    } else {
        if (Type == "momentum") {
            FSRTitle = Particle + " " + Type;
        } else {
            FSRTitle = "#" + Type + "_{" + ParticleShort + "}";
        }
    }

    return FSRTitle;
}
//</editor-fold>

// GetTitle function ----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetTitle function">
string hData::GetTitle(const string &Source) {
    string Title, Type = GetType(Source), ParicleShort = GetParticleNameShort(Source), Paricle = GetParticleName(Source);

    if (Type == "momentum") {
        Title = Paricle + " " + Type;
    } else {
        Title = "#" + Type + "_{" + ParicleShort + "}";
    }

    return Title;
}
//</editor-fold>

// GetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetStatsTitle function">
string hData::GetStatsTitle(const string &Source) {
    string StatsTitle, StatsType;

    string FS = GetFS(Source), Type = GetType(Source), ParticleShort = GetParticleNameShort(Source);

    if (findSubstring(Source, "#theta")) { // for momentum ACorrection plots
        StatsType = "#theta_{" + ParticleShort + "}";
    } else if (findSubstring(Source, "#phi")) { // for momentum ACorrection plots
        StatsType = "#phi_{" + ParticleShort + "}";
    }

    StatsTitle = StatsType + " (" + FS + ")";

    return StatsTitle;
}
//</editor-fold>

// SetXLabel function ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetXLabel function">
string hData::SetXLabel(const string &Source) {
    string XLabel, ParicleShort = GetParticleNameShort(Source);

    if (findSubstring(Source, "momentum")) {
        XLabel = "P_{" + ParicleShort + "} [GeV/c]";
    } else if (findSubstring(Source, "#theta")) {
        XLabel = "#theta_{" + ParicleShort + "} [Deg]";
    } else if (findSubstring(Source, "#phi")) {
        XLabel = "#phi_{" + ParicleShort + "} [Deg]";
    }

    return XLabel;
}
//</editor-fold>

// SetSaveDir function --------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetSaveDir function">
string hData::SetSaveDir(const string &Source, const string &BaseSaveDir, const string &Mod) {
    string SaveDir, TestSaveDir;

    string Particle = GetParticleName(Source), ParticleLC = GetParticleNameLC(Source), FS = GetFS(Source), Type = GetType(Source);

    if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        SaveDir = BaseSaveDir + "/00_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
        TestSaveDir = SaveDir + "Cloned_hist_test/";
    } else {
        if (findSubstring(Source, ", FD)") || findSubstring(Source, "FD " + Particle) || findSubstring(Source, "FD " + ParticleLC)) {
            SaveDir = BaseSaveDir + "/01_FD_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        } else if (findSubstring(Source, ", CD)") || findSubstring(Source, "CD " + Particle) || findSubstring(Source, "CD " + ParticleLC)) {
            SaveDir = BaseSaveDir + "/02_CD_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        } else {
            SaveDir = BaseSaveDir + "/" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
            TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
        }
    }

    if ((Mod == "test") || (Mod == "Test")) {
        system(("mkdir -p " + TestSaveDir).c_str());

        return TestSaveDir;
    } else {
        system(("mkdir -p " + SaveDir).c_str());

        return SaveDir;
    }
}
//</editor-fold>

// ReplaceSubStr function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReplaceSubStr function">
void hData::ReplaceSubStr(string &str, const string &subStr, const string &replacement) {
    bool PrintOut = false;

    if (PrintOut) { cout << "\nstr0 = " << str << "\n"; }

    string TempSource = str;

    size_t pos = TempSource.find(subStr);

    // Iterate till index position of substring is valid:
    while (pos != std::string::npos) {
        // Replace the first occurrence of substring in string from position pos onwards:
        TempSource.replace(pos, subStr.length(), replacement);

        // Get the index position of next occurrence of substring in string:
        pos = TempSource.find(subStr, pos + replacement.length());
    }

    str = TempSource;

    if (PrintOut) { cout << "str1 = " << str << "\n"; }
}
//</editor-fold>
