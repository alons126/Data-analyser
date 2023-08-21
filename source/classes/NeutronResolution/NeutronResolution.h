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

#include "../clas12ana/clas12ana.h"
#include "../hPlots/hPlot1D.h"
#include "../DSCuts/DSCuts.h"
#include "../../functions/GeneralFunctions.h"

using namespace std;

class NeutronResolution {
private:
    bool nResTestMode;

//    string SmearMode = "slices"; // Smear by resolution fit width values from slices
    string SmearMode = "pol1";     // Smear by fitted linear function to resolution Gaussian width
//    string SmearMode = "pol3";   // Smear by fitted 3rd degree polynomial function to resolution Gaussian width
//    string ShiftMode = "slices"; // Shift by resolution fit width values from slices
    string ShiftMode = "pol1";     // Shift by fitted linear function to resolution Gaussian mean
//    string ShiftMode = "pol3";   // Shift by fitted 3rd degree polynomial function to resolution Gaussian mean


    vector<hPlot1D> ResSlices;
    vector<vector<double>> ResSlicesLimits;
    vector<DSCuts> ResSlicesFitVar;
    vector<DSCuts> ResSlicesHistVar;
    vector<DSCuts> Loaded_Res_Slices_FitVar;
    vector<DSCuts> Loaded_Res_Slices_HistVar;
    vector<int> FittedSlices;

    double SliceUpperMomLim; // upper lim for momentum slices to be set after neutron upper momentum th.

    double hSliceUpperLim = 1.5;
    double hSliceLowerLim = -1.5;
//    double hSliceUpperLim = 3.;
//    double hSliceLowerLim = -3.;
    int hSliceNumOfBin = 65;

    TList *ResSlicePlots = new TList();
    TFolder *FittedNeutronResSlices = new TFolder("Fitted neutron resolution slices", "Fitted neutron resolution slices");

    TRandom3 *Rand = new TRandom3();

    string SlicesSavePath;
    string SlicesSaveNamePrefix;

    double delta;
    double deltaFactor = 1.;
    int NumberOfSlices = 0;

    double Neutron_Momentum_cut = 9999.; // from clas12ana
    string SName;

public:

// default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution() = default;

// constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution(const string &SampleName, const string &NucleonCutsDirectory, const string &Particle, double beamE, double nMomTh, const string &SavePath = "./",
                      double DeltaSlices = 0.2, bool VaryingDelta = false, const string &SmearM = "pol1", const string &ShiftM = "pol1", bool nRes_test = false);

// ReadInputParam function ----------------------------------------------------------------------------------------------------------------------------------------------

    void ReadInputParam(const char *filename);

// SetUpperMomCut function ----------------------------------------------------------------------------------------------------------------------------------------------

    void SetUpperMomCut(const string &SampleName, const string &NucleonCutsDirectory);

// hFillResPlots function -----------------------------------------------------------------------------------------------------------------------------------------------

    void hFillResPlots(double Momentum, double Resolution, double Weight);

// SliceFitDrawAndSave function -----------------------------------------------------------------------------------------------------------------------------------------

    void SliceFitDrawAndSave(const string &SampleName, const string &Particle, double beamE);

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveResSlices(const string &SampleName, const string &Particle, TCanvas *h1DCanvas, const string &plots_path, const string &NeutronResolutionDirectory);

// LogResDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogResDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status);

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogFitDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status);

// LogHistDataToFile function -------------------------------------------------------------------------------------------------------------------------------------------

    void LogHistDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                           const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status);

// ReadResDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

    void ReadResDataParam(const char *filename, const string &SampleName, const string &NucleonCutsDirectory);

// PSmear function ------------------------------------------------------------------------------------------------------------------------------------------------------

    double PSmear(bool apply_nucleon_SmearAndShift, double Momentum);

// NShift function ------------------------------------------------------------------------------------------------------------------------------------------------------

    double NShift(bool apply_nucleon_SmearAndShift, double Momentum);

// Other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Set functions
    void SetHistSliceUpperLim(double hsUpperLim) { hSliceUpperLim = hsUpperLim; };

    void SetHistSliceLowerLim(double hsLowerLim) { hSliceLowerLim = hsLowerLim; };

    void SetSmearMode(const string &SmearM) { SmearMode = SmearM; };

    void SetShiftMode(const string &ShiftM) { ShiftMode = ShiftM; };

    void SetSmearAndShiftModes(const string &SmearM, const string &ShiftM) { SmearMode = SmearM, ShiftMode = ShiftM; };

};

#endif //NEUTRONRESOLUTION_H
