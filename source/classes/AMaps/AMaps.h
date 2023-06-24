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

    vector<hPlot2D> ElectronTLBinHitMaps, ProtonTLBinHitMaps;
    hPlot2D NeutronTLHitMap;

    vector<hPlot2D> ElectronRecoBinHitMaps, ProtonRecoBinHitMaps;
    hPlot2D NeutronRecoHitMap;

    int NumberOfPBins;

    double hBinLowerXLim = -180;
    double hBinUpperXLim = 180;
    double hBinLowerYLim = 0;
    double hBinUpperYLim = 50;

    int hBinNumOfXBins = 250;
    int hBinNumOfYBins = 250;

    TList *RefrenceHitMapsPlots = new TList();
    TList *TLHitMaps = new TList();
    TList *RecoHitMaps = new TList();
    TList *DivHitMaps = new TList();
//    TFolder *TLHitMaps = new TFolder("TL_hit_maps", "TL_hit_maps");
//    TFolder *RecoHitMaps = new TFolder("Reco_hit_maps", "Reco_hit_maps");
//    TFolder *DivHitMaps = new TFolder("Div_Hit_Maps", "Div_Hit_Maps");

    string BinSavePath;
    string BinSaveNamePrefix;

public:

// default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    AMaps() = default;

// constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    AMaps(double beamE, const string &SavePath = "./", double NumberOfBins = 50);

// SetBins function -----------------------------------------------------------------------------------------------------------------------------------------------------

    void SetBins(double beamE, double NumberOfBins);

// isElectron function -----------------------------------------------------------------------------------------------------------------------------------------------

    bool isElectron(const string &SampleType);

// isProton function -----------------------------------------------------------------------------------------------------------------------------------------------

    bool isProton(const string &SampleType);

// isNeutron function -----------------------------------------------------------------------------------------------------------------------------------------------

    bool isNeutron(const string &SampleType);

// isTL function -----------------------------------------------------------------------------------------------------------------------------------------------

    bool isTL(const string &SampleType);

// hFillHitMaps function -----------------------------------------------------------------------------------------------------------------------------------------------

    void hFillHitMaps(const string &SampleType, const string &particle, double Momentum, double Theta, double Phi, double Weight);

// DrawAndSaveHitMaps function ----------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveHitMaps(const string &SampleName, TCanvas *h1DCanvas, const string &CutsDirectory);

//// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------
//
//    void LogFitDataToFile(const string &SampleName, const string &plots_path, const string &CutsDirectory, const string &Nucleon_Cuts_Status,
//                          const string &FD_photons_Status, const string &Efficiency_Status);
//
//// ReadFitDataParam function --------------------------------------------------------------------------------------------------------------------------------------------
//
//    void ReadFitDataParam(const char *filename);
//
//// PSmear function --------------------------------------------------------------------------------------------------------------------------------------------
//
//    double PSmear(bool apply_proton_smearing, double Momentum);

};


#endif //AMAPS_H
