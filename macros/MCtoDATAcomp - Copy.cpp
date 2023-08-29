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

//    MC_Hist->SetLineColor(kBlue);
//    MC_Hist->SetLineStyle(0);
//    MC_Hist->SetLineWidth(2);
//    MC_Hist->GetYaxis()->SetRangeUser(0, 5);
    MC_Hist->Draw();
    Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_MC_Hist.png")).c_str());

    cout << "\n\ntest 2\n\n";

//    DATA_Hist->SetLineColor(kBlack);
//    DATA_Hist->SetLineStyle(0);
//    DATA_Hist->SetLineWidth(2);
//    DATA_Hist->GetYaxis()->SetRangeUser(0, 5);
    DATA_Hist->Draw("same");
    Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_comp_Hist.png")).c_str());
    Canvas->Clear();

    cout << "\n\ntest 3\n\n";

//    DATA_Hist->SetLineColor(kBlack);
//    DATA_Hist->SetLineStyle(0);
//    DATA_Hist->SetLineWidth(2);
//    DATA_Hist->GetYaxis()->SetRangeUser(0, 5);
    DATA_Hist->Draw();
    Canvas->SaveAs((SaveDir + "/" + (to_string(counter) + "_DATA_Hist.png")).c_str());
    Canvas->Clear();

    cout << "\n\ntest 4\n\n";


    exit(0);
}

////<editor-fold desc="ng">
//void MCtoDATAcomp() {
//    string SaveDir = "MC_to_DATA_comperison";
//    system(("mkdir -p " + SaveDir).c_str());
//
//    TFile *MC_file = new TFile("C12_simulation_G18_Q204_6GeV_plots.root");
//    if (!MC_file) { cout << "\n\nMCtoDATAcomp: MC file not found! Exiting...", exit(0); }
//
//    TFile *DATA_file = new TFile("recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    if (!DATA_file) { cout << "\n\nDATAtoDATAcomp: DATA file not found! Exiting...", exit(0); }
//
//    //<editor-fold desc="Canvas definitions">
//    int canvas_x = 1000, canvas_y = 750;
//
//    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", canvas_x, canvas_y); // normal res
//    Canvas->SetGrid();
//
//    Canvas->SetBottomMargin(0.14);
//    Canvas->SetLeftMargin(0.16);
//    Canvas->SetRightMargin(0.12);
//
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 6 * canvas_x, 2 * canvas_y);
//    CanvasMulti->Divide(3, 1);
//    CanvasMulti->cd(1)->SetGrid(), CanvasMulti->cd(2)->SetGrid(), CanvasMulti->cd(3)->SetGrid();
//
//    CanvasMulti->cd(1)->SetBottomMargin(0.14), CanvasMulti->cd(1)->SetLeftMargin(0.16), CanvasMulti->cd(1)->SetRightMargin(0.12);
//    CanvasMulti->cd(2)->SetBottomMargin(0.14), CanvasMulti->cd(2)->SetLeftMargin(0.16), CanvasMulti->cd(2)->SetRightMargin(0.12);
//    CanvasMulti->cd(3)->SetBottomMargin(0.14), CanvasMulti->cd(3)->SetLeftMargin(0.16), CanvasMulti->cd(3)->SetRightMargin(0.12);
//
//    Canvas->cd();
//    //</editor-fold>
//
//    int counter = 0;
//
//    TList *MC_list = MC_file->GetListOfKeys();
//    TList *DATA_list = DATA_file->GetListOfKeys();
//
//    TKey *MC_key, *DATA_key;
//
//    TIter MC_iter(MC_list); //or TIter iter(MC_list->MakeIterator());
//    TIter DATA_iter(DATA_list); //or TIter iter(MC_list->MakeIterator());
//
//    static TString classname("TH1D");
//
//    while (MC_key = (TKey *) MC_iter()) {
//        if (MC_key->GetClassName() == classname) {
//            TH1D *MC_hist = (TH1D *) MC_key->ReadObject();
//            string MC_hist_title = MC_hist->GetTitle();
//
//            if (MC_hist) {
//                while (DATA_key = (TKey *) DATA_iter()) {
//                    if (DATA_key->GetClassName() == classname) {
//                        TH1D *DATA_hist = (TH1D *) DATA_key->ReadObject();
//                        string DATA_hist_title = DATA_hist->GetTitle();
//
//                        if (DATA_hist) {
//                            if (findSubstring(MC_hist_title, "ratio") && (MC_hist_title == DATA_hist_title)) {
//                                if (!findSubstring(MC_hist_title, "vs") && !findSubstring(MC_hist_title, "vs.")
//                                    && !findSubstring(MC_hist_title, "VS") && !findSubstring(MC_hist_title, "VS.")) {
//                                    ++counter;
//
//                                    cout << "\n\n" << MC_hist_title << "\n";
//                                    cout << DATA_hist_title << "\n";
////                    MCtoDATAcompare(counter, Canvas, CanvasMulti, TempHist_MC, TempHist_DATA, SaveDir);
//
//                                    MC_hist->Draw();
//                                    Canvas->SaveAs("MC_Hist.png");
//                                    Canvas->Clear();
//
//                                    exit(0);
//
//                                }
//                            }
//
//
//                        }
//                    }
//                }
//            }
//        }
//    }
////    for (TObject *keyAsObj_MC: *MC_file->GetListOfKeys()) {
////        auto key_MC = dynamic_cast<TKey *>(keyAsObj_MC);
////
////        TH1D *TempHist_MC = (TH1D *) keyAsObj_MC;
////        string TempHistTitle_MC = TempHist_MC->GetTitle();
////
////        for (TObject *keyAsObj_DATA: *DATA_file->GetListOfKeys()) {
////            auto key_DATA = dynamic_cast<TKey *>(keyAsObj_DATA);
////
////            TH1D *TempHist_DATA = (TH1D *) keyAsObj_DATA;
////            string TempHistTitle_DATA = TempHist_DATA->GetTitle();
////
//    //        }
////    }
//
///*
//    while (MC_key = (TKey *) MC_iter()) {
//        if (MC_key->GetClassName() == classname) {
//            TH1D *MC_hist = (TH1D *) MC_key->ReadObject();
//            string MC_hist_title = MC_hist->GetTitle();
//
//            if (MC_hist) {
//                while (DATA_key = (TKey *) DATA_iter()) {
//                    if (DATA_key->GetClassName() == classname) {
//                        TH1D *DATA_hist = (TH1D *) DATA_key->ReadObject();
//                        string DATA_hist_title = DATA_hist->GetTitle();
//
//                        if (DATA_hist) {
//                            if (findSubstring(MC_hist_title, "ratio") && (MC_hist_title == DATA_hist_title)) {
//                                if (!findSubstring(MC_hist_title, "vs") && !findSubstring(MC_hist_title, "vs.")
//                                    && !findSubstring(MC_hist_title, "VS") && !findSubstring(MC_hist_title, "VS.")) {
//                                    ++counter;
//
//                                    cout << "\n\n" << MC_hist_title << "\n";
//                                    cout << DATA_hist_title << "\n";
////                    MCtoDATAcompare(counter, Canvas, CanvasMulti, TempHist_MC, TempHist_DATA, SaveDir);
//
//                                    MC_hist->Draw();
//                                    Canvas->SaveAs("MC_Hist.png");
//                                    Canvas->Clear();
//
//                                    exit(0);
//
//                                }
//                            }
//
//
//                        }
//                    }
//                }
//            }
//        }
//    }
//*/
//
///*
//    //<editor-fold desc="Setting CanvasACorr2x3Electron">
//    TCanvas *CanvasACorr2x3Electron = new TCanvas("CanvasACorr2x3Electron", "CanvasACorr2x3Electron", 1000 * 3, 750 * 2); // normal res
//
////    CanvasACorr2x3Electron->cd();
////    CanvasACorr2x3Electron->Divide(3, 2);
//    //</editor-fold>
//
//    //<editor-fold desc="Momentum acceptance correction plots">
//    string MomACorrSaveName = "./01_Electron_Mom_ACorr_" + SampleName + ".png";
//
//    CanvasACorr2x3Electron->Divide(3, 2);
//
//    TH1D *RecPlot_mom_1p = (TH1D *) f->Get("Electron momentum APID (1p)");
//    RecPlot_mom_1p->SetLineColor(kBlue);
//    RecPlot_mom_1p->SetLineStyle(0);
//    RecPlot_mom_1p->SetLineWidth(2);
//    RecPlot_mom_1p->SetStats(1);
//    RecPlot_mom_1p->Sumw2();
//    RecPlot_mom_1p->Rebin(2);
//
//    TH1D *TruthPlot_mom_1p = (TH1D *) f->Get("TL Electron momentum AC (1p)");
//    TruthPlot_mom_1p->SetLineColor(kBlue);
//    TruthPlot_mom_1p->SetStats(1);
//    TruthPlot_mom_1p->Sumw2();
//    TruthPlot_mom_1p->Rebin(2);
//
//    TH1D *ACorrPlot_mom_1p = (TH1D *) f->Get("TL Electron momentum AC (1p)");
//    TH1D *ACorrPlot_mom_1p_Clone = (TH1D *) ACorrPlot_mom_1p->Clone("Electron momentum #alpha (1p)");
//    ACorrPlot_mom_1p_Clone->SetLineColor(kBlue);
//    ACorrPlot_mom_1p_Clone->SetStats(0);
//    ACorrPlot_mom_1p_Clone->Sumw2();
////    ACorrPlot_mom_1p_Clone->Rebin(2);
//    ACorrPlot_mom_1p_Clone->Divide(RecPlot_mom_1p);
//
//    TH1D *RecPlot_mom_1n = (TH1D *) f->Get("Electron momentum APID (1n)");
//    RecPlot_mom_1n->SetLineStyle(0);
//    RecPlot_mom_1n->SetLineWidth(2);
//    RecPlot_mom_1n->SetLineColor(kBlue);
//    RecPlot_mom_1n->SetStats(1);
//    RecPlot_mom_1n->Sumw2();
//    RecPlot_mom_1n->Rebin(2);
//
//    TH1D *TruthPlot_mom_1n = (TH1D *) f->Get("TL Electron momentum AC (1n)");
//    TruthPlot_mom_1n->SetLineColor(kBlue);
//    TruthPlot_mom_1n->SetStats(1);
//    TruthPlot_mom_1n->Sumw2();
//    TruthPlot_mom_1n->Rebin(2);
//
//    TH1D *ACorrPlot_mom_1n = (TH1D *) f->Get("TL Electron momentum AC (1n)");
//    TH1D *ACorrPlot_mom_1n_Clone = (TH1D *) ACorrPlot_mom_1n->Clone("Electron momentum #alpha (1n)");
//    ACorrPlot_mom_1n_Clone->SetLineColor(kBlue);
//    ACorrPlot_mom_1n_Clone->SetStats(0);
//    ACorrPlot_mom_1n_Clone->Sumw2();
////    ACorrPlot_mom_1n_Clone->Rebin(2);
//    ACorrPlot_mom_1n_Clone->Divide(RecPlot_mom_1n);
//
//    if (!RecPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_mom_1p_Clone) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!RecPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_mom_1n_Clone) { cout << "\n\nEmpty hist\n\n\n"; }
//
//    CanvasACorr2x3Electron->cd(1);
//    CanvasACorr2x3Electron->cd(1)->SetGrid();
//    CanvasACorr2x3Electron->cd(1)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(1)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(1)->SetRightMargin(0.12);
//    RecPlot_mom_1p->Draw();
//
//    CanvasACorr2x3Electron->cd(2);
//    CanvasACorr2x3Electron->cd(2)->SetGrid();
//    CanvasACorr2x3Electron->cd(2)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(2)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(2)->SetRightMargin(0.12);
//    TruthPlot_mom_1p->Draw();
//
//    CanvasACorr2x3Electron->cd(3);
//    CanvasACorr2x3Electron->cd(3)->SetGrid();
//    CanvasACorr2x3Electron->cd(3)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(3)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(3)->SetRightMargin(0.12);
//    ACorrPlot_mom_1p_Clone->Draw();
//
//    CanvasACorr2x3Electron->cd(4);
//    CanvasACorr2x3Electron->cd(4)->SetGrid();
//    CanvasACorr2x3Electron->cd(4)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(4)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(4)->SetRightMargin(0.12);
//    RecPlot_mom_1n->Draw();
//
//    CanvasACorr2x3Electron->cd(5);
//    CanvasACorr2x3Electron->cd(5)->SetGrid();
//    CanvasACorr2x3Electron->cd(5)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(5)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(5)->SetRightMargin(0.12);
//    TruthPlot_mom_1n->Draw();
//
//    CanvasACorr2x3Electron->cd(6);
//    CanvasACorr2x3Electron->cd(6)->SetGrid();
//    CanvasACorr2x3Electron->cd(6)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(6)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(6)->SetRightMargin(0.12);
//    ACorrPlot_mom_1n_Clone->Draw();
//
//    CanvasACorr2x3Electron->SaveAs(MomACorrSaveName.c_str());
//    CanvasACorr2x3Electron->Clear();
//    //</editor-fold>
//
//    //<editor-fold desc="Theta acceptance correction plots">
//    string ThetaACorrSaveName = "./02_Electron_Theta_ACorr_" + SampleName + ".png";
//
//    CanvasACorr2x3Electron->Divide(3, 2);
//
//    TH1D *RecPlot_Theta_1p = (TH1D *) f->Get("#theta_{e} (All Int., 1p, FD)");
//    RecPlot_Theta_1p->SetLineStyle(0);
//    RecPlot_Theta_1p->SetLineWidth(2);
//    RecPlot_Theta_1p->SetLineColor(kBlue);
//    RecPlot_Theta_1p->SetStats(1);
//    RecPlot_Theta_1p->Sumw2();
//    RecPlot_Theta_1p->Rebin(2);
//
//    TH1D *TruthPlot_Theta_1p = (TH1D *) f->Get("TL #theta^{truth}_{e} AC (1p)");
//    TruthPlot_Theta_1p->SetLineColor(kBlue);
//    TruthPlot_Theta_1p->SetStats(1);
//    TruthPlot_Theta_1p->Sumw2();
//    TruthPlot_Theta_1p->Rebin(2);
//
//    TH1D *ACorrPlot_Theta_1p = (TH1D *) f->Get("Electron theta #alpha (1p)");
//    ACorrPlot_Theta_1p->SetLineColor(kBlue);
////    ACorrPlot_Theta_1p->Sumw2();
////    ACorrPlot_Theta_1p->Rebin(2);
////    ACorrPlot_Theta_1p->Divide(TruthPlot_Theta_1p);
//
//    TH1D *RecPlot_Theta_1n = (TH1D *) f->Get("#theta_{e} (All Int., 1n, FD)");
//    RecPlot_Theta_1n->SetLineStyle(0);
//    RecPlot_Theta_1n->SetLineWidth(2);
//    RecPlot_Theta_1n->SetLineColor(kBlue);
//    RecPlot_Theta_1n->SetStats(1);
//    RecPlot_Theta_1n->Sumw2();
//    RecPlot_Theta_1n->Rebin(2);
//
//    TH1D *TruthPlot_Theta_1n = (TH1D *) f->Get("TL #theta^{truth}_{e} AC (1n)");
//    TruthPlot_Theta_1n->SetLineColor(kBlue);
//    TruthPlot_Theta_1n->SetStats(1);
//    TruthPlot_Theta_1n->Sumw2();
//    TruthPlot_Theta_1n->Rebin(2);
//
//    TH1D *ACorrPlot_Theta_1n = (TH1D *) f->Get("Electron theta #alpha (1n)");
//    ACorrPlot_Theta_1n->SetLineColor(kBlue);
////    ACorrPlot_Theta_1n->Sumw2();
////    ACorrPlot_Theta_1n->Rebin(2);
////    ACorrPlot_Theta_1n->Divide(TruthPlot_Theta_1n);
//
//    if (!RecPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!RecPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//
//    CanvasACorr2x3Electron->cd(1);
//    CanvasACorr2x3Electron->cd(1)->SetGrid();
//    CanvasACorr2x3Electron->cd(1)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(1)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(1)->SetRightMargin(0.12);
//    RecPlot_Theta_1p->Draw();
//
//    CanvasACorr2x3Electron->cd(2);
//    CanvasACorr2x3Electron->cd(2)->SetGrid();
//    CanvasACorr2x3Electron->cd(2)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(2)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(2)->SetRightMargin(0.12);
//    TruthPlot_Theta_1p->Draw();
//
//    CanvasACorr2x3Electron->cd(3);
//    CanvasACorr2x3Electron->cd(3)->SetGrid();
//    CanvasACorr2x3Electron->cd(3)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(3)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(3)->SetRightMargin(0.12);
//    ACorrPlot_Theta_1p->Draw();
//
//    CanvasACorr2x3Electron->cd(4);
//    CanvasACorr2x3Electron->cd(4)->SetGrid();
//    CanvasACorr2x3Electron->cd(4)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(4)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(4)->SetRightMargin(0.12);
//    RecPlot_Theta_1n->Draw();
//
//    CanvasACorr2x3Electron->cd(5);
//    CanvasACorr2x3Electron->cd(5)->SetGrid();
//    CanvasACorr2x3Electron->cd(5)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(5)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(5)->SetRightMargin(0.12);
//    TruthPlot_Theta_1n->Draw();
//
//    CanvasACorr2x3Electron->cd(6);
//    CanvasACorr2x3Electron->cd(6)->SetGrid();
//    CanvasACorr2x3Electron->cd(6)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(6)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(6)->SetRightMargin(0.12);
//    ACorrPlot_Theta_1n->Draw();
//
//    CanvasACorr2x3Electron->SaveAs(ThetaACorrSaveName.c_str());
//    CanvasACorr2x3Electron->Clear();
//    //</editor-fold>
//
//    //<editor-fold desc="Phi acceptance correction plots">
//    string PhiACorrSaveName = "./03_Electron_Phi_ACorr_" + SampleName + ".png";
//
//    CanvasACorr2x3Electron->Divide(3, 2);
//
//    TH1D *RecPlot_Phi_1p = (TH1D *) f->Get("#phi_{e} 1p (All Int., FD)");
//    RecPlot_Phi_1p->SetLineStyle(0);
//    RecPlot_Phi_1p->SetLineWidth(2);
//    RecPlot_Phi_1p->SetLineColor(kBlue);
//    RecPlot_Phi_1p->SetStats(1);
//    RecPlot_Phi_1p->Sumw2();
//    RecPlot_Phi_1p->Rebin(2);
//
//    TH1D *TruthPlot_Phi_1p = (TH1D *) f->Get("TL #phi^{truth}_{e} AC (1p)");
//    TruthPlot_Phi_1p->SetLineColor(kBlue);
//    TruthPlot_Phi_1p->SetStats(1);
//    TruthPlot_Phi_1p->Sumw2();
//    TruthPlot_Phi_1p->Rebin(2);
//
//    TH1D *ACorrPlot_Phi_1p = (TH1D *) f->Get("Electron phi #alpha (1p)");
//    ACorrPlot_Phi_1p->SetLineColor(kBlue);
//
////    ACorrPlot_Phi_1p->Sumw2();
////    ACorrPlot_Phi_1p->Rebin(2);
////    ACorrPlot_Phi_1p->Divide(TruthPlot_Phi_1p);
//
//    TH1D *RecPlot_Phi_1n = (TH1D *) f->Get("#phi_{e} 1n (All Int., FD)");
//    RecPlot_Phi_1n->SetLineStyle(0);
//    RecPlot_Phi_1n->SetLineWidth(2);
//    RecPlot_Phi_1n->SetLineColor(kBlue);
//    RecPlot_Phi_1n->SetStats(1);
//    RecPlot_Phi_1n->Sumw2();
//    RecPlot_Phi_1n->Rebin(2);
//
//    TH1D *TruthPlot_Phi_1n = (TH1D *) f->Get("TL #phi^{truth}_{e} AC (1n)");
//    TruthPlot_Phi_1n->SetLineColor(kBlue);
//    TruthPlot_Phi_1n->SetStats(1);
//    TruthPlot_Phi_1n->Sumw2();
//    TruthPlot_Phi_1n->Rebin(2);
//
//    TH1D *ACorrPlot_Phi_1n = (TH1D *) f->Get("Electron phi #alpha (1n)");
//    ACorrPlot_Phi_1n->SetLineColor(kBlue);
//
////    ACorrPlot_Phi_1n->Sumw2();
////    ACorrPlot_Phi_1n->Rebin(2);
////    ACorrPlot_Phi_1n->Divide(TruthPlot_Phi_1n);
//
//    if (!RecPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!RecPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//
//    CanvasACorr2x3Electron->cd(1);
//    CanvasACorr2x3Electron->cd(1)->SetGrid();
//    CanvasACorr2x3Electron->cd(1)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(1)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(1)->SetRightMargin(0.12);
//    RecPlot_Phi_1p->Draw();
//
//    CanvasACorr2x3Electron->cd(2);
//    CanvasACorr2x3Electron->cd(2)->SetGrid();
//    CanvasACorr2x3Electron->cd(2)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(2)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(2)->SetRightMargin(0.12);
//    TruthPlot_Phi_1p->Draw();
//
//    CanvasACorr2x3Electron->cd(3);
//    CanvasACorr2x3Electron->cd(3)->SetGrid();
//    CanvasACorr2x3Electron->cd(3)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(3)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(3)->SetRightMargin(0.12);
//    ACorrPlot_Phi_1p->Draw();
//
//    CanvasACorr2x3Electron->cd(4);
//    CanvasACorr2x3Electron->cd(4)->SetGrid();
//    CanvasACorr2x3Electron->cd(4)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(4)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(4)->SetRightMargin(0.12);
//    RecPlot_Phi_1n->Draw();
//
//    CanvasACorr2x3Electron->cd(5);
//    CanvasACorr2x3Electron->cd(5)->SetGrid();
//    CanvasACorr2x3Electron->cd(5)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(5)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(5)->SetRightMargin(0.12);
//    TruthPlot_Phi_1n->Draw();
//
//    CanvasACorr2x3Electron->cd(6);
//    CanvasACorr2x3Electron->cd(6)->SetGrid();
//    CanvasACorr2x3Electron->cd(6)->SetBottomMargin(0.14);
//    CanvasACorr2x3Electron->cd(6)->SetLeftMargin(0.17);
//    CanvasACorr2x3Electron->cd(6)->SetRightMargin(0.12);
//    ACorrPlot_Phi_1n->Draw();
//
//    CanvasACorr2x3Electron->SaveAs(PhiACorrSaveName.c_str());
//    CanvasACorr2x3Electron->Clear();
//    //</editor-fold>
//*/
//
//    cout << "\n";
//
//}
////</editor-fold>

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
    static TString classname("TH1D");

    for (TObject *keyAsObj_MC: *MC_file->GetListOfKeys()) {
        auto key_MC = (TKey*) keyAsObj_MC;
        if (key_MC->GetClassName() != classname) { continue; }

        TH1D *TempHist_MC = (TH1D *) key_MC->ReadObject();
        string TempHistTitle_MC = TempHist_MC->GetTitle();

        cout << "\n\n" << TempHistTitle_MC << "\n";
//        auto key_MC = dynamic_cast<TKey *>(keyAsObj_MC);
//        if (key_MC->GetClassName() != classname) { continue; }
//
//        TH1D *TempHist_MC = (TH1D *) key_MC->ReadObject();
//        string TempHistTitle_MC = TempHist_MC->GetTitle();

    }

//    for (TObject *keyAsObj_MC: *MC_file->GetListOfKeys()) {
//        auto key_MC = dynamic_cast<TKey *>(keyAsObj_MC);
//        if (key_MC->GetClassName() != classname) { continue; }
//
//        TH1D *TempHist_MC = (TH1D *) key_MC->ReadObject();
//        string TempHistTitle_MC = TempHist_MC->GetTitle();
//
//        if (!findSubstring(TempHistTitle_MC, "ratio")) { continue; }
//
//        for (TObject *keyAsObj_DATA: *DATA_file->GetListOfKeys()) {
//            auto key_DATA = dynamic_cast<TKey *>(keyAsObj_DATA);
//            if (key_DATA->GetClassName() != classname) { continue; }
//
//            TH1D *TempHist_DATA = (TH1D *) key_DATA->ReadObject();
//            string TempHistTitle_DATA = TempHist_DATA->GetTitle();
//
//            if (findSubstring(TempHistTitle_MC, "ratio") &&
//                TempHistTitle_MC == TempHistTitle_DATA) {
//
//                TempHist_MC->Draw();
//                Canvas->SaveAs("MC_Hist.png");
//                Canvas->Clear();
//
//            }
//        }
//    }

/*
    //<editor-fold desc="Setting CanvasACorr2x3Electron">
    TCanvas *CanvasACorr2x3Electron = new TCanvas("CanvasACorr2x3Electron", "CanvasACorr2x3Electron", 1000 * 3, 750 * 2); // normal res

//    CanvasACorr2x3Electron->cd();
//    CanvasACorr2x3Electron->Divide(3, 2);
    //</editor-fold>

    //<editor-fold desc="Momentum acceptance correction plots">
    string MomACorrSaveName = "./01_Electron_Mom_ACorr_" + SampleName + ".png";

    CanvasACorr2x3Electron->Divide(3, 2);

    TH1D *RecPlot_mom_1p = (TH1D *) f->Get("Electron momentum APID (1p)");
    RecPlot_mom_1p->SetLineColor(kBlue);
    RecPlot_mom_1p->SetLineStyle(0);
    RecPlot_mom_1p->SetLineWidth(2);
    RecPlot_mom_1p->SetStats(1);
    RecPlot_mom_1p->Sumw2();
    RecPlot_mom_1p->Rebin(2);

    TH1D *TruthPlot_mom_1p = (TH1D *) f->Get("TL Electron momentum AC (1p)");
    TruthPlot_mom_1p->SetLineColor(kBlue);
    TruthPlot_mom_1p->SetStats(1);
    TruthPlot_mom_1p->Sumw2();
    TruthPlot_mom_1p->Rebin(2);

    TH1D *ACorrPlot_mom_1p = (TH1D *) f->Get("TL Electron momentum AC (1p)");
    TH1D *ACorrPlot_mom_1p_Clone = (TH1D *) ACorrPlot_mom_1p->Clone("Electron momentum #alpha (1p)");
    ACorrPlot_mom_1p_Clone->SetLineColor(kBlue);
    ACorrPlot_mom_1p_Clone->SetStats(0);
    ACorrPlot_mom_1p_Clone->Sumw2();
//    ACorrPlot_mom_1p_Clone->Rebin(2);
    ACorrPlot_mom_1p_Clone->Divide(RecPlot_mom_1p);

    TH1D *RecPlot_mom_1n = (TH1D *) f->Get("Electron momentum APID (1n)");
    RecPlot_mom_1n->SetLineStyle(0);
    RecPlot_mom_1n->SetLineWidth(2);
    RecPlot_mom_1n->SetLineColor(kBlue);
    RecPlot_mom_1n->SetStats(1);
    RecPlot_mom_1n->Sumw2();
    RecPlot_mom_1n->Rebin(2);

    TH1D *TruthPlot_mom_1n = (TH1D *) f->Get("TL Electron momentum AC (1n)");
    TruthPlot_mom_1n->SetLineColor(kBlue);
    TruthPlot_mom_1n->SetStats(1);
    TruthPlot_mom_1n->Sumw2();
    TruthPlot_mom_1n->Rebin(2);

    TH1D *ACorrPlot_mom_1n = (TH1D *) f->Get("TL Electron momentum AC (1n)");
    TH1D *ACorrPlot_mom_1n_Clone = (TH1D *) ACorrPlot_mom_1n->Clone("Electron momentum #alpha (1n)");
    ACorrPlot_mom_1n_Clone->SetLineColor(kBlue);
    ACorrPlot_mom_1n_Clone->SetStats(0);
    ACorrPlot_mom_1n_Clone->Sumw2();
//    ACorrPlot_mom_1n_Clone->Rebin(2);
    ACorrPlot_mom_1n_Clone->Divide(RecPlot_mom_1n);

    if (!RecPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_mom_1p_Clone) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_mom_1n_Clone) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasACorr2x3Electron->cd(1);
    CanvasACorr2x3Electron->cd(1)->SetGrid();
    CanvasACorr2x3Electron->cd(1)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(1)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(1)->SetRightMargin(0.12);
    RecPlot_mom_1p->Draw();

    CanvasACorr2x3Electron->cd(2);
    CanvasACorr2x3Electron->cd(2)->SetGrid();
    CanvasACorr2x3Electron->cd(2)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(2)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(2)->SetRightMargin(0.12);
    TruthPlot_mom_1p->Draw();

    CanvasACorr2x3Electron->cd(3);
    CanvasACorr2x3Electron->cd(3)->SetGrid();
    CanvasACorr2x3Electron->cd(3)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(3)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(3)->SetRightMargin(0.12);
    ACorrPlot_mom_1p_Clone->Draw();

    CanvasACorr2x3Electron->cd(4);
    CanvasACorr2x3Electron->cd(4)->SetGrid();
    CanvasACorr2x3Electron->cd(4)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(4)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(4)->SetRightMargin(0.12);
    RecPlot_mom_1n->Draw();

    CanvasACorr2x3Electron->cd(5);
    CanvasACorr2x3Electron->cd(5)->SetGrid();
    CanvasACorr2x3Electron->cd(5)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(5)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(5)->SetRightMargin(0.12);
    TruthPlot_mom_1n->Draw();

    CanvasACorr2x3Electron->cd(6);
    CanvasACorr2x3Electron->cd(6)->SetGrid();
    CanvasACorr2x3Electron->cd(6)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(6)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(6)->SetRightMargin(0.12);
    ACorrPlot_mom_1n_Clone->Draw();

    CanvasACorr2x3Electron->SaveAs(MomACorrSaveName.c_str());
    CanvasACorr2x3Electron->Clear();
    //</editor-fold>

    //<editor-fold desc="Theta acceptance correction plots">
    string ThetaACorrSaveName = "./02_Electron_Theta_ACorr_" + SampleName + ".png";

    CanvasACorr2x3Electron->Divide(3, 2);

    TH1D *RecPlot_Theta_1p = (TH1D *) f->Get("#theta_{e} (All Int., 1p, FD)");
    RecPlot_Theta_1p->SetLineStyle(0);
    RecPlot_Theta_1p->SetLineWidth(2);
    RecPlot_Theta_1p->SetLineColor(kBlue);
    RecPlot_Theta_1p->SetStats(1);
    RecPlot_Theta_1p->Sumw2();
    RecPlot_Theta_1p->Rebin(2);

    TH1D *TruthPlot_Theta_1p = (TH1D *) f->Get("TL #theta^{truth}_{e} AC (1p)");
    TruthPlot_Theta_1p->SetLineColor(kBlue);
    TruthPlot_Theta_1p->SetStats(1);
    TruthPlot_Theta_1p->Sumw2();
    TruthPlot_Theta_1p->Rebin(2);

    TH1D *ACorrPlot_Theta_1p = (TH1D *) f->Get("Electron theta #alpha (1p)");
    ACorrPlot_Theta_1p->SetLineColor(kBlue);
//    ACorrPlot_Theta_1p->Sumw2();
//    ACorrPlot_Theta_1p->Rebin(2);
//    ACorrPlot_Theta_1p->Divide(TruthPlot_Theta_1p);

    TH1D *RecPlot_Theta_1n = (TH1D *) f->Get("#theta_{e} (All Int., 1n, FD)");
    RecPlot_Theta_1n->SetLineStyle(0);
    RecPlot_Theta_1n->SetLineWidth(2);
    RecPlot_Theta_1n->SetLineColor(kBlue);
    RecPlot_Theta_1n->SetStats(1);
    RecPlot_Theta_1n->Sumw2();
    RecPlot_Theta_1n->Rebin(2);

    TH1D *TruthPlot_Theta_1n = (TH1D *) f->Get("TL #theta^{truth}_{e} AC (1n)");
    TruthPlot_Theta_1n->SetLineColor(kBlue);
    TruthPlot_Theta_1n->SetStats(1);
    TruthPlot_Theta_1n->Sumw2();
    TruthPlot_Theta_1n->Rebin(2);

    TH1D *ACorrPlot_Theta_1n = (TH1D *) f->Get("Electron theta #alpha (1n)");
    ACorrPlot_Theta_1n->SetLineColor(kBlue);
//    ACorrPlot_Theta_1n->Sumw2();
//    ACorrPlot_Theta_1n->Rebin(2);
//    ACorrPlot_Theta_1n->Divide(TruthPlot_Theta_1n);

    if (!RecPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasACorr2x3Electron->cd(1);
    CanvasACorr2x3Electron->cd(1)->SetGrid();
    CanvasACorr2x3Electron->cd(1)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(1)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(1)->SetRightMargin(0.12);
    RecPlot_Theta_1p->Draw();

    CanvasACorr2x3Electron->cd(2);
    CanvasACorr2x3Electron->cd(2)->SetGrid();
    CanvasACorr2x3Electron->cd(2)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(2)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(2)->SetRightMargin(0.12);
    TruthPlot_Theta_1p->Draw();

    CanvasACorr2x3Electron->cd(3);
    CanvasACorr2x3Electron->cd(3)->SetGrid();
    CanvasACorr2x3Electron->cd(3)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(3)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(3)->SetRightMargin(0.12);
    ACorrPlot_Theta_1p->Draw();

    CanvasACorr2x3Electron->cd(4);
    CanvasACorr2x3Electron->cd(4)->SetGrid();
    CanvasACorr2x3Electron->cd(4)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(4)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(4)->SetRightMargin(0.12);
    RecPlot_Theta_1n->Draw();

    CanvasACorr2x3Electron->cd(5);
    CanvasACorr2x3Electron->cd(5)->SetGrid();
    CanvasACorr2x3Electron->cd(5)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(5)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(5)->SetRightMargin(0.12);
    TruthPlot_Theta_1n->Draw();

    CanvasACorr2x3Electron->cd(6);
    CanvasACorr2x3Electron->cd(6)->SetGrid();
    CanvasACorr2x3Electron->cd(6)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(6)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(6)->SetRightMargin(0.12);
    ACorrPlot_Theta_1n->Draw();

    CanvasACorr2x3Electron->SaveAs(ThetaACorrSaveName.c_str());
    CanvasACorr2x3Electron->Clear();
    //</editor-fold>

    //<editor-fold desc="Phi acceptance correction plots">
    string PhiACorrSaveName = "./03_Electron_Phi_ACorr_" + SampleName + ".png";

    CanvasACorr2x3Electron->Divide(3, 2);

    TH1D *RecPlot_Phi_1p = (TH1D *) f->Get("#phi_{e} 1p (All Int., FD)");
    RecPlot_Phi_1p->SetLineStyle(0);
    RecPlot_Phi_1p->SetLineWidth(2);
    RecPlot_Phi_1p->SetLineColor(kBlue);
    RecPlot_Phi_1p->SetStats(1);
    RecPlot_Phi_1p->Sumw2();
    RecPlot_Phi_1p->Rebin(2);

    TH1D *TruthPlot_Phi_1p = (TH1D *) f->Get("TL #phi^{truth}_{e} AC (1p)");
    TruthPlot_Phi_1p->SetLineColor(kBlue);
    TruthPlot_Phi_1p->SetStats(1);
    TruthPlot_Phi_1p->Sumw2();
    TruthPlot_Phi_1p->Rebin(2);

    TH1D *ACorrPlot_Phi_1p = (TH1D *) f->Get("Electron phi #alpha (1p)");
    ACorrPlot_Phi_1p->SetLineColor(kBlue);

//    ACorrPlot_Phi_1p->Sumw2();
//    ACorrPlot_Phi_1p->Rebin(2);
//    ACorrPlot_Phi_1p->Divide(TruthPlot_Phi_1p);

    TH1D *RecPlot_Phi_1n = (TH1D *) f->Get("#phi_{e} 1n (All Int., FD)");
    RecPlot_Phi_1n->SetLineStyle(0);
    RecPlot_Phi_1n->SetLineWidth(2);
    RecPlot_Phi_1n->SetLineColor(kBlue);
    RecPlot_Phi_1n->SetStats(1);
    RecPlot_Phi_1n->Sumw2();
    RecPlot_Phi_1n->Rebin(2);

    TH1D *TruthPlot_Phi_1n = (TH1D *) f->Get("TL #phi^{truth}_{e} AC (1n)");
    TruthPlot_Phi_1n->SetLineColor(kBlue);
    TruthPlot_Phi_1n->SetStats(1);
    TruthPlot_Phi_1n->Sumw2();
    TruthPlot_Phi_1n->Rebin(2);

    TH1D *ACorrPlot_Phi_1n = (TH1D *) f->Get("Electron phi #alpha (1n)");
    ACorrPlot_Phi_1n->SetLineColor(kBlue);

//    ACorrPlot_Phi_1n->Sumw2();
//    ACorrPlot_Phi_1n->Rebin(2);
//    ACorrPlot_Phi_1n->Divide(TruthPlot_Phi_1n);

    if (!RecPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasACorr2x3Electron->cd(1);
    CanvasACorr2x3Electron->cd(1)->SetGrid();
    CanvasACorr2x3Electron->cd(1)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(1)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(1)->SetRightMargin(0.12);
    RecPlot_Phi_1p->Draw();

    CanvasACorr2x3Electron->cd(2);
    CanvasACorr2x3Electron->cd(2)->SetGrid();
    CanvasACorr2x3Electron->cd(2)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(2)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(2)->SetRightMargin(0.12);
    TruthPlot_Phi_1p->Draw();

    CanvasACorr2x3Electron->cd(3);
    CanvasACorr2x3Electron->cd(3)->SetGrid();
    CanvasACorr2x3Electron->cd(3)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(3)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(3)->SetRightMargin(0.12);
    ACorrPlot_Phi_1p->Draw();

    CanvasACorr2x3Electron->cd(4);
    CanvasACorr2x3Electron->cd(4)->SetGrid();
    CanvasACorr2x3Electron->cd(4)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(4)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(4)->SetRightMargin(0.12);
    RecPlot_Phi_1n->Draw();

    CanvasACorr2x3Electron->cd(5);
    CanvasACorr2x3Electron->cd(5)->SetGrid();
    CanvasACorr2x3Electron->cd(5)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(5)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(5)->SetRightMargin(0.12);
    TruthPlot_Phi_1n->Draw();

    CanvasACorr2x3Electron->cd(6);
    CanvasACorr2x3Electron->cd(6)->SetGrid();
    CanvasACorr2x3Electron->cd(6)->SetBottomMargin(0.14);
    CanvasACorr2x3Electron->cd(6)->SetLeftMargin(0.17);
    CanvasACorr2x3Electron->cd(6)->SetRightMargin(0.12);
    ACorrPlot_Phi_1n->Draw();

    CanvasACorr2x3Electron->SaveAs(PhiACorrSaveName.c_str());
    CanvasACorr2x3Electron->Clear();
    //</editor-fold>
*/

    cout << "\n";

}
//</editor-fold>
