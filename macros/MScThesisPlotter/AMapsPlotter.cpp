#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>

#include "../../source/functions/GeneralFunctions.h"
#include "Histofinder2D.cpp"

using namespace std;

/* Command to run code: clas12root /PATH2CODE/PlotAMaps.cpp -b -q */

void plotHistograms(const char *filename, const string &ParticleNameShort1, const int &numHistograms, const string &OutputPath = "") {
    bool PrintOutHistName = false;
    bool PrintOut = false;
    bool ShowStats = false;

    double x_1 = 0.2, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
//    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    string ParticleNameShort2 = "{" + ParticleNameShort1 + "}";
    string SaveName;

    string TL_root_file_prefix = "01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root";
    string Reco_root_file_prefix = "02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root";
    string Ratio_root_file_prefix = "03_AMap_Ratio_-_C12x4_simulation_G18_Q204_6GeV.root";
    string cPart_Sep_AMaps_root_file_prefix = "04_cPart_Sep_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root";
    string Finalized_AMaps_root_file_prefix = "05_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root";

    if (ParticleNameShort1 != "n") {
        // Open the ROOT file containing histograms
        TFile *file = new TFile(filename);

        if (!file || file->IsZombie()) {
            std::cerr << "Error: Could not open file: " << filename << std::endl;
            return;
        }

//    // Create a canvas to plot histograms in a 3x3 grid
        TCanvas *canvas = new TCanvas("c", "c", 1000 * 3 * 1.5, 750 * 3 * 1.5);
//    TCanvas *canvas = new TCanvas("c", "c", 1000 * 4 * 2, 750 * 3 * 2);

        canvas->Divide(3, 3); // Divide the canvas into a 3x3 grid

        int canvasIndex = 1;
        int canvasIndexMax;

        if (ParticleNameShort1 != "n") {
            canvasIndex = 1;
        } else {
            canvasIndex = 0;
        }

        if (numHistograms <= 9) {
            canvasIndexMax = numHistograms;
        } else {
            canvasIndexMax = 9;
        }

        int numOfFigures = 1;

        TH2D *Histogram1D;

        int Counter = 0, HistogramCounter = 0, SliceCounter = 0;
        static TString classname("TH2D");

        TKey *Key;
        TIter Next((TList *) file->GetListOfKeys());

        while (Key = (TKey *) Next()) {
            TH2D *Histogram2DTemp = (TH2D *) Key->ReadObj();

            string Histogram2DTempName = Histogram2DTemp->GetName();

            if (PrintOutHistName) { cout << Histogram2DTempName << "\n\n"; }

            if (findSubstring(Histogram2DTempName, ParticleNameShort2)) {
                ++HistogramCounter;

                if (PrintOut) { cout << "\nKey name: " << Histogram2DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n"; }

                canvas->cd(canvasIndex), canvas->cd(canvasIndex)->SetGrid();
                canvas->cd(canvasIndex)->SetBottomMargin(0.14), canvas->cd(canvasIndex)->SetLeftMargin(0.18), canvas->cd(canvasIndex)->SetRightMargin(0.12);

                if (Histogram2DTemp->Integral() == 0.) {
                    TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
                    displayText->SetTextSize(diplayTextSize);
                    displayText->SetFillColor(0);
                    displayText->AddText("Empty histogram");
                    displayText->SetTextAlign(22);
                    Histogram2DTemp->Draw();
                    displayText->Draw();
                } else {
                    Histogram2DTemp->Draw("colz");
                }

                if (!ShowStats) { Histogram2DTemp->SetStats(0); }

                ++canvasIndex;

                if (findSubstring(Histogram2DTempName, "TL ") && !findSubstring(Histogram2DTempName, "/TL")) {
                    SaveName = ParticleNameShort1 + "_TL_" + to_string(numOfFigures) + ".png";
                } else if (findSubstring(Histogram2DTempName, "Reco ")) {
                    SaveName = ParticleNameShort1 + "_Reco_" + to_string(numOfFigures) + ".png";
                } else if (findSubstring(Histogram2DTempName, "Reco/TL ratio") &&
                           !findSubstring(Histogram2DTempName, " AMaps ")) {
                    SaveName = ParticleNameShort1 + "_Ratio_" + to_string(numOfFigures) + ".png";
                    Histogram2DTemp->SetStats(0);
                } else if (findSubstring(Histogram2DTempName, " AMap ")) {
                    SaveName = ParticleNameShort1 + "_AMaps_" + to_string(numOfFigures) + ".png";
                }

                if ((canvasIndex > canvasIndexMax) || (HistogramCounter == numHistograms)) {
                    canvas->SaveAs((OutputPath + SaveName).c_str());
                    canvas->Clear();

                    canvas->Divide(3, 3); // Divide the canvas into a 3x3 grid

                    ++numOfFigures;

                    canvasIndex = 1;
                }
            }

            if (HistogramCounter > numHistograms) { break; }
        }

        delete canvas;
    } else {
        SaveName = "nFD_maps.png";

        string FileName0 = filename;
        string FileName = FileName0.substr(0, FileName0.find_last_of('/') + 1);

        cout << "\n";

        cout << "\n\nLoading 'TL Neutron Hit Map (AMaps)' from:\n" << FileName + TL_root_file_prefix << "\n";
        TFile *TL_root_file = new TFile((FileName + TL_root_file_prefix).c_str());
        TH2D *TL_Histogram2D = (TH2D *) TL_root_file->Get("TL Neutron Hit Map (AMaps)");
        if (!TL_root_file || TL_root_file->IsZombie()) { std::cerr << "Error: Could not open file: " << FileName << std::endl, exit(0); }
        if (!TL_Histogram2D) {
            std::cerr << "Error: Could not open histogram: TL Neutron Hit Map (AMaps)" << std::endl;
            std::cerr << "File path:\n" << FileName + TL_root_file_prefix << std::endl;
            exit(0);
        }

        cout << "\n\nLoading 'Reco Neutron Hit Map (AMaps)' from:\n" << FileName + TL_root_file_prefix << "\n";
        TFile *Reco_root_file = new TFile((FileName + Reco_root_file_prefix).c_str());
        TH2D *Reco_Histogram2D = (TH2D *) Reco_root_file->Get("Reco Neutron Hit Map (AMaps)");
        if (!Reco_root_file || Reco_root_file->IsZombie()) { std::cerr << "Error: Could not open file: " << FileName << std::endl, exit(0); }
        if (!Reco_Histogram2D) {
            std::cerr << "Error: Could not open histogram: Reco Neutron Hit Map (AMaps)" << std::endl;
            std::cerr << "File path:\n" << FileName + Reco_root_file_prefix << std::endl;
            exit(0);
        }

        cout << "\n\nLoading 'Neutron Reco/TL ratio (AMaps)' from:\n" << FileName + TL_root_file_prefix << "\n";
        TFile *Ratio_root_file = new TFile((FileName + Ratio_root_file_prefix).c_str());
        TH2D *Ratio_Histogram2D = Histofinder2D((FileName + Ratio_root_file_prefix).c_str(), "Neutron Reco/TL ratio (");
//        TH2D *Ratio_Histogram2D = (TH2D *) Ratio_root_file->Get("Neutron Reco/TL ratio (AMaps)");
        if (!Ratio_root_file || Ratio_root_file->IsZombie()) { std::cerr << "Error: Could not open file: " << FileName << std::endl, exit(0); }
        if (!Ratio_Histogram2D) {
            std::cerr << "Error: Could not open histogram: Neutron Reco/TL ratio (AMaps)" << std::endl;
            std::cerr << "File path:\n" << FileName + Ratio_root_file_prefix << std::endl;
            exit(0);
        }

        cout << "\n\nLoading 'Neutron_AMap (AMaps)' from:\n" << FileName + TL_root_file_prefix << "\n";
        TFile *Finalized_AMaps_root_file = new TFile((FileName + Finalized_AMaps_root_file_prefix).c_str());
        TH2D *Finalized_AMaps_Histogram2D = (TH2D *) Finalized_AMaps_root_file->Get("Neutron_AMap (AMaps)");
        if (!Finalized_AMaps_root_file || Finalized_AMaps_root_file->IsZombie()) { std::cerr << "Error: Could not open file: " << FileName << std::endl, exit(0); }
        if (!Finalized_AMaps_Histogram2D) {
            std::cerr << "Error: Could not open histogram: Neutron_AMap (AMaps)" << std::endl;
            std::cerr << "File path:\n" << FileName + Finalized_AMaps_root_file_prefix << std::endl;
            exit(0);
        }

        cout << "\nplotting into canvas...\n";
        TCanvas *canvas = new TCanvas("c", "c", 1000 * 2, 750 * 2);
        canvas->Divide(2, 2); // Divide the canvas into a 3x3 grid

        canvas->cd(1), canvas->cd(1)->SetGrid();
        canvas->cd(1)->SetBottomMargin(0.14), canvas->cd(1)->SetLeftMargin(0.18), canvas->cd(1)->SetRightMargin(0.12);

        if (TL_Histogram2D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            TL_Histogram2D->Draw();
            displayText->Draw();
        } else {
            TL_Histogram2D->Draw("colz");
        }

        if (!ShowStats) { TL_Histogram2D->SetStats(0); }

        canvas->cd(2), canvas->cd(2)->SetGrid();
        canvas->cd(2)->SetBottomMargin(0.14), canvas->cd(2)->SetLeftMargin(0.18), canvas->cd(2)->SetRightMargin(0.12);

        if (Reco_Histogram2D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Reco_Histogram2D->Draw();
            displayText->Draw();
        } else {
            Reco_Histogram2D->Draw("colz");
        }

        if (!ShowStats) { Reco_Histogram2D->SetStats(0); }

        canvas->cd(3), canvas->cd(3)->SetGrid();
        canvas->cd(3)->SetBottomMargin(0.14), canvas->cd(3)->SetLeftMargin(0.18), canvas->cd(3)->SetRightMargin(0.12);

        if (Ratio_Histogram2D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Ratio_Histogram2D->Draw();
            displayText->Draw();
        } else {
            Ratio_Histogram2D->Draw("colz");
        }

        if (!ShowStats) { Ratio_Histogram2D->SetStats(0); }

        canvas->cd(4), canvas->cd(4)->SetGrid();
        canvas->cd(4)->SetBottomMargin(0.14), canvas->cd(4)->SetLeftMargin(0.18), canvas->cd(4)->SetRightMargin(0.12);

        if (Finalized_AMaps_Histogram2D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            Finalized_AMaps_Histogram2D->Draw();
            displayText->Draw();
        } else {
            Finalized_AMaps_Histogram2D->Draw("colz");
        }

        if (!ShowStats) { Finalized_AMaps_Histogram2D->SetStats(0); }

        cout << "\nOutputPath + SaveName = " << OutputPath + SaveName << "\n";

        canvas->SaveAs((OutputPath + SaveName).c_str());

        delete canvas;
    }
}

void AMapsPlotter(const string &InputPath = "", const string &OutputPath = "") {
    string PlotsPath_prefix = "12_Acceptance_maps_plots/01_AMaps_1e_cut/05_Generated_maps/";
    string PlotsPath;

    if (InputPath != "") {
        PlotsPath = InputPath + PlotsPath_prefix;
    } else {
        PlotsPath = PlotsPath_prefix;
    }

    string TL_root_file_prefix = "01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root";
    string Reco_root_file_prefix = "02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root";
    string Ratio_root_file_prefix = "03_AMap_Ratio_-_C12x4_simulation_G18_Q204_6GeV.root";
    string cPart_Sep_AMaps_root_file_prefix = "04_cPart_Sep_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root";

    plotHistograms((PlotsPath + TL_root_file_prefix).c_str(), "e", 9, OutputPath);
    plotHistograms((PlotsPath + Reco_root_file_prefix).c_str(), "e", 9, OutputPath);
    plotHistograms((PlotsPath + Ratio_root_file_prefix).c_str(), "e", 9, OutputPath);
    plotHistograms((PlotsPath + cPart_Sep_AMaps_root_file_prefix).c_str(), "e", 9, OutputPath);

    plotHistograms((PlotsPath + TL_root_file_prefix).c_str(), "p", 9, OutputPath);
    plotHistograms((PlotsPath + Reco_root_file_prefix).c_str(), "p", 9, OutputPath);
    plotHistograms((PlotsPath + Ratio_root_file_prefix).c_str(), "p", 9, OutputPath);
    plotHistograms((PlotsPath + cPart_Sep_AMaps_root_file_prefix).c_str(), "p", 9, OutputPath);

    plotHistograms((PlotsPath + TL_root_file_prefix).c_str(), "n", 4, OutputPath);
}
