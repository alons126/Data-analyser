//
// Created by alons on 22/06/2023.
//

#include "AMaps.h"
#include "CustomSliceLimits.h"

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
AMaps::AMaps(const string &SampleName, bool reformat_e_bins, bool varying_P_e_bins, bool varying_P_nuc_bins, double beamE, const string &AMapsMode, const string &SavePath,
             int nOfNucMomBins, int nOfElecMomBins, int hnsNumOfXBins, int hnsNumOfYBins, int hesNumOfXBins, int hesNumOfYBins) {
    AMaps_Mode = AMapsMode;
    SName = SampleName;
    AMapSavePath = SavePath;
    HistNucSliceNumOfXBins = hnsNumOfXBins;
    HistNucSliceNumOfYBins = hnsNumOfYBins;
    HistElectronSliceNumOfXBins = hesNumOfXBins;
    HistElectronSliceNumOfYBins = hesNumOfYBins;
    NumberNucOfMomSlices = nOfNucMomBins;
    NumberElecOfMomSlices = nOfElecMomBins;

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

    string AMapSavePathGeneratedAMapCopy = AMapSavePath + "05_Generated_maps/";
    system(("mkdir -p " + AMapSavePathGeneratedAMapCopy).c_str());
    AMapCopySavePath = AMapSavePathGeneratedAMapCopy;
    //</editor-fold>

    //<editor-fold desc="Setting AMapsMode_TitleAddition">
    string AMapsMode_TitleAddition;

    if (AMaps_Mode != "") {
        AMapsMode_TitleAddition = AMaps_Mode;
    } else {
        AMapsMode_TitleAddition = "";
    }
    //</editor-fold>

    SetBins(varying_P_nuc_bins, beamE);
    SetElectronBins(reformat_e_bins, varying_P_e_bins, beamE);

    //<editor-fold desc="Acceptance maps BC">
    string hStatsTitleAMapBCElectron = "Electron_AMap_BC", hTitleAMapBCElectron = "Electron AMap BC", hSaveNameAMapBCElectron = "01_e_AMap_BC";
    ElectronAMapBC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCElectron, hTitleAMapBCElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                             SavePathAMapsBC,
                             hSaveNameAMapBCElectron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins,
                             HistElectronSliceNumOfYBins);

    string hStatsTitleAMapBCProton = "Proton_AMap_BC", hTitleAMapBCProton = "Proton AMap BC", hSaveNameAMapBCProton = "02_p_AMap_BC";
    ProtonAMapBC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCProton, hTitleAMapBCProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                           SavePathAMapsBC,
                           hSaveNameAMapBCProton, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins,
                           HistNucSliceNumOfYBins);

    string hStatsTitleAMapBCNeutron = "Neutron_AMap_BC", hTitleAMapBCNeutron = "Neutron AMap BC", hSaveNameAMapBCNeutron = "03_n_AMap_BC";
    NeutronAMapBC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCNeutron, hTitleAMapBCNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                            SavePathAMapsBC,
                            hSaveNameAMapBCNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins,
                            HistNucSliceNumOfYBins);

    string hStatsTitleAMapBCNucleon = "Nucleon_AMap_BC", hTitleAMapBCNucleon = "Nucleon AMap BC", hSaveNameAMapBCNucleon = "04_nuc_AMap_BC";
    NucleonAMapBC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCNucleon, hTitleAMapBCNucleon, "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]",
                            SavePathAMapsBC,
                            hSaveNameAMapBCNucleon, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins,
                            HistNucSliceNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Setting electron histograms">
    for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
        double BinLowerLim = ElectronMomSliceLimits.at(i).at(0), BinUpperLim = ElectronMomSliceLimits.at(i).at(1);

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
        hPlot2D hPBinTLElectron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleTLElectron, hTitleTLElectron,
                                          "#phi_{e} [Deg]", "#theta_{e} [Deg]", AMapSavePathTLElectron, hSaveNameTLElectron,
                                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                          HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
        ElectronTLAMapsBySlice.push_back(hPBinTLElectron);
        //</editor-fold>

        //<editor-fold desc="Electron Reco. Acceptance maps">
        string hStatsTitleRecoElectron = "Reco P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                         to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoElectron = "Reco P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                    to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoElectron = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoElectron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoElectron, hTitleRecoElectron,
                                            "#phi_{e} [Deg]", "#theta_{e} [Deg]", AMapSavePathRecoElectron, hSaveNameRecoElectron,
                                            hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                            HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
        ElectronRecoAMapsBySlice.push_back(hPBinRecoElectron);
        //</editor-fold>

        //<editor-fold desc="Electron Reco./TL Ratio">
        string hStatsTitleRecoToTLRatioElectron =
                "Electron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                             to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoToTLRatioElectron = to_string(i + 1) + "_e_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioElectron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoToTLRatioElectron, hTitleRecoToTLRatioElectron,
                                                     "#phi_{e} [Deg]", "#theta_{e} [Deg]", AMapSavePathRecoToTLElectron,
                                                     hSaveNameRecoToTLRatioElectron,
                                                     hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                                     HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
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
        hPlot2D hPBinSepAMapsElectron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleSepAMapsElectron, hTitleSepAMapsElectron,
                                                "#phi_{e} [Deg]", "#theta_{e} [Deg]", AMapSavePathSepAMapsElectron, hSaveNameSepAMapsElectron,
                                                hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins,
                                                HistElectronSliceNumOfYBins);
        ElectronAMapsBySlice.push_back(hPBinSepAMapsElectron);
        //</editor-fold>

        //</editor-fold>
    }
    //</editor-fold>

    //<editor-fold desc="Setting nucleon histograms">

    //<editor-fold desc="Setting nucleon slice histograms">
    for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
        double BinLowerLim = NucleonMomSliceLimits.at(i).at(0), BinUpperLim = NucleonMomSliceLimits.at(i).at(1);

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
        hPlot2D hPBinTLProton = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleTLProton, hTitleTLProton,
                                        "#phi_{p} [Deg]", "#theta_{p} [Deg]", AMapSavePathTLProton, hSaveNameTLProton,
                                        hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                        HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        ProtonTLAMapsBySlice.push_back(hPBinTLProton);
        //</editor-fold>

        //<editor-fold desc="Proton Reco. Acceptance maps">
        string hStatsTitleRecoProton = "Reco P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoProton = "Reco P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoProton = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoProton = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoProton, hTitleRecoProton,
                                          "#phi_{p} [Deg]", "#theta_{p} [Deg]", AMapSavePathRecoProton, hSaveNameRecoProton,
                                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                          HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        ProtonRecoAMapsBySlice.push_back(hPBinRecoProton);
        //</editor-fold>

        //<editor-fold desc="Proton Reco./TL Ratio">
        string hStatsTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                           to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoToTLRatioProton = to_string(i + 1) + "_p_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                              to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioProton = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoToTLRatioProton, hTitleRecoToTLRatioProton,
                                                   "#phi_{p} [Deg]", "#theta_{p} [Deg]", AMapSavePathRecoToTLProton, hSaveNameRecoToTLRatioProton,
                                                   hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                                   HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
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
        hPlot2D hPBinSepAMapsProton = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleSepAMapsProton, hTitleSepAMapsProton,
                                              "#phi_{p} [Deg]", "#theta_{p} [Deg]", AMapSavePathSepAMapsProton, hSaveNameSepAMapsProton,
                                              hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                              HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
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
        hPlot2D hPBinTLNeutron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleTLNeutron, hTitleTLNeutron,
                                         "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathTLNeutron, hSaveNameTLNeutron,
                                         hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                         HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        NeutronTLAMapsBySlice.push_back(hPBinTLNeutron);
        //</editor-fold>

        //<editor-fold desc="Neutron Reco. Acceptance maps">
        string hStatsTitleRecoNeutron = "Reco P_{n} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                        to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoNeutron = "Reco P_{n} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                   to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoNeutron = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                      to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoNeutron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoNeutron, hTitleRecoNeutron,
                                           "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathRecoNeutron, hSaveNameRecoNeutron,
                                           hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                           HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        NeutronRecoAMapsBySlice.push_back(hPBinRecoNeutron);
        //</editor-fold>

        //<editor-fold desc="Neutron Reco./TL Ratio">
        string hStatsTitleRecoToTLRatioNeutron =
                "Neutron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioNeutron = "Neutron Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                            to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
        string hSaveNameRecoToTLRatioNeutron = to_string(i + 1) + "_p_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                               to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioNeutron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoToTLRatioNeutron, hTitleRecoToTLRatioNeutron,
                                                    "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathRecoToTLNeutron,
                                                    hSaveNameRecoToTLRatioNeutron,
                                                    hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                                    HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
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
        hPlot2D hPBinSepAMapsNeutron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleSepAMapsNeutron, hTitleSepAMapsNeutron,
                                               "#phi_{n} [Deg]", "#theta_{n} [Deg]", AMapSavePathSepAMapsNeutron, hSaveNameSepAMapsNeutron,
                                               hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim,
                                               HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
        NeutronAMapsBySlice.push_back(hPBinSepAMapsNeutron);
        //</editor-fold>

        //</editor-fold>

    }
    //</editor-fold>

    //<editor-fold desc="Setting neutron Acceptance maps">

    //<editor-fold desc="Neutron TL Acceptance maps">
    string hStatsTitleTLNeutron = "TL Neutron Hit Map", hTitleTLNeutron = "TL Neutron Hit Map", hSaveNameTLNeutron = "TL_n_AMap";
    NeutronTLAMap = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleTLNeutron, hTitleTLNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                            AMapSavePathTLNeutron,
                            hSaveNameTLNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins,
                            HistNucSliceNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Neutron Reco. Acceptance maps">
    string hStatsTitleRecoNeutron = "Reco Neutron Hit Map", hTitleRecoNeutron = "Reco Neutron Hit Map", hSaveNameRecoNeutron = "Reco_n_AMap";
    NeutronRecoAMap = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoNeutron, hTitleRecoNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                              AMapSavePathRecoNeutron,
                              hSaveNameRecoNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins,
                              HistNucSliceNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Neutron Reco./TL Ratio">
    string hStatsTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hSaveNameRecoToTLNeutron = "Neutron_Ratio";
    NeutronRecoToTLRatio = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoToTLNeutron, hTitleRecoToTLNeutron, "#phi_{n} [Deg]",
                                   "#theta_{n} [Deg]",
                                   AMapSavePathRecoToTLNeutron, hSaveNameRecoToTLNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim,
                                   hBinUpperYLim,
                                   HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Finalized acceptance maps">
    string hStatsTitleAMapElectron = "Electron_AMap";
    string hTitleAMapElectron = "Electron AMap for (Reco./TL)#geq" + to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapElectron = "01_e_AMap";
    ElectronAMap = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapElectron, hTitleAMapElectron, "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                           AMapSavePathAMap,
                           hSaveNameAMapElectron,
                           hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);

    string hStatsTitleAMapProton = "Proton_AMap";
    string hTitleAMapProton = "Proton AMap for (Reco./TL)#geq" + to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapProton = "02_p_AMap";
    ProtonAMap = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapProton, hTitleAMapProton, "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                         AMapSavePathAMap,
                         hSaveNameAMapProton, hBinLowerXLim,
                         hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    string hStatsTitleAMapNeutron = "Neutron_AMap";
    string hTitleAMapNeutron = "Neutron AMap for (Reco./TL)#geq" + to_string_with_precision(Neutral_particle_min_Ratio, 2);
    string hSaveNameAMapNeutron = "03_n_AMap";
    NeutronAMap = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapNeutron, hTitleAMapNeutron, "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                          AMapSavePathAMap,
                          hSaveNameAMapNeutron,
                          hBinLowerXLim,
                          hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    string hStatsTitleAMapNucleon = "Nucleon_AMap";
    string hTitleAMapNucleon =
            "Nucleon AMap for (Reco./TL)_{n}#geq" + to_string_with_precision(Neutral_particle_min_Ratio, 2) + " and (Reco./TL)_{c}#geq" +
            to_string_with_precision(Charged_particle_min_Ratio, 2);
    string hSaveNameAMapNucleon = "04_nuc_AMap";
    NucleonAMap = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapNucleon, hTitleAMapNucleon, "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]",
                          AMapSavePathAMap,
                          hSaveNameAMapNucleon,
                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    //</editor-fold>

}
//</editor-fold>

//<editor-fold desc="AMaps loading constructor">
AMaps::AMaps(const string &AcceptanceMapsDirectory, const string &SampleName,
             const bool &Electron_single_slice_test, const bool &Nucleon_single_slice_test, const vector<int> &TestSlices) {
    /* Load slices and their limits */
    ReadAMapLimits((AcceptanceMapsDirectory + SampleName + "/e_AMap_by_slice/e_slice_limits.par").c_str(), Loaded_ElectronMomSliceLimits);
    ReadAMapLimits((AcceptanceMapsDirectory + SampleName + "/p_AMap_by_slice/p_slice_limits.par").c_str(), Loaded_NucleonMomSliceLimits);

    /* Load separate maps */
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, Loaded_e_AMap_Slices);
    ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, Loaded_e_WMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, Loaded_p_AMap_Slices);
    ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, Loaded_p_WMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, Loaded_n_AMap_Slices);
    ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, Loaded_n_WMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Nucleon", Loaded_NucleonMomSliceLimits, Loaded_nuc_AMap_Slices);

    /* Load combined maps */
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/e_AMap_file.par").c_str(), Loaded_e_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/p_AMap_file.par").c_str(), Loaded_p_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/n_AMap_file.par").c_str(), Loaded_n_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/nuc_AMap_file.par").c_str(), Loaded_nuc_AMap);

    HistElectronSliceNumOfXBins = 100;  // 100 by Default
    HistElectronSliceNumOfYBins = 100;  // 100 by Default
    HistNucSliceNumOfXBins = 75;  // 75 by Default
    HistNucSliceNumOfYBins = 75;  // 75 by Default

    e_single_slice_test = Electron_single_slice_test, nuc_single_slice_test = Nucleon_single_slice_test;
    Slices2Test = TestSlices;
}
//</editor-fold>

// SetBins functions ----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetBins function">
void AMaps::SetBins(bool varying_P_nuc_bins, double beamE) {
    if (!varying_P_nuc_bins) {
        bool InvertedPrintOut = false;
        bool RegPrintOut = false;

        double InvertedPLowerLim = (1 / beamE);
        double InvertedPUpper = (1 / Nucleon_Momentum_Slice_Th);
        double Delta = (InvertedPUpper - InvertedPLowerLim) / NumberNucOfMomSlices;

        for (int i = 0; i < NumberNucOfMomSlices; i++) {
            double InvertedBinLower = InvertedPLowerLim + i * Delta;
            double InvertedBinUpper = InvertedBinLower + Delta;

            if (InvertedPrintOut) {
                cout << "\n\nInvertedBinLower = " << InvertedBinLower << "\n";
                cout << "InvertedBinUpper = " << InvertedBinUpper << "\n";
                cout << "i = " << i << "\n";
                cout << "Delta = " << Delta << "\n\n";
            }

            InvertedNucleonMomSliceLimits.push_back({InvertedBinLower, InvertedBinUpper});
        }

        if (InvertedPrintOut && !RegPrintOut) { exit(0); }

        for (int i = (NumberNucOfMomSlices - 1); i >= 0; i--) {
            double BinLower = 1 / InvertedNucleonMomSliceLimits.at(i).at(1);
            double BinUpper = 1 / InvertedNucleonMomSliceLimits.at(i).at(0);

            if (RegPrintOut) {
                cout << "\n\nBinLower = " << BinLower << "\n";
                cout << "BinUpper = " << BinUpper << "\n";
                cout << "i = " << i << "\n";
            }

            NucleonMomSliceLimits.push_back({BinLower, BinUpper});
        }

        if (RegPrintOut) { exit(0); }
    } else {
        //TODO: separate by SampleName?
        bool RegPrintOut = false;

        NucleonMomSliceLimits = CustomNucleonMomSliceLimits_C12x4_simulation_G18_Q204_6GeV;
        NumberNucOfMomSlices = NucleonMomSliceLimits.size();

        if (RegPrintOut) {
            for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
                cout << "\n\nSliceLowerLimit = " << NucleonMomSliceLimits.at(i).at(0) << "\n";
                cout << "SliceUpperLimit = " << NucleonMomSliceLimits.at(i).at(1) << "\n";
                cout << "i = " << i << "\n";
            }

            exit(0);
        }
    }
}
//</editor-fold>

//<editor-fold desc="SetElectronBins function">
void AMaps::SetElectronBins(bool reformat_e_bins, bool varying_P_e_bins, double beamE) {
    bool InvertedPrintOut = false;
    bool RegPrintOut = false;

    if (reformat_e_bins) {
        double InvertedPLowerLim = (1 / beamE);
        double InvertedPUpperLim = (1 / Nucleon_Momentum_Slice_Th);
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


            if (InvBinLower >= InvertedPLowerLim) {
                InvertedElectronMomSliceLimits.push_back({InvBinLower, InvBinUpper});
                ++iter;
            } else {
                SliceAndDice = false;
            }
        }

        NumOfElectronMomBins = InvertedElectronMomSliceLimits.size();

        if (InvertedPrintOut && !RegPrintOut) { exit(0); }

        if (RegPrintOut) { cout << "\n\n---------------------------------------------------\n"; }

        for (int i = 0; i < NumOfElectronMomBins; i++) {
            double BinLower = 1 / InvertedElectronMomSliceLimits.at(i).at(1);
            double BinUpper = 1 / InvertedElectronMomSliceLimits.at(i).at(0);

            if (RegPrintOut) {
                cout << "\n\nBinLower = " << BinLower << "\n";
                cout << "BinUpper = " << BinUpper << "\n";
                cout << "i = " << i << "\n";
            }

            ElectronMomSliceLimits.push_back({BinLower, BinUpper});
        }

        if (RegPrintOut) { exit(0); }
    } else if (varying_P_e_bins) {
        if (findSubstring(SName, "C12_simulation_6GeV_T5")) { // Old sample
            ElectronMomSliceLimits = CustomElectronMomSliceLimits_C12_simulation_6GeV_T5;
        } else { // New sample (24M; 1-foil & 4-foil)
            ElectronMomSliceLimits = CustomElectronMomSliceLimits_C12x4_simulation_G18_Q204_6GeV;
        }

        int NumOfElectronMomBins = ElectronMomSliceLimits.size();

        if (RegPrintOut) {
            for (int i = 0; i < NumOfElectronMomBins; i++) {
                cout << "\n\nElectronMomSliceLimits.at(" << i << ").at(" << 0 << ") = " << ElectronMomSliceLimits.at(i).at(0) << "\n";
                cout << "ElectronMomSliceLimits.at(" << i << ").at(" << 1 << ") = " << ElectronMomSliceLimits.at(i).at(1) << "\n";
            }
        }

        if (RegPrintOut) { exit(0); }
    } else {
        double InvertedPLowerLim = (1 / beamE);
        double InvertedPUpper = (1 / Electron_Momentum_Slice_Th);
        double Delta = (InvertedPUpper - InvertedPLowerLim) / NumberElecOfMomSlices;

        for (int i = 0; i < (NumberElecOfMomSlices - 1); i++) {
            double InvertedBinLower = InvertedPLowerLim + i * Delta;
            double InvertedBinUpper = InvertedBinLower + Delta;

            if (InvertedPrintOut) {
                cout << "\n\nInvertedBinLower = " << InvertedBinLower << "\n";
                cout << "InvertedBinUpper = " << InvertedBinUpper << "\n";
                cout << "i = " << i << "\n";
                cout << "Delta = " << Delta << "\n\n";
            }

            InvertedElectronMomSliceLimits.push_back({InvertedBinLower, InvertedBinUpper});
        }

        if (InvertedPrintOut && !RegPrintOut) { exit(0); }

        if (RegPrintOut) { cout << "\n\n---------------------------------------------\n\n"; }

        for (int i = 0; i < (NumberElecOfMomSlices - 1); i++) {
            double BinLower = beamE - (1 / InvertedElectronMomSliceLimits.at(i).at(0));
            double BinUpper = beamE - (1 / InvertedElectronMomSliceLimits.at(i).at(1));

            ElectronMomSliceLimits.push_back({BinLower, BinUpper});
        }

        ElectronMomSliceLimits.push_back({ElectronMomSliceLimits.at(ElectronMomSliceLimits.size() - 1).at(1), beamE});

        if (RegPrintOut) {
            for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
                double BinLower = ElectronMomSliceLimits.at(i).at(0);
                double BinUpper = ElectronMomSliceLimits.at(i).at(1);

                cout << "\nBinLower = " << BinLower << "\n";
                cout << "BinUpper = " << BinUpper << "\n";
                cout << "i = " << i << "\n\n";
            }
        }

        if (RegPrintOut) { exit(0); }
    }
}
//</editor-fold>

////<editor-fold desc="SetElectronBins function (original)">
//void AMaps::SetElectronBins(bool reformat_e_bins, bool varying_P_e_bins, double beamE) {
//    bool InvertedPrintOut = false;
//    bool RegPrintOut = false;
//
//    if (reformat_e_bins) {
//        double InvertedPLowerLim = (1 / beamE);
//        double InvertedPUpperLim = (1 / Nucleon_Momentum_Slice_Th);
//        double p2 = 1.57819;
//        double Delta = (InvertedPUpperLim - InvertedPLowerLim) / NumberNucOfMomSlices;
//        double delta = (InvertedPUpperLim - (1 / p2));
//        double Ratio = (InvertedPUpperLim - InvertedPLowerLim) / delta;
//
//        int NumOfElectronMomBins;
//
//        bool SliceAndDice = true;
//        int iter = 0;
//
//        while (SliceAndDice) {
//            double InvBinLower;
//            double InvBinUpper;
//            double deltaLoop = delta;
//
//            if (iter == 0) {
//                InvBinUpper = InvertedPUpperLim;
//                InvBinLower = InvBinUpper - deltaLoop;
//            } else {
//                InvBinUpper = InvBinLower;
//
//                deltaLoop = delta / 12;
//
//                if (InvBinUpper - deltaLoop > 0) {
//                    InvBinLower = InvBinUpper - deltaLoop;
//                } else {
//                    InvBinLower = InvertedPLowerLim;
//                }
//            }
//
//            if (InvertedPrintOut) {
//                cout << "\n\nInvBinLower = " << InvBinLower << "\n";
//                cout << "InvBinUpper = " << InvBinUpper << "\n";
//                cout << "iter = " << iter << "\n";
//                cout << "delta = " << delta << "\n";
//                cout << "deltaLoop = " << deltaLoop << "\n\n";
//            }
//
////            InvertedElectronMomSliceLimits.push_back({InvBinLower, InvBinUpper});
//
////            if (InvBinUpper >= InvertedPLowerLim) {
//            if (InvBinLower >= InvertedPLowerLim) {
////            if (InvBinUpper - deltaLoop >= InvertedPLowerLim) {
//                InvertedElectronMomSliceLimits.push_back({InvBinLower, InvBinUpper});
//                ++iter;
//            } else {
//                SliceAndDice = false;
//            }
//        }
//
//        NumOfElectronMomBins = InvertedElectronMomSliceLimits.size();
//
//        if (InvertedPrintOut && !RegPrintOut) { exit(0); }
//
//        if (RegPrintOut) { cout << "\n\n---------------------------------------------------\n"; }
//
//        for (int i = 0; i < NumOfElectronMomBins; i++) {
////        for (int i = (NumOfElectronMomBins - 1); i >= 0; i--) {
////        for (int i = (NumberNucOfMomSlices - 2); i >= 0; i--) {
////        for (int i = (NumOfElectronMomBins - 1); i >= 0; i--) {
//            double BinLower = 1 / InvertedElectronMomSliceLimits.at(i).at(1);
//            double BinUpper = 1 / InvertedElectronMomSliceLimits.at(i).at(0);
//
//            if (RegPrintOut) {
//                cout << "\n\nBinLower = " << BinLower << "\n";
//                cout << "BinUpper = " << BinUpper << "\n";
//                cout << "i = " << i << "\n";
//            }
//
//            ElectronMomSliceLimits.push_back({BinLower, BinUpper});
//        }
//
//        if (RegPrintOut) { exit(0); }
//    } else if (varying_P_e_bins) {
//        if (findSubstring(SName, "C12_simulation_6GeV_T5")) { // Old sample
//            ElectronMomSliceLimits = {{0.4, 1.6},
//                                     {1.6, 2.2},
//                                     {2.2, 2.8},
//                                     {2.8, 4},
//                                     {4,   5},
//                                     {5,   6}};
//        } else { // New sample (24M)
//            ElectronMomSliceLimits = {{0.4, 0.6},
//                                     {0.6, 0.8},
//                                     {0.8, 1.2},
//                                     {1.2, 1.6},
//                                     {1.6, 2.2},
//                                     {2.2, 2.8},
//                                     {2.8, 4},
//                                     {4,   5},
//                                     {5,   6}};
//        }
//
//        int NumOfElectronMomBins = ElectronMomSliceLimits.size();
//
//        if (RegPrintOut) {
//            for (int i = 0; i < NumOfElectronMomBins; i++) {
//                cout << "\n\nElectronMomSliceLimits.at(" << i << ").at(" << 0 << ") = " << ElectronMomSliceLimits.at(i).at(0) << "\n";
//                cout << "ElectronMomSliceLimits.at(" << i << ").at(" << 1 << ") = " << ElectronMomSliceLimits.at(i).at(1) << "\n";
//            }
//        }
//
//        if (RegPrintOut) { exit(0); }
//    } else {
//        double InvertedPLowerLim = (1 / beamE);
//        double InvertedPUpperLim = (1 / Nucleon_Momentum_Slice_Th);
//        double Delta = (InvertedPUpperLim - InvertedPLowerLim) / NumberNucOfMomSlices;
//
//        for (int i = 0; i < NumberNucOfMomSlices; i++) {
//            double InvertedBinLower = InvertedPLowerLim + i * Delta;
//            double InvertedBinUpper = InvertedBinLower + Delta;
//
//            if (InvertedPrintOut) {
//                cout << "\n\nInvertedBinLower = " << InvertedBinLower << "\n";
//                cout << "InvertedBinUpper = " << InvertedBinUpper << "\n";
//                cout << "i = " << i << "\n";
//                cout << "Delta = " << Delta << "\n\n";
//            }
//
//            InvertedElectronMomSliceLimits.push_back({InvertedBinLower, InvertedBinUpper});
//        }
//
//        if (InvertedPrintOut && !RegPrintOut) { exit(0); }
//
//        for (int i = (NumberNucOfMomSlices - 1); i >= 0; i--) {
//            double BinLower = 1 / InvertedElectronMomSliceLimits.at(i).at(1);
//            double BinUpper = 1 / InvertedElectronMomSliceLimits.at(i).at(0);
//
//            if (RegPrintOut) {
//                cout << "\n\nBinLower = " << BinLower << "\n";
//                cout << "BinUpper = " << BinUpper << "\n";
//                cout << "i = " << i << "\n";
//            }
//
//            ElectronMomSliceLimits.push_back({BinLower, BinUpper});
//        }
//
//        if (RegPrintOut) { exit(0); }
//    }
//}
////</editor-fold>

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

        NucleonMomSliceLimits.push_back({LowerLim, UpperLim});

        if (LowerLim <= Nucleon_Momentum_Slice_Th) {
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

// isReco function --------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="isReco function">
bool AMaps::isReco(const string &SampleType) {
    if (SampleType == "Reco" || SampleType == "Reconstruction") {
        return true;
    } else {
        return false;
    }
}
//</editor-fold>

// hFillHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hFillHitMaps function">
void AMaps::hFillHitMaps(const string &SampleType, const string &particle, double Momentum, double Theta, double Phi, double Weight) {
    bool is_e = isElectron(particle), is_p = isProton(particle), is_n = isNeutron(particle);
    bool is_TL = isTL(SampleType), is_Reco = isReco(SampleType);

    bool TL_e_PrintOut = false, TL_p_PrintOut = false, Reco_e_PrintOut = false, Reco_p_PrintOut = false;

    //<editor-fold desc="Safety checks (AMaps::hFillHitMaps)">
    if (is_e && is_p && is_n) { cout << "\n\nAMaps::hFillHitMaps: particle can't all particles! Exiting...\n", exit(0); }
    if (!is_e && !is_p && !is_n) { cout << "\n\nAMaps::hFillHitMaps: particle must be an electron, proton or neutron! Exiting...\n", exit(0); }
    if (is_e && is_p) { cout << "\n\nAMaps::hFillHitMaps: particle can't be both electrons and protons! Exiting...\n", exit(0); }
    if (is_e && is_n) { cout << "\n\nAMaps::hFillHitMaps: particle can't be both electrons and neutrons! Exiting...\n", exit(0); }
    if (is_p && is_n) { cout << "\n\nAMaps::hFillHitMaps: particle can't be both protons and neutrons! Exiting...\n", exit(0); }

    if (is_TL && is_Reco) { cout << "\n\nAMaps::hFillHitMaps: particle can't be both TL and Reco! Exiting...\n", exit(0); }
    if (!is_TL && !is_Reco) { cout << "\n\nAMaps::hFillHitMaps: particle must be either TL and Reco! Exiting...\n", exit(0); }
    //</editor-fold>

    if (is_TL) {
        if (is_e) { // electrons are charged -> look for correct momentum slice!
            for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
                if ((Momentum >= ElectronMomSliceLimits.at(i).at(0)) && (Momentum < ElectronMomSliceLimits.at(i).at(1))) {
                    ElectronTLAMapsBySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && TL_e_PrintOut) {
                        cout << "\n";
                        cout << "ElectronMomSliceLimits.at(" << i << ").at(0) = " << ElectronMomSliceLimits.at(i).at(0) << "\n";
                        cout << "ElectronMomSliceLimits.at(" << i << ").at(1) = " << ElectronMomSliceLimits.at(i).at(1) << "\n";
                        cout << "particle = " << particle << "\n";
                        cout << "is_e = " << is_e << "\n";
                        cout << "is_TL = " << is_TL << "\n";
                        cout << "is_Reco = " << is_Reco << "\n";
                        cout << "Momentum = " << Momentum << "\n";
                        cout << "Theta = " << Theta << "\n";
                        cout << "Phi = " << Phi << "\n";
                    }

                    //<editor-fold desc="Safety checks (AMaps::hFillHitMaps)">
                    if (ElectronMomSliceLimits.at(i).at(0) > ElectronMomSliceLimits.at(i).at(1)) {
                        cout << "\n\nAMaps::hFillHitMaps: electron momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
                    //</editor-fold>

                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p) { // protons are charged -> look for correct momentum slice!
            for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
                if ((Momentum >= NucleonMomSliceLimits.at(i).at(0)) && (Momentum < NucleonMomSliceLimits.at(i).at(1))) {
                    ProtonTLAMapsBySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && TL_p_PrintOut) {
                        cout << "\n";
                        cout << "NucleonMomSliceLimits.at(" << i << ").at(0) = " << NucleonMomSliceLimits.at(i).at(0) << "\n";
                        cout << "NucleonMomSliceLimits.at(" << i << ").at(1) = " << NucleonMomSliceLimits.at(i).at(1) << "\n";
                        cout << "particle = " << particle << "\n";
                        cout << "is_p = " << is_p << "\n";
                        cout << "is_TL = " << is_TL << "\n";
                        cout << "is_Reco = " << is_Reco << "\n";
                        cout << "Momentum = " << Momentum << "\n";
                        cout << "Theta = " << Theta << "\n";
                        cout << "Phi = " << Phi << "\n";
                    }

                    //<editor-fold desc="Safety checks (AMaps::hFillHitMaps)">
                    if (NucleonMomSliceLimits.at(i).at(0) > NucleonMomSliceLimits.at(i).at(1)) {
                        cout << "\n\nAMaps::hFillHitMaps: nucleon momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
                    //</editor-fold>

                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n) { // neutrons are neutral -> same fill all slices!
            NeutronTLAMap.hFill(Phi, Theta, Weight);

            for (int Bin = 0; Bin < NucleonMomSliceLimits.size(); Bin++) {
                NeutronTLAMapsBySlice.at(Bin).hFill(Phi, Theta, Weight);
            }
        }
    } else if (is_Reco) {
        if (is_e) { // electrons are charged -> look for correct momentum slice!
            ElectronAMapBC.hFill(Phi, Theta, Weight);

            for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
                if ((Momentum >= ElectronMomSliceLimits.at(i).at(0)) && (Momentum < ElectronMomSliceLimits.at(i).at(1))) {
                    ElectronRecoAMapsBySlice.at(i).hFill(Phi, Theta, Weight);
                    ElectronRecoToTLRatioBySlice.at(i).hFill(Phi, Theta, Weight);
                    ElectronAMapsBySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && Reco_e_PrintOut) {
                        cout << "\n";
                        cout << "ElectronMomSliceLimits.at(" << i << ").at(0) = " << ElectronMomSliceLimits.at(i).at(0) << "\n";
                        cout << "ElectronMomSliceLimits.at(" << i << ").at(1) = " << ElectronMomSliceLimits.at(i).at(1) << "\n";
                        cout << "particle = " << particle << "\n";
                        cout << "is_p = " << is_p << "\n";
                        cout << "is_TL = " << is_TL << "\n";
                        cout << "is_Reco = " << is_Reco << "\n";
                        cout << "Momentum = " << Momentum << "\n";
                        cout << "Theta = " << Theta << "\n";
                        cout << "Phi = " << Phi << "\n";
                    }

                    //<editor-fold desc="Safety checks (AMaps::hFillHitMaps)">
                    if (ElectronMomSliceLimits.at(i).at(0) > ElectronMomSliceLimits.at(i).at(1)) {
                        cout << "\n\nAMaps::hFillHitMaps: electron momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
                    //</editor-fold>

                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p) { // protons are charged -> look for correct momentum slice!
            ProtonAMapBC.hFill(Phi, Theta, Weight);

            for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
                if ((Momentum >= NucleonMomSliceLimits.at(i).at(0)) && (Momentum < NucleonMomSliceLimits.at(i).at(1))) {
                    ProtonRecoAMapsBySlice.at(i).hFill(Phi, Theta, Weight);
                    ProtonRecoToTLRatioBySlice.at(i).hFill(Phi, Theta, Weight);
                    ProtonAMapsBySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && Reco_p_PrintOut) {
                        cout << "\n";
                        cout << "NucleonMomSliceLimits.at(" << i << ").at(0) = " << NucleonMomSliceLimits.at(i).at(0) << "\n";
                        cout << "NucleonMomSliceLimits.at(" << i << ").at(1) = " << NucleonMomSliceLimits.at(i).at(1) << "\n";
                        cout << "particle = " << particle << "\n";
                        cout << "is_p = " << is_p << "\n";
                        cout << "is_TL = " << is_TL << "\n";
                        cout << "is_Reco = " << is_Reco << "\n";
                        cout << "Momentum = " << Momentum << "\n";
                        cout << "Theta = " << Theta << "\n";
                        cout << "Phi = " << Phi << "\n";
                    }

                    //<editor-fold desc="Safety checks (AMaps::hFillHitMaps)">
                    if (NucleonMomSliceLimits.at(i).at(0) > NucleonMomSliceLimits.at(i).at(1)) {
                        cout << "\n\nAMaps::hFillHitMaps: nucleon momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
                    //</editor-fold>

                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n) { // neutrons are neutral -> same fill all slices!
            NeutronAMapBC.hFill(Phi, Theta, Weight);

            NeutronRecoAMap.hFill(Phi, Theta, Weight);
            NeutronRecoToTLRatio.hFill(Phi, Theta, Weight);
            NeutronAMap.hFill(Phi, Theta, Weight);

            for (int Bin = 0; Bin < NucleonMomSliceLimits.size(); Bin++) {
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
        for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
            if (ElectronRecoToTLDiv) {
                ElectronRecoToTLRatioBySlice.at(i).hDivision(ElectronTLAMapsBySlice.at(i).GetHistogram2D());
            }
        }
    }

    if (ProtonRecoToTLDiv) {
        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            if (ProtonRecoToTLDiv) { ProtonRecoToTLRatioBySlice.at(i).hDivision(ProtonTLAMapsBySlice.at(i).GetHistogram2D()); }
        }
    }

    if (NeutronRecoToTLDiv) {
        NeutronRecoToTLRatio.hDivision(NeutronTLAMap.GetHistogram2D());

        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            if (NeutronRecoToTLDiv) { NeutronRecoToTLRatioBySlice.at(i).hDivision(NeutronTLAMapsBySlice.at(i).GetHistogram2D()); }
        }
    }
}
//</editor-fold>

// GenerateSeparateCPartAMaps function ----------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateSeparateCPartAMaps function">
void AMaps::GenerateSeparateCPartAMaps(double cP_minR) {
    for (int bin = 0; bin < ElectronMomSliceLimits.size(); bin++) {
        for (int i = 0; i < (HistElectronSliceNumOfXBins + 1); i++) {
            for (int j = 0; j < (HistElectronSliceNumOfYBins + 1); j++) {
                if (ElectronRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) {
                    ElectronAMapsBySlice.at(bin).hFillByBin(i, j, 0);
                }
            }
        }

        if (AMaps_Mode == "AMaps") {
            //TODO: move from here
            ElectronRecoToTLRatioBySlice.at(bin).ApplyZMaxLim(1.2);
        }

        //<editor-fold desc="Fill e_AMap_Slices">
        vector <vector<int>> e_AMap_slice;
        vector <vector<double>> e_WMap_slice;

        for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
            vector<int> e_AMap_slice_col;
            vector<double> e_WMap_slice_col;

            for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                if (ElectronRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                    e_AMap_slice_col.push_back(1);
                    e_WMap_slice_col.push_back(ElectronRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1));
                } else {
                    e_AMap_slice_col.push_back(0);
                    e_WMap_slice_col.push_back(0);
                }
            }

            e_AMap_slice.push_back(e_AMap_slice_col);
            e_WMap_slice.push_back(e_WMap_slice_col);
        }

        e_AMap_Slices.push_back(e_AMap_slice);
        e_WMap_Slices.push_back(e_WMap_slice);
        //</editor-fold>

    }

    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
        for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
            for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                if (ProtonRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) {
                    ProtonAMapsBySlice.at(bin).hFillByBin(i, j, 0);
                }
            }
        }

        if (AMaps_Mode == "AMaps") {
            //TODO: move from here
            ProtonRecoToTLRatioBySlice.at(bin).ApplyZMaxLim(1.2);
        }

        //<editor-fold desc="Fill p_AMap_Slices">
        vector <vector<int>> p_AMap_slice;
        vector <vector<double>> p_WMap_slice;

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            vector<int> p_AMap_slice_col;
            vector<double> p_WMap_slice_col;

            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (ProtonRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                    p_AMap_slice_col.push_back(1);
                    p_WMap_slice_col.push_back(ProtonRecoToTLRatioBySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1));
                } else {
                    p_AMap_slice_col.push_back(0);
                    p_WMap_slice_col.push_back(0);
                }
            }

            p_AMap_slice.push_back(p_AMap_slice_col);
            p_WMap_slice.push_back(p_WMap_slice_col);
        }

        p_AMap_Slices.push_back(p_AMap_slice);
        p_WMap_Slices.push_back(p_WMap_slice);
        //</editor-fold>

    }
}
//</editor-fold>

// GenerateCPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateCPartAMaps function">
void AMaps::GenerateCPartAMaps(double cP_minR) {
    GenerateSeparateCPartAMaps(cP_minR);

    for (int bin = 0; bin < ElectronMomSliceLimits.size(); bin++) { ElectronAMap.hAdd(ElectronAMapsBySlice.at(bin).GetHistogram2D()); }

    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) { ProtonAMap.hAdd(ProtonAMapsBySlice.at(bin).GetHistogram2D()); }

    for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
        vector<int> e_AMap_col;

        for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
            if (ElectronAMap.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                e_AMap_col.push_back(1);
            } else {
                e_AMap_col.push_back(0);
            }
        }

        e_AMap.push_back(e_AMap_col);
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> p_AMap_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (ProtonAMap.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                p_AMap_col.push_back(1);
            } else {
                p_AMap_col.push_back(0);
            }
        }

        p_AMap.push_back(p_AMap_col);
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

                for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) { NeutronAMapsBySlice.at(bin).hFillByBin(i, j, 0); }
            }
        }
    }

    if (AMaps_Mode == "AMaps") {
        for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
            NeutronRecoToTLRatioBySlice.at(bin).ApplyZMaxLim(1.2);
        }

        NeutronRecoToTLRatio.ApplyZMaxLim(1.2);
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> n_AMap_col;
        vector<double> n_WMap_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (NeutronRecoToTLRatio.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= nP_minR) {
                n_AMap_col.push_back(1);
                n_WMap_col.push_back(NeutronRecoToTLRatio.GetHistogram2D()->GetBinContent(j + 1, i + 1));
            } else {
                n_AMap_col.push_back(0);
                n_WMap_col.push_back(0);
            }
        }

        n_AMap.push_back(n_AMap_col);
        n_WMap.push_back(n_WMap_col);
    }

    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
        NeutronAMapsBySlice.push_back(NeutronAMap);
        n_AMap_Slices.push_back(n_AMap);
        n_WMap_Slices.push_back(n_WMap);
    }
}
//</editor-fold>

// GenerateNucleonAMap function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GenerateNucleonAMap function">
void AMaps::GenerateNucleonAMap() {
    for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
        for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
            if ((NeutronAMapBC.GetHistogram2D()->GetBinContent(i, j) != 0) &&
                (ProtonAMapBC.GetHistogram2D()->GetBinContent(i, j) != 0)) {
                NucleonAMapBC.hFillByBin(i, j, NeutronRecoAMap.GetHistogram2D()->GetBinContent(i, j));
            }

            if ((NeutronAMap.GetHistogram2D()->GetBinContent(i, j) != 0) &&
                (ProtonAMap.GetHistogram2D()->GetBinContent(i, j) != 0)) {
                NucleonAMap.hFillByBin(i, j, NeutronRecoAMap.GetHistogram2D()->GetBinContent(i, j));
            }
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

    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
        vector <vector<int>> nuc_slice;

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
        nuc_WMap_Slices.push_back(nuc_slice); //TODO: figure out if really need these!
    }
}
//</editor-fold>

// SaveHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SaveHitMaps function">

//TODO: separate into AMaps and WMaps

void AMaps::SaveHitMaps(const string &SampleName, const string &AcceptanceMapsDirectory) {
    string AMapSliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceElectronSavePath).c_str());
    string WMapSliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceElectronSavePath).c_str());
    string AMapSliceElectronSavePathCopy = AMapCopySavePath + "/e_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceElectronSavePathCopy).c_str());
    string WMapSliceElectronSavePathCopy = AMapCopySavePath + "/e_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceElectronSavePathCopy).c_str());

    string AMapSliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceProtonSavePath).c_str());
    string WMapSliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceProtonSavePath).c_str());
    string AMapSliceProtonSavePathCopy = AMapCopySavePath + "/p_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceProtonSavePathCopy).c_str());
    string WMapSliceProtonSavePathCopy = AMapCopySavePath + "/p_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceProtonSavePathCopy).c_str());

    string AMapSliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceNeutronSavePath).c_str());
    string WMapSliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNeutronSavePath).c_str());
    string AMapSliceNeutronSavePathCopy = AMapCopySavePath + "/n_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceNeutronSavePathCopy).c_str());
    string WMapSliceNeutronSavePathCopy = AMapCopySavePath + "/n_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNeutronSavePathCopy).c_str());

    string AMapSliceNucleonSavePath = AcceptanceMapsDirectory + SampleName + "/nuc_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceNucleonSavePath).c_str());
    string WMapSliceNucleonSavePath = AcceptanceMapsDirectory + SampleName + "/nuc_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNucleonSavePath).c_str());
    string AMapSliceNucleonSavePathCopy = AMapCopySavePath + "/nuc_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceNucleonSavePathCopy).c_str());
    string WMapSliceNucleonSavePathCopy = AMapCopySavePath + "/nuc_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNucleonSavePathCopy).c_str());

    //<editor-fold desc="Save electron slices">
    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        ofstream e_AMap_TempFile, e_WMap_TempFile;

        string AMapTempFileName = "e_AMap_file_from_" + to_string_with_precision(ElectronMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                  to_string_with_precision(ElectronMomSliceLimits.at(Slice).at(1), 2) + ".par";
        string WMapTempFileName = "e_WMap_file_from_" + to_string_with_precision(ElectronMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                  to_string_with_precision(ElectronMomSliceLimits.at(Slice).at(1), 2) + ".par";

        e_AMap_TempFile.open(AMapSliceElectronSavePath + AMapTempFileName);
        e_WMap_TempFile.open(WMapSliceElectronSavePath + WMapTempFileName);

        e_AMap_TempFile << "Lower_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(0) << "\n";
        e_AMap_TempFile << "Upper_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
        e_AMap_TempFile << "\n";

        for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
            e_AMap_TempFile << "Line\t";
            e_WMap_TempFile << "Weight\t";

            for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                if (j != HistElectronSliceNumOfXBins - 1) {
                    e_AMap_TempFile << e_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    e_WMap_TempFile << e_WMap_Slices.at(Slice).at(i).at(j) << ":";
                } else {
                    e_AMap_TempFile << e_AMap_Slices.at(Slice).at(i).at(j);
                    e_WMap_TempFile << e_WMap_Slices.at(Slice).at(i).at(j);
                }
            }

            e_AMap_TempFile << "\n";
            e_WMap_TempFile << "\n";
        }

        e_AMap_TempFile.close();
        e_WMap_TempFile.close();

        system(("cp " + AMapSliceElectronSavePath + AMapTempFileName + " " + AMapSliceElectronSavePathCopy + AMapTempFileName).c_str());
        system(("cp " + WMapSliceElectronSavePath + WMapTempFileName + " " + WMapSliceElectronSavePathCopy + WMapTempFileName).c_str());
    }
    //</editor-fold>

    //<editor-fold desc="Save proton, neutron & nucleon slices">
    for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
        ofstream p_AMap_TempFile, p_WMap_TempFile, n_AMap_TempFile, n_WMap_TempFile, nuc_AMap_TempFile, nuc_WMap_TempFile;

        string ProtonAMapTempFileName = "p_AMap_file_from_" + to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                        to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        string ProtonWMapTempFileName = "p_WMap_file_from_" + to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                        to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        string NeutronAMapTempFileName = "n_AMap_file_from_" + to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                         to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        string NeutronWMapTempFileName = "n_WMap_file_from_" + to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                         to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        string NucleonAMapTempFileName = "nuc_AMap_file_from_" + to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                         to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        string NucleonWMapTempFileName = "nuc_WMap_file_from_" + to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                         to_string_with_precision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";

        p_AMap_TempFile.open(AMapSliceProtonSavePath + ProtonAMapTempFileName);
        p_WMap_TempFile.open(WMapSliceProtonSavePath + ProtonWMapTempFileName);
        n_AMap_TempFile.open(AMapSliceNeutronSavePath + NeutronAMapTempFileName);
        n_WMap_TempFile.open(WMapSliceNeutronSavePath + NeutronWMapTempFileName);
        nuc_AMap_TempFile.open(AMapSliceNucleonSavePath + NucleonAMapTempFileName);
        nuc_WMap_TempFile.open(WMapSliceNucleonSavePath + NucleonWMapTempFileName);

        p_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        p_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        p_AMap_TempFile << "\n";
        n_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        n_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        n_AMap_TempFile << "\n";
        nuc_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        nuc_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        nuc_AMap_TempFile << "\n";

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            p_AMap_TempFile << "Line\t";
            p_WMap_TempFile << "Weight\t";
            n_AMap_TempFile << "Line\t";
            n_WMap_TempFile << "Weight\t";
            nuc_AMap_TempFile << "Line\t";
            nuc_WMap_TempFile << "Weight\t";

            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (j != HistNucSliceNumOfXBins - 1) {
                    p_AMap_TempFile << p_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    p_WMap_TempFile << p_WMap_Slices.at(Slice).at(i).at(j) << ":";
                    n_AMap_TempFile << n_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    n_WMap_TempFile << n_WMap_Slices.at(Slice).at(i).at(j) << ":";
                    nuc_AMap_TempFile << nuc_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    nuc_WMap_TempFile << nuc_WMap_Slices.at(Slice).at(i).at(j) << ":";
                } else {
                    p_AMap_TempFile << p_AMap_Slices.at(Slice).at(i).at(j);
                    p_WMap_TempFile << p_WMap_Slices.at(Slice).at(i).at(j);
                    n_AMap_TempFile << n_AMap_Slices.at(Slice).at(i).at(j);
                    n_WMap_TempFile << n_WMap_Slices.at(Slice).at(i).at(j);
                    nuc_AMap_TempFile << nuc_AMap_Slices.at(Slice).at(i).at(j);
                    nuc_WMap_TempFile << nuc_WMap_Slices.at(Slice).at(i).at(j);
                }
            }

            p_AMap_TempFile << "\n";
            p_WMap_TempFile << "\n";
            n_AMap_TempFile << "\n";
            n_WMap_TempFile << "\n";
            nuc_AMap_TempFile << "\n";
            nuc_WMap_TempFile << "\n";
        }

        p_AMap_TempFile.close();
        p_WMap_TempFile.close();
        n_AMap_TempFile.close();
        n_WMap_TempFile.close();
        nuc_AMap_TempFile.close();
        nuc_WMap_TempFile.close();

        system(("cp " + AMapSliceProtonSavePath + ProtonAMapTempFileName + " " + AMapSliceProtonSavePathCopy + ProtonAMapTempFileName).c_str());
        system(("cp " + WMapSliceProtonSavePath + ProtonWMapTempFileName + " " + WMapSliceProtonSavePathCopy + ProtonWMapTempFileName).c_str());
        system(("cp " + AMapSliceNeutronSavePath + NeutronAMapTempFileName + " " + AMapSliceNeutronSavePathCopy +
                NeutronAMapTempFileName).c_str());
        system(("cp " + WMapSliceNeutronSavePath + NeutronWMapTempFileName + " " + WMapSliceNeutronSavePathCopy +
                NeutronWMapTempFileName).c_str());
        system(("cp " + AMapSliceNucleonSavePath + NucleonAMapTempFileName + " " + AMapSliceNucleonSavePathCopy +
                NucleonAMapTempFileName).c_str());
        system(("cp " + WMapSliceNucleonSavePath + NucleonWMapTempFileName + " " + WMapSliceNucleonSavePathCopy +
                NucleonWMapTempFileName).c_str());
    }
    //</editor-fold>

    //<editor-fold desc="Save combined maps">
    //TODO: figure out rather or not to keep these combind maps!
    ofstream e_AMap_file, p_AMap_file, n_AMap_file, nuc_AMap_file;

    e_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/e_AMap_file.par");
    p_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/p_AMap_file.par");
    n_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/n_AMap_file.par");
    nuc_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/nuc_AMap_file.par");
//    e_AMap_file.open(AMapCopySavePath + "e_AMap_file.par");
//    p_AMap_file.open(AMapCopySavePath + "p_AMap_file.par");
//    n_AMap_file.open(AMapCopySavePath + "n_AMap_file.par");
//    nuc_AMap_file.open(AMapCopySavePath + "nuc_AMap_file.par");
////    e_AMap_file.open(AMapSavePath + "e_AMap_file.par");
////    p_AMap_file.open(AMapSavePath + "p_AMap_file.par");
////    n_AMap_file.open(AMapSavePath + "n_AMap_file.par");
////    nuc_AMap_file.open(AMapSavePath + "nuc_AMap_file.par");

    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        e_AMap_file << "e_slice_" << (Slice + 1) << "\t" << ElectronMomSliceLimits.at(Slice).at(0) << ":" << ElectronMomSliceLimits.at(Slice).at(1)
                    << "\n";
    }

    for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
        p_AMap_file << "p_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1)
                    << "\n";
        n_AMap_file << "n_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1)
                    << "\n";
        nuc_AMap_file << "nuc_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":"
                      << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
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

    system(("cp " + AcceptanceMapsDirectory + SampleName + "/e_AMap_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/p_AMap_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/n_AMap_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/nuc_AMap_file.par " + AMapCopySavePath).c_str());
    //</editor-fold>

    //<editor-fold desc="Slice limits">
    ofstream e_slice_limits, p_slice_limits, n_slice_limits, nuc_slice_limits;

    e_slice_limits.open(AMapSliceElectronSavePath + "e_slice_limits.par");
    p_slice_limits.open(AMapSliceProtonSavePath + "p_slice_limits.par");
    n_slice_limits.open(AMapSliceNeutronSavePath + "n_slice_limits.par");
    nuc_slice_limits.open(AMapSliceNucleonSavePath + "nuc_slice_limits.par");

    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        e_slice_limits << "e_slice_" << (Slice + 1) << "\t" << ElectronMomSliceLimits.at(Slice).at(0) << ":"
                       << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
    }

    for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
        p_slice_limits << "p_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":"
                       << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        n_slice_limits << "n_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":"
                       << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        nuc_slice_limits << "nuc_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":"
                         << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
    }

    e_slice_limits << "\n";
    p_slice_limits << "\n";
    n_slice_limits << "\n";
    nuc_slice_limits << "\n";

    e_slice_limits.close();
    p_slice_limits.close();
    n_slice_limits.close();
    nuc_slice_limits.close();

    system(("cp " + AMapSliceElectronSavePath + "e_slice_limits.par " + AMapSliceElectronSavePathCopy).c_str());
    system(("cp " + AMapSliceProtonSavePath + "p_slice_limits.par " + AMapSliceProtonSavePathCopy).c_str());
    system(("cp " + AMapSliceNeutronSavePath + "n_slice_limits.par " + AMapSliceNeutronSavePathCopy).c_str());
    system(("cp " + AMapSliceNucleonSavePath + "nuc_slice_limits.par " + AMapSliceNucleonSavePathCopy).c_str());
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
    for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
        ElectronTLAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, true);
        ElectronRecoAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, true);
        ElectronRecoToTLRatioBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, true);
        ElectronAMapsBySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, true);
    }

    for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
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
    TFile *AMapsBC_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMapsBC_prefix + SampleName + ".root").c_str(),
                                              "recreate");
    AMapsBC_ref_AMaps_fout->cd();
    AcceptanceMapsBC->Write();
    AMapsBC_ref_AMaps_fout->Write();
    AMapsBC_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMapsBC_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* TL Acceptance maps */
    TFile *TLAMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_TL_prefix + SampleName + ".root").c_str(),
                                              "recreate");
    TLAMaps_ref_AMaps_fout->cd();
    TLAMaps->Write();
    TLAMaps_ref_AMaps_fout->Write();
    TLAMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMap_TL_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* Reco Acceptance maps */
    TFile *RecoAMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_Reco_prefix + SampleName + ".root").c_str(),
                                                "recreate");
    RecoAMaps_ref_AMaps_fout->cd();
    RecoAMaps->Write();
    RecoAMaps_ref_AMaps_fout->Write();
    RecoAMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMap_Reco_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* Ratio Acceptance maps */
    TFile *RatioHitMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_Ratio_prefix + SampleName + ".root").c_str(),
                                                   "recreate");
    RatioHitMaps_ref_AMaps_fout->cd();
    AMapsRatio->Write();
    RatioHitMaps_ref_AMaps_fout->Write();
    RatioHitMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMap_Ratio_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* Charged particle separate AMaps */
    TFile *cPartAMaps_ref_AMaps_fout = new TFile(
            (AcceptanceMapsDirectory + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    cPartAMaps_ref_AMaps_fout->cd();
    Charged_particle_Sep_AMaps->Write();
    cPartAMaps_ref_AMaps_fout->Write();
    cPartAMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* Acceptance maps */
    TFile *AMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMaps_prefix + SampleName + ".root").c_str(),
                                            "recreate");
    AMaps_ref_AMaps_fout->cd();
    AcceptanceMaps->Write();
    AMaps_ref_AMaps_fout->Write();
    AMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMaps_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());
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
        ElectronMomSliceLimits.push_back(MomBinsLimitsTemp);
    } else if (isProton(Particle) || isNeutron(Particle)) {
        NucleonMomSliceLimits.push_back(MomBinsLimitsTemp);
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
void AMaps::SetSlicesFromHistTitle(TH2D *Histogram2D, vector <vector<double>> MomBinsLimits) {
    bool PrintOut = false;

    string Title = Histogram2D->GetTitle();
    string SliceLowerLimStr = Title.substr((Title.find_first_of('#')) - 4, 4);
    string SliceUpperLimStr = Title.substr(Title.find_last_of('#') + 4, 4);

    double SliceLowerLim = stod(SliceLowerLimStr);
    double SliceUpperLim = stod(SliceUpperLimStr);

    vector<double> MomBinsLimitsTemp = {SliceLowerLim, SliceUpperLim};
    NucleonMomSliceLimits.push_back(MomBinsLimitsTemp);

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
            SetSlicesFromHistTitle(TempHist, NucleonMomSliceLimits);
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

    LoadedElectronAMaps0 = (TH2D *) AMaps_RootFile->Get("Electron_AMap");
    if (!LoadedElectronAMaps0) { cout << "\n\nAMaps::ReadHitMaps: could not load Electron_AMap from root file! Exiting...\n", exit(0); }

    LoadedProtonAMap = (TH2D *) AMaps_RootFile->Get("Proton_AMap");
    if (!LoadedProtonAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Proton_AMap from root file! Exiting...\n", exit(0); }

    LoadedNeutronAMap = (TH2D *) AMaps_RootFile->Get("Neutron_AMap");
    if (!LoadedNeutronAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Neutron_AMap from root file! Exiting...\n", exit(0); }

    LoadedNucleonAMap = (TH2D *) AMaps_RootFile->Get("Nucleon_AMap");
    if (!LoadedNucleonAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Nucleon_AMap from root file! Exiting...\n", exit(0); }
    //</editor-fold>

    cout << "\n\nAcceptance maps loaded!\n\n";
}
//</editor-fold>

// ReadAMapLimits function ----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadAMapLimits function">
void AMaps::ReadAMapLimits(const char *filename, vector <vector<double>> &Loaded_particle_limits) {
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
        cout << "\n\nAMaps::ReadAMap: file not found! Exiting...\n\n", exit(0);
    }
}
//</editor-fold>

// ReadAMapSlices function (AMaps) --------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadAMapSlices function (AMaps)">
void AMaps::ReadAMapSlices(const string &SampleName, const string &AcceptanceMapsDirectory, const string &Particle,
                           const vector <vector<double>> &Loaded_particle_limits, vector <vector<vector < int>>

> &Loaded_Particle_AMap_Slices) {
string ParticleShort;

if (
isElectron(Particle)
) {
ParticleShort = "e";
} else if (
isProton(Particle)
) {
ParticleShort = "p";
} else if (
isNeutron(Particle)
) {
ParticleShort = "n";
} else {
ParticleShort = "nuc";
}

for (
int Slice = 0;
Slice<Loaded_particle_limits.

size();

Slice++) {
vector <vector<int>> Loaded_Particle_AMap_TempSlice;

string TempFileName = ParticleShort + "_AMap_by_slice/" + ParticleShort + "_AMap_file_from_" +
                      to_string_with_precision(Loaded_particle_limits.at(Slice).at(0), 2) + "_to_" +
                      to_string_with_precision(Loaded_particle_limits.at(Slice).at(1), 2) + ".par";

ReadAMap((AcceptanceMapsDirectory
+ SampleName + "/" + TempFileName).

c_str(), Loaded_Particle_AMap_TempSlice

);

Loaded_Particle_AMap_Slices.
push_back(Loaded_Particle_AMap_TempSlice);
}
}
//</editor-fold>

// ReadWMapSlices function (WMaps) --------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadWMapSlices function (WMaps)">
void AMaps::ReadWMapSlices(const string &SampleName, const string &AcceptanceMapsDirectory, const string &Particle,
                           const vector <vector<double>> &Loaded_particle_limits,
                           vector <vector<vector < double>>

> &Loaded_Particle_WMap_Slices) {
string ParticleShort;

if (
isElectron(Particle)
) {
ParticleShort = "e";
} else if (
isProton(Particle)
) {
ParticleShort = "p";
} else if (
isNeutron(Particle)
) {
ParticleShort = "n";
} else {
ParticleShort = "nuc";
}

for (
int Slice = 0;
Slice<Loaded_particle_limits.

size();

Slice++) {
vector <vector<double>> Loaded_Particle_WMap_TempSlice;

string TempFileName = ParticleShort + "_WMap_by_slice/" + ParticleShort + "_WMap_file_from_" +
                      to_string_with_precision(Loaded_particle_limits.at(Slice).at(0), 2) + "_to_" +
                      to_string_with_precision(Loaded_particle_limits.at(Slice).at(1), 2) + ".par";

ReadWMap((AcceptanceMapsDirectory
+ SampleName + "/" + TempFileName).

c_str(), Loaded_Particle_WMap_TempSlice

);

Loaded_Particle_WMap_Slices.
push_back(Loaded_Particle_WMap_TempSlice);
}
}
//</editor-fold>

// ReadAMap function (AMaps) --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadAMap function (AMaps)">
/* A function that reads AMaps */

void AMaps::ReadAMap(const char *filename, vector <vector<int>> &Loaded_particle_AMap) {
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
        cout << "\n\nAMaps::ReadAMap: file:\n" << filename << "\nwas not found! Exiting...\n\n", exit(0);
    }
}
//</editor-fold>

// ReadWMap function (WMaps) --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadWMap function (WMaps)">
/* A function that reads WMaps */

void AMaps::ReadWMap(const char *filename, vector <vector<double>> &Loaded_particle_WMaps) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        string tp;

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            string parameter, parameter2;
            ss >> parameter; // get cut identifier

            if (findSubstring(parameter, "Weight")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                string LineEntry;
                vector<double> col;

                while (getline(ss2, LineEntry, ':')) { col.push_back(stod(LineEntry)); }

                Loaded_particle_WMaps.push_back(col);
            }
        }
    } else {
        cout << "\n\nAMaps::ReadWMap: file:\n" << filename << "\nwas not found! Exiting...\n\n", exit(0);
    }
}
//</editor-fold>

// MatchAngToHitMap function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MatchAngToHitMap function (original with neutron FC 'bug')">
bool AMaps::MatchAngToHitMap(const string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC) {
    int e_InitialSlice = 0, e_FinalSlice = Loaded_ElectronMomSliceLimits.size();
    int p_InitialSlice = 0, p_FinalSlice = Loaded_NucleonMomSliceLimits.size(), n_InitialSlice = 0, n_FinalSlice = Loaded_NucleonMomSliceLimits.size();

    int e_TestSlice = Slices2Test.at(0), p_TestSlice = Slices2Test.at(1), n_TestSlice = Slices2Test.at(2);

    if (e_single_slice_test) { e_InitialSlice = e_TestSlice - 1, e_FinalSlice = e_TestSlice; }

    if (nuc_single_slice_test) {
        p_InitialSlice = p_TestSlice - 1, p_FinalSlice = p_TestSlice, n_InitialSlice = n_TestSlice - 1, n_FinalSlice = n_TestSlice;
    }

    if (isElectron(Particle)) {
        for (int Slice = e_InitialSlice; Slice < e_FinalSlice; Slice++) {
            if (Momentum >= Loaded_ElectronMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_ElectronMomSliceLimits.at(Slice).at(1)) {
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
        for (int Slice = p_InitialSlice; Slice < p_FinalSlice; Slice++) {
            if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
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
                                if (NucleonOverlappingFC) {
                                    if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                } else {
                                    if (Loaded_p_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                }
                            } // end of find right phi if
                        }
                    } // end of find right theta if
                }
            } // end of if the right momentum
        }
    } else if (isNeutron(Particle)) {
        for (int Slice = n_InitialSlice; Slice < n_FinalSlice; Slice++) {
            if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
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
                                if (NucleonOverlappingFC) {
                                    if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                } else {
                                    if (Loaded_n_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
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

////<editor-fold desc="MatchAngToHitMap function (original after neutron FC 'bug' fix)">
//bool AMaps::MatchAngToHitMap(const string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC) {
//    int e_InitialSlice = 0, e_FinalSlice = Loaded_ElectronMomSliceLimits.size();
//    int p_InitialSlice = 0, p_FinalSlice = Loaded_NucleonMomSliceLimits.size(), n_InitialSlice = 0, n_FinalSlice = Loaded_NucleonMomSliceLimits.size();
//
//    int e_TestSlice = Slices2Test.at(0), p_TestSlice = Slices2Test.at(1), n_TestSlice = Slices2Test.at(2);
//
//    if (e_single_slice_test) { e_InitialSlice = e_TestSlice - 1, e_FinalSlice = e_TestSlice; }
//
//    if (nuc_single_slice_test) { p_InitialSlice = p_TestSlice - 1, p_FinalSlice = p_TestSlice, n_InitialSlice = n_TestSlice - 1, n_FinalSlice = n_TestSlice; }
//
//    if (isElectron(Particle)) {
//        for (int Slice = e_InitialSlice; Slice < e_FinalSlice; Slice++) {
//            if (Momentum >= Loaded_ElectronMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_ElectronMomSliceLimits.at(Slice).at(1)) {
//                for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
//                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / HistElectronSliceNumOfYBins;
//                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                        for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
//                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / HistElectronSliceNumOfXBins;
//                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                if (Loaded_e_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                    return true;
//                                } else {
//                                    return false;
//                                }
//                            } // end of find right phi if
//                        }
//                    } // end of find right theta if
//                }
//            } // end of if the right momentum
//        }
//    } else if (isProton(Particle)) {
//        for (int Slice = p_InitialSlice; Slice < p_FinalSlice; Slice++) {
//            if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
//                for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                if (NucleonOverlappingFC) {
//                                    if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                        return true;
//                                    } else {
//                                        return false;
//                                    }
//                                } else {
//                                    if (Loaded_p_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                        return true;
//                                    } else {
//                                        return false;
//                                    }
//                                }
//                            } // end of find right phi if
//                        }
//                    } // end of find right theta if
//                }
//            } // end of if the right momentum
//        }
//    } else if (isNeutron(Particle)) {
//        if (NucleonOverlappingFC) {
//            for (int Slice = n_InitialSlice; Slice < n_FinalSlice; Slice++) {
//                if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
//                    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                        double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                        double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                        double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                        if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                                double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                                double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                                double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                                if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                    if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                        return true;
//                                    } else {
//                                        return false;
//                                    }
//                                } // end of find right phi if
//                            }
//                        } // end of find right theta if
//                    }
//                } // end of if the right momentum
//            }
//        } else {
//            /* If we're not applying overlapping fiducial cuts (in momRes calculations, for example),
//               then don't look for momentum based cuts, since neutron momentum can be greater than beamE (bad
//               neutrons with P_n > beamE will be cut later!) */
//
//            for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                    for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                        double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                        double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                        double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                        if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                            if (Loaded_n_AMap.at(i).at(j) != 0) {
//                                return true;
//                            } else {
//                                return false;
//                            }
//                        } // end of find right phi if
//                    }
//                } // end of find right theta if
//            }
//        }
//    } // end of if Particle
//
//    return false;
//}
////</editor-fold>

// GetWeight function ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetWeight function">
double AMaps::GetWeight(bool apply_kinematical_weights, const string &Particle, double Momentum, double Theta, double Phi) {
    bool PrintOut = false;

    if (apply_kinematical_weights) {
        if (isElectron(Particle)) {
            return 1;
        } else if (isProton(Particle)) {
            for (int Slice = 0; Slice < Loaded_NucleonMomSliceLimits.size(); Slice++) {
                if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
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
                                    double Weight;

                                    if (Loaded_p_WMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        Weight = (1 / (Loaded_p_WMap_Slices.at(Slice).at(i).at(j)));

                                        if (PrintOut) {
                                            cout << "MomentumLowerLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                            cout << "Momentum = " << Momentum << "\n";
                                            cout << "MomentumUpperLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";

                                            cout << "ThetaLowerLimTemp = " << ThetaLowerLimTemp << "\n";
                                            cout << "Theta = " << Theta << "\n";
                                            cout << "ThetaUpperLimTemp = " << ThetaUpperLimTemp << "\n\n";

                                            cout << "PhiLowerLimTemp = " << PhiLowerLimTemp << "\n";
                                            cout << "Phi = " << Phi << "\n";
                                            cout << "PhiUpperLimTemp = " << PhiUpperLimTemp << "\n\n";

                                            cout << "Weight = " << Weight << "\n\n";

                                            cout << "1/Weight = " << (1 / Weight) << "\n\n\n\n";
                                        }
                                    } else {
                                        Weight = 0;
                                    }

                                    return Weight;
                                } // end of find right phi if
                            }
                        } // end of find right theta if
                    }
                } // end of if the right momentum
            }
        } else if (isNeutron(Particle)) {
            for (int Slice = 0; Slice < Loaded_NucleonMomSliceLimits.size(); Slice++) {
                if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
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
                                    double Weight;

                                    if (Loaded_n_WMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        Weight = (1 / (Loaded_n_WMap_Slices.at(Slice).at(i).at(j)));

                                        if (PrintOut) {
                                            cout << "MomentumLowerLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                            cout << "Momentum = " << Momentum << "\n";
                                            cout << "MomentumUpperLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";

                                            cout << "ThetaLowerLimTemp = " << ThetaLowerLimTemp << "\n";
                                            cout << "Theta = " << Theta << "\n";
                                            cout << "ThetaUpperLimTemp = " << ThetaUpperLimTemp << "\n\n";

                                            cout << "PhiLowerLimTemp = " << PhiLowerLimTemp << "\n";
                                            cout << "Phi = " << Phi << "\n";
                                            cout << "PhiUpperLimTemp = " << PhiUpperLimTemp << "\n\n";

                                            cout << "Weight = " << Weight << "\n\n";

                                            cout << "1/Weight = " << (1 / Weight) << "\n\n\n\n";
                                        }
                                    } else {
                                        Weight = 0;
                                    }

                                    return Weight;
                                } // end of find right phi if
                            }
                        } // end of find right theta if
                    }
                } // end of if the right momentum
            }
        } // end of if Particle

        return 0;
    } else {
        return 1;
    }
}
//</editor-fold>

// IsInFDQuery function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="IsInFDQuery function">
bool AMaps::IsInFDQuery(bool Generate_AMaps, const DSCuts &ThetaFD, const string &Particle, double Momentum, double Theta, double Phi,
                        bool NucleonOverlappingFC) {
    bool inFDQuery, part_inSomeSector;

    if (!Generate_AMaps) {
        part_inSomeSector = MatchAngToHitMap(Particle, Momentum, Theta, Phi, NucleonOverlappingFC);
    } else {
        part_inSomeSector = true;
    }

    inFDQuery = (part_inSomeSector && (Theta >= ThetaFD.GetLowerCutConst()) && (Theta <= ThetaFD.GetUpperCutConst()));

    return inFDQuery;
}
//</editor-fold>
