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

// MCtoDATAcompare function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MCtoDATAcompare function">
void MCtoDATAcompare(int counter, TCanvas *Canvas, TCanvas *CanvasMulti, TH1D *MC_Histogram, TH1D *DATA_Histogram, string SaveDir) {
    Canvas->cd();

    hData Properties;

    string Histogram_Type = Properties.GetType(MC_Histogram->GetTitle());
    string Histogram_Particle = Properties.GetParticleNameShort(MC_Histogram->GetTitle());
    string FinalState = Properties.GetFS(MC_Histogram->GetTitle());

    TLine *EquiLine = new TLine(MC_Histogram->GetXaxis()->GetXmin(), 1, MC_Histogram->GetXaxis()->GetXmax(), 1);
    EquiLine->SetLineWidth(2);
    EquiLine->SetLineColor(kBlack);

    MC_Histogram->SetLineColor(kRed);
    MC_Histogram->SetLineStyle(0);
    MC_Histogram->SetLineWidth(2);
    MC_Histogram->GetYaxis()->SetRangeUser(0, 5);
    MC_Histogram->Draw();
    EquiLine->Draw("same");

    if (Histogram_Particle != "") {
        Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_MC_" + Histogram_Particle + "_" + Histogram_Type + "_" + FinalState + ".png")).c_str());
    } else {
        Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_MC_" + Histogram_Type + "_" + FinalState + ".png")).c_str());
    }

    DATA_Histogram->SetLineColor(kBlue);
    DATA_Histogram->SetLineStyle(0);
    DATA_Histogram->SetLineWidth(2);
    DATA_Histogram->GetYaxis()->SetRangeUser(0, 5);
    DATA_Histogram->Draw("same");
    EquiLine->Draw("same");

    auto Comparison_legend = new TLegend(0.87, 0.875, 0.87 - 0.2, 0.825 - 0.05);
    TLegendEntry *MC_Entry = Comparison_legend->AddEntry(MC_Histogram, "Simulation", "l");
    TLegendEntry *DATA_Entry = Comparison_legend->AddEntry(DATA_Histogram, "Data", "l");
    Comparison_legend->Draw("same");

    if (Histogram_Particle != "") {
        Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_" + Histogram_Particle + "_" + Histogram_Type + "_Comparison" + FinalState + ".png")).c_str());
    } else {
        Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_" + Histogram_Type + "_Comparison" + FinalState + ".png")).c_str());
    }

    Canvas->Clear();

    DATA_Histogram->SetLineColor(kBlue);
    DATA_Histogram->SetLineStyle(0);
    DATA_Histogram->SetLineWidth(2);
    DATA_Histogram->GetYaxis()->SetRangeUser(0, 5);
    DATA_Histogram->Draw();
    EquiLine->Draw("same");

    if (Histogram_Particle != "") {
        Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_DATA_" + Histogram_Particle + "_" + Histogram_Type + "_" + FinalState + ".png")).c_str());
    } else {
        Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_DATA_" + Histogram_Type + "_" + FinalState + ".png")).c_str());
    }

    Canvas->Clear();

//    exit(0);
}
//</editor-fold>

// MCtoDATAcomp function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MCtoDATAcomp function">
void MCtoDATAcomp() {
    string SaveDir = "MC_to_DATA_comperison";
    system(("rm -r " + SaveDir).c_str());
    system(("mkdir -p " + SaveDir).c_str());

    TFile *MC_file = new TFile("C12_simulation_G18_Q204_6GeV_plots.root");
    if (!MC_file) { cout << "\n\nMCtoDATAcomp: MC file not found! Exiting...", exit(0); }

    TFile *DATA_file = new TFile("recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
    if (!DATA_file) { cout << "\n\nDATAtoDATAcomp: DATA file not found! Exiting...", exit(0); }

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

    int counter = 0;
//    static TString classname("TH1D");

    TKey *MC_key, *DATA_key;
    TIter MC_next((TList *) MC_file->GetListOfKeys());
//    TIter MC_next((TList *) MC_file->GetListOfKeys()), DATA_next((TList *) DATA_file->GetListOfKeys());

    while (MC_key = (TKey *) MC_next()) {
        TH1D *MC_Histogram = (TH1D *) MC_key->ReadObj();
        string MC_Histogram_title = MC_Histogram->GetTitle();
        if (!findSubstring(MC_Histogram_title, "ratio")) { continue; }

        TIter DATA_next((TList *) DATA_file->GetListOfKeys());

        while (DATA_key = (TKey *) DATA_next()) {
            TH1D *DATA_Histogram = (TH1D *) DATA_key->ReadObj();
            string DATA_Histogram_title = DATA_Histogram->GetTitle();
            if (!findSubstring(DATA_Histogram_title, "ratio")) { continue; }

            if (findSubstring(DATA_Histogram_title, "ratio") && (MC_Histogram_title == DATA_Histogram_title)) {
                ++counter;
                MCtoDATAcompare(counter, Canvas, CanvasMulti, MC_Histogram, DATA_Histogram, SaveDir);
            }
        } // end of DATA while

//        cout << "\n\n\nTEST TEST TEST\n\n\n";

    } // end of MC while

    cout << "\n";

}
//</editor-fold>