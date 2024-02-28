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

TH2D *Histofinder2D(TFile *file, const char *Histogram2DNameSubstring) {
    bool PrintOut = false;
    bool PrintOutResult = false;

    TH2D *Histogram2D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    TString classname("TH2D");
//    static TString classname("TH2D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());
    TH2D *Histogram2DTemp;

    while ((Key = (TKey *) Next())) {
        Histogram2DTemp = (TH2D *) Key->ReadObj();

        string Histogram2DTempName = Histogram2DTemp->GetName();

        if (PrintOut) { cout << Histogram2DTempName << "\n\n"; }

        if (findSubstring(Histogram2DTempName, Histogram2DNameSubstring)) {
            HistogramFound = true;

            if (PrintOutResult) {
                cout << "\n\nKey name: " << Histogram2DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n";
                cout << "Histogram2DNameSubstring: " << Histogram2DNameSubstring << "\n\n";
            }

            Histogram2D = (TH2D *) Key->ReadObj();
            break;
        }
    }

    if (!HistogramFound) {
        cout << "\n\nHistofinder2D: could not find histogram!\n";
        exit(0);

        return Histogram2D;
    } else {
        return Histogram2D;
    }
}

TH2D *Histofinder2D(TFile *file, const char *Histogram2DNameSubstring, const bool TLmom) {
    bool PrintOut = false;
    bool PrintOut1 = false;

    TH2D *Histogram2D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    TString classname("TH2D");
    TString classnameTFolder("TFolder");
    TString classnameTHStack("THStack");
//    static TString classname("TH2D");
//    static TString classnameTFolder("TFolder");
//    static TString classnameTHStack("THStack");
    string FoundHistClass;
    string FoundHistName;

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());
    TH2D *Histogram2DTemp;

    while ((Key = (TKey *) Next())) {

        string Histogram2DTempName = ((TH2D *) Key->ReadObj())->GetName();

        if (PrintOut1) { cout << Histogram2DTempName << "\n\n"; }

        if (findSubstring(Histogram2DTempName, Histogram2DNameSubstring) &&
            (Key->GetClassName() != classnameTFolder("TFolder")) && (Key->GetClassName() != classnameTHStack("THStack"))) {
            HistogramFound = true;

            if (PrintOut) { cout << "\n\nKey name: " << ((TH2D *) Key->ReadObj())->GetName() << "; Type: " << Key->GetClassName() << "\n\n"; }

            Histogram2D = (TH2D *) Key->ReadObj();
            FoundHistClass = Key->GetClassName();
            FoundHistName = Histogram2D->GetName();
            break;
        }
    }
    /*
        while ((Key = (TKey *) Next())) {
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
            }
        }
    */

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
