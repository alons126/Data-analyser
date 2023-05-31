//
// Created by alons on 12/05/2023.
//

#ifndef FITFUNCTION_H
#define FITFUNCTION_H

#include <iostream>
#include <fstream>
#include <string>

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

using namespace std;

Double_t FitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
//    if (par[1] != 0) { arg = (v[0] - 1) / par[1]; } // 2 parameters
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}

#endif //FITFUNCTION_H
