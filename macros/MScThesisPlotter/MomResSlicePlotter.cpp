#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>

#include "../../source/functions/GeneralFunctions.h"

#if Independent1DMomResDraw
#include "Histofinder1D.cpp"
#include "TitleAligner.cpp"
#include "../../source/classes/hData/hData.cpp"
#endif

using namespace std;

/* Command to run code: clas12root /PATH2CODE/MomResSlicePlotter.cpp -b -q */

/*
void MomResSliceFitParam(const char *filename, const string &NucleonCutsDirectory, const string &MomentumType,
                         vector<double> &Amp, vector<double> &mean, vector<double> &Std) {
    ifstream infile;
    string FileInput = "../../Source/data/NeutronResolution/Res_data_-_C12x4_simulation_G18_Q204_6GeV/" + filename;

    infile.open(FileInput.c_str());

    if (infile.is_open()) {
        string tp;

        //remove 3 lines of header
        for (int i = 0; i < 3; i++)
            getline(infile, tp);

        while (getline(infile, tp)) {
            stringstream ss(tp);
            string parameter, parameter2;
            double value;
            ss >> parameter;

            if (findSubstring(parameter, ("fit_" + MomentumType + "_Slice_#")) {
                //get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);
                string TempInput;
                string detector;
                int count = 0; //parameter number
                int pid = -99;
                vector<double> par;

                while (getline(ss2, TempInput, ':')) {
                    if (count == 0)
                        pid = stoi(TempInput);
                    else if (count < 3)
                        par.push_back(atof(TempInput.c_str()));
                    else if (count == 3)
                        detector = TempInput;

                    count++;
                }
            }
        }
    } else
        cout << "Parameter file didn't read in " << endl;
}
*/

Double_t CFitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}

void MomResSlicePlotter(const char *filename, const string &ParticleNameShortInput, const int &InitialSlice, const int &FinalSlice, const string &MomentumTyp,
                        const string &OutputPathInput = "") {
    bool PresMode = false, ExamPresMode = false;

#if PresentationMode
    PresMode = true;
#endif

#if ExamPresentationMode
    ExamPresMode = true;
#endif

    bool PrintOutHistName = false;
    bool PrintOut = false;
    bool ShowStats = false;
    string OutputPath = OutputPathInput + "/" + ParticleNameShortInput + "_" + MomentumTyp + "_MomResSlices";
    system(("mkdir -p " + OutputPath).c_str());

    hData particles;

    double x_1 = 0.2, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;
    int LineWidth = 3;
//    int LineWidth = 4;

    string SaveName, SaveNamePrefix, Filename = filename;

    if (findSubstring(Filename, "momResS1T") || findSubstring(Filename, "momResS1")) {
        SaveNamePrefix = "momResS1";
    } else if (findSubstring(Filename, "momResS2T") || findSubstring(Filename, "momResS2")) {
        SaveNamePrefix = "momResS2";
    } else if (findSubstring(Filename, "momResS2RT")) {
        SaveNamePrefix = "momResS2RT";
    }

    // Open the ROOT file containing histograms
    TFile *file = new TFile(filename);

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return;
    }

//    // Create a canvas to plot histograms in a 3x3 grid
    TCanvas *canvas = new TCanvas("c", "c", 1000 * 3 * 1.5 * 0.6, 750 * 6 * 1.5 * 0.6);
//    TCanvas *canvas = new TCanvas("c", "c", 1000 * 3 * 1.5, 750 * 6 * 1.5);

    canvas->Divide(3, 6); // Divide the canvas into a 4x16 grid

    int canvasIndex = 1;
    int canvasIndexMax = 3 * 6;
    int numOfFigures = 1;
    int numHistograms = FinalSlice - InitialSlice;

    TH1D *Histogram1D;

    int Counter = 0, HistogramCounter = 0, SliceCounter = 0;
    static TString classname("TH1D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());
    TH1D *Histogram1DTemp;

    while ((Key = (TKey *) Next())) {
        Histogram1DTemp = (TH1D *) Key->ReadObj();

        string Histogram1DTempName = Histogram1DTemp->GetName(), Histogram1DTempTitle = Histogram1DTemp->GetTitle();

        if (PrintOutHistName) { cout << Histogram1DTempName << "\n\n"; }

        if (findSubstring(Histogram1DTempName, (ParticleNameShortInput + " res.")) &&
            findSubstring(Histogram1DTempName, MomentumTyp)) {
            ++HistogramCounter;

            if (HistogramCounter >= InitialSlice && HistogramCounter <= FinalSlice) {
                if (PrintOut) { cout << "\nKey name: " << Histogram1DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n"; }

                canvas->cd(canvasIndex), canvas->cd(canvasIndex)->SetGrid();
                canvas->cd(canvasIndex)->SetBottomMargin(0.14), canvas->cd(canvasIndex)->SetLeftMargin(0.18), canvas->cd(canvasIndex)->SetRightMargin(0.12);

                if (Histogram1DTemp->Integral() == 0.) {
                    TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
                    displayText->SetTextSize(diplayTextSize), displayText->SetFillColor(0), displayText->AddText("Empty histogram"), displayText->SetTextAlign(22);
                    Histogram1DTemp->Draw(), displayText->Draw();
                } else {
                    string Histogram1DTemp_Title = Histogram1DTemp->GetTitle();
                    string Histogram1DTemp_xLabel = Histogram1DTemp->GetXaxis()->GetTitle();
                    string xLabel_2Replace = "R_{" + ParticleNameShortInput + "FD} = " + "(P^{truth}_{" + ParticleNameShortInput + "FD} - P^{reco}_{" +
                                             ParticleNameShortInput + "FD})/P^{truth}_{" + ParticleNameShortInput + "FD}";
                    string xLabel_Replacement = "R_{" + ParticleNameShortInput + "FD}";

                    Histogram1DTemp->SetLineWidth(LineWidth);

                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, " (1n, FD)", " in 1nFD");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, " (1p, FD)", " in 1pFD");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "1pFD", "1pFD0pCD");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "1nFD", "1nFD0pCD");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "[GeV/c]", "[GeV]");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, xLabel_2Replace, xLabel_Replacement);
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "Proton resolution", "R_{pFD}");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "Neutron resolution", "R_{nFD}");

#if PresentationMode
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "1pFD0pCD", "1p");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "1nFD0pCD", "1n");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "1pFD1pCD", "2p");
                    TitleAligner(particles, Histogram1DTemp, Histogram1DTemp_Title, Histogram1DTemp_xLabel, "1nFD1pCD", "1n1p");
#endif

                    auto *ListOfFunctions = Histogram1DTemp->GetListOfFunctions();
                    auto *FitParam = (TPaveText *) ListOfFunctions->At(0);

                    TF1 *func = new TF1("fit", CFitFunction, -1., 1., 3); // create a function with 3 parameters in the range [-3,3]
                    func->SetLineColor(kRed);

                    auto amp0 = FitParam->GetLine(0);
                    string Amp1 = amp0->GetTitle(), Amp = Amp1.substr(Amp1.find_last_of("=") + 1);

                    auto mean0 = FitParam->GetLine(1);
                    string Mean1 = mean0->GetTitle(), Mean = Mean1.substr(Mean1.find_last_of("=") + 1);

                    auto std0 = FitParam->GetLine(2);
                    string Std1 = std0->GetTitle(), Std = Std1.substr(Std1.find_last_of("=") + 1);

                    func->SetParameters(stod(Amp), stod(Mean), stod(Std)); // start fit with histogram's max and mean
                    func->SetLineWidth(LineWidth);

                    double xOffset = SetxOffset1D(ShowStats) + 0.045, yOffset = SetyOffset1D(ShowStats) + 0.035;
                    auto ListOfFunctionsTemp = Histogram1DTemp->GetListOfFunctions();
                    auto *FitParamTemp = (TPaveText *) ListOfFunctions->At(0);

                    FitParamTemp->SetX1NDC(FitParamTemp->GetX1NDC() + xOffset), FitParamTemp->SetX2NDC(FitParamTemp->GetX2NDC() + xOffset);
                    FitParamTemp->SetY1NDC(FitParamTemp->GetY1NDC() + yOffset), FitParamTemp->SetY2NDC(FitParamTemp->GetY2NDC() + yOffset);

                    Histogram1DTemp->Draw(), gPad->Update();
                    func->Draw("same && c");
                }

                if (!ShowStats) {
                    Histogram1DTemp->SetStats(0);
                } else {
                    Histogram1DTemp->SetStats(1);
                }

                ++canvasIndex;

                SaveName = ParticleNameShortInput + "_" + SaveNamePrefix + "_" + to_string(numOfFigures) + ".png";

                if ((canvasIndex > canvasIndexMax) || (HistogramCounter == FinalSlice)) {
                    canvas->SaveAs((OutputPath + "/" + SaveName).c_str());
                    canvas->Clear();

                    canvas->Divide(3, 6); // Divide the canvas into a 3x6 grid

                    ++numOfFigures;

                    canvasIndex = 1;
                }
            }
        }

        if (HistogramCounter > FinalSlice) { break; }
    }

    delete canvas;
    delete file;
}