//
// Created by alons on 08/07/2023.
//

#ifndef DEFFICIENCY_H
#define DEFFICIENCY_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

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

#include "../hData/hData.cpp"
//#include "../hPlots/hPlot1D.cpp"
#include "../hPlots/hPlot1D.h"
#include "../../functions/GeneralFunctions.h"
//#include "../../functions/EventProperties/GetParticleName.h"
//#include "../../functions/EventProperties/GetParticleNameLC.h"
//#include "../../functions/EventProperties/GetParticleNameShort.h"

using namespace std;

class DEfficiency : public hData {
private:

    bool weighted_plots = true;

//    TList *ACorr_data = new TList();

    /* Loading histograms */
    TH1D *Histogram1D_REC, *RPlot_Clone, *RPlot_Clone_test, *RPlot_Clone_test_rebined;
    string RPlot_Clone_StatsTitle, RPlot_Clone_test_StatsTitle, RPlot_Clone_test_rebined_StatsTitle;

    TH1D *Histogram1D_Truth, *TLPlot_Clone, *TLPlot_Clone_test, *TLPlot_Clone_test_rebined;
    string TLPlot_Clone_StatsTitle, TLPlot_Clone_test_StatsTitle, TLPlot_Clone_test_rebined_StatsTitle;

    string TLPlot_SaveNamePath, RPlot_SaveNamePath;

public:

// constructors ---------------------------------------------------------------------------------------------------------------------------------------------------------

    // Default constructor:
    DEfficiency() = default;

/*
// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleName(const string &Source);

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameLC(const string &Source);

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameShort(const string &Source);

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

    string GetType(const string &Source);

// GetTitle function -----------------------------------------------------------------------------------------------------------------------------------------------------

    string GetTitle(const string &Source);

// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

    string GetFS(const string &Source);

// SetXLabel function -------------------------------------------------------------------------------------------------------------------------------------------------------

    string SetXLabel(const string &Source);

// SetSaveDir function -------------------------------------------------------------------------------------------------------------------------------------------------------

    string SetSaveDir(const string &Source, const string &BaseSaveDir, const string &Mod = "");

*/

// ResetHistograms function ---------------------------------------------------------------------------------------------------------------------------------------------

    void ResetHistograms();

// LoadHistograms functions ---------------------------------------------------------------------------------------------------------------------------------------------

    void LoadHistograms(string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot);

    void LoadHistograms(string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot);

// DrawACorrHistograms function ----------------------------------------------------------------------------------------------------------------------------------------------

    void DrawACorrHistograms(bool save_ACorr_data, string &SampleName, TList *Histogram_list, TList *ACorr_data, string &ACorr_data_Dir);

// DrawAndSaveACorrPlots function ---------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveACorrPlots(bool save_ACorr_data, string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list, TList *ACorr_data,
                               string &ACorr_data_Dir);

    void DrawAndSaveACorrPlots(bool save_ACorr_data, string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot, TList *Histogram_list, TList *ACorr_data,
                               string &ACorr_data_Dir);

// DrawAndSaveHistogram1D function --------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveHistogram1D(TCanvas *HistCanvas, TList *Histogram_list, TH1D *Histogram1D, int LineStyle, int kColor, bool ShowStats, bool weighted_plots,
                                bool rebin_plots, const string &HistSaveDir);

};

#endif //DEFFICIENCY_H
