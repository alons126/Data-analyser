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

#include "source/functions/GeneralFunctions.h"
#include "source/constants.h"

using namespace std;

TH1D *Histofinder1D(const char *filename, const char *Histogram1DNameSubstring) {
    bool PrintOut = true;

    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    TH1D *Histogram1D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    static TString classname("TH1D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while (Key = (TKey *) Next()) {
        TH1D *Histogram1DTemp = (TH1D *) Key->ReadObj();

        string Histogram1DTempName = Histogram1DTemp->GetName();

        if (PrintOut) { cout << Histogram1DTempName << "\n\n"; }

        if (findSubstring(Histogram1DTempName, Histogram1DNameSubstring)) {
            HistogramFound = true;

            if (PrintOut) { cout << "\n\nKey name: " << Histogram1DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n\n"; }

            Histogram1D = (TH1D *) Key->ReadObj();

            break;
        }
    }

    if (!HistogramFound) {
        cout << "\n\nHistofinder1D: could not find histogram!\n";

        return Histogram1D;
    } else {
        return Histogram1D;
    }
}

/*
void Histofinder1D(const char *filename, const char *Histogram1DNameSubstring, TH1D *Histogram1D) {
    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    bool HistogramFound = false;

    TH1D *Histogram1DTemp;

    int Counter = 0, HistogramCounter = 0;
    static TString classname("TH1D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while (Key = (TKey *) Next()) {
        TH1D *Histogram1DTemp = (TH1D *) Key->ReadObj();

        string Histogram1DTempName = Histogram1DTemp->GetName();

        cout << Histogram1DTempName << "\n\n";

        if (findSubstring(Histogram1DTempName, Histogram1DNameSubstring)) {
            HistogramFound = true;

            cout << "\n\nKey name: " << Histogram1DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n\n";

            Histogram1D = (TH1D *) Key->ReadObj();
//            Histogram1DTemp = (TH1D *) file->Get((Histogram1DTempName).c_str());

            break;
        }
    } // end of MC while

    if (!HistogramFound) {
        cout << "\n\nHistofinder1D: could not find histogram!\n";
    }
//    else {
//        Histogram1D = Histogram1DTemp;
//    }
}
*/

/*
void Histofinder1D(const char *filename, const char *Histogram1DNameSubstring, TH1D *Histogram1D) {
    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    bool HistogramFound = false;

    TH1D *Histogram1DTemp;

    for (TObject *keyAsObj: *file->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        string HistNameTemp = key->GetName();
        cout << key->GetName() << "\n\n";

        if (findSubstring(HistNameTemp, Histogram1DNameSubstring)) {
            HistogramFound = true;

            cout << "\n\nKey name: " << key->GetName() << " Type: " << key->GetClassName() << endl;

            Histogram1DTemp = (TH1D *) file->Get(HistNameTemp);

            break;
        }
    }

    if (!HistogramFound) {
        cout << "\n\nHistofinder1D: could not find histogram!\n";
    } else {
        Histogram1D = Histogram1DTemp;
    }
}*/
