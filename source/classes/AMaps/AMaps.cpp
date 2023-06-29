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
//    SetMaps();

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

    string hStatsTitleAMapNeutron = "Neutron_AMap";
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
//    ReadHitMaps(RefrenceHitMapsDirectory, SampleName);

    ReadAMap("e_hit_map_file.par", Loaded_e_Hit_Map);
    cout << "\n\nLoaded_e_Hit_Map.size() = " << Loaded_e_Hit_Map.size() << "\n\n";

    ReadAMap("p_hit_map_file.par", Loaded_p_Hit_Map);

    hBinNumOfXBins = 100;  // 100 by Default
    hBinNumOfYBins = 100;  // 100 by Default
}
//</editor-fold>

// SetMaps function -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetMaps function">
void AMaps::SetMaps() {
    for (int i = 0; i < hBinNumOfYBins; i++) {
        vector<int> col;

        for (int j = 0; j < hBinNumOfXBins; j++) { col.push_back(0); }

        e_Hit_Map.push_back(col);
        p_Hit_Map.push_back(col);
        n_Hit_Map.push_back(col);
        nuc_Hit_Map.push_back(col);
    }
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

    for (int i = 0; i < hBinNumOfYBins; i++) {
        vector<int> e_col, p_col;

        for (int j = 0; j < hBinNumOfXBins; j++) {
            if (ElectronAMap.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                e_col.push_back(1);
            } else {
                e_col.push_back(0);
            }

            if (ProtonAMap.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                p_col.push_back(1);
            } else {
                p_col.push_back(0);
            }
        }

        e_Hit_Map.push_back(e_col);
        p_Hit_Map.push_back(p_col);
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

// GenerateNucleonAMap function -----------------------------------------------------------------------------------------------------------------------------------------

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

// SaveHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

void AMaps::SaveHitMaps(const string &SampleName, const string &RefrenceHitMapsDirectory) {
    ofstream e_hit_map_file;
    ofstream p_hit_map_file;

    e_hit_map_file.open("e_hit_map_file.par");
    p_hit_map_file.open("p_hit_map_file.par");

    for (int i = 0; i < hBinNumOfYBins; i++) {
        e_hit_map_file << "Line\t\t\t";
        p_hit_map_file << "Line\t\t\t";
//        e_hit_map_file << "Line_" << i + 1 << "\t\t\t";
//        p_hit_map_file << "Line_" << i + 1 << "\t\t\t";

        for (int j = 0; j < hBinNumOfXBins; j++) {
            if (j != hBinNumOfXBins - 1) {
                e_hit_map_file << e_Hit_Map.at(i).at(j) << ":";
                p_hit_map_file << p_Hit_Map.at(i).at(j) << ":";
            } else {
                e_hit_map_file << e_Hit_Map.at(i).at(j);
                p_hit_map_file << p_Hit_Map.at(i).at(j);
            }
        }

        e_hit_map_file << "\n";
        p_hit_map_file << "\n";
    }

    e_hit_map_file.close();
    p_hit_map_file.close();

    system(("cp e_hit_map_file.par " + RefrenceHitMapsDirectory + SampleName).c_str());
    system(("cp p_hit_map_file.par " + RefrenceHitMapsDirectory + SampleName).c_str());
}

// DrawAndSaveHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveHitMaps function">
void AMaps::DrawAndSaveHitMaps(const string &SampleName, TCanvas *h1DCanvas, const string &RefrenceHitMapsDirectory) {
    string SampleNameTemp = SampleName;

    system(("mkdir -p " + RefrenceHitMapsDirectory + SampleName).c_str());

    CalcHitMapsRatio(calc_Electron_RecoToTL_Ratio, calc_Proton_RecoToTL_Ratio, calc_Neutron_RecoToTL_Ratio);

    GenerateCPartAMaps(Charged_particle_min_Ratio);
    GenerateNPartAMaps(Neutral_particle_min_Ratio);
    GenerateNucleonAMap();

    SaveHitMaps(SampleName, RefrenceHitMapsDirectory);

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
    AMapsBC_ref_hit_maps_fout->cd();
    AcceptanceMapsBC->Write();
    AMapsBC_ref_hit_maps_fout->Write();
    AMapsBC_ref_hit_maps_fout->Close();

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

// HistCounter function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="HistCounter function">
int AMaps::HistCounter(const char *fname) {
    TKey *key;
    TFile *f = TFile::Open(fname, "READ");

//    if (!f || f->IsZombie()) {
//        cout << "Unable to open " << fname << " for reading..." << endl;
//        return;
//    }

    Int_t total = 0;
    TIter next((TList *) f->GetListOfKeys());

//    while (key == (TKey *) next())
//    {
//    while (key = (TKey *) next()) {
    while ((key = (TKey *) next())) {
        TClass *cl = gROOT->GetClass(key->GetClassName());

        if (cl->InheritsFrom("TH1")) {
            // the following line is not needed if you only want
            // to count the histograms
            TH1 *h = (TH1 *) key->ReadObj();
//            cout << "Histo found: " << h->GetName() << " - " << h->GetTitle() << endl;
            total++;
        }
    }

    cout << "\n\nFound " << total << " Histograms\n" << endl;

    return total;
}
//</editor-fold>

// SetHistBinsFromHistTitle function ------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetHistBinsFromHistTitle function">
void AMaps::SetHistBinsFromHistTitle(TH2D *Histogram2D) {
    bool PrintOut = false;

//    hBinNumOfXBins = Histogram2D->GetNbinsX();
//    hBinNumOfYBins = Histogram2D->GetNbinsY();

    string Title = Histogram2D->GetTitle();
    string NumOfXBinsStr = Title.substr((Title.find_last_of('(')) + 1, 3);
    string NumOfYBinsStr = Title.substr(Title.find_last_of('x') + 1, 3);

    hBinNumOfXBins = stoi(NumOfXBinsStr);
    hBinNumOfYBins = stoi(NumOfYBinsStr);

    if (PrintOut) {
        cout << "\n\nTitle = " << Title << "\n\n";
        cout << "NumOfXBinsStr = " << NumOfXBinsStr << "\n";
        cout << "NumOfYBinsStr = " << NumOfYBinsStr << "\n\n";

        cout << "hBinNumOfXBins = " << hBinNumOfXBins << "\n";
        cout << "hBinNumOfYBins = " << hBinNumOfYBins << "\n\n";
    }
}
//</editor-fold>

// SetSlicesFromHistTitle function --------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetSlicesFromHistTitle function">
void AMaps::SetSlicesFromHistTitle(TH2D *Histogram2D) {
    bool PrintOut = false;

    string Title = Histogram2D->GetTitle();
    string SliceLowerLimStr = Title.substr((Title.find_first_of('#')) - 4, 4);
    string SliceUpperLimStr = Title.substr(Title.find_last_of('#') + 4, 4);

    double SliceLowerLim = stod(SliceLowerLimStr);
    double SliceUpperLim = stod(SliceUpperLimStr);

    vector<double> MomBinsLimitsTemp = {SliceLowerLim, SliceUpperLim};

    PBinsLimits.push_back(MomBinsLimitsTemp);

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
void AMaps::ReadHitMaps(const string &RefrenceHitMapsDirectory, const string &SampleName) {
    bool PrintKeys = false;

    //<editor-fold desc="Load AMapsBC">
    string AMapsBC_RootFile_FileName = RefrenceHitMapsDirectory + "/" + SampleName + "/" + AMapsBC_prefix + SampleName + ".root";
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
    string Hit_Maps_TL_RootFile_FileName = RefrenceHitMapsDirectory + "/" + SampleName + "/" + Hit_Maps_TL_prefix + SampleName + ".root";
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
            ElectronTLBinHitMaps.push_back(Temp2DHist);
            SetSlicesFromHistTitle(TempHist, PBinsLimits);
        } else if (findSubstring(TempHist->GetTitle(), "{p}")) {
            ProtonTLBinHitMaps.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{n}")) {
            NeutronTLHitMap.SetHistogram2D(TempHist);
        }

        ++counter;
    }
    //</editor-fold>

    //<editor-fold desc="Load Hit_Maps_Reco">
    string Hit_Maps_Reco_RootFile_FileName = RefrenceHitMapsDirectory + "/" + SampleName + "/" + Hit_Maps_Reco_prefix + SampleName + ".root";
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
            ElectronRecoBinHitMaps.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{p}")) {
            ProtonRecoBinHitMaps.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{n}")) {
            NeutronRecoHitMap.SetHistogram2D(TempHist);
        }
    }
    //</editor-fold>

    //<editor-fold desc="Load Hit_Maps_Ratio">
    string Hit_Maps_Ratio_RootFile_FileName = RefrenceHitMapsDirectory + "/" + SampleName + "/" + Hit_Maps_Ratio_prefix + SampleName + ".root";
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
            ElectronRecoToTLRatio.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{p}")) {
            ProtonRecoToTLRatio.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "{n}")) {
            NeutronRecoToTLRatio.SetHistogram2D(TempHist);
        }
    }
    //</editor-fold>

    //<editor-fold desc="Load cPart_Sep_AMaps">
    string cPart_Sep_AMaps_RootFile_FileName = RefrenceHitMapsDirectory + "/" + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root";
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
            ElectronSepAMaps.push_back(Temp2DHist);
        } else if (findSubstring(TempHist->GetTitle(), "Proton") || findSubstring(TempHist->GetTitle(), "proton")) {
            ProtonSepAMaps.push_back(Temp2DHist);
        }
    }
    //</editor-fold>

    //<editor-fold desc="Load AMaps">
    string AMaps_RootFile_FileName = RefrenceHitMapsDirectory + "/" + SampleName + "/" + AMaps_prefix + SampleName + ".root";
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

    cout << "\n\nHit maps loaded!\n\n";
}
//</editor-fold>

// ReadAMap function ----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadAMap function">
void AMaps::ReadAMap(const char *filename, vector<vector<int>> &Loaded_particle_hit_map) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        string tp;

//        // remove 3 lines of header
//        for (int i = 0; i < 3; i++) { getline(infile, tp); }

        int Line = 1;

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            string parameter, parameter2;
            ss >> parameter; // get cut identifier

            if (findSubstring(parameter, "Line")) {
//            if (findSubstring(parameter, ("Line_" + to_string(Line)))) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                string LineEntry;
                vector<int> col;

//                int count = 0; // parameter number
//                string CutNameTemp = parameter;
//                int SliceNumberTemp;
//                double SliceLowerBoundaryTemp, SliceUpperBoundaryTemp, FitMeanTemp, FitSigmaTemp;

                while (getline(ss2, LineEntry, ':')) {
                    col.push_back(stoi(LineEntry));
                }

                cout << "\n\ncol.size() = " << col.size() << "\n\n";
                Loaded_particle_hit_map.push_back(col);
            }
        }
    } else {
        cout << "\n\nReadAMap: file not found! Exiting...\n\n", exit(0);
    }
}
//</editor-fold>

// MatchAngToHitMap function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MatchAngToHitMap function">
bool AMaps::MatchAngToHitMap(const string &Particle, double Momentum, double Theta, double Phi) {
    if (isElectron(Particle)) {
//        double Ylength;
//        Ylength = (double) Loaded_e_Hit_Map.size();

        for (int i = 0; i < hBinNumOfYBins; i++) {
//        for (int i = 0; i < Loaded_e_Hit_Map.size(); i++) {
            double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / hBinNumOfYBins;
            double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
            double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

            if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                double Xlength;
//                Xlength = (double) Loaded_e_Hit_Map.at(i).size();

                for (int j = 0; j < hBinNumOfXBins; j++) {
                    double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / hBinNumOfXBins;
                    double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                    double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                    if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {

//                        cout << "\n\n\nTEST TEST TEST\n\n\n";

                        if (Loaded_e_Hit_Map.at(i).at(j) != 0) {
                            return true;
                        } else {
                            return false;
                        }
                    } // end of find right phi if
                }
            } // end of find right theta if
        }
        /*
        for (int i = 0; i < (hBinNumOfYBins + 1); i++) {
            double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (hBinNumOfYBins);
            double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
            double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

            if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                for (int j = 0; j < (hBinNumOfXBins + 1); j++) {
                    double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (hBinNumOfXBins);
                    double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                    double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                    if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                        if (LoadedElectronAMaps0->GetBinContent(i, j) != 0) {
//                            cout << "\n\LoadedElectronAMaps0->GetBinContent(i, j) = " << LoadedElectronAMaps0->GetBinContent(i, j) << "\n\n";
                            return true;
                        } else {
                            return false;
                        }
                    } // end of find right phi if
                }
            } // end of find right theta if
        }
*/

        /*
        for (int k = 0; k < PBinsLimits.size(); k++) {
            if ((Momentum >= PBinsLimits.at(k).at(0)) && (Momentum < PBinsLimits.at(k).at(1))) {
                for (int i = 0; i < (hBinNumOfYBins + 1); i++) {
                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (hBinNumOfYBins);
                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                        for (int j = 0; j < (hBinNumOfXBins + 1); j++) {
                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (hBinNumOfXBins);
                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                if (ElectronSepAMaps.at(k).GetHistogram2D()->GetBinContent(i, j) != 0.) {
                                    return true;
                                } else {
                                    return false;
                                }
                            } // end of find right phi if
                        }
                    } // end of find right theta if
                }
            } // end of in momentum range if
        } // end of loop over Slices
*/
    } else if (isProton(Particle)) {
        for (int i = 0; i < (hBinNumOfYBins + 1); i++) {
            double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (hBinNumOfYBins);
            double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
            double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

            if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                for (int j = 0; j < (hBinNumOfXBins + 1); j++) {
                    double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (hBinNumOfXBins);
                    double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                    double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                    if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                        if (LoadedNucleonAMap->GetBinContent(i, j) != 0) {
                            if (Phi > 100) {
                                exit(0);
                            }
//                            cout << "\n\LoadedNucleonAMap->GetBinContent(i, j) = " << LoadedNucleonAMap->GetBinContent(i, j) << "\n\n";
                            return true;
                        } else {
                            return false;
                        }
                    } // end of find right phi if
                }
            } // end of find right theta if
        }
        /*        for (int k = 0; k < PBinsLimits.size(); k++) {
                if ((Momentum >= PBinsLimits.at(k).at(0)) && (Momentum < PBinsLimits.at(k).at(1))) {
                    for (int i = 0; i < (hBinNumOfYBins + 1); i++) {
                        double dThetaTemp = (hBinLowerYLim - hBinUpperYLim) / (hBinNumOfYBins);
                        double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                        double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                        if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                            for (int j = 0; j < (hBinNumOfXBins + 1); j++) {
                                double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (hBinNumOfXBins);
                                double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                                double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                                if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                    if (ProtonSepAMaps.at(k).GetHistogram2D()->GetBinContent(i, j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                } // end of find right phi if
                            }

    //                        break;
                        } // end of find right theta if
                    }

    //                break;
                } // end of in momentum range if
        } // end of loop over Slices*/
    } else if (isNeutron(Particle)) {
        for (int i = 0; i < (hBinNumOfYBins + 1); i++) {
            double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (hBinNumOfYBins);
            double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
            double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

            if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                for (int j = 0; j < (hBinNumOfXBins + 1); j++) {
                    double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (hBinNumOfXBins);
                    double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                    double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                    if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                        if (LoadedNucleonAMap->GetBinContent(i, j) != 0) {
//                            cout << "\n\LoadedNucleonAMap->GetBinContent(i, j) = " << LoadedNucleonAMap->GetBinContent(i, j) << "\n\n";
                            return true;
                        } else {
                            return false;
                        }
                    } // end of find right phi if
                }
            } // end of find right theta if
        }
    } // end of if isElectron(Particle)

    return false;
}
//</editor-fold>
