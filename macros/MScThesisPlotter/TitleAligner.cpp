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

void TitleAligner(hData &Utilities, TH1D *Sim_Histogram1D, TH1D *Data_Histogram1D, const string &OriginToreplace, const string &Replacement) {
    string Sim_Histogram1D_Title = Sim_Histogram1D->GetTitle();
    string Sim_Histogram1D_xLabel = Sim_Histogram1D->GetXaxis()->GetTitle();
    string Sim_Histogram1D_yLabel = Sim_Histogram1D->GetYaxis()->GetTitle();

    if (findSubstring(Sim_Histogram1D_Title, OriginToreplace)) {
        Utilities.ReplaceSubStr(Sim_Histogram1D_Title, OriginToreplace, Replacement);
        Sim_Histogram1D->SetTitle(Sim_Histogram1D_Title.c_str());
    }

    if (findSubstring(Sim_Histogram1D_xLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Sim_Histogram1D_xLabel, OriginToreplace, Replacement);
        Sim_Histogram1D->GetXaxis()->SetTitle(Sim_Histogram1D_xLabel.c_str());
    }

    if (findSubstring(Sim_Histogram1D_yLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Sim_Histogram1D_yLabel, OriginToreplace, Replacement);
        Sim_Histogram1D->GetYaxis()->SetTitle(Sim_Histogram1D_yLabel.c_str());
    }

    string Data_Histogram1D_Title = Data_Histogram1D->GetTitle();
    string Data_Histogram1D_xLabel = Data_Histogram1D->GetXaxis()->GetTitle();
    string Data_Histogram1D_yLabel = Data_Histogram1D->GetYaxis()->GetTitle();

    if (findSubstring(Data_Histogram1D_Title, OriginToreplace)) {
        Utilities.ReplaceSubStr(Data_Histogram1D_Title, OriginToreplace, Replacement);
        Data_Histogram1D->SetTitle(Data_Histogram1D_Title.c_str());
    }

    if (findSubstring(Data_Histogram1D_xLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Data_Histogram1D_xLabel, OriginToreplace, Replacement);
        Data_Histogram1D->GetXaxis()->SetTitle(Data_Histogram1D_xLabel.c_str());
    }

    if (findSubstring(Data_Histogram1D_yLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Data_Histogram1D_yLabel, OriginToreplace, Replacement);
        Data_Histogram1D->GetYaxis()->SetTitle(Data_Histogram1D_yLabel.c_str());
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

void TitleAligner(hData &Utilities, TGraph *Graph1D, string &Graph1D_Title, string &Graph1D_xLabel, const string &OriginToreplace, const string &Replacement) {
    if (findSubstring(Graph1D_Title, OriginToreplace)) {
        Utilities.ReplaceSubStr(Graph1D_Title, OriginToreplace, Replacement);
        Graph1D->SetTitle(Graph1D_Title.c_str());
    }

    if (findSubstring(Graph1D_xLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Graph1D_xLabel, OriginToreplace, Replacement);
        Graph1D->GetXaxis()->SetTitle(Graph1D_xLabel.c_str());
    }
}

void TitleAligner(hData &Utilities, TGraph *Graph1D, string &Graph1D_Title, string &Graph1D_xLabel, string &Graph1D_yLabel,
                  const string &OriginToreplace, const string &Replacement) {
    if (findSubstring(Graph1D_Title, OriginToreplace)) {
        Utilities.ReplaceSubStr(Graph1D_Title, OriginToreplace, Replacement);
        Graph1D->SetTitle(Graph1D_Title.c_str());
    }

    if (findSubstring(Graph1D_xLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Graph1D_xLabel, OriginToreplace, Replacement);
        Graph1D->GetXaxis()->SetTitle(Graph1D_xLabel.c_str());
    }

    if (findSubstring(Graph1D_yLabel, OriginToreplace)) {
        Utilities.ReplaceSubStr(Graph1D_yLabel, OriginToreplace, Replacement);
        Graph1D->GetYaxis()->SetTitle(Graph1D_yLabel.c_str());
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
