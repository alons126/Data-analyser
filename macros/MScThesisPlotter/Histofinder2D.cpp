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
    if (!file) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    TH2D *Histogram2D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    static TString classname("TH2D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while ((Key = (TKey *) Next())) {
//    while (Key = (TKey *) Next()) {
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

TH2D *Histofinder2D(const char *filename, const char *Histogram2DNameSubstring, const bool TLmom) {
    bool PrintOut = false;
    bool PrintOut1 = false;

    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nHistofinder2D: could not load root file! Exiting...\n", exit(0); }

    TH2D *Histogram2D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    static TString classname("TH2D");
    static TString classnameTFolder("TFolder");
    static TString classnameTHStack("THStack");
    string FoundHistClass;
    string FoundHistName;

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while ((Key = (TKey *) Next())) {
//    while (Key = (TKey *) Next()) {
        TH2D *Histogram2DTemp = (TH2D *) Key->ReadObj();

        string Histogram2DTempName = Histogram2DTemp->GetName();

        if (PrintOut1) { cout << Histogram2DTempName << "\n\n"; }

        if (findSubstring(Histogram2DTempName, Histogram2DNameSubstring) &&
            (Key->GetClassName() != classnameTFolder("TFolder")) && (Key->GetClassName() != classnameTHStack("THStack"))) {
            HistogramFound = true;

            if (PrintOut) { cout << "\n\nKey name: " << Histogram2DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n\n"; }

            Histogram2D = (TH2D *) Key->ReadObj();
            FoundHistClass = Key->GetClassName();
            FoundHistName = Histogram2D->GetName();
            break;

            /*
            string Histogram2DxLable = Histogram2DTemp->GetXaxis()->GetTitle();
            string Histogram2DTitle = Histogram2DTemp->GetTitle();

            if (PrintOut) {
                cout << "\nHistogram2DxLable = " << Histogram2DxLable << "\n";
                cout << "Histogram2DTitle = " << Histogram2DTitle << "\n";
                cout << "TLmom = " << TLmom << "\n";
            }

            if ((TLmom || !findSubstring(Histogram2DxLable, "Momentum"))) {
                HistogramFound = true;

                Histogram2D = (TH2D *) Key->ReadObj();
                FoundHistClass = Key->GetClassName();
                break;
            }
            */
        }
    }

    if (!HistogramFound) {
        cout << "\n\nHistofinder2D: could not find histogram!\n";
        cout << "Histogram2DNameSubstring = " << Histogram2DNameSubstring << "\n";
        exit(0);

        return Histogram2D;
    } else {
        if (PrintOut) {
            cout << "\n\nHistofinder2D: histogram found!\n";
            cout << "FoundHistClass = " << FoundHistClass << "\n";
            cout << "FoundHistName = " << FoundHistName << "\n";
        }

        return Histogram2D;
    }
}
