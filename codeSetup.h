
#ifndef ANALYSER_CODESETUP_H
#define ANALYSER_CODESETUP_H

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
#include <sstream>


// ======================================================================================================================================================================
// Code version
// ======================================================================================================================================================================

std::string Ver = "DetSim testings";

// ======================================================================================================================================================================
// Functions definitions
// ======================================================================================================================================================================

//<editor-fold desc="Functions definitions">

//<editor-fold desc="to_string_with_precision function">
template<typename T>
std::string to_string_with_precision(const T a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
//</editor-fold>

//<editor-fold desc="BoolToString function">
inline const char *const BoolToString(bool b) {
    return b ? "true" : "false";
}
//</editor-fold>

//<editor-fold desc="testPrint functions">
void testPrint() { cout << "\n"; }

void testPrint(string varString = "") {
    if (varString == "") {
        cout << "\n";
    } else {
        cout << varString << "\n";
    }
}

void testPrint(int var, string varString = "") {
    if (varString == "") {
        cout << "\n";
    } else {
        cout << varString << " = " << var << "\n";
    }
}

void testPrint(double var, string varString = "") {
    if (varString == "") {
        cout << "\n";
    } else {
        cout << varString << " = " << var << "\n";
    }
}
//</editor-fold>

//<editor-fold desc="rCalc function">
double rCalc(double x, double y, double z) {
    double r = sqrt(x * x + y * y + z * z);
    return r;
}
//</editor-fold>

//<editor-fold desc="MakeDirectory">
void MakeDirectory(bool Create_Directory, std::string Plots_Parent_Folder, std::string Plots_Daughter_Folder, bool Clear_Parent_Folder_content = false,
                   std::string Parent_Folder = "./plots") {

    string MakeDirectory = "mkdir -p " + Parent_Folder;
    string RemoveDirectoryContent = "rm -r " + Parent_Folder + "/" + Plots_Parent_Folder + "/*";
//    cout << "rm -r " + Parent_Folder + "/" + Plots_Parent_Folder + "/*" << "\n\n";

    if (Clear_Parent_Folder_content == true && Create_Directory == true) {
        system(RemoveDirectoryContent.c_str());
        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
    } else if (Clear_Parent_Folder_content == false && Create_Directory == true) {
        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
    }

    /*
//    if (Clear_Parent_Folder_content == true) {
//        system(RemoveDirectoryContent.c_str());
//    }
//
//    if (Create_Directory == true) {
//        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
////        cout << MakeDir + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder << "\n\n";
//    }
     */

}
//</editor-fold>

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

            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(plot_cut, ("Lower cut = " + to_string_with_precision(Cut,0)).c_str(), "l");

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

//</editor-fold>

// ======================================================================================================================================================================
// file & path definitions
// ======================================================================================================================================================================

// path definitions -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="path definitions">
std::string plots_path = "./plots/";
std::string plots_file_type = "_plots.root";
//</editor-fold>

// file_name definitions and selection ----------------------------------------------------------------------------------------------------------------------------------

std::string file_name = "general_file";

// hipo files:

//std::string AnalyseFile = "/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo";
//std::string AnalyseFile = "e_on_1000060120_EMMEC_MCmaster_test.root";
std::string AnalyseFile = "/mnt/d/e4nu/hipo_data_files/recon_c12_6gev/recon_c12_6gev.hipo";
//std::string AnalyseFile = "/home/alon/project/recon_c12_6gev_9_torus-1.0.hipo";

std::string AnalyseFilePath = "mnt/d/e4nu/hipo_data_files";
//std::string AnalyseFilePath = "/mnt/d/e4nu/hipo_data_files/";
//std::string AnalyseFileSample = "recon_c12_6gev"; // Justin's ~1M
std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_1"; // my test with Justin's code; no pions data saved

//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1"; // my test with Josh's code
//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_2"; // my test with Justin's code
//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_2_first_10"; // my test with Justin's code
//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_2_first_100"; // my test with Justin's code
//std::string AnalyseFileSample = "recon_c12_6gev";
std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";

//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
//std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/" + "first_50_test" + "/";


//// root files:
//
//std::string AnalyseFile = "./e_on_1000060120_598636MeV.gst.root";
//
//std::string AnalyseFilePath = "mnt/d/e4nu/hipo_data_files";
//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
//std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
//
////std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
////std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/" + "first_50_test" + "/";
//
////std::string AnalyseFileDir = "/home/alon/project/temp/";

// ======================================================================================================================================================================
// Strucks definitions
// ======================================================================================================================================================================

// TODO: add strucks is needed

// ======================================================================================================================================================================
// Classes definitions
// ======================================================================================================================================================================

//double beamE; // electron energy declaration

// TargetParameters class -----------------------------------------------------------------------------------------------------------------------------------------------

class TargetParameters {
protected:
    string TargetElement;
    int TargetElementPDG, ProbePDG;
    double BindingEnergyToNucleus;

public:
    void SetTargetElement(string TargetElementName) { TargetElement = TargetElementName; }

    void SetTargetElementPDG(int TargetElementPDGNumber) { TargetElementPDG = TargetElementPDGNumber; }

    void SetProbePDG(int ProbePDGNumber) { ProbePDG = ProbePDGNumber; }

    void SetBindingEnergyToNucleus(double BindingEnergy) { BindingEnergyToNucleus = BindingEnergy; }

    string GetTargetElement() { return TargetElement; }

    int GetTargetElementPDG() { return TargetElementPDG; }

    int GetProbePDG() { return ProbePDG; }

    double GetBindingEnergyToNucleus() { return BindingEnergyToNucleus; }
};

// Histogram class ------------------------------------------------------------------------------------------------------------------------------------------------------

class Histogram {
protected:
    TH1D Histogram1D;
    TH2D Histogram2D;
    string HistogramTitle, HistogramStatTitle, XaxisTitle, YaxisTitle;
    int HistogramNumberOfXBins, HistogramNumberOfYBins;
    double UpperXlim, LowerXlim, UpperYlim, LowerYlim, UpperZlim, LowerZlim;
    bool NormalizeHistogram;
    bool CustomNormalization;
    double CustomNormalizationFactor;
//    string Histogram1DTitle;
    string Histogram1DTitleReactions;
    double TitleSize;
    double LabelSizex;
    double LabelSizey;
    TList *HistogramList;
    int LineWidth;
    bool LogScalePlot;
    bool LinearScalePlot;
    THStack *Histogram1DStack;
    string Histogram1DSaveName;
    string Histogram1DSaveNamePath;
    string FinalState;
    int kColor = 1;
    bool CenterTitle = true;
    bool AddToStack = false;
    bool ShowStats = true;
    bool Title2 = false;
    bool ApplyPlotCuts = false;
    double PlotCuts = 0;
    double PlotXmax = 0;
    bool PlotHistogramMax = true;

public:
//  Set methods:
    void SetHistogram1D(TH1D Histogram) { Histogram1D = Histogram; }

    void SetHistogram2D(TH2D Histogram) { Histogram2D = Histogram; }

    void SetHistogramTitle(string hTitle) { HistogramTitle = hTitle; }

    void SetHistogramStatTitle(string sTitle) { HistogramStatTitle = sTitle; }

    void SetXaxisTitle(string xTitle) { XaxisTitle = xTitle; }

    void SetYaxisTitle(string yTitle) { YaxisTitle = yTitle; }

    void SetHistogramHeadTitles(string hTitle, string sTitle) {
        HistogramTitle = hTitle;
        HistogramStatTitle = sTitle;
    }

    void SetHistogramTitles(string hTitle, string sTitle, string xTitle, string yTitle) {
        HistogramTitle = hTitle;
        HistogramStatTitle = sTitle;
        XaxisTitle = xTitle;
        YaxisTitle = yTitle;
    }

    void SetHistogramTitles(string hTitle, string sTitle, string xTitle) {
        HistogramTitle = hTitle;
        HistogramStatTitle = sTitle;
        XaxisTitle = xTitle;
    }

    void SetAxisTitles(string xTitle, string yTitle) {
        XaxisTitle = xTitle;
        YaxisTitle = yTitle;
    }

    void SetHistogramNumberOfXBins(int NumberOfXBins) { HistogramNumberOfXBins = NumberOfXBins; }

    void SetHistogramNumberOfYBins(int NumberOfYBins) { HistogramNumberOfYBins = NumberOfYBins; }

    void SetHistogramNumberOfBins(int NumberOfXBins, int NumberOfYBins) {
        HistogramNumberOfXBins = NumberOfXBins;
        HistogramNumberOfYBins = NumberOfYBins;
    }

    void SetUpperXlim(double uXlim) { UpperXlim = uXlim; }

    void SetLowerXlim(double lXlim) { LowerXlim = lXlim; }

    void SetUpperYlim(double uYlim) { UpperYlim = uYlim; }

    void SetLowerYlim(double lYlim) { LowerYlim = lYlim; }

    void SetUpperZlim(double uZlim) { UpperZlim = uZlim; }

    void SetLowerZlim(double lZlim) { LowerZlim = lZlim; }

    void SetXAxisLimits(double uXlim, double lXlim) {
        UpperXlim = uXlim;
        LowerXlim = lXlim;
    }

    void SetYAxisLimits(double uYlim, double lYlim) {
        UpperYlim = uYlim;
        LowerYlim = lYlim;
    }

    void SetZAxisLimits(double uZlim, double lZlim) {
        UpperZlim = uZlim;
        LowerZlim = lZlim;
    }

    void SetAxisLimits(double uXlim, double lXlim, double uYlim, double lYlim, double uZlim, double lZlim) {
        UpperZlim = uZlim;
        LowerZlim = lZlim;
    }

    void SetNormalizeHistogram(bool nHistogram) { NormalizeHistogram = nHistogram; }

    void SetCustomNormalization(bool cNormalization) { CustomNormalization = cNormalization; }

    void SetCustomNormalizationFactor(bool cnFactor) { CustomNormalizationFactor = cnFactor; }

    void SetHistogram1DTitleReactions(string h1drReactions) { Histogram1DTitleReactions = h1drReactions; }

    void SetTitleSize(double tSize) { TitleSize = tSize; }

    void SetLabelSizex(double lSizex) { LabelSizex = lSizex; }

    void SetLabelSizey(double lSizey) { LabelSizey = lSizey; }

//    void SetHistogramList(TList *hList) { *HistogramList = hList; }

    void SetLineWidth(int lWidth) { LineWidth = lWidth; }

    void SetLogScalePlot(bool lsPlot) { LogScalePlot = lsPlot; }

    void SetLinearScalePlot(bool lsPlot) { LinearScalePlot = lsPlot; }

//    void SetHistogram1DStack(THStack *h1dStack) { *Histogram1DStack = h1dStack; }

    void SetHistogram1DSaveName(string h1dsName) { Histogram1DSaveName = h1dsName; }

    void SetHistogram1DSaveNamePath(string h1dsNamePath) { Histogram1DSaveNamePath = h1dsNamePath; }

    void SetFinalState(string fState) { FinalState = fState; }

    void SetkColor(int kCol = 1) { kColor = kCol; }

    void SetCenterTitle(bool cTitle = true) { CenterTitle = cTitle; }

    void SetAddToStack(bool atStack = false) { AddToStack = atStack; }

    void SetShowStats(bool sStats = true) { ShowStats = sStats; }

    void SetTitle2(bool T2 = false) { Title2 = T2; }

    void SetApplyPlotCuts(bool apCuts = false) { ApplyPlotCuts = apCuts; }

    void SetPlotCuts(double pCuts = 0) { PlotCuts = pCuts; }

    void SetPlotXmax(double pXmax = 0) { PlotXmax = pXmax; }

    void SetPlotHistogramMax(bool phMax = true) { PlotHistogramMax = phMax; }

//  Get methods:
    TH1D GetHistogram1D() { return Histogram1D; }

    TH2D GetHistogram2D() { return Histogram2D; }

    string GetHistogramTitle() { return HistogramTitle; }

    string GetHistogramStatTitle(string sTitle) { return HistogramStatTitle; }

    string GetXaxisTitle(string xTitle) { return XaxisTitle = xTitle; }

    string GetYaxisTitle(string yTitle) { return YaxisTitle = yTitle; }

    int GetHistogramNumberOfXBins(int NumberOfXBins) { return HistogramNumberOfXBins; }

    int GetHistogramNumberOfYBins(int NumberOfYBins) { return HistogramNumberOfYBins; }

    double GetUpperXlim() { return UpperXlim; }

    double GetLowerXlim() { return LowerXlim; }

    double GetUpperYlim() { return UpperYlim; }

    double GetLowerYlim() { return LowerYlim; }

    double GetUpperZlim() { return UpperZlim; }

    double GetLowerZlim() { return LowerZlim; }

    bool GetNormalizeHistogram() { return NormalizeHistogram; }

    bool GetCustomNormalization() { return CustomNormalization; }

    bool GetCustomNormalizationFactor() { return CustomNormalizationFactor; }

    string GetHistogram1DTitleReactions() { return Histogram1DTitleReactions; }

    double GetTitleSize() { return TitleSize; }

    double GetLabelSizex() { return LabelSizex; }

    double GetLabelSizey() { return LabelSizey; }

//    TList GetHistogramList() { return *HistogramList; }

    int GetLineWidth() { return LineWidth; }

    bool GetLogScalePlot() { return LogScalePlot; }

    bool GetLinearScalePlot() { return LinearScalePlot; }

//    THStack GetHistogram1DStack() { return *Histogram1DStack; }

    string GetHistogram1DSaveName() { return Histogram1DSaveName; }

    string GetHistogram1DSaveNamePath() { return Histogram1DSaveNamePath; }

    string GetFinalState() { return FinalState; }

    int GetkColor() { return kColor; }

    bool GetCenterTitle() { return CenterTitle; }

    bool GetAddToStack() { return AddToStack; }

    bool GetShowStats() { return ShowStats; }

    bool GetTitle2() { return Title2; }

    bool GetApplyPlotCuts() { return ApplyPlotCuts; }

    double GetPlotCuts() { return PlotCuts; }

    double GetPlotXmax() { return PlotXmax; }

    bool GetPlotHistogramMax() { return PlotHistogramMax; }
};


// ======================================================================================================================================================================
// BEnergyToNucleus definition
// ======================================================================================================================================================================

double BEnergyToNucleus;

// ======================================================================================================================================================================
// Simulation parameters extraction
// ======================================================================================================================================================================

int Target_pdg, Probe_pdg;
std::string Target_nucleus, Probe;
double BeamEnergy;

// ======================================================================================================================================================================
// Histogram ranges
// ======================================================================================================================================================================

// Range variables definitions ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Histogram range variables">

//<editor-fold desc="Chi2 plots">
double Chi2_upper_lim, Chi2_lower_lim;
//</editor-fold>

//<editor-fold desc="Vertex plots">
double Vertex_upper_lim, Vertex_lower_lim, dV_upper_lim, dV_lower_lim;
//</editor-fold>

//<editor-fold desc="Theta histograms">

//<editor-fold desc="Theta histograms (2p)">
double theta_lp_upper_lim_2p, theta_lp_lower_lim_2p, theta_p1_upper_lim_2p, theta_p1_lower_lim_2p, theta_p2_upper_lim_2p, theta_p2_lower_lim_2p;
double dtheta_upper_lim_2p, dtheta_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Theta histograms (1n1p)">
double theta_lp_upper_lim_1n1p, theta_lp_lower_lim_1n1p, theta_p_upper_lim_1n1p, theta_p_lower_lim_1n1p, theta_n_upper_lim_1n1p, theta_n_lower_lim_1n1p;
double dtheta_upper_lim_1n1p, dtheta_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Phi histograms">

//<editor-fold desc="Phi histograms (2p)">
double phi_lp_upper_lim_2p, phi_lp_lower_lim_2p, phi_p1_upper_lim_2p, phi_p1_lower_lim_2p, phi_p2_upper_lim_2p, phi_p2_lower_lim_2p;
double dphi_upper_lim_2p, dphi_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Phi histograms (1n1p)">
double phi_lp_upper_lim_1n1p, phi_lp_lower_lim_1n1p, phi_p_upper_lim_1n1p, phi_p_lower_lim_1n1p, phi_n_upper_lim_1n1p, phi_n_lower_lim_1n1p;
double dphi_upper_lim_1n1p, dphi_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy histograms">

//<editor-fold desc="Energy histograms (2p)">
double fsEl_upper_lim_2p, fsEl_lower_lim_2p;
double fsEl_QEL_upper_lim_2p, fsEl_QEL_lower_lim_2p, fsEl_MEC_upper_lim_2p, fsEl_MEC_lower_lim_2p;
double fsEl_RES_upper_lim_2p, fsEl_RES_lower_lim_2p, fsEl_DIS_upper_lim_2p, fsEl_DIS_lower_lim_2p;
double fsEl_VS_theta_lp_upper_lim_2p_x, fsEl_VS_theta_lp_lower_lim_2p_x, fsEl_VS_theta_lp_upper_lim_2p_y, fsEl_VS_theta_lp_lower_lim_2p_y;
//</editor-fold>

//<editor-fold desc="Energy histograms (1n1p)">
double fsEl_upper_lim_1n1p, fsEl_lower_lim_1n1p;
double fsEl_QEL_upper_lim_1n1p, fsEl_QEL_lower_lim_1n1p, fsEl_MEC_upper_lim_1n1p, fsEl_MEC_lower_lim_1n1p;
double fsEl_RES_upper_lim_1n1p, fsEl_RES_lower_lim_1n1p, fsEl_DIS_upper_lim_1n1p, fsEl_DIS_lower_lim_1n1p;
double fsEl_VS_theta_lp_upper_lim_1n1p_x, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_y, fsEl_VS_theta_lp_lower_lim_1n1p_y;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms">

//<editor-fold desc="Energy Transfer histograms (all interactions)">

//<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
double E_Trans_all_ang_all_int_upper_lim_2p, E_Trans_all_ang_all_int_lower_lim_2p;
double E_Trans15_all_upper_lim_2p, E_Trans15_all_lower_lim_2p, E_Trans45_all_upper_lim_2p, E_Trans45_all_lower_lim_2p, E_Trans90_all_upper_lim_2p, E_Trans90_all_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
double E_Trans_all_ang_all_int_upper_lim_1n1p, E_Trans_all_ang_all_int_lower_lim_1n1p;
double E_Trans15_all_upper_lim_1n1p, E_Trans15_all_lower_lim_1n1p, E_Trans45_all_upper_lim_1n1p, E_Trans45_all_lower_lim_1n1p, E_Trans90_all_upper_lim_1n1p, E_Trans90_all_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only)">

//<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
double E_Trans15_QEL_upper_lim_2p, E_Trans15_QEL_lower_lim_2p, E_Trans45_QEL_upper_lim_2p, E_Trans45_QEL_lower_lim_2p, E_Trans90_QEL_upper_lim_2p, E_Trans90_QEL_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
double E_Trans15_QEL_upper_lim_1n1p, E_Trans15_QEL_lower_lim_1n1p, E_Trans45_QEL_upper_lim_1n1p, E_Trans45_QEL_lower_lim_1n1p, E_Trans90_QEL_upper_lim_1n1p, E_Trans90_QEL_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only)">

//<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
double E_Trans15_MEC_upper_lim_2p, E_Trans15_MEC_lower_lim_2p, E_Trans45_MEC_upper_lim_2p, E_Trans45_MEC_lower_lim_2p, E_Trans90_MEC_upper_lim_2p, E_Trans90_MEC_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
double E_Trans15_MEC_upper_lim_1n1p, E_Trans15_MEC_lower_lim_1n1p, E_Trans45_MEC_upper_lim_1n1p, E_Trans45_MEC_lower_lim_1n1p, E_Trans90_MEC_upper_lim_1n1p, E_Trans90_MEC_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only)">

//<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
double E_Trans15_RES_upper_lim_2p, E_Trans15_RES_lower_lim_2p, E_Trans45_RES_upper_lim_2p, E_Trans45_RES_lower_lim_2p, E_Trans90_RES_upper_lim_2p, E_Trans90_RES_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
double E_Trans15_RES_upper_lim_1n1p, E_Trans15_RES_lower_lim_1n1p, E_Trans45_RES_upper_lim_1n1p, E_Trans45_RES_lower_lim_1n1p, E_Trans90_RES_upper_lim_1n1p, E_Trans90_RES_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only)">

//<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
double E_Trans15_DIS_upper_lim_2p, E_Trans15_DIS_lower_lim_2p, E_Trans45_DIS_upper_lim_2p, E_Trans45_DIS_lower_lim_2p, E_Trans90_DIS_upper_lim_2p, E_Trans90_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
double E_Trans15_DIS_upper_lim_1n1p, E_Trans15_DIS_lower_lim_1n1p, E_Trans45_DIS_upper_lim_1n1p, E_Trans45_DIS_lower_lim_1n1p, E_Trans90_DIS_upper_lim_1n1p, E_Trans90_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//</editor-fold>

//<editor-fold desc="Inclusive Energy Transfer histograms">
double E_Trans15_all_inclusive_upper_lim, E_Trans15_all_inclusive_lower_lim;
double E_Trans15_QEL_inclusive_upper_lim, E_Trans15_QEL_inclusive_lower_lim, E_Trans15_MEC_inclusive_upper_lim, E_Trans15_MEC_inclusive_lower_lim;
double E_Trans15_RES_inclusive_upper_lim, E_Trans15_RES_inclusive_lower_lim, E_Trans15_DIS_inclusive_upper_lim, E_Trans15_DIS_inclusive_lower_lim;
double E_Trans_VS_q_all_inclusive_upper_lim_x, E_Trans_VS_q_all_inclusive_lower_lim_x, E_Trans_VS_q_all_inclusive_upper_lim_y, E_Trans_VS_q_all_inclusive_lower_lim_y;
double E_Trans_VS_q_QEL_inclusive_upper_lim_x, E_Trans_VS_q_QEL_inclusive_lower_lim_x, E_Trans_VS_q_QEL_inclusive_upper_lim_y, E_Trans_VS_q_QEL_inclusive_lower_lim_y;
double E_Trans_VS_q_MEC_inclusive_upper_lim_x, E_Trans_VS_q_MEC_inclusive_lower_lim_x, E_Trans_VS_q_MEC_inclusive_upper_lim_y, E_Trans_VS_q_MEC_inclusive_lower_lim_y;

double E_Trans_VS_q3_all_upper_lim_x_2p, E_Trans_VS_q3_all_lower_lim_x_2p, E_Trans_VS_q3_all_upper_lim_y_2p, E_Trans_VS_q3_all_lower_lim_y_2p;
double E_Trans_VS_q3_QEL_upper_lim_x_2p, E_Trans_VS_q3_QEL_lower_lim_x_2p, E_Trans_VS_q3_QEL_upper_lim_y_2p, E_Trans_VS_q3_QEL_lower_lim_y_2p;
double E_Trans_VS_q3_MEC_upper_lim_x_2p, E_Trans_VS_q3_MEC_lower_lim_x_2p, E_Trans_VS_q3_MEC_upper_lim_y_2p, E_Trans_VS_q3_MEC_lower_lim_y_2p;

double E_Trans_VS_q3_all_upper_lim_x_1n1p, E_Trans_VS_q3_all_lower_lim_x_1n1p, E_Trans_VS_q3_all_upper_lim_y_1n1p, E_Trans_VS_q3_all_lower_lim_y_1n1p;
double E_Trans_VS_q3_QEL_upper_lim_x_1n1p, E_Trans_VS_q3_QEL_lower_lim_x_1n1p, E_Trans_VS_q3_QEL_upper_lim_y_1n1p, E_Trans_VS_q3_QEL_lower_lim_y_1n1p;
double E_Trans_VS_q3_MEC_upper_lim_x_1n1p, E_Trans_VS_q3_MEC_lower_lim_x_1n1p, E_Trans_VS_q3_MEC_upper_lim_y_1n1p, E_Trans_VS_q3_MEC_lower_lim_y_1n1p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms">

//<editor-fold desc="E_cal range">
double E_cal_QEL_upper_lim_range, E_cal_QEL_lower_lim_range, E_cal_MEC_upper_lim_range, E_cal_MEC_lower_lim_range;
double E_cal_RES_upper_lim_range, E_cal_RES_lower_lim_range, E_cal_DIS_upper_lim_range, E_cal_DIS_lower_lim_range;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (2p)">
double E_cal_QEL_upper_lim_2p, E_cal_QEL_lower_lim_2p, E_cal_MEC_upper_lim_2p, E_cal_MEC_lower_lim_2p;
double E_cal_RES_upper_lim_2p, E_cal_RES_lower_lim_2p, E_cal_DIS_upper_lim_2p, E_cal_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (1n1p)">
double E_cal_QEL_upper_lim_1n1p, E_cal_QEL_lower_lim_1n1p, E_cal_MEC_upper_lim_1n1p, E_cal_MEC_lower_lim_1n1p;
double E_cal_RES_upper_lim_1n1p, E_cal_RES_lower_lim_1n1p, E_cal_DIS_upper_lim_1n1p, E_cal_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Momentum histograms">

//<editor-fold desc="Momentum histograms (2p)">
double P_L_hist_upper_lim_2p, P_L_hist_lower_lim_2p, P_R_hist_upper_lim_2p, P_R_hist_lower_lim_2p, P_lp_hist_upper_lim_2p, P_lp_hist_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Momentum histograms (1n1p)">
double P_p_hist_upper_lim_1n1p, P_p_hist_lower_lim_1n1p, P_n_hist_upper_lim_1n1p, P_n_hist_lower_lim_1n1p, P_lp_hist_upper_lim_1n1p, P_lp_hist_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="MicroBooNE plots">

//<editor-fold desc="MicroBooNE gamma plots (unweighted)">
double gamma_Lab_hist_upper_lim_2p, gamma_Lab_hist_lower_lim_2p, gamma_mu_p_tot_upper_lim_2p, gamma_mu_p_tot_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (Q4 weighted)">
double gamma_Lab_hist_weighted_upper_lim_1n1p, gamma_Lab_hist_weighted_lower_lim_1n1p, gamma_mu_p_tot_weighted_upper_lim_1n1p, gamma_mu_p_tot_weighted_lower_lim_1n1p;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (no pions, for every interaction)">
double gamma_Lab_all_hist_upper_lim, gamma_Lab_all_hist_lower_lim, gamma_Lab_all_hist_weighted_upper_lim, gamma_Lab_all_hist_weighted_lower_lim;
double gamma_Lab_QEL_hist_upper_lim, gamma_Lab_QEL_hist_lower_lim, gamma_Lab_QEL_hist_weighted_upper_lim, gamma_Lab_QEL_hist_weighted_lower_lim;
double gamma_Lab_MEC_hist_upper_lim, gamma_Lab_MEC_hist_lower_lim, gamma_Lab_MEC_hist_weighted_upper_lim, gamma_Lab_MEC_hist_weighted_lower_lim;
double gamma_Lab_RES_hist_upper_lim, gamma_Lab_RES_hist_lower_lim, gamma_Lab_RES_hist_weighted_upper_lim, gamma_Lab_RES_hist_weighted_lower_lim;
double gamma_Lab_DIS_hist_upper_lim, gamma_Lab_DIS_hist_lower_lim, gamma_Lab_DIS_hist_weighted_upper_lim, gamma_Lab_DIS_hist_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE dP_T plots">
double dP_T_hist_upper_lim, dP_T_hist_lower_lim, dP_T_hist_weighted_upper_lim, dP_T_hist_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
double P_L_hist_upper_lim, P_L_hist_lower_lim, P_R_hist_upper_lim, P_R_hist_lower_lim, P_lp_hist_upper_lim, P_lp_hist_lower_lim, P_pion_hist_upper_lim, P_pion_hist_lower_lim;
//</editor-fold>

//</editor-fold>

//</editor-fold>

#endif //ANALYSER_CODESETUP_H
