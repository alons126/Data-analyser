//
// Created by alons on 10/05/2023.
//

#include <iostream>
#include <fstream>
#include <string>

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

#include "../source/functions/GeneralFunctions.h"
#include "../source/classes/hData/hData.cpp"

using namespace std;

// MCtoDATAcompareHistograms function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MCtoDATAcompareHistograms function">
void MCtoDATAcompareHistograms(int HistogramCounter, TCanvas *Canvas, TCanvas *CanvasMulti, TH1D *MC_Histogram, TH1D *DATA_Histogram, string SaveDirHistograms) {
    Canvas->cd();

    hData Properties;

    bool MC_Save = false;
    bool DATA_Save = false;
    bool Comparison_Save = true;

    string Histogram_Type = Properties.GetType(MC_Histogram->GetTitle());
    string Histogram_Particle = Properties.GetParticleNameShort(MC_Histogram->GetTitle());
    string FinalState = Properties.GetFS(MC_Histogram->GetTitle());

    if (FinalState == "1n" || FinalState == "1p" || FinalState == "pFDpCD" || FinalState == "nFDpCD") {
        string SavePathHistograms = SaveDirHistograms + "/" + FinalState + "/" + Histogram_Type + "/";

        system(("mkdir -p " + SavePathHistograms).c_str());

        DATA_Histogram->Scale(MC_Histogram->Integral() / DATA_Histogram->Integral());

        MC_Histogram->SetStats(0);
        DATA_Histogram->SetStats(0);

        //<editor-fold desc="Saving MC_Histogram">
        MC_Histogram->SetLineColor(kBlue);
        MC_Histogram->SetLineStyle(0);
        MC_Histogram->SetLineWidth(3);
//    MC_Histogram->GetYaxis()->SetRangeUser(0, 5);
        MC_Histogram->Draw();

        if (MC_Save) {
            if (Histogram_Particle != "") {
                Canvas->SaveAs((SavePathHistograms + (to_string(HistogramCounter) + "_MC_" + Histogram_Particle + "_" + Histogram_Type + "_" +
                                                      FinalState + ".png")).c_str());
            } else {
                Canvas->SaveAs((SavePathHistograms + (to_string(HistogramCounter) + "_MC_" + Histogram_Type + "_" +
                                                      FinalState + ".png")).c_str());
            }
        }
        //</editor-fold>

        //<editor-fold desc="Saving MC-DATA comparison">
        DATA_Histogram->SetLineColor(kRed);
        DATA_Histogram->SetLineStyle(0);
        DATA_Histogram->SetLineWidth(2);
//    DATA_Histogram->GetYaxis()->SetRangeUser(0, 5);
        DATA_Histogram->Draw("same");

        auto Comparison_legend = new TLegend(0.87, 0.875, 0.87 - 0.2, 0.825 - 0.05);
        TLegendEntry *MC_Entry = Comparison_legend->AddEntry(MC_Histogram, "Simulation", "l");
        TLegendEntry *DATA_Entry = Comparison_legend->AddEntry(DATA_Histogram, "Data (scaled)", "l");
        Comparison_legend->Draw("same");

        if (Comparison_Save) {
            if (Histogram_Particle != "") {
                Canvas->SaveAs(
                        (SavePathHistograms + (to_string(HistogramCounter) + "_" + Histogram_Particle + "_" + Histogram_Type + "_Comp" + "_" +
                                               FinalState + ".png")).c_str());
            } else {
                Canvas->SaveAs((SavePathHistograms + (to_string(HistogramCounter) + "_" + Histogram_Type + "_Comp" + "_" +
                                                      FinalState + ".png")).c_str());
            }
        }

        Canvas->Clear();
        //</editor-fold>

        //<editor-fold desc="Saving DATA_Histogram">
        DATA_Histogram->SetLineColor(kRed);
        DATA_Histogram->SetLineStyle(0);
        DATA_Histogram->SetLineWidth(2);
//    DATA_Histogram->GetYaxis()->SetRangeUser(0, 5);

        if (DATA_Save) {
            DATA_Histogram->Draw();

            if (Histogram_Particle != "") {
                Canvas->SaveAs((SavePathHistograms + (to_string(HistogramCounter) + "_DATA_" + Histogram_Particle + "_" + Histogram_Type + "_" +
                                                      FinalState + ".png")).c_str());
            } else {
                Canvas->SaveAs((SavePathHistograms + (to_string(HistogramCounter) + "_DATA_" + Histogram_Type + "_" +
                                                      FinalState + ".png")).c_str());
            }

            Canvas->Clear();
        }
        //</editor-fold>

//        cout << "\n\nMC_Histogram->GetMaximum() = " << MC_Histogram->GetMaximum() << "\n";
//        cout << "DATA_Histogram->GetMaximum() = " << DATA_Histogram->GetMaximum() << "\n";
//        cout << "Ymax = " << Ymax << "\n\n\n";

//    exit(0);

    }}
//</editor-fold>

// MCtoDATAcompareRatios function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MCtoDATAcompareRatios function">
void MCtoDATAcompareRatios(int RatioCounter, TCanvas *Canvas, TCanvas *CanvasMulti, TH1D *MC_Histogram, TH1D *DATA_Histogram, string SaveDirRatios) {
    Canvas->cd();

    hData Properties;

    bool MC_Save = false;
    bool DATA_Save = false;
    bool Comparison_Save = true;

    string Histogram_Type = Properties.GetType(MC_Histogram->GetTitle());
    string Histogram_Particle = Properties.GetParticleNameShort(MC_Histogram->GetTitle());
    string FinalState = Properties.GetFS(MC_Histogram->GetTitle());

    TLine *EquiLine = new TLine(MC_Histogram->GetXaxis()->GetXmin(), 1, MC_Histogram->GetXaxis()->GetXmax(), 1);
    EquiLine->SetLineWidth(2);
    EquiLine->SetLineColor(kBlack);

    for (int i = 2; i < 6; i++) {

        //<editor-fold desc="Setting y range">
        MC_Histogram->GetYaxis()->SetRangeUser(0, i);
        DATA_Histogram->GetYaxis()->SetRangeUser(0, i);
        //</editor-fold>

        //<editor-fold desc="Saving MC_Histogram">
        MC_Histogram->SetLineColor(kBlue);
        MC_Histogram->SetLineStyle(0);
        MC_Histogram->SetLineWidth(3);
//    MC_Histogram->GetYaxis()->SetRangeUser(0, 5);
        MC_Histogram->Draw();
        EquiLine->Draw("same");

        if (MC_Save) {
            if (Histogram_Particle != "") {
                Canvas->SaveAs((SaveDirRatios + "/" + (to_string(RatioCounter) + "R" + to_string(i) + "_MC_" + Histogram_Particle + "_" + Histogram_Type + "_" +
                                                       FinalState + ".png")).c_str());
            } else {
                Canvas->SaveAs((SaveDirRatios + "/" + (to_string(RatioCounter) + "R" + to_string(i) + "_MC_" + Histogram_Type + "_" +
                                                       FinalState + ".png")).c_str());
            }
        }
        //</editor-fold>

        //<editor-fold desc="Saving MC-DATA comparison">
        DATA_Histogram->SetLineColor(kRed);
        DATA_Histogram->SetLineStyle(0);
        DATA_Histogram->SetLineWidth(2);
//    DATA_Histogram->GetYaxis()->SetRangeUser(0, 5);
        DATA_Histogram->Draw("same");
        EquiLine->Draw("same");

        auto Comparison_legend = new TLegend(0.87, 0.875, 0.87 - 0.2, 0.825 - 0.05);
        TLegendEntry *MC_Entry = Comparison_legend->AddEntry(MC_Histogram, "Simulation", "l");
        TLegendEntry *DATA_Entry = Comparison_legend->AddEntry(DATA_Histogram, "Data", "l");
        Comparison_legend->Draw("same");

        if (Comparison_Save) {
            if (Histogram_Particle != "") {
                Canvas->SaveAs(
                        (SaveDirRatios + "/" + (to_string(RatioCounter) + "R" + to_string(i) + "_" + Histogram_Particle + "_" + Histogram_Type + "_Comp" + "_" +
                                                FinalState + ".png")).c_str());
            } else {
                Canvas->SaveAs((SaveDirRatios + "/" + (to_string(RatioCounter) + "R" + to_string(i) + "_" + Histogram_Type + "_Comp" + "_" +
                                                       FinalState + ".png")).c_str());
            }
        }

        Canvas->Clear();
        //</editor-fold>

        //<editor-fold desc="Saving DATA_Histogram">
        DATA_Histogram->SetLineColor(kRed);
        DATA_Histogram->SetLineStyle(0);
        DATA_Histogram->SetLineWidth(2);
//    DATA_Histogram->GetYaxis()->SetRangeUser(0, 5);

        if (DATA_Save) {
            DATA_Histogram->Draw();
            EquiLine->Draw("same");

            if (Histogram_Particle != "") {
                Canvas->SaveAs((SaveDirRatios + "/" + (to_string(RatioCounter) + "R" + to_string(i) + "_DATA_" + Histogram_Particle + "_" + Histogram_Type + "_" +
                                                       FinalState + ".png")).c_str());
            } else {
                Canvas->SaveAs((SaveDirRatios + "/" + (to_string(RatioCounter) + "R" + to_string(i) + "_DATA_" + Histogram_Type + "_" +
                                                       FinalState + ".png")).c_str());
            }

            Canvas->Clear();
        }
        //</editor-fold>

//        cout << "\n\nMC_Histogram->GetMaximum() = " << MC_Histogram->GetMaximum() << "\n";
//        cout << "DATA_Histogram->GetMaximum() = " << DATA_Histogram->GetMaximum() << "\n";
//        cout << "Ymax = " << Ymax << "\n\n\n";
    }


//    exit(0);
}
//</editor-fold>

// MCtoDATAcomp function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MCtoDATAcomp function">
void MCtoDATAcomp() {
    string SaveDirRatios = "MC_to_DATA_comperison";
    string SaveDirHistograms = "MC_to_DATA_comperison/Histograms";

    system(("rm -r " + SaveDirRatios).c_str());
    system(("mkdir -p " + SaveDirRatios).c_str());
    system(("mkdir -p " + SaveDirHistograms).c_str());

    hData Properties;

    TFile *MC_file = new TFile("C12_simulation_G18_Q204_6GeV_plots.root");
    if (!MC_file) { cout << "\n\nMCtoDATAcomp: MC file not found! Exiting...\n", exit(0); }

    TFile *DATA_file = new TFile("C12x4_data_6GeV_run_015188_plots.root");
    if (!DATA_file) { cout << "\n\nDATAtoDATAcomp: DATA file not found! Exiting...\n", exit(0); }

    //<editor-fold desc="Canvas definitions">
    int canvas_x = 1000, canvas_y = 750;

    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", canvas_x, canvas_y); // normal res
    Canvas->SetGrid();

    Canvas->SetBottomMargin(0.14);
    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 6 * canvas_x, 2 * canvas_y);
    CanvasMulti->Divide(3, 1);
    CanvasMulti->cd(1)->SetGrid(), CanvasMulti->cd(2)->SetGrid(), CanvasMulti->cd(3)->SetGrid();

    CanvasMulti->cd(1)->SetBottomMargin(0.14), CanvasMulti->cd(1)->SetLeftMargin(0.16), CanvasMulti->cd(1)->SetRightMargin(0.12);
    CanvasMulti->cd(2)->SetBottomMargin(0.14), CanvasMulti->cd(2)->SetLeftMargin(0.16), CanvasMulti->cd(2)->SetRightMargin(0.12);
    CanvasMulti->cd(3)->SetBottomMargin(0.14), CanvasMulti->cd(3)->SetLeftMargin(0.16), CanvasMulti->cd(3)->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    int RatioCounter = 0, HistogramCounter = 0;
    static TString classname("TH1D");

    TKey *MC_key, *DATA_key;
    TIter MC_next((TList *) MC_file->GetListOfKeys());

    while (MC_key = (TKey *) MC_next()) {
        TH1D *MC_Histogram = (TH1D *) MC_key->ReadObj();
        string MC_Histogram_title = MC_Histogram->GetTitle();

        if (MC_key->GetClassName() != classname("TH1D")) { continue; }
        if (!(!findSubstring(MC_Histogram_title, "vs") && !findSubstring(MC_Histogram_title, "vs.") &&
              !findSubstring(MC_Histogram_title, "VS") && !findSubstring(MC_Histogram_title, "VS."))) { continue; }

        if (!findSubstring(MC_Histogram_title, "nFDpCD/pFDpCD")) {
            string MC_Histogram_type = Properties.GetType(MC_Histogram_title);

            if (MC_Histogram_type != "" || findSubstring(MC_Histogram_title,"Leading")) {
//            if (MC_Histogram_type != "") {

                /*
                TH1D *DATA_Histogram = (TH1D *) DATA_file->Get(MC_Histogram->GetName());
                string DATA_Histogram_title = DATA_Histogram->GetTitle();

                cout << "\n\n\nMC_Histogram_title = " << MC_Histogram_title << "\n";
                cout << "DATA_Histogram_title = " << DATA_Histogram_title << "\n";
                cout << "MC_key->GetClassName() = " << MC_key->GetClassName() << "\n";

                ++HistogramCounter;
                MCtoDATAcompareHistograms(HistogramCounter, Canvas, CanvasMulti, MC_Histogram, DATA_Histogram, SaveDirHistograms);
*/

                TIter DATA_next((TList *) DATA_file->GetListOfKeys());

                while (DATA_key = (TKey *) DATA_next()) {
                    TH1D *DATA_Histogram = (TH1D *) DATA_key->ReadObj();
                    string DATA_Histogram_title = DATA_Histogram->GetTitle();

                    if (DATA_key->GetClassName() != classname("TH1D")) { continue; }
                    if (!(!findSubstring(DATA_Histogram_title, "vs") && !findSubstring(DATA_Histogram_title, "vs.") &&
                          !findSubstring(DATA_Histogram_title, "VS") && !findSubstring(DATA_Histogram_title, "VS."))) { continue; }
                    if (findSubstring(DATA_Histogram_title, "nFDpCD/pFDpCD")) { continue; }

                    string DATA_Histogram_type = Properties.GetType(DATA_Histogram_title);

                    if ((DATA_Histogram_type != "" || findSubstring(DATA_Histogram_title,"Leading")) &&
                    (MC_Histogram_title == DATA_Histogram_title)) {
                        ++HistogramCounter;
                        MCtoDATAcompareHistograms(HistogramCounter, Canvas, CanvasMulti, MC_Histogram, DATA_Histogram, SaveDirHistograms);

//                        cout << "\n\n\nMC_Histogram_title = " << MC_Histogram_title << "\n";
//                        cout << "DATA_Histogram_title = " << DATA_Histogram_title << "\n";

                        break;
                    }
                } // end of DATA while
            }

            //            continue;
        } else {
            TIter DATA_next((TList *) DATA_file->GetListOfKeys());

            while (DATA_key = (TKey *) DATA_next()) {
                TH1D *DATA_Histogram = (TH1D *) DATA_key->ReadObj();
                string DATA_Histogram_title = DATA_Histogram->GetTitle();
                if (!findSubstring(DATA_Histogram_title, "nFDpCD/pFDpCD") ||
                    !(!findSubstring(DATA_Histogram_title, "vs") && !findSubstring(DATA_Histogram_title, "vs.") &&
                      !findSubstring(DATA_Histogram_title, "VS") && !findSubstring(DATA_Histogram_title, "VS."))) { continue; }

                if (findSubstring(DATA_Histogram_title, "nFDpCD/pFDpCD") && (MC_Histogram_title == DATA_Histogram_title)) {
                    ++RatioCounter;
                    MCtoDATAcompareRatios(RatioCounter, Canvas, CanvasMulti, MC_Histogram, DATA_Histogram, SaveDirRatios);
                    break;
                }
            } // end of DATA while
        }

//        cout << "\n\n\nTEST TEST TEST\n\n\n";

    } // end of MC while

    cout << "\n";

}
//</editor-fold>
