//
// Created by alons on 20/06/2023.
//

#include "NeutronResolution.h"

// NeutronResolution constructor ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="NeutronResolution constructor">
NeutronResolution::NeutronResolution(double beamE, const string &SavePath, double DeltaSlices) {
    SlicesSavePath = SavePath;
    delta = DeltaSlices;

    bool SliceAndDice = true;
    double SliceLowerLim = 0.;
    double SliceUpperLim = SliceLowerLim + delta;
    int SliceNumber = 0;

    while (SliceAndDice) {
        ++SliceNumber;

        int SliceUpperLimPrecision;
        if (SliceUpperLim == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

        string hStatsTitle = "n res. - " + to_string_with_precision(SliceLowerLim, 2) + " #leq P^{truth}_{n} #leq " +
                             to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " GeV/c";
        string hTitle = "Neutron resolution for " + to_string_with_precision(SliceLowerLim, 2) + " #leq P^{truth}_{n} #leq " +
                        to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " GeV/c";
        string hSaveName = to_string(SliceNumber) + "_res_plot_for_TL_P_n_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                           to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
        string hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
//        string hCutName = to_string(SliceNumber) + "_n_res_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
//                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

        hPlot1D hResolutionSlice = hPlot1D("1n", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{nFD} - P^{rec.}_{nFD})/P^{truth}_{nFD}", SlicesSavePath, hSaveName,
                                           hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
        DSCuts ResolutionSliceFitCuts = DSCuts(hCutName, "FD", "Neutron", "1n", 0, -9999, 9999);
        ResolutionSliceFitCuts.SetSliceUpperb(SliceUpperLim);
        ResolutionSliceFitCuts.SetSliceLowerb(SliceLowerLim);
        ResolutionSliceFitCuts.SetSliceNumber(SliceNumber);

        ResolutionSlices.push_back(hResolutionSlice);
        ResolutionSlicesLimits.push_back({SliceLowerLim, SliceUpperLim});
        ResolutionSlicesFitVar.push_back(ResolutionSliceFitCuts);

        if ((SliceLowerLim + delta) > beamE) {
            SliceAndDice = false;
        } else {
            SliceLowerLim += delta;

            if ((SliceUpperLim + delta) > beamE) {
                SliceUpperLim = beamE;
            } else {
                SliceUpperLim += delta;
            }
        }
    }

    NumberOfSlices = SliceNumber;
}
//</editor-fold>

// hFillResPlots function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hFillResPlots function">
void NeutronResolution::hFillResPlots(double Momentum, double Resolution, double Weight) {
    for (int i = 0; i < NumberOfSlices; i++) {
        if (i + 1 < NumberOfSlices) {
            if ((Momentum >= ResolutionSlicesLimits.at(i).at(0)) &&
                (Momentum < ResolutionSlicesLimits.at(i).at(1))) { ResolutionSlices.at(i).hFill(Resolution, Weight); }
        }
//        else {
//            if ((Momentum >= ResolutionSlicesLimits.at(i).at(0)) &&
//                (Momentum <= ResolutionSlicesLimits.at(i).at(1))) { ResolutionSlices.at(i).hFill(Resolution, Weight); }
//        }
    }
}
//</editor-fold>

// SliceFitDrawAndSave function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SliceFitDrawAndSave function">
void NeutronResolution::SliceFitDrawAndSave(const string &SampleName, double beamE) {
    for (int i = 0; i < NumberOfSlices; i++) {
        cout << "\n\n";

        TCanvas *SliceFitCanvas = new TCanvas("SliceFitCanvas", "SliceFitCanvas", 1000, 750); // normal res
        SliceFitCanvas->SetGrid();
        SliceFitCanvas->SetBottomMargin(0.14);
        SliceFitCanvas->SetLeftMargin(0.18);
        SliceFitCanvas->SetRightMargin(0.12);
        SliceFitCanvas->cd();

        //<editor-fold desc="Setting sNameFlag">
        string sNameFlag;

        if (findSubstring(SampleName, "simulation")) {
            sNameFlag = "s";
        } else if (findSubstring(SampleName, "data")) {
            sNameFlag = "d";
        }
        //</editor-fold>

        TH1D *hSlice = (TH1D *) ResolutionSlices.at(i).GetHistogram();

        if (hSlice->Integral() != 0.) { // Fit only the non-empty histograms
            FittedSlices.push_back(i); // Log slices that are been fitted

            /* A fit to a gaussian with 3 parameters: f(x) = p0*exp(-0.5((x-p1)/p2)^2)). */
            hSlice->Fit("gaus");

            /* Get fitted function to TF1 plot */
            TF1 *func = hSlice->GetFunction("gaus");
            func->SetLineColor(kRed);

            double FitAmp = func->GetParameter(0); // get p0
            double FitMean = func->GetParameter(1); // get p1
            double FitStd = func->GetParameter(2); // get p2

            ResolutionSlicesFitVar.at(i).SetMean(func->GetParameter(1));
            ResolutionSlicesFitVar.at(i).SetUpperCut(func->GetParameter(2));

            double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
            double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
            double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
            double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

            TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
            FitParam->SetBorderSize(1);
            FitParam->SetTextFont(0);
            FitParam->SetFillColor(0);
            FitParam->SetTextAlign(12);
            FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 8)).c_str());
            FitParam->AddText(("Fit mean = " + to_string_with_precision(FitMean, 8)).c_str());
            FitParam->AddText(("Fit std = " + to_string_with_precision(FitStd, 8)).c_str());
            ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
            FitParam->Draw("same");

            int SliceUpperLimPrecision;
            if (ResolutionSlicesLimits.at(i).at(1) == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

            string hSlice_CloneSaveDir = ResolutionSlices.at(i).GetHistogram1DSaveNamePath() + "00_Fitted_res_slices/";
            string hSlice_CloneSaveName = hSlice_CloneSaveDir + sNameFlag + ResolutionSlices.at(i).GetHistogram1DSaveName() + "_fitted.png";
            system(("mkdir -p " + hSlice_CloneSaveDir).c_str());

            SliceFitCanvas->SaveAs(hSlice_CloneSaveName.c_str());

            FittedNeutronResolutionSlices->Add(hSlice);

            SliceFitCanvas->Clear();
            delete SliceFitCanvas;

        } else {
            continue;
        }
    }
}
//</editor-fold>

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveResSlices function">
void NeutronResolution::DrawAndSaveResSlices(const string &SampleName, TCanvas *h1DCanvas, const string &plots_path, const string &CutsDirectory) {
    string SampleNameTemp = SampleName;

//    TList *ResSlicePlots = new TList();
    ResSlicePlots->Add(FittedNeutronResolutionSlices);

    for (int i = 0; i < NumberOfSlices; i++) { ResolutionSlices.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, ResSlicePlots, false, true, 1., 9999, 9999, 0, false); }

    TFile *PlotsFolder_fout = new TFile((plots_path + "/Neutron_resolution_plots_-_" + SampleName + ".root").c_str(), "recreate");
    PlotsFolder_fout->cd();
    ResSlicePlots->Write();
    PlotsFolder_fout->Write();
    PlotsFolder_fout->Close();

    TFile *CutsDirectory_fout = new TFile((CutsDirectory + "Neutron_resolution_plots_-_" + SampleName + ".root").c_str(), "recreate");
    CutsDirectory_fout->cd();
    ResSlicePlots->Write();
    CutsDirectory_fout->Write();
    CutsDirectory_fout->Close();
}
//</editor-fold>

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

void NeutronResolution::LogFitDataToFile(const string &SampleName, const string &plots_path, const string &CutsDirectory,
                                         const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    ofstream Neutron_res_fit_param;
    std::string Neutron_res_fit_paramFilePath = CutsDirectory + "Neutron_res_fit_param_-_" + SampleName + ".par";

    Neutron_res_fit_param.open(Neutron_res_fit_paramFilePath);
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian fit) #\n";
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "\n";
    Neutron_res_fit_param << "# Neutron resolution parameters are for - " + SampleName + ":\t" + Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_fit_param << "\n";
    Neutron_res_fit_param << "# Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:FitMean:FitSigma\n";
    Neutron_res_fit_param << "\n";

    for (int FittedSlice: FittedSlices) {
        DSCuts TempCut = ResolutionSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":" << TempCut.GetSliceUpperb()
                              << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }

    Neutron_res_fit_param.close();

    system(("cp " + Neutron_res_fit_paramFilePath + " " + plots_path).c_str()); // Copy fitted parameters file to plots folder for easy download from the ifarm
}
