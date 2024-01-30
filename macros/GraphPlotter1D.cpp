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

using namespace std;

void GraphPlotter1D() {
    cout << "\n\n";
    const char *filename = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/03_momRes_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_nResS1_Eff1_v2"
                           "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";

    const string Graph1DNameCopy = "g_Std_pol1_wKC";

//    HistogramCanvas->Clear();
    TCanvas *c1 = new TCanvas("can1", "can2", 1000, 750); // normal res
    c1->cd()->SetGrid(), c1->cd()->SetBottomMargin(0.14), c1->cd()->SetLeftMargin(0.18), c1->cd()->SetRightMargin(0.12), c1->cd();

    TFile *file = new TFile(filename);
    if (!file) { cout << "\nInvalid file! Exiting...\n", exit(0); }

    TFolder *momResDir = (TFolder *) file->Get("Fitted neutron resolution slice width");
    if (!momResDir) { cout << "\nInvalid folder! Exiting...\n", exit(0); }

    TGraph *Graph1D = (TGraph *) momResDir->FindObject(Graph1DNameCopy.c_str());
    if (!Graph1D) { cout << "\nInvalid graph! Exiting...\n", exit(0); }

    auto *funcList = Graph1D->GetListOfFunctions();
    auto *Legend = (TLegend *) funcList->At(1);
    auto *FitParam = (TPaveText *) funcList->At(2);
//
//    gPad->Update();
//    Legend->SetX1(Legend->GetX1() - 0.001);
//    Legend->SetX2(Legend->GetX2() - 0.001);

    double Legend_x1_BaseLine = gPad->GetUxmin() + 0.2, Legend_y1_BaseLine = gStyle->GetStatY(); // Top right
    double Legend_x2_BaseLine = Legend_x1_BaseLine + 0.05, Legend_y2_BaseLine = gStyle->GetStatY(); // Bottom left

    double Legend_x1_OneLine = Legend_x1_BaseLine, Legend_y1_OneLine = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_OneLine = Legend_x2_BaseLine - 0.2, Legend_y2_OneLine = Legend_y2_BaseLine - 0.25; // Bottom left

    double Legend_x1_TwoLines = Legend_x1_BaseLine, Legend_y1_TwoLines = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_TwoLines = Legend_x2_BaseLine - 0.2, Legend_y2_TwoLines = Legend_y2_BaseLine - 0.3; // Bottom left

    double Legend_x1_ThreeLines = Legend_x1_BaseLine, Legend_y1_ThreeLines = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_ThreeLines = Legend_x2_BaseLine - 0.2, Legend_y2_ThreeLines = Legend_y2_BaseLine - 0.35; // Bottom left

    double Legend_x1 = 0., Legend_y1 = 0.; // Top right
    double Legend_x2 = 0. + 0.1 + 0.1, Legend_y2 = 0. + 0.05; // Bottom left
    double Legend_xOffset = 0.2, Legend_yOffset = 0.825;
    Legend->SetX1(Legend_x1 + Legend_xOffset), Legend->SetY1(Legend_y1 + Legend_yOffset);
    Legend->SetX2(Legend_x2 + Legend_xOffset), Legend->SetY2(Legend_y2 + Legend_yOffset);

    double FitParam_x1 = 0., FitParam_y1 = 0.; // Top right
    double FitParam_x2 = 0. + 0.1 + 0.1 + 0.075, FitParam_y2 = 0. + 0.05 + 0.075; // Bottom left
    double FitParam_xOffset = 0.2, FitParam_yOffset = 0.825 - 0.075 - 0.075;
    FitParam->SetX1(FitParam_x1 + FitParam_xOffset), FitParam->SetY1(FitParam_y1 + FitParam_yOffset);
    FitParam->SetX2(FitParam_x2 + FitParam_xOffset), FitParam->SetY2(FitParam_y2 + FitParam_yOffset);

//    FitParam->SetX1(FitParam->GetX1()), FitParam->SetY1(FitParam->GetY1());
//    FitParam->SetX2(FitParam->GetX2()), FitParam->SetY2(FitParam->GetY2());
////    Legend->SetX1(Legend->GetX1()), Legend->SetY1(Legend->GetY1());
////    Legend->SetX2(Legend->GetX2()), Legend->SetY2(Legend->GetY2());
////
////    FitParam->SetX1(FitParam->GetX1()), FitParam->SetY1(FitParam->GetY1());
////    FitParam->SetX2(FitParam->GetX2()), FitParam->SetY2(FitParam->GetY2());

/*
//    gPad->Update();
    if (Graph1DNameCopy == "g_Std_pol1_wKC") {
        Legend->SetX1(Legend_x1_OneLine - 0.5 - 0.05 - 0.05 - 0.05 - 0.05 - 0.05);
        Legend->SetX2(Legend_x2_OneLine - 0.05 - 0.05 - 0.05 - 0.05 - 0.05 - 0.05 - 0.025);
    }

    if (Graph1DNameCopy == "g_Std_pol1_wKC") {
        FitParam->SetX1(Legend_x1_OneLine - 0.5 - 0.05 - 0.05 - 0.05 - 0.05 - 0.05);
        FitParam->SetY1(Legend_y1_OneLine - 0.115);
        FitParam->SetX2(Legend_x2_OneLine - 0.05 - 0.05 - 0.05 - 0.05 - 0.05 - 0.025);
    }
*/

    Legend->SetTextSize(0.03), Legend->SetTextAlign(12);
    FitParam->SetTextSize(0.03), FitParam->SetTextAlign(12);

    /* Histogram normalization setup */
    bool NormalizeHistogram;
    bool CustomNormalization;
    double CustomNormalizationFactor;

    /* Histogram appearance setup */
    int LineColor = 1;
    int LineWidth = 2;
    vector<double> Graph1DTitleSizes = {0.06, 0.0425, 0.0425}; // {TitleSize, LabelSizex, LabelSizey}
    bool CenterTitle = true;
    bool ShowStats = true;

    /* Histogram stack stuff */
    bool AddToStack = false;

    /* Histogram cuts setup */
    bool ShowPlotCuts = true;
    double PlotCuts = 0;
    double PlotXmax = 0;
    bool PlotHistogramMax = true;

    /* Histogram scale setup */
    bool LogScalePlot = false, LinearScalePlot = true;

    c1->cd();

    const double beamE = 5.98636;


    double Graph1D_integral; // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

//    Graph1D->GetXaxis()->SetTitleSize(0.06);
//    Graph1D->GetXaxis()->SetLabelSize(0.0425);
//    Graph1D->GetXaxis()->CenterTitle(true);
////    Graph1D->GetYaxis()->SetTitle("Number of events");
//    Graph1D->GetYaxis()->SetTitleSize(0.06);
//    Graph1D->GetYaxis()->SetLabelSize(0.0425);
//    Graph1D->GetYaxis()->CenterTitle(true);
//    Graph1D->SetLineWidth(LineWidth);
//    MScThesisPlotsList->Add(Graph1D);

    Graph1D->Draw("ap");

//    string SaveNameDir = "plot.png";
//    const char *SaveDir = SaveNameDir.c_str();
//    c1->SaveAs(SaveDir);
//    delete c1;
}
