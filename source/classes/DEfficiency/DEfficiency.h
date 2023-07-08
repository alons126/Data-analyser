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

#include "../../functions/GeneralFunctions.h"
//#include "../../functions/EventProperties/GetParticleName.h"
//#include "../../functions/EventProperties/GetParticleNameLC.h"
//#include "../../functions/EventProperties/GetParticleNameShort.h"
#include "../hPlots/hPlot1D.h"

using namespace std;

class DEfficiency {
private:

    bool weighted_plots = true;

    TList *ACorr_data = new TList();

public:

// constructors ---------------------------------------------------------------------------------------------------------------------------------------------------------

    // Default constructor:
    DEfficiency() = default;

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

// DrawAndSaveHistogram1D function -------------------------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveHistogram1D(TCanvas *HistCanvas, TList *Histogram_list, TH1D *Histogram1D, int LineStyle, int kColor, bool ShowStats, bool weighted_plots,
                                bool rebin_plots, const string &HistSaveDir);

// DrawAndSaveACorrPlots function -------------------------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveACorrPlots(bool save_ACorr_data, string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot,
                               TList *Histogram_list, TList *ACorr_data, string &ACorr_data_Dir);

};


#endif //DEFFICIENCY_H
