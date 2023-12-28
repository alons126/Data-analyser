#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>

#include "source/functions/GeneralFunctions.h"

using namespace std;

/* Command to run code: clas12root /PATH2CODE/PlotAMaps.cpp -b -q */

void plotHistograms(const char *filename, const string &ParticleNameShort1, const int &numHistograms) {
    bool PrintOutHistName = false;
    bool PrintOut = false;

    string ParticleNameShort2 = "{" + ParticleNameShort1 + "}";
    string SaveName;

    // Open the ROOT file containing histograms
    TFile *file = new TFile(filename);

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return;
    }

//    // Create a canvas to plot histograms in a 3x3 grid
    TCanvas *canvas = new TCanvas("c", "c", 1000 * 4 * 1.5, 750 * 3 * 1.5);
//    TCanvas *canvas = new TCanvas("c", "c", 1000 * 4 * 2, 750 * 3 * 2);

    if (ParticleNameShort1 != "n") {
        canvas->Divide(4, 3); // Divide the canvas into a 3x3 grid
    }

    int canvasIndex = 1;
    int canvasIndexMax;

    if (ParticleNameShort1 != "n") {
        canvasIndex = 1;
    } else {
        canvasIndex = 0;
    }

    if (numHistograms <= 12) {
        canvasIndexMax = numHistograms;
    } else {
        canvasIndexMax = 12;
    }

    int numOfFigures = 1;

    TH1D *Histogram1D;

    int Counter = 0, HistogramCounter = 0, SliceCounter = 0;
    static TString classname("TH1D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while (Key = (TKey *) Next()) {
        TH1D *Histogram1DTemp = (TH1D *) Key->ReadObj();

        string Histogram1DTempName = Histogram1DTemp->GetName();

        if (PrintOutHistName) { cout << Histogram1DTempName << "\n\n"; }

        if (findSubstring(Histogram1DTempName, ParticleNameShort2)) {
            ++HistogramCounter;

            if (PrintOut) { cout << "\nKey name: " << Histogram1DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n"; }

            canvas->cd(canvasIndex), canvas->cd(canvasIndex)->SetGrid();
            canvas->cd(canvasIndex)->SetBottomMargin(0.14), canvas->cd(canvasIndex)->SetLeftMargin(0.18), canvas->cd(canvasIndex)->SetRightMargin(0.12);
            Histogram1DTemp->Draw("colz");

            ++canvasIndex;

            if (findSubstring(Histogram1DTempName, "TL ") && !findSubstring(Histogram1DTempName, "/TL")) {
                SaveName = ParticleNameShort1 + "_TL_" + to_string(numOfFigures) + ".png";
            } else if (findSubstring(Histogram1DTempName, "Reco ")) {
                SaveName = ParticleNameShort1 + "_Reco_" + to_string(numOfFigures) + ".png";
            } else if (findSubstring(Histogram1DTempName, "Reco/TL ratio") &&
                       !findSubstring(Histogram1DTempName, " AMaps ")) {
                SaveName = ParticleNameShort1 + "_Ratio_" + to_string(numOfFigures) + ".png";
                Histogram1DTemp->SetStats(0);
            } else if (findSubstring(Histogram1DTempName, " AMap ")) {
                SaveName = ParticleNameShort1 + "_AMaps_" + to_string(numOfFigures) + ".png";
            }

            if ((canvasIndex > canvasIndexMax) || (HistogramCounter == numHistograms)) {
                canvas->SaveAs(SaveName.c_str());
                canvas->Clear();

                if (ParticleNameShort1 != "n") {
                    canvas->Divide(4, 3); // Divide the canvas into a 3x3 grid
                }

                ++numOfFigures;

                canvasIndex = 1;
            }
        }

        if (HistogramCounter > numHistograms) { break; }
    }

    delete canvas;
}

////<editor-fold desc="First working">
//void plotHistograms(const char *filename, const string &ParticleNameShort1, const int &numHistograms) {
//    bool PrintOut = false;
//
////    int numHistograms = 9;
////    const char* filename = "01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root";
////    string ParticleNameShort1 = "p";
//    string ParticleNameShort2 = "{" + ParticleNameShort1 + "}";
//    string SaveName;
//
//    // Open the ROOT file containing histograms
//    TFile *file = new TFile(filename);
//
//    if (!file || file->IsZombie()) {
//        std::cerr << "Error: Could not open file: " << filename << std::endl;
//        return;
//    }
//
////    // Create a canvas to plot histograms in a 3x3 grid
//    TCanvas *canvas = new TCanvas("c", "c", 1000 * 4 * 2, 750 * 3 * 2);
//    canvas->Divide(4, 3); // Divide the canvas into a 3x3 grid
//
//    int canvasIndex = 1;
//    int numOfFigures = 1;
//
//    int numOfFiguresMax;
//
//    if (numHistograms <= 12) {
//        numOfFiguresMax = 1;
//    } else if (numHistograms % 12 != 0) {
//        numOfFiguresMax = (numHistograms / 12);
//    } else if (numHistograms % 12 != 0) {
//        numOfFiguresMax = (numHistograms / 12) + 1;
//    }
//
//    TH1D *Histogram1D;
//
////    bool HistogramFound = false;
//    int Counter = 0, HistogramCounter = 0;
//    static TString classname("TH1D");
//
//    TKey *Key;
//    TIter Next((TList *) file->GetListOfKeys());
//
//    while (Key = (TKey *) Next()) {
//        TH1D *Histogram1DTemp = (TH1D *) Key->ReadObj();
//
//        string Histogram1DTempName = Histogram1DTemp->GetName();
//
//        if (PrintOut) { cout << Histogram1DTempName << "\n\n"; }
//
//        if (findSubstring(Histogram1DTempName, ParticleNameShort2)) {
//            ++HistogramCounter;
////            HistogramFound = true;
//
//            if (PrintOut) { cout << "\n\nKey name: " << Histogram1DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n\n"; }
//
//            canvas->cd(canvasIndex);
//            canvas->cd(canvasIndex)->SetGrid();
//            Histogram1DTemp->Draw("colz");
//
//            ++canvasIndex;
//
//            if (findSubstring(Histogram1DTempName, "TL ") && !findSubstring(Histogram1DTempName, "/TL")) {
//                SaveName = to_string(numOfFigures) + "_TL_" + ParticleNameShort1 + ".png";
//            } else if (findSubstring(Histogram1DTempName, "Reco ")) {
//                SaveName = to_string(numOfFigures) + "_Reco_" + ParticleNameShort1 + ".png";
//            } else if (findSubstring(Histogram1DTempName, "Reco/TL ratio") &&
//                       !findSubstring(Histogram1DTempName, " AMaps ")) {
//                SaveName = to_string(numOfFigures) + "_Ratio_" + ParticleNameShort1 + ".png";
//                Histogram1DTemp->SetStats(0);
//            } else if (findSubstring(Histogram1DTempName, " AMap ")) {
//                SaveName = to_string(numOfFigures) + "_AMaps_" + ParticleNameShort1 + ".png";
//            }
//        }
//
////        cout << canvasIndex << "\n";
////
////        if (canvasIndex == 9) {
////            canvas->SaveAs(SaveName.c_str());
////            canvas->Clear();
////
////            ++numOfFigures;
////        }
//
//        if (HistogramCounter > numHistograms) { break; }
//    }
//
//    canvas->SaveAs(SaveName.c_str());
//    delete canvas;
//}
////</editor-fold>

void PlotAMaps() {
    string PlotsPath = "C12x4_simulation_G18_Q204_6GeV_S03ACNC_AMaps_Eff1/12_Acceptance_maps_plots/01_AMaps_1e_cut/05_Generated_maps/";

    string TL_root_file_prefix = "01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root";
    string TL_Reco_file_prefix = "02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root";
    string TL_Ratio_file_prefix = "03_AMap_Ratio_-_C12x4_simulation_G18_Q204_6GeV.root";
    string TL_cPart_Sep_AMaps_file_prefix = "04_cPart_Sep_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root";

    plotHistograms(TL_root_file_prefix.c_str(), "e", 35);
    plotHistograms(TL_Reco_file_prefix.c_str(), "e", 35);
    plotHistograms(TL_Ratio_file_prefix.c_str(), "e", 35);
    plotHistograms(TL_cPart_Sep_AMaps_file_prefix.c_str(), "e", 35);

    plotHistograms(TL_root_file_prefix.c_str(), "p", 9);
    plotHistograms(TL_Reco_file_prefix.c_str(), "p", 9);
    plotHistograms(TL_Ratio_file_prefix.c_str(), "p", 9);
    plotHistograms(TL_cPart_Sep_AMaps_file_prefix.c_str(), "p", 9);

    plotHistograms((TL_root_file_prefix).c_str(), "n", 1);
    plotHistograms((TL_Reco_file_prefix).c_str(), "n", 1);
    plotHistograms((TL_Ratio_file_prefix).c_str(), "n", 1);
    plotHistograms((TL_cPart_Sep_AMaps_file_prefix).c_str(), "n", 1);
//    plotHistograms((PlotsPath + TL_root_file_prefix).c_str(), "n", 1);
//    plotHistograms((PlotsPath + TL_Reco_file_prefix).c_str(), "n", 1);
//    plotHistograms((PlotsPath + TL_Ratio_file_prefix).c_str(), "n", 1);
//    plotHistograms((PlotsPath + TL_cPart_Sep_AMaps_file_prefix).c_str(), "n", 1);
}
