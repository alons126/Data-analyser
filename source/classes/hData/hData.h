//
// Created by alons on 09/07/2023.
//

#ifndef HDATA_H
#define HDATA_H

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

using namespace std;

class hData {
private:
    int StandardCanvasWidth = 1000, StandardCanvasHeight = 750;
    double BottomMargin = 0.14, LeftMargin = 0.16, RightMargin = 0.16, TopMargin = 0.12;

public:

// constructors ---------------------------------------------------------------------------------------------------------------------------------------------------------

// Default constructor:
    hData() = default;

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleName(const string &Source, const bool &PluralParticles = false);

    string GetParticleName1(const string &Source, const bool &PluralParticles = false);

// GetParticleNameFromSubscript function --------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameFromSubscript(const string &Source, const bool &PluralParticles = false);

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameLC(const string &Source, const bool &PluralParticles = false);

// GetParticleNameLCFromSubscript function ------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameLCFromSubscript(const string &Source, const bool &PluralParticles = false);

// GetParticleNameShort function ----------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameShort(const string &Source);

// GetParticleNameShortFromSubscript function ---------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameShortFromSubscript(const string &Source);

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

    string GetType(const string &Source);

// GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

    string GetDRegionExplicit(const string &Source, const bool &ReturnGoingRegion = false);

// GetDRegion function --------------------------------------------------------------------------------------------------------------------------------------------------

    string GetDRegion(const string &Source, const bool &ReturnGoingRegion = false);

// GetFSRTitle function -------------------------------------------------------------------------------------------------------------------------------------------------

    string GetFSRTitle(const string &Source, const string &PlotsT);

// GetTitle function ----------------------------------------------------------------------------------------------------------------------------------------------------

    string GetTitle(const string &Source);

// GetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

    string GetStatsTitle(const string &Source);

// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

    string GetFS(const string &Source);

// GetTopology function -------------------------------------------------------------------------------------------------------------------------------------------------

    string GetTopology(const string &Source);

// Other get function ---------------------------------------------------------------------------------------------------------------------------------------------------

    int GetStandardCanvasWidth() { return StandardCanvasWidth; }

    int GetStandardCanvasHeight() { return StandardCanvasHeight; }

    int GetBottomMargin() { return BottomMargin; }

    int GetLeftMargin() { return LeftMargin; }

    int GetRightMargin() { return RightMargin; }

    int GetTopMargin() { return TopMargin; }

// SetXLabel function ---------------------------------------------------------------------------------------------------------------------------------------------------

    string SetXLabel(const string &Source);

// SetSaveDir function --------------------------------------------------------------------------------------------------------------------------------------------------

    string SetSaveDir(const string &Source, const string &BaseSaveDir, const string &Mod = "");

// ReplaceSubStr function -----------------------------------------------------------------------------------------------------------------------------------------------

    void ReplaceSubStr(string &str, const string &subStr, const string &replacement);

};

#endif //HDATA_H
