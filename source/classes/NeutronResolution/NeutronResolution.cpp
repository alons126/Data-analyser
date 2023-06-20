//
// Created by alons on 20/06/2023.
//

#include "NeutronResolution.h"

// NeutronResolution constructor ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="NeutronResolution constructor">
NeutronResolution::NeutronResolution(double beamE, const string &SavePath, double DeltaSlices) {
//NeutronResolution::NeutronResolution(const string &SampleName, double beamE, const string &SavePath, double DeltaSlices) {
    SlicesSavePath = SavePath;
    delta = DeltaSlices;
//    RunSampleName = SampleName;

    bool SliceAndDice = true;
    double SliceLowerLim = 0.;
    double SliceUpperLim = SliceLowerLim + delta;
    int SliceNumber = 0;

    while (SliceAndDice) {
        ++SliceNumber;
        
        string hStatsTitle = "n res. - " + to_string_with_precision(SliceLowerLim, 2) + " #leq P^{truth}_{n} #leq " + to_string_with_precision(SliceUpperLim, 2)
                             + " GeV/c";
        string hTitle = "Neutron resolution for " + to_string_with_precision(SliceLowerLim, 2) + " #leq P^{truth}_{n} #leq " + to_string_with_precision(SliceUpperLim, 2)
                        + " GeV/c";
        string hSaveName = to_string(SliceNumber) + "_res_plot_for_TL_P_n_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                           to_string_with_precision(SliceUpperLim, 2);

        hPlot1D hResolutionSlice = hPlot1D("1n", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{nFD} - P^{rec.}_{nFD})/P^{truth}_{nFD}", SlicesSavePath, hSaveName,
                                           hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
        ResolutionSlices.push_back(hResolutionSlice);

        if ((beamE - SliceLowerLim) < delta) {
            SliceAndDice = false;
        } else {
            SliceLowerLim += delta;
            SliceUpperLim += delta;
        }
    }

    NumberOfSlices = SliceNumber;
}
//</editor-fold>

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveResSlices function">
void NeutronResolution::DrawAndSaveResSlices(const string &SampleName, TCanvas *h1DCanvas, const string &plots_path, const string &CutsDirectory) {
    string SampleNameTemp = SampleName;

    TList *ResSlicePlots = new TList();
//    const string& ResSlicePlotsListName = plots_path;
    const char *ResSlicePlotsFolderPath = (plots_path + "/Neutron_resolution_plots.root").c_str();
    const char *ResSliceCutsDirectoryPath = (CutsDirectory + "Neutron_resolution_plots.root").c_str();
//    const char *ResSlicePlotsFolderPath = (plots_path + "/Neutron_resolution_plots.root").c_str();
//    const char *ResSliceCutsDirectoryPath = (CutsDirectory + "Neutron_resolution_plots.root").c_str();

    for (int i = 0; i < NumberOfSlices; i++) { ResolutionSlices.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, ResSlicePlots, false, true, 1., 9999, 9999, 0, false); }

    TFile *PlotsFolder_fout = new TFile((plots_path + "/Neutron_resolution_plots.root").c_str(), "recreate");
//    TFile *PlotsFolder_fout = new TFile(ResSlicePlotsFolderPath, "recreate");
    PlotsFolder_fout->cd();
    ResSlicePlots->Write();
    PlotsFolder_fout->Write();
    PlotsFolder_fout->Close();

    TFile *CutsDirectory_fout = new TFile((CutsDirectory + "Neutron_resolution_plots.root").c_str(), "recreate");
//    TFile *CutsDirectory_fout = new TFile(ResSliceCutsDirectoryPath, "recreate");
    CutsDirectory_fout->cd();
    ResSlicePlots->Write();
    CutsDirectory_fout->Write();
    CutsDirectory_fout->Close();
}
//</editor-fold>