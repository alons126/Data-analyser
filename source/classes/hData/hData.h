//
// Created by alons on 09/07/2023.
//

#ifndef hData_H
#define hData_H

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
public:

// constructors ---------------------------------------------------------------------------------------------------------------------------------------------------------

// Default constructor:
    hData() = default;

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleName(const string &Source);

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameLC(const string &Source);

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameShort(const string &Source);

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

    string GetType(const string &Source);

// GetTitle function ----------------------------------------------------------------------------------------------------------------------------------------------------

    string GetTitle(const string &Source);

// GetStatsTitle function -----------------------------------------------------------------------------------------------------------------------------------------------

    string GetStatsTitle(const string &Source);

// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

    string GetFS(const string &Source);

// SetXLabel function ---------------------------------------------------------------------------------------------------------------------------------------------------

    string SetXLabel(const string &Source);

// SetSaveDir function --------------------------------------------------------------------------------------------------------------------------------------------------

    string SetSaveDir(const string &Source, const string &BaseSaveDir, const string &Mod = "");

};

#endif //hData_H
