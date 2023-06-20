//
// Created by alons on 20/06/2023.
//

#ifndef NEUTRONRESOLUTION_H
#define NEUTRONRESOLUTION_H

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

#include "../hPlots/hPlot1D.h"
#include "../DSCuts/DSCuts.h"
#include "../../functions/GeneralFunctions.h"

using namespace std;

class NeutronResolution {
private:
    vector<hPlot1D> ResolutionSlices;
    vector<vector<double>> ResolutionSlicesLimits;
    vector<DSCuts> ResolutionSlicesFitVar;
    vector<DSCuts> LoadedResolutionSlicesFitVar;
    vector<int> FittedSlices;

    double hSliceUpperLim = 1.5;
    double hSliceLowerLim = -1.5;
//    double hSliceUpperLim = 3.;
//    double hSliceLowerLim = -3.;
    int hSliceNumOfBin = 65;

    TList *ResSlicePlots = new TList();
    TFolder *FittedNeutronResolutionSlices = new TFolder("Fitted neutron resolution slices", "Fitted neutron resolution slices");

    string SlicesSavePath;
    string SlicesSaveNamePrefix;

    double delta{};
    int NumberOfSlices = 0;

public:

// default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution() = default;

// constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution(double beamE, const string &SavePath = "./", double DeltaSlices = 0.1);

// hFillResPlots function -----------------------------------------------------------------------------------------------------------------------------------------------

    void hFillResPlots(double Momentum, double Resolution, double Weight);

// SliceFitDrawAndSave function -----------------------------------------------------------------------------------------------------------------------------------------

    void SliceFitDrawAndSave(const string &SampleName, double beamE);

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveResSlices(const string &SampleName, TCanvas *h1DCanvas, const string &plots_path, const string &CutsDirectory);

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogFitDataToFile(const string &SampleName, const string &plots_path, const string &CutsDirectory, const string &Nucleon_Cuts_Status,
                          const string &FD_photons_Status, const string &Efficiency_Status);

// ReadFitDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

    void ReadFitDataParam(const char *filename);

// ProtonSmear function --------------------------------------------------------------------------------------------------------------------------------------------

    double ProtonSmear(bool apply_proton_smearing, double Momentum);
};

#endif //NEUTRONRESOLUTION_H
