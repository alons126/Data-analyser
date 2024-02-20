//
// Created by alons on 20/02/2024.
//

#ifndef CUSTOMSLICELIMITS_H
#define CUSTOMSLICELIMITS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>
#include <string>

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

//<editor-fold desc="C12_simulation_6GeV_T5 (Old sample)">
vector <vector<double>> CustomElectronMomSliceLimits_C12_simulation_6GeV_T5 = {{0.4, 1.6},
                                                                               {1.6, 2.2},
                                                                               {2.2, 2.8},
                                                                               {2.8, 4},
                                                                               {4,   5},
                                                                               {5,   6}};
//</editor-fold>

//<editor-fold desc="C12x4_simulation_G18_Q204_6GeV (New sample (24M; 1-foil & 4-foil))">

//<editor-fold desc="v3 cuts">
vector <vector<double>> CustomElectronMomSliceLimits_C12x4_simulation_G18_Q204_6GeV = {{0.00, 0.60},
                                                                                       {0.60, 0.80},
                                                                                       {0.80, 1.20},
                                                                                       {1.20, 1.60},
                                                                                       {1.60, 1.80},
                                                                                       {1.80, 2.00},
                                                                                       {2.00, 2.20},
                                                                                       {2.20, 2.40},
                                                                                       {2.40, 2.50},
                                                                                       {2.50, 2.60},
                                                                                       {2.60, 2.70},
                                                                                       {2.70, 2.80},
                                                                                       {2.80, 2.90},
                                                                                       {2.90, 3.00},
                                                                                       {3.00, 3.10},
                                                                                       {3.10, 3.20},
                                                                                       {3.20, 3.30},
                                                                                       {3.30, 3.40},
                                                                                       {3.40, 3.50},
                                                                                       {3.50, 3.60},
                                                                                       {3.60, 3.65},
                                                                                       {3.65, 3.70},
                                                                                       {3.70, 3.75},
                                                                                       {3.75, 3.80},
                                                                                       {3.80, 3.85},
                                                                                       {3.85, 3.90},
                                                                                       {3.90, 3.95},
                                                                                       {3.95, 4.00},
                                                                                       {4.00, 4.05},
                                                                                       {4.05, 4.10},
                                                                                       {4.10, 4.15},
                                                                                       {4.15, 4.20},
                                                                                       {4.20, 4.25},
                                                                                       {4.25, 4.30},
                                                                                       {4.30, 4.35},
                                                                                       {4.35, 4.40},
                                                                                       {4.40, 4.45},
                                                                                       {4.45, 4.50},
                                                                                       {4.50, 4.55},
                                                                                       {4.55, 4.60},
                                                                                       {4.60, 4.65},
                                                                                       {4.65, 4.70},
                                                                                       {4.70, 4.75},
                                                                                       {4.75, 4.80},
                                                                                       {4.80, 4.85},
                                                                                       {4.85, 4.90},
                                                                                       {4.90, 4.95},
                                                                                       {4.95, 5.00},
                                                                                       {5.00, 5.05},
                                                                                       {5.05, 5.10},
                                                                                       {5.10, 5.15},
                                                                                       {5.15, 5.20},
                                                                                       {5.20, 5.40},
                                                                                       {5.40, 6.00}};

vector <vector<double>> CustomNucleonMomSliceLimits_C12x4_simulation_G18_Q204_6GeV = {{0.4,      0.446273},
                                                                                      {0.446273, 0.504651},
                                                                                      {0.504651, 0.580602},
                                                                                      {0.580602, 0.683465},
                                                                                      {0.683465, 0.830624},
                                                                                      {0.830624, 1.0},
                                                                                      {1.0,      6.0}};

/*
vector <vector<double>> ElectronMomSliceLimits = {{0.0, 0.6},
                                                  {0.6, 0.8},
                                                  {0.8, 1.2},
                                                  {1.2, 1.6},
                                                  {1.6, 1.8},
                                                  {1.8, 2.0},
                                                  {2.0, 2.2},
                                                  {2.2, 2.4},
                                                  {2.4, 2.5},
                                                  {2.5, 2.6},
                                                  {2.6, 2.7},
                                                  {2.7, 2.8},
                                                  {2.8, 2.9},
                                                  {2.9, 3.0},
                                                  {3.0, 3.1},
                                                  {3.1, 3.2},
                                                  {3.2, 3.3},
                                                  {3.3, 3.4},
                                                  {3.4, 3.5},
                                                  {3.5, 3.6},
                                                  {3.6, 3.7},
                                                  {3.7, 3.8},
                                                  {3.8, 3.9},
                                                  {3.9, 4.0},
                                                  {4.0, 4.1},
                                                  {4.1, 4.2},
                                                  {4.2, 4.3},
                                                  {4.3, 4.5},
                                                  {4.5, 4.6},
                                                  {4.6, 4.7},
                                                  {4.7, 4.8},
                                                  {4.8, 4.9},
                                                  {4.9, 5.0},
                                                  {5.0, 5.1},
                                                  {5.1, 5.2},
                                                  {5.2, 6.0}};
*/

//</editor-fold>

////<editor-fold desc="v2 cuts">
//vector <vector<double>> ElectronMomSliceLimits = {{0.,  0.6},
//                                                  {0.6, 0.8},
//                                                  {0.8, 1.2},
//                                                  {1.2, 1.6},
//                                                  {1.6, 2.2},
//                                                  {2.2, 2.8},
//                                                  {2.8, 4},
//                                                  {4,   5},
//                                                  {5,   6}};
////</editor-fold>

//</editor-fold>

#endif //CUSTOMSLICELIMITS_H