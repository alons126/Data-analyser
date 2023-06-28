//
// Created by alons on 22/06/2023.
//

#include "AMaps.h"

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TROOT.h>

// AMaps constructors ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="AMaps generation constructor">
AMaps::AMaps(double beamE, const string &SavePath, int nOfMomBins, int hbNumOfXBins, int hbNumOfYBins) {
    BinSavePath = SavePath;
    hBinNumOfXBins = hbNumOfXBins;
    hBinNumOfYBins = hbNumOfYBins;
    NumberOfMomBins = nOfMomBins;

    string SavePathAMapsBC = BinSavePath + "00b_AMaps_BC_from_class/";
    system(("mkdir -p " + SavePathAMapsBC).c_str());

    string BinSavePathTLElectron = SavePath + "01a_Electron_TL_Hit_Maps/";
    system(("mkdir -p " + BinSavePathTLElectron).c_str());
    string BinSavePathRecoElectron = SavePath + "01b_Electron_Reco_Hit_Maps/";
    system(("mkdir -p " + BinSavePathRecoElectron).c_str());
    string BinSavePathRecoToTLElectron = SavePath + "01c_Electron_RecoToTL_Ratio/";
    system(("mkdir -p " + BinSavePathRecoToTLElectron).c_str());
    string BinSavePathSepAMapsElectron = BinSavePath + "01d_Electron_Separate_AMaps/";
    system(("mkdir -p " + BinSavePathSepAMapsElectron).c_str());

    string BinSavePathTLProton = SavePath + "02a_Proton_TL_Hit_Maps/";
    system(("mkdir -p " + BinSavePathTLProton).c_str());
    string BinSavePathRecoProton = SavePath + "02b_Proton_Reco_Hit_Maps/";
    system(("mkdir -p " + BinSavePathRecoProton).c_str());
    string BinSavePathRecoToTLProton = SavePath + "02c_Proton_RecoToTL_Ratio/";
    system(("mkdir -p " + BinSavePathRecoToTLProton).c_str());
    string BinSavePathSepAMapsProton = BinSavePath + "02d_Proton_Separate_AMaps/";
    system(("mkdir -p " + BinSavePathSepAMapsProton).c_str());

    string BinSavePathTLNeutron = SavePath + "03a_Neutron_TL_Hit_Maps/";
    system(("mkdir -p " + BinSavePathTLNeutron).c_str());
    string BinSavePathRecoNeutron = SavePath + "03b_Neutron_Reco_Hit_Maps/";
    system(("mkdir -p " + BinSavePathRecoNeutron).c_str());
    string BinSavePathRecoToTLNeutron = SavePath + "03c_Neutron_RecoToTL_Ratio/";
    system(("mkdir -p " + BinSavePathRecoToTLNeutron).c_str());

    string BinSavePathAMap = BinSavePath + "04_Finalized_AMaps/";
    system(("mkdir -p " + BinSavePathAMap).c_str());

    SetBins(beamE);
//    SetBins(beamE, nOfMomBins);

    //<editor-fold desc="Acceptance maps BC">
    string hStatsTitleAMapBCElectron = "Electron_AMap_BC", hTitleAMapBCElectron = "Electron AMap BC", hSaveNameAMapBCElectron = "01_e_AMap_BC";
    ElectronAMapBC = hPlot2D("", "", hStatsTitleAMapBCElectron, hTitleAMapBCElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]", SavePathAMapsBC, hSaveNameAMapBCElectron,
                             hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);

    string hStatsTitleAMapBCProton = "Proton_AMap_BC", hTitleAMapBCProton = "Proton AMap BC", hSaveNameAMapBCProton = "02_p_AMap_BC";
    ProtonAMapBC = hPlot2D("", "", hStatsTitleAMapBCProton, hTitleAMapBCProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]", SavePathAMapsBC, hSaveNameAMapBCProton,
                           hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);

    string hStatsTitleAMapBCNeutron = "Neutron_AMap_BC", hTitleAMapBCNeutron = "Neutron AMap BC", hSaveNameAMapBCNeutron = "03_n_AMap_BC";
    NeutronAMapBC = hPlot2D("", "", hStatsTitleAMapBCNeutron, hTitleAMapBCNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", SavePathAMapsBC, hSaveNameAMapBCNeutron,
                            hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);

    string hStatsTitleAMapBCNucleon = "Nucleon_AMap_BC", hTitleAMapBCNucleon = "Nucleon AMap BC", hSaveNameAMapBCNucleon = "04_Nucleon_AMap_BC";
    NucleonAMapBC = hPlot2D("", "", hStatsTitleAMapBCNucleon, hTitleAMapBCNucleon, "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]", SavePathAMapsBC, hSaveNameAMapBCNucleon,
                            hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
    //</editor-fold>

    for (int i = 0; i < PBinsLimits.size(); i++) {
        double BinLowerLim = PBinsLimits.at(i).at(0), BinUpperLim = PBinsLimits.at(i).at(1);

        int BinUpperLimPrecision;
        if (BinUpperLim == beamE) { BinUpperLimPrecision = 3; } else { BinUpperLimPrecision = 2; }

        string BinDensity = " (" + to_string(hBinNumOfXBins) + "x" + to_string(hBinNumOfYBins) + ")";

        //<editor-fold desc="Setting electron hit maps">

        //<editor-fold desc="Electron TL hit map">
        string hStatsTitleTLElectron = "TL P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleTLElectron = "TL P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameTLElectron = to_string(i + 1) + "_TL_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinTLElectron = hPlot2D("", "", hStatsTitleTLElectron, hTitleTLElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]", BinSavePathTLElectron,
                                          hSaveNameTLElectron,
                                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ElectronTLBinHitMaps.push_back(hPBinTLElectron);
        //</editor-fold>

        //<editor-fold desc="Electron Reco. hit maps">
        string hStatsTitleRecoElectron = "Reco P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                         to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoElectron = "Reco P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                    to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoElectron = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoElectron = hPlot2D("", "", hStatsTitleRecoElectron, hTitleRecoElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]", BinSavePathRecoElectron,
                                            hSaveNameRecoElectron,
                                            hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ElectronRecoBinHitMaps.push_back(hPBinRecoElectron);
        //</editor-fold>

        //<editor-fold desc="Electron Reco./TL Ratio">
        string hStatsTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                             to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoToTLRatioElectron = to_string(i + 1) + "_e_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioElectron = hPlot2D("", "", hStatsTitleRecoToTLRatioElectron, hTitleRecoToTLRatioElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                                     BinSavePathRecoToTLElectron,
                                                     hSaveNameRecoToTLRatioElectron,
                                                     hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ElectronRecoToTLRatio.push_back(hPBinRecoToTLRatioElectron);
        //</editor-fold>

        //<editor-fold desc="Electron separate AMaps">
        string hStatsTitleSepAMapsElectron = "Electron AMap for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                             to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                             to_string_with_precision(Charged_particle_min_Ratio, 2);
        string hTitleSepAMapsElectron = "Electron AMap for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                        to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                        to_string_with_precision(Charged_particle_min_Ratio, 2);
        string hSaveNameSepAMapsElectron = to_string(i + 1) + "_e_SepAMap_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                           to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinSepAMapsElectron = hPlot2D("", "", hStatsTitleSepAMapsElectron, hTitleSepAMapsElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                                BinSavePathSepAMapsElectron,
                                                hSaveNameSepAMapsElectron,
                                                hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ElectronSepAMaps.push_back(hPBinSepAMapsElectron);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Setting proton hit maps">

        //<editor-fold desc="Proton TL hit maps">
        string hStatsTitleTLProton = "TL P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleTLProton = "TL P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameTLProton = to_string(i + 1) + "_TL_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                   to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinTLProton = hPlot2D("", "", hStatsTitleTLProton, hTitleTLProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]", BinSavePathTLProton, hSaveNameTLProton,
                                        hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ProtonTLBinHitMaps.push_back(hPBinTLProton);
        //</editor-fold>

        //<editor-fold desc="Proton Reco. hit maps">
        string hStatsTitleRecoProton = "Reco P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoProton = "Reco P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoProton = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoProton = hPlot2D("", "", hStatsTitleRecoProton, hTitleRecoProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]", BinSavePathRecoProton,
                                          hSaveNameRecoProton,
                                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ProtonRecoBinHitMaps.push_back(hPBinRecoProton);
        //</editor-fold>

        //<editor-fold desc="Proton Reco./TL Ratio">
        string hStatsTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                           to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoToTLRatioProton = to_string(i + 1) + "_p_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                              to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioProton = hPlot2D("", "", hStatsTitleRecoToTLRatioProton, hTitleRecoToTLRatioProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                                                   BinSavePathRecoToTLProton,
                                                   hSaveNameRecoToTLRatioProton,
                                                   hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ProtonRecoToTLRatio.push_back(hPBinRecoToTLRatioProton);
        //</editor-fold>

        //<editor-fold desc="Proton separate AMaps">
        string hStatsTitleSepAMapsProton = "Proton AMap for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                           to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                           to_string_with_precision(Charged_particle_min_Ratio, 2);
        string hTitleSepAMapsProton = "Proton AMap for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                      to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                      to_string_with_precision(Charged_particle_min_Ratio, 2);
        string hSaveNameSepAMapsProton = to_string(i + 1) + "_p_SepAMap_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                         to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinSepAMapsProton = hPlot2D("", "", hStatsTitleSepAMapsProton, hTitleSepAMapsProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                                              BinSavePathSepAMapsProton,
                                              hSaveNameSepAMapsProton,
                                              hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ProtonSepAMaps.push_back(hPBinSepAMapsProton);
        //</editor-fold>

        //</editor-fold>

    }

    //<editor-fold desc="Setting neutron hit maps">

    //<editor-fold desc="Neutron TL hit maps">
    string hStatsTitleTLNeutron = "TL Neutron Hit Map", hTitleTLNeutron = "TL Neutron Hit Map", hSaveNameTLNeutron = "TL_n_Hit_Map";
    NeutronTLHitMap = hPlot2D("", "", hStatsTitleTLNeutron, hTitleTLNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", BinSavePathTLNeutron, hSaveNameTLNeutron,
                              hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Neutron Reco. hit maps">
    string hStatsTitleRecoNeutron = "Reco Neutron Hit Map", hTitleRecoNeutron = "Reco Neutron Hit Map", hSaveNameRecoNeutron = "Reco_n_Hit_Map";
    NeutronRecoHitMap = hPlot2D("", "", hStatsTitleRecoNeutron, hTitleRecoNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", BinSavePathRecoNeutron,
                                hSaveNameRecoNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Neutron Reco./TL Ratio">
    string hStatsTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hSaveNameRecoToTLNeutron = "Neutron_Ratio";
    NeutronRecoToTLRatio = hPlot2D("", "", hStatsTitleRecoToTLNeutron, hTitleRecoToTLNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", BinSavePathRecoToTLNeutron,
                                   hSaveNameRecoToTLNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Finalized acceptance maps">
    string hStatsTitleAMapElectron = "Electron_AMap";
    string hTitleAMapElectron = "Electron AMap for (Reco./TL)#geq" + to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapElectron = "01_e_AMap";
    ElectronAMap = hPlot2D("", "", hStatsTitleAMapElectron, hTitleAMapElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]", BinSavePathAMap, hSaveNameAMapElectron,
                           hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);

    string hStatsTitleAMapProton = "Proton_AMap";
    string hTitleAMapProton = "Proton AMap for (Reco./TL)#geq" + to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapProton = "02_p_AMap";
    ProtonAMap = hPlot2D("", "", hStatsTitleAMapProton, hTitleAMapProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]", BinSavePathAMap, hSaveNameAMapProton, hBinLowerXLim,
                         hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);

    string hStatsTitleAMapNeutron = "Neutron AMap for (Reco./TL)#geq" + to_string_with_precision(Neutral_particle_min_Ratio, 2);
    string hTitleAMapNeutron = "Neutron AMap for (Reco./TL)#geq" + to_string_with_precision(Neutral_particle_min_Ratio, 2);
    string hSaveNameAMapNeutron = "03_n_AMap";
    NeutronAMap = hPlot2D("", "", hStatsTitleAMapNeutron, hTitleAMapNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", BinSavePathAMap, hSaveNameAMapNeutron, hBinLowerXLim,
                          hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);

    string hStatsTitleAMapNucleon = "Nucleon_AMap";
    string hTitleAMapNucleon = "Nucleon AMap for (Reco./TL)_{n}#geq" + to_string_with_precision(Neutral_particle_min_Ratio, 2) + " and (Reco./TL)_{c}#geq" +
                               to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapNucleon = "04_Nucleon_AMap";
    NucleonAMap = hPlot2D("", "", hStatsTitleAMapNucleon, hTitleAMapNucleon, "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]", BinSavePathAMap, hSaveNameAMapNucleon,
                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
    //</editor-fold>

}
//</editor-fold>

//<editor-fold desc="AMaps loading constructor">
AMaps::AMaps(const string &RefrenceHitMapsDirectory, const string &SampleName) {
    ReadHitMaps(RefrenceHitMapsDirectory, SampleName);
}
//</editor-fold>

// SetBins function -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetBins function">
void AMaps::SetBins(double beamE) {
    double InvertedPLowerLim = (1 / beamE);
    double InvertedPUpper = (1 / MomBinTh);
    double Delta = (InvertedPUpper - InvertedPLowerLim) / NumberOfMomBins;

    bool InvertedPrintOut = false;
    bool RegPrintOut = false;

    for (int i = 0; i < NumberOfMomBins; i++) {
        double InvertedBinLower = InvertedPLowerLim + i * Delta;
        double InvertedBinUpper = InvertedBinLower + Delta;

        if (InvertedPrintOut) {
            cout << "\n\nInvertedBinLower = " << InvertedBinLower << "\n";
            cout << "InvertedBinUpper = " << InvertedBinUpper << "\n";
            cout << "i = " << i << "\n";
            cout << "Delta = " << Delta << "\n\n";
        }

        InvertedPBinsLimits.push_back({InvertedBinLower, InvertedBinUpper});
    }

    if (InvertedPrintOut) { exit(0); }

    for (int i = (NumberOfMomBins - 1); i >= 0; i--) {
        double BinLower = 1 / InvertedPBinsLimits.at(i).at(1);
        double BinUpper = 1 / InvertedPBinsLimits.at(i).at(0);

        if (RegPrintOut) {
            cout << "\n\nBinLower = " << BinLower << "\n";
            cout << "BinUpper = " << BinUpper << "\n";
            cout << "i = " << i << "\n";
        }

        PBinsLimits.push_back({BinLower, BinUpper});
    }

    if (RegPrintOut) { exit(0); }
}
//</editor-fold>

//<editor-fold desc="SetBins function (old)">
void AMaps::SetBins(double beamE, double nOfMomBins) {
    double BinUpperLim = beamE;

    bool SliceAndDice = true;
    int BinNumber = 1;

    while (SliceAndDice) {
        double UpperLim, LowerLim;

        if (BinNumber == 1) {
            UpperLim = BinUpperLim;
            LowerLim = (BinUpperLim / nOfMomBins);
        } else {
            UpperLim = LowerLim;
            LowerLim = (UpperLim / nOfMomBins);
        }

        PBinsLimits.push_back({LowerLim, UpperLim});

        if (LowerLim <= MomBinTh) {
            SliceAndDice = false;
        } else {
            ++BinNumber;
        }
    }
}
//</editor-fold>

// isElectron function --------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="isElectron function">
bool AMaps::isElectron(const string &SampleType) {
    if (SampleType == "Electron" || SampleType == "electron") {
        return true;
    } else {
        return false;
    }
}
//</editor-fold>

// isProton function ----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="isProton function">
bool AMaps::isProton(const string &SampleType) {
    if (SampleType == "Proton" || SampleType == "proton") {
        return true;
    } else {
        return false;
    }
}
//</editor-fold>

// isNeutron function ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="isNeutron function">
bool AMaps::isNeutron(const string &SampleType) {
    if (SampleType == "Neutron" || SampleType == "neutron") {
        return true;
    } else {
        return false;
    }
}
//</editor-fold>

// isTL function --------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="isTL function">
bool AMaps::isTL(const string &SampleType) {
    if (SampleType == "TL" || SampleType == "truth level" || SampleType == "truth-level" || SampleType == "Truth-Level") {
        return true;
    } else {
        return false;
    }
    /*
    if (SampleType == "TL" || SampleType == "truth level" || SampleType == "truth-level" || SampleType == "Truth-Level") {
        return true;
    } else if (findSubstring(SampleType,"Rec") || findSubstring(SampleType,"rec")) {
        return false;
    } else {
        cout << "\n\nAMaps::isTL: could not configure SampleType. Exiting...\n", exit(0);
    }
*/
}
//</editor-fold>

// hFillHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hFillHitMaps function">
void AMaps::hFillHitMaps(const string &SampleType, const string &particle, double Momentum, double Theta, double Phi, double Weight) {
    bool is_e = isElectron(particle), is_p = isProton(particle), is_n = isNeutron(particle), is_TL = isTL(SampleType);

    if (is_TL) {
        if (is_e) {
            for (int i = 0; i < PBinsLimits.size(); i++) {
                if ((Momentum >= PBinsLimits.at(i).at(0)) && (Momentum < PBinsLimits.at(i).at(1))) {
                    ElectronTLBinHitMaps.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p) {
            for (int i = 0; i < PBinsLimits.size(); i++) {
                if ((Momentum >= PBinsLimits.at(i).at(0)) && (Momentum < PBinsLimits.at(i).at(1))) {
                    ProtonTLBinHitMaps.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n) { NeutronTLHitMap.hFill(Phi, Theta, Weight); }
    } else {
        if (is_e) {
            ElectronAMapBC.hFill(Phi, Theta, Weight);

            for (int i = 0; i < PBinsLimits.size(); i++) {
                if ((Momentum >= PBinsLimits.at(i).at(0)) && (Momentum < PBinsLimits.at(i).at(1))) {
                    ElectronRecoBinHitMaps.at(i).hFill(Phi, Theta, Weight);
                    ElectronRecoToTLRatio.at(i).hFill(Phi, Theta, Weight);
                    ElectronSepAMaps.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p) {
            ProtonAMapBC.hFill(Phi, Theta, Weight);

            for (int i = 0; i < PBinsLimits.size(); i++) {
                if ((Momentum >= PBinsLimits.at(i).at(0)) && (Momentum < PBinsLimits.at(i).at(1))) {
                    ProtonRecoBinHitMaps.at(i).hFill(Phi, Theta, Weight);
                    ProtonRecoToTLRatio.at(i).hFill(Phi, Theta, Weight);
                    ProtonSepAMaps.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n) {
            NeutronAMapBC.hFill(Phi, Theta, Weight);

            NeutronRecoHitMap.hFill(Phi, Theta, Weight);
            NeutronRecoToTLRatio.hFill(Phi, Theta, Weight);
            NeutronAMap.hFill(Phi, Theta, Weight);
        }
    }
}
//</editor-fold>

// CalcHitMapsRatio function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="CalcHitMapsRatio function">
void AMaps::CalcHitMapsRatio(bool ElectronRecoToTLDiv, bool ProtonRecoToTLDiv, bool NeutronRecoToTLDiv) {
    if (ElectronRecoToTLDiv || ProtonRecoToTLDiv) {
        for (int i = 0; i < PBinsLimits.size(); i++) {
            if (ElectronRecoToTLDiv) { ElectronRecoToTLRatio.at(i).hDivision(ElectronTLBinHitMaps.at(i).GetHistogram2D()); }

            if (ProtonRecoToTLDiv) { ProtonRecoToTLRatio.at(i).hDivision(ProtonTLBinHitMaps.at(i).GetHistogram2D()); }
        }
    }

    if (NeutronRecoToTLDiv) { NeutronRecoToTLRatio.hDivision(NeutronTLHitMap.GetHistogram2D()); }
}
//</editor-fold>

// GenerateSeparateCPartAMaps function ----------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateSeparateCPartAMaps function">
void AMaps::GenerateSeparateCPartAMaps(double cP_minR) {
    for (int bin = 0; bin < PBinsLimits.size(); bin++) {
        for (int i = 0; i < (hBinNumOfXBins + 1); i++) {
            for (int j = 0; j < (hBinNumOfYBins + 1); j++) {
                if (ElectronRecoToTLRatio.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { ElectronSepAMaps.at(bin).hFillByBin(i, j, 0); }

                if (ProtonRecoToTLRatio.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { ProtonSepAMaps.at(bin).hFillByBin(i, j, 0); }
            }
        }

        ElectronSepAMaps.at(bin).ApplyZMinLim(cP_minR);
        ProtonSepAMaps.at(bin).ApplyZMinLim(cP_minR);
    }
}
//</editor-fold>

// GenerateCPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateCPartAMaps function">
void AMaps::GenerateCPartAMaps(double cP_minR) {
    GenerateSeparateCPartAMaps(cP_minR);

    for (int bin = 0; bin < PBinsLimits.size(); bin++) {
        ElectronAMap.hAdd(ElectronSepAMaps.at(bin).GetHistogram2D());
        ProtonAMap.hAdd(ProtonSepAMaps.at(bin).GetHistogram2D());
    }
}
//</editor-fold>

// GenerateNPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateNPartAMaps function">
void AMaps::GenerateNPartAMaps(double nP_minR) {
    for (int i = 0; i < (hBinNumOfXBins + 1); i++) {
        for (int j = 0; j < (hBinNumOfYBins + 1); j++) {
            if (NeutronRecoToTLRatio.GetHistogram2D()->GetBinContent(i, j) < nP_minR) { NeutronAMap.hFillByBin(i, j, 0); }
        }
    }
}
//</editor-fold>

// GenerateNucleonAMap function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateNucleonAMap function">
void AMaps::GenerateNucleonAMap() {
    for (int i = 0; i < (hBinNumOfXBins + 1); i++) {
        for (int j = 0; j < (hBinNumOfYBins + 1); j++) {
            if ((NeutronAMapBC.GetHistogram2D()->GetBinContent(i, j) != 0) &&
                (ProtonAMapBC.GetHistogram2D()->GetBinContent(i, j) != 0)) { NucleonAMapBC.hFillByBin(i, j, NeutronRecoHitMap.GetHistogram2D()->GetBinContent(i, j)); }

            if ((NeutronAMap.GetHistogram2D()->GetBinContent(i, j) != 0) &&
                (ProtonAMap.GetHistogram2D()->GetBinContent(i, j) != 0)) { NucleonAMap.hFillByBin(i, j, NeutronRecoHitMap.GetHistogram2D()->GetBinContent(i, j)); }
        }
    }
}
//</editor-fold>

// DrawAndSaveHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveHitMaps function">
void AMaps::DrawAndSaveHitMaps(const string &SampleName, TCanvas *h1DCanvas, const string &RefrenceHitMapsDirectory) {
    string SampleNameTemp = SampleName;

    system(("mkdir -p " + RefrenceHitMapsDirectory + SampleName).c_str());

    CalcHitMapsRatio(calc_Electron_RecoToTL_Ratio, calc_Proton_RecoToTL_Ratio, calc_Neutron_RecoToTL_Ratio);

    GenerateCPartAMaps(Charged_particle_min_Ratio);
    GenerateNPartAMaps(Neutral_particle_min_Ratio);
    GenerateNucleonAMap();

    /* Acceptance maps BC */
    ElectronAMapBC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, true);
    ProtonAMapBC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, true);
    NeutronAMapBC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, true);
    NucleonAMapBC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, true);

    for (int i = 0; i < PBinsLimits.size(); i++) {
        /* Electron maps */
        ElectronTLBinHitMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLHitMaps, true);
        ElectronRecoBinHitMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoHitMaps, true);
        ElectronRecoToTLRatio.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, HitMapsRatio, true);
        ElectronSepAMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, true);

        /* Proton maps */
        ProtonTLBinHitMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLHitMaps, true);
        ProtonRecoBinHitMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoHitMaps, true);
        ProtonRecoToTLRatio.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, HitMapsRatio, true);
        ProtonSepAMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, true);
    }

    /* Neutron maps */
    NeutronTLHitMap.hDrawAndSave(SampleNameTemp, h1DCanvas, TLHitMaps, true);
    NeutronRecoHitMap.hDrawAndSave(SampleNameTemp, h1DCanvas, RecoHitMaps, true);
    NeutronRecoToTLRatio.hDrawAndSave(SampleNameTemp, h1DCanvas, HitMapsRatio, true);

    /* Finalized acceptance maps */
    ElectronAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, true);
    ProtonAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, true);
    NeutronAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, true);
    NucleonAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, true);

    //<editor-fold desc="Save TL hit maps to plots directory">
    /* Acceptance maps BC */
    TFile *AMapsBC_plots_path_fout = new TFile((BinSavePath + "/" + AMapsBC_prefix + SampleName + ".root").c_str(), "recreate");
    AMapsBC_plots_path_fout->cd();
    AcceptanceMapsBC->Write();
    AMapsBC_plots_path_fout->Write();
    AMapsBC_plots_path_fout->Close();

    /* TL hit maps */
    TFile *TLHitMaps_plots_path_fout = new TFile((BinSavePath + "/" + Hit_Maps_TL_prefix + SampleName + ".root").c_str(), "recreate");
    TLHitMaps_plots_path_fout->cd();
    TLHitMaps->Write();
    TLHitMaps_plots_path_fout->Write();
    TLHitMaps_plots_path_fout->Close();

    /* Reco hit maps */
    TFile *RecoHitMaps_plots_path_fout = new TFile((BinSavePath + "/" + Hit_Maps_Reco_prefix + SampleName + ".root").c_str(), "recreate");
    RecoHitMaps_plots_path_fout->cd();
    RecoHitMaps->Write();
    RecoHitMaps_plots_path_fout->Write();
    RecoHitMaps_plots_path_fout->Close();

    /* Ratio hit maps */
    TFile *RatioHitMaps_plots_path_fout = new TFile((BinSavePath + "/" + Hit_Maps_Ratio_prefix + SampleName + ".root").c_str(), "recreate");
    RatioHitMaps_plots_path_fout->cd();
    HitMapsRatio->Write();
    RatioHitMaps_plots_path_fout->Write();
    RatioHitMaps_plots_path_fout->Close();

    /* Charged particle separate AMaps */
    TFile *cPartAMaps_plots_path_fout = new TFile((BinSavePath + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    cPartAMaps_plots_path_fout->cd();
    Charged_particle_Sep_AMaps->Write();
    cPartAMaps_plots_path_fout->Write();
    cPartAMaps_plots_path_fout->Close();

    /* Acceptance maps */
    TFile *AMaps_plots_path_fout = new TFile((BinSavePath + "/" + AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    AMaps_plots_path_fout->cd();
    AcceptanceMaps->Write();
    AMaps_plots_path_fout->Write();
    AMaps_plots_path_fout->Close();
    //</editor-fold>

    //<editor-fold desc="Save TL hit maps to refrence hit maps directory">
    /* Acceptance maps BC */
    TFile *AMapsBC_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/" + AMapsBC_prefix + SampleName + ".root").c_str(), "recreate");
    AMapsBC_plots_path_fout->cd();
    AcceptanceMapsBC->Write();
    AMapsBC_plots_path_fout->Write();
    AMapsBC_plots_path_fout->Close();

    /* TL hit maps */
    TFile *TLHitMaps_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/" + Hit_Maps_TL_prefix + SampleName + ".root").c_str(), "recreate");
    TLHitMaps_ref_hit_maps_fout->cd();
    TLHitMaps->Write();
    TLHitMaps_ref_hit_maps_fout->Write();
    TLHitMaps_ref_hit_maps_fout->Close();

    /* Reco hit maps */
    TFile *RecoHitMaps_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/" + Hit_Maps_Reco_prefix + SampleName + ".root").c_str(), "recreate");
    RecoHitMaps_ref_hit_maps_fout->cd();
    RecoHitMaps->Write();
    RecoHitMaps_ref_hit_maps_fout->Write();
    RecoHitMaps_ref_hit_maps_fout->Close();

    /* Ratio hit maps */
    TFile *RatioHitMaps_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/" + Hit_Maps_Ratio_prefix + SampleName + ".root").c_str(), "recreate");
    RatioHitMaps_ref_hit_maps_fout->cd();
    HitMapsRatio->Write();
    RatioHitMaps_ref_hit_maps_fout->Write();
    RatioHitMaps_ref_hit_maps_fout->Close();

    /* Charged particle separate AMaps */
    TFile *cPartAMaps_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    cPartAMaps_ref_hit_maps_fout->cd();
    Charged_particle_Sep_AMaps->Write();
    cPartAMaps_ref_hit_maps_fout->Write();
    cPartAMaps_ref_hit_maps_fout->Close();

    /* Acceptance maps */
    TFile *AMaps_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/" + AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    AMaps_ref_hit_maps_fout->cd();
    AcceptanceMaps->Write();
    AMaps_ref_hit_maps_fout->Write();
    AMaps_ref_hit_maps_fout->Close();
    //</editor-fold>

}
//</editor-fold>

// ReadHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadHitMaps function">
void AMaps::ReadHitMaps(const string &RefrenceHitMapsDirectory, const string &SampleName) {
    TFile *AMapsBC_RootFile = new TFile((RefrenceHitMapsDirectory + "/" + SampleName + "/" + AMapsBC_prefix + SampleName + ".root").c_str());
    if (!AMapsBC_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load AMapsBC root file! Exiting...\n", exit(0); }

    TFile *Hit_Maps_TL_RootFile = new TFile((RefrenceHitMapsDirectory + "/" + SampleName + "/" + Hit_Maps_TL_prefix + SampleName + ".root").c_str());
    if (!Hit_Maps_TL_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    TFile *Hit_Maps_Reco_RootFile = new TFile((RefrenceHitMapsDirectory + "/" + SampleName + "/" + Hit_Maps_Reco_prefix + SampleName + ".root").c_str());
    if (!Hit_Maps_Reco_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_Reco root file! Exiting...\n", exit(0); }

    TFile *Hit_Maps_Ratio_RootFile = new TFile((RefrenceHitMapsDirectory + "/" + SampleName + "/" + Hit_Maps_Ratio_prefix + SampleName + ".root").c_str());
    if (!Hit_Maps_Ratio_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_Ratio root file! Exiting...\n", exit(0); }

    TFile *cPart_Sep_AMaps_RootFile = new TFile((RefrenceHitMapsDirectory + "/" + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root").c_str());
    if (!cPart_Sep_AMaps_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load cPart_Sep_AMaps root file! Exiting...\n", exit(0); }

    TFile *AMaps_RootFile = new TFile((RefrenceHitMapsDirectory + "/" + SampleName + "/" + AMaps_prefix + SampleName + ".root").c_str());
    if (!AMaps_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load AMaps root file! Exiting...\n", exit(0); }


//    cout << "\n\n\nloaded OK!\n", exit(0);
}
//</editor-fold>
