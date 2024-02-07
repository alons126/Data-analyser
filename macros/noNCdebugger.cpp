/* root BetaFitAndSave.cpp -q -b */

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

#include "macros/MScThesisPlotter/HistPlotter1D.cpp"
#include "macros/MScThesisPlotter/HistPlotter2D.cpp"
#include "macros/MScThesisPlotter/AMapsPlotter.cpp"
#include "macros/MScThesisPlotter/GraphPlotter1D.cpp"

using namespace std;

/* sudo mount -t drvfs E: /mnt/e */
/* clas12root macros/MScThesisPlotter/MScThesisPlotter.cpp -b -q */

void noNCdebugger() {
    hData utilities;

    const char *filename = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S02ACnoNC_v2/C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *filename = "/mnt/e/C12x4_data_6GeV/01_Initial_runs/C12x4_data_6GeV_S02ACnoNC_v2/C12x4_data_6GeV_plots.root";
    if (!filename) {cout << "\n\nEmpty file! Exiting...\n\n", exit(0); }

    TH1D *dPhi_pFD_pCD_Histogram1D = Histofinder1D(filename, "#Delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN (All Int., 2p)");
    if (!dPhi_pFD_pCD_Histogram1D) {cout << "\n\nEmpty histogram! Exiting...\n\n", exit(0); }

    TCanvas *dPhi_pFD_pCD_c = new TCanvas("dPhi_pFD_pCD_c1", "dPhi_pFD_pCD_c2", utilities.GetStandardCanvasWidth() * 1.25, utilities.GetStandardCanvasHeight() * 1.25); // normal res
    dPhi_pFD_pCD_c->cd()->SetGrid(), dPhi_pFD_pCD_c->cd()->SetBottomMargin(0.14), dPhi_pFD_pCD_c->cd()->SetLeftMargin(0.16), dPhi_pFD_pCD_c->cd()->SetRightMargin(0.16), dPhi_pFD_pCD_c->cd()->SetTopMargin(
            0.12), dPhi_pFD_pCD_c->cd();

    dPhi_pFD_pCD_Histogram1D->Draw();
    gPad->Update();
    double dPhi_pFD_pCD_mean = dPhi_pFD_pCD_Histogram1D->GetBinCenter(dPhi_pFD_pCD_Histogram1D->GetMaximumBin());

    TLine *dPhi_pFD_pCD_peak = new TLine(dPhi_pFD_pCD_mean, gPad->GetUymin(), dPhi_pFD_pCD_mean, gPad->GetUymax());
    dPhi_pFD_pCD_peak->SetLineWidth(3);
    dPhi_pFD_pCD_peak->SetLineColor(kRed);
    dPhi_pFD_pCD_peak->Draw("same");

    cout << "\ndPhi_pFD_pCD_mean = " << dPhi_pFD_pCD_mean << "\n";

    TH1D *dPhi_p1_p2_Histogram1D = Histofinder1D(filename, "#Delta#phi for small #Delta#theta_{1/2} - ZOOMIN (All Int., 2p)");
    if (!dPhi_p1_p2_Histogram1D) {cout << "\n\nEmpty histogram! Exiting...\n\n", exit(0); }

    TCanvas *dPhi_p1_p2_c = new TCanvas("dPhi_p1_p2_c1", "dPhi_p1_p2_c2", utilities.GetStandardCanvasWidth() * 1.25, utilities.GetStandardCanvasHeight() * 1.25); // normal res
    dPhi_p1_p2_c->cd()->SetGrid(), dPhi_p1_p2_c->cd()->SetBottomMargin(0.14), dPhi_p1_p2_c->cd()->SetLeftMargin(0.16), dPhi_p1_p2_c->cd()->SetRightMargin(0.16), dPhi_p1_p2_c->cd()->SetTopMargin(
            0.12), dPhi_p1_p2_c->cd();

    dPhi_p1_p2_Histogram1D->Draw();
    gPad->Update();
    double dPhi_p1_p2_mean = dPhi_p1_p2_Histogram1D->GetBinCenter(dPhi_p1_p2_Histogram1D->GetMaximumBin());

    TLine *dPhi_p1_p2_peak = new TLine(dPhi_p1_p2_mean, gPad->GetUymin(), dPhi_p1_p2_mean, gPad->GetUymax());
    dPhi_p1_p2_peak->SetLineWidth(3);
    dPhi_p1_p2_peak->SetLineColor(kRed);
    dPhi_p1_p2_peak->Draw("same");

    cout << "\ndPhi_p1_p2_mean = " << dPhi_p1_p2_mean << "\n";

}
