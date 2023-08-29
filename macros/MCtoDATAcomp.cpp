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

using namespace std;

void MCtoDATAcompare(int counter, TCanvas *Canvas, TCanvas *CanvasMulti, TH1D *MC_Hist, TH1D *DATA_Hist, string SaveDir) {
    Canvas->cd();

    cout << "\n\ntest 1\n\n";

    MC_Hist->SetLineColor(kBlue);
    MC_Hist->SetLineStyle(0);
    MC_Hist->SetLineWidth(2);
    MC_Hist->GetYaxis()->SetRangeUser(0, 5);
    MC_Hist->Draw();
    Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_MC_Hist.png")).c_str());

    cout << "\n\ntest 2\n\n";

    DATA_Hist->SetLineColor(kBlack);
    DATA_Hist->SetLineStyle(0);
    DATA_Hist->SetLineWidth(2);
    DATA_Hist->GetYaxis()->SetRangeUser(0, 5);
    DATA_Hist->Draw("same");
    Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_comp_Hist.png")).c_str());
    Canvas->Clear();

    cout << "\n\ntest 3\n\n";

    DATA_Hist->SetLineColor(kBlack);
    DATA_Hist->SetLineStyle(0);
    DATA_Hist->SetLineWidth(2);
    DATA_Hist->GetYaxis()->SetRangeUser(0, 5);
    DATA_Hist->Draw();
    Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_DATA_Hist.png")).c_str());
    Canvas->Clear();

    cout << "\n\ntest 4\n\n";

    exit(0);
}

//<editor-fold desc="og">
void MCtoDATAcomp() {
    string SaveDir = "MC_to_DATA_comperison";
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
    TIter MC_next((TList *) MC_file->GetListOfKeys()), DATA_next((TList *) DATA_file->GetListOfKeys());

    while (MC_key = (TKey *) MC_next()) {
        TH1D *MC_Histogram = (TH1D *) MC_key->ReadObj();
        string MC_Histogram_title = MC_Histogram->GetTitle();
        if (!findSubstring(MC_Histogram_title, "ratio")) { continue; }

        while (DATA_key = (TKey *) DATA_next()) {
            TH1D *DATA_Histogram = (TH1D *) DATA_key->ReadObj();
            string DATA_Histogram_title = DATA_Histogram->GetTitle();
            if (!findSubstring(DATA_Histogram_title, "ratio")) { continue; }

            if (findSubstring(DATA_Histogram_title, "ratio") && (MC_Histogram_title == DATA_Histogram_title)) {
                MCtoDATAcompare(counter, Canvas, CanvasMulti, MC_Histogram, DATA_Histogram, SaveDir);
            }
        } // end of DATA while
    } // end of MC while

//    //<editor-fold desc="COPIED">
//    TFile *f = TFile::Open(fname, "READ");
//    if (!MC_file || MC_file->IsZombie()) {
//        cout << "Unable to open " << fname << " for reading..." << endl;
//        return;
//    }
//    while (key = (TKey *) next()) {
//        TClass *cl = gROOT->GetClass(key->GetClassName());
//        if (cl->InheritsFrom("TH1")) {
//            // the following line is not needed if you only want
//            // to count the histograms
//            TH1 *h = (TH1 *) key->ReadObj();
//            cout << "Histo found: " << h->GetName() << " - " << h->GetTitle() << endl;
//            total++;
//        }
//    }
//
//    cout << "Found " << total << " Histograms" << endl;
//    //</editor-fold>

//<editor-fold desc="OG">
//    for (TObject *keyAsObj_MC: *MC_file->GetListOfKeys()) {
//        auto key_MC = (TKey*) keyAsObj_MC;
//        if (key_MC->GetClassName() != classname) { continue; }
//
//        TH1D *TempHist_MC = (TH1D *) key_MC->ReadObject();
//        string TempHistTitle_MC = TempHist_MC->GetTitle();
//
//        cout << "\n\n" << TempHistTitle_MC << "\n";
//    }
//</editor-fold>

    cout << "\n";

}
//</editor-fold>
