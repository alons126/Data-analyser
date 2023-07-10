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
AMaps::AMaps(bool reformat_e_bins, bool equi_P_e_bins, double beamE, const string &SavePath, int nOfMomBins, int hnsNumOfXBins, int hnsNumOfYBins,
             int hesNumOfXBins, int hesNumOfYBins) {
    AMapSavePath = SavePath;
    HistNucSliceNumOfXBins = hnsNumOfXBins;
    HistNucSliceNumOfYBins = hnsNumOfYBins;
    HistElectronSliceNumOfXBins = hesNumOfXBins;
    HistElectronSliceNumOfYBins = hesNumOfYBins;
    NumberNucOfMomSlices = nOfMomBins;

    //<editor-fold desc="Setting saving directories">
    string SavePathAMapsBC = AMapSavePath + "00b_AMaps_BC_from_class/";
    system(("mkdir -p " + SavePathAMapsBC).c_str());

    string AMapSavePathTLElectron = SavePath + "01a_Electron_TL_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathTLElectron).c_str());
    string AMapSavePathRecoElectron = SavePath + "01b_Electron_Reco_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathRecoElectron).c_str());
    string AMapSavePathRecoToTLElectron = SavePath + "01c_Electron_RecoToTL_Ratio/";
    system(("mkdir -p " + AMapSavePathRecoToTLElectron).c_str());
    string AMapSavePathSepAMapsElectron = AMapSavePath + "01d_Electron_Separate_AMaps/";
    system(("mkdir -p " + AMapSavePathSepAMapsElectron).c_str());

    string AMapSavePathTLProton = SavePath + "02a_Proton_TL_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathTLProton).c_str());
    string AMapSavePathRecoProton = SavePath + "02b_Proton_Reco_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathRecoProton).c_str());
    string AMapSavePathRecoToTLProton = SavePath + "02c_Proton_RecoToTL_Ratio/";
    system(("mkdir -p " + AMapSavePathRecoToTLProton).c_str());
    string AMapSavePathSepAMapsProton = AMapSavePath + "02d_Proton_Separate_AMaps/";
    system(("mkdir -p " + AMapSavePathSepAMapsProton).c_str());

    string AMapSavePathTLNeutron = SavePath + "03a_Neutron_TL_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathTLNeutron).c_str());
    string AMapSavePathRecoNeutron = SavePath + "03b_Neutron_Reco_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathRecoNeutron).c_str());
    string AMapSavePathRecoToTLNeutron = SavePath + "03c_Neutron_RecoToTL_Ratio/";
    system(("mkdir -p " + AMapSavePathRecoToTLNeutron).c_str());
    string AMapSavePathSepAMapsNeutron = AMapSavePath + "03d_Neutron_Separate_AMaps/";
    system(("mkdir -p " + AMapSavePathSepAMapsNeutron).c_str());

    string AMapSavePathAMap = AMapSavePath + "04_Finalized_AMaps/";
    system(("mkdir -p " + AMapSavePathAMap).c_str());
    //</editor-fold>

    SetBins(beamE);
    SetElectronBins(reformat_e_bins, equi_P_e_bins, beamE);

    //<editor-fold desc="Acceptance maps BC">
    string hStatsTitleAMapBCElectron = "Electron_AMap_BC", hTitleAMapBCElectron = "Electron AMap BC", hSaveNameAMapBCElectron = "01_e_AMap_BC";
    ElectronAMapBC = hPlot2D("", "", hStatsTitleAMapBCElectron, hTitleAMapBCElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]", SavePathAMapsBC, hSaveNameAMapBCElectron,
                             hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);

    string hStatsTitleAMapBCProton = "Proton_AMap_BC", hTitleAMapBCProton = "Proton AMap BC", hSaveNameAMapBCProton = "02_p_AMap_BC";
    ProtonAMapBC = hPlot2D("", "", hStatsTitleAMapBCProton, hTitleAMapBCProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]", SavePathAMapsBC, hSaveNameAMapBCProton,
                           hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    string hStatsTitleAMapBCNeutron = "Neutron_AMap_BC", hTitleAMapBCNeutron = "Neutron AMap BC", hSaveNameAMapBCNeutron = "03_n_AMap_BC";
    NeutronAMapBC = hPlot2D("", "", hStatsTitleAMapBCNeutron, hTitleAMapBCNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", SavePathAMapsBC, hSaveNameAMapBCNeutron,
                            hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    string hStatsTitleAMapBCNucleon = "Nucleon_AMap_BC", hTitleAMapBCNucleon = "Nucleon AMap BC", hSaveNameAMapBCNucleon = "04_nuc_AMap_BC";
    NucleonAMapBC = hPlot2D("", "", hStatsTitleAMapBCNucleon, hTitleAMapBCNucleon, "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]", SavePathAMapsBC, hSaveNameAMapBCNucleon,
                            hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    //</editor-fold>

    for (int i = 0; i < ElectronMomBinsLimits.size(); i++) {
        double BinLowerLim = ElectronMomBinsLimits.at(i).at(0), BinUpperLim = ElectronMomBinsLimits.at(i).at(1);

        int BinUpperLimPrecision;
        if (BinUpperLim == beamE) { BinUpperLimPrecision = 3; } else { BinUpperLimPrecision = 2; }

        string BinDensity = " (" + to_string(HistElectronSliceNumOfXBins) + "x" + to_string(HistElectronSliceNumOfYBins) + ")";

        //<editor-fold desc="Setting electron Acceptance maps">

        //<editor-fold desc="Electron TL hit map">
        string hStatsTitleTLElectron = "TL P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleTLElectron = "TL P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameTLElectron = to_string(i + 1) + "_TL_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinTLElectron = hPlot2D("", "", hStatsTitleTLElectron, hTitleTLElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]", AMapSavePathTLElectron,
                                          hSaveNameTLElectron,
                                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
        ElectronTLAMapsBySlice.push_back(hPBinTLElectron);
        //</editor-fold>

        //<editor-fold desc="Electron Reco. Acceptance maps">
        string hStatsTitleRecoElectron = "Reco P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                         to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoElectron = "Reco P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                    to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoElectron = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoElectron = hPlot2D("", "", hStatsTitleRecoElectron, hTitleRecoElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]", AMapSavePathRecoElectron,
                                            hSaveNameRecoElectron,
                                            hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
        ElectronRecoAMapsBySlice.push_back(hPBinRecoElectron);
        //</editor-fold>

        //<editor-fold desc="Electron Reco./TL Ratio">
        string hStatsTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                             to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoToTLRatioElectron = to_string(i + 1) + "_e_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioElectron = hPlot2D("", "", hStatsTitleRecoToTLRatioElectron, hTitleRecoToTLRatioElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                                     AMapSavePathRecoToTLElectron,
                                                     hSaveNameRecoToTLRatioElectron,
                                                     hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins,
                                                     HistElectronSliceNumOfYBins);
        ElectronRecoToTLRatioBySlice.push_back(hPBinRecoToTLRatioElectron);
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
                                                AMapSavePathSepAMapsElectron,
                                                hSaveNameSepAMapsElectron,
                                                hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
        ElectronAMapsBySlice.push_back(hPBinSepAMapsElectron);
        //</editor-fold>

        //</editor-fold>
    }

    for (int i = 0; i < PBinsLimits.size(); i++) {
        double BinLowerLim = PBinsLimits.at(i).at(0), BinUpperLim = PBinsLimits.at(i).at(1);

        int BinUpperLimPrecision;
        if (BinUpperLim == beamE) { BinUpperLimPrecision = 3; } else { BinUpperLimPrecision = 2; }

        string BinDensity = " (" + to_string(HistNucSliceNumOfXBins) + "x" + to_string(HistNucSliceNumOfYBins) + ")";

        //<editor-fold desc="Setting proton Acceptance maps">

        //<editor-fold desc="Proton TL Acceptance maps">
        string hStatsTitleTLProton = "TL P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleTLProton = "TL P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameTLProton = to_string(i + 1) + "_TL_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                   to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinTLProton = hPlot2D("", "", hStatsTitleTLProton, hTitleTLProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]", AMapSavePathTLProton, hSaveNameTLProton,
                                        hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        ProtonTLAMapsBySlice.push_back(hPBinTLProton);
        //</editor-fold>

        //<editor-fold desc="Proton Reco. Acceptance maps">
        string hStatsTitleRecoProton = "Reco P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoProton = "Reco P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoProton = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoProton = hPlot2D("", "", hStatsTitleRecoProton, hTitleRecoProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]", AMapSavePathRecoProton,
                                          hSaveNameRecoProton,
                                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        ProtonRecoAMapsBySlice.push_back(hPBinRecoProton);
        //</editor-fold>

        //<editor-fold desc="Proton Reco./TL Ratio">
        string hStatsTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                           to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoToTLRatioProton = to_string(i + 1) + "_p_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                              to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioProton = hPlot2D("", "", hStatsTitleRecoToTLRatioProton, hTitleRecoToTLRatioProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                                                   AMapSavePathRecoToTLProton,
                                                   hSaveNameRecoToTLRatioProton,
                                                   hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        ProtonRecoToTLRatioBySlice.push_back(hPBinRecoToTLRatioProton);
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
                                              AMapSavePathSepAMapsProton,
                                              hSaveNameSepAMapsProton,
                                              hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        ProtonAMapsBySlice.push_back(hPBinSepAMapsProton);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Setting neutron Acceptance maps">

        //<editor-fold desc="Neutron TL Acceptance maps">
        string hStatsTitleTLNeutron = "TL P_{n} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                      to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleTLNeutron = "TL P_{n} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                 to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameTLNeutron = to_string(i + 1) + "_TL_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                    to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinTLNeutron = hPlot2D("", "", hStatsTitleTLNeutron, hTitleTLNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathTLNeutron, hSaveNameTLNeutron,
                                         hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        NeutronTLAMapsBySlice.push_back(hPBinTLNeutron);
        //</editor-fold>

        //<editor-fold desc="Neutron Reco. Acceptance maps">
        string hStatsTitleRecoNeutron = "Reco P_{n} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                        to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoNeutron = "Reco P_{n} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                   to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoNeutron = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                      to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoNeutron = hPlot2D("", "", hStatsTitleRecoNeutron, hTitleRecoNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathRecoNeutron,
                                           hSaveNameRecoNeutron,
                                           hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        NeutronRecoAMapsBySlice.push_back(hPBinRecoNeutron);
        //</editor-fold>

        //<editor-fold desc="Neutron Reco./TL Ratio">
        string hStatsTitleRecoToTLRatioNeutron = "Neutron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                                 to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioNeutron = "Neutron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                            to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoToTLRatioNeutron = to_string(i + 1) + "_p_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                               to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioNeutron = hPlot2D("", "", hStatsTitleRecoToTLRatioNeutron, hTitleRecoToTLRatioNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                                                    AMapSavePathRecoToTLNeutron,
                                                    hSaveNameRecoToTLRatioNeutron,
                                                    hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        NeutronRecoToTLRatioBySlice.push_back(hPBinRecoToTLRatioNeutron);
        //</editor-fold>

        //<editor-fold desc="Neutron separate AMaps">
        string hStatsTitleSepAMapsNeutron = "Neutron AMap for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                            to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                            to_string_with_precision(Neutral_particle_min_Ratio, 2);
        string hTitleSepAMapsNeutron = "Neutron AMap for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                       to_string_with_precision(Neutral_particle_min_Ratio, 2);
        string hSaveNameSepAMapsNeutron = to_string(i + 1) + "_n_SepAMap_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                          to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinSepAMapsNeutron = hPlot2D("", "", hStatsTitleSepAMapsNeutron, hTitleSepAMapsNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                                               AMapSavePathSepAMapsNeutron,
                                               hSaveNameSepAMapsNeutron,
                                               hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        NeutronAMapsBySlice.push_back(hPBinSepAMapsNeutron);
        //</editor-fold>

        //</editor-fold>

    }

    //<editor-fold desc="Setting neutron Acceptance maps">

    //<editor-fold desc="Neutron TL Acceptance maps">
    string hStatsTitleTLNeutron = "TL Neutron Hit Map", hTitleTLNeutron = "TL Neutron Hit Map", hSaveNameTLNeutron = "TL_n_AMap";
    NeutronTLAMap = hPlot2D("", "", hStatsTitleTLNeutron, hTitleTLNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathTLNeutron, hSaveNameTLNeutron,
                              hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Neutron Reco. Acceptance maps">
    string hStatsTitleRecoNeutron = "Reco Neutron Hit Map", hTitleRecoNeutron = "Reco Neutron Hit Map", hSaveNameRecoNeutron = "Reco_n_AMap";
    NeutronRecoAMap = hPlot2D("", "", hStatsTitleRecoNeutron, hTitleRecoNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathRecoNeutron,
                                hSaveNameRecoNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Neutron Reco./TL Ratio">
    string hStatsTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hSaveNameRecoToTLNeutron = "Neutron_Ratio";
    NeutronRecoToTLRatio = hPlot2D("", "", hStatsTitleRecoToTLNeutron, hTitleRecoToTLNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathRecoToTLNeutron,
                                   hSaveNameRecoToTLNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Finalized acceptance maps">
    string hStatsTitleAMapElectron = "Electron_AMap";
    string hTitleAMapElectron = "Electron AMap for (Reco./TL)#geq" + to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapElectron = "01_e_AMap";
    ElectronAMap = hPlot2D("", "", hStatsTitleAMapElectron, hTitleAMapElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]", AMapSavePathAMap, hSaveNameAMapElectron,
                           hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);

    string hStatsTitleAMapProton = "Proton_AMap";
    string hTitleAMapProton = "Proton AMap for (Reco./TL)#geq" + to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapProton = "02_p_AMap";
    ProtonAMap = hPlot2D("", "", hStatsTitleAMapProton, hTitleAMapProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]", AMapSavePathAMap, hSaveNameAMapProton, hBinLowerXLim,
                         hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    string hStatsTitleAMapNeutron = "Neutron_AMap";
    string hTitleAMapNeutron = "Neutron AMap for (Reco./TL)#geq" + to_string_with_precision(Neutral_particle_min_Ratio, 2);
    string hSaveNameAMapNeutron = "03_n_AMap";
    NeutronAMap = hPlot2D("", "", hStatsTitleAMapNeutron, hTitleAMapNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathAMap, hSaveNameAMapNeutron,
                          hBinLowerXLim,
                          hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    string hStatsTitleAMapNucleon = "Nucleon_AMap";
    string hTitleAMapNucleon = "Nucleon AMap for (Reco./TL)_{n}#geq" + to_string_with_precision(Neutral_particle_min_Ratio, 2) + " and (Reco./TL)_{c}#geq" +
                               to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapNucleon = "04_nuc_AMap";
    NucleonAMap = hPlot2D("", "", hStatsTitleAMapNucleon, hTitleAMapNucleon, "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]", AMapSavePathAMap, hSaveNameAMapNucleon,
                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    //</editor-fold>

}
//</editor-fold>

//<editor-fold desc="AMaps loading constructor">
AMaps::AMaps(const string &AcceptanceMapsDirectory, const string &SampleName) {
    ReadAMapLimits((AcceptanceMapsDirectory + SampleName + "/e_AMap_by_slice/e_slice_limits.par").c_str(), Loaded_ElectronMomBinsLimits);
    ReadAMapLimits((AcceptanceMapsDirectory + SampleName + "/p_AMap_by_slice/p_slice_limits.par").c_str(), Loaded_PBinsLimits);

    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomBinsLimits, Loaded_e_AMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Proton", Loaded_PBinsLimits, Loaded_p_AMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Neutron", Loaded_PBinsLimits, Loaded_n_AMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Nucleon", Loaded_PBinsLimits, Loaded_nuc_AMap_Slices);

    ReadAMap((AcceptanceMapsDirectory + SampleName + "/e_AMap_file.par").c_str(), Loaded_e_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/p_AMap_file.par").c_str(), Loaded_p_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/n_AMap_file.par").c_str(), Loaded_n_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/nuc_AMap_file.par").c_str(), Loaded_nuc_AMap);

    HistElectronSliceNumOfXBins = 100;  // 100 by Default
    HistElectronSliceNumOfYBins = 100;  // 100 by Default
    HistNucSliceNumOfXBins = 75;  // 100 by Default
    HistNucSliceNumOfYBins = 75;  // 100 by Default
}
//</editor-fold>

// SetBins functions ----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetBins function">
void AMaps::SetBins(double beamE) {
    double InvertedPLowerLim = (1 / beamE);
    double InvertedPUpper = (1 / MomBinTh);
    double Delta = (InvertedPUpper - InvertedPLowerLim) / NumberNucOfMomSlices;

    bool InvertedPrintOut = false;
    bool RegPrintOut = false;

    for (int i = 0; i < NumberNucOfMomSlices; i++) {
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

    if (InvertedPrintOut && !RegPrintOut) { exit(0); }

    for (int i = (NumberNucOfMomSlices - 1); i >= 0; i--) {
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

//<editor-fold desc="SetElectronBins function">
void AMaps::SetElectronBins(bool reformat_e_bins, bool equi_P_e_bins, double beamE) {
    bool InvertedPrintOut = false;
    bool RegPrintOut = false;

    if (reformat_e_bins) {
        double InvertedPLowerLim = (1 / beamE);
        double InvertedPUpperLim = (1 / MomBinTh);
        double p2 = 1.57819;
        double Delta = (InvertedPUpperLim - InvertedPLowerLim) / NumberNucOfMomSlices;
        double delta = (InvertedPUpperLim - (1 / p2));
        double Ratio = (InvertedPUpperLim - InvertedPLowerLim) / delta;

        int NumOfElectronMomBins;

        bool SliceAndDice = true;
        int iter = 0;

        while (SliceAndDice) {
            double InvBinLower;
            double InvBinUpper;
            double deltaLoop = delta;

            if (iter == 0) {
                InvBinUpper = InvertedPUpperLim;
                InvBinLower = InvBinUpper - deltaLoop;
            } else {
                InvBinUpper = InvBinLower;

                deltaLoop = delta / 12;

                if (InvBinUpper - deltaLoop > 0) {
                    InvBinLower = InvBinUpper - deltaLoop;
                } else {
                    InvBinLower = InvertedPLowerLim;
                }
            }

            if (InvertedPrintOut) {
                cout << "\n\nInvBinLower = " << InvBinLower << "\n";
                cout << "InvBinUpper = " << InvBinUpper << "\n";
                cout << "iter = " << iter << "\n";
                cout << "delta = " << delta << "\n";
                cout << "deltaLoop = " << deltaLoop << "\n\n";
            }

//            ElectronInvertedMomBinsLimits.push_back({InvBinLower, InvBinUpper});

//            if (InvBinUpper >= InvertedPLowerLim) {
            if (InvBinLower >= InvertedPLowerLim) {
//            if (InvBinUpper - deltaLoop >= InvertedPLowerLim) {
                ElectronInvertedMomBinsLimits.push_back({InvBinLower, InvBinUpper});
                ++iter;
            } else {
                SliceAndDice = false;
            }
        }

        NumOfElectronMomBins = ElectronInvertedMomBinsLimits.size();

        if (InvertedPrintOut && !RegPrintOut) { exit(0); }

        if (RegPrintOut) { cout << "\n\n---------------------------------------------------\n"; }

        for (int i = 0; i < NumOfElectronMomBins; i++) {
//        for (int i = (NumOfElectronMomBins - 1); i >= 0; i--) {
//        for (int i = (NumberNucOfMomSlices - 2); i >= 0; i--) {
//        for (int i = (NumOfElectronMomBins - 1); i >= 0; i--) {
            double BinLower = 1 / ElectronInvertedMomBinsLimits.at(i).at(1);
            double BinUpper = 1 / ElectronInvertedMomBinsLimits.at(i).at(0);

            if (RegPrintOut) {
                cout << "\n\nBinLower = " << BinLower << "\n";
                cout << "BinUpper = " << BinUpper << "\n";
                cout << "i = " << i << "\n";
            }

            ElectronMomBinsLimits.push_back({BinLower, BinUpper});
        }

        if (RegPrintOut) { exit(0); }
    } else if (equi_P_e_bins) {
        ElectronMomBinsLimits = {{0.4, 1.6},
                                 {1.6, 2.2},
                                 {2.2, 2.8},
                                 {2.8, 4},
                                 {4,   5},
                                 {5,   6}};
//        ElectronMomBinsLimits = {{0.4, 1},
//                                 {1,   1.6},
//                                 {1.6, 2.2},
//                                 {2.2, 2.8},
//                                 {2.8, 4},
//                                 {4,   5},
//                                 {5,   6}};
        int NumOfElectronMomBins = ElectronMomBinsLimits.size();

        if (RegPrintOut) {
            for (int i = 0; i < NumOfElectronMomBins; i++) {
                cout << "\n\nElectronMomBinsLimits.at(" << i << ").at(" << 0 << ") = " << ElectronMomBinsLimits.at(i).at(0) << "\n";
                cout << "ElectronMomBinsLimits.at(" << i << ").at(" << 1 << ") = " << ElectronMomBinsLimits.at(i).at(1) << "\n";
            }
        }

        if (RegPrintOut) { exit(0); }
    } else {
        double InvertedPLowerLim = (1 / beamE);
        double InvertedPUpperLim = (1 / MomBinTh);
        double Delta = (InvertedPUpperLim - InvertedPLowerLim) / NumberNucOfMomSlices;

        for (int i = 0; i < NumberNucOfMomSlices; i++) {
            double InvertedBinLower = InvertedPLowerLim + i * Delta;
            double InvertedBinUpper = InvertedBinLower + Delta;

            if (InvertedPrintOut) {
                cout << "\n\nInvertedBinLower = " << InvertedBinLower << "\n";
                cout << "InvertedBinUpper = " << InvertedBinUpper << "\n";
                cout << "i = " << i << "\n";
                cout << "Delta = " << Delta << "\n\n";
            }

            ElectronInvertedMomBinsLimits.push_back({InvertedBinLower, InvertedBinUpper});
        }

        if (InvertedPrintOut && !RegPrintOut) { exit(0); }

        for (int i = (NumberNucOfMomSlices - 1); i >= 0; i--) {
            double BinLower = 1 / ElectronInvertedMomBinsLimits.at(i).at(1);
            double BinUpper = 1 / ElectronInvertedMomBinsLimits.at(i).at(0);

            if (RegPrintOut) {
                cout << "\n\nBinLower = " << BinLower << "\n";
                cout << "BinUpper = " << BinUpper << "\n";
                cout << "i = " << i << "\n";
            }

            ElectronMomBinsLimits.push_back({BinLower, BinUpper});
        }

        if (RegPrintOut) { exit(0); }
    }
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
}
//</editor-fold>

// hFillHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hFillHitMaps function">
void AMaps::hFillHitMaps(const string &SampleType, const string &particle, double Momentum, double Theta, double Phi, double Weight) {
    bool is_e = isElectron(particle), is_p = isProton(particle), is_n = isNeutron(particle), is_TL = isTL(SampleType);

    if (is_TL) {
        if (is_e) {
            for (int i = 0; i < ElectronMomBinsLimits.size(); i++) {
                if ((Momentum >= ElectronMomBinsLimits.at(i).at(0)) && (Momentum < ElectronMomBinsLimits.at(i).at(1))) {
                    ElectronTLAMapsBySlice.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p) {
            for (int i = 0; i < PBinsLimits.size(); i++) {
                if ((Momentum >= PBinsLimits.at(i).at(0)) && (Momentum < PBinsLimits.at(i).at(1))) {
                    ProtonTLAMapsBySlice.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n) {
            NeutronTLAMap.hFill(Phi, Theta, Weight);

            for (int Bin = 0; Bin < PBinsLimits.size(); Bin++) { NeutronTLAMapsBySlice.at(Bin).hFill(Phi, Theta, Weight); }
        }
    } else {
        if (is_e) {
            ElectronAMapBC.hFill(Phi, Theta, Weight);

            for (int i = 0; i < ElectronMomBinsLimits.size(); i++) {
                if ((Momentum >= ElectronMomBinsLimits.at(i).at(0)) && (Momentum < ElectronMomBinsLimits.at(i).at(1))) {
                    ElectronRecoAMapsBySlice.at(i).hFill(Phi, Theta, Weight);
                    ElectronRecoToTLRatioBySlice.at(i).hFill(Phi, Theta, Weight);
                    ElectronAMapsBySlice.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p) {
            ProtonAMapBC.hFill(Phi, Theta, Weight);

            for (int i = 0; i < PBinsLimits.size(); i++) {
                if ((Momentum >= PBinsLimits.at(i).at(0)) && (Momentum < PBinsLimits.at(i).at(1))) {
                    ProtonRecoAMapsBySlice.at(i).hFill(Phi, Theta, Weight);
                    ProtonRecoToTLRatioBySlice.at(i).hFill(Phi, Theta, Weight);
                    ProtonAMapsBySlice.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n) {
            NeutronAMapBC.hFill(Phi, Theta, Weight);

            NeutronRecoAMap.hFill(Phi, Theta, Weight);
            NeutronRecoToTLRatio.hFill(Phi, Theta, Weight);
            NeutronAMap.hFill(Phi, Theta, Weight);

            for (int Bin = 0; Bin < PBinsLimits.size(); Bin++) {
                NeutronRecoAMapsBySlice.at(Bin).hFill(Phi, Theta, Weight);
                NeutronRecoToTLRatioBySlice.at(Bin).hFill(Phi, Theta, Weight);
                NeutronAMapsBySlice.at(Bin).hFill(Phi, Theta, Weight);
            }
        }
    }
}
//</editor-fold>

// CalcAMapsRatio function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="CalcAMapsRatio function">
void AMaps::CalcAMapsRatio(bool ElectronRecoToTLDiv, bool ProtonRecoToTLDiv, bool NeutronRecoToTLDiv) {
    if (ElectronRecoToTLDiv) {
        for (int i = 0; i < ElectronMomBinsLimits.size(); i++) {
            if (ElectronRecoToTLDiv) {
                ElectronRecoToTLRatioBySlice.at(i).hDivision(ElectronTLAMapsBySlice.at(i).GetHistogram2D());
            }
        }
    }

    if (ProtonRecoToTLDiv) {
        for (int i = 0; i < PBinsLimits.size(); i++) {
            if (ProtonRecoToTLDiv) { ProtonRecoToTLRatioBySlice.at(i).hDivision(ProtonTLAMapsBySlice.at(i).GetHistogram2D()); }
        }
    }

    if (NeutronRecoToTLDiv) {
        NeutronRecoToTLRatio.hDivision(NeutronTLAMap.GetHistogram2D());

        for (int i = 0; i < PBinsLimits.size(); i++) {
            if (NeutronRecoToTLDiv) { NeutronRecoToTLRatioBySlice.at(i).hDivision(NeutronTLAMapsBySlice.at(i).GetHistogram2D()); }
        }
    }
}
//</editor-fold>

// GenerateSeparateCPartAMaps function ----------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateSeparateCPartAMaps function">
void AMaps::GenerateSeparateCPartAMaps(double cP_minR) {
    for (int bin = 0; bin < ElectronMomBinsLimits.size(); bin++) {
        for (int i = 0; i < (HistElectronSliceNumOfXBins + 1); i++) {
            for (int j = 0; j < (HistElectronSliceNumOfYBins + 1); j++) {
                if (ElectronRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { ElectronAMapsBySlice.at(bin).hFillByBin(i, j, 0); }
            }
        }

        ElectronAMapsBySlice.at(bin).ApplyZMinLim(cP_minR);

        //<editor-fold desc="Fill e_AMap_Slices">
        vector<vector<int>> e_slice;

        for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
            vector<int> e_col;

            for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                if (ElectronRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                    e_col.push_back(1);
                } else {
                    e_col.push_back(0);
                }
            }

            e_slice.push_back(e_col);
        }

        e_AMap_Slices.push_back(e_slice);
        //</editor-fold>

    }

    for (int bin = 0; bin < PBinsLimits.size(); bin++) {
        for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
            for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                if (ProtonRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { ProtonAMapsBySlice.at(bin).hFillByBin(i, j, 0); }
            }
        }

        ProtonAMapsBySlice.at(bin).ApplyZMinLim(cP_minR);

        //<editor-fold desc="Fill p_AMap_Slices">
        vector<vector<int>> p_slice;

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            vector<int> p_col;

            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (ProtonRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                    p_col.push_back(1);
                } else {
                    p_col.push_back(0);
                }
            }

            p_slice.push_back(p_col);
        }

        p_AMap_Slices.push_back(p_slice);
        //</editor-fold>

    }
}
//</editor-fold>

// GenerateCPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateCPartAMaps function">
void AMaps::GenerateCPartAMaps(double cP_minR) {
    GenerateSeparateCPartAMaps(cP_minR);

    for (int bin = 0; bin < ElectronMomBinsLimits.size(); bin++) { ElectronAMap.hAdd(ElectronAMapsBySlice.at(bin).GetHistogram2D()); }

    for (int bin = 0; bin < PBinsLimits.size(); bin++) { ProtonAMap.hAdd(ProtonAMapsBySlice.at(bin).GetHistogram2D()); }

    for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
        vector<int> e_col;

        for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
            if (ElectronAMap.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                e_col.push_back(1);
            } else {
                e_col.push_back(0);
            }
        }

        e_AMap.push_back(e_col);
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> p_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (ProtonAMap.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                p_col.push_back(1);
            } else {
                p_col.push_back(0);
            }
        }

        p_AMap.push_back(p_col);
    }
}
//</editor-fold>

// GenerateNPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateNPartAMaps function">
void AMaps::GenerateNPartAMaps(double nP_minR) {
    for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
        for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
            if (NeutronRecoToTLRatio.GetHistogram2D()->GetBinContent(i, j) < nP_minR) {
                NeutronAMap.hFillByBin(i, j, 0);

                for (int bin = 0; bin < PBinsLimits.size(); bin++) { NeutronAMapsBySlice.at(bin).hFillByBin(i, j, 0); }
            }
        }
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> n_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (NeutronAMap.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= nP_minR) {
                n_col.push_back(1);
            } else {
                n_col.push_back(0);
            }
        }

        n_AMap.push_back(n_col);
    }

    for (int bin = 0; bin < PBinsLimits.size(); bin++) {
        NeutronAMapsBySlice.push_back(NeutronAMap);
        n_AMap_Slices.push_back(n_AMap);
    }
}
//</editor-fold>

// GenerateNucleonAMap function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateNucleonAMap function">
void AMaps::GenerateNucleonAMap() {
    for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
        for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
            if ((NeutronAMapBC.GetHistogram2D()->GetBinContent(i, j) != 0) &&
                (ProtonAMapBC.GetHistogram2D()->GetBinContent(i, j) != 0)) { NucleonAMapBC.hFillByBin(i, j, NeutronRecoAMap.GetHistogram2D()->GetBinContent(i, j)); }

            if ((NeutronAMap.GetHistogram2D()->GetBinContent(i, j) != 0) &&
                (ProtonAMap.GetHistogram2D()->GetBinContent(i, j) != 0)) { NucleonAMap.hFillByBin(i, j, NeutronRecoAMap.GetHistogram2D()->GetBinContent(i, j)); }
        }
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> nuc_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (NucleonAMap.GetHistogram2D()->GetBinContent(j + 1, i + 1) != 0) {
                nuc_col.push_back(1);
            } else {
                nuc_col.push_back(0);
            }
        }

        nuc_AMap.push_back(nuc_col);
    }

    for (int bin = 0; bin < PBinsLimits.size(); bin++) {
        vector<vector<int>> nuc_slice;

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            vector<int> nuc_col;

            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if ((p_AMap_Slices.at(bin).at(i).at(j) == 1) && (n_AMap_Slices.at(bin).at(i).at(j) == 1)) {
                    nuc_col.push_back(1);
                } else {
                    nuc_col.push_back(0);
                }
            }

            nuc_slice.push_back(nuc_col);
        }

        nuc_AMap_Slices.push_back(nuc_slice);
    }

//    cout << "\n\nGenerateNucleonAMap OK!\n", exit(0);
}
//</editor-fold>

// SaveHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SaveHitMaps function">
void AMaps::SaveHitMaps(const string &SampleName, const string &AcceptanceMapsDirectory) {
    string SliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_AMap_by_slice/";
    system(("mkdir -p " + SliceElectronSavePath).c_str());

    string SliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_AMap_by_slice/";
    system(("mkdir -p " + SliceProtonSavePath).c_str());

    string SliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_AMap_by_slice/";
    system(("mkdir -p " + SliceNeutronSavePath).c_str());

    string SliceNucleonSavePath = AcceptanceMapsDirectory + SampleName + "/nuc_AMap_by_slice/";
    system(("mkdir -p " + SliceNucleonSavePath).c_str());

    //<editor-fold desc="Save electron slices">
    for (int Slice = 0; Slice < ElectronMomBinsLimits.size(); Slice++) {
        ofstream e_AMap_TempFile;

        string TempFileName = "e_AMap_file_from_" + to_string_with_precision(ElectronMomBinsLimits.at(Slice).at(0), 2) + "_to_" +
                              to_string_with_precision(ElectronMomBinsLimits.at(Slice).at(1), 2) + ".par";

        e_AMap_TempFile.open(SliceElectronSavePath + TempFileName);

        e_AMap_TempFile << "Lower_P_lim:\t" << +ElectronMomBinsLimits.at(Slice).at(0) << "\n";
        e_AMap_TempFile << "Upper_P_lim:\t" << +ElectronMomBinsLimits.at(Slice).at(1) << "\n";
        e_AMap_TempFile << "\n";

        for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
            e_AMap_TempFile << "Line\t";

            for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                if (j != HistElectronSliceNumOfXBins - 1) {
                    e_AMap_TempFile << e_AMap_Slices.at(Slice).at(i).at(j) << ":";
                } else {
                    e_AMap_TempFile << e_AMap_Slices.at(Slice).at(i).at(j);
                }
            }

            e_AMap_TempFile << "\n";
        }

        e_AMap_TempFile.close();

//        system(("cp " + AMapSavePath + "e_AMap_TempFile.par " + AcceptanceMapsDirectory + SampleName + "/" + e_AMap_file_from).c_str());
    }
    //</editor-fold>

    //<editor-fold desc="Save proton, neutron & nucleon slices">
    for (int Slice = 0; Slice < PBinsLimits.size(); Slice++) {
        ofstream p_AMap_TempFile, n_AMap_TempFile, nuc_AMap_TempFile;

        string ProtonTempFileName = "p_AMap_file_from_" + to_string_with_precision(PBinsLimits.at(Slice).at(0), 2) + "_to_" +
                                    to_string_with_precision(PBinsLimits.at(Slice).at(1), 2) + ".par";
        string NeutronTempFileName = "n_AMap_file_from_" + to_string_with_precision(PBinsLimits.at(Slice).at(0), 2) + "_to_" +
                                     to_string_with_precision(PBinsLimits.at(Slice).at(1), 2) + ".par";
        string NucleonTempFileName = "nuc_AMap_file_from_" + to_string_with_precision(PBinsLimits.at(Slice).at(0), 2) + "_to_" +
                                     to_string_with_precision(PBinsLimits.at(Slice).at(1), 2) + ".par";

        p_AMap_TempFile.open(SliceProtonSavePath + ProtonTempFileName);
        n_AMap_TempFile.open(SliceNeutronSavePath + NeutronTempFileName);
        nuc_AMap_TempFile.open(SliceNucleonSavePath + NucleonTempFileName);

        p_AMap_TempFile << "Lower_P_lim:\t" << PBinsLimits.at(Slice).at(0) << "\n";
        p_AMap_TempFile << "Upper_P_lim:\t" << PBinsLimits.at(Slice).at(1) << "\n";
        p_AMap_TempFile << "\n";
        n_AMap_TempFile << "Lower_P_lim:\t" << PBinsLimits.at(Slice).at(0) << "\n";
        n_AMap_TempFile << "Upper_P_lim:\t" << PBinsLimits.at(Slice).at(1) << "\n";
        n_AMap_TempFile << "\n";
        nuc_AMap_TempFile << "Lower_P_lim:\t" << PBinsLimits.at(Slice).at(0) << "\n";
        nuc_AMap_TempFile << "Upper_P_lim:\t" << PBinsLimits.at(Slice).at(1) << "\n";
        nuc_AMap_TempFile << "\n";

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            p_AMap_TempFile << "Line\t";
            n_AMap_TempFile << "Line\t";
            nuc_AMap_TempFile << "Line\t";

            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (j != HistNucSliceNumOfXBins - 1) {
                    p_AMap_TempFile << p_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    n_AMap_TempFile << n_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    nuc_AMap_TempFile << nuc_AMap_Slices.at(Slice).at(i).at(j) << ":";
                } else {
                    p_AMap_TempFile << p_AMap_Slices.at(Slice).at(i).at(j);
                    n_AMap_TempFile << n_AMap_Slices.at(Slice).at(i).at(j);
                    nuc_AMap_TempFile << nuc_AMap_Slices.at(Slice).at(i).at(j);
                }
            }

            p_AMap_TempFile << "\n";
            n_AMap_TempFile << "\n";
            nuc_AMap_TempFile << "\n";
        }

        p_AMap_TempFile.close();
        n_AMap_TempFile.close();
        nuc_AMap_TempFile.close();

//        system(("cp " + AMapSavePath + "p_AMap_TempFile.par " + AcceptanceMapsDirectory + SampleName + "/" + p_AMap_file_from).c_str());
//        system(("cp " + AMapSavePath + "n_AMap_TempFile.par " + AcceptanceMapsDirectory + SampleName + "/" + n_AMap_file_from).c_str());
//        system(("cp " + AMapSavePath + "nuc_AMap_TempFile.par " + AcceptanceMapsDirectory + SampleName + "/" + nuc_AMap_file_from).c_str());
    }
    //</editor-fold>

    //<editor-fold desc="Save combined maps">
    ofstream e_AMap_file, p_AMap_file, n_AMap_file, nuc_AMap_file;

    e_AMap_file.open(AMapSavePath + "e_AMap_file.par");
    p_AMap_file.open(AMapSavePath + "p_AMap_file.par");
    n_AMap_file.open(AMapSavePath + "n_AMap_file.par");
    nuc_AMap_file.open(AMapSavePath + "nuc_AMap_file.par");

    for (int Slice = 0; Slice < ElectronMomBinsLimits.size(); Slice++) {
        e_AMap_file << "e_slice_" << (Slice + 1) << "\t" << ElectronMomBinsLimits.at(Slice).at(0) << ":" << ElectronMomBinsLimits.at(Slice).at(1) << "\n";
    }

    for (int Slice = 0; Slice < PBinsLimits.size(); Slice++) {
        p_AMap_file << "p_slice_" << (Slice + 1) << "\t" << PBinsLimits.at(Slice).at(0) << ":" << PBinsLimits.at(Slice).at(1) << "\n";
        n_AMap_file << "n_slice_" << (Slice + 1) << "\t" << PBinsLimits.at(Slice).at(0) << ":" << PBinsLimits.at(Slice).at(1) << "\n";
        nuc_AMap_file << "nuc_slice_" << (Slice + 1) << "\t" << PBinsLimits.at(Slice).at(0) << ":" << PBinsLimits.at(Slice).at(1) << "\n";
    }

    for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
        e_AMap_file << "Line\t";

        for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
            if (j != HistElectronSliceNumOfXBins - 1) {
                e_AMap_file << e_AMap.at(i).at(j) << ":";
            } else {
                e_AMap_file << e_AMap.at(i).at(j);
            }
        }

        e_AMap_file << "\n";
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        p_AMap_file << "Line\t";
        n_AMap_file << "Line\t";
        nuc_AMap_file << "Line\t";

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (j != HistNucSliceNumOfXBins - 1) {
                p_AMap_file << p_AMap.at(i).at(j) << ":";
                n_AMap_file << n_AMap.at(i).at(j) << ":";
                nuc_AMap_file << nuc_AMap.at(i).at(j) << ":";
            } else {
                p_AMap_file << p_AMap.at(i).at(j);
                n_AMap_file << n_AMap.at(i).at(j);
                nuc_AMap_file << nuc_AMap.at(i).at(j);
            }
        }

        p_AMap_file << "\n";
        n_AMap_file << "\n";
        nuc_AMap_file << "\n";
    }

    e_AMap_file.close();
    p_AMap_file.close();
    n_AMap_file.close();
    nuc_AMap_file.close();

    system(("cp " + AMapSavePath + "e_AMap_file.par " + AcceptanceMapsDirectory + SampleName).c_str());
    system(("cp " + AMapSavePath + "p_AMap_file.par " + AcceptanceMapsDirectory + SampleName).c_str());
    system(("cp " + AMapSavePath + "n_AMap_file.par " + AcceptanceMapsDirectory + SampleName).c_str());
    system(("cp " + AMapSavePath + "nuc_AMap_file.par " + AcceptanceMapsDirectory + SampleName).c_str());
    //</editor-fold>

    //<editor-fold desc="Slice limits">
    ofstream e_slice_limits, p_slice_limits, n_slice_limits, nuc_slice_limits;

    e_slice_limits.open(SliceElectronSavePath + "e_slice_limits.par");
    p_slice_limits.open(SliceProtonSavePath + "p_slice_limits.par");
    n_slice_limits.open(SliceNeutronSavePath + "n_slice_limits.par");
    nuc_slice_limits.open(SliceNucleonSavePath + "nuc_slice_limits.par");

    for (int Slice = 0; Slice < ElectronMomBinsLimits.size(); Slice++) {
        e_slice_limits << "e_slice_" << (Slice + 1) << "\t" << ElectronMomBinsLimits.at(Slice).at(0) << ":" << ElectronMomBinsLimits.at(Slice).at(1) << "\n";
    }

    for (int Slice = 0; Slice < PBinsLimits.size(); Slice++) {
        p_slice_limits << "p_slice_" << (Slice + 1) << "\t" << PBinsLimits.at(Slice).at(0) << ":" << PBinsLimits.at(Slice).at(1) << "\n";
        n_slice_limits << "n_slice_" << (Slice + 1) << "\t" << PBinsLimits.at(Slice).at(0) << ":" << PBinsLimits.at(Slice).at(1) << "\n";
        nuc_slice_limits << "nuc_slice_" << (Slice + 1) << "\t" << PBinsLimits.at(Slice).at(0) << ":" << PBinsLimits.at(Slice).at(1) << "\n";
    }

    e_slice_limits << "\n";
    p_slice_limits << "\n";
    n_slice_limits << "\n";
    nuc_slice_limits << "\n";

    e_slice_limits.close();
    p_slice_limits.close();
    n_slice_limits.close();
    nuc_slice_limits.close();

    system(("cp " + AMapSavePath + "e_slice_limits.par " + AcceptanceMapsDirectory + SampleName).c_str());
    system(("cp " + AMapSavePath + "p_slice_limits.par " + AcceptanceMapsDirectory + SampleName).c_str());
    system(("cp " + AMapSavePath + "n_slice_limits.par " + AcceptanceMapsDirectory + SampleName).c_str());
    system(("cp " + AMapSavePath + "nuc_slice_limits.par " + AcceptanceMapsDirectory + SampleName).c_str());
    //</editor-fold>

}
//</editor-fold>

// DrawAndSaveHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveHitMaps function">
void AMaps::DrawAndSaveHitMaps(const string &SampleName, TCanvas *h1DCanvas, const string &AcceptanceMapsDirectory) {
    string SampleNameTemp = SampleName;

    system(("mkdir -p " + AcceptanceMapsDirectory + SampleName).c_str());

    CalcAMapsRatio(calc_Electron_RecoToTL_Ratio, calc_Proton_RecoToTL_Ratio, calc_Neutron_RecoToTL_Ratio);

    GenerateCPartAMaps(Charged_particle_min_Ratio);
    GenerateNPartAMaps(Neutral_particle_min_Ratio);
    GenerateNucleonAMap();

    SaveHitMaps(SampleName, AcceptanceMapsDirectory);

    /* Acceptance maps BC */
    ElectronAMapBC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, true);
    ProtonAMapBC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, true);
    NeutronAMapBC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, true);
    NucleonAMapBC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, true);

    /* Electron maps */
    for (int i = 0; i < ElectronMomBinsLimits.size(); i++) {
        ElectronTLAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, true);
        ElectronRecoAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, true);
        ElectronRecoToTLRatioBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, true);
        ElectronAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, true);
    }

    for (int i = 0; i < PBinsLimits.size(); i++) {
        /* Proton maps */
        ProtonTLAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, true);
        ProtonRecoAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, true);
        ProtonRecoToTLRatioBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, true);
        ProtonAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, true);

        /* Neutron maps */
        NeutronTLAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, true);
        NeutronRecoAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, true);
        NeutronRecoToTLRatioBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, true);
        NeutronAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, true);
    }

    /* Neutron maps */
    NeutronTLAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, true);
    NeutronRecoAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, true);
    NeutronRecoToTLRatio.hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, true);

    /* Finalized acceptance maps */
    ElectronAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, true);
    ProtonAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, true);
    NeutronAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, true);
    NucleonAMap.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, true);

    //<editor-fold desc="Save TL Acceptance maps to plots directory">
    /* Acceptance maps BC */
    TFile *AMapsBC_plots_path_fout = new TFile((AMapSavePath + "/" + AMapsBC_prefix + SampleName + ".root").c_str(), "recreate");
    AMapsBC_plots_path_fout->cd();
    AcceptanceMapsBC->Write();
    AMapsBC_plots_path_fout->Write();
    AMapsBC_plots_path_fout->Close();

    /* TL Acceptance maps */
    TFile *TLAMaps_plots_path_fout = new TFile((AMapSavePath + "/" + AMap_TL_prefix + SampleName + ".root").c_str(), "recreate");
    TLAMaps_plots_path_fout->cd();
    TLAMaps->Write();
    TLAMaps_plots_path_fout->Write();
    TLAMaps_plots_path_fout->Close();

    /* Reco Acceptance maps */
    TFile *RecoAMaps_plots_path_fout = new TFile((AMapSavePath + "/" + AMap_Reco_prefix + SampleName + ".root").c_str(), "recreate");
    RecoAMaps_plots_path_fout->cd();
    RecoAMaps->Write();
    RecoAMaps_plots_path_fout->Write();
    RecoAMaps_plots_path_fout->Close();

    /* Ratio Acceptance maps */
    TFile *RatioHitMaps_plots_path_fout = new TFile((AMapSavePath + "/" + AMap_Ratio_prefix + SampleName + ".root").c_str(), "recreate");
    RatioHitMaps_plots_path_fout->cd();
    AMapsRatio->Write();
    RatioHitMaps_plots_path_fout->Write();
    RatioHitMaps_plots_path_fout->Close();

    /* Charged particle separate AMaps */
    TFile *cPartAMaps_plots_path_fout = new TFile((AMapSavePath + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    cPartAMaps_plots_path_fout->cd();
    Charged_particle_Sep_AMaps->Write();
    cPartAMaps_plots_path_fout->Write();
    cPartAMaps_plots_path_fout->Close();

    /* Acceptance maps */
    TFile *AMaps_plots_path_fout = new TFile((AMapSavePath + "/" + AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    AMaps_plots_path_fout->cd();
    AcceptanceMaps->Write();
    AMaps_plots_path_fout->Write();
    AMaps_plots_path_fout->Close();
    //</editor-fold>

    //<editor-fold desc="Save TL Acceptance maps to refrence Acceptance maps directory">
    /* Acceptance maps BC */
    TFile *AMapsBC_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMapsBC_prefix + SampleName + ".root").c_str(), "recreate");
    AMapsBC_ref_AMaps_fout->cd();
    AcceptanceMapsBC->Write();
    AMapsBC_ref_AMaps_fout->Write();
    AMapsBC_ref_AMaps_fout->Close();

    /* TL Acceptance maps */
    TFile *TLAMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_TL_prefix + SampleName + ".root").c_str(), "recreate");
    TLAMaps_ref_AMaps_fout->cd();
    TLAMaps->Write();
    TLAMaps_ref_AMaps_fout->Write();
    TLAMaps_ref_AMaps_fout->Close();

    /* Reco Acceptance maps */
    TFile *RecoAMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_Reco_prefix + SampleName + ".root").c_str(), "recreate");
    RecoAMaps_ref_AMaps_fout->cd();
    RecoAMaps->Write();
    RecoAMaps_ref_AMaps_fout->Write();
    RecoAMaps_ref_AMaps_fout->Close();

    /* Ratio Acceptance maps */
    TFile *RatioHitMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_Ratio_prefix + SampleName + ".root").c_str(), "recreate");
    RatioHitMaps_ref_AMaps_fout->cd();
    AMapsRatio->Write();
    RatioHitMaps_ref_AMaps_fout->Write();
    RatioHitMaps_ref_AMaps_fout->Close();

    /* Charged particle separate AMaps */
    TFile *cPartAMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    cPartAMaps_ref_AMaps_fout->cd();
    Charged_particle_Sep_AMaps->Write();
    cPartAMaps_ref_AMaps_fout->Write();
    cPartAMaps_ref_AMaps_fout->Close();

    /* Acceptance maps */
    TFile *AMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    AMaps_ref_AMaps_fout->cd();
    AcceptanceMaps->Write();
    AMaps_ref_AMaps_fout->Write();
    AMaps_ref_AMaps_fout->Close();
    //</editor-fold>

}
//</editor-fold>

// HistCounter function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="HistCounter function">
int AMaps::HistCounter(const char *fname) {
    bool PrintHistInfo = false;

    TKey *key;
    TFile *f = TFile::Open(fname, "READ");

    Int_t total = 0;
    TIter next((TList *) f->GetListOfKeys());

    while ((key = (TKey *) next())) {
        TClass *cl = gROOT->GetClass(key->GetClassName());

        if (cl->InheritsFrom("TH1")) {
            // the following line is not needed if you only want
            // to count the histograms
            TH1 *h = (TH1 *) key->ReadObj();

            if (PrintHistInfo) { cout << "Histo found: " << h->GetName() << " - " << h->GetTitle() << endl; }

            total++;
        }
    }

    if (PrintHistInfo) { cout << "\n\nFound " << total << " Histograms\n" << endl; }

    return total;
}
//</editor-fold>

// SetHistBinsFromHistTitle function ------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetHistBinsFromHistTitle function">
void AMaps::SetHistBinsFromHistTitle(TH2D *Histogram2D) {
    bool PrintOut = false;

//    HistNucSliceNumOfXBins = Histogram2D->GetNbinsX();
//    HistNucSliceNumOfYBins = Histogram2D->GetNbinsY();

    string Title = Histogram2D->GetTitle();
    string NumOfXBinsStr = Title.substr((Title.find_last_of('(')) + 1, 3);
    string NumOfYBinsStr = Title.substr(Title.find_last_of('x') + 1, 3);

    HistNucSliceNumOfXBins = stoi(NumOfXBinsStr);
    HistNucSliceNumOfYBins = stoi(NumOfYBinsStr);

    if (PrintOut) {
        cout << "\n\nTitle = " << Title << "\n\n";
        cout << "NumOfXBinsStr = " << NumOfXBinsStr << "\n";
        cout << "NumOfYBinsStr = " << NumOfYBinsStr << "\n\n";

        cout << "HistNucSliceNumOfXBins = " << HistNucSliceNumOfXBins << "\n";
        cout << "HistNucSliceNumOfYBins = " << HistNucSliceNumOfYBins << "\n\n";
    }
}
//</editor-fold>

// SetSlicesFromHistTitle function --------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetSlicesFromHistTitle function">
void AMaps::SetSlicesFromHistTitle(TH2D *Histogram2D, const string &Particle) {
    bool PrintOut = false;

    string Title = Histogram2D->GetTitle();
    string SliceLowerLimStr = Title.substr((Title.find_first_of('#')) - 4, 4);
    string SliceUpperLimStr = Title.substr(Title.find_last_of('#') + 4, 4);

    double SliceLowerLim = stod(SliceLowerLimStr);
    double SliceUpperLim = stod(SliceUpperLimStr);

    vector<double> MomBinsLimitsTemp = {SliceLowerLim, SliceUpperLim};

    if (isElectron(Particle)) {
        ElectronMomBinsLimits.push_back(MomBinsLimitsTemp);
    } else if (isProton(Particle) || isNeutron(Particle)) {
        PBinsLimits.push_back(MomBinsLimitsTemp);
    }

    if (PrintOut) {
        cout << "\n\nTitle = " << Title << "\n\n";
        cout << "SliceLowerLimStr = " << SliceLowerLimStr << "\n";
        cout << "SliceUpperLimStr = " << SliceUpperLimStr << "\n\n";

        cout << "SliceLowerLim = " << SliceLowerLim << "\n";
        cout << "SliceUpperLim = " << SliceUpperLim << "\n\n";
    }
}
//</editor-fold>

//<editor-fold desc="SetSlicesFromHistTitle function (original)">
void AMaps::SetSlicesFromHistTitle(TH2D *Histogram2D, vector<vector<double>> MomBinsLimits) {
    bool PrintOut = false;

    string Title = Histogram2D->GetTitle();
    string SliceLowerLimStr = Title.substr((Title.find_first_of('#')) - 4, 4);
    string SliceUpperLimStr = Title.substr(Title.find_last_of('#') + 4, 4);

    double SliceLowerLim = stod(SliceLowerLimStr);
    double SliceUpperLim = stod(SliceUpperLimStr);

    vector<double> MomBinsLimitsTemp = {SliceLowerLim, SliceUpperLim};
    PBinsLimits.push_back(MomBinsLimitsTemp);
//    MomBinsLimits.push_back(MomBinsLimitsTemp);

    if (PrintOut) {
        cout << "\n\nTitle = " << Title << "\n\n";
        cout << "SliceLowerLimStr = " << SliceLowerLimStr << "\n";
        cout << "SliceUpperLimStr = " << SliceUpperLimStr << "\n\n";

        cout << "SliceLowerLim = " << SliceLowerLim << "\n";
        cout << "SliceUpperLim = " << SliceUpperLim << "\n\n";

        cout << "MomBinsLimitsTemp.size() = " << MomBinsLimitsTemp.size() << "\n\n";
    }
}
//</editor-fold>

// ReadHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadHitMaps function">
void AMaps::ReadHitMaps(const string &AcceptanceMapsDirectory, const string &SampleName) {
    bool PrintKeys = false;

    //<editor-fold desc="Load AMapsBC">
    string AMapsBC_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMapsBC_prefix + SampleName + ".root";
    TFile *AMapsBC_RootFile = new TFile(AMapsBC_RootFile_FileName.c_str());
    if (!AMapsBC_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load AMapsBC root file! Exiting...\n", exit(0); }
//    int NumOfAMapsBC = HistCounter(AMapsBC_RootFile_FileName.c_str());

    for (TObject *keyAsObj: *AMapsBC_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *) keyAsObj;

        if (findSubstring(TempHist->GetTitle(), "Electron") || findSubstring(TempHist->GetTitle(), "electron")) {
            ElectronAMapBC.SetHistogram2D(TempHist);
        } else if (findSubstring(TempHist->GetTitle(), "Proton") || findSubstring(TempHist->GetTitle(), "proton")) {
            ProtonAMapBC.SetHistogram2D(TempHist);
        } else if (findSubstring(TempHist->GetTitle(), "Neutron") || findSubstring(TempHist->GetTitle(), "neutron")) {
            NeutronAMapBC.SetHistogram2D(TempHist);
        } else if (findSubstring(TempHist->GetTitle(), "Nucleon") || findSubstring(TempHist->GetTitle(), "nucleon")) {
            NucleonAMapBC.SetHistogram2D(TempHist);
        }
    }
    //</editor-fold>

    //<editor-fold desc="Load Hit_Maps_TL">
    string Hit_Maps_TL_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_TL_prefix + SampleName + ".root";
    TFile *Hit_Maps_TL_RootFile = new TFile(Hit_Maps_TL_RootFile_FileName.c_str());
    if (!Hit_Maps_TL_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }
//    int NumOfHit_Maps_TL = HistCounter(Hit_Maps_TL_RootFile_FileName.c_str());

    int counter = 0;

    for (TObject *keyAsObj: *Hit_Maps_TL_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *) keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (counter == 0) { SetHistBinsFromHistTitle(TempHist); }

        if (findSubstring(TempHist->GetTitle(), "{e}")) {
            ElectronTLAMapsBySlice.push_back(Temp2DHist);
            SetSlicesFromHistTitle(TempHist, PBinsLimits);
        } else if (findSubstring(TempHist->GetTitle(), "{p}")) {
            ProtonTLAMapsBySlice.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{n}")) {
            NeutronTLAMap.SetHistogram2D(TempHist);
        }

        ++counter;
    }
    //</editor-fold>

    //<editor-fold desc="Load Hit_Maps_Reco">
    string Hit_Maps_Reco_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_Reco_prefix + SampleName + ".root";
    TFile *Hit_Maps_Reco_RootFile = new TFile(Hit_Maps_Reco_RootFile_FileName.c_str());
    if (!Hit_Maps_Reco_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_Reco root file! Exiting...\n", exit(0); }
//    int NumOfHit_Maps_Reco = HistCounter(Hit_Maps_Reco_RootFile_FileName.c_str());

    for (TObject *keyAsObj: *Hit_Maps_Reco_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *) keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (findSubstring(TempHist->GetTitle(), "{e}")) {
            ElectronRecoAMapsBySlice.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{p}")) {
            ProtonRecoAMapsBySlice.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{n}")) {
            NeutronRecoAMap.SetHistogram2D(TempHist);
        }
    }
    //</editor-fold>

    //<editor-fold desc="Load Hit_Maps_Ratio">
    string Hit_Maps_Ratio_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_Ratio_prefix + SampleName + ".root";
    TFile *Hit_Maps_Ratio_RootFile = new TFile(Hit_Maps_Ratio_RootFile_FileName.c_str());
    if (!Hit_Maps_Ratio_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_Ratio root file! Exiting...\n", exit(0); }
//    int NumOfHit_Maps_Ratio = HistCounter(Hit_Maps_Ratio_RootFile_FileName.c_str());

    for (TObject *keyAsObj: *Hit_Maps_Ratio_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *) keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (findSubstring(TempHist->GetTitle(), "{e}")) {
            ElectronRecoToTLRatioBySlice.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{p}")) {
            ProtonRecoToTLRatioBySlice.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{n}")) {
            NeutronRecoToTLRatio.SetHistogram2D(TempHist);
        }
    }
    //</editor-fold>

    //<editor-fold desc="Load cPart_Sep_AMaps">
    string cPart_Sep_AMaps_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root";
    TFile *cPart_Sep_AMaps_RootFile = new TFile(cPart_Sep_AMaps_RootFile_FileName.c_str());
    if (!cPart_Sep_AMaps_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load cPart_Sep_AMaps root file! Exiting...\n", exit(0); }
//    int NumOfcPart_Sep_AMaps = HistCounter(cPart_Sep_AMaps_RootFile_FileName.c_str());

    for (TObject *keyAsObj: *cPart_Sep_AMaps_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *) keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (findSubstring(TempHist->GetTitle(), "Electron") || findSubstring(TempHist->GetTitle(), "electron")) {
            ElectronAMapsBySlice.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "Proton") || findSubstring(TempHist->GetTitle(), "proton")) {
            ProtonAMapsBySlice.push_back(Temp2DHist);
        }
    }
    //</editor-fold>

    //<editor-fold desc="Load AMaps">
    string AMaps_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMaps_prefix + SampleName + ".root";
    TFile *AMaps_RootFile = new TFile(AMaps_RootFile_FileName.c_str());
    if (!AMaps_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load AMaps root file! Exiting...\n", exit(0); }
//    int NumOfAMaps = HistCounter(AMaps_RootFile_FileName.c_str());

    LoadedElectronAMaps0 = (TH2D *) AMaps_RootFile->Get("Electron_AMap");
    if (!LoadedElectronAMaps0) { cout << "\n\nAMaps::ReadHitMaps: could not load Electron_AMap from root file! Exiting...\n", exit(0); }

    LoadedProtonAMap = (TH2D *) AMaps_RootFile->Get("Proton_AMap");
    if (!LoadedProtonAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Proton_AMap from root file! Exiting...\n", exit(0); }

    LoadedNeutronAMap = (TH2D *) AMaps_RootFile->Get("Neutron_AMap");
    if (!LoadedNeutronAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Neutron_AMap from root file! Exiting...\n", exit(0); }

    LoadedNucleonAMap = (TH2D *) AMaps_RootFile->Get("Nucleon_AMap");
    if (!LoadedNucleonAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Nucleon_AMap from root file! Exiting...\n", exit(0); }


//    for (TObject *keyAsObj: *AMaps_RootFile->GetListOfKeys()) {
//        auto key = dynamic_cast<TKey *>(keyAsObj);
//
//        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }
//
//        TH2D *TempHist = (TH2D *) keyAsObj;
//        hPlot2D Temp2DHist;
//        Temp2DHist.SetHistogram2D(TempHist);
//
//        if (findSubstring(TempHist->GetTitle(), "Electron") || findSubstring(TempHist->GetTitle(), "electron")) {
//            ElectronAMap.SetHistogram2D(TempHist);
//        } else if (findSubstring(TempHist->GetTitle(), "Proton") || findSubstring(TempHist->GetTitle(), "proton")) {
//            ProtonAMap.SetHistogram2D(TempHist);
//            LoadedProtonAMap = (TH2D *) keyAsObj;
//        } else if (findSubstring(TempHist->GetTitle(), "Neutron") || findSubstring(TempHist->GetTitle(), "neutron")) {
//            NeutronAMap.SetHistogram2D(TempHist);
//            LoadedNeutronAMap = (TH2D *) keyAsObj;
//        } else if (findSubstring(TempHist->GetTitle(), "Nucleon") || findSubstring(TempHist->GetTitle(), "nucleon")) {
//            NucleonAMap.SetHistogram2D(TempHist);
//            LoadedNucleonAMap = (TH2D *) keyAsObj;
//        }
//    }
    //</editor-fold>

    cout << "\n\nAcceptance maps loaded!\n\n";
}
//</editor-fold>

// ReadAMapLimits function ----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadAMapLimits function">
void AMaps::ReadAMapLimits(const char *filename, vector<vector<double>> &Loaded_particle_limits) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        string tp;

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            string parameter, parameter2;
            ss >> parameter; // get cut identifier

            if (findSubstring(parameter, "_slice_")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                string LineEntry;
                vector<double> particle_limits;

                while (getline(ss2, LineEntry, ':')) { particle_limits.push_back(stod(LineEntry)); }

                Loaded_particle_limits.push_back(particle_limits);
            }
        }
    } else {
        cout << "\n\nReadAMap: file not found! Exiting...\n\n", exit(0);
    }
}
//</editor-fold>

// ReadAMapSlices function ----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadAMapSlices function">
void AMaps::ReadAMapSlices(const string &SampleName, const string &AcceptanceMapsDirectory, const string &Particle,
                           const vector<vector<double>> &Loaded_particle_limits, vector<vector<vector<int>>> &Loaded_Particle_AMap_Slices) {
    string ParticleShort;

    if (isElectron(Particle)) {
        ParticleShort = "e";
    } else if (isProton(Particle)) {
        ParticleShort = "p";
    } else if (isNeutron(Particle)) {
        ParticleShort = "n";
    } else {
        ParticleShort = "nuc";
    }

    for (int Slice = 0; Slice < Loaded_particle_limits.size(); Slice++) {
        vector<vector<int>> Loaded_Particle_AMap_TempSlice;

        string TempFileName = ParticleShort + "_AMap_by_slice/" + ParticleShort + "_AMap_file_from_" +
                              to_string_with_precision(Loaded_particle_limits.at(Slice).at(0), 2) + "_to_" +
                              to_string_with_precision(Loaded_particle_limits.at(Slice).at(1), 2) + ".par";

        ReadAMap((AcceptanceMapsDirectory + SampleName + "/" + TempFileName).c_str(), Loaded_Particle_AMap_TempSlice);

        Loaded_Particle_AMap_Slices.push_back(Loaded_Particle_AMap_TempSlice);
    }
}
//</editor-fold>

// ReadAMap function ----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadAMap function">
void AMaps::ReadAMap(const char *filename, vector<vector<int>> &Loaded_particle_AMap) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        string tp;

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            string parameter, parameter2;
            ss >> parameter; // get cut identifier

            if (findSubstring(parameter, "Line")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                string LineEntry;
                vector<int> col;

                while (getline(ss2, LineEntry, ':')) { col.push_back(stoi(LineEntry)); }

                Loaded_particle_AMap.push_back(col);
            }
        }
    } else {
        cout << "\n\nReadAMap: file:\n" << filename << "\nwas not found! Exiting...\n\n", exit(0);
//        cout << "\n\nReadAMap: file not found! Exiting...\n\n", exit(0);
    }
}
//</editor-fold>

// MatchAngToHitMap function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MatchAngToHitMap function">
bool AMaps::MatchAngToHitMap(const string &Particle, double Momentum, double Theta, double Phi) {
    if (isElectron(Particle)) {
        for (int Slice = 0; Slice < Loaded_ElectronMomBinsLimits.size(); Slice++) {
            if (Momentum >= Loaded_ElectronMomBinsLimits.at(Slice).at(0) && Momentum <= Loaded_ElectronMomBinsLimits.at(Slice).at(1)) {
                for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / HistElectronSliceNumOfYBins;
                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                        for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / HistElectronSliceNumOfXBins;
                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                if (Loaded_e_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                    return true;
                                } else {
                                    return false;
                                }
                            } // end of find right phi if
                        }
                    } // end of find right theta if
                }
            } // end of if the right momentum
        }
    } else if (isProton(Particle)) {
        for (int Slice = 0; Slice < Loaded_PBinsLimits.size(); Slice++) {
            if (Momentum >= Loaded_PBinsLimits.at(Slice).at(0) && Momentum <= Loaded_PBinsLimits.at(Slice).at(1)) {
                for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                if (Loaded_p_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                    return true;
                                } else {
                                    return false;
                                }
                            } // end of find right phi if
                        }
                    } // end of find right theta if
                }
            } // end of if the right momentum
        }
    } else if (isNeutron(Particle)) {
        for (int Slice = 0; Slice < Loaded_PBinsLimits.size(); Slice++) {
            if (Momentum >= Loaded_PBinsLimits.at(Slice).at(0) && Momentum <= Loaded_PBinsLimits.at(Slice).at(1)) {
                for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                if (Loaded_n_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                    return true;
                                } else {
                                    return false;
                                }
                            } // end of find right phi if
                        }
                    } // end of find right theta if
                }
            } // end of if the right momentum
        }
    } // end of if Particle

    return false;
}
//</editor-fold>

// IsInFDQuery function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="IsInFDQuery function">
bool AMaps::IsInFDQuery(bool generate_AMaps, const DSCuts &ThetaFD, const string &Particle, double Momentum, double Theta, double Phi) {
    bool inFDQuery, part_inSomeSector;

    if (!generate_AMaps) {
        part_inSomeSector = MatchAngToHitMap(Particle, Momentum, Theta, Phi);
    } else {
        part_inSomeSector = true;
    }

    //TODO: ask Adi is I should do an FD angle cut on electrons
    inFDQuery = (part_inSomeSector && (Theta >= ThetaFD.GetLowerCutConst()) && (Theta <= ThetaFD.GetUpperCutConst()));

//    if (isElectron(Particle)) {
//        inFDQuery = (part_inSomeSector && (Theta >= ThetaFD.GetLowerCutConst()) && (Theta <= ThetaFD.GetUpperCutConst()));
//    } else {
//        inFDQuery = part_inSomeSector;
//    }

    return inFDQuery;
}
//</editor-fold>