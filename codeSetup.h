
#ifndef ANALYSER_CODESETUP_H
#define ANALYSER_CODESETUP_H

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
//#include <sstream>


// ======================================================================================================================================================================
// Code version
// ======================================================================================================================================================================

std::string Ver = "DetSim testings";

// ======================================================================================================================================================================
// Functions definitions
// ======================================================================================================================================================================

//<editor-fold desc="Functions definitions">

//template <typename T>
//std::string to_string_with_precision(const T a_value, const int n = 6) {
//    std::ostringstream out;
//    out.precision(n);
//    out << std::fixed << a_value;
//    return out.str();
//}

//<editor-fold desc="BoolToString function">
inline const char *const BoolToString(bool b) {
    return b ? "true" : "false";
}
//</editor-fold>

//<editor-fold desc="rCalc function">
double rCalc(double x, double y, double z) {
    double r = sqrt(x * x + y * y + z * z);
    return r;
}
//</editor-fold>

// TODO: figure out how to clear parent content appropriately
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

// TODO: to finish
////<editor-fold desc="DirectoryChecker function">
//void DirectoryChecker(std::string Directory) {
//    if (IsPathExist(Directory.c_str())) {
//        cout << Directory << " exists!\n\n";
//    } else {
//        string commend = "mkdir " + Directory;
//        system(commend.c_str());
//        cout << Directory << " does not exists! directory created.\n\n";
//    }
//}
////</editor-fold>

// TODO: to finish
//<editor-fold desc="histPlotter1D function">
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
                   bool plot_chi2_cuts = false,
                   double chi2_cuts = 0,
                   double chi2_max = 0) {

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
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units");
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

    if (plot_chi2_cuts == true) {
        gPad->Update();
        double Upper_cut = chi2_cuts + chi2_max;
        double Lower_cut = -chi2_cuts + chi2_max;
        double Chi2_max = chi2_max;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(chi2_max, 0., chi2_max, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
//            upper_cut->SetLineColor(kMagenta);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
//            lower_cut->SetLineColor(kMagenta);
            max_location->Draw("same");
            max_location->SetLineColor(kGreen);
//            lower_cut->SetLineColor(kMagenta);

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string(Lower_cut)).c_str(), "l");
            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string(Chi2_max)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + to_string(max_location)).c_str(), "l");
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

//std::string AnalyseFile = "/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo";
std::string AnalyseFile = "/mnt/d/e4nu/hipo_data_files/recon_c12_6gev/recon_c12_6gev.hipo";
//std::string AnalyseFile = "/home/alon/project/recon_c12_6gev_9_torus-1.0.hipo";
//std::string AnalyseFile = "/mnt/d/e4nu/root_data_files/Fixing SuSAv2/Julia's script/e_on_1000060120_2222MeV_em.gst.root";
//std::string AnalyseFile = "12C_2222GeV_GEM21_11a_00_000_wfsi_mk2.root";

std::string AnalyseFilePath = "mnt/d/e4nu/hipo_data_files";
//std::string AnalyseFilePath = "/mnt/d/e4nu/hipo_data_files/";
//std::string AnalyseFileSample = "recon_c12_6gev";
std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4/test";
//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4";
//std::string AnalyseFileSample = "/recon_c12_6gev/";
std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";

//std::string AnalyseFileDir = "/home/alon/project/temp/";

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
