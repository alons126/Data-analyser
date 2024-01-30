//
// Created by alons on 08/07/2023.
//

#include "DEfficiency.h"

// ResetHistograms function ----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ResetHistograms function">
void DEfficiency::ResetHistograms() {
    Histogram1D_REC->Reset();
    RPlot_Clone_StatsTitle = "";
    RPlot_Clone->Reset();
    RPlot_Clone_test_StatsTitle = "";
    RPlot_Clone_test->Reset();
    RPlot_Clone_test_rebined_StatsTitle = "";
    RPlot_Clone_test_rebined->Reset();

    Histogram1D_Truth->Reset();
    TLPlot_Clone_StatsTitle = "";
    TLPlot_Clone->Reset();
    TLPlot_Clone_test_StatsTitle = "";
    TLPlot_Clone_test->Reset();
    TLPlot_Clone_test_rebined_StatsTitle = "";
    TLPlot_Clone_test_rebined->Reset();

    EffAndACorr_SaveNamePath = "";
}
//</editor-fold>

// LoadHistograms function ----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LoadHistograms function">
void DEfficiency::LoadHistograms(const string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot) {
    Histogram1D_REC = RPlot.GetHistogram();
    RPlot_Clone_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned";
    RPlot_Clone = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_StatsTitle).c_str());
    RPlot_Clone_test_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned test";
    RPlot_Clone_test = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_test_StatsTitle).c_str());
    RPlot_Clone_test_rebined_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned test rebined";
    RPlot_Clone_test_rebined = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { RPlot_Clone_test_rebined->Rebin(2); }

    Histogram1D_Truth = TLPlot.GetHistogram();
    TLPlot_Clone_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned";
    TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    TLPlot_Clone_test_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test";
    TLPlot_Clone_test = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    TLPlot_Clone_test_rebined_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TLPlot_Clone_test_rebined = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { TLPlot_Clone_test_rebined->Rebin(2); }
    EffAndACorr_SaveNamePath = TLPlot.GetHistogram1DSaveNamePath();
}
//</editor-fold>

//<editor-fold desc="LoadHistograms function">
void DEfficiency::LoadHistograms(const string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot) {
    string StatsTitle = GetStatsTitle(RPlot->GetTitle());

    RPlot_Clone_StatsTitle = "reco. " + StatsTitle + " - cloned";
    RPlot_Clone = (TH1D *) RPlot->Clone((RPlot_Clone_StatsTitle).c_str());
    RPlot_Clone_test_StatsTitle = "reco. " + StatsTitle + " - cloned test";
    RPlot_Clone_test = (TH1D *) RPlot->Clone((RPlot_Clone_test_StatsTitle).c_str());
    RPlot_Clone_test_rebined_StatsTitle = "reco. " + StatsTitle + " - cloned test rebined";
    RPlot_Clone_test_rebined = (TH1D *) RPlot->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { RPlot_Clone_test_rebined->Rebin(2); }

    Histogram1D_Truth = TLPlot.GetHistogram();
    TLPlot_Clone_StatsTitle = "Truth " + StatsTitle + " - cloned";
    TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    TLPlot_Clone_test_StatsTitle = "Truth " + StatsTitle + " - cloned test";
    TLPlot_Clone_test = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    TLPlot_Clone_test_rebined_StatsTitle = "Truth " + StatsTitle + " - cloned test rebined";
    TLPlot_Clone_test_rebined = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { TLPlot_Clone_test_rebined->Rebin(2); }

    EffAndACorr_SaveNamePath = TLPlot.GetHistogram1DSaveNamePath();
}
//</editor-fold>

// DrawACorrHistograms function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawACorrHistograms function">
void DEfficiency::DrawACorrHistograms(bool save_ACorr_data, const string &SampleName, TList *Histogram_list, TList *ACorr_data, string &ACorr_data_Dir) {

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res

    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);
    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    string ACorrectionRecTitle = RPlot_Clone->GetTitle();

    string ACorrectionParticle = GetParticleName(ACorrectionRecTitle);
    string ACorrectionParticleLC = GetParticleNameLC(ACorrectionRecTitle);
    string ACorrectionParticleShort = GetParticleNameShort(ACorrectionRecTitle);

    string ACorrectionType = GetType(ACorrectionRecTitle);
    string ACorrectionTitle = GetTitle(ACorrectionRecTitle);

    string ACorrectionXLabel = SetXLabel(ACorrectionRecTitle);

    //<editor-fold desc="Setting y axis label">
    string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    string ACorrectionYLabel = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabel_Truth + "/" + xLabel_REC + "^{rec}";
//    string ACorrectionYLabel = "#epsilon_{eff} = " + xLabel_REC + "^{rec}" + "/" + xLabel_Truth;
    //</editor-fold>

    string ACorrectionFS = GetFS(ACorrectionRecTitle);

    string ACorrectionSaveDir = SetSaveDir(ACorrectionRecTitle, EffAndACorr_SaveNamePath);
    string ACorrectionTestSaveDir = SetSaveDir(ACorrectionRecTitle, EffAndACorr_SaveNamePath, "test");

    //<editor-fold desc="Setting save name">
    string sNameFlag;

    if (findSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    string RPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone.png";
    string RPlot_Clone_test_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "01a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test.png";
    string RPlot_Clone_test_rebined_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "01b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test_rebined.png";
    string TLPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone.png";
    string TLPlot_Clone_test_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "02a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test.png";
    string TLPlot_Clone_test_rebined_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "02b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test_rebined.png";
    string ACorrection_plot_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_" + ACorrectionFS + ".png";
    //</editor-fold>

    TH1D *ACorrection_plot = (TH1D *) TLPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #alpha" + " (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->SetTitle((ACorrectionTitle + " acceptance correction #alpha = 1/#epsilon_{eff} (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->GetYaxis()->SetTitle((ACorrectionYLabel).c_str());
    ACorrection_plot->GetXaxis()->SetTitle((ACorrectionXLabel).c_str());

    string ACorrection_plot_Name = ACorrection_plot->GetName();
    string TFolder_Name = ACorrection_plot_Name + " folder";
    TFolder *ACorrectionComponentPlots = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

    DrawAndSaveHistogram1D(Canvas, Histogram_list, ACorrectionComponentPlots, RPlot_Clone_test, 1, kBlue, true, false, false, RPlot_Clone_test_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, ACorrectionComponentPlots, TLPlot_Clone_test, 1, kBlue, true, false, false, TLPlot_Clone_test_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, ACorrectionComponentPlots, RPlot_Clone_test_rebined, 1, kBlue, true, false, false, RPlot_Clone_test_rebined_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, ACorrectionComponentPlots, TLPlot_Clone_test_rebined, 1, kBlue, true, false, false, TLPlot_Clone_test_rebined_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, ACorrectionComponentPlots, RPlot_Clone, 1, kBlue, true, true, true, RPlot_Clone_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, ACorrectionComponentPlots, TLPlot_Clone, 1, kBlue, true, true, true, TLPlot_Clone_SaveName);
//    DrawAndSaveHistogram1D(Canvas, Histogram_list, RPlot_Clone_test, 1, kBlue, true, false, false, RPlot_Clone_test_SaveName);
//    DrawAndSaveHistogram1D(Canvas, Histogram_list, TLPlot_Clone_test, 1, kBlue, true, false, false, TLPlot_Clone_test_SaveName);
//    DrawAndSaveHistogram1D(Canvas, Histogram_list, RPlot_Clone_test_rebined, 1, kBlue, true, false, false, RPlot_Clone_test_rebined_SaveName);
//    DrawAndSaveHistogram1D(Canvas, Histogram_list, TLPlot_Clone_test_rebined, 1, kBlue, true, false, false, TLPlot_Clone_test_rebined_SaveName);
//    DrawAndSaveHistogram1D(Canvas, Histogram_list, RPlot_Clone, 1, kBlue, true, true, true, RPlot_Clone_SaveName);
//    DrawAndSaveHistogram1D(Canvas, Histogram_list, TLPlot_Clone, 1, kBlue, true, true, true, TLPlot_Clone_SaveName);

    //<editor-fold desc="Plotting and saving ACorrection_plot">
    ACorrection_plot->SetLineStyle(1);
    ACorrection_plot->SetLineColor(kBlue);
    ACorrection_plot->SetStats(false);
    ACorrection_plot->GetXaxis()->SetTitleSize(0.06);
    ACorrection_plot->GetXaxis()->SetLabelSize(0.0425);
    ACorrection_plot->GetXaxis()->CenterTitle(true);
    ACorrection_plot->GetYaxis()->SetTitle("Number of events");
    ACorrection_plot->GetYaxis()->SetTitleSize(0.06);
    ACorrection_plot->GetYaxis()->SetLabelSize(0.0425);
    ACorrection_plot->GetYaxis()->CenterTitle(true);
    ACorrection_plot->SetLineWidth(2);

    if (plot_errorbars) { ACorrection_plot->Sumw2(); }

    if (rebin_plots) { ACorrection_plot->Rebin(2); }

    ACorrection_plot->Divide(RPlot_Clone);
    ACorrection_plot->Draw();
    ACorrection_plot->SetStats(0);
    Histogram_list->Add(ACorrectionComponentPlots);
    Histogram_list->Add(ACorrection_plot);
    Canvas->SaveAs((ACorrection_plot_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Save acceptance correction data">
    if (save_ACorr_data) {
        system(("rm -r " + ACorr_data_Dir).c_str()); // clear old ACorr_data_Dir
        system(("mkdir -p " + ACorr_data_Dir).c_str()); // recreate ACorr_data_Dir

        string ACorr_data_StatsTitle;

        if ((ACorrectionFS == "pFDpCD") || (ACorrectionFS == "nFDpCD")) {
            if (findSubstring(ACorrectionRecTitle, ", FD)") ||
                findSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticle) ||
                findSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticleLC)) {
                ACorr_data_StatsTitle = "FD_" + ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            } else if (findSubstring(ACorrectionRecTitle, ", CD)") ||
                       findSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticle) ||
                       findSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticleLC)) {
                ACorr_data_StatsTitle = "CD_" + ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            } else {
                ACorr_data_StatsTitle = ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            }
        } else {
            ACorr_data_StatsTitle = ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
        }

        TH1D *ACorr_factor = (TH1D *) ACorrection_plot->Clone(ACorr_data_StatsTitle.c_str());

        ACorr_factor->SetLineStyle(1);
        ACorr_factor->SetLineColor(kBlue);

        if (plot_errorbars) { ACorr_factor->Sumw2(); }

        ACorr_factor->Draw();
        ACorr_factor->SetStats(0);
        ACorr_data->Add(ACorr_factor);
        Canvas->Clear();
    }
    //</editor-fold>

    delete Canvas;
}
//</editor-fold>

// DrawAndSaveACorrPlots function ---------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveACorrPlots function">
void DEfficiency::DrawAndSaveACorrPlots(bool save_ACorr_data, const string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list, TList *ACorr_data,
                                        string &ACorr_data_Dir) {
    LoadHistograms(SampleName, TLPlot, RPlot);
    DrawACorrHistograms(save_ACorr_data, SampleName, Histogram_list, ACorr_data, ACorr_data_Dir);
//    ResetHistograms();
}
//</editor-fold>

//<editor-fold desc="DrawAndSaveACorrPlots function">
void DEfficiency::DrawAndSaveACorrPlots(bool save_ACorr_data, const string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot, TList *Histogram_list, TList *ACorr_data,
                                        string &ACorr_data_Dir) {
    LoadHistograms(SampleName, TLPlot, RPlot);
    DrawACorrHistograms(save_ACorr_data, SampleName, Histogram_list, ACorr_data, ACorr_data_Dir);
//    ResetHistograms();
}
//</editor-fold>

// DrawAndSaveHistogram1D function --------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveHistogram1D function">
void DEfficiency::DrawAndSaveHistogram1D(TCanvas *HistCanvas, TList *Histogram_list, TFolder *Histogram_folder, TH1D *Histogram1D, int LineStyle, int kColor, bool ShowStats,
                                         bool PlotErrorbars, bool RebinPlots, const string &HistSaveDir) {
    //TODO: remove TList *Histogram_list from arguments if not needed
    Histogram1D->SetLineStyle(LineStyle);
    Histogram1D->SetLineColor(kColor);
    Histogram1D->SetStats(ShowStats);
    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitle("Number of events");
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(2);

    if (PlotErrorbars) { Histogram1D->Sumw2(); }

    if (RebinPlots) { Histogram1D->Rebin(2); }

    Histogram1D->Draw();
    Histogram_folder->Add(Histogram1D);
//    Histogram_list->Add(Histogram1D);

    HistCanvas->SaveAs(HistSaveDir.c_str());
    HistCanvas->Clear();
}
//</editor-fold>
