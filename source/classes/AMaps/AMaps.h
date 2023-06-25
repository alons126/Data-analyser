//
// Created by alons on 22/06/2023.
//

#ifndef AMAPS_H
#define AMAPS_H

#include <iostream>
#include <fstream>
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

#include "clas12reader.h"

#include "../hPlots/hPlot2D.h"
#include "../DSCuts/DSCuts.h"
#include "../../functions/GeneralFunctions.h"

using namespace std;

class AMaps {
private:

    /* Acceptance maps from class and before cuts (to be compared with one generated with the file) */
    hPlot2D ElectronAMapBC, ProtonAMapBC, NeutronAMapBC, NucleonAMapBC;

    vector<vector<double>> PBinsLimits;

    double LowerBinMin = 0.3;

    int NumberOfPBins;

    double hBinLowerXLim = -180, hBinUpperXLim = 180;
    double hBinLowerYLim = 0, hBinUpperYLim = 50;

    int hBinNumOfXBins; // 100 by Default
    int hBinNumOfYBins; // 100 by Default

    /* TL hit maps */
    vector<hPlot2D> ElectronTLBinHitMaps, ProtonTLBinHitMaps;
    hPlot2D NeutronTLHitMap;

    /* Reco. hit maps */
    vector<hPlot2D> ElectronRecoBinHitMaps, ProtonRecoBinHitMaps;
    hPlot2D NeutronRecoHitMap;

    /* Reco./TL ratio maps */
    vector<hPlot2D> ElectronRecoToTLRatio, ProtonRecoToTLRatio;
    hPlot2D NeutronRecoToTLRatio;

    /* Separated acceptance maps (= reco. above ration th.) */
    vector<hPlot2D> ElectronSepAMaps, ProtonSepAMaps;   // separated AMaps for each bin

    /* Acceptance maps (= reco. above ration th.) */
    hPlot2D ElectronAMap; // combined electron AMap
    hPlot2D ProtonAMap;   // combined proton AMap
    hPlot2D NeutronAMap;
    hPlot2D NucleonAMap;

    bool calc_Electron_RecoToTL_Ratio = true, calc_Proton_RecoToTL_Ratio = true, calc_Neutron_RecoToTL_Ratio = true;

//    double Charged_particle_min_Ratio = 0.;
//    double Neutral_particle_min_Ratio = 0.;
    double Charged_particle_min_Ratio = 0.7;
    double Neutral_particle_min_Ratio = 0.3;

    TList *AcceptanceMapsBC = new TList();
    TList *TLHitMaps = new TList();
    TList *RecoHitMaps = new TList();
    TList *HitMapsRatio = new TList();
    TList *Charged_particle_Sep_AMaps = new TList();
    TList *AcceptanceMaps = new TList();

    string BinSavePath;

public:

// default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    AMaps() = default;

// constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    AMaps(double beamE, const string &SavePath = "./", double NumberOfBins = 50, int hbNumOfXBins = 100, int hbNumOfYBins = 100);

// SetBins function -----------------------------------------------------------------------------------------------------------------------------------------------------

    void SetBins(double beamE, double NumberOfBins);

// isElectron function --------------------------------------------------------------------------------------------------------------------------------------------------

    bool isElectron(const string &SampleType);

// isProton function ----------------------------------------------------------------------------------------------------------------------------------------------------

    bool isProton(const string &SampleType);

// isNeutron function ---------------------------------------------------------------------------------------------------------------------------------------------------

    bool isNeutron(const string &SampleType);

// isTL function --------------------------------------------------------------------------------------------------------------------------------------------------------

    bool isTL(const string &SampleType);

// hFillHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

    void hFillHitMaps(const string &SampleType, const string &particle, double Momentum, double Theta, double Phi, double Weight);

// CalcHitMapsRatio function --------------------------------------------------------------------------------------------------------------------------------------------

    void CalcHitMapsRatio(bool ElectronRecoToTLDiv = true, bool PotonRecoToTLDiv = true, bool NeutronRecoToTLDiv = true);

// GenerateSeparateCPartAMaps function ----------------------------------------------------------------------------------------------------------------------------------

    void GenerateSeparateCPartAMaps(double cP_minR);

// GenerateCPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

    void GenerateCPartAMaps(double cP_minR);

// GenerateNPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

    void GenerateNPartAMaps(double nP_minR);

// GenerateNucleonAMap function -----------------------------------------------------------------------------------------------------------------------------------------

    void GenerateNucleonAMap();

// DrawAndSaveHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveHitMaps(const string &SampleName, TCanvas *h1DCanvas, const string &RefrenceHitMapsDirectory);

//// ReadFitDataParam function ------------------------------------------------------------------------------------------------------------------------------------------
//
//    void ReadFitDataParam(const char *filename);
//
//// PSmear function ----------------------------------------------------------------------------------------------------------------------------------------------------
//
//    double PSmear(bool apply_proton_smearing, double Momentum);

// Other methods --------------------------------------------------------------------------------------------------------------------------------------------------------

    // Set methods
    void SetNeutralParticleMinRatio(double npmr) { Neutral_particle_min_Ratio = npmr; }

    void SetCargedParticleMinRatio(double cpmr) { Charged_particle_min_Ratio = cpmr; }

    // Get methods
    double GetNeutralParticleMinRatio() { return Neutral_particle_min_Ratio; }

    double GetCargedParticleMinRatio() { return Charged_particle_min_Ratio; }

};


#endif //AMAPS_H
