//_____________________macro fitexample.C___________________________
#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
#include <sstream>

//template<typename T>;
//
//std::string to_string_with_precision(const T a_value, const int n = 2) {
//    std::ostringstream out;
//    out.precision(n);
//    out << std::fixed << a_value;
//    return out.str();
//}

Double_t FitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
//    if (par[1] != 0) { arg = (v[0] - 1) / par[1]; } // 2 parameters
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}

void fitexample() {
    cout << "\n\n";

    TFile *f = new
    TFile("./plots_C12_simulation_6GeV_T5_first_10_-_ALL_CUTS2222/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//    TFile("./plots_C12_simulation_6GeV_T5_first_100_-_ALL_CUTS222/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//    TFile("./plots_C12_simulation_6GeV_T5_first_100_-_ALL_CUTS2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//    TFile("./plots_C12_simulation_6GeV_T5_first_100_-_ALL_CUTS22/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//    TFile("./plots_C12_simulation_6GeV_T5_-_ALL_CUTS2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");

    TH1D *hpx = (TH1D *) f->Get("#beta of n from '#gamma' (1n, FD)");

    //<editor-fold desc="Old 3">
    //create a function with 3 parameters in the range [-3,3]
    TF1 *func = new TF1("fit", FitFunction, 0, 2, 3);
    func->SetLineColor(kRed);

    double max = hpx->GetMaximum();
//    double max = hpx->GetBinCenter(hpx->GetMaximumBin());
    double mean = hpx->GetMean();

    func->SetParameters(max, mean, 0.001);
//    func->SetParameters(5, 1, 0.001);
//    func->SetParameters(300, 1, 0.001);
    func->SetParNames("Constant", "Mean_value", "Sigma");

    cout << "max = " << max << "\n";
//    cout << "hpx->GetBinCenter(hpx->GetMaximumBin()) = " << hpx->GetBinCenter(hpx->GetMaximumBin()) << "\n";
//    cout << "hpx->GetMaximumBin() = " << hpx->GetMaximumBin() << "\n\n";

//    func->SetParLimits(0, 3.5, 1.2*max); // amp limits
    func->SetParLimits(0, 0.7*max, 1.2*max); // amp limits
//    func->SetParLimits(1, 0.9*mean, 1.1*mean); // mean limits
////    func->SetParLimits(0, 215, 500);
//    func->SetParLimits(1, 0.999, 1.05);
//    func->SetParLimits(2, 0.00001, 0.05);

//    func->FixParameter(1, 1);
//    func->SetParLimits(1, 1, 1.05);
    hpx->Fit("fit");

    hpx->Draw();
    hpx->SetLineColor(kBlack);






    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
    auto Cut_legend = new TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);

    double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
    double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;
//    double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
//    double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.245;

    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam-0.05+0.05, x_2_FitParam, y_2_FitParam+0.05, "NDC");
//    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    deltaPParam->SetBorderSize(1);
    deltaPParam->SetTextFont(0);
    deltaPParam->SetTextSize(0.03);
    deltaPParam->SetFillColor(0);
    deltaPParam->SetTextAlign(12);
    deltaPParam->AddText("#beta = 0.0000000");
    deltaPParam->Draw("same");

//    double deltaPRel_UncertaintyU = 0.2;
//
//    TLine *upper_cut = new TLine(gPad->GetFrame()->GetX2()-0.5, deltaPRel_UncertaintyU, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyU);
//    upper_cut->SetLineColor(kRed);
//    upper_cut->Draw("same");
//    //    TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());


/*//    Histogram1D->GetListOfFunctions()->Remove(Histogram1D->GetFunction("gaus"));

    TF1 *fit = hpx->GetFunction("gaus");
    double Amp = fit->GetParameter(0); // get p0
    double Mean = fit->GetParameter(1); // get p1
    double Std = fit->GetParameter(2); // get p2

    cout << "\n\n\nAmp:\t" << Amp << "\n";
    cout << "Mean:\t" << Mean << "\n";
    cout << "Std:\t" << Std << "\n\n\n\n";

    double titleSize = 0.06;
    double labelSizex = 0.0425;
    double labelSizey = 0.0425;

    int lineWidth = 2;
    bool logScalePlot = false;
    bool linearScalePlot = true;

    int kColor = kBlack;
    bool centerTitle = true;
    bool showStats = true;
    bool title2 = false;
    bool apply_plot_cuts = true;

    double factor = 2.;
    double plot_cuts = factor * Std;
    double plot_Xmax = Mean;

    double Upper_cut = plot_cuts + plot_Xmax;
    double Lower_cut = -plot_cuts + plot_Xmax;
    double plot_xmax = plot_Xmax;
    TLine *upper_cut = new
    TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
    upper_cut->SetLineWidth(lineWidth);
    TLine *lower_cut = new
    TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
    lower_cut->SetLineWidth(lineWidth);
    TLine *max_location = new
    TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
    max_location->SetLineWidth(lineWidth + 1);

    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;

    auto Cut_legend = new
    TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);
    gPad->Update();
    upper_cut->Draw("same");
    upper_cut->SetLineColor(kBlue);

    lower_cut->Draw("same");
    lower_cut->SetLineColor(kRed);

    max_location->Draw("same");
    max_location->SetLineColor(kGreen);

    TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("UC = " + to_string_with_precision(Upper_cut)).c_str(), "l");
    TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("LC = " + to_string_with_precision(Lower_cut)).c_str(), "l");
    TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Mean = " + to_string_with_precision(plot_xmax)).c_str(), "l");

    Cut_legend->Draw("same");

    double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
    double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.41;

    TPaveText *FitParam = new
    TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "TL && NDC");
//            FitParam->SetTextStyle(1);
    FitParam->SetBorderSize(1);
//    FitParam->UseCurrentStyle();
//    FitParam->SetBBoxY1(1);
//    FitParam->SetTextFont(0);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(11);
    FitParam->AddText(("Fit amp = " + to_string_with_precision(Amp, 3)).c_str());
//    ((TText*)FitParam->GetListOfLines()->Last())->ETitlePos(kCenter);
//    gPad->Update();
    FitParam->AddText(("Fit std = " + to_string_with_precision(Std, 3)).c_str());
    FitParam->AddText(("Fit mean = " + to_string_with_precision(Mean, 3)).c_str());
    FitParam->AddText(("Cuts = std * " + to_string_with_precision(factor, 2)).c_str());
    FitParam->Draw("same");*/
    //</editor-fold>

//    //<editor-fold desc="Old 2">
//    //create a function with 3 parameters in the range [-3,3]
//    TF1 *func = new
////    TF1("fit", FitFunction, 0.985, 1.04, 3);
//    TF1("fit", FitFunction, "R", "", 0.9, 1.1);
////    TF1("fit", FitFunction, 0.9, 1.1, 3);
//    func->SetLineColor(kRed);
//    func->SetParameters(250, 1, 0.001);
//    func->SetParNames("Constant", "Mean_value", "Sigma");
////    func->SetParLimits(1, 1.0, 1.06);
////    func->SetParLimits(0, 225, 300);
//    func->SetParLimits(2, 0.00005, 0.2);
//
//
////    func->SetParNames("Constant", "Mean_value", "Sigma");
////    func->SetParNames("p0", "p1", "p2");
////    func->SetParameters(250, 0.001);
//////    func->SetParameters(666, hpx->GetRMS());
//////    func->SetParameters(1000, hpx->GetMean(), hpx->GetRMS());
////    func->SetParNames("Constant","Sigma");
////    func->SetParLimits(0,225,260);
////    func->SetParLimits(1,0.00005,0.006);
//////    func->SetParNames("Constant", "Mean_value", "Sigma");
//////    func->SetParNames("p0", "p1", "p2");
//    hpx->Fit("fit");
//    hpx->Draw();
//    hpx->SetLineColor(kBlack);
//
///*//    Histogram1D->GetListOfFunctions()->Remove(Histogram1D->GetFunction("gaus"));
//
//    TF1 *fit = hpx->GetFunction("gaus");
//    double Amp = fit->GetParameter(0); // get p0
//    double Mean = fit->GetParameter(1); // get p1
//    double Std = fit->GetParameter(2); // get p2
//
//    cout << "\n\n\nAmp:\t" << Amp << "\n";
//    cout << "Mean:\t" << Mean << "\n";
//    cout << "Std:\t" << Std << "\n\n\n\n";
//
//    double titleSize = 0.06;
//    double labelSizex = 0.0425;
//    double labelSizey = 0.0425;
//
//    int lineWidth = 2;
//    bool logScalePlot = false;
//    bool linearScalePlot = true;
//
//    int kColor = kBlack;
//    bool centerTitle = true;
//    bool showStats = true;
//    bool title2 = false;
//    bool apply_plot_cuts = true;
//
//    double factor = 2.;
//    double plot_cuts = factor * Std;
//    double plot_Xmax = Mean;
//
//    double Upper_cut = plot_cuts + plot_Xmax;
//    double Lower_cut = -plot_cuts + plot_Xmax;
//    double plot_xmax = plot_Xmax;
//    TLine *upper_cut = new
//    TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
//    upper_cut->SetLineWidth(lineWidth);
//    TLine *lower_cut = new
//    TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
//    lower_cut->SetLineWidth(lineWidth);
//    TLine *max_location = new
//    TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
//    max_location->SetLineWidth(lineWidth + 1);
//
//    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
//    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
//
//    auto Cut_legend = new
//    TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);
//    gPad->Update();
//    upper_cut->Draw("same");
//    upper_cut->SetLineColor(kBlue);
//
//    lower_cut->Draw("same");
//    lower_cut->SetLineColor(kRed);
//
//    max_location->Draw("same");
//    max_location->SetLineColor(kGreen);
//
//    TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("UC = " + to_string_with_precision(Upper_cut)).c_str(), "l");
//    TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("LC = " + to_string_with_precision(Lower_cut)).c_str(), "l");
//    TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Mean = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//
//    Cut_legend->Draw("same");
//
//    double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
//    double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.41;
//
//    TPaveText *FitParam = new
//    TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "TL && NDC");
////            FitParam->SetTextStyle(1);
//    FitParam->SetBorderSize(1);
////    FitParam->UseCurrentStyle();
////    FitParam->SetBBoxY1(1);
////    FitParam->SetTextFont(0);
//    FitParam->SetFillColor(0);
//    FitParam->SetTextAlign(11);
//    FitParam->AddText(("Fit amp = " + to_string_with_precision(Amp, 3)).c_str());
////    ((TText*)FitParam->GetListOfLines()->Last())->ETitlePos(kCenter);
////    gPad->Update();
//    FitParam->AddText(("Fit std = " + to_string_with_precision(Std, 3)).c_str());
//    FitParam->AddText(("Fit mean = " + to_string_with_precision(Mean, 3)).c_str());
//    FitParam->AddText(("Cuts = std * " + to_string_with_precision(factor, 2)).c_str());
//    FitParam->Draw("same");*/
//    //</editor-fold>

//    //<editor-fold desc="Old">
//    //    //create a function with 3 parameters in the range [-3,3]
////    TF1 * func = new
////    TF1("fit", FitFunction, -3, 3, 3);
////    func->SetLineColor(kBlue);
////    func->SetParameters(1000, hpx->GetMean(), hpx->GetRMS());
////    func->SetParNames("Constant", "Mean_value", "Sigma");
//////    func->SetParNames("p0", "p1", "p2");
////    hpx->Fit("fit");
////    hpx->Fit("gaus", "W");
////    hpx->Fit("gaus", "WW");
////    hpx->Fit("gaus", "I");
////    hpx->Fit("gaus", "F");
////    hpx->Fit("gaus", "U");
////    hpx->Fit("gaus", "E");
////    hpx->Fit("gaus", "M");
////    hpx->Fit("gaus", "S");
//    hpx->Fit("gaus");
////    c1->clear();
//    hpx->Draw();
//
////    Histogram1D->GetListOfFunctions()->Remove(Histogram1D->GetFunction("gaus"));
//
//    TF1 *fit = hpx->GetFunction("gaus");
//    double Amp = fit->GetParameter(0); // get p0
//    double Mean = fit->GetParameter(1); // get p1
//    double Std = fit->GetParameter(2); // get p2
//
//    cout << "\n\n\nAmp:\t" << Amp << "\n";
//    cout << "Mean:\t" << Mean << "\n";
//    cout << "Std:\t" << Std << "\n\n\n\n";
//
//    double titleSize = 0.06;
//    double labelSizex = 0.0425;
//    double labelSizey = 0.0425;
//
//    int lineWidth = 2;
//    bool logScalePlot = false;
//    bool linearScalePlot = true;
//
//    int kColor = kBlack;
//    bool centerTitle = true;
//    bool showStats = true;
//    bool title2 = false;
//    bool apply_plot_cuts = true;
//
//    double factor = 2.;
//    double plot_cuts = factor * Std;
//    double plot_Xmax = Mean;
//
//    double Upper_cut = plot_cuts + plot_Xmax;
//    double Lower_cut = -plot_cuts + plot_Xmax;
//    double plot_xmax = plot_Xmax;
//    TLine *upper_cut = new
//    TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
//    upper_cut->SetLineWidth(lineWidth);
//    TLine *lower_cut = new
//    TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
//    lower_cut->SetLineWidth(lineWidth);
//    TLine *max_location = new
//    TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
//    max_location->SetLineWidth(lineWidth + 1);
//
//    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
//    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
//
//    auto Cut_legend = new
//    TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);
//    gPad->Update();
//    upper_cut->Draw("same");
//    upper_cut->SetLineColor(kBlue);
//
//    lower_cut->Draw("same");
//    lower_cut->SetLineColor(kRed);
//
//    max_location->Draw("same");
//    max_location->SetLineColor(kGreen);
//
//    TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("UC = " + to_string_with_precision(Upper_cut)).c_str(), "l");
//    TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("LC = " + to_string_with_precision(Lower_cut)).c_str(), "l");
//    TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Mean = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//
//    Cut_legend->Draw("same");
//
//    double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
//    double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.41;
//
//    TPaveText *FitParam = new
//    TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "TL && NDC");
////            FitParam->SetTextStyle(1);
//    FitParam->SetBorderSize(1);
////    FitParam->UseCurrentStyle();
////    FitParam->SetBBoxY1(1);
////    FitParam->SetTextFont(0);
//    FitParam->SetFillColor(0);
//    FitParam->SetTextAlign(11);
//    FitParam->AddText(("Fit amp = " + to_string_with_precision(Amp, 3)).c_str());
////    ((TText*)FitParam->GetListOfLines()->Last())->ETitlePos(kCenter);
////    gPad->Update();
//    FitParam->AddText(("Fit std = " + to_string_with_precision(Std, 3)).c_str());
//    FitParam->AddText(("Fit mean = " + to_string_with_precision(Mean, 3)).c_str());
//    FitParam->AddText(("Cuts = std * " + to_string_with_precision(factor, 2)).c_str());
//    FitParam->Draw("same");
//    //</editor-fold>
}

//void fitexample() {
//    TFile * f = new
//    TFile("./plots/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//
//    TH1F *hpx = (TH1F *) f->Get("#chi^{2}_{p} (1e cut, FD)");
//
//    //create a function with 3 parameters in the range [-3,3]
//    TF1 * func = new
//    TF1("fit", FitFunction, -3, 3, 3);
//    func->SetParameters(500, hpx->GetMean(), hpx->GetRMS());
//    func->SetParNames("Constant", "Mean_value", "Sigma");
////    func->SetParNames("p0", "p1", "p2");
//    hpx->Fit("fit");
//
//
// }