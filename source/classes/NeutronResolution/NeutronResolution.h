//
// Created by alons on 20/06/2023.
//

#ifndef NEUTRONRESOLUTION_H
#define NEUTRONRESOLUTION_H

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

#include "../hPlots/hPlot1D.h"
#include "../DSCuts/DSCuts.h"
#include "../../functions/GeneralFunctions.h"

using namespace std;

class NeutronResolution {
private:
    vector<hPlot1D> ResSlices;
    vector<vector<double>> ResSlicesLimits;
    vector<DSCuts> ResSlicesFitVar;
    vector<DSCuts> Loaded_Res_Slices_FitVar;
    vector<int> FittedSlices;

    double hSliceUpperLim = 1.5;
    double hSliceLowerLim = -1.5;
//    double hSliceUpperLim = 3.;
//    double hSliceLowerLim = -3.;
    int hSliceNumOfBin = 65;

    TList *ResSlicePlots = new TList();
    TFolder *FittedNeutronResSlices = new TFolder("Fitted neutron resolution slices", "Fitted neutron resolution slices");

    string SlicesSavePath;
    string SlicesSaveNamePrefix;

    double delta;
    double deltaFactor = 1.;
    int NumberOfSlices = 0;

public:

// default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution() = default;

// constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution(const string &SampleName, const string &Particle, double beamE, double nMomTh, const string &SavePath = "./", double DeltaSlices = 0.2,
                      bool VaryingDelta = false);

// hFillResPlots function -----------------------------------------------------------------------------------------------------------------------------------------------

    void hFillResPlots(double Momentum, double Resolution, double Weight);

// SliceFitDrawAndSave function -----------------------------------------------------------------------------------------------------------------------------------------

    void SliceFitDrawAndSave(const string &SampleName, const string &Particle, double beamE);

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveResSlices(const string &SampleName, const string &Particle, TCanvas *h1DCanvas, const string &plots_path, const string &CutsDirectory);

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogFitDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &CutsDirectory,
                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status);

// ReadFitDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

    void ReadFitDataParam(const char *filename);

// PSmear function ------------------------------------------------------------------------------------------------------------------------------------------------------

    double PSmear(bool apply_nucleon_SmearAndShift, double Momentum);

// NShift function ------------------------------------------------------------------------------------------------------------------------------------------------------

    double NShift(bool apply_nucleon_SmearAndShift, double Momentum);

// Other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Set functions
    void SethSliceUpperLim(double hsUpperLim) { hSliceUpperLim = hsUpperLim; };

    void SethSliceLowerLim(double hsLowerLim) { hSliceLowerLim = hsLowerLim; };

};

#endif //NEUTRONRESOLUTION_H
