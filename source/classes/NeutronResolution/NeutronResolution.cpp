//
// Created by alons on 20/06/2023.
//

#include "NeutronResolution.h"

// NeutronResolution constructor ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="NeutronResolution constructor">
NeutronResolution::NeutronResolution(double beamE, const string &SavePath, double DeltaSlices) {
    SlicesSavePath = SavePath, delta = DeltaSlices;

    bool SliceAndDice = true;
    double SliceLowerLim = 0., SliceUpperLim = SliceLowerLim + delta;
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

        hPlot1D hResolutionSlice = hPlot1D("1n", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{nFD} - P^{rec.}_{nFD})/P^{truth}_{nFD}", SlicesSavePath, hSaveName,
                                           hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
        DSCuts ResSliceFitCuts = DSCuts(hCutName, "FD", "Neutron", "1n", 0, -9999, 9999);

        ResSliceFitCuts.SetSliceUpperb(SliceUpperLim);
        ResSliceFitCuts.SetSliceLowerb(SliceLowerLim);
        ResSliceFitCuts.SetSliceNumber(SliceNumber);

        ResSlices.push_back(hResolutionSlice);
        ResSlicesLimits.push_back({SliceLowerLim, SliceUpperLim});
        ResSlicesFitVar.push_back(ResSliceFitCuts);

        if ((SliceLowerLim + delta) > beamE) {
            SliceAndDice = false;
        } else {
            SliceLowerLim += delta;

            if ((SliceUpperLim + delta) > beamE) { SliceUpperLim = beamE; } else { SliceUpperLim += delta; }
        }
    }

    NumberOfSlices = SliceNumber;
}
//</editor-fold>

// hFillResPlots function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hFillResPlots function">
void NeutronResolution::hFillResPlots(double Momentum, double Resolution, double Weight) {
    for (int i = 0; i < NumberOfSlices; i++) {
        if ((Momentum >= ResSlicesLimits.at(i).at(0)) && (Momentum < ResSlicesLimits.at(i).at(1))) {
            ResSlices.at(i).hFill(Resolution, Weight);
            break; // no need to keep the loop going after filling histogram
        }
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

        TH1D *hSlice = (TH1D *) ResSlices.at(i).GetHistogram();

        if (hSlice->Integral() != 0.) { // Fit only the non-empty histograms
            FittedSlices.push_back(i); // Log slices that are been fitted

            /* A fit to a gaussian with 3 parameters: f(x) = p0*exp(-0.5((x-p1)/p2)^2)). */
            hSlice->Fit("gaus");

            /* Get fitted function to TF1 plot */
            TF1 *func = hSlice->GetFunction("gaus");
            func->SetLineColor(kRed);

            double FitAmp = func->GetParameter(0);  // get p0
            double FitMean = func->GetParameter(1); // get p1
            double FitStd = func->GetParameter(2);  // get p2

            ResSlicesFitVar.at(i).SetMean(func->GetParameter(1));
            ResSlicesFitVar.at(i).SetUpperCut(func->GetParameter(2));

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
            if (ResSlicesLimits.at(i).at(1) == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

            string hSlice_CloneSaveDir = ResSlices.at(i).GetHistogram1DSaveNamePath() + "00_Fitted_res_slices/";
            string hSlice_CloneSaveName = hSlice_CloneSaveDir + sNameFlag + ResSlices.at(i).GetHistogram1DSaveName() + "_fitted.png";
            system(("mkdir -p " + hSlice_CloneSaveDir).c_str());

            SliceFitCanvas->SaveAs(hSlice_CloneSaveName.c_str());

            FittedNeutronResSlices->Add(hSlice);

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

    ResSlicePlots->Add(FittedNeutronResSlices);

    for (int i = 0; i < NumberOfSlices; i++) { ResSlices.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, ResSlicePlots, false, true, 1., 9999, 9999, 0, false); }

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

//<editor-fold desc="LogFitDataToFile function">
void NeutronResolution::LogFitDataToFile(const string &SampleName, const string &plots_path, const string &CutsDirectory,
                                         const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    ofstream Neutron_res_fit_param;
    std::string Neutron_res_fit_paramFilePath = CutsDirectory + "Neutron_res_fit_param_-_" + SampleName + ".par";

    Neutron_res_fit_param.open(Neutron_res_fit_paramFilePath);
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian fit) #\n";
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "\n";
    Neutron_res_fit_param << "# Neutron resolution parameters are for:\n";
    Neutron_res_fit_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_fit_param << "#run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_fit_param << "#delta:\t\t" << delta << "\n";
    Neutron_res_fit_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:FitMean:FitSigma\n";
    Neutron_res_fit_param << "\n";

    for (int FittedSlice: FittedSlices) {
        DSCuts TempCut = ResSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":" << TempCut.GetSliceUpperb()
                              << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }

    Neutron_res_fit_param.close();

    system(("cp " + Neutron_res_fit_paramFilePath + " " + plots_path).c_str()); // Copy fitted parameters file to plots folder for easy download from the ifarm
}
//</editor-fold>

// ReadFitDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadFitDataParam function">
void NeutronResolution::ReadFitDataParam(const char *filename) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        string tp;

        // remove 3 lines of header
        for (int i = 0; i < 3; i++) { getline(infile, tp); }

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            string parameter, parameter2;
            ss >> parameter; // get cut identifier

            if (findSubstring(parameter, "Slice_#")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);
                string SliceParam;
                int count = 0; // parameter number

                string CutNameTemp = parameter;
                int SliceNumberTemp;
                double SliceLowerBoundaryTemp, SliceUpperBoundaryTemp, FitMeanTemp, FitSigmaTemp;

                while (getline(ss2, SliceParam, ':')) {
                    if (count == 0) {
                        SliceNumberTemp = stoi(SliceParam);
                    } else if (count == 1) {
                        SliceLowerBoundaryTemp = stod(SliceParam);
                    } else if (count == 2) {
                        SliceUpperBoundaryTemp = stod(SliceParam);
                    } else if (count == 3) {
                        FitMeanTemp = stod(SliceParam);
                    } else if (count == 4) {
                        FitSigmaTemp = stod(SliceParam);
                    }

                    count++;
                }

                DSCuts TempCut = DSCuts(CutNameTemp, "FD", "Neutron", "1n", FitMeanTemp, -9999, FitSigmaTemp);
                TempCut.SetSliceUpperb(SliceLowerBoundaryTemp);
                TempCut.SetSliceLowerb(SliceUpperBoundaryTemp);
                TempCut.SetSliceNumber(SliceNumberTemp);

                LoadedResSlicesFitVar.push_back(TempCut);
            }
        }
    }
//    else {
//        cout << "Parameter file didn't read in " << endl;
//    }
}
//</editor-fold>

// ProtonSmear function -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ProtonSmear function">
double NeutronResolution::ProtonSmear(bool apply_proton_smearing, double Momentum) {
    if (!apply_proton_smearing) {
        return Momentum;
    } else {
        for (DSCuts LoadedResolutionSlice: LoadedResSlicesFitVar) {
            TRandom3 *Rand = new TRandom3();
            double Smearing = Rand->Gaus(LoadedResolutionSlice.GetMean(), LoadedResolutionSlice.GetUpperCut());

            //TODO: check with Adi/Julia if the smearing factor can be negative
            double SmearingFactor = (Smearing * Smearing);

            if ((LoadedResolutionSlice.GetSliceLowerb() <= Momentum) && (LoadedResolutionSlice.GetSliceUpperb() >= Momentum)) { return SmearingFactor * Momentum; }
        }
    }

    return Momentum;
}
//</editor-fold>
