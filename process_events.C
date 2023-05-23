#include<iostream>
#include<cstdio>
#include<fstream>
#include<cmath>
#include<TH1.h>
#include<TLine.h>
#include<THStack.h>
#include<TLatex.h>
#include<TFile.h>
#include<TDirectoryFile.h>
#include<TGraph.h>
#include<TTree.h>
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;

void process_events() {

    string fileName = "Downloads/C12_G18_10a_02_11b_207052MeV.root";

    cout << "\n\nLoaded file:\t" << fileName << "\n";

    double beamE = 2.07052;
    double pi = 3.14159265359;

    TH1D *h1 = new TH1D("#omega for (e,e')", "#omega for (e,e')", 100, 0, beamE * 1.1);
    TH1D *h2 = new TH1D("#omega for (e,e'p)", "#omega for (e,e'p)", 100, 0, beamE * 1.1);
    TH1D *h3 = new TH1D("#omega for (e,e'p)X", "#omega for (e,e'p)X", 100, 0, beamE * 1.1);
    TH1D *h4 = new TH1D("#omega for (e,e'FDp)X", "#omega for (e,e'FDp)X", 100, 0, beamE * 1.1);
    TH1D *h5 = new TH1D("#omega for (e,e'pFD)X", "#omega for (e,e'pFD)X", 100, 0, beamE * 1.1);
    TH1D *h6 = new TH1D("#omega for (e,eFD'pFD)X", "#omega for (e,eFD'pFD)X", 100, 0, beamE * 1.1);

    TH1D *h7 = new TH1D("#omega for (e,e'p) w/P_{p}#leq0.3 GeV", "#omega for (e,e'p) w/P_{p}#leq0.3 GeV", 100, 0, beamE * 1.1);
    TH1D *h8 = new TH1D("#omega for (e,e'p)X w/P_{p}#leq0.3 GeV", "#omega for (e,e'p)X w/P_{p}#leq0.3 GeV", 100, 0, beamE * 1.1);
    TH1D *h9 = new TH1D("#omega for (e,e'FDp)X w/P_{p}#leq0.3 GeV", "#omega for (e,e'FDp)X w/P_{p}#leq0.3 GeV", 100, 0, beamE * 1.1);
    TH1D *h10 = new TH1D("#omega for (e,e'pFD)X w/P_{p}#leq0.3 GeV", "#omega for (e,e'pFD)X w/P_{p}#leq0.3 GeV", 100, 0, beamE * 1.1);
    TH1D *h11 = new TH1D("#omega for (e,eFD'pFD)X w/P_{p}#leq0.3 GeV", "#omega for (e,eFD'pFD)X w/P_{p}#leq0.3 GeV", 100, 0, beamE * 1.1);

    TH1D *h12 = new TH1D("#omega for (e,e'p) w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", "#omega for (e,e'p) w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", 100, 0,
                         beamE * 1.1);
    TH1D *h13 = new TH1D("#omega for (e,e'p)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", "#omega for (e,e'p)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", 100, 0,
                         beamE * 1.1);
    TH1D *h14 = new TH1D("#omega for (e,e'FDp)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", "#omega for (e,e'FDp)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", 100, 0,
                         beamE * 1.1);
    TH1D *h15 = new TH1D("#omega for (e,e'pFD)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", "#omega for (e,e'pFD)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", 100, 0,
                         beamE * 1.1);
    TH1D *h16 = new TH1D("#omega for (e,eFD'pFD)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", "#omega for (e,eFD'pFD)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma", 100, 0,
                         beamE * 1.1);

    TFile *file = new TFile(fileName.c_str(), "fileName");
    TTree *gst = (TTree *) file->Get("gst");

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    c1->SetGrid();

    //<editor-fold desc="Declaration of leaf types">
    // Declaration of leaf types
    Int_t iev;
    Int_t neu;
    Int_t fspl;
    Int_t tgt;
    Int_t Z;
    Int_t A;
    Int_t hitnuc;
    Int_t hitqrk;
    Int_t resid;
    Bool_t sea;
    Bool_t qel;
    Bool_t mec;
    Bool_t res;
    Bool_t dis;
    Bool_t coh;
    Bool_t dfr;
    Bool_t imd;
    Bool_t imdanh;
    Bool_t singlek;
    Bool_t nuel;
    Bool_t em;
    Bool_t cc;
    Bool_t nc;
    Bool_t charm;
    Bool_t amnugamma;
    Int_t neut_code;
    Int_t nuance_code;
    Double_t wght;
    Double_t xs;
    Double_t ys;
    Double_t ts;
    Double_t Q2s;
    Double_t Ws;
    Double_t x;
    Double_t y;
    Double_t t;
    Double_t Q2;
    Double_t W;
    Double_t EvRF;
    Double_t Ev;
    Double_t pxv;
    Double_t pyv;
    Double_t pzv;
    Double_t En;
    Double_t pxn;
    Double_t pyn;
    Double_t pzn;
    Double_t El;
    Double_t pxl;
    Double_t pyl;
    Double_t pzl;
    Double_t pl;
    Double_t cthl;
    Int_t nfp;
    Int_t nfn;
    Int_t nfpip;
    Int_t nfpim;
    Int_t nfpi0;
    Int_t nfkp;
    Int_t nfkm;
    Int_t nfk0;
    Int_t nfem;
    Int_t nfother;
    Int_t nip;
    Int_t nin;
    Int_t nipip;
    Int_t nipim;
    Int_t nipi0;
    Int_t nikp;
    Int_t nikm;
    Int_t nik0;
    Int_t niem;
    Int_t niother;
    Int_t ni;
    Int_t pdgi[6];   //[ni]
    Int_t resc[6];   //[ni]
    Double_t Ei[6];   //[ni]
    Double_t pxi[6];   //[ni]
    Double_t pyi[6];   //[ni]
    Double_t pzi[6];   //[ni]
    Int_t nf;
    Int_t pdgf[nf];   //[nf]
    Double_t Ef[nf];   //[nf]
    Double_t pxf[nf];   //[nf]
    Double_t pyf[nf];   //[nf]
    Double_t pzf[nf];   //[nf]
    Double_t pf[nf];   //[nf]
    Double_t cthf[nf];   //[nf]
//    Int_t pdgf[15];   //[nf]
//    Double_t Ef[15];   //[nf]
//    Double_t pxf[15];   //[nf]
//    Double_t pyf[15];   //[nf]
//    Double_t pzf[15];   //[nf]
//    Double_t pf[15];   //[nf]
//    Double_t cthf[15];   //[nf]
    Double_t vtxx;
    Double_t vtxy;
    Double_t vtxz;
    Double_t vtxt;
    Double_t sumKEf;
    Double_t calresp0;

// List of branches
    TBranch *b_iev;   //!
    TBranch *b_neu;   //!
    TBranch *b_fspl;   //!
    TBranch *b_tgt;   //!
    TBranch *b_Z;   //!
    TBranch *b_A;   //!
    TBranch *b_hitnuc;   //!
    TBranch *b_hitqrk;   //!
    TBranch *b_resid;   //!
    TBranch *b_sea;   //!
    TBranch *b_qel;   //!
    TBranch *b_mec;   //!
    TBranch *b_res;   //!
    TBranch *b_dis;   //!
    TBranch *b_coh;   //!
    TBranch *b_dfr;   //!
    TBranch *b_imd;   //!
    TBranch *b_imdanh;   //!
    TBranch *b_singlek;   //!
    TBranch *b_nuel;   //!
    TBranch *b_em;   //!
    TBranch *b_cc;   //!
    TBranch *b_nc;   //!
    TBranch *b_charm;   //!
    TBranch *b_amnugamma;   //!
    TBranch *b_neut_code;   //!
    TBranch *b_nuance_code;   //!
    TBranch *b_wght;   //!
    TBranch *b_xs;   //!
    TBranch *b_ys;   //!
    TBranch *b_ts;   //!
    TBranch *b_Q2s;   //!
    TBranch *b_Ws;   //!
    TBranch *b_x;   //!
    TBranch *b_y;   //!
    TBranch *b_t;   //!
    TBranch *b_Q2;   //!
    TBranch *b_W;   //!
    TBranch *b_EvRF;   //!
    TBranch *b_Ev;   //!
    TBranch *b_pxv;   //!
    TBranch *b_pyv;   //!
    TBranch *b_pzv;   //!
    TBranch *b_En;   //!
    TBranch *b_pxn;   //!
    TBranch *b_pyn;   //!
    TBranch *b_pzn;   //!
    TBranch *b_El;   //!
    TBranch *b_pxl;   //!
    TBranch *b_pyl;   //!
    TBranch *b_pzl;   //!
    TBranch *b_pl;   //!
    TBranch *b_cthl;   //!
    TBranch *b_nfp;   //!
    TBranch *b_nfn;   //!
    TBranch *b_nfpip;   //!
    TBranch *b_nfpim;   //!
    TBranch *b_nfpi0;   //!
    TBranch *b_nfkp;   //!
    TBranch *b_nfkm;   //!
    TBranch *b_nfk0;   //!
    TBranch *b_nfem;   //!
    TBranch *b_nfother;   //!
    TBranch *b_nip;   //!
    TBranch *b_nin;   //!
    TBranch *b_nipip;   //!
    TBranch *b_nipim;   //!
    TBranch *b_nipi0;   //!
    TBranch *b_nikp;   //!
    TBranch *b_nikm;   //!
    TBranch *b_nik0;   //!
    TBranch *b_niem;   //!
    TBranch *b_niother;   //!
    TBranch *b_ni;   //!
    TBranch *b_pdgi;   //!
    TBranch *b_resc;   //!
    TBranch *b_Ei;   //!
    TBranch *b_pxi;   //!
    TBranch *b_pyi;   //!
    TBranch *b_pzi;   //!
    TBranch *b_nf;   //!
    TBranch *b_pdgf;   //!
    TBranch *b_Ef;   //!
    TBranch *b_pxf;   //!
    TBranch *b_pyf;   //!
    TBranch *b_pzf;   //!
    TBranch *b_pf;   //!
    TBranch *b_cthf;   //!
    TBranch *b_vtxx;   //!
    TBranch *b_vtxy;   //!
    TBranch *b_vtxz;   //!
    TBranch *b_vtxt;   //!
    TBranch *b_sumKEf;   //!
    TBranch *b_calresp0;   //!
    //</editor-fold>

    //<editor-fold desc="Setting branch pointers">
    gst->SetBranchAddress("cc", &cc);
    gst->SetBranchAddress("qel", &qel);
    gst->SetBranchAddress("mec", &mec);
    gst->SetBranchAddress("res", &res);
    gst->SetBranchAddress("dis", &dis);
    gst->SetBranchAddress("W", &W);
    gst->SetBranchAddress("Q2", &Q2);
    gst->SetBranchAddress("El", &El);
    gst->SetBranchAddress("pxl", &pxl);
    gst->SetBranchAddress("pyl", &pyl);
    gst->SetBranchAddress("pzl", &pzl);
    gst->SetBranchAddress("Ev", &Ev);
    gst->SetBranchAddress("pxv", &pxv);
    gst->SetBranchAddress("pyv", &pyv);
    gst->SetBranchAddress("pzv", &pzv);
    gst->SetBranchAddress("Ef", &Ef);
    gst->SetBranchAddress("pf", &pf);
    gst->SetBranchAddress("pdgf", &pdgf);
    gst->SetBranchAddress("pxf", &pxf);
    gst->SetBranchAddress("pyf", &pyf);
    gst->SetBranchAddress("pzf", &pzf);
    gst->SetBranchAddress("nf", &nf);
    gst->SetBranchAddress("nfp", &nfp);
    gst->SetBranchAddress("nfn", &nfn);
    gst->SetBranchAddress("nfpip", &nfpip);
    gst->SetBranchAddress("nfpim", &nfpim);
    gst->SetBranchAddress("nfpi0", &nfpi0);
    gst->SetBranchAddress("nfkp", &nfkp);
    gst->SetBranchAddress("nfkm", &nfkm);
    gst->SetBranchAddress("nfk0", &nfk0);
    gst->SetBranchAddress("nfem", &nfem);
    gst->SetBranchAddress("nfother", &nfother);
    //</editor-fold>

    const long nevents = gst->GetEntries();

    for (int i = 0; i < nevents; ++i) {
        gst->GetEntry(i);

        double theta_lp = acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180 / pi;
        double omega = Ev - El;

        bool lp_in_FD = (theta_lp >= 5. && theta_lp <= 40.);

        h1->Fill(omega); // omega for (e,e')

        if (nfp == 1) {

            int prot_ind;

            int numOfPip_mom = 0;
            int numOfPim_mom = 0;
            int numOfPi0_mom = 0;
            int numOfPi0_mom_FD = 0;
            int numOfPh_mom = 0;
            int numOfPh_mom_FD = 0;

//            cout << "\n\n\nTEST TEST TEST\n\n\n";
//            cout << "\n\n\nnf = " << nf << "\n\n\n";

            for (int i = 0; i < nf; ++i) {
//            for (int i = 0; i < pf.size(); ++i) {

                double theta_part = acos(pzf[i] / sqrt(pxf[i] * pxf[i] + pyf[i] * pyf[i] + pzf[i] * pzf[i])) * 180 / pi;
                bool part_in_FD = (theta_part >= 5. && theta_part <= 40.);

                if (pdgf[i] == 2212) {
                    prot_ind = i;
                } else if (pdgf[i] == 211 && (sqrt(pxf[i] * pxf[i] + pyf[i] * pyf[i] + pzf[i] * pzf[i]) >= 0.2)) {
                    ++numOfPip_mom;
                } else if (pdgf[i] == -211 && (sqrt(pxf[i] * pxf[i] + pyf[i] * pyf[i] + pzf[i] * pzf[i]) >= 0.2)) {
                    ++numOfPim_mom;
                } else if (pdgf[i] == 111 && (sqrt(pxf[i] * pxf[i] + pyf[i] * pyf[i] + pzf[i] * pzf[i]) >= 0.58)) {
                    ++numOfPi0_mom;

                    if (part_in_FD) { ++numOfPi0_mom_FD; }
                } else if (pdgf[i] == 22 && (sqrt(pxf[i] * pxf[i] + pyf[i] * pyf[i] + pzf[i] * pzf[i]) >= 0.3)) {
                    ++numOfPh_mom;

                    if (part_in_FD) { ++numOfPh_mom_FD; }
                }
            }

            double theta_p = acos(pzf[prot_ind] / sqrt(pxf[prot_ind] * pxf[prot_ind] + pyf[prot_ind] * pyf[prot_ind] + pzf[prot_ind] * pzf[prot_ind])) * 180 / pi;
            double P_p = sqrt(pxf[prot_ind] * pxf[prot_ind] + pyf[prot_ind] * pyf[prot_ind] + pzf[prot_ind] * pzf[prot_ind]);

            bool p_in_FD = (theta_p >= 5. && theta_p <= 40.);

            h3->Fill(omega); // omega for (e,e'p)X

            if (nf == 1) {
                h2->Fill(omega); // omega for (e,e'p)
            }

            if (lp_in_FD) {
                h4->Fill(omega); // omega for (e,e'FDp)X
            }

            if (p_in_FD) {
                h4->Fill(omega); // omega for (e,e'pFD)X
            }

            if (lp_in_FD && p_in_FD) {
                h5->Fill(omega); // omega for (e,e'pFD)X
            }

            if (P_p <= 0.3) {
                if (nf == 1) {
                    h7->Fill(omega); // omega for (e,e'p) w/P_{p}#leq0.3 GeV
                }

                h8->Fill(omega); // omega for (e,e'p)X w/P_{p}#leq0.3 GeV

                if (lp_in_FD) {
                    h9->Fill(omega); // omega for (e,e'FDp)X w/P_{p}#leq0.3 GeV
                }

                if (p_in_FD) {
                    h10->Fill(omega); // omega for (e,e'pFD)X w/P_{p}#leq0.3 GeV
                }

                if (lp_in_FD && p_in_FD) {
                    h11->Fill(omega); // omega for (e,eFD'pFD)X w/P_{p}#leq0.3 GeV
                }

                if (numOfPi0_mom_FD == 0 && numOfPh_mom_FD == 0) {
                    if (nf == 1) {
                        h12->Fill(omega); // omega for (e,e'p) w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma
                    }

                    h13->Fill(omega); // omega for (e,e'p)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma

                    if (lp_in_FD) {
                        h14->Fill(omega); // omega for (e,e'FDp)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma
                    }

                    if (p_in_FD) {
                        h15->Fill(omega); // omega for (e,e'pFD)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma
                    }

                    if (lp_in_FD && p_in_FD) {
                        h16->Fill(omega); // omega for (e,eFD'pFD)X w/P_{p}#leq0.3 GeV & no FD #pi^{0},#gamma
                    }
                }
            }
        }
    }

    h1->Draw();
    c1->SaveAs("h1.png");
    c1->Clear();

    h2->Draw();
    c1->SaveAs("h2.png");
    c1->Clear();

    h3->Draw();
    c1->SaveAs("h3.png");
    c1->Clear();

    h4->Draw();
    c1->SaveAs("h4.png");
    c1->Clear();

    h5->Draw();
    c1->SaveAs("h5.png");
    c1->Clear();

    h6->Draw();
    c1->SaveAs("h6.png");
    c1->Clear();

    h7->Draw();
    c1->SaveAs("h7.png");
    c1->Clear();

    h8->Draw();
    c1->SaveAs("h8.png");
    c1->Clear();

    h9->Draw();
    c1->SaveAs("h9.png");
    c1->Clear();

    h10->Draw();
    c1->SaveAs("h10.png");
    c1->Clear();

    h11->Draw();
    c1->SaveAs("h11.png");
    c1->Clear();

    h12->Draw();
    c1->SaveAs("h12.png");
    c1->Clear();

    h13->Draw();
    c1->SaveAs("h13.png");
    c1->Clear();

    h14->Draw();
    c1->SaveAs("h14.png");
    c1->Clear();

    h15->Draw();
    c1->SaveAs("h15.png");
    c1->Clear();

    h16->Draw();
    c1->SaveAs("h16.png");
    c1->Clear();

    cout << "\n\nFinished!\n\n";
}