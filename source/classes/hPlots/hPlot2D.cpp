//
// Created by alons on 07/04/2023.
//

#include "hPlot2D.h"

// Constructor definition -----------------------------------------------------------------------------------------------------------------------------------------------

hPlot2D::hPlot2D(std::string h2DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat,
                 double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 250, int hnoYb = 250) {
    Histogram2DTitles["Histogram2DTitleReactions"] = h2DtReactions, Histogram2DTitles["FinalState"] = fState, Histogram2DTitles["DetectorRegion"] = dRegion;
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;

    Histogram2D = new TH2D(
            (Histogram2DTitles["HistogramStatTitle"] + " (" + Histogram2DTitles["Histogram2DTitleReactions"] + ", " + Histogram2DTitles["FinalState"] + ", " +
             Histogram2DTitles["DetectorRegion"] + ")").c_str(),
            (Histogram2DTitles["HistogramTitle"] + " (" + Histogram2DTitles["Histogram2DTitleReactions"] + ", " + Histogram2DTitles["FinalState"] + ", " +
             Histogram2DTitles["DetectorRegion"] + ")" + ";" + Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"]).c_str(),
            HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1),
            HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
}

hPlot2D::hPlot2D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat,
                 double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 250, int hnoYb = 250) {
    Histogram2DTitles["FinalState"] = fState, Histogram2DTitles["DetectorRegion"] = dRegion;
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;

    Histogram2D = new TH2D(
            (Histogram2DTitles["HistogramStatTitle"] + " (" + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"] + ")").c_str(),
            (Histogram2DTitles["HistogramTitle"] + " (" + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"] + ")" + ";" +
             Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"]).c_str(),
            HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1),
            HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
}

hPlot2D::hPlot2D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat, std::string sPath, std::string sName,
                 double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 250, int hnoYb = 250) {
    Histogram2DTitles["FinalState"] = fState, Histogram2DTitles["DetectorRegion"] = dRegion;
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;
    Histogram2DSaveNamePath = sPath;
    Histogram2DSaveName = sName;

    Histogram2D = new TH2D(
            (Histogram2DTitles["HistogramStatTitle"] + " (" + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"] + ")").c_str(),
            (Histogram2DTitles["HistogramTitle"] + " (" + Histogram2DTitles["FinalState"] + ", " + Histogram2DTitles["DetectorRegion"] + ")" + ";" +
             Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"]).c_str(),
            HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1),
            HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
}

hPlot2D::hPlot2D(std::string hst, std::string ht, std::string xat, std::string yat,
                 double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 250, int hnoYb = 250) {
    Histogram2DTitles["HistogramStatTitle"] = hst, Histogram2DTitles["HistogramTitle"] = ht, Histogram2DTitles["XaxisTitle"] = xat, Histogram2DTitles["YaxisTitle"] = yat;
    HistogramXAxisLimits.push_back(LowerXlim), HistogramXAxisLimits.push_back(UpperXlim);
    HistogramYAxisLimits.push_back(LowerYlim), HistogramYAxisLimits.push_back(UpperYlim);
    HistogramNumberOfXBins = hnoXb, HistogramNumberOfYBins = hnoYb;

    Histogram2D = new TH2D(
            (Histogram2DTitles["HistogramStatTitle"]).c_str(),
            (Histogram2DTitles["HistogramTitle"] + ";" + Histogram2DTitles["XaxisTitle"] + ";" + Histogram2DTitles["YaxisTitle"]).c_str(),
            HistogramNumberOfXBins, HistogramXAxisLimits.at(0), HistogramXAxisLimits.at(1),
            HistogramNumberOfYBins, HistogramYAxisLimits.at(0), HistogramYAxisLimits.at(1));
}

// histPlotter2D function-----------------------------------------------------------------------------------------------------------------------------------------

// histPlotter2D function (regular)
void
hPlot2D::histPlotter2D(std::string &SampleName, TCanvas *Histogram2DCanvas, TH2D *Histogram2D, TList *Histogram_list, std::string Histogram2DSaveNameDir,
                       std::string Histogram2DSaveName, bool showStats = true) {

    std::string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.16, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    Histogram2D->SetTitleSize(Histogram2DTitleSizes.at(0), "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(Histogram2DTitleSizes.at(1));
    Histogram2D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetYaxis()->SetLabelSize(Histogram2DTitleSizes.at(2));
    Histogram2D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetZaxis()->SetLabelSize(Histogram2DTitleSizes.at(3));
    Histogram_list->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        Histogram2D->SetStats(0);
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->SetTextAlign(12);
        displayText->AddText("Empty histogram");
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        Histogram2D->Draw("colz");
    }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.875);

    if (showStats == false) { Histogram2D->SetStats(0); }

    if (ZLogScalePlot == true) {
        Histogram2DCanvas->SetLogz(1);
        Histogram2DCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + "_zLogScale.png").c_str());
    }

    if (ZLinearScalePlot == true) {
        Histogram2DCanvas->SetLogz(0);
        Histogram2DCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + "_zLinearScale.png").c_str());
    }

    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);
    Histogram2DCanvas->Clear();
//    if (ZLogScalePlot == true) { Histogram2DCanvas->SetLogz(1); }
//
//    if (showStats == false) { Histogram2D->SetStats(0); }
//
//    gStyle->SetStatX(0.87);
//    gStyle->SetStatY(0.875);
//    Histogram2DCanvas->SaveAs((Histogram2DSaveNameDir + sNameFlag + Histogram2DSaveName + ".png").c_str());
//    gStyle->SetStatX(DefStatX);
//    gStyle->SetStatY(DefStatY);
//    Histogram2DCanvas->Clear();
}

void hPlot2D::hDrawAndSave(std::string &SampleName, TCanvas *h2DCanvas, TList *hList, bool showStats) {
    histPlotter2D(SampleName, h2DCanvas, Histogram2D, hList, Histogram2DSaveNamePath, Histogram2DSaveName, showStats);
}

// histPlotter2D function (Beta vs. P plots, all particles):
void hPlot2D::histPlotter2D(TCanvas *Histogram2DCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez,
                            TList *Histogram_list, bool zlogScalePlot, std::string Histogram2DSaveNameDir, std::string Histogram2DSaveName, TF1 *Beta_function1,
                            TF1 *Beta_function2, TF1 *Beta_function3, TF1 *Beta_function4, TF1 *Beta_function5, TF1 *Beta_function6, TF1 *Beta_function7,
                            TF1 *Beta_function8, TF1 *Beta_function9) {

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.16, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
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

    if (zlogScalePlot == true) { Histogram2DCanvas->SetLogz(1); }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);
    Histogram2DCanvas->SaveAs((Histogram2DSaveNameDir + Histogram2DSaveName).c_str());
    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);
    Histogram2DCanvas->Clear();
}

void hPlot2D::hDrawAndSave(TCanvas *h2DCanvas, TList *hList, std::string FinalState, TF1 *Beta_function1, TF1 *Beta_function2, TF1 *Beta_function3, TF1 *Beta_function4,
                           TF1 *Beta_function5, TF1 *Beta_function6, TF1 *Beta_function7, TF1 *Beta_function8, TF1 *Beta_function) {
    histPlotter2D(h2DCanvas, Histogram2D, Histogram2DTitleSizes.at(0), CenterTitle, Histogram2DTitleSizes.at(1), Histogram2DTitleSizes.at(2), Histogram2DTitleSizes.at(3),
                  hList, ZLogScalePlot, Histogram2DSaveNamePath, Histogram2DSaveName, Beta_function1, Beta_function2, Beta_function3, Beta_function4, Beta_function5,
                  Beta_function6, Beta_function7, Beta_function8, Beta_function);
}

// histPlotter2D function (Beta vs. P plots, single particle):
void hPlot2D::histPlotter2D(TCanvas *Histogram2DCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez,
                            TList *Histogram_list, bool zlogScalePlot, std::string Histogram2DSaveNameDir, std::string Histogram2DSaveName, TF1 *Beta_function1,
                            std::string particle1, bool plot_legend) {

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

    if (zlogScalePlot == true) { Histogram2DCanvas->SetLogz(1); }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);
    Histogram2DCanvas->SaveAs((Histogram2DSaveNameDir + Histogram2DSaveName).c_str());
    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);
    Histogram2DCanvas->Clear();
}

void hPlot2D::hDrawAndSave(TCanvas *h2DCanvas, TList *hList, std::string FinalState, TF1 *Beta_function1, std::string particle1, bool plot_legend) {
    histPlotter2D(h2DCanvas, Histogram2D, Histogram2DTitleSizes.at(0), CenterTitle, Histogram2DTitleSizes.at(1), Histogram2DTitleSizes.at(2), Histogram2DTitleSizes.at(3),
                  hList, ZLogScalePlot, Histogram2DSaveNamePath, Histogram2DSaveName, Beta_function1, particle1, plot_legend);
}

// histPlotter2D function (Beta vs. P plots, by charge):
void hPlot2D::histPlotter2D(TCanvas *Histogram2DCanvas, TH2D *Histogram2D, double titleSize, bool centerTitle, double labelSizex, double labelSizey, double labelSizez,
                            TList *Histogram_list, bool zlogScalePlot, std::string Histogram2DSaveNameDir, std::string Histogram2DSaveName, TF1 *Beta_function1,
                            std::string particle1, TF1 *Beta_function2, std::string particle2, TF1 *Beta_function3, std::string particle3, bool plot_legend) {

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

    if (zlogScalePlot == true) { Histogram2DCanvas->SetLogz(1); }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);
    Histogram2DCanvas->SaveAs((Histogram2DSaveNameDir + Histogram2DSaveName).c_str());
    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);
    Histogram2DCanvas->Clear();
}

// hDrawAndSave function-----------------------------------------------------------------------------------------------------------------------------------------

void hPlot2D::hDrawAndSave(TCanvas *h2DCanvas, TList *hList, std::string FinalState, TF1 *Beta_function1, std::string particle1, TF1 *Beta_function2,
                           std::string particle2, TF1 *Beta_function3, std::string particle3, bool plot_legend) {
    histPlotter2D(h2DCanvas, Histogram2D, Histogram2DTitleSizes.at(0), CenterTitle, Histogram2DTitleSizes.at(1), Histogram2DTitleSizes.at(2), Histogram2DTitleSizes.at(3),
                  hList, ZLogScalePlot, Histogram2DSaveNamePath, Histogram2DSaveName, Beta_function1, particle1, Beta_function2, particle2, Beta_function3, particle3,
                  plot_legend);
}

