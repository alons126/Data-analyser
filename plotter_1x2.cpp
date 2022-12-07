//
// replaces - splines_plot2.cpp
// COMMAND4ROOT: root -b -q plotter_1x2.cpp
//

#include<iostream>
#include<TH1.h>
#include<TFile.h>
#include<TDirectoryFile.h>
#include<TGraph.h>
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;

void plotter_1x2() {

//    COMMAND4ROOT:     clas12root -b -q splines_plot1.cpp
//    code for 1x2 plots

    double BottomMargin = 0.125;
    double LeftMargin = 0.12;
    double RightMargin = -0.12;
//    double BottomMargin = 0.1275;
//    double LeftMargin = 0.1275;
    bool centerXtitle = true;
    bool centerYtitle = true;

    TCanvas *c = new TCanvas("c", "c", 1600, 1200*2);
    c->Divide(1, 2);

    string filename = "recon_c12_6gev_plots.root";

//    string graphTitle1 = "#font[12]{#theta_{l}} (All Interactions, 1n1p)";
//    string graphTitle2 = "#font[12]{#theta_{l}} (All Interactions, 2p)";
//    string graphTitle3 = "#font[12]{#phi_{l}} (All Interactions, 1n1p)";
//    string graphTitle4 = "#font[12]{#phi_{l}} (All Interactions, 2p)";
    string graphTitle1 = "Final State E_{l} (All Interactions, 1n1p)";
    string graphTitle2 = "Final State E_{l} (All Interactions, 2p)";
//    string graphTitle3 = "#phi_{l} (All Interactions, 1n1p)";
//    string graphTitle4 = "#phi_{l} (All Interactions, 2p)";
//    string graphTitle1 = "#font[12]{#theta_{l}} of Outgoing Lepton (All Interactions, 1n1p)";
//    string graphTitle2 = "#font[12]{#theta_{l}} of Outgoing Lepton (All Interactions, 2p)";
//    string graphTitle3 = "#font[12]{#phi_{l}} of Outgoing Lepton (All Interactions, 1n1p)";
//    string graphTitle4 = "#font[12]{#phi_{l}} of Outgoing Lepton (All Interactions, 2p)";

    TFile *fileinput = TFile::Open(filename.c_str());

    TGraph *graph1 = (TGraph *) fileinput->Get("Final State E_{l} det sim (1n1p)");
    TGraph *graph2 = (TGraph *) fileinput->Get("Final State E_{l} det sim (2p)");
//    TGraph *graph3 = (TGraph *) fileinput->Get("#phi_{l} det sim (1n1p)");
//    TGraph *graph4 = (TGraph *) fileinput->Get("#phi_{l} det sim (2p)");

    c->cd(1);
    c->cd(1)->SetGrid();
    c->cd(1)->SetBottomMargin(BottomMargin);
    c->cd(1)->SetLeftMargin(LeftMargin);
    graph1->SetLineColor(kBlue);
////    graph1->SetLineWidth(4);
//    graph1->GetXaxis()->SetRangeUser(0, 5);
//    graph1->GetYaxis()->SetRangeUser(0, 625E6);
    graph1->SetTitle(graphTitle1.c_str());
//    graph1->GetXaxis()->SetTitleSize(0.05);
//    graph1->GetYaxis()->SetTitleSize(0.05);
//    graph1->GetXaxis()->CenterTitle(centerXtitle);
//    graph1->GetYaxis()->CenterTitle(centerXtitle);
//    graph1->SetStats(0);
//    graph1->GetHistogram()->SetStats(1);
//    gStyle->SetOptStat(000001111);
    graph1->Draw();

    c->cd(2);
    c->cd(2)->SetGrid();
    c->cd(2)->SetBottomMargin(BottomMargin);
    c->cd(2)->SetLeftMargin(LeftMargin);
////    graph2->SetLineColor(kRed);
////    graph2->SetLineWidth(4);
//    graph2->GetXaxis()->SetRangeUser(0, 5);
//    graph2->GetYaxis()->SetRangeUser(0, 625E6);
    graph2->SetTitle(graphTitle2.c_str());
//    graph2->GetXaxis()->SetTitleSize(0.05);
//    graph2->GetYaxis()->SetTitleSize(0.05);
//    graph2->GetXaxis()->CenterTitle(centerXtitle);
//    graph2->GetYaxis()->CenterTitle(centerXtitle);
    graph2->Draw();

//    c->cd(3);
//    c->cd(3)->SetGrid();
//    c->cd(3)->SetBottomMargin(BottomMargin);
//    c->cd(3)->SetLeftMargin(LeftMargin);
//    graph3->SetLineColor(kBlue);
//////    graph3->SetLineWidth(4);
////    graph3->GetXaxis()->SetRangeUser(0, 5);
////    graph3->GetYaxis()->SetRangeUser(0, 625E6);
//    graph3->SetTitle(graphTitle3.c_str());
////    graph3->GetXaxis()->SetTitleSize(0.05);
////    graph3->GetYaxis()->SetTitleSize(0.05);
////    graph3->GetXaxis()->CenterTitle(centerXtitle);
////    graph3->GetYaxis()->CenterTitle(centerXtitle);
//    graph3->Draw();
//
//    c->cd(4);
//    c->cd(4)->SetGrid();
//    c->cd(4)->SetBottomMargin(BottomMargin);
//    c->cd(4)->SetLeftMargin(LeftMargin);
//////    graph4->SetLineColor(kRed);
//////    graph4->SetLineWidth(4);
////    graph4->GetXaxis()->SetRangeUser(0, 5);
////    graph4->GetYaxis()->SetRangeUser(0, 625E6);
//    graph4->SetTitle(graphTitle4.c_str());
////    graph4->GetXaxis()->SetTitleSize(0.05);
////    graph4->GetYaxis()->SetTitleSize(0.05);
////    graph4->GetXaxis()->CenterTitle(centerXtitle);
////    graph4->GetYaxis()->CenterTitle(centerXtitle);
//    graph4->Draw();

    c->SaveAs("El Comparison.png");
    c->Clear();

    return;
}

