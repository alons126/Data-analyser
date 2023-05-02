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
    HistogramStatsTitle = hst;
    Histogram1DTitles["HistogramStatTitle"] = hst;
    HistogramTitle = ht;
    Histogram1DTitles["HistogramTitle"] = ht;
    Histogram1DTitles["Histogram1DTitleReactions"] = h1DtReactions, Histogram1DTitles["FinalState"] = fState, Histogram1DTitles["DetectorRegion"] = dRegion;
    Histogram1DTitles["XaxisTitle"] = xat;
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
    HistogramStatsTitle = hst;
    Histogram1DTitles["HistogramStatTitle"] = hst;
    HistogramTitle = ht;
    Histogram1DTitles["HistogramTitle"] = ht;
    Histogram1DTitles["FinalState"] = fState, Histogram1DTitles["DetectorRegion"] = dRegion, Histogram1DTitles["XaxisTitle"] = xat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramNumberOfXBins = 100;

    Histogram1D = new TH1D((Histogram1DTitles["HistogramStatTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")").c_str(),
                           (Histogram1DTitles["HistogramTitle"] + " (" + Histogram1DTitles["FinalState"] + ", " + Histogram1DTitles["DetectorRegion"] + ")" + ";" +
                            Histogram1DTitles["XaxisTitle"] + ";").c_str(), HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1));
}

hPlot1D::hPlot1D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string sPath, std::string sName, double LowerXlim,
                 double UpperXlim, int hnob = 100) {
    HistogramStatsTitle = hst;
    Histogram1DTitles["HistogramStatTitle"] = hst;
    HistogramTitle = ht;
    Histogram1DTitles["HistogramTitle"] = ht;
    Histogram1DTitles["FinalState"] = fState, Histogram1DTitles["DetectorRegion"] = dRegion;
    Histogram1DTitles["XaxisTitle"] = xat;
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
    HistogramStatsTitle = hst;
    Histogram1DTitles["HistogramStatTitle"] = hst;
    HistogramTitle = ht;
    Histogram1DTitles["HistogramTitle"] = ht;
    Histogram1DTitles["XaxisTitle"] = xat;
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
//    double x_1 = 0.16, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
        TLine *upper_cut = new TLine((Upper_cut + plot_xmax), 0., (Upper_cut + plot_xmax), gPad->GetFrame()->GetY2());
//        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(LineWidth);
        TLine *lower_cut = new TLine((Lower_cut + plot_xmax), 0., (Lower_cut + plot_xmax), gPad->GetFrame()->GetY2());
//        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
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

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut + plot_xmax, 3)).c_str(), "l");
//            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut + plot_xmax, 3)).c_str(), "l");
//            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax, 3)).c_str(), "l");
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
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
//    double x_1 = 0.16, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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
        TLine *upper_cut = new TLine((Upper_cut + plot_xmax), 0., (Upper_cut + plot_xmax), gPad->GetFrame()->GetY2());
//        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(LineWidth);
        TLine *lower_cut = new TLine((Lower_cut + plot_xmax), 0., (Lower_cut + plot_xmax), gPad->GetFrame()->GetY2());
//        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
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

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut + plot_xmax, 3)).c_str(), "l");
//            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut + plot_xmax, 3)).c_str(), "l");
//            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax, 3)).c_str(), "l");
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

// histPlotter1DwFit function (unified) -------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1DwFit function">
void hPlot1D::histPlotter1DwFit(std::string SampleName, TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                                double custom_normalization_factor, string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list,
                                string Histogram1DSaveName, const string &Histogram1DSaveNamePath, string finalState, double &plot_Xmax, double &plot_lcut,
                                double &plot_ucut, double factor, bool plot_max = true, string particle = "") {

    std::string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
//    double x_1 = 0.16, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

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

//        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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

        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
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

    if (particle != "") { cout << "\n\nFit results for " << particle << " in " << finalState << " are (" << Histogram1DTitleReactions << "):\n\n"; }

    if (ApplyPlotCuts == true) {
        double Amp, Mean, Std;

        if (Histogram1D->Integral() != 0.) { // don't fit if histogram is empty (leads to an error!)
            /* A fit to a gaussian with 3 parameters: f(x) = p0*exp(-0.5((x-p1)/p2)^2)). */
            cout << "\n\n";
            Histogram1D->Fit("gaus");
            cout << "\n\n";

            /* Get fitted function to TF1 plot */
            TF1 *fit = Histogram1D->GetFunction("gaus");
            fit->SetLineColor(kMagenta);

            /* Set fitted plot parameters from TF1 plot */
            Amp = fit->GetParameter(0); // get p0
            Mean = fit->GetParameter(1); // get p1
            Std = fit->GetParameter(2); // get p2

            /* Set cut parameters from TF1 plot */
            //todo: set these as 9999 if histogram is empty?
            plot_Xmax = Mean;
            plot_ucut = Std * factor; // Cut up to 2 Std from mean
            plot_lcut = -(Std * factor); // Cut up to 2 Std from mean

            /* Remove TF1 plot from histogram */
            Histogram1D->GetListOfFunctions()->Remove(Histogram1D->GetFunction("gaus"));

            /* Draw TF1 plot as curved function */
            fit->SetLineColor(kMagenta);
            fit->Draw("same && C");
        }

        gPad->Update();

        double Upper_cut = plot_ucut + plot_Xmax, Lower_cut = plot_lcut + plot_Xmax, plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(LineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(LineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(LineWidth + 1);

        double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
        double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
        auto Cut_legend = new TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);

        if (Histogram1D->Integral() != 0.) {
            if (fabs(plot_ucut) != 9999) { upper_cut->Draw("same"); }
            upper_cut->SetLineColor(kBlue);

            if (fabs(plot_lcut) != 9999) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);

            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("UC = " + to_string_with_precision(Upper_cut, 3)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("LC = " + to_string_with_precision(Lower_cut, 3)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Mean = " + to_string_with_precision(plot_xmax, 3)).c_str(), "l");
            }

            Cut_legend->Draw("same");

            double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
            double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.245;

            TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
            FitParam->SetBorderSize(1);
            FitParam->SetTextFont(0);
            FitParam->SetFillColor(0);
//            FitParam->SetTextAlign(11);
//            FitParam->SetTextAlign(22);
            FitParam->SetTextAlign(12);
            FitParam->AddText(("Fit amp = " + to_string_with_precision(Amp, 8)).c_str());
            FitParam->AddText(("Fit std = " + to_string_with_precision(Std, 8)).c_str());
            FitParam->AddText(("Fit mean = " + to_string_with_precision(Mean, 8)).c_str());
            ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
            FitParam->AddText(("Cuts = std * " + to_string_with_precision(factor, 2) + " = " + to_string_with_precision(plot_ucut, 3)).c_str());
            ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
            FitParam->Draw("same");
        }
    }

    if (particle != "") { cout << "\n"; }

    if (LogScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        std::string Histogram1DSaveNameDir;

        if (Histogram1DTitles["DetectorRegion"] == "") {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_log_scale.png";
        } else {
            Histogram1DSaveNameDir = Histogram1DSaveNamePath + sNameFlag + Histogram1DSaveName + "_log_scale_" + Histogram1DTitles["DetectorRegion"] + ".png";
        }
//        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + FinalState + ".png";
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
//        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + FinalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

// hDrawAndSave function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hDrawAndSave function">
void hPlot1D::hDrawAndSave(std::string &SampleName, TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor,
                           double plot_lower_cut = -9999, double plot_upper_cut = 9999, double plot_Xmax = 0, bool plotMax = false) {
    histPlotter1D(SampleName, h1DCanvas, Histogram1D, normHistogram, cNormalization, cNormalizationFactor, hList, plot_lower_cut, plot_upper_cut, plot_Xmax, plotMax);
}

void hPlot1D::hDrawAndSave(TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor) {
    histPlotter1D(h1DCanvas, Histogram1D, normHistogram, cNormalization, cNormalizationFactor, Histogram1DTitles["HistogramTitle"],
                  Histogram1DTitles["Histogram1DTitleReactions"], Histogram1DTitleSizes.at(0), Histogram1DTitleSizes.at(1), Histogram1DTitleSizes.at(2), hList, LineWidth,
                  LogScalePlot, LinearScalePlot, Histogram1DSaveName, Histogram1DSaveNamePath, Histogram1DTitles["FinalState"], CenterTitle, ShowStats, Title2,
                  ApplyPlotCuts, PlotCuts, PlotXmax, PlotHistogramMax);
}
//</editor-fold>

// hDrawAndSaveWFit function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hDrawAndSaveWFit function">
void hPlot1D::hDrawAndSaveWFit(std::string &SampleName, TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor,
                               double factor, double &plot_lower_cut, double &plot_upper_cut, double &plot_Xmax, bool plotMax = false) {
    histPlotter1DwFit(SampleName, h1DCanvas, Histogram1D, normHistogram, cNormalization, cNormalizationFactor, Histogram1DTitles["HistogramTitle"],
                      Histogram1DTitles["Histogram1DTitleReactions"], hList, Histogram1DSaveName, Histogram1DSaveNamePath, Histogram1DTitles["FinalState"], plot_Xmax,
                      plot_lower_cut, plot_upper_cut, factor, plotMax);
}
//</editor-fold>

// FitFunction function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="FitFunction function">
Double_t hPlot1D::FitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
    if (par[2] != 0) arg = (v[0] - par[1]) / par[2];

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}
//</editor-fold>

// hLogEventCuts function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hLogEventCuts function">
void hPlot1D::hLogEventCuts(clas12::region_part_ptr Particle, double Lower_cut, double Upper_cut, double CutCenter = 0, double Weight = 1) {
    TVector3 P;
    P.SetMagThetaPhi(Particle->getP(), Particle->getTheta(), Particle->getPhi());

    if ((fabs(Upper_cut) == 9999) && (fabs(Lower_cut) == 9999)) {
        Histogram1D->Fill(P.Mag(), Weight);
    } else if ((fabs(Upper_cut) != 9999) && (fabs(Lower_cut) == 9999)) {
        if (P.Mag() <= Upper_cut) { Histogram1D->Fill(P.Mag(), Weight); }
    } else if ((fabs(Upper_cut) == 9999) && (fabs(Lower_cut) != 9999)) {
        if (P.Mag() >= Lower_cut) { Histogram1D->Fill(P.Mag(), Weight); }
    } else if ((fabs(Upper_cut) != 9999) && (fabs(Lower_cut) != 9999)) {
        if ((P.Mag() >= Lower_cut) && (P.Mag() <= Upper_cut)) { Histogram1D->Fill(P.Mag(), Weight); }
    }
}
//</editor-fold>