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

#include "PlotsACorr2x3Nucleon.cpp"
#include "PlotsACorr2x3Electron.cpp"
#include "PlotsEff2x3Nucleon.cpp"
#include "PlotsEff2x3Electron.cpp"

using namespace std;

void PlotsEffAndACorr2x3() {

//    string SampleName = "C12_simulation_6GeV_T5_first_10_wNC_wFDph_Eff1";
//    string SampleName = "C12_simulation_6GeV_T5_first_100";
//    string SampleName = "C12_simulation_6GeV_T5_first_250";
//    string SampleName = "C12_simulation_6GeV_T5";
    string SampleName = "C12_simulation_6GeV_T5_wNC_noFDph_Eff2";

//    string SampleName = "C12_simulation_G18_2GeV_wNC_noFDph_Eff2";
//    string SampleName = "C12_simulation_G18_2GeV_wNC_wFDph_Eff2";

//    PlotsACorr2x3Nucleon(SampleName);
//    PlotsACorr2x3Electron(SampleName);

    PlotsEff2x3Nucleon(SampleName);
    PlotsEff2x3Electron(SampleName);
}

