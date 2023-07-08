//
// Created by alons on 08/07/2023.
//

#include "DEfficiency.h"

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleName function">
string DEfficiency::GetParticleName(string Source) {
    string ParticleName;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleName = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleName = "Electron";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleName = "Proton";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        ParticleName = "Neutron";
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "Piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "Piminus";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleName = "Kplus";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleName = "Kminus";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleName = "Photon";
    } else {
        ParticleName = "Unknown";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameLC function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameLC function">
string DEfficiency::GetParticleNameLC(string Source) {
    string ParticleName;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleName = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleName = "electron";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleName = "proton";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        ParticleName = "neutron";
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "piminus";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleName = "kplus";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleName = "kminus";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleName = "photon";
    } else {
        ParticleName = "unknown";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameShort function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameShort function">
string DEfficiency::GetParticleNameShort(string Source) {
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
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleNameShort = "K^{+}";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleNameShort = "K^{-}";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleNameShort = "Photon";
    } else {
        ParticleNameShort = "#gamma";
    }

    return ParticleNameShort;
}
//</editor-fold>

// GetFS function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetFS function">
string DEfficiency::GetFS(string Source) {
    string FinalState;

    if (findSubstring(Source, "1e_cut") || findSubstring(Source, "1e cut") || findSubstring(Source, "1e Cut")) {
        FinalState = "1e cut";
    } else if (findSubstring(Source, "1p")) {
        FinalState = "1p";
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

    return FinalState;
}
//</editor-fold>

// GetType function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetType function">
string DEfficiency::GetType(string Source) {
    string Type;

    if (findSubstring(Source, "momentum")) { // for momentum ACorrection plots
        Type = "momentum";
    }

    return Type;
}
//</editor-fold>