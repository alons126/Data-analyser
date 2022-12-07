//
// replaces - splines_plot.cpp
// COMMAND4ROOT: root splines_plot.cpp+ .L (from Julia)
// COMMAND4ROOT: root -b -q plotter_1x1.cpp (mine)
//

#include<iostream>
#include<TH1.h>
#include<TFile.h>
#include<TDirectoryFile.h>
#include<TGraph.h>
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;

void plotter_1x1() {

//    TCanvas *c = new TCanvas("c", "c", 800, 600);
    TCanvas *c = new TCanvas("c", "c", 1600, 1200);
    c->SetBottomMargin(0.1275);
    c->SetLeftMargin(0.1275);
    c->SetGrid();

//    std::string filename1 = "temp/spline_asportes_11_1000060120_2222_wfsi_SuSAv2_fixed_original_table.root";
//    std::string filename2 = "temp/spline_asportes_11_1000060120_2222_wfsi_SuSAv2_fixed_my_tables.root";
//    std::string filename1 = "spline_asportes_11_1000060120_2222_wfsi_SuSAv2_fixed_original_table.root";
//    std::string filename2 = "spline_asportes_11_1000060120_2222_wfsi_SuSAv2_fixed_my_tables_no_zeros.root";

//    std::string filename1 = "spline_asportes_11_1000060120_2222_wfsi_SuSAv2_fixed_original_table.root";
    std::string filename1 = "spline_asportes_11_1000060120_2222_wfsi_SuSAv2_fixed_original_table_test_2.root";
    std::string filename2 = "total_xsec.root";


    std::string graphTitle = "Xsec Comparison - New vs Old Total Tensor Tables";

    TFile *fxsec1 = TFile::Open(filename1.c_str());
    TFile *fxsec2 = TFile::Open(filename2.c_str());

    TDirectoryFile *mydir1 = (TDirectoryFile *) fxsec1->Get("e-_C12");
    TDirectoryFile *mydir2 = (TDirectoryFile *) fxsec2->Get("e-_C12");

    TGraph *graph1 = (TGraph *) mydir1->Get("tot_em");
//    TGraph *graph2 = (TGraph *) mydir2->Get("tot_em");
    TGraph *graph2 = (TGraph *) mydir2->Get("mec_em");

    graph1->SetLineColor(kRed);
    graph1->SetLineWidth(4);
    graph1->GetXaxis()->SetRangeUser(0, 5);
    graph1->GetYaxis()->SetRangeUser(0, 625E6);
    graph1->SetTitle(graphTitle.c_str());
    graph1->GetXaxis()->SetTitleSize(0.05);
    graph1->GetYaxis()->SetTitleSize(0.05);
    graph1->GetXaxis()->CenterTitle(true);
    graph1->GetYaxis()->CenterTitle(true);

    graph2->SetLineColor(kBlue);
    graph2->SetLineWidth(2);
    graph2->GetXaxis()->SetRangeUser(0, 5);
    graph2->GetYaxis()->SetRangeUser(0, 652E6);
    graph2->SetTitle(graphTitle.c_str());
    graph1->GetXaxis()->SetTitleSize(0.05);
    graph1->GetYaxis()->SetTitleSize(0.05);
    graph1->GetXaxis()->CenterTitle(true);
    graph1->GetYaxis()->CenterTitle(true);

    graph1->Draw("AL");
    graph2->Draw("L same");

    auto legend = new TLegend(0.128, 0.8, 0.378, 0.9);
//    auto legend = new TLegend(0.1, 0.8, 0.35, 0.9);
//    legend->AddEntry(graph1, "original_table_test_2");
    legend->AddEntry(graph1, "Original table");
    legend->AddEntry(graph2, "New table");
//    legend->AddEntry(graph2, "new_table_test_4");
//    legend->AddEntry(graph2, "original_table_test_2");
    legend->Draw();

    c->SaveAs("Xsec Comparison - New vs Old Tensor Tables.png");
    c->Clear();

    return;
}

