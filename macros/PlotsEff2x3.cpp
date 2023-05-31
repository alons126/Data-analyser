//
// Created by alons on 10/05/2023.
//

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

#include "PlotsEff2x3Nucleon.cpp"
#include "PlotsEff2x3Electron.cpp"

using namespace std;

void PlotsEff2x3() {

    string SampleName = "C12_simulation_6GeV_T5_first_10";
//    string SampleName = "C12_simulation_6GeV_T5_first_100";
//    string SampleName = "C12_simulation_6GeV_T5_first_250";
//    string SampleName = "C12_simulation_6GeV_T5";
//
    PlotsEff2x3Nucleon(SampleName);
    PlotsEff2x3Electron(SampleName);
}

