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

TH1D *Histofinder1D(const char *filename, const char *Histogram1DNameSubstring, const bool TLmom = false) {
    bool PrintOut = false;
    bool PrintOut1 = false;

    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    TH1D *Histogram1D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    static TString classname("TH1D");
    static TString classnameTFolder("TFolder");
    static TString classnameTHStack("THStack");
    string FoundHistName;

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while (Key = (TKey *) Next()) {
        TH1D *Histogram1DTemp = (TH1D *) Key->ReadObj();

        string Histogram1DTempName = Histogram1DTemp->GetName();

        if (PrintOut1) { cout << Histogram1DTempName << "\n\n"; }

        if (findSubstring(Histogram1DTempName, Histogram1DNameSubstring) &&
            (Key->GetClassName() != classnameTFolder("TFolder")) && (Key->GetClassName() != classnameTHStack("THStack"))) {

            if (PrintOut) { cout << "\n\nKey name: " << Histogram1DTemp->GetName() << "; Type: " << Key->GetClassName() << "\n\n"; }

            string Histogram1DxLable = Histogram1DTemp->GetXaxis()->GetTitle();
            string Histogram1DTitle = Histogram1DTemp->GetTitle();

            if (PrintOut) {
                cout << "\nHistogram1DxLable = " << Histogram1DxLable << "\n";
                cout << "Histogram1DTitle = " << Histogram1DTitle << "\n";
                cout << "TLmom = " << TLmom << "\n";
            }

            if ((TLmom || !findSubstring(Histogram1DxLable, "Momentum"))) {
                HistogramFound = true;

                Histogram1D = (TH1D *) Key->ReadObj();

                FoundHistName = Key->GetClassName();
                break;
            }
        }
    }

    if (!HistogramFound) {
        cout << "\n\nHistofinder1D: could not find histogram!\n";
        cout << "Histogram1DNameSubstring = " << Histogram1DNameSubstring << "\n";
        exit(0);
        return Histogram1D;
    } else {
        if (PrintOut) {
            cout << "\n\nHistofinder1D: histogram found!\n";
            cout << "FoundHistName = " << FoundHistName << "\n";
        }

        return Histogram1D;
    }
}
