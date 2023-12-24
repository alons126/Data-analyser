#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>

#include "source/functions/GeneralFunctions.h"

using namespace std;

void plotHistograms(const char *filename, const string &ParticleNameShort1, const int &numHistograms) {
    bool PrintOut = false;

//    int numHistograms = 9;
//    const char* filename = "01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root";
//    string ParticleNameShort1 = "p";
    string ParticleNameShort2 = "{" + ParticleNameShort1 + "}";
    string SaveName;

    // Open the ROOT file containing histograms
    TFile *file = new TFile(filename);

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return;
    }

    // Create a canvas to plot histograms in a 3x3 grid
    TCanvas *canvas = new TCanvas("canvas", "Histograms", 1000*3, 750*3);
    canvas->Divide(3, 3); // Divide the canvas into a 3x3 grid

    int canvasIndex = 1;
    int div9 = 1;

    TH1D *Histogram1D;

//    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    static TString classname("TH1D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while (Key = (TKey *) Next()) {
        TH1D *Histogram1DTemp = (TH1D *) Key->ReadObj();

        string Histogram1DTempName = Histogram1DTemp->GetName();

        if (PrintOut) { cout << Histogram1DTempName << "\n\n"; }

        if (findSubstring(Histogram1DTempName, ParticleNameShort2)) {
            ++HistogramCounter;
//            HistogramFound = true;

            if (PrintOut) { cout << "\n\nKey name: " << Histogram1DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n\n"; }

            canvas->cd(canvasIndex);
            canvas->cd(canvasIndex)->SetGrid();
            Histogram1DTemp->Draw("colz");

            ++canvasIndex;

            if (findSubstring(Histogram1DTempName, "TL ") && !findSubstring(Histogram1DTempName, "/TL")) {
                SaveName = "TL_" + ParticleNameShort1 + ".png";
            } else if (findSubstring(Histogram1DTempName, "Reco ")) {
                SaveName = "Reco_" + ParticleNameShort1 + ".png";
            } else if (findSubstring(Histogram1DTempName, "Reco/TL ratio") &&
                       !findSubstring(Histogram1DTempName, " AMaps ")) {
                SaveName = "Ratio_" + ParticleNameShort1 + ".png";
            } else if (findSubstring(Histogram1DTempName, " AMap ")) {
                SaveName = "AMaps_" + ParticleNameShort1 + ".png";
            }
        }

        if (HistogramCounter > numHistograms) {
            break;
        }
//        else if (HistogramCounter == 9) {
//            canvas->SaveAs((div9 + SaveName).c_str());
//            ++div9;
//            canvasIndex = 0;
//        }
    }

    canvas->SaveAs(SaveName.c_str());
//    if (!HistogramFound) {
//        cout << "\n\nHistofinder1D: could not find histogram!\n";
//
//        return Histogram1D;
//    } else {
//        return Histogram1D;
//    }
}

void PlotAMaps() {
    string PlotsPath = "C12x4_simulation_G18_Q204_6GeV_S03ACNC_AMaps_Eff1/12_Acceptance_maps_plots/01_AMaps_1e_cut/05_Generated_maps/";

    plotHistograms((PlotsPath + "01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root").c_str(), "p", 9);
    plotHistograms((PlotsPath + "02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root").c_str(), "p", 9);
    plotHistograms((PlotsPath + "03_AMap_Ratio_-_C12x4_simulation_G18_Q204_6GeV.root").c_str(), "p", 9);
    plotHistograms((PlotsPath + "04_cPart_Sep_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root").c_str(), "p", 9);
}
