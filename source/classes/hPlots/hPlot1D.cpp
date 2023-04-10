//
// Created by alons on 06/04/2023.
//

#include "hPlot1D.h"
#include "../../functions/GeneralFunctions.h"

bool hPlot1D::findSubstring(std::string string1, std::string string2) {
    if (string1.find(string2) != string::npos) {
        return true;
    } else {
        return false;
    }
}

// Constructor definition -----------------------------------------------------------------------------------------------------------------------------------------------

hPlot1D::hPlot1D(std::string h1DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim,
                 int hnob = 100) {
    Histogram1DTitles["Histogram1DTitleReactions"] = h1DtReactions, Histogram1DTitles["FinalState"] = fState, Histogram1DTitles["DetectorRegion"] = dRegion;
    Histogram1DTitles["HistogramStatTitle"] = hst, Histogram1DTitles["HistogramTitle"] = ht, Histogram1DTitles["XaxisTitle"] = xat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramNumberOfXBins = 100;

    Histogram1D = new TH1D(
            (Histogram1DTitles["HistogramStatTitle"] + " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ", " +
             Histogram1DTitles["DetectorRegion"] + ")").c_str(),
            (Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ", " +
             Histogram1DTitles["DetectorRegion"] + ")" + ";" + Histogram1DTitles["XaxisTitle"] + ";").c_str(), HistogramNumberOfXBins, HistogramXAxisLimits.at(0),
            HistogramXAxisLimits.at(1));
}

hPlot1D::hPlot1D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob = 100) {
    Histogram1DTitles["FinalState"] = fState, Histogram1DTitles["DetectorRegion"] = dRegion;
    Histogram1DTitles["HistogramStatTitle"] = hst, Histogram1DTitles["HistogramTitle"] = ht, Histogram1DTitles["XaxisTitle"] = xat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramNumberOfXBins = 100;

    Histogram1D = new TH1D((Histogram1DTitles["HistogramStatTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")").c_str(),
                           (Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")" + ";" +
                            Histogram1DTitles["XaxisTitle"] + ";").c_str(), HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1));
}

hPlot1D::hPlot1D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string sPath, std::string sName, double LowerXlim,
                 double UpperXlim, int hnob = 100) {
    Histogram1DTitles["FinalState"] = fState, Histogram1DTitles["DetectorRegion"] = dRegion;
    Histogram1DTitles["HistogramStatTitle"] = hst, Histogram1DTitles["HistogramTitle"] = ht, Histogram1DTitles["XaxisTitle"] = xat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramNumberOfXBins = 100;
    Histogram1DSaveNamePath = sPath;
    Histogram1DSaveName = sName;

    string StatTitle;

    if (Histogram1DTitles["DetectorRegion"] == "") {
        StatTitle = Histogram1DTitles["HistogramStatTitle"] + " (" + Histogram1DTitles["FinalState"] + ")";
    } else {
        StatTitle = Histogram1DTitles["HistogramStatTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")";
    }

    Histogram1D = new TH1D(StatTitle.c_str(),
                           (Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")" + ";" +
                            Histogram1DTitles["XaxisTitle"] + ";").c_str(), HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1));
}

hPlot1D::hPlot1D(std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob) {
    Histogram1DTitles["HistogramStatTitle"] = hst, Histogram1DTitles["HistogramTitle"] = ht, Histogram1DTitles["XaxisTitle"] = xat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramNumberOfXBins = 100;

    Histogram1D = new TH1D((Histogram1DTitles["HistogramStatTitle"]).c_str(), (Histogram1DTitles["HistogramTitle"] + ";" + Histogram1DTitles["XaxisTitle"] + ";").c_str(),
                           HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1));
}

// histPlotter1D function (old) -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (old)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            std::string Histogram1DTitle, std::string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey,
                            TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName,
                            std::string Histogram1DSaveNamePath, std::string finalState, int kColor = 1, bool centerTitle = true, bool addToStack = false,
                            bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true) {

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
        string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

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
        std::string title;

        if (title2 == false) {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
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

    Histogram1D->GetXaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
    Histogram1D->GetXaxis()->SetLabelSize(Histogram1DTitleSizes.at(1));
    Histogram1D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram1D->GetYaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
    Histogram1D->GetYaxis()->SetLabelSize(Histogram1DTitleSizes.at(2));
    Histogram1D->GetYaxis()->CenterTitle(CenterTitle);
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
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(LineColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

//<editor-fold desc="histPlotter1D function (old, stackless)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            string Histogram1DTitle, string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                            int lineWidth, bool logScalePlot, bool linearScalePlot, string Histogram1DSaveName, const string &Histogram1DSaveNamePath, string finalState,
                            bool centerTitle = true, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0,
                            bool plot_max = true) {

    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.16, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

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
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
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

    if (showStats == false) { Histogram1D->SetStats(0); }

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
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut, 3)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut, 3)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax, 3)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    Histogram1DCanvas->Clear();

}
//</editor-fold>

// histPlotter1D function (unsymmetric cuts for SF plots) ---------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (unsymmetric cuts for SF plots)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot,
                            bool linearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, std::string finalState,
                            int kColor = 1, bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_upper_cut = 0,
                            double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true) {

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
        std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle0 = title.c_str();
        Histogram1D->SetTitle(HistogramTitle0);
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
        std::string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }
//        std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle0 = title.c_str();
        Histogram1D->SetTitle(HistogramTitle0);
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

    Histogram1D->GetXaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
//    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(Histogram1DTitleSizes.at(1));
//    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram1D->GetYaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
//    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(Histogram1DTitleSizes.at(2));
//    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(CenterTitle);
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
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(LineColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

//<editor-fold desc="histPlotter1D function (unsymmetric cuts for SF plots, stackless)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                            string Histogram1DSaveName, const string &Histogram1DSaveNamePath, string finalState, bool showStats = true, bool title2 = false,
                            bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true) {

    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.16, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

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
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
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

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) { Histogram1D->SetStats(0); }

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

            if (plot_lower_cut != -1) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);

            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir;

        if (finalState == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        }

        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

// histPlotter1D function (one-sided cuts for Nphe plots) ---------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (one-sided cuts for Nphe plots)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot,
                            bool linearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, std::string finalState,
                            int kColor = 1, bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cut = 0,
                            double plot_Xmax = 0, bool plot_max = true) {

//  Normalization factor:
    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle0 = title.c_str();
        Histogram1D->SetTitle(HistogramTitle0);
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
        std::string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }
//        std::string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle0 = title.c_str();
        Histogram1D->SetTitle(HistogramTitle0);
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

    Histogram1D->GetXaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
//    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(Histogram1DTitleSizes.at(1));
//    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram1D->GetYaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
//    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(Histogram1DTitleSizes.at(2));
//    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(CenterTitle);
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
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(LineColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

//<editor-fold desc="histPlotter1D function (one-sided cuts for Nphe plots, stackless)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                            string Histogram1DSaveName, const string &Histogram1DSaveNamePath, string finalState, bool showStats = true, bool title2 = false,
                            bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0, bool plot_max = true) {

    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.16, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title;

        if (finalState == "") {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
        } else {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        }

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
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
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

    Histogram1D->GetXaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
    Histogram1D->GetXaxis()->SetLabelSize(Histogram1DTitleSizes.at(1));
    Histogram1D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram1D->GetYaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
    Histogram1D->GetYaxis()->SetLabelSize(Histogram1DTitleSizes.at(2));
    Histogram1D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) { Histogram1D->SetStats(0); }

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
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(plot_cut, ("Lower cut = " + to_string_with_precision(Cut, 0)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        if (finalState == "") {
            string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale.png";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            Histogram1DCanvas->SaveAs(SaveDir);
        } else {
            string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            Histogram1DCanvas->SaveAs(SaveDir);
        }
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        if (finalState == "") {
            string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale.png";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            Histogram1DCanvas->SaveAs(SaveDir);
        } else {
            string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
            const char *SaveDir = Histogram1DSaveNameDir.c_str();
            Histogram1DCanvas->SaveAs(SaveDir);
        }
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>


// histPlotter1D function (unified) -------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (unified)">
void hPlot1D::histPlotter1D(std::string &SampleName, TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                            double custom_normalization_factor, TList *Histogram_list, THStack *Histogram1DStack, double plot_lower_cut = -9999,
                            double plot_upper_cut = 9999, double plot_Xmax = 0, bool plot_max = true) {

    std::string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

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
        std::string title;

        if (Histogram1DTitles["Histogram1DTitleReactions"] == "" && Histogram1DTitles["DetectorRegion"] == "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ")" + " - Normalized";
        } else if (Histogram1DTitles["Histogram1DTitleReactions"] != "" && Histogram1DTitles["DetectorRegion"] == "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ")" +
                    " - Normalized";
        } else if (Histogram1DTitles["Histogram1DTitleReactions"] == "" && Histogram1DTitles["DetectorRegion"] != "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")" + " - Normalized";
        } else {
            title = Histogram1DTitles["HistogramTitle"] +
                    " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")" +
                    " - Normalized";
        }
//        std::string title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ")" +
//                            " - Normalized";
        const char *HistogramTitle0 = title.c_str();
        Histogram1D->SetTitle(HistogramTitle0);
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
        std::string title;

        if (Histogram1DTitles["Histogram1DTitleReactions"] == "" && Histogram1DTitles["DetectorRegion"] == "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ")";
        } else if (Histogram1DTitles["Histogram1DTitleReactions"] != "" && Histogram1DTitles["DetectorRegion"] == "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ")";
        } else if (Histogram1DTitles["Histogram1DTitleReactions"] == "" && Histogram1DTitles["DetectorRegion"] != "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")";
        } else {
            title = Histogram1DTitles["HistogramTitle"] +
                    " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")";
        }

        const char *HistogramTitle0 = title.c_str();
        Histogram1D->SetTitle(HistogramTitle0);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
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

    Histogram1D->GetXaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
    Histogram1D->GetXaxis()->SetLabelSize(Histogram1DTitleSizes.at(1));
    Histogram1D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram1D->GetYaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
    Histogram1D->GetYaxis()->SetLabelSize(Histogram1DTitleSizes.at(2));
    Histogram1D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram1D->SetLineWidth(LineWidth);
    Histogram_list->Add(Histogram1D);

    if (ShowStats == false) { Histogram1D->SetStats(0); }

    if (ApplyPlotCuts == true) {
        gPad->Update();
        double Upper_cut = plot_upper_cut;
        double Lower_cut = plot_lower_cut;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(LineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(LineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(LineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            if (fabs(plot_upper_cut) != 9999) { upper_cut->Draw("same"); }
            upper_cut->SetLineColor(kBlue);

            if (fabs(plot_lower_cut) != 9999) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);

            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir;

        if (Histogram1DTitles["DetectorRegion"] == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_log_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_log_scale_" + Histogram1DTitles["DetectorRegion"] + ".png";
        }
//        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (LinearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir;

        if (Histogram1DTitles["DetectorRegion"] == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_linear_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitles["DetectorRegion"] + ".png";
        }
//        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (AddToStack == true) {
        Histogram1D->SetLineColor(LineColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

//<editor-fold desc="histPlotter1D function (unified, stackless)">
void hPlot1D::histPlotter1D(std::string &SampleName, TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                            double custom_normalization_factor, TList *Histogram_list, double plot_lower_cut = -9999, double plot_upper_cut = 9999, double plot_Xmax = 0,
                            bool plot_max = true) {

    std::string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

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
        std::string title;

        if (Histogram1DTitles["Histogram1DTitleReactions"] == "" && Histogram1DTitles["DetectorRegion"] == "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ")" + " - Normalized";
        } else if (Histogram1DTitles["Histogram1DTitleReactions"] != "" && Histogram1DTitles["DetectorRegion"] == "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ")" +
                    " - Normalized";
        } else if (Histogram1DTitles["Histogram1DTitleReactions"] == "" && Histogram1DTitles["DetectorRegion"] != "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")" + " - Normalized";
        } else {
            title = Histogram1DTitles["HistogramTitle"] +
                    " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")" +
                    " - Normalized";
        }
//        std::string title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ")" +
//                            " - Normalized";
        const char *HistogramTitle0 = title.c_str();
        Histogram1D->SetTitle(HistogramTitle0);
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
        std::string title;

        if (Histogram1DTitles["Histogram1DTitleReactions"] == "" && Histogram1DTitles["DetectorRegion"] == "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ")";
        } else if (Histogram1DTitles["Histogram1DTitleReactions"] != "" && Histogram1DTitles["DetectorRegion"] == "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ")";
        } else if (Histogram1DTitles["Histogram1DTitleReactions"] == "" && Histogram1DTitles["DetectorRegion"] != "") {
            title = Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")";
        } else {
            title = Histogram1DTitles["HistogramTitle"] +
                    " (" + Histogram1DTitles["Histogram1DTitleReactions"] + ", " + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")";
        }

        const char *HistogramTitle0 = title.c_str();
        Histogram1D->SetTitle(HistogramTitle0);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
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

    Histogram1D->GetXaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
    Histogram1D->GetXaxis()->SetLabelSize(Histogram1DTitleSizes.at(1));
    Histogram1D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram1D->GetYaxis()->SetTitleSize(Histogram1DTitleSizes.at(0));
    Histogram1D->GetYaxis()->SetLabelSize(Histogram1DTitleSizes.at(2));
    Histogram1D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram1D->SetLineWidth(LineWidth);
    Histogram_list->Add(Histogram1D);

    if (ShowStats == false) { Histogram1D->SetStats(0); }

    if (ApplyPlotCuts == true) {
        gPad->Update();
        double Upper_cut = plot_upper_cut;
        double Lower_cut = plot_lower_cut;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(LineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(LineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(LineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            if (fabs(plot_upper_cut) != 9999) { upper_cut->Draw("same"); }
            upper_cut->SetLineColor(kBlue);

            if (fabs(plot_lower_cut) != 9999) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);

            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (LogScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir;

        if (Histogram1DTitles["DetectorRegion"] == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_log_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_log_scale_" + Histogram1DTitles["DetectorRegion"] + ".png";
        }
//        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (LinearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        std::string Histogram1DSaveNameDir;

        if (Histogram1DTitles["DetectorRegion"] == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_linear_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitles["DetectorRegion"] + ".png";
        }
//        std::string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

// hDrawAndSave function ------------------------------------------------------------------------------------------------------------------------------------------------

void hPlot1D::hDrawAndSave(std::string &SampleName, TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor,
                           double plot_lower_cut = -9999, double plot_upper_cut = 9999, double plot_Xmax = 0, bool plotMax = false) {
    histPlotter1D(SampleName, h1DCanvas, Histogram1D, normHistogram, cNormalization, cNormalizationFactor, hList, plot_lower_cut, plot_upper_cut, plot_Xmax, plotMax);

//    histPlotter1D(h1DCanvas, Histogram1D, normHistogram, cNormalization, cNormalizationFactor, Histogram1DTitles["HistogramTitle"],
//                  Histogram1DTitles["Histogram1DTitleReactions"], hList, LineWidth, LogScalePlot, LinearScalePlot, Histogram1DSaveName, Histogram1DSaveNamePath,
//                  Histogram1DTitles["FinalState"], ShowStats, Title2, ApplyPlotCuts, sigma, mean, plotMax);
//
//    histPlotter1D(h1DCanvas, Histogram1D, normHistogram, cNormalization, cNormalizationFactor, Histogram1DTitles["HistogramTitle"],
//                  Histogram1DTitles["Histogram1DTitleReactions"], Histogram1DTitleSizes.at(0), Histogram1DTitleSizes.at(1), Histogram1DTitleSizes.at(2), hList, LineWidth,
//                  LogScalePlot, LinearScalePlot, Histogram1DSaveName, Histogram1DSaveNamePath, Histogram1DTitles["FinalState"], CenterTitle, ShowStats, Title2,
//                  ApplyPlotCuts, sigma, mean, plotMax);
}

void hPlot1D::hDrawAndSave(TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor) {
    histPlotter1D(h1DCanvas, Histogram1D, normHistogram, cNormalization, cNormalizationFactor, Histogram1DTitles["HistogramTitle"],
                  Histogram1DTitles["Histogram1DTitleReactions"], Histogram1DTitleSizes.at(0), Histogram1DTitleSizes.at(1), Histogram1DTitleSizes.at(2), hList, LineWidth,
                  LogScalePlot, LinearScalePlot, Histogram1DSaveName, Histogram1DSaveNamePath, Histogram1DTitles["FinalState"], CenterTitle, ShowStats, Title2,
                  ApplyPlotCuts, PlotCuts, PlotXmax, PlotHistogramMax);
}
//void hPlot1D::hDrawAndSave(TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor, std::string FinalState) {
//    histPlotter1D(h1DCanvas, Histogram1D, normHistogram, cNormalization, cNormalizationFactor, Histogram1DTitles["HistogramTitle"],
//                  Histogram1DTitles["Histogram1DTitleReactions"], Histogram1DTitleSizes.at(0), Histogram1DTitleSizes.at(1), Histogram1DTitleSizes.at(2), hList,
//                  LineWidth, LogScalePlot, LinearScalePlot, Histogram1DSaveName, Histogram1DSaveNamePath, FinalState, CenterTitle, ShowStats, Title2, ApplyPlotCuts,
//                  PlotCuts, PlotXmax, PlotHistogramMax);
//}
