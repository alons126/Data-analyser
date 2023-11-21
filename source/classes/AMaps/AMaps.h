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
    vector<hPlot2D> ElectronTLAMapsBySlice, ProtonTLAMapsBySlice, NeutronTLAMapsBySlice;
    hPlot2D NeutronTLAMap;

    /* Reco. Acceptance maps */
    vector<hPlot2D> ElectronRecoAMapsBySlice, ProtonRecoAMapsBySlice, NeutronRecoAMapsBySlice;
    hPlot2D NeutronRecoAMap;

    /* Reco./TL ratio maps */
    vector<hPlot2D> ElectronRecoToTLRatioBySlice, ProtonRecoToTLRatioBySlice, NeutronRecoToTLRatioBySlice;
    hPlot2D NeutronRecoToTLRatio;

    /* Separated acceptance & weight maps (= reco. above ratio th.) */
    vector<hPlot2D> ElectronAMapsBySlice, ProtonAMapsBySlice, NeutronAMapsBySlice; // separated AMaps for each slice

    /* Acceptance maps (= reco. above ratio th.) */
    hPlot2D ElectronAMap; // combined electron AMap
    hPlot2D ProtonAMap;   // combined proton AMap
    hPlot2D NeutronAMap;
    hPlot2D NucleonAMap;

    /* Acceptance maps */
    vector<vector<int>> e_AMap;
    vector<vector<vector<int>>> e_AMap_Slices;
    vector<vector<int>> p_AMap;
    vector<vector<vector<int>>> p_AMap_Slices;
    vector<vector<int>> n_AMap;
    vector<vector<vector<int>>> n_AMap_Slices;
    vector<vector<int>> nuc_AMap;
    vector<vector<vector<int>>> nuc_AMap_Slices;

    /* Weight maps */
    vector<vector<vector<double>>> e_WMap_Slices;
    vector<vector<vector<double>>> p_WMap_Slices;
    vector<vector<double>> n_WMap;
    vector<vector<vector<double>>> n_WMap_Slices;

    bool calc_Electron_RecoToTL_Ratio = true, calc_Proton_RecoToTL_Ratio = true, calc_Neutron_RecoToTL_Ratio = true;

    double Charged_particle_min_Ratio = 0.7;
    double Neutral_particle_min_Ratio = 0.4;
//    double Neutral_particle_min_Ratio = 0.2; // ORIGINAL!!!

    TList *AcceptanceMapsBC = new TList();
    TList *TLAMaps = new TList();
    TList *RecoAMaps = new TList();
    TList *AMapsRatio = new TList();
    TList *Charged_particle_Sep_AMaps = new TList();
    TList *AcceptanceMaps = new TList();

    string AMapSavePath;

    string AMapsBC_prefix = "00_AMaps_BC_-_";
    string AMap_TL_prefix = "01_AMap_TL_-_";
    string AMap_Reco_prefix = "02_AMap_Reco_-_";
    string AMap_Ratio_prefix = "03_AMap_Ratio_-_";
    string cPart_Sep_AMaps_prefix = "04_cPart_Sep_AMaps_-_";
    string AMaps_prefix = "05_AMaps_-_";

    vector<vector<double>> Loaded_ElectronMomBinsLimits;
    vector<vector<double>> Loaded_PBinsLimits;

    /* Loaded acceptance maps */
    vector<vector<int>> Loaded_e_AMap;
    vector<vector<vector<int>>> Loaded_e_AMap_Slices;
    vector<vector<int>> Loaded_p_AMap;
    vector<vector<vector<int>>> Loaded_p_AMap_Slices;
    vector<vector<int>> Loaded_n_AMap;
    vector<vector<vector<int>>> Loaded_n_AMap_Slices;
    vector<vector<int>> Loaded_nuc_AMap;
    vector<vector<vector<int>>> Loaded_nuc_AMap_Slices;

    /* Loaded weight maps */
    vector<vector<vector<double>>> Loaded_e_WMap_Slices;
    vector<vector<vector<double>>> Loaded_p_WMap_Slices;
    vector<vector<int>> Loaded_n_WMap;
    vector<vector<vector<double>>> Loaded_n_WMap_Slices;

    /* Loaded Acceptance maps */
    //TODO: delete these histograms if the .par loading works
    vector<TH2 *> LoadedElectronAMaps, LoadedProtonAMaps;   // separated AMaps for each bin
    TH2D *LoadedElectronAMaps0;
    TH2D *LoadedProtonAMap;
    TH2D *LoadedNeutronAMap;
    TH2D *LoadedNucleonAMap;

    string SName;
public:

// default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    AMaps() = default;

// constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    // AMaps generation constructor:
    AMaps(const string &SampleName, bool reformat_e_bins, bool equi_P_e_bins, double beamE, const string &SavePath = "./",
          int nOfMomBins = 4, int hnsNumOfXBins = 75, int hnsNumOfYBins = 75, int hesNumOfXBins = 100, int hesNumOfYBins = 100);

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

// CalcAMapsRatio function --------------------------------------------------------------------------------------------------------------------------------------------

    void CalcAMapsRatio(bool ElectronRecoToTLDiv = true, bool PotonRecoToTLDiv = true, bool NeutronRecoToTLDiv = true);

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
                        const vector<vector<double>> &Loaded_particle_limits, vector<vector<vector<int>>> &Loaded_Particle_AMap_Slices);

    void ReadAMapSlices(const string &SampleName, const string &AcceptanceMapsDirectory, const string &Particle,
                        const vector<vector<double>> &Loaded_particle_limits, vector<vector<vector<double>>> &Loaded_Particle_WMap_Slices);

// ReadAMap function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void ReadAMap(const char *filename, vector<vector<int>> &Loaded_particle_AMap);

    void ReadAMap(const char *filename, vector<vector<double>> &Loaded_particle_WMap);

// MatchAngToHitMap function --------------------------------------------------------------------------------------------------------------------------------------------

    bool MatchAngToHitMap(const string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC = true);

// GetWeight function ---------------------------------------------------------------------------------------------------------------------------------------------------

    double GetWeight(bool apply_kinematical_weights, const string &Particle, double Momentum, double Theta, double Phi);

// IsInFDQuery function -------------------------------------------------------------------------------------------------------------------------------------------------

    bool IsInFDQuery(bool generate_AMaps, const DSCuts &ThetaFD, const string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC = true);

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
