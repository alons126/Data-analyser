//
// Created by alons on 08/07/2023.
//

#include "DEfficiency.h"

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleName function">
string DEfficiency::GetParticleName(const string &Source) {
    string ParticleName;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleName = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleName = "Electron";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleName = "Proton";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        ParticleName = "Neutron";
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "Piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "Piminus";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleName = "Kplus";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleName = "Kminus";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleName = "Photon";
    } else {
        ParticleName = "Unknown";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameLC function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameLC function">
string DEfficiency::GetParticleNameLC(const string &Source) {
    string ParticleName;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleName = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleName = "electron";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleName = "proton";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        ParticleName = "neutron";
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "piminus";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleName = "kplus";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleName = "kminus";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleName = "photon";
    } else {
        ParticleName = "unknown";
    }

    return ParticleName;
}
//</editor-fold>

// GetParticleNameShort function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetParticleNameShort function">
string DEfficiency::GetParticleNameShort(const string &Source) {
    string ParticleNameShort;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleNameShort = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleNameShort = "e";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleNameShort = "p";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        ParticleNameShort = "n";
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleNameShort = "#pi^{+}";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleNameShort = "#pi^{-}";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleNameShort = "K^{+}";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleNameShort = "K^{-}";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleNameShort = "Photon";
    } else {
        ParticleNameShort = "#gamma";
    }

    return ParticleNameShort;
}
//</editor-fold>

// GetFS function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetFS function">
string DEfficiency::GetFS(const string &Source) {
    string FinalState;

    if (findSubstring(Source, "1e_cut") || findSubstring(Source, "1e cut") || findSubstring(Source, "1e Cut")) {
        FinalState = "1e cut";
    } else if (findSubstring(Source, "1p")) {
        FinalState = "1p";
    } else if (findSubstring(Source, "1n")) {
        FinalState = "1n";
    } else if (findSubstring(Source, "1n1p")) {
        FinalState = "1n1p";
    } else if (findSubstring(Source, "1e2p")) {
        FinalState = "1e2p";
    } else if (findSubstring(Source, "2p")) {
        FinalState = "2p";
    } else if (findSubstring(Source, "pFDpCD")) {
        FinalState = "pFDpCD";
    } else if (findSubstring(Source, "nFDpCD")) {
        FinalState = "nFDpCD";
    }

    return FinalState;
}
//</editor-fold>

// GetType function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetType function">
string DEfficiency::GetType(const string &Source) {
    string Type;

    if (findSubstring(Source, "momentum")) { // for momentum efficiency plots
        Type = "momentum";
    } else if (findSubstring(Source, "#theta")) { // for theta efficiency plots
        Type = "theta";
    } else if (findSubstring(Source, "#phi")) { // for phi efficiency plots
        Type = "phi";
    }

    return Type;
}
//</editor-fold>

// GetTitle function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetTitle function">
string DEfficiency::GetTitle(const string &Source) {
    string Title, Type = GetType(Source), ParicleShort = GetParticleNameShort(Source), Paricle = GetParticleName(Source);

    if (Type == "momentum") {
        Title = Paricle + " " + Type;
    } else {
        Title = "#" + Type + "_{" + ParicleShort + "}";
    }

    return Title;
}
//</editor-fold>

// SetXLabel function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetXLabel function">
string DEfficiency::SetXLabel(const string &Source) {
    string XLabel;

    if (findSubstring(Source, "momentum")) {
        XLabel = "P_{" + Source + "} [GeV/c]";
    } else if (findSubstring(Source, "#theta")) {
        XLabel = Source + " [Deg]";
    } else if (findSubstring(Source, "#phi")) {
        XLabel = Source + " [Deg]";
    }

    return XLabel;
}
//</editor-fold>

// SetSaveDir function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetSaveDir function">
string DEfficiency::SetSaveDir(const string &Source, const string &BaseSaveDir, const string &Mod) {
    string SaveDir, TestSaveDir;

    string Particle = GetParticleName(Source), ParticleLC = GetParticleNameLC(Source), FS = GetFS(Source), Type = GetType(Source);

    if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        SaveDir = BaseSaveDir + "/00_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
        TestSaveDir = SaveDir + "Cloned_hist_test/";
    } else {
        if (findSubstring(Source, ", FD)") || findSubstring(Source, "FD " + Particle) || findSubstring(Source, "FD " + ParticleLC)) {
            SaveDir = BaseSaveDir + "/01_FD_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        } else if (findSubstring(Source, ", CD)") || findSubstring(Source, "CD " + Particle) || findSubstring(Source, "CD " + ParticleLC)) {
            SaveDir = BaseSaveDir + "/02_CD_" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        } else {
            SaveDir = BaseSaveDir + "/" + Particle + "_" + Type + "_ACorrection_plots_" + FS + "/";
            TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
        }
    }

    if ((Mod == "test") || (Mod == "Test")) {
        system(("mkdir -p " + TestSaveDir).c_str());

        return TestSaveDir;
    } else {
        system(("mkdir -p " + SaveDir).c_str());

        return SaveDir;
    }
}
//</editor-fold>

// DrawAndSaveHistogram1D function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveHistogram1D function">
void DEfficiency::DrawAndSaveHistogram1D(TCanvas *HistCanvas, TList *Histogram_list, TH1D *Histogram1D, int LineStyle, int kColor, bool ShowStats, bool weighted_plots,
                                         bool rebin_plots, const string &HistSaveDir) {
    Histogram1D->SetLineStyle(LineStyle);
    Histogram1D->SetLineColor(kColor);
    Histogram1D->SetStats(ShowStats);
    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(2);

    if (weighted_plots) { Histogram1D->Sumw2(); }

    if (rebin_plots) { Histogram1D->Rebin(2); }

    Histogram1D->Draw();
    Histogram_list->Add(Histogram1D);

    HistCanvas->SaveAs(HistSaveDir.c_str());
    HistCanvas->Clear();
}
//</editor-fold>

// DrawAndSaveACorrPlots function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveACorrPlots function">
void DEfficiency::DrawAndSaveACorrPlots(bool save_ACorr_data, string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot,
                                        TList *Histogram_list, TList *ACorr_data, string &ACorr_data_Dir) {
    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
//    Canvas->cd();
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    TH1D *Histogram1D_REC = RPlot.GetHistogram();
    string RPlot_Clone_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned";
    TH1D *RPlot_Clone = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_StatsTitle).c_str());
    string RPlot_Clone_test_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned test";
    TH1D *RPlot_Clone_test = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_test_StatsTitle).c_str());
    string RPlot_Clone_test_rebined_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *RPlot_Clone_test_rebined = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    RPlot_Clone_test_rebined->Rebin(2);

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    string TLPlot_Clone_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned";
    TH1D *TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    string TLPlot_Clone_test_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test";
    TH1D *TLPlot_Clone_test = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    string TLPlot_Clone_test_rebined_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *TLPlot_Clone_test_rebined = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    TLPlot_Clone_test_rebined->Rebin(2);
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

    string ACorrectionSaveDir = SetSaveDir(ACorrectionRecTitle, TLPlot.GetHistogram1DSaveNamePath());
    string ACorrectionTestSaveDir = SetSaveDir(ACorrectionRecTitle, TLPlot.GetHistogram1DSaveNamePath(), "test");

    //<editor-fold desc="Setting save name">
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
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
//    TH1D *ACorrection_plot = (TH1D *) RPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #epsilon_{eff}" + " (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->SetTitle((ACorrectionTitle + " acceptance correction #alpha = 1/#epsilon_{eff} (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->GetYaxis()->SetTitle((ACorrectionYLabel).c_str());
    ACorrection_plot->GetXaxis()->SetTitle((ACorrectionXLabel).c_str());

    DrawAndSaveHistogram1D(Canvas, Histogram_list, RPlot_Clone_test, 1, kBlue, true, false, false, RPlot_Clone_test_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, TLPlot_Clone_test, 1, kBlue, true, false, false, TLPlot_Clone_test_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, RPlot_Clone_test_rebined, 1, kBlue, true, false, false, RPlot_Clone_test_rebined_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, TLPlot_Clone_test_rebined, 1, kBlue, true, false, false, TLPlot_Clone_test_rebined_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, RPlot_Clone, 1, kBlue, true, true, true, RPlot_Clone_SaveName);
    DrawAndSaveHistogram1D(Canvas, Histogram_list, TLPlot_Clone, 1, kBlue, true, true, true, TLPlot_Clone_SaveName);

    //<editor-fold desc="Plotting and saving ACorrection_plot">
    ACorrection_plot->SetLineStyle(1);
    ACorrection_plot->SetLineColor(kBlue);

    if (weighted_plots) { ACorrection_plot->Sumw2(); }

    ACorrection_plot->Rebin(2);
    ACorrection_plot->Divide(RPlot_Clone);
//    ACorrection_plot->Divide(TLPlot_Clone);
    ACorrection_plot->Draw();
    ACorrection_plot->SetStats(0);
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

        if (weighted_plots) { ACorr_factor->Sumw2(); }

        ACorr_factor->Draw();
        ACorr_factor->SetStats(0);
        ACorr_data->Add(ACorr_factor);
        Canvas->Clear();
    }
    //</editor-fold>

    delete Canvas;
}
//</editor-fold>