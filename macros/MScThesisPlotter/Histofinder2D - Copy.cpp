/* root BetaFitAndSave.cpp -q -b */

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

#include "../../source/functions/GeneralFunctions.h"
#include "../../source/constants.h"

using namespace std;

TH2D *Histofinder2D(const char *filename, const char *Histogram2DNameSubstring) {
    bool PrintOut = false;
    bool PrintOutResult = false;

    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nHistofinder2D: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    TH2D *Histogram2D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    static TString classname("TH2D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while (Key = (TKey *) Next()) {
        TH2D *Histogram2DTemp = (TH2D *) Key->ReadObj();

        string Histogram2DTempName = Histogram2DTemp->GetName();

        if (PrintOut) { cout << Histogram2DTempName << "\n\n"; }

        if (findSubstring(Histogram2DTempName, Histogram2DNameSubstring)) {
            HistogramFound = true;

            if (PrintOutResult) {
                cout << "\n\nKey name: " << Histogram2DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n";
                cout << "filename: " << filename << "\n\n";
                cout << "Histogram2DNameSubstring: " << Histogram2DNameSubstring << "\n\n";
            }

            Histogram2D = (TH2D *) Key->ReadObj();

            break;
        }
    }

    if (!HistogramFound) {
        cout << "\n\nHistofinder2D: could not find histogram!\n";

        return Histogram2D;
    } else {
        return Histogram2D;
    }
}
