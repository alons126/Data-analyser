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
    vector<vector<double>> PBinsLimits;

    int NumberOfPBins;

    double hBinLowerXLim = -180;
    double hBinUpperXLim = 180;
    double hBinLowerYLim = 0;
    double hBinUpperYLim = 50;

//    int hBinNumOfXBins = 65;
//    int hBinNumOfYBins = 65;
    int hBinNumOfXBins = 100;
    int hBinNumOfYBins = 100;
//    int hBinNumOfXBins = 150;
//    int hBinNumOfYBins = 150;
//    int hBinNumOfXBins = 200;
//    int hBinNumOfYBins = 200;
//    int hBinNumOfXBins = 250;
//    int hBinNumOfYBins = 250;

    vector<hPlot2D> ElectronTLBinHitMaps;
    vector<hPlot2D> ProtonTLBinHitMaps;
    hPlot2D NeutronTLHitMap;

    vector<hPlot2D> ElectronRecoBinHitMaps;
    vector<hPlot2D> ProtonRecoBinHitMaps;
    hPlot2D NeutronRecoHitMap;

    vector<hPlot2D> ElectronRecoToTLRatio;
    vector<hPlot2D> ProtonRecoToTLRatio;
    hPlot2D NeutronRecoToTLRatio;

    vector<hPlot2D> ElectronSepAMaps; // separated electron AMaps for each bin
    vector<hPlot2D> ProtonSepAMaps;   // separated electron AMaps for each bin

    hPlot2D ElectronAMap; // combined electron AMap
    hPlot2D ProtonAMap;   // combined proton AMap
    hPlot2D NeutronAMap;
    hPlot2D NucleonAMap;

    bool calc_Electron_RecoToTL_Ratio = true;
    bool calc_Proton_RecoToTL_Ratio = true;
    bool calc_Neutron_RecoToTL_Ratio = true;

    double Charged_particle_min_Ration = 0.7;
    double Neutral_particle_min_Ration = 0.3;

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

    AMaps(double beamE, const string &SavePath = "./", double NumberOfBins = 50);

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

//// LogFitDataToFile function ------------------------------------------------------------------------------------------------------------------------------------------
//
//    void LogFitDataToFile(const string &SampleName, const string &plots_path, const string &CutsDirectory, const string &Nucleon_Cuts_Status,
//                          const string &FD_photons_Status, const string &Efficiency_Status);
//
//// ReadFitDataParam function ------------------------------------------------------------------------------------------------------------------------------------------
//
//    void ReadFitDataParam(const char *filename);
//
//// PSmear function ----------------------------------------------------------------------------------------------------------------------------------------------------
//
//    double PSmear(bool apply_proton_smearing, double Momentum);

};


#endif //AMAPS_H
