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
#include <string>

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

    vector<vector<double>> ElectronInvertedMomBinsLimits, ElectronMomBinsLimits;
    vector<vector<double>> InvertedPBinsLimits, PBinsLimits;

    double hBinLowerXLim = -180, hBinUpperXLim = 180;
    double hBinLowerYLim = 0, hBinUpperYLim = 50;

    int HistElectronSliceNumOfXBins;  // 100 by Default
    int HistElectronSliceNumOfYBins;  // 100 by Default
    int HistNucSliceNumOfXBins;       // 75 by Default
    int HistNucSliceNumOfYBins;       // 75 by Default
    int NumberNucOfMomSlices;         // 10 by Default

    double MomBinTh = 0.4;

    /* TL Acceptance maps */
    vector<hPlot2D> ElectronTLHitMapsBySlice, ProtonTLHitMapsBySlice, NeutronTLHitMapsBySlice;
    hPlot2D NeutronTLHitMap;

    /* Reco. Acceptance maps */
    vector<hPlot2D> ElectronRecoHitMapsBySlice, ProtonRecoHitMapsBySlice, NeutronRecoHitMapsBySlice;
    hPlot2D NeutronRecoHitMap;

    /* Reco./TL ratio maps */
    vector<hPlot2D> ElectronRecoToTLRatioBySlice, ProtonRecoToTLRatioBySlice, NeutronRecoToTLRatioBySlice;
    hPlot2D NeutronRecoToTLRatio;

    /* Separated acceptance maps (= reco. above ration th.) */
    vector<hPlot2D> ElectronAMapsBySlice, ProtonAMapsBySlice, NeutronAMapsBySlice; // separated AMaps for each slice

    /* Acceptance maps (= reco. above ration th.) */
    hPlot2D ElectronAMap; // combined electron AMap
    hPlot2D ProtonAMap;   // combined proton AMap
    hPlot2D NeutronAMap;
    hPlot2D NucleonAMap;

    vector<vector<int>> e_Hit_Map;
    vector<vector<vector<int>>> e_Hit_Map_Slices;
    vector<vector<int>> p_Hit_Map;
    vector<vector<vector<int>>> p_Hit_Map_Slices;
    vector<vector<int>> n_Hit_Map;
    vector<vector<vector<int>>> n_Hit_Map_Slices;
    vector<vector<int>> nuc_Hit_Map;
    vector<vector<vector<int>>> nuc_Hit_Map_Slices;

    bool calc_Electron_RecoToTL_Ratio = true, calc_Proton_RecoToTL_Ratio = true, calc_Neutron_RecoToTL_Ratio = true;

//    double Charged_particle_min_Ratio = 0.;
//    double Neutral_particle_min_Ratio = 0.;
    double Charged_particle_min_Ratio = 0.7;
//    double Neutral_particle_min_Ratio = 0.2;
//    double Neutral_particle_min_Ratio = 0.05;
//    double Neutral_particle_min_Ratio = 0.25;
    double Neutral_particle_min_Ratio = 0.3;

    TList *AcceptanceMapsBC = new TList();
    TList *TLHitMaps = new TList();
    TList *RecoHitMaps = new TList();
    TList *HitMapsRatio = new TList();
    TList *Charged_particle_Sep_AMaps = new TList();
    TList *AcceptanceMaps = new TList();

    string HitMapSavePath;

    string AMapsBC_prefix = "00_AMaps_BC_-_";
    string AMap_TL_prefix = "01_AMap_TL_-_";
    string AMap_Reco_prefix = "02_AMap_Reco_-_";
    string AMap_Ratio_prefix = "03_AMap_Ratio_-_";
    string cPart_Sep_AMaps_prefix = "04_cPart_Sep_AMaps_-_";
    string AMaps_prefix = "05_AMaps_-_";

    vector<vector<double>> Loaded_ElectronMomBinsLimits;
    vector<vector<double>> Loaded_PBinsLimits;

    vector<vector<int>> Loaded_e_Hit_Map;
    vector<vector<vector<int>>> Loaded_e_Hit_Map_Slices;
    vector<vector<int>> Loaded_p_Hit_Map;
    vector<vector<vector<int>>> Loaded_p_Hit_Map_Slices;
    vector<vector<int>> Loaded_n_Hit_Map;
    vector<vector<vector<int>>> Loaded_n_Hit_Map_Slices;
    vector<vector<int>> Loaded_nuc_Hit_Map;
    vector<vector<vector<int>>> Loaded_nuc_Hit_Map_Slices;

    /* Loaded Acceptance maps */
    //TODO: delete these histograms if the .par loading works
    vector<TH2 *> LoadedElectronAMaps, LoadedProtonAMaps;   // separated AMaps for each bin
    TH2D *LoadedElectronAMaps0;
    TH2D *LoadedProtonAMap;
    TH2D *LoadedNeutronAMap;
    TH2D *LoadedNucleonAMap;

public:

// default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    AMaps() = default;

// constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    // AMaps generation constructor:
    AMaps(bool reformat_e_bins, bool equi_P_e_bins, double beamE, const string &SavePath = "./", int nOfMomBins = 4, int hnsNumOfXBins = 75, int hnsNumOfYBins = 75,
          int hesNumOfXBins = 100, int hesNumOfYBins = 100);

    // AMaps loading constructor:
    AMaps(const string &AcceptanceMapsDirectory, const string &SampleName);

// SetBins functions ----------------------------------------------------------------------------------------------------------------------------------------------------

    void SetBins(double beamE);

    void SetElectronBins(bool reformat_e_bins, bool equi_P_e_bins, double beamE);

    void SetBins(double beamE, double NumberNucOfMomSlices); // old

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

// SaveHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

    void SaveHitMaps(const string &SampleName, const string &AcceptanceMapsDirectory);

// DrawAndSaveHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveHitMaps(const string &SampleName, TCanvas *h1DCanvas, const string &AcceptanceMapsDirectory);

// HistCounter function -------------------------------------------------------------------------------------------------------------------------------------------------

    int HistCounter(const char *fname);

// SetHistBinsFromHistTitle function ------------------------------------------------------------------------------------------------------------------------------------

    void SetHistBinsFromHistTitle(TH2D *Histogram2D);

// SetSlicesFromHistTitle function --------------------------------------------------------------------------------------------------------------------------------------

    void SetSlicesFromHistTitle(TH2D *Histogram2D, const string &Particle);

    void SetSlicesFromHistTitle(TH2D *Histogram2D, vector<vector<double>> MomBinsLimits);

// ReadHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadHitMaps(const string &AcceptanceMapsDirectory, const string &SampleName);

// ReadAMapLimits function ----------------------------------------------------------------------------------------------------------------------------------------------

    void ReadAMapLimits(const char *filename, vector<vector<double>> &Loaded_particle_limits);

// ReadAMapSlices function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadAMapSlices(const string &SampleName, const string &AcceptanceMapsDirectory, const string &Particle,
                        const vector<vector<double>> &Loaded_particle_limits, vector<vector<vector<int>>> &Loaded_Particle_Hit_Map_Slices);

// ReadAMap function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadAMap(const char *filename, vector<vector<int>> &Loaded_particle_hit_map);

// MatchAngToHitMap function --------------------------------------------------------------------------------------------------------------------------------------------

    bool MatchAngToHitMap(const string &Particle, double Momentum, double Theta, double Phi);

// IsInFDQuery function -------------------------------------------------------------------------------------------------------------------------------------------------

    bool IsInFDQuery(bool generate_AMaps, const DSCuts &ThetaFD, const string &Particle, double Momentum, double Theta, double Phi);

// Other methods --------------------------------------------------------------------------------------------------------------------------------------------------------

    // Set methods
    void SetNPartMinRatio(double npmr) { Neutral_particle_min_Ratio = npmr; }

    void SetCPartMinRatio(double cpmr) { Charged_particle_min_Ratio = cpmr; }

    // Get methods
    double GetNPartMinRatio() { return Neutral_particle_min_Ratio; }

    double GetCPartMinRatio() { return Charged_particle_min_Ratio; }

    double GetPBinsLimitsSize() { return PBinsLimits.size(); }

    double GetElectronPBinsLimitsSize() { return ElectronMomBinsLimits.size(); }

};


#endif //AMAPS_H
