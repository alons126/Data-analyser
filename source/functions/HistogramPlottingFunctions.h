//
// Created by alons on 27/01/2023.
//

#ifndef DETECTORSIMULATIONANALYSER_C_HISTOGRAMPLOTTINGFUNCTIONS_H
#define DETECTORSIMULATIONANALYSER_C_HISTOGRAMPLOTTINGFUNCTIONS_H

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
#include <sstream>

// ======================================================================================================================================================================
// histPlotter1D function
// ======================================================================================================================================================================

// histPlotter1D function (old) -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (old)">
void histPlotter1D(TCanvas *Histogram1DCanvas, //The canvas
                   TH1D *Histogram1D, //The histogram
                   bool normalize_Histogram, //Normalize histogram or not
                   bool custom_normalization, //Normalize histogram or not
                   double custom_normalization_factor, //Normalize histogram or not
                   string Histogram1DTitle,
                   string Histogram1DTitleReactions,
                   double titleSize,
                   double labelSizex,
                   double labelSizey,
                   TList *Histogram_list,
                   int lineWidth,
                   bool logScalePlot,
                   bool linearScalePlot,
                   THStack *Histogram1DStack,
                   string Histogram1DSaveName,
                   string Histogram1DSaveNamePath,
                   string finalState,
                   int kColor = 1,
                   bool centerTitle = true,
                   bool addToStack = false,
                   bool showStats = true,
                   bool title2 = false,
                   bool apply_plot_cuts = false,
                   double plot_cuts = 0,
                   double plot_Xmax = 0,
                   bool plot_max = true) {

//  Normalization factor:
    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
//    double x_1 = 0.2, y_1 = 0.3, x_2 = 0.9, y_2 = 0.7;
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
//    double x_1 = 0.15, y_1 = 0.3, x_2 = 0.85, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }
    } else if (normalize_Histogram == false) {
        string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }
//        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
//            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
//            upper_cut->SetLineColor(kMagenta);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
//            lower_cut->SetLineColor(kMagenta);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
//            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + to_string_with_precision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

//    if (showStats == false) {
//        Histogram1D->SetStats(0);
//        gStyle->SetOptStat(000001111);
//        gROOT->ForceStyle();
////        gStyle->SetOptStat(111110);
////        Histogram1D->SetOptStat(111110);
//    } else if (showStats == true) {
////        gStyle->SetOptStat(000001111);
//        gStyle->SetOptStat(111110);
//        gROOT->ForceStyle();
//    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();

}
//</editor-fold>

// histPlotter1D function (unsymmetric cuts for SF plots) ---------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (unsymmetric cuts for SF plots)">
void histPlotter1D(TCanvas *Histogram1DCanvas, //The canvas
                   TH1D *Histogram1D, //The histogram
                   bool normalize_Histogram, //Normalize histogram or not
                   bool custom_normalization, //Normalize histogram or not
                   double custom_normalization_factor, //Normalize histogram or not
                   string Histogram1DTitle,
                   string Histogram1DTitleReactions,
                   TList *Histogram_list,
                   int lineWidth,
                   bool logScalePlot,
                   bool linearScalePlot,
                   THStack *Histogram1DStack,
                   string Histogram1DSaveName,
                   string Histogram1DSaveNamePath,
                   string finalState,
                   int kColor = 1,
                   bool addToStack = false,
                   bool showStats = true,
                   bool title2 = false,
                   bool apply_plot_cuts = false,
                   double plot_upper_cut = 0,
                   double plot_lower_cut = 0,
                   double plot_Xmax = 0,
                   bool plot_max = true) {

//  Normalization factor:
    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
//    double x_1 = 0.2, y_1 = 0.3, x_2 = 0.9, y_2 = 0.7;
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
//    double x_1 = 0.15, y_1 = 0.3, x_2 = 0.85, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }
    } else if (normalize_Histogram == false) {
        string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }
//        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
//            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Upper_cut = plot_upper_cut;
        double Lower_cut = plot_lower_cut;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            if (plot_upper_cut != -1) { upper_cut->Draw("same"); }
            upper_cut->SetLineColor(kBlue);
//            upper_cut->SetLineColor(kMagenta);

            if (plot_lower_cut != -1) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);
//            lower_cut->SetLineColor(kMagenta);

            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
//            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + to_string_with_precision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

//    if (showStats == false) {
//        Histogram1D->SetStats(0);
//        gStyle->SetOptStat(000001111);
//        gROOT->ForceStyle();
////        gStyle->SetOptStat(111110);
////        Histogram1D->SetOptStat(111110);
//    } else if (showStats == true) {
////        gStyle->SetOptStat(000001111);
//        gStyle->SetOptStat(111110);
//        gROOT->ForceStyle();
//    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();

}
//</editor-fold>

// histPlotter1D function (one-sided cuts for nphe plots) ---------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (one-sided cuts for nphe plots)">
void histPlotter1D(TCanvas *Histogram1DCanvas, //The canvas
                   TH1D *Histogram1D, //The histogram
                   bool normalize_Histogram, //Normalize histogram or not
                   bool custom_normalization, //Normalize histogram or not
                   double custom_normalization_factor, //Normalize histogram or not
                   string Histogram1DTitle,
                   string Histogram1DTitleReactions,
                   TList *Histogram_list,
                   int lineWidth,
                   bool logScalePlot,
                   bool linearScalePlot,
                   THStack *Histogram1DStack,
                   string Histogram1DSaveName,
                   string Histogram1DSaveNamePath,
                   string finalState,
                   int kColor = 1,
                   bool addToStack = false,
                   bool showStats = true,
                   bool title2 = false,
                   bool apply_plot_cuts = false,
                   double plot_cut = 0,
                   double plot_Xmax = 0,
                   bool plot_max = true) {

//  Normalization factor:
    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
//    double x_1 = 0.2, y_1 = 0.3, x_2 = 0.9, y_2 = 0.7;
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
//    double x_1 = 0.15, y_1 = 0.3, x_2 = 0.85, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }
    } else if (normalize_Histogram == false) {
        string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }
//        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
//            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Cut = plot_cut;
        double plot_xmax = plot_Xmax;
        TLine *plot_cut = new TLine(Cut, 0., Cut, gPad->GetFrame()->GetY2());
        plot_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            plot_cut->Draw("same");
            plot_cut->SetLineColor(kRed);
//            plot_cut->SetLineColor(kMagenta);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
//            plot_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(plot_cut, ("Lower cut = " + to_string_with_precision(Cut, 0)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(plot_cut, ("Peak location = " + to_string_with_precision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

//    if (showStats == false) {
//        Histogram1D->SetStats(0);
//        gStyle->SetOptStat(000001111);
//        gROOT->ForceStyle();
////        gStyle->SetOptStat(111110);
////        Histogram1D->SetOptStat(111110);
//    } else if (showStats == true) {
////        gStyle->SetOptStat(000001111);
//        gStyle->SetOptStat(111110);
//        gROOT->ForceStyle();
//    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();

}
//</editor-fold>

// histPlotter1D function (Chi2 for separate plots) ---------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (Chi2 for separate plots)">
void histPlotter1D(TCanvas *Histogram1DCanvas1, // canvas c1 of other histograms
                   TCanvas *Histogram1DCanvas2, // canvas c2 of chi2 histograms
                   TH1D *Histogram1D, //The histogram
                   bool normalize_Histogram, //Normalize histogram or not
                   bool custom_normalization, //Normalize histogram or not
                   double custom_normalization_factor, //Normalize histogram or not
                   string Histogram1DTitle1,
                   string Histogram1DTitle2,
                   string Histogram1DTitle3,
                   string Particle,
                   double titleSize,
                   double labelSizex,
                   double labelSizey,
                   TList *Histogram_list,
                   int lineWidth,
                   bool logScalePlot,
                   bool linearScalePlot,
                   THStack *Histogram1DStack,
                   string Histogram1DSaveName,
                   string Histogram1DSaveNamePath,
                   int kColor = 1,
                   bool centerTitle = true,
                   bool addToStack = false,
                   bool showStats = true,
                   bool title2 = false,
                   bool apply_plot_cuts = false,
                   double plot_cuts = 0,
                   double plot_Xmax = 0,
                   bool plot_max = true) {

    double TitleFontSize = gStyle->GetTitleFontSize();
    Histogram1DCanvas2->cd();

    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")" + " - Normalized";
        string HistogramTitle =
                "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "} - #bar{#chi^{2}}_{" + Particle + "," + Histogram1DTitle3 +
                "}| #leq " + to_string_with_precision(plot_cuts) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }

    } else if (normalize_Histogram == false) {
        string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")";
        } else {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ")";
        }

        HistogramTitle =
                "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle + "," + Histogram1DTitle3 +
                "})_{max}| #leq " + to_string_with_precision(plot_cuts) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
//            upper_cut->SetLineColor(kMagenta);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
//            lower_cut->SetLineColor(kMagenta);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
//            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + to_string_with_precision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (logScalePlot == true) {
        Histogram1DCanvas2->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + Histogram1DTitle3 + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas2->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas2->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitle3 + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas2->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas2->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    Histogram1DCanvas1->cd();

}
//</editor-fold>

// histPlotter1D function (applying all Chi2 cuts) ----------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (applying all Chi2 cuts)">
void histPlotter1D(TCanvas *Histogram1DCanvas1, // canvas c1 of other histograms
                   TCanvas *Histogram1DCanvas2, // canvas c2 of chi2 histograms
                   TH1D *Histogram1D, //The histogram
                   bool normalize_Histogram, //Normalize histogram or not
                   bool custom_normalization, //Normalize histogram or not
                   double custom_normalization_factor, //Normalize histogram or not
                   string Histogram1DTitle1,
                   string Histogram1DTitle2,
                   string Histogram1DTitle3,
                   string Particle1,
                   string Particle2,
                   double titleSize,
                   double labelSizex,
                   double labelSizey,
                   TList *Histogram_list,
                   int lineWidth,
                   bool logScalePlot,
                   bool linearScalePlot,
                   THStack *Histogram1DStack,
                   string Histogram1DSaveName,
                   string Histogram1DSaveNamePath,
                   int kColor = 1,
                   bool centerTitle = true,
                   bool addToStack = false,
                   bool showStats = true,
                   bool title2 = false,
                   bool apply_plot_cuts = false,
                   double plot_cuts1 = 0,
                   double plot_Xmax1 = 0,
                   double plot_cuts2 = 0,
                   double plot_Xmax2 = 0,
                   bool plot_max = true) {

    double TitleFontSize = gStyle->GetTitleFontSize();
    Histogram1DCanvas2->cd();

    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

//    TPaveText *PrametesrBox1 = new TPaveText(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 - 0.4, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 - 0.05, "NDC");
//    PrametesrBox1->SetTextSize(12);
//    PrametesrBox1->SetFillColor(0);
////            PrametesrBox1->SetTextAlign(12);
//    PrametesrBox1->AddText(("|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle1 + "," +
//                           Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts1)).c_str());
//    PrametesrBox1->AddText(("|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle2 + "," +
//                           Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts2)).c_str());

//    TPaveText *PrametesrBox = new TPaveText(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 - 0.4, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 - 0.05, "NDC");
//    PrametesrBox->SetTextSize(12);
//    PrametesrBox->SetFillColor(0);
////            PrametesrBox->SetTextAlign(12);
//    PrametesrBox->AddText(("|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle1 + "," +
//                           Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts1)).c_str());
//    PrametesrBox->AddText(("|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle2 + "," +
//                           Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts2)).c_str());

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")" + " - Normalized";
        string HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle1 + "," +
                                Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," +
                                Histogram1DTitle3 + "} -(#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts2) +
                                ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
//            PrametesrBox1->Draw();
        }

    } else if (normalize_Histogram == false) {
        string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")";
        } else {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle1 + "," +
                         Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 +
                         "} - (#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts2) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
//            PrametesrBox1->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Upper_cut = plot_cuts1 + plot_Xmax1;
        double Lower_cut = -plot_cuts1 + plot_Xmax1;
        double plot_xmax1 = plot_Xmax1;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax1, 0., plot_Xmax1, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax1)).c_str(), "l");
            }

            Cut_legend->Draw("same");

        }
    }

    if (logScalePlot == true) {
        Histogram1DCanvas2->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + Histogram1DTitle3 + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas2->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas2->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitle3 + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas2->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas2->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    Histogram1DCanvas1->cd();

}
//</editor-fold>

// ======================================================================================================================================================================
// histPlotter2D function
// ======================================================================================================================================================================

// histPlotter2D function (Beta vs. P plots, all particles) -------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter2D function (Beta vs. P plots, all particles)">
void histPlotter2D(TCanvas *Histogram1DCanvas,
                   TH2D *Histogram2D,
                   double titleSize,
                   bool centerTitle,
                   double labelSizex,
                   double labelSizey,
                   double labelSizez,
                   TList *Histogram_list,
                   bool zlogScalePlot,
                   string Histogram1DSaveNameDir,
                   string Histogram1DSaveName,
                   TF1 *Beta_function1,
                   TF1 *Beta_function2,
                   TF1 *Beta_function3,
                   TF1 *Beta_function4,
                   TF1 *Beta_function5,
                   TF1 *Beta_function6,
                   TF1 *Beta_function7,
                   TF1 *Beta_function8,
                   TF1 *Beta_function9) {

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    Histogram2D->SetTitleSize(titleSize, "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram2D->GetXaxis()->CenterTitle(centerTitle);
    Histogram2D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram2D->GetYaxis()->CenterTitle(centerTitle);
    Histogram2D->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        Beta_function1->Draw("same");
        Beta_function2->Draw("same");
        Beta_function3->Draw("same");
        Beta_function4->Draw("same");
        Beta_function5->Draw("same");
        Beta_function6->Draw("same");
        Beta_function7->Draw("same");
        Beta_function8->Draw("same");
        Beta_function9->Draw("same");
    }

    if (zlogScalePlot == true) {
        Histogram1DCanvas->SetLogz(1);
    }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);
    Histogram1DCanvas->SaveAs((Histogram1DSaveNameDir + Histogram1DSaveName).c_str());
    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);
    Histogram1DCanvas->Clear();

}
//</editor-fold>

// histPlotter2D function (Beta vs. P plots, single particle) -----------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter2D function (Beta vs. P plots, single particle)">
void histPlotter2D(TCanvas *Histogram1DCanvas,
                   TH2D *Histogram2D,
                   double titleSize,
                   bool centerTitle,
                   double labelSizex,
                   double labelSizey,
                   double labelSizez,
                   TList *Histogram_list,
                   bool zlogScalePlot,
                   string Histogram1DSaveNameDir,
                   string Histogram1DSaveName,
                   TF1 *Beta_function1,
                   string particle1,
                   bool plot_legend) {

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.165, y_1 = 0.3, x_2 = 0.865, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    Histogram2D->SetTitleSize(titleSize, "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram2D->GetXaxis()->CenterTitle(centerTitle);
    Histogram2D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram2D->GetYaxis()->CenterTitle(centerTitle);
    Histogram2D->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        Histogram2D->SetStats(0);
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram2D->Draw("colz");
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        Beta_function1->SetLineColor(kRed);
        Beta_function1->Draw("same");
    }

    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.25, 0.87 - 0.2, 0.725 - 0.3);

    if ((plot_legend == true) && (Histogram2D->Integral() != 0.)) {

        TLegendEntry *Beta_function1_entry = Beta_vs_P_legend->AddEntry(Beta_function1, particle1.c_str(), "l");

        Beta_vs_P_legend->Draw("same");
    }

    if (zlogScalePlot == true) {
        Histogram1DCanvas->SetLogz(1);
    }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);
    Histogram1DCanvas->SaveAs((Histogram1DSaveNameDir + Histogram1DSaveName).c_str());
    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);
    Histogram1DCanvas->Clear();

}
//</editor-fold>

// histPlotter2D function (Beta vs. P plots, by charge) -----------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter2D function (Beta vs. P plots, by charge)">
void histPlotter2D(TCanvas *Histogram1DCanvas,
                   TH2D *Histogram2D,
                   double titleSize,
                   bool centerTitle,
                   double labelSizex,
                   double labelSizey,
                   double labelSizez,
                   TList *Histogram_list,
                   bool zlogScalePlot,
                   string Histogram1DSaveNameDir,
                   string Histogram1DSaveName,
                   TF1 *Beta_function1,
                   string particle1,
                   TF1 *Beta_function2,
                   string particle2,
                   TF1 *Beta_function3,
                   string particle3,
                   bool plot_legend) {

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.165, y_1 = 0.3, x_2 = 0.865, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    Histogram2D->SetTitleSize(titleSize, "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram2D->GetXaxis()->CenterTitle(centerTitle);
    Histogram2D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram2D->GetYaxis()->CenterTitle(centerTitle);
    Histogram2D->GetZaxis()->SetLabelSize(labelSizez);
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        Histogram2D->SetStats(0);
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram2D->Draw("colz");
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");

        Beta_function1->SetLineColor(kBlue);
        Beta_function1->Draw("same");
        Beta_function2->SetLineColor(kGreen);
        Beta_function2->Draw("same");
        Beta_function3->SetLineColor(kRed);
        Beta_function3->Draw("same");
    }

    auto Beta_vs_P_legend = new TLegend(0.87, 0.725 - 0.2, 0.87 - 0.2, 0.725 - 0.3);

    if ((plot_legend == true) && (Histogram2D->Integral() != 0.)) {

        TLegendEntry *Beta_function1_entry = Beta_vs_P_legend->AddEntry(Beta_function1, particle1.c_str(), "l");
        TLegendEntry *Beta_function2_entry = Beta_vs_P_legend->AddEntry(Beta_function2, particle2.c_str(), "l");
        TLegendEntry *Beta_function3_entry = Beta_vs_P_legend->AddEntry(Beta_function3, particle3.c_str(), "l");

        Beta_vs_P_legend->Draw("same");
    }

    if (zlogScalePlot == true) {
        Histogram1DCanvas->SetLogz(1);
    }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);
    Histogram1DCanvas->SaveAs((Histogram1DSaveNameDir + Histogram1DSaveName).c_str());
    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);
    Histogram1DCanvas->Clear();

}
//</editor-fold>

#endif //DETECTORSIMULATIONANALYSER_C_HISTOGRAMPLOTTINGFUNCTIONS_H
