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

//#include "../clas12ana/clas12ana.h"
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


    vector <hPlot1D> ResSlices;
    vector <vector<double>> ResSlicesLimits;
    vector <DSCuts> ResSlicesFitVar;
    vector <DSCuts> ResSlicesHistVar;
    vector <DSCuts> Loaded_Res_Slices_FitVar;
    vector <DSCuts> Loaded_Res_Slices_HistVar;
    vector<int> FittedSlices;

    double SliceUpperMomLim; // upper lim for momentum slices - loaded from file

    double SliceUpperMomLimPC, SliceLowerMomLimPC; // lower lim for momentum slices - set by constructor

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

    //<editor-fold desc="Correction and smear fit variables">

    //TODO: add to vectors?

//    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);

//    TGraph *g_Corr_pol1, g_Corr_pol1_wPC;
//    TGraph *g_Corr_pol2, g_Corr_pol2_wPC;
//    TGraph *g_Corr_pol3, g_Corr_pol3_wPC;

//    TF1 *f_Corr_pol1, f_Corr_pol1_wPC; // A*x + B
//    TF1 *f_Corr_pol2, f_Corr_pol2_wPC; // A*x*x + B*x + C
//    TF1 *f_Corr_pol3, f_Corr_pol3_wPC; // A*x*x*x + B*x*x + C*x + D

    double A_Corr_pol1, A_Corr_pol1_Error, B_Corr_pol1, B_Corr_pol1_Error;
    double ChiSquare_Corr_pol1, NDF_Corr_pol1;
    double A_Corr_pol1_wPC, A_Corr_pol1_wPC_Error, B_Corr_pol1_wPC, B_Corr_pol1_wPC_Error;
    double ChiSquare_Corr_pol1_wPC, NDF_Corr_pol1_wPC;

    double A_Corr_pol2, A_Corr_pol2_Error, B_Corr_pol2, B_Corr_pol2_Error, C_Corr_pol2, C_Corr_pol2_Error;
    double ChiSquare_Corr_pol2, NDF_Corr_pol2;
    double A_Corr_pol2_wPC, A_Corr_pol2_wPC_Error, B_Corr_pol2_wPC, B_Corr_pol2_wPC_Error, C_Corr_pol2_wPC, C_Corr_pol2_wPC_Error;
    double ChiSquare_Corr_pol2_wPC, NDF_Corr_pol2_wPC;

    double A_Corr_pol3, A_Corr_pol3_Error, B_Corr_pol3, B_Corr_pol3_Error, C_Corr_pol3, C_Corr_pol3_Error, D_Corr_pol3, D_Corr_pol3_Error;
    double ChiSquare_Corr_pol3, NDF_Corr_pol3;
    double A_Corr_pol3_wPC, A_Corr_pol3_wPC_Error, B_Corr_pol3_wPC, B_Corr_pol3_wPC_Error, C_Corr_pol3_wPC, C_Corr_pol3_wPC_Error, D_Corr_pol3_wPC, D_Corr_pol3_wPC_Error;
    double ChiSquare_Corr_pol3_wPC, NDF_Corr_pol3_wPC;
    //</editor-fold>

    double Neutron_Momentum_cut = 9999.; // from clas12ana
    string SName;

public:

// default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution() = default;

// constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution(const string &SampleName, const string &NucleonCutsDirectory, const string &Particle, double beamE, const DSCuts &FD_nucleon_momentum_cut,
                      double nMomTh, const string &SavePath = "./", double DeltaSlices = 0.2, bool VaryingDelta = false, const string &SmearM = "pol1",
                      const string &ShiftM = "pol1", bool nRes_test = false);

// ReadInputParam function ----------------------------------------------------------------------------------------------------------------------------------------------

    void ReadInputParam(const char *filename);

// SetUpperMomCut function ----------------------------------------------------------------------------------------------------------------------------------------------

    void SetUpperMomCut(const string &SampleName, const string &NucleonCutsDirectory);

// hFillResPlots function -----------------------------------------------------------------------------------------------------------------------------------------------

    void hFillResPlots(double Momentum, double Resolution, double Weight);

// SliceFitDrawAndSave function -----------------------------------------------------------------------------------------------------------------------------------------

    void SliceFitDrawAndSave(const string &SampleName, const string &Particle, double beamE);

// Fitter functions -----------------------------------------------------------------------------------------------------------------------------------------------------

    void Fitter_Corr_pol1();

    void Fitter_Corr_pol2();

    void Fitter_Corr_pol3();

    void Fitter_Corr_pol1_wPC();

    void Fitter_Corr_pol2_wPC();

    void Fitter_Corr_pol3_wPC();

    void Fitter_Std_pol1();

    void Fitter_Std_pol2();

    void Fitter_Std_pol3();

    void Fitter_Std_pol1_wPC();

    void Fitter_Std_pol2_wPC();

    void Fitter_Std_pol3_wPC();

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

    double GetSliceUpperMomLim() { return SliceUpperMomLim; };

};

#endif //NEUTRONRESOLUTION_H
