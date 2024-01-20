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
    bool isNeutron = false, isProton = false;
    string MomResParticle = "";

    bool momResTestMode, momResS2CalcMode, momResS2RunMode;
    bool ForceSmallProtonResLimits = false;

    string SmearMode = "NONE", CorrMode = "NONE";

    vector <hPlot1D> ResTLMomSlices, ResRecoMomSlices;
    vector <vector<double>> ResTLMomSlicesLimits, ResRecoMomSlicesLimits;
    vector <DSCuts> ResTLMomSlicesFitVar, ResRecoMomSlicesFitVar;
    vector <DSCuts> ResTLMomSlicesHistVar, ResRecoMomSlicesHistVar;
    vector <DSCuts> Loaded_Res_Slices_FitVar;
    vector <DSCuts> Loaded_Res_Slices_HistVar;
    vector<int> FittedTLMomSlices, FittedRecoMomSlices;

    double SliceUpperMomLim; // upper lim for momentum slices - loaded from file

    double SliceUpperMomLimKC, SliceLowerMomLimKC; // lower lim for momentum slices - set by constructor

    double hSliceUpperLim = 1.1;
    double hSliceLowerLim = -1.1;
    int hSliceNumOfBin = 65;

    TList *ResSlicePlots = new TList();
    TFolder *FittedNeutronResSlices = new TFolder("Fitted neutron resolution slices", "Fitted neutron resolution slices");
    TFolder *FittedNeutronResSlicesMean = new TFolder("Fitted neutron resolution slice mean", "Fitted neutron resolution slice mean");
    TFolder *FittedNeutronResSlicesWidth = new TFolder("Fitted neutron resolution slice width", "Fitted neutron resolution slice width");
    TFolder *FittedProtonResSlices = new TFolder("Fitted proton resolution slices", "Fitted proton resolution slices");
    TFolder *FittedProtonResSlicesMean = new TFolder("Fitted proton resolution slice mean", "Fitted proton resolution slice mean");
    TFolder *FittedProtonResSlicesWidth = new TFolder("Fitted proton resolution slice width", "Fitted proton resolution slice width");

    TRandom3 *Rand = new TRandom3();

    string SlicesSavePath;
    string SlicesSaveNamePrefix;

    double delta, deltaFactor = 1.;
    int TL_NumberOfSlices = 0, Reco_NumberOfSlices = 0;

    //<editor-fold desc="Correction and smear fit variables">
    vector <vector<double>> TL_FitParam_Smear_pol1, Reco_FitParam_Smear_pol1;
    // {{A_Smear_pol1, B_Smear_pol1},
    // {A_Smear_pol1_Error, B_Smear_pol1_Error},
    // {ChiSquare_Smear_pol1, NDF_Smear_pol1},}
    vector <vector<double>> TL_FitParam_Smear_pol1_wKC, Reco_FitParam_Smear_pol1_wKC;
    // {{A_Smear_pol1_wKC, B_Smear_pol1_wKC},
    // {A_Smear_pol1_wKC_Error, B_Smear_pol1_wKC_Error},
    // {ChiSquare_Smear_pol1_wKC, NDF_Smear_pol1_wKC},}

    vector <vector<double>> TL_FitParam_Smear_pol2, Reco_FitParam_Smear_pol2;
    // {{A_Smear_pol2, B_Smear_pol2, C_Smear_pol2},
    // {A_Smear_pol2_Error, B_Smear_pol2_Error, C_Smear_pol2_Error},
    // {ChiSquare_Smear_pol2, NDF_Smear_pol2},}
    vector <vector<double>> TL_FitParam_Smear_pol2_wKC, Reco_FitParam_Smear_pol2_wKC;
    // {{A_Smear_pol2_wKC, B_Smear_pol2_wKC, C_Smear_pol2_wKC},
    // {A_Smear_pol2_wKC_Error, B_Smear_pol2_wKC_Error, C_Smear_pol2_wKC_Error},
    // {ChiSquare_Smear_pol2_wKC, NDF_Smear_pol2_wKC},}

    vector <vector<double>> TL_FitParam_Smear_pol3, Reco_FitParam_Smear_pol3;
    // {{A_Smear_pol2, B_Smear_pol2, C_Smear_pol2, D_Smear_pol2},
    // {A_Smear_pol2_Error, B_Smear_pol2_Error, C_Smear_pol2_Error, D_Smear_pol2_Error},
    // {ChiSquare_Smear_pol2, NDF_Smear_pol2},}
    vector <vector<double>> TL_FitParam_Smear_pol3_wKC, Reco_FitParam_Smear_pol3_wKC;
    // {{A_Smear_pol2_wKC, B_Smear_pol2_wKC, C_Smear_pol2_wKC, D_Smear_pol2_wKC},
    // {A_Smear_pol2_wKC_Error, B_Smear_pol2_wKC_Error, C_Smear_pol2_wKC_Error, D_Smear_pol2_wKC_Error},
    // {ChiSquare_Smear_pol2_wKC, NDF_Smear_pol2_wKC},}

    vector <vector<double>> TL_FitParam_Corr_pol1, Reco_FitParam_Corr_pol1;
    // {{A_Corr_pol1, B_Corr_pol1},
    // {A_Corr_pol1_Error, B_Corr_pol1_Error},
    // {ChiSquare_Corr_pol1, NDF_Corr_pol1},}
    vector <vector<double>> TL_FitParam_Corr_pol1_wKC, Reco_FitParam_Corr_pol1_wKC;
    // {{A_Corr_pol1_wKC, B_Corr_pol1_wKC},
    // {A_Corr_pol1_wKC_Error, B_Corr_pol1_wKC_Error},
    // {ChiSquare_Corr_pol1_wKC, NDF_Corr_pol1_wKC},}

    vector <vector<double>> TL_FitParam_Corr_pol2, Reco_FitParam_Corr_pol2;
    // {{A_Corr_pol2, B_Corr_pol2, C_Corr_pol2},
    // {A_Corr_pol2_Error, B_Corr_pol2_Error, C_Corr_pol2_Error},
    // {ChiSquare_Corr_pol2, NDF_Corr_pol2},}
    vector <vector<double>> TL_FitParam_Corr_pol2_wKC, Reco_FitParam_Corr_pol2_wKC;
    // {{A_Corr_pol2_wKC, B_Corr_pol2_wKC, C_Corr_pol2_wKC},
    // {A_Corr_pol2_wKC_Error, B_Corr_pol2_wKC_Error, C_Corr_pol2_wKC_Error},
    // {ChiSquare_Corr_pol2_wKC, NDF_Corr_pol2_wKC},}

    vector <vector<double>> TL_FitParam_Corr_pol3, Reco_FitParam_Corr_pol3;
    // {{A_Corr_pol2, B_Corr_pol2, C_Corr_pol2, D_Corr_pol2},
    // {A_Corr_pol2_Error, B_Corr_pol2_Error, C_Corr_pol2_Error, D_Corr_pol2_Error},
    // {ChiSquare_Corr_pol2, NDF_Corr_pol2},}
    vector <vector<double>> TL_FitParam_Corr_pol3_wKC, Reco_FitParam_Corr_pol3_wKC;
    // {{A_Corr_pol2_wKC, B_Corr_pol2_wKC, C_Corr_pol2_wKC, D_Corr_pol2_wKC},
    // {A_Corr_pol2_wKC_Error, B_Corr_pol2_wKC_Error, C_Corr_pol2_wKC_Error, D_Corr_pol2_wKC_Error},
    // {ChiSquare_Corr_pol2_wKC, NDF_Corr_pol2_wKC},}


    //TODO: add to vectors?

    double A_Corr_pol1, A_Corr_pol1_Error, B_Corr_pol1, B_Corr_pol1_Error;
    double ChiSquare_Corr_pol1, NDF_Corr_pol1;
    double A_Corr_pol1_wKC, A_Corr_pol1_wKC_Error, B_Corr_pol1_wKC, B_Corr_pol1_wKC_Error;
    double ChiSquare_Corr_pol1_wKC, NDF_Corr_pol1_wKC;

    double A_Corr_pol2, A_Corr_pol2_Error, B_Corr_pol2, B_Corr_pol2_Error, C_Corr_pol2, C_Corr_pol2_Error;
    double ChiSquare_Corr_pol2, NDF_Corr_pol2;
    double A_Corr_pol2_wKC, A_Corr_pol2_wKC_Error, B_Corr_pol2_wKC, B_Corr_pol2_wKC_Error, C_Corr_pol2_wKC, C_Corr_pol2_wKC_Error;
    double ChiSquare_Corr_pol2_wKC, NDF_Corr_pol2_wKC;

    double A_Corr_pol3, A_Corr_pol3_Error, B_Corr_pol3, B_Corr_pol3_Error, C_Corr_pol3, C_Corr_pol3_Error, D_Corr_pol3, D_Corr_pol3_Error;
    double ChiSquare_Corr_pol3, NDF_Corr_pol3;
    double A_Corr_pol3_wKC, A_Corr_pol3_wKC_Error, B_Corr_pol3_wKC, B_Corr_pol3_wKC_Error, C_Corr_pol3_wKC, C_Corr_pol3_wKC_Error, D_Corr_pol3_wKC, D_Corr_pol3_wKC_Error;
    double ChiSquare_Corr_pol3_wKC, NDF_Corr_pol3_wKC;

    double A_Std_pol1, A_Std_pol1_Error, B_Std_pol1, B_Std_pol1_Error;
    double ChiSquare_Std_pol1, NDF_Std_pol1;
    double A_Std_pol1_wKC, A_Std_pol1_wKC_Error, B_Std_pol1_wKC, B_Std_pol1_wKC_Error;
    double ChiSquare_Std_pol1_wKC, NDF_Std_pol1_wKC;

    double A_Std_pol2, A_Std_pol2_Error, B_Std_pol2, B_Std_pol2_Error, C_Std_pol2, C_Std_pol2_Error;
    double ChiSquare_Std_pol2, NDF_Std_pol2;
    double A_Std_pol2_wKC, A_Std_pol2_wKC_Error, B_Std_pol2_wKC, B_Std_pol2_wKC_Error, C_Std_pol2_wKC, C_Std_pol2_wKC_Error;
    double ChiSquare_Std_pol2_wKC, NDF_Std_pol2_wKC;

    double A_Std_pol3, A_Std_pol3_Error, B_Std_pol3, B_Std_pol3_Error, C_Std_pol3, C_Std_pol3_Error, D_Std_pol3, D_Std_pol3_Error;
    double ChiSquare_Std_pol3, NDF_Std_pol3;
    double A_Std_pol3_wKC, A_Std_pol3_wKC_Error, B_Std_pol3_wKC, B_Std_pol3_wKC_Error, C_Std_pol3_wKC, C_Std_pol3_wKC_Error, D_Std_pol3_wKC, D_Std_pol3_wKC_Error;
    double ChiSquare_Std_pol3_wKC, NDF_Std_pol3_wKC;
    //</editor-fold>

    //<editor-fold desc="Loaded correction and smear fit variables">
    double Loaded_A_Std_pol1, Loaded_B_Std_pol1;
    double Loaded_A_Std_pol1_Error, Loaded_B_Std_pol1_Error;
    double Loaded_ChiSquare_Std_pol1, Loaded_NDF_Std_pol1;
    double Loaded_A_Std_pol1_wKC, Loaded_B_Std_pol1_wKC;
    double Loaded_A_Std_pol1_wKC_Error, Loaded_B_Std_pol1_wKC_Error;
    double Loaded_ChiSquare_Std_pol1_wKC, Loaded_NDF_Std_pol1_wKC;

    double Loaded_A_Std_pol2, Loaded_B_Std_pol2, Loaded_C_Std_pol2;
    double Loaded_A_Std_pol2_Error, Loaded_B_Std_pol2_Error, Loaded_C_Std_pol2_Error;
    double Loaded_ChiSquare_Std_pol2, Loaded_NDF_Std_pol2;
    double Loaded_A_Std_pol2_wKC, Loaded_B_Std_pol2_wKC, Loaded_C_Std_pol2_wKC;
    double Loaded_A_Std_pol2_wKC_Error, Loaded_B_Std_pol2_wKC_Error, Loaded_C_Std_pol2_wKC_Error;
    double Loaded_ChiSquare_Std_pol2_wKC, Loaded_NDF_Std_pol2_wKC;

    double Loaded_A_Std_pol3, Loaded_B_Std_pol3, Loaded_C_Std_pol3, Loaded_D_Std_pol3;
    double Loaded_A_Std_pol3_Error, Loaded_B_Std_pol3_Error, Loaded_C_Std_pol3_Error, Loaded_D_Std_pol3_Error;
    double Loaded_ChiSquare_Std_pol3, Loaded_NDF_Std_pol3;
    double Loaded_A_Std_pol3_wKC, Loaded_B_Std_pol3_wKC, Loaded_C_Std_pol3_wKC, Loaded_D_Std_pol3_wKC;
    double Loaded_A_Std_pol3_wKC_Error, Loaded_B_Std_pol3_wKC_Error, Loaded_C_Std_pol3_wKC_Error, Loaded_D_Std_pol3_wKC_Error;
    double Loaded_ChiSquare_Std_pol3_wKC, Loaded_NDF_Std_pol3_wKC;

    string Loaded_Std_coefficients_path;
    vector<double> Loaded_Std_coefficients_values;
    vector <string> Loaded_Std_coefficients_names;

    double Loaded_A_Corr_pol1, Loaded_B_Corr_pol1;
    double Loaded_A_Corr_pol1_Error, Loaded_B_Corr_pol1_Error;
    double Loaded_ChiSquare_Corr_pol1, Loaded_NDF_Corr_pol1;
    double Loaded_A_Corr_pol1_wKC, Loaded_B_Corr_pol1_wKC;
    double Loaded_A_Corr_pol1_wKC_Error, Loaded_B_Corr_pol1_wKC_Error;
    double Loaded_ChiSquare_Corr_pol1_wKC, Loaded_NDF_Corr_pol1_wKC;

    double Loaded_A_Corr_pol2, Loaded_B_Corr_pol2, Loaded_C_Corr_pol2;
    double Loaded_A_Corr_pol2_Error, Loaded_B_Corr_pol2_Error, Loaded_C_Corr_pol2_Error;
    double Loaded_ChiSquare_Corr_pol2, Loaded_NDF_Corr_pol2;
    double Loaded_A_Corr_pol2_wKC, Loaded_B_Corr_pol2_wKC, Loaded_C_Corr_pol2_wKC;
    double Loaded_A_Corr_pol2_wKC_Error, Loaded_B_Corr_pol2_wKC_Error, Loaded_C_Corr_pol2_wKC_Error;
    double Loaded_ChiSquare_Corr_pol2_wKC, Loaded_NDF_Corr_pol2_wKC;

    double Loaded_A_Corr_pol3, Loaded_B_Corr_pol3, Loaded_C_Corr_pol3, Loaded_D_Corr_pol3;
    double Loaded_A_Corr_pol3_Error, Loaded_B_Corr_pol3_Error, Loaded_C_Corr_pol3_Error, Loaded_D_Corr_pol3_Error;
    double Loaded_ChiSquare_Corr_pol3, Loaded_NDF_Corr_pol3;
    double Loaded_A_Corr_pol3_wKC, Loaded_B_Corr_pol3_wKC, Loaded_C_Corr_pol3_wKC, Loaded_D_Corr_pol3_wKC;
    double Loaded_A_Corr_pol3_wKC_Error, Loaded_B_Corr_pol3_wKC_Error, Loaded_C_Corr_pol3_wKC_Error, Loaded_D_Corr_pol3_wKC_Error;
    double Loaded_ChiSquare_Corr_pol3_wKC, Loaded_NDF_Corr_pol3_wKC;

    string Loaded_Corr_coefficients_path;
    vector<double> Loaded_Corr_coefficients_values;
    vector <string> Loaded_Corr_coefficients_names;
    //</editor-fold>

    double Neutron_Momentum_cut = 9999.; // from clas12ana
    string SName;

public:

// Default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution() = default;

// Other constructors ---------------------------------------------------------------------------------------------------------------------------------------------------

    NeutronResolution(const string &Particle);

    NeutronResolution(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE,
                      const DSCuts &FD_nucleon_momentum_cut, const double &ParticleMomTh, bool const &Calculate_momResS2,
                      bool const &Run_in_momResS2,
                      const string &NeutronResolutionDirectory, const string &SavePath = "./", const double &DeltaSlices = 0.2,
                      const bool &VaryingDelta = false,
                      const string &SmearM = "pol1", const string &CorrM = "pol1", bool momRes_test = false, bool ForceSmallpResLimits = false);

// SetMomResCalculations function ---------------------------------------------------------------------------------------------------------------------------------------

    void SetMomResCalculations(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE,
                               const DSCuts &FD_nucleon_momentum_cut, const double &ParticleMomTh, bool const &Calculate_momResS2,
                               bool const &Run_in_momResS2,
                               const string &NeutronResolutionDirectory, const string &SavePath = "./", const double &DeltaSlices = 0.2,
                               const bool &VaryingDelta = false,
                               const string &SmearM = "pol1", const string &CorrM = "pol1", bool momRes_test = false,
                               bool ForceSmallpResLimits = false);

// SetMomResSlicesByType function ---------------------------------------------------------------------------------------------------------------------------------------

    void SetMomResSlicesByType(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh,
                               const string &MomentumType, const string &SavePath = "./", const bool &VaryingDelta = false,
                               const bool &momRes_test = false,
                               const bool &ForceSmallpResLimits = false);

// SetMomResSlices function ---------------------------------------------------------------------------------------------------------------------------------------

    void SetMomResSlices(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh,
                         const string &SavePath,
                         const bool &VaryingDelta, const bool &momRes_test, const bool &ForceSmallpResLimits, vector <hPlot1D> &ResSlices0,
                         vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0,
                         int &NumberOfSlices0);

// LoadFitParam function ---------------------------------------------------------------------------------------------------------------------------------------

    void LoadFitParam(const string &SampleName, const string &NucleonCutsDirectory, bool const &Calculate_momResS2,
                      const string &NeutronResolutionDirectory);

// ReadInputParam function ----------------------------------------------------------------------------------------------------------------------------------------------

    void ReadInputParam(const char *filename);

// SetUpperMomCut function ----------------------------------------------------------------------------------------------------------------------------------------------

    void SetUpperMomCut(const string &SampleName, const string &NucleonCutsDirectory);

// hFillResPlots function -----------------------------------------------------------------------------------------------------------------------------------------------

    void hFillResPlots(const double &TL_momentum, const double &Resolution, const double &Weight);

    void hFillResPlots(const double &Momentum, const double &Resolution, const double &Weight, vector <hPlot1D> &ResSlices0,
                       vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0,
                       int &NumberOfSlices0);

// hFillResPlotsByType function -----------------------------------------------------------------------------------------------------------------------------------------------

    void hFillResPlotsByType(const double &MomentumTL, const double &MomentumReco, const double &Resolution, const double &Weight);

// SliceFitDrawAndSave function -----------------------------------------------------------------------------------------------------------------------------------------

    void SliceFitDrawAndSave(const string &SampleName, const double &beamE);

    void SliceFitDrawAndSave(const string &SampleName, const double &beamE, const string &MomentumType, vector <hPlot1D> &ResSlices0,
                             vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0,
                             vector<int> &FittedSlices0, int &NumberOfSlices0);

// SliceFitDrawAndSaveByType function -----------------------------------------------------------------------------------------------------------------------------------

    void SliceFitDrawAndSaveByType(const string &SampleName, const double &beamE);

// Fitter functions -----------------------------------------------------------------------------------------------------------------------------------------------------

    void Fitter_Std_pol1();

    void Fitter_Std_pol1_wKC();

    void Fitter_Std_pol2();

    void Fitter_Std_pol2_wKC();

    void Fitter_Std_pol3();

    void Fitter_Std_pol3_wKC();

    void Fitter_Corr_pol1();

    void Fitter_Corr_pol1_wKC();

    void Fitter_Corr_pol2();

    void Fitter_Corr_pol2_wKC();

    void Fitter_Corr_pol3();

    void Fitter_Corr_pol3_wKC();


    void PolyFitterByType(const string &MomentumType, const int &PolynomialDegree, const string &FitType,
                          const string &MomentumFitRange, vector <vector<double>> &FitParam);

//    TCanvas *SetFitterCanvas(const string &FitCanvasName = "Fit_Canvas", const string &FitCanvasTitle = "Fit_Canvas",
//                             const int &wtopx = 1000, const int &wtopy = 750, const double
//                             );

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveResSlices(const string &SampleName, TCanvas *h1DCanvas, const string &plots_path, const string &NeutronResolutionDirectory);

// LogResDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogResDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory,
                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status);

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogFitDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory,
                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status);

// LogHistDataToFile function -------------------------------------------------------------------------------------------------------------------------------------------

    void LogHistDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory,
                           const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status);

// ReadResDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

    void ReadResDataParam(const char *filename, const bool &Calculate_momResS2, const string &SampleName, const string &NucleonCutsDirectory,
                          const bool &Load_correction = false, const bool &Load_smearing = false);

// PSmear function ------------------------------------------------------------------------------------------------------------------------------------------------------

    double PSmear(const bool &apply_nucleon_SmearAndCorr, const double &Momentum);

// NCorr function -------------------------------------------------------------------------------------------------------------------------------------------------------

    double NCorr(const bool &apply_nucleon_SmearAndCorr, const double &Momentum);

// GetMomResMu function -------------------------------------------------------------------------------------------------------------------------------------------------------

    double GetMomResMu(const bool &apply_nucleon_SmearAndCorr, const double &Momentum);

// Other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Set functions
    void SetHistSliceUpperLim(double hsUpperLim) { hSliceUpperLim = hsUpperLim; };

    void SetHistSliceLowerLim(double hsLowerLim) { hSliceLowerLim = hsLowerLim; };

    void SetSmearMode(const string &SmearM) { SmearMode = SmearM; };

    void SetCorrMode(const string &ShiftM) { CorrMode = ShiftM; };

    void SetSmearAndCorrModes(const string &SmearM, const string &ShiftM) { SmearMode = SmearM, CorrMode = ShiftM; };

    void SetForceSmallpResLimits(const bool &fsprl) { ForceSmallProtonResLimits = fsprl; };

    // Get functions
    double GetSliceUpperMomLim() { return SliceUpperMomLim; };

    string Get_SmearMode() { return SmearMode; };

    string Get_Loaded_Std_coefficients_path() { return Loaded_Std_coefficients_path; };

    vector<double> Get_Loaded_Std_coefficients_values() { return Loaded_Std_coefficients_values; };

    vector <string> Get_Loaded_Std_coefficients_names() { return Loaded_Std_coefficients_names; };

    string Get_CorrMode() { return CorrMode; };

    string Get_Loaded_Corr_coefficients_path() { return Loaded_Corr_coefficients_path; };

    vector<double> Get_Loaded_Corr_coefficients_values() { return Loaded_Corr_coefficients_values; };

    vector <string> Get_Loaded_Corr_coefficients_names() { return Loaded_Corr_coefficients_names; };

};

#endif //NEUTRONRESOLUTION_H
