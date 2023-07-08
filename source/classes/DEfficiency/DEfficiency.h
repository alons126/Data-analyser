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

    TList *ACorr_data = new TList();

public:

// constructors ---------------------------------------------------------------------------------------------------------------------------------------------------------

    // Default constructor:
    DEfficiency() = default;

// GetParticleName function ---------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleName(string Source);

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameLC(string Source);

// GetParticleNameLC function -------------------------------------------------------------------------------------------------------------------------------------------

    string GetParticleNameShort(string Source);

// GetType function -----------------------------------------------------------------------------------------------------------------------------------------------------

    string GetType(string Source);

// GetFS function -------------------------------------------------------------------------------------------------------------------------------------------------------

    string GetFS(string Source);

};


#endif //DEFFICIENCY_H
