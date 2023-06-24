//
// Created by alons on 22/06/2023.
//

#include "AMaps.h"

// AMaps constructor ----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="AMaps constructor">
AMaps::AMaps(double beamE, const string &SavePath, double NumberOfBins) {
    BinSavePath = SavePath;
    string BinSavePathTLElectron = SavePath + "01a_Electron_TL_Hit_Maps/";
    system(("mkdir -p " + BinSavePathTLElectron).c_str());
    string BinSavePathRecoElectron = SavePath + "01b_Electron_Reco_Hit_Maps/";
    system(("mkdir -p " + BinSavePathRecoElectron).c_str());
    string BinSavePathRecoToTLElectron = SavePath + "01c_Electron_RecoToTL_Ratio/";
    system(("mkdir -p " + BinSavePathRecoToTLElectron).c_str());

    string BinSavePathTLProton = SavePath + "02a_Proton_TL_Hit_Maps/";
    system(("mkdir -p " + BinSavePathTLProton).c_str());
    string BinSavePathRecoProton = SavePath + "02b_Proton_Reco_Hit_Maps/";
    system(("mkdir -p " + BinSavePathRecoProton).c_str());
    string BinSavePathRecoToTLProton = SavePath + "02c_Proton_RecoToTL_Ratio/";
    system(("mkdir -p " + BinSavePathRecoToTLProton).c_str());

    string BinSavePathTLNeutron = SavePath + "03a_Neutron_TL_Hit_Maps/";
    system(("mkdir -p " + BinSavePathTLNeutron).c_str());
    string BinSavePathRecoNeutron = SavePath + "03b_Neutron_Reco_Hit_Maps/";
    system(("mkdir -p " + BinSavePathRecoNeutron).c_str());
    string BinSavePathRecoToTLNeutron = SavePath + "03c_Neutron_RecoToTL_Ratio/";
    system(("mkdir -p " + BinSavePathRecoToTLNeutron).c_str());

    SetBins(beamE, NumberOfBins);

    for (int i = 0; i < PBinsLimits.size(); i++) {
        double BinLowerLim = PBinsLimits.at(i).at(0);
        double BinUpperLim = PBinsLimits.at(i).at(1);

        int BinUpperLimPrecision;
        if (BinUpperLim == beamE) { BinUpperLimPrecision = 3; } else { BinUpperLimPrecision = 2; }

        //<editor-fold desc="Setting electron hit maps">

        //<editor-fold desc="Electron TL hit map">
        string hStatsTitleTLElectron = "TL P_{e} bin for - " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleTLElectron = "TL P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hSaveNameTLElectron = to_string(i) + "_TL_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinTLElectron = hPlot2D("1e cut", "", hStatsTitleTLElectron, hTitleTLElectron, "#theta_{e} [Deg]", "#phi_{e} [Deg]", BinSavePathTLElectron,
                                          hSaveNameTLElectron,
                                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ElectronTLBinHitMaps.push_back(hPBinTLElectron);
        //</editor-fold>

        //<editor-fold desc="Electron Reco. hit maps">
        string hStatsTitleRecoElectron = "Reco P_{e} bin for - " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                         to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoElectron = "Reco P_{e} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                    to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hSaveNameRecoElectron = to_string(i) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoElectron = hPlot2D("1e cut", "", hStatsTitleRecoElectron, hTitleRecoElectron, "#theta_{e} [Deg]", "#phi_{e} [Deg]", BinSavePathRecoElectron,
                                            hSaveNameRecoElectron,
                                            hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ElectronRecoBinHitMaps.push_back(hPBinRecoElectron);
        //</editor-fold>

        //<editor-fold desc="Electron Reco. to TL Ratio">
        string hStatsTitleRecoToTLRatioElectron = "Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioElectron = "Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                             to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hSaveNameRecoToTLRatioElectron = to_string(i) + "_Reco_to_TL_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioElectron = hPlot2D("1e cut", "", hStatsTitleRecoToTLRatioElectron, hTitleRecoToTLRatioElectron, "#theta_{e} [Deg]", "#phi_{e} [Deg]",
                                                     BinSavePathRecoToTLElectron,
                                                     hSaveNameRecoToTLRatioElectron,
                                                     hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ElectronRecoToTLRatio.push_back(hPBinRecoToTLRatioElectron);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Setting proton hit maps">

        //<editor-fold desc="Proton TL hit maps">
        string hStatsTitleTLProton = "TL P_{p} bin for - " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleTLProton = "TL P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hSaveNameTLProton = to_string(i) + "_TL_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                   to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinTLProton = hPlot2D("1e cut", "", hStatsTitleTLProton, hTitleTLProton, "#theta_{p} [Deg]", "#phi_{p} [Deg]", BinSavePathTLProton, hSaveNameTLProton,
                                        hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ProtonTLBinHitMaps.push_back(hPBinTLProton);
        //</editor-fold>

        //<editor-fold desc="Proton Reco. hit maps">
        string hStatsTitleRecoProton = "Reco P_{p} bin for - " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                       to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoProton = "Reco P_{p} bin for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                  to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hSaveNameRecoProton = to_string(i) + "_Reco_P_bin_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                     to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoProton = hPlot2D("1e cut", "", hStatsTitleRecoProton, hTitleRecoProton, "#theta_{p} [Deg]", "#phi_{p} [Deg]", BinSavePathRecoProton,
                                          hSaveNameRecoProton,
                                          hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ProtonRecoBinHitMaps.push_back(hPBinRecoProton);
        //</editor-fold>

        //<editor-fold desc="Proton Reco. to TL Ratio">
        string hStatsTitleRecoToTLRatioProton = "Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hTitleRecoToTLRatioProton = "Reco/TL ratio for " + to_string_with_precision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                           to_string_with_precision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
        string hSaveNameRecoToTLRatioProton = to_string(i) + "_Reco_to_TL_Ratio_for_P_from_" + to_string_with_precision(BinLowerLim, 2) + "_to_" +
                                              to_string_with_precision(BinUpperLim, BinUpperLimPrecision);
        hPlot2D hPBinRecoToTLRatioProton = hPlot2D("1e cut", "", hStatsTitleRecoToTLRatioProton, hTitleRecoToTLRatioProton, "#theta_{p} [Deg]", "#phi_{p} [Deg]",
                                                   BinSavePathRecoToTLProton,
                                                   hSaveNameRecoToTLRatioProton,
                                                   hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
        ProtonRecoToTLRatio.push_back(hPBinRecoToTLRatioProton);
        //</editor-fold>

        //</editor-fold>

    }

    //<editor-fold desc="Setting neutron hit maps">

    //<editor-fold desc="Neutron TL hit maps">
    string hStatsTitleTLNeutron = "TL Neutron Hit Map", hTitleTLNeutron = "TL Neutron Hit Map", hSaveNameTLNeutron = "TL_Neutron_Hit_Map";
    NeutronTLHitMap = hPlot2D("1e cut", "", hStatsTitleTLNeutron, hTitleTLNeutron, "#theta_{n} [Deg]", "#phi_{n} [Deg]", BinSavePathTLNeutron, hSaveNameTLNeutron,
                              hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Neutron Reco. hit maps">
    string hStatsTitleRecoNeutron = "Reco Neutron Hit Map", hTitleRecoNeutron = "Reco Neutron Hit Map", hSaveNameRecoNeutron = "Reco_Neutron_Hit_Map";
    NeutronRecoHitMap = hPlot2D("1e cut", "", hStatsTitleRecoNeutron, hTitleRecoNeutron, "#theta_{n} [Deg]", "#phi_{n} [Deg]", BinSavePathRecoNeutron,
                                hSaveNameRecoNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);
    //</editor-fold>

    //<editor-fold desc="Neutron Reco. to TL Ratio">
    string hStatsTitleRecoToTLNeutron = "Reco/TL ratio", hTitleRecoToTLNeutron = "Reco/TL ratio", hSaveNameRecoToTLNeutron = "Reco_to_TL_Ratio";
    NeutronRecoToTLRatio = hPlot2D("1e cut", "", hStatsTitleRecoToTLNeutron, hTitleRecoToTLNeutron, "#theta_{n} [Deg]", "#phi_{n} [Deg]", BinSavePathRecoToTLNeutron,
                                   hSaveNameRecoToTLNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, hBinNumOfXBins, hBinNumOfYBins);

    //</editor-fold>

    //</editor-fold>

}
//</editor-fold>

// SetBins function -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetBins function">
void AMaps::SetBins(double beamE, double NumberOfBins) {
    double BinUpperLim = beamE;

    bool SliceAndDice = true;
    int BinNumber = 1;

    while (SliceAndDice) {
        double UpperLim, LowerLim;

        if (BinNumber == 1) {
            UpperLim = BinUpperLim;
            LowerLim = (BinUpperLim / NumberOfBins);
        } else {
            UpperLim = LowerLim;
            LowerLim = (UpperLim / NumberOfBins);
        }

        cout << "\n\nUpperLim = " << UpperLim << "\n";
        cout << "LowerLim = " << LowerLim << "\n";

        PBinsLimits.push_back({LowerLim, UpperLim});

        if (LowerLim <= 0.3) {
            SliceAndDice = false;
        } else {
            ++BinNumber;
        }
    }

    NumberOfPBins = BinNumber;
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
            for (int i = 0; i < PBinsLimits.size(); i++) {
                if ((Momentum >= PBinsLimits.at(i).at(0)) && (Momentum < PBinsLimits.at(i).at(1))) {
                    ElectronRecoBinHitMaps.at(i).hFill(Phi, Theta, Weight);
                    ElectronRecoToTLRatio.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p) {
            for (int i = 0; i < PBinsLimits.size(); i++) {
                if ((Momentum >= PBinsLimits.at(i).at(0)) && (Momentum < PBinsLimits.at(i).at(1))) {
                    ProtonRecoBinHitMaps.at(i).hFill(Phi, Theta, Weight);
                    ProtonRecoToTLRatio.at(i).hFill(Phi, Theta, Weight);
                    break; // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n) {
            NeutronRecoHitMap.hFill(Phi, Theta, Weight);
            NeutronRecoToTLRatio.hFill(Phi, Theta, Weight);
        }
    }
}
//</editor-fold>

// CalcHitMapsRatio function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="CalcHitMapsRatio function">
void AMaps::CalcHitMapsRatio(bool ElectronRecoToTLDiv, bool ProtonRecoToTLDiv, bool NeutronRecoToTLDiv) {
    if (ElectronRecoToTLDiv || ProtonRecoToTLDiv) {
        for (int i = 0; i < PBinsLimits.size(); i++) {
            if (ElectronRecoToTLDiv) {
                ElectronRecoToTLRatio.at(i).hDivision(ElectronTLBinHitMaps.at(i).GetHistogram2D());
            }

            if (ProtonRecoToTLDiv) {
                ElectronRecoToTLRatio.at(i).hDivision(ElectronTLBinHitMaps.at(i).GetHistogram2D());
            }
        }
    }

    if (NeutronRecoToTLDiv) {
        NeutronRecoToTLRatio.hDivision(NeutronTLHitMap.GetHistogram2D());
    }
}
//</editor-fold>

// DrawAndSaveHitMaps function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveHitMaps function">
void AMaps::DrawAndSaveHitMaps(const string &SampleName, TCanvas *h1DCanvas, const string &RefrenceHitMapsDirectory) {
    string SampleNameTemp = SampleName;

    system(("mkdir -p " + RefrenceHitMapsDirectory + SampleName).c_str());

    CalcHitMapsRatio(calc_Electron_RecoToTL_Ratio, calc_Proton_RecoToTL_Ratio, calc_Neutron_RecoToTL_Ratio);

    for (int i = 0; i < PBinsLimits.size(); i++) {
        ElectronTLBinHitMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLHitMaps, true);
        ElectronRecoBinHitMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoHitMaps, true);
        ElectronRecoToTLRatio.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, HitMapsRatio, true);

        ProtonTLBinHitMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLHitMaps, true);
        ProtonRecoBinHitMaps.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoHitMaps, true);
        ProtonRecoToTLRatio.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, HitMapsRatio, true);
    }

    NeutronTLHitMap.hDrawAndSave(SampleNameTemp, h1DCanvas, TLHitMaps, true);
    NeutronRecoHitMap.hDrawAndSave(SampleNameTemp, h1DCanvas, RecoHitMaps, true);
    NeutronRecoToTLRatio.hDrawAndSave(SampleNameTemp, h1DCanvas, HitMapsRatio, true);

    //<editor-fold desc="Save TL hit maps to plots directory">
    /* TL hit maps */
    TFile *TLHitMaps_plots_path_fout = new TFile((BinSavePath + "/01_Hit_Maps_TL_-_" + SampleName + ".root").c_str(), "recreate");
    TLHitMaps_plots_path_fout->cd();
    TLHitMaps->Write();
    TLHitMaps_plots_path_fout->Write();
    TLHitMaps_plots_path_fout->Close();

    /* Reco hit maps */
    TFile *RecoHitMaps_plots_path_fout = new TFile((BinSavePath + "/02_Hit_Maps_Reco_-_" + SampleName + ".root").c_str(), "recreate");
    RecoHitMaps_plots_path_fout->cd();
    RecoHitMaps->Write();
    RecoHitMaps_plots_path_fout->Write();
    RecoHitMaps_plots_path_fout->Close();

    /* Ratio hit maps */
    TFile *RatioHitMaps_plots_path_fout = new TFile((BinSavePath + "/03_Hit_Maps_Ratio_-_" + SampleName + ".root").c_str(), "recreate");
    RatioHitMaps_plots_path_fout->cd();
    HitMapsRatio->Write();
    RatioHitMaps_plots_path_fout->Write();
    RatioHitMaps_plots_path_fout->Close();
    //</editor-fold>

//    //<editor-fold desc="Save TL hit maps to plots directory">
//    /* TL hit maps */
//    TFile *TLHitMaps_plots_path_fout = new TFile((plots_path + "/Hit_Maps_TL_-_" + SampleName + ".root").c_str(), "recreate");
//    TLHitMaps_plots_path_fout->cd();
//    TLHitMaps->Write();
//    TLHitMaps_plots_path_fout->Write();
//    TLHitMaps_plots_path_fout->Close();
//
//    /* Reco hit maps */
//    TFile *RecoHitMaps_plots_path_fout = new TFile((plots_path + "/Hit_Maps_Reco_-_" + SampleName + ".root").c_str(), "recreate");
//    RecoHitMaps_plots_path_fout->cd();
//    RecoHitMaps->Write();
//    RecoHitMaps_plots_path_fout->Write();
//    RecoHitMaps_plots_path_fout->Close();
//
//    /* Ratio hit maps */
//    TFile *RatioHitMaps_plots_path_fout = new TFile((plots_path + "/Hit_Maps_Ratio_-_" + SampleName + ".root").c_str(), "recreate");
//    RatioHitMaps_plots_path_fout->cd();
//    HitMapsRatio->Write();
//    RatioHitMaps_plots_path_fout->Write();
//    RatioHitMaps_plots_path_fout->Close();
//    //</editor-fold>

    //<editor-fold desc="Save TL hit maps to refrence hit maps directory">
    /* TL hit maps */
    TFile *TLHitMaps_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/Hit_Maps_TL_-_" + SampleName + ".root").c_str(), "recreate");
    TLHitMaps_ref_hit_maps_fout->cd();
    TLHitMaps->Write();
    TLHitMaps_ref_hit_maps_fout->Write();
    TLHitMaps_ref_hit_maps_fout->Close();

    /* Reco hit maps */
    TFile *RecoHitMaps_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/Hit_Maps_Reco_-_" + SampleName + ".root").c_str(), "recreate");
    RecoHitMaps_ref_hit_maps_fout->cd();
    RecoHitMaps->Write();
    RecoHitMaps_ref_hit_maps_fout->Write();
    RecoHitMaps_ref_hit_maps_fout->Close();

    /* Ratio hit maps */
    TFile *RatioHitMaps_ref_hit_maps_fout = new TFile((RefrenceHitMapsDirectory + SampleName + "/Hit_Maps_Ratio_-_" + SampleName + ".root").c_str(), "recreate");
    RatioHitMaps_ref_hit_maps_fout->cd();
    HitMapsRatio->Write();
    RatioHitMaps_ref_hit_maps_fout->Write();
    RatioHitMaps_ref_hit_maps_fout->Close();
    //</editor-fold>

}
//</editor-fold>
