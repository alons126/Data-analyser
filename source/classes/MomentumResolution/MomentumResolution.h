//
// Created by alons on 20/06/2023.
//

#ifndef MOMENTUMRESOLUTION_H
#define MOMENTUMRESOLUTION_H

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

#if MomResDebugMacro
#include "../hData/hData.cpp"
#include "../hPlots/hPlot1D.cpp"
#else

#include "../hPlots/hPlot1D.h"

#endif

#include "../DSCuts/DSCuts.h"
#include "../../functions/GeneralFunctions.h"

using namespace std;

class MomentumResolution {
private:
    bool isNeutron = false, isProton = false;
    string MomResParticle = "";

    bool momResTestMode, momResS2CalcMode, momResS2RunMode;

    bool ForceSmallProtonResLimits = false;

    bool FitDebuggingMode = false;

    double Ebeam;

    string SmearMode = "NONE", CorrMode = "NONE";

    string RecoSliceFitAlgorithm = "", TLSliceFitAlgorithm = "";
    vector <hPlot1D> ResTLMomSlices, ResRecoMomSlices;
    vector <vector<double>> ResTLMomSlicesLimits, ResRecoMomSlicesLimits;
    vector <DSCuts> ResTLMomSlicesFitVar, ResRecoMomSlicesFitVar;
    vector <DSCuts> ResTLMomSlicesHistVar, ResRecoMomSlicesHistVar;
    vector <DSCuts> Loaded_Res_Slices_FitVar;
    vector <DSCuts> Loaded_Res_Slices_HistVar;
    vector<int> FittedTLMomSlices, FittedRecoMomSlices;

    double SliceUpperMomLim; // upper lim for momentum slices - loaded from file
    double SliceUpperMomLimKC_mu, SliceLowerMomLimKC_mu; // lower lim for momentum slices - set by constructor
    double SliceUpperMomLimKC_sigma, SliceLowerMomLimKC_sigma; // lower lim for momentum slices - set by constructor

    double hSliceUpperLim = 1.1;
    double hSliceLowerLim = -1.1;
    int hSliceNumOfBin = 65;

    TList *ResSlicePlots = new TList();
    TFolder *FittedTLNeutronResSlices = new TFolder("Fitted TL neutron resolution slices", "Fitted TL neutron resolution slices");
    TFolder *FittedTLNeutronResSlicesMean = new TFolder("Fitted TL neutron resolution slice mean", "Fitted TL neutron resolution slice mean");
    TFolder *FittedTLNeutronResSlicesWidth = new TFolder("Fitted TL neutron resolution slice width", "Fitted TL neutron resolution slice width");
    TFolder *FittedTLProtonResSlices = new TFolder("Fitted TL proton resolution slices", "Fitted TL proton resolution slices");
    TFolder *FittedTLProtonResSlicesMean = new TFolder("Fitted TL proton resolution slice mean", "Fitted TL proton resolution slice mean");
    TFolder *FittedTLProtonResSlicesWidth = new TFolder("Fitted TL proton resolution slice width", "Fitted TL proton resolution slice width");
    TFolder *FittedRecoNeutronResSlices = new TFolder("Fitted reco neutron resolution slices", "Fitted reco neutron resolution slices");
    TFolder *FittedRecoNeutronResSlicesMean = new TFolder("Fitted reco neutron resolution slice mean", "Fitted reco neutron resolution slice mean");
    TFolder *FittedRecoNeutronResSlicesWidth = new TFolder("Fitted reco neutron resolution slice width", "Fitted reco neutron resolution slice width");
    TFolder *FittedRecoProtonResSlices = new TFolder("Fitted reco proton resolution slices", "Fitted reco proton resolution slices");
    TFolder *FittedRecoProtonResSlicesMean = new TFolder("Fitted reco proton resolution slice mean", "Fitted reco proton resolution slice mean");
    TFolder *FittedRecoProtonResSlicesWidth = new TFolder("Fitted reco proton resolution slice width", "Fitted reco proton resolution slice width");

    //<editor-fold desc="Random number generators">
    string RandomNumGenerator = "TRandom3"; // TRandom3 by default

    TRandom1 *Rand1 = new TRandom1();
    TRandom2 *Rand2 = new TRandom2();
    TRandom3 *Rand3 = new TRandom3();
    //</editor-fold>

    double delta, deltaFactor = 1.;
    int TL_NumberOfSlices = 0, Reco_NumberOfSlices = 0;

    //<editor-fold desc="Correction and smear fit variables">
    /* General vector structure:   {{A, B, ...}, {A_Err, B_Err, ...}, {ChiSquare_Smear_pol2_wKC, NDF_Smear_pol2_wKC},} */

    //<editor-fold desc="Correction and smear fit variables">
    vector <vector<double>> TL_FitParam_Smear_pol1, Reco_FitParam_Smear_pol1;
    vector <vector<double>> TL_FitParam_Smear_pol1_wKC, Reco_FitParam_Smear_pol1_wKC;

    vector <vector<double>> TL_FitParam_Smear_pol2, Reco_FitParam_Smear_pol2;
    vector <vector<double>> TL_FitParam_Smear_pol2_wKC, Reco_FitParam_Smear_pol2_wKC;

    vector <vector<double>> TL_FitParam_Smear_pol3, Reco_FitParam_Smear_pol3;
    vector <vector<double>> TL_FitParam_Smear_pol3_wKC, Reco_FitParam_Smear_pol3_wKC;

    vector <vector<double>> TL_FitParam_Corr_pol1, Reco_FitParam_Corr_pol1;
    vector <vector<double>> TL_FitParam_Corr_pol1_wKC, Reco_FitParam_Corr_pol1_wKC;

    vector <vector<double>> TL_FitParam_Corr_pol2, Reco_FitParam_Corr_pol2;
    vector <vector<double>> TL_FitParam_Corr_pol2_wKC, Reco_FitParam_Corr_pol2_wKC;

    vector <vector<double>> TL_FitParam_Corr_pol3, Reco_FitParam_Corr_pol3;
    vector <vector<double>> TL_FitParam_Corr_pol3_wKC, Reco_FitParam_Corr_pol3_wKC;
    //</editor-fold>

    //<editor-fold desc="Loaded correction and smear fit variables">
    string Loaded_Std_coefficients_path;
    vector<double> Loaded_Smear_coefficients_values;
    vector <string> Loaded_Smear_coefficients_names;

    vector <vector<double>> Loaded_TL_FitParam_Smear_pol1, Loaded_Reco_FitParam_Smear_pol1;
    vector <vector<double>> Loaded_TL_FitParam_Smear_pol1_wKC, Loaded_Reco_FitParam_Smear_pol1_wKC;
    vector <vector<double>> Loaded_TL_FitParam_Smear_pol2, Loaded_Reco_FitParam_Smear_pol2;
    vector <vector<double>> Loaded_TL_FitParam_Smear_pol2_wKC, Loaded_Reco_FitParam_Smear_pol2_wKC;
    vector <vector<double>> Loaded_TL_FitParam_Smear_pol3, Loaded_Reco_FitParam_Smear_pol3;
    vector <vector<double>> Loaded_TL_FitParam_Smear_pol3_wKC, Loaded_Reco_FitParam_Smear_pol3_wKC;

    string Loaded_Corr_coefficients_path;
    vector<double> Loaded_Corr_coefficients_values;
    vector <string> Loaded_Corr_coefficients_names;

    vector <vector<double>> Loaded_TL_FitParam_Corr_pol1, Loaded_Reco_FitParam_Corr_pol1;
    vector <vector<double>> Loaded_TL_FitParam_Corr_pol1_wKC, Loaded_Reco_FitParam_Corr_pol1_wKC;
    vector <vector<double>> Loaded_TL_FitParam_Corr_pol2, Loaded_Reco_FitParam_Corr_pol2;
    vector <vector<double>> Loaded_TL_FitParam_Corr_pol2_wKC, Loaded_Reco_FitParam_Corr_pol2_wKC;
    vector <vector<double>> Loaded_TL_FitParam_Corr_pol3, Loaded_Reco_FitParam_Corr_pol3;
    vector <vector<double>> Loaded_TL_FitParam_Corr_pol3_wKC, Loaded_Reco_FitParam_Corr_pol3_wKC;
    //</editor-fold>

    //</editor-fold>

    double Neutron_Momentum_cut = 9999.; // from clas12ana
    string SName;

    string SlicesSavePath;
    string SlicesSaveNamePrefix;

public:

// Default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    MomentumResolution() = default;

// Other constructors ---------------------------------------------------------------------------------------------------------------------------------------------------

    MomentumResolution(const string &Particle);

// MomResInit function --------------------------------------------------------------------------------------------------------------------------------------------------

    void MomResInit(const bool &plot_and_fit_MomRes, const bool &Calculate_momResS2, const bool &Run_with_momResS2, const string &SampleName,
                    const string &NucleonCutsDirectory, const double &beamE, const DSCuts &MomRes_mu_cuts, const DSCuts &MomRes_sigma_cuts, const double &ParticleMomTh,
                    const string &MomentumResolutionDirectory, const string &SavePath = "./", const double &DeltaSlices = 0.2, const bool &VaryingDelta = false,
                    const string &SmearM = "pol1", const string &CorrM = "pol1", const bool &momRes_test = false, const bool &ForceSmallpResLimits = false,
                    const bool &FitDebugging = false);

    void SetMomResCalculations(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE, const DSCuts &MomRes_mu_cuts,
                               const DSCuts &MomRes_sigma_cuts, const double &ParticleMomTh, bool const &Calculate_momResS2, bool const &Run_in_momResS2,
                               const string &MomentumResolutionDirectory, const string &SavePath = "./", const double &DeltaSlices = 0.2, const bool &VaryingDelta = false,
                               const string &SmearM = "pol1", const string &CorrM = "pol1", const bool momRes_test = false, const bool ForceSmallpResLimits = false,
                               const bool &FitDebugging = false);

    void SetMomResSlicesByType(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh, const string &MomentumType,
                               const string &SavePath = "./", const bool &VaryingDelta = false, const bool &momRes_test = false, const bool &ForceSmallpResLimits = false,
                               const bool &FitDebugging = false);

    void SetMomResSlices(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh, const string &MomentumType,
                         const string &SavePath, const bool &VaryingDelta, const bool &momRes_test, const bool &ForceSmallpResLimits, vector <hPlot1D> &ResSlices0,
                         vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0, int &NumberOfSlices0,
                         const bool &FitDebugging = false);

    void SetUpperMomCut(const string &SampleName, const string &NucleonCutsDirectory, const bool &FitDebugging = false);

    void LoadFitParam(const string &SampleName, const string &NucleonCutsDirectory, bool const &Calculate_momResS2, const string &MomentumResolutionDirectory);

// ReadInputParam function ----------------------------------------------------------------------------------------------------------------------------------------------

    void ReadInputParam(const char *filename);

// hFillResPlotsByType function -----------------------------------------------------------------------------------------------------------------------------------------

    void hFillResPlotsByType(const double &MomentumTL, const double &MomentumReco, const double &Resolution, const double &Weight);

    void hFillResPlots(const double &Momentum, const double &Resolution, const double &Weight, vector <hPlot1D> &ResSlices0, vector <vector<double>> &ResSlicesLimits0,
                       int &NumberOfSlices0);

// SliceFitDrawAndSaveByType function -----------------------------------------------------------------------------------------------------------------------------------

    void SliceFitDrawAndSaveByType(const string &SampleName, const double &beamE);

    void SliceFitDrawAndSave(const string &SampleName, const double &beamE, const string &MomentumType, vector <hPlot1D> &ResSlices0,
                             vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0, vector<int> &FittedSlices0,
                             int &NumberOfSlices0);

    void PolyFitter(const string &MomentumType, const int &PolynomialDegree, const string &FitType, const string &MomentumFitRange, vector <vector<double>> &FitParam);

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveResSlices(const string &SampleName, TCanvas *h1DCanvas, const string &plots_path, const string &MomentumResolutionDirectory);

// LogResDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogResDataToFile(const string &SampleName, const string &plots_path, const string &MomentumResolutionDirectory);

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

    void LogFitDataToFile(const string &SampleName, const string &plots_path, const string &MomentumResolutionDirectory);

    void AutoLogger(const string &SampleName, const string &plots_path, const string &MomentumResolutionDirectory, const string &LogHeader,
                    const vector <vector<double>> &Vector2Log, ofstream &Neutron_res_fit_param);

// LogHistDataToFile function -------------------------------------------------------------------------------------------------------------------------------------------

    void LogHistDataToFile(const string &SampleName, const string &plots_path, const string &MomentumResolutionDirectory);

// ReadResDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

    void ReadResDataParam(const char *filename, const bool &Calculate_momResS2, const string &SampleName, const string &NucleonCutsDirectory, const string &MomentumType,
                          const bool &Load_correction = false, const bool &Load_smearing = false);

    void AutoReader(const string &MomentumType, const int &PolynomialDegree, const string &parameter, basic_istream<char> &ss2, vector <vector<double>> &Loading_Dest,
                    vector<double> &Loaded_coefficients_values, vector <string> &Loaded_coefficients_names);

    void AutoReader(const string &MomentumType, const int &PolynomialDegree, const string &parameter, basic_istream<char> &ss2, vector <vector<double>> &Loading_Dest);

// PSmear function ------------------------------------------------------------------------------------------------------------------------------------------------------

    double PSmear(const bool &apply_nucleon_SmearAndCorr, const double &Momentum);

// NCorr function -------------------------------------------------------------------------------------------------------------------------------------------------------

    double NCorr(const bool &apply_nucleon_SmearAndCorr, const double &Momentum);

// Other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Set functions
    void SetHistSliceUpperLim(double hsUpperLim) { hSliceUpperLim = hsUpperLim; };

    void SetHistSliceLowerLim(double hsLowerLim) { hSliceLowerLim = hsLowerLim; };

    void SetSmearMode(const string &SmearM) { SmearMode = SmearM; };

    void SetCorrMode(const string &ShiftM) { CorrMode = ShiftM; };

    void SetSmearAndCorrModes(const string &SmearM, const string &ShiftM) { SmearMode = SmearM, CorrMode = ShiftM; };

    void SetForceSmallpResLimits(const bool &fsprl) { ForceSmallProtonResLimits = fsprl; };

    void OverwriteResRecoMomSlices(vector<TH1D *> ResSlices0) {
        for (int i = 0; i < ResRecoMomSlices.size(); i++) { ResRecoMomSlices.at(i).SetHistogram1D(ResSlices0.at(i)); }
    }

    void OverwriteResTLMomSlices(vector<TH1D *> ResSlices0) {
        for (int i = 0; i < ResTLMomSlices.size(); i++) { ResTLMomSlices.at(i).SetHistogram1D(ResSlices0.at(i)); }
    }

    void SetResRecoMomSlicesLimits(vector <vector<double>> ResSlicesLimits0) { ResRecoMomSlicesLimits = ResSlicesLimits0; };

    void SetResTLMomSlicesLimits(vector <vector<double>> ResSlicesLimits0) { ResTLMomSlicesLimits = ResSlicesLimits0; };

    void SetResRecoMomSlicesFitVar(vector <DSCuts> ResSlicesFitVar0) { ResRecoMomSlicesFitVar = ResSlicesFitVar0; };

    void SetResTLMomSlicesFitVar(vector <DSCuts> ResSlicesFitVar0) { ResTLMomSlicesFitVar = ResSlicesFitVar0; };

    void SetResRecoMomSlicesHistVar(vector <DSCuts> ResSlicesHistVar0) { ResRecoMomSlicesHistVar = ResSlicesHistVar0; };

    void SetResTLMomSlicesHistVar(vector <DSCuts> ResSlicesHistVar0) { ResTLMomSlicesHistVar = ResSlicesHistVar0; };

    void SetFittedRecoMomSlices(vector<int> FittedSlices0) { FittedRecoMomSlices = FittedSlices0; };

    void SetFittedTLMomSlices(vector<int> FittedSlices0) { FittedTLMomSlices = FittedSlices0; };

    // Get functions
    double GetSliceUpperMomLim() { return SliceUpperMomLim; };

    string Get_SmearMode() { return SmearMode; };

    string Get_Loaded_Std_coefficients_path() { return Loaded_Std_coefficients_path; };

    vector<double> Get_Loaded_Smear_coefficients_values() { return Loaded_Smear_coefficients_values; };

    vector <string> Get_Loaded_Smear_coefficients_names() { return Loaded_Smear_coefficients_names; };

    string Get_CorrMode() { return CorrMode; };

    string Get_Loaded_Corr_coefficients_path() { return Loaded_Corr_coefficients_path; };

    vector<double> Get_Loaded_Corr_coefficients_values() { return Loaded_Corr_coefficients_values; };

    vector <string> Get_Loaded_Corr_coefficients_names() { return Loaded_Corr_coefficients_names; };

    vector <vector<double>> GetResRecoMomSlicesLimits() { return ResRecoMomSlicesLimits; };

    vector <vector<double>> GetResTLMomSlicesLimits() { return ResTLMomSlicesLimits; };

    vector <DSCuts> GetResRecoMomSlicesFitVar() { return ResRecoMomSlicesFitVar; };

    vector <DSCuts> GetResTLMomSlicesFitVar() { return ResTLMomSlicesFitVar; };

    vector <DSCuts> GetResRecoMomSlicesHistVar() { return ResRecoMomSlicesHistVar; };

    vector <DSCuts> GetResTLMomSlicesHistVar() { return ResTLMomSlicesHistVar; };

    vector<int> GetFittedRecoMomSlices() { return FittedRecoMomSlices; };

    vector<int> GetFittedTLMomSlices() { return FittedTLMomSlices; };

    string GetRandNumGenerator() { return RandomNumGenerator; };

};

#endif //MOMENTUMRESOLUTION_H
