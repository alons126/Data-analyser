//
// Created by alons on 09/07/2023.
//

#include "hData.h"

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleName function">
string hData::GetParticleName(const string &Source) {
    string ParticleName, FS = GetFS(Source);

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
    } else if ((FS == "nFDpCD") || (FS == "pFDpCD")) {
        ParticleName = "Nucleon";
    } else {
        ParticleName = "Unknown";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameLC function">
string hData::GetParticleNameLC(const string &Source) {
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

// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetFS function">
string hData::GetFS(const string &Source) {
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

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetType function">
string hData::GetType(const string &Source) {
    string Type;

    if (findSubstring(Source, "momentum")) { // for momentum efficiency plots
        Type = "momentum";
    } else if (findSubstring(Source, "W ")) { // for theta efficiency plots
        Type = "W";
    } else if (findSubstring(Source, "Q^{2}")) { // for theta efficiency plots
        Type = "Q2";
    } else if (findSubstring(Source, "E_{e}")) { // for theta efficiency plots
        Type = "E_e";
    } else if (findSubstring(Source, "#omega")) { // for theta efficiency plots
        Type = "omega";
    } else if (findSubstring(Source, "E_{cal}")) { // for theta efficiency plots
        Type = "Ecal";
    } else if (findSubstring(Source, "#deltaP_{T,tot}")) { // for theta efficiency plots
        Type = "deltaP_T_tot";
    } else if (findSubstring(Source, "#deltaP_{T,L}")) { // for theta efficiency plots
        Type = "deltaP_T_L";
    } else if (findSubstring(Source, "#delta#alpha_{T,tot}")) { // for theta efficiency plots
        Type = "deltaAlpha_T_tot";
    } else if (findSubstring(Source, "#delta#alpha_{T,L}")) { // for theta efficiency plots
        Type = "deltaAlpha_T_L";
    } else if (findSubstring(Source, "#delta#phi_{T,tot}")) { // for theta efficiency plots
        Type = "deltaPhi_T_tot";
    } else if (findSubstring(Source, "#delta#phi_{T,L}")) { // for theta efficiency plots
        Type = "deltaPhi_T_L";
    } else if (findSubstring(Source, "#theta")) { // for theta efficiency plots
        Type = "theta";
    } else if (findSubstring(Source, "#phi")) { // for phi efficiency plots
        Type = "phi";
    }

    return Type;
}
//</editor-fold>

// GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetDRegion function">
string hData::GetDRegion(const string &Source) {
    string DRegion, Type = GetType(Source), Particle = GetParticleName(Source), ParticleLC = GetParticleNameLC(Source);

    if (findSubstring(Source, ", FD)") || findSubstring(Type, "FD " + Particle) ||
        findSubstring(Source, "FD " + ParticleLC)) {
        DRegion = "FD";
    } else if (findSubstring(Source, ", CD)") || findSubstring(Type, "CD " + Particle) ||
               findSubstring(Source, "CD " + ParticleLC)) {
        DRegion = "CD";
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
        if (Type == "W" || Type == "Q2" || Type == "E_e" || Type == "omega" || Type == "Ecal" || Type == "deltaP_T_tot" || Type == "deltaP_T_L" ||
            Type == "deltaAlpha_T_tot" || Type == "deltaAlpha_T_L" || Type == "deltaPhi_T_tot" || Type == "deltaPhi_T_L") {
            FSRTitle = Type + " ratio - ";
        } else {
            if (Particle == "Electron") {
                if (Type == "momentum") {
                    FSRTitle = Particle + " " + Type + " ratio";
                } else {
                    FSRTitle = Particle + " #" + Type + " ratio";
                }
            } else {
                if (DRegion == "FD") {
                    if (Type == "momentum") {
                        FSRTitle = DRegion + " nucleon " + Type + " ratio";
                    } else {
                        FSRTitle = DRegion + " nucleon #" + Type + " ratio";
                    }
                } else if (DRegion == "CD") {
                    if (Type == "momentum") {
                        FSRTitle = DRegion + " proton " + Type + " ratio";
                    } else {
                        FSRTitle = DRegion + " proton #" + Type + " ratio";
                    }
                }
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
