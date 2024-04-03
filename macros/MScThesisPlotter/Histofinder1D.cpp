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

TH1D *Histofinder1D(TFile *file, const char *Histogram1DNameSubstring, const bool TLmom = false) {
    bool PrintOut = false;
    bool PrintOut1 = false;

    TH1D *Histogram1D;

    bool HistogramFound = false;
    int Counter = 0, HistogramCounter = 0;
    TString classname("TH1D");
    TString classnameTH2D("TH2D");
    TString classnameTFolder("TFolder");
    TString classnameTHStack("THStack");
    string FoundHistName;

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while ((Key = (TKey *) Next())) {
        string Histogram1DTempName = ((TH1D *) Key->ReadObj())->GetName();

        if (PrintOut1) { cout << Histogram1DTempName << "\n\n"; }

        if (findSubstring(Histogram1DTempName, Histogram1DNameSubstring) &&
            (Key->GetClassName() != classnameTH2D("TH2D")) &&
            (Key->GetClassName() != classnameTFolder("TFolder")) &&
            (Key->GetClassName() != classnameTHStack("THStack"))) {

            if (PrintOut) { cout << "\n\nKey name: " << ((TH1D *) Key->ReadObj())->GetName() << "; Type: " << Key->GetClassName() << "\n\n"; }

            string Histogram1DxLable = ((TH1D *) Key->ReadObj())->GetXaxis()->GetTitle();
            string Histogram1DTitle = ((TH1D *) Key->ReadObj())->GetTitle();

            if (PrintOut) {
                cout << "\nHistogram1DxLable = " << Histogram1DxLable << "\n";
                cout << "Histogram1DTitle = " << Histogram1DTitle << "\n";
                cout << "TLmom = " << TLmom << "\n";
            }

            if ((TLmom || !findSubstring(Histogram1DxLable, "Momentum"))) {
                HistogramFound = true;

                Histogram1D = ((TH1D *) Key->ReadObj());
                FoundHistName = Key->GetClassName();
                break;
            }
        }
    }

    if (!HistogramFound) {
        cout << "\n\nHistofinder1D: could not find histogram!\n";
        cout << "TLmom = " << TLmom << "\n";
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
