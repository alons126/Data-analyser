//
// Created by alons on 06/04/2023.
//

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
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

#include <TBenchmark.h>

#include "HipoChain.h"
#include "clas12reader.h"

#include "setup/codeSetup.h"
#include "source/classes/hPlots/hPlot1D.cpp"

using namespace std;
using namespace clas12;

void hPlot1D_example() {
    TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
    c1->SetGrid();
    c1->SetBottomMargin(0.14);
    c1->SetLeftMargin(0.14);
    c1->SetRightMargin(0.12);

    c1->cd();

    hPlot1D testHist = hPlot1D("hst", "ht", "xat", 0, 1.25);
    testHist.hFill(1);
    testHist.SimpleDraw();
    testHist.SimpleSave(c1);
}