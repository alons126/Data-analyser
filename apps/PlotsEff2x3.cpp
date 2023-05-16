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

using namespace std;

void PlotsEff2x3() {

    system("root -l -b -q /home/alon/project/apps/PlotsEff2x3Nucleon.cpp");
    system("root -l -b -q /home/alon/project/apps/PlotsEff2x3Electron.cpp");

}

