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

#include "../../source/classes/hData/hData.h"
#include "../../source/functions/GeneralFunctions.h"

using namespace std;

void TitleAligner(hData &Utilities, TH1D *Histogram1D, string &Histogram1D_Title, string &Histogram1D_xLabel, const string &OriginToreplace, const string &Replacement) {
    if (findSubstring(Histogram1D_Title, OriginToreplace)) {
        Utilities.ReplaceSubStr(Histogram1D_Title, OriginToreplace, Replacement);
        Histogram1D->SetTitle(Histogram1D_Title.c_str());
    }

    if (findSubstring(Histogram1D_xLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Histogram1D_xLabel, OriginToreplace, Replacement);
        Histogram1D->GetXaxis()->SetTitle(Histogram1D_xLabel.c_str());
    }
}

void TitleAligner(hData &Utilities, THStack *Stack1D, string &Histogram1D_Title, string &Histogram1D_xLabel, const string &OriginToreplace, const string &Replacement) {
    if (findSubstring(Histogram1D_Title, OriginToreplace)) {
        Utilities.ReplaceSubStr(Histogram1D_Title, OriginToreplace, Replacement);
        Stack1D->SetTitle(Histogram1D_Title.c_str());
    }

    if (findSubstring(Histogram1D_xLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Histogram1D_xLabel, OriginToreplace, Replacement);
        Stack1D->GetXaxis()->SetTitle(Histogram1D_xLabel.c_str());
    }
}

void TitleAligner(TH2D *Histogram2D, string &Histogram2D_Title, string &Histogram2D_xLabel, string &Histogram2D_yLabel, const string &OriginToreplace,
             const string &Replacement) {
    hData Utilities;

    if (findSubstring(Histogram2D_Title, OriginToreplace)) {
        Utilities.ReplaceSubStr(Histogram2D_Title, OriginToreplace, Replacement);
        Histogram2D->SetTitle(Histogram2D_Title.c_str());
    }

    if (findSubstring(Histogram2D_xLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Histogram2D_xLabel, OriginToreplace, Replacement);
        Histogram2D->GetXaxis()->SetTitle(Histogram2D_xLabel.c_str());
    }

    if (findSubstring(Histogram2D_yLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Histogram2D_yLabel, OriginToreplace, Replacement);
        Histogram2D->GetYaxis()->SetTitle(Histogram2D_yLabel.c_str());
    }
}
