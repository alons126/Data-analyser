//
// Created by alons on 27/01/2023.
//

#ifndef GENERALFUNCTIONS_H
#define GENERALFUNCTIONS_H

//TODO: recheck which of these #include-s are needed and which aren't

/*
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
#include <sstream>
*/

/*
#include <cstdlib>
#include <chrono>
#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <iomanip>

#include "clas12reader.h"
*/

#if FullAnalyser
#include "clas12reader.h"

using namespace clas12;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                      General functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GetCurrentDirectory function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetCurrentDirectory function">
/* Usage: get the directory of the main.c code */

string GetCurrentDirectory() {
    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));

    string WorkingDirectory = pwd;

    return WorkingDirectory;
}
//</editor-fold>

// findSubstring function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="findSubstring function">
/* Usage: in getBeanE */

bool findSubstring(string string1, string string2) {
    if (string1.find(string2) != string::npos) {
        return true;
    } else {
        return false;
    }
}
//</editor-fold>

// to_string_with_precision function ------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="to_string_with_precision function">
/* Usage: convert a number to string with n figures after the decimal point in the plotting functions */

template<typename T>
string to_string_with_precision(const T a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
//</editor-fold>

// rCalc function -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="rCalc function">
/* Usage: calculate vector magnitude, given x,y,z components. */

double rCalc(double x, double y, double z) {
    double r = sqrt(x * x + y * y + z * z);
    return r;
}
//</editor-fold>

// LogEventCuts functions -----------------------------------------------------------------------------------------------------------------------------------------------

#if FullAnalyser

//<editor-fold desc="LogEventCuts function (original/MicroBooNE)">
/* Usage: in cut parameter testings, log to an AC histogram given the cuts. May be applied to other cuts in the future. */

void LogEventCuts(TH1D *Histogram1D, clas12::region_part_ptr Particle, string CutType, double Upper_cut, double Lower_cut, double CutCenter = 0) {
    if (CutType == "momentum" || CutType == "") {
        TVector3 P;
        P.SetMagThetaPhi(Particle->getP(), Particle->getTheta(), Particle->getPhi());

        if ((Upper_cut == -1) && (Lower_cut == -1)) {
            Histogram1D->Fill(P.Mag());
        } else if ((Upper_cut != -1) && (Lower_cut == -1)) {
            if (P.Mag() <= Upper_cut) { Histogram1D->Fill(P.Mag()); }
        } else if ((Upper_cut == -1) && (Lower_cut != -1)) {
            if (P.Mag() >= Lower_cut) { Histogram1D->Fill(P.Mag()); }
        } else if ((Upper_cut != -1) && (Lower_cut != -1)) {
            if ((P.Mag() >= Lower_cut) && (P.Mag() <= Upper_cut)) { Histogram1D->Fill(P.Mag()); }
        }
    }
}
//</editor-fold>

//<editor-fold desc="LogEventCuts function (1e2p and 2p)">
/* Usage: in cut parameter testings, log to an AC histogram given the cuts. */

void LogEventCuts(TH1D *Histogram1D, clas12::region_part_ptr Particle, double Lower_cut, double Upper_cut, double CutCenter = 0) {
    TVector3 P;
    P.SetMagThetaPhi(Particle->getP(), Particle->getTheta(), Particle->getPhi());

    if ((Upper_cut == -1) && (Lower_cut == -1)) {
        Histogram1D->Fill(P.Mag());
    } else if ((Upper_cut != -1) && (Lower_cut == -1)) {
        if (P.Mag() <= Upper_cut) { Histogram1D->Fill(P.Mag()); }
    } else if ((Upper_cut == -1) && (Lower_cut != -1)) {
        if (P.Mag() >= Lower_cut) { Histogram1D->Fill(P.Mag()); }
    } else if ((Upper_cut != -1) && (Lower_cut != -1)) {
        if ((P.Mag() >= Lower_cut) && (P.Mag() <= Upper_cut)) { Histogram1D->Fill(P.Mag()); }
    }
}
//</editor-fold>

#endif

// BoolToString function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="BoolToString function">
/* Usage: convert bool variables to string. Used to log setup to file. */

inline const char *const BoolToString(bool b) { return b ? "true" : "false"; }
//</editor-fold>

// fit functions --------------------------------------------------------------------------------------------------------------------------------------------------

////<editor-fold desc="testPrint functions">
///* Usage: general functions used to print variables. */
//
//Double_t FitFunction(Double_t *v, Double_t *par) {
//    Double_t arg = 0;
//    if (par[2] != 0) arg = (v[0] - par[1]) / par[2];
//
//    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
//    return fitval;
//}
//
//void fitexample() {
//    TFile *f = new TFile("hsimple.root");
//
//    TH1F *hpx = (TH1F *) f->Get("hpx");
//
//    //create a function with 3 parameters in the range [-3,3]
//    TF1 *func = new TF1("fit", FitFunction, -3, 3, 3);
//    func->SetParameters(500, hpx->GetMean(), hpx->GetRMS());
//    func->SetParNames("Constant", "Mean_value", "Sigma");
//    hpx->Fit("fit");
//}
////</editor-fold>

#endif //GENERALFUNCTIONS_H
