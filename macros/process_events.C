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

    system("mkdir -p TL_plots");
    system("mkdir -p TL_plots/Momentum_plots_1e_cut");
    system("mkdir -p TL_plots/Momentum_plots_1e1pX");
    system("mkdir -p TL_plots/Momentum_plots_1e1p");

    TH1D *P_p_1e_cut_FD = new TH1D("Proton momentum BC for 5#circ<#theta_{p}<40#circ (1e cut)",
                                   "Proton momentum BC for 5#circ<#theta_{p}<40#circ (1e cut); P_{p} [GeV]", 100, 0, beamE * 1.1);
    TH1D *P_p_1e1pX_FD = new TH1D("Proton momentum BC for 5#circ<#theta_{p}<40#circ (1e1pX)",
                                  "Proton momentum BC for 5#circ<#theta_{p}<40#circ (1e1pX); P_{p} [GeV]", 100, 0, beamE * 1.1);
    TH1D *P_p_1e1p_FD = new TH1D("Proton momentum BC for 5#circ<#theta_{p}<40#circ (1e1p)",
                                 "Proton momentum BC for 5#circ<#theta_{p}<40#circ (1e1p); P_{p} [GeV]", 100, 0, beamE * 1.1);

    TH1D *_1eInclusive = new TH1D("#omega for (e,e')", "#omega for (e,e'); #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e')
    TH1D *_1e1p = new TH1D("#omega for 1e1p", "#omega for 1e1p; #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'p)
    TH1D *_1e1pX = new TH1D("#omega for 1e1pX", "#omega for 1e1pX; #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'p)X
    TH1D *_1eFD1pX = new TH1D("#omega for 1e{FD}1pX", "#omega for 1e{FD}1pX; #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'FDp)X
    TH1D *_1e1pFDX = new TH1D("#omega for 1e1p{FD}X", "#omega for 1e1p{FD}X; #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'pFD)X
    TH1D *_1eFD1pFDX = new TH1D("#omega for 1e{FD}1p{FD}X", "#omega for 1e{FD}1p{FD}X; #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,eFD'pFD)X

    TH1D *_1e1p_wPpth = new TH1D("#omega for 1e1p w/P_{p}#geq0.3 GeV", "#omega for 1e1p w/P_{p}#geq0.3 GeV; #omega = Ev - El [GeV]",
                                 100, 0, beamE * 1.1); // (e,e'p)
    TH1D *_1e1pX_wPpth = new TH1D("#omega for 1e1pX w/P_{p}#geq0.3 GeV", "#omega for 1e1pX w/P_{p}#geq0.3 GeV; #omega = Ev - El [GeV]",
                                  100, 0, beamE * 1.1); // (e,e'p)
    TH1D *_1eFD1pX_wPpth = new TH1D("#omega for 1e{FD}1pX w/P_{p}#geq0.3 GeV", "#omega for 1e{FD}1pX w/P_{p}#geq0.3 GeV; #omega = Ev - El [GeV]",
                                    100, 0, beamE * 1.1); // (e,e'FDp)X
    TH1D *_1e1pFDX_wPpth = new TH1D("#omega for 1e1p{FD}X w/P_{p}#geq0.3 GeV", "#omega for 1e1p{FD}X w/P_{p}#geq0.3 GeV; #omega = Ev - El [GeV]",
                                    100, 0, beamE * 1.1); // (e,e'pFD)X
    TH1D *_1eFD1pFDX_wPpth = new TH1D("#omega for 1e{FD}1p{FD}X w/P_{p}#geq0.3 GeV", "#omega for 1e{FD}1p{FD}X w/P_{p}#geq0.3 GeV; #omega = Ev - El [GeV]",
                                      100, 0, beamE * 1.1); // (e,eFD'pFD)X

    TH1D *_1e1p0pizeroFD0phFDX_wPpth_wPphth_wPpi0th = new TH1D("#omega for 1e1p0#pi^{0}{FD}0#gamma{FD}X w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV",
                                                               "#omega for 1e1p0#pi^{0}{FD}0#gamma{FD} w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV;"
                                                               " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'p)
    TH1D *_1eFD1p0pizeroFD0phFDX_wPpth_wPphth_wPpi0th = new TH1D("#omega for 1e{FD}1p0#pi^{0}{FD}0#gamma{FD}X w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV",
                                                                 "#omega for 1e{FD}1p0#pi^{0}{FD}0#gamma{FD}X w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV;"
                                                                 " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'FDp)X
    TH1D *_1e1pFD0pizeroFD0phFDX_wPpth_wPphth_wPpi0th = new TH1D("#omega for 1e1p{FD}0#pi^{0}{FD}0#gamma{FD}X w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV",
                                                                 "#omega for 1e1p{FD}0#pi^{0}{FD}0#gamma{FD}X w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV;"
                                                                 " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'pFD)X
    TH1D *_1eFD1pFD0pizeroFD0phFDX_wPpth_wPphth_wPpi0th = new TH1D("#omega for 1e{FD}1p{FD}0#pi^{0}{FD}0#gamma{FD}X"
                                                                   " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV",
                                                                   "#omega for 1e{FD}1p{FD}0#pi^{0}{FD}0#gamma{FD}X"
                                                                   " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV; #omega = Ev - El [GeV]",
                                                                   100, 0, beamE * 1.1); // (e,eFD'pFD)X

    TH1D *_1e1p0pizeroFD0phFD0cPiX_wPpth_wPphth_wPpi0th_wKCpith = new TH1D("#omega for 1e1p0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}X"
                                                                           " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV",
                                                                           "#omega for 1e1p0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}X"
                                                                           " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV;"
                                                                           " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'p)
    TH1D *_1eFD1p0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith = new TH1D("#omega for 1e{FD}1p0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}X"
                                                                            " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV",
                                                                            "#omega for 1e{FD}1p0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}X"
                                                                            " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV;"
                                                                            " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'FDp)X
    TH1D *_1e1pFD0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith = new TH1D("#omega for 1e1p{FD}0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}X"
                                                                            " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV",
                                                                            "#omega for 1e1p{FD}0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}X"
                                                                            " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV;"
                                                                            " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'pFD)X
    TH1D *_1eFD1pFD0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith = new TH1D("#omega for 1e{FD}1p{FD}0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}X"
                                                                              " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV",
                                                                              "#omega for 1e{FD}1p{FD}0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}X"
                                                                              " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV;"
                                                                              " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,eFD'pFD)X

    TH1D *f_1e1p0pizeroFD0phFD0cPiXn_wPpth_wPphth_wPpi0th_wKCpith = new TH1D("#omega for 1e1p0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}Xn"
                                                                             " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV",
                                                                             "#omega for 1e1p0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}Xn"
                                                                             " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV;"
                                                                             " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'p)
    TH1D *f_1eFD1p0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith = new TH1D("#omega for 1e{FD}1p0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}Xn"
                                                                              " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV",
                                                                              "#omega for 1e{FD}1p0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}Xn"
                                                                              " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV;"
                                                                              " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'FDp)X
    TH1D *f_1e1pFD0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith = new TH1D("#omega for 1e1p{FD}0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}Xn"
                                                                              " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV",
                                                                              "#omega for 1e1p{FD}0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}Xn"
                                                                              " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV;"
                                                                              " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,e'pFD)X
    TH1D *f_1eFD1pFD0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith = new TH1D("#omega for 1e{FD}1p{FD}0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}Xn"
                                                                                " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV",
                                                                                "#omega for 1e{FD}1p{FD}0#pi^{0}{FD}0#gamma{FD}0#pi^{#pm}Xn"
                                                                                " w/P_{p}#geq0.3, P_{#pi^{0}}#geq0.58, P_{#gamma}#geq0.3 GeV, P_{#pi^{#pm}}#geq0.2 GeV;"
                                                                                " #omega = Ev - El [GeV]", 100, 0, beamE * 1.1); // (e,eFD'pFD)X

    TFile *file = new TFile(fileName.c_str(), "fileName");
    TTree *gst = (TTree *) file->Get("gst");

    TCanvas *c1 = new TCanvas("c1", "c1", 1200, 900);
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

        _1eInclusive->Fill(omega); // omega for (e,e')


        for (int t = 0; t < nf; ++t) {
            double theta_part = acos(pzf[t] / sqrt(pxf[t] * pxf[t] + pyf[t] * pyf[t] + pzf[t] * pzf[t])) * 180 / pi;
            bool part_in_FD = (theta_part >= 5. && theta_part <= 40.);

            if (pdgf[t] == 2212 && part_in_FD) { P_p_1e_cut_FD->Fill(pf[t]); }
        }

        if (nfp == 1) {
            int prot_1e1pX_ind;

            for (int k = 0; k < nf; ++k) {
                double theta_part = acos(pzf[k] / sqrt(pxf[k] * pxf[k] + pyf[k] * pyf[k] + pzf[k] * pzf[k])) * 180 / pi;
                bool part_in_FD = (theta_part >= 5. && theta_part <= 40.);

                if (pdgf[k] == 2212) { prot_1e1pX_ind = k; }
            }

            double theta_p = acos(pzf[prot_1e1pX_ind] / sqrt(pxf[prot_1e1pX_ind] * pxf[prot_1e1pX_ind] + pyf[prot_1e1pX_ind] * pyf[prot_1e1pX_ind] +
                                                             pzf[prot_1e1pX_ind] * pzf[prot_1e1pX_ind])) * 180 / pi;
            double P_p = sqrt(pxf[prot_1e1pX_ind] * pxf[prot_1e1pX_ind] + pyf[prot_1e1pX_ind] * pyf[prot_1e1pX_ind] + pzf[prot_1e1pX_ind] * pzf[prot_1e1pX_ind]);

            bool p_in_FD = (theta_p >= 5. && theta_p <= 40.);

            if (p_in_FD) { P_p_1e1pX_FD->Fill(P_p); }

        }

        if (nf == 1 && nfp == 1) {
            double theta_p = acos(pzf[0] / sqrt(pxf[0] * pxf[0] + pyf[0] * pyf[0] + pzf[0] * pzf[0])) * 180 / pi;
            double P_p = sqrt(pxf[0] * pxf[0] + pyf[0] * pyf[0] + pzf[0] * pzf[0]);

            bool p_in_FD = (theta_p >= 5. && theta_p <= 40.);

            if (p_in_FD) { P_p_1e1p_FD->Fill(P_p); }

        }


        if (nfp == 1) {
            int prot_1e1p_ind;

            for (int k = 0; k < nf; ++k) {
                double theta_part = acos(pzf[k] / sqrt(pxf[k] * pxf[k] + pyf[k] * pyf[k] + pzf[k] * pzf[k])) * 180 / pi;
                bool part_in_FD = (theta_part >= 5. && theta_part <= 40.);

                if (pdgf[k] == 2212) { prot_1e1p_ind = k; }
            }

            double theta_p = acos(pzf[prot_1e1p_ind] /
                                  sqrt(pxf[prot_1e1p_ind] * pxf[prot_1e1p_ind] + pyf[prot_1e1p_ind] * pyf[prot_1e1p_ind] + pzf[prot_1e1p_ind] * pzf[prot_1e1p_ind])) *
                             180 / pi;
            double P_p = sqrt(pxf[prot_1e1p_ind] * pxf[prot_1e1p_ind] + pyf[prot_1e1p_ind] * pyf[prot_1e1p_ind] + pzf[prot_1e1p_ind] * pzf[prot_1e1p_ind]);

            bool p_in_FD = (theta_p >= 5. && theta_p <= 40.);

            //<editor-fold desc="1e1p plots">
            _1e1pX->Fill(omega); // omega for (e,e'p)X

            if (nf == 1) {
                _1e1p->Fill(omega); // omega for (e,e'p)
            }

            if (lp_in_FD) {
                _1eFD1pX->Fill(omega); // omega for (e,e'FDp)X
            }

            if (p_in_FD) {
                _1e1pFDX->Fill(omega); // omega for (e,e'pFD)X
            }

            if (lp_in_FD && p_in_FD) {
                _1eFD1pFDX->Fill(omega); // omega for (e,eFD'pFD)X
            }
            //</editor-fold>

        }

        int prot_ind = -1, prot_ind_mom = -1;

        int numOfProt_mom = 0, numOfProt_mom_FD = 0;
        int numOfPi0_mom = 0, numOfPi0_mom_FD = 0;
        int numOfPh = 0, numOfPh_mom = 0, numOfPh_mom_FD = 0;
        int numOfPip_mom = 0;
        int numOfPim_mom = 0;

        for (int j = 0; j < nf; ++j) {
            double theta_part = acos(pzf[j] / sqrt(pxf[j] * pxf[j] + pyf[j] * pyf[j] + pzf[j] * pzf[j])) * 180 / pi;
            double mom_part = sqrt(pxf[j] * pxf[j] + pyf[j] * pyf[j] + pzf[j] * pzf[j]);
            bool part_in_FD = (theta_part >= 5. && theta_part <= 40.);

            if (pdgf[j] == 2212) { prot_ind = j; }

            if (pdgf[j] == 2212 && mom_part >= 0.3) {
                prot_ind_mom = j;
                ++numOfProt_mom;

                if (part_in_FD) { ++numOfProt_mom_FD; }
            } else if (pdgf[j] == 211 && (mom_part >= 0.2)) {
                ++numOfPip_mom;
            } else if (pdgf[j] == -211 && (mom_part >= 0.2)) {
                ++numOfPim_mom;
            } else if (pdgf[j] == 111 && (mom_part >= 0.58)) {
                ++numOfPi0_mom;

                if (part_in_FD) { ++numOfPi0_mom_FD; }
            } else if (pdgf[j] == 22) {
                ++numOfPh;

                if (mom_part >= 0.3) {
                    ++numOfPh_mom;

                    if (part_in_FD) { ++numOfPh_mom_FD; }
                }
            }
        }

        double theta_p_mom = acos(pzf[prot_ind_mom] /
                                  sqrt(pxf[prot_ind_mom] * pxf[prot_ind_mom] + pyf[prot_ind_mom] * pyf[prot_ind_mom] + pzf[prot_ind_mom] * pzf[prot_ind_mom])) * 180 / pi;

        bool p_mom_in_FD = (theta_p_mom >= 5. && theta_p_mom <= 40.);

        if (numOfProt_mom == 1) {

            //<editor-fold desc="1e1p w/ P_p th. plots">
            if (nf == nfp) {
                _1e1p_wPpth->Fill(omega); // omega for (e,e'p) w/P_{p}#geq0.3 GeV
            }

            _1e1pX_wPpth->Fill(omega); // omega for (e,e'p)X w/P_{p}#geq0.3 GeV

            if (lp_in_FD) {
                _1eFD1pX_wPpth->Fill(omega); // omega for (e,e'FDp)X w/P_{p}#geq0.3 GeV
            }

            if (p_mom_in_FD) {
                _1e1pFDX_wPpth->Fill(omega); // omega for (e,e'pFD)X w/P_{p}#geq0.3 GeV
            }

            if (lp_in_FD && p_mom_in_FD) {
                _1eFD1pFDX_wPpth->Fill(omega); // omega for (e,eFD'pFD)X w/P_{p}#geq0.3 GeV
            }
            //</editor-fold>

            //<editor-fold desc="1e1p w/ P_p, P_ph and P_pi0 th. plots">
            if (numOfPi0_mom_FD == 0 && numOfPh_mom_FD == 0) {
                _1e1p0pizeroFD0phFDX_wPpth_wPphth_wPpi0th->Fill(omega); // omega for (e,e'p)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma

                if (lp_in_FD) {
                    _1eFD1p0pizeroFD0phFDX_wPpth_wPphth_wPpi0th->Fill(omega); // omega for (e,e'FDp)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }

                if (p_mom_in_FD) {
                    _1e1pFD0pizeroFD0phFDX_wPpth_wPphth_wPpi0th->Fill(omega); // omega for (e,e'pFD)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }

                if (lp_in_FD && p_mom_in_FD) {
                    _1eFD1pFD0pizeroFD0phFDX_wPpth_wPphth_wPpi0th->Fill(omega); // omega for (e,eFD'pFD)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }
            }
            //</editor-fold>

            //<editor-fold desc="1e1p w/ P_p, P_ph, P_pi0 and P_cPi th. plots">
            if (numOfPi0_mom_FD == 0 && numOfPh_mom_FD == 0 && numOfPim_mom == 0 && numOfPip_mom == 0) {
                _1e1p0pizeroFD0phFD0cPiX_wPpth_wPphth_wPpi0th_wKCpith->Fill(omega); // omega for (e,e'p)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma

                if (lp_in_FD) {
                    _1eFD1p0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith->Fill(omega); // omega for (e,e'FDp)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }

                if (p_mom_in_FD) {
                    _1e1pFD0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith->Fill(omega); // omega for (e,e'pFD)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }

                if (lp_in_FD && p_mom_in_FD) {
                    _1eFD1pFD0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith->Fill(omega); // omega for (e,eFD'pFD)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }
            }
            //</editor-fold>

            //<editor-fold desc="1e1p w/ P_p, P_ph, P_pi0 and P_cPi th. plots">
            if ((numOfPi0_mom_FD == 0 && numOfPh_mom_FD == 0 && numOfPim_mom == 0 && numOfPip_mom == 0) &&
                nf == nfp + nfpi0 + nfpip + nfpim + nfn + numOfPh) {
                f_1e1p0pizeroFD0phFD0cPiXn_wPpth_wPphth_wPpi0th_wKCpith->Fill(omega); // omega for (e,e'p)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma

                if (lp_in_FD) {
                    f_1eFD1p0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith->Fill(omega); // omega for (e,e'FDp)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }

                if (p_mom_in_FD) {
                    f_1e1pFD0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith->Fill(omega); // omega for (e,e'pFD)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }

                if (lp_in_FD && p_mom_in_FD) {
                    f_1eFD1pFD0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith->Fill(omega); // omega for (e,eFD'pFD)X w/P_{p}#geq0.3 GeV & no FD #pi^{0},#gamma
                }
            }
            //</editor-fold>

        }

    }

    P_p_1e_cut_FD->Draw();
    c1->SaveAs("TL_plots/Momentum_plots_1e_cut/01_P_p_for_1e_cut.png");
    c1->Clear();

    P_p_1e1pX_FD->Draw();
    c1->SaveAs("TL_plots/Momentum_plots_1e1pX/01_P_p_for_1e1pX.png");
    c1->Clear();

    P_p_1e1p_FD->Draw();
    c1->SaveAs("TL_plots/Momentum_plots_1e1p/01_P_p_for_1e1p.png");
    c1->Clear();

    _1eInclusive->Draw();
    c1->SaveAs("TL_plots/00_1eInclusive.png");
    c1->Clear();

    _1e1p->Draw();
    c1->SaveAs("TL_plots/01_1e1p.png");
    c1->Clear();

    _1e1pX->Draw();
    c1->SaveAs("TL_plots/02_1e1pX.png");
    c1->Clear();

    _1eFD1pX->Draw();
    c1->SaveAs("TL_plots/03_1eFD1pX.png");
    c1->Clear();

    _1e1pFDX->Draw();
    c1->SaveAs("TL_plots/04_1e1pFDX.png");
    c1->Clear();

    _1eFD1pFDX->Draw();
    c1->SaveAs("TL_plots/05_1eFD1pFDX.png");
    c1->Clear();

    _1e1p_wPpth->Draw();
    c1->SaveAs("TL_plots/06_1e1p_wPpth.png");
    c1->Clear();

    _1e1pX_wPpth->Draw();
    c1->SaveAs("TL_plots/07_1e1pX_wPpth.png");
    c1->Clear();

    _1eFD1pX_wPpth->Draw();
    c1->SaveAs("TL_plots/08_1eFD1pX_wPpth.png");
    c1->Clear();

    _1e1pFDX_wPpth->Draw();
    c1->SaveAs("TL_plots/09_1e1pFDX_wPpth.png");
    c1->Clear();

    _1eFD1pFDX_wPpth->Draw();
    c1->SaveAs("TL_plots/10_1eFD1pFDX_wPpth.png");
    c1->Clear();

    _1e1p0pizeroFD0phFDX_wPpth_wPphth_wPpi0th->Draw();
    c1->SaveAs("TL_plots/11_1e1p0pizeroFD0phFDX_wPpth_wPphth_wPpi0th.png");
    c1->Clear();

    _1eFD1p0pizeroFD0phFDX_wPpth_wPphth_wPpi0th->Draw();
    c1->SaveAs("TL_plots/12_1eFD1p0pizeroFD0phFDX_wPpth_wPphth_wPpi0th.png");
    c1->Clear();

    _1e1pFD0pizeroFD0phFDX_wPpth_wPphth_wPpi0th->Draw();
    c1->SaveAs("TL_plots/13_1e1pFD0pizeroFD0phFDX_wPpth_wPphth_wPpi0th.png");
    c1->Clear();

    _1eFD1pFD0pizeroFD0phFDX_wPpth_wPphth_wPpi0th->Draw();
    c1->SaveAs("TL_plots/14_1eFD1pFD0pizeroFD0phFDX_wPpth_wPphth_wPpi0th.png");
    c1->Clear();

    _1e1p0pizeroFD0phFD0cPiX_wPpth_wPphth_wPpi0th_wKCpith->Draw();
    c1->SaveAs("TL_plots/15_1e1p0pizeroFD0phFD0cPiX_wPpth_wPphth_wPpi0th_wKCpith.png");
    c1->Clear();

    _1eFD1p0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith->Draw();
    c1->SaveAs("TL_plots/16_1eFD1p0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith.png");
    c1->Clear();

    _1e1pFD0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith->Draw();
    c1->SaveAs("TL_plots/17_1e1pFD0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith.png");
    c1->Clear();

    _1eFD1pFD0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith->Draw();
    c1->SaveAs("TL_plots/18_1eFD1pFD0pizeroFD0phFDcPiX_wPpth_wPphth_wPpi0th_wKCpith.png");
    c1->Clear();

    f_1e1p0pizeroFD0phFD0cPiXn_wPpth_wPphth_wPpi0th_wKCpith->Draw();
    c1->SaveAs("TL_plots/19_1e1p0pizeroFD0phFD0cPiXn_wPpth_wPphth_wPpi0th_wKCpith.png");
    c1->Clear();

    f_1eFD1p0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith->Draw();
    c1->SaveAs("TL_plots/20_1eFD1p0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith.png");
    c1->Clear();

    f_1e1pFD0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith->Draw();
    c1->SaveAs("TL_plots/21_1e1pFD0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith.png");
    c1->Clear();

    f_1eFD1pFD0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith->Draw();
    c1->SaveAs("TL_plots/22_1eFD1pFD0pizeroFD0phFDcPiXn_wPpth_wPphth_wPpi0th_wKCpith.png");
    c1->Clear();

    cout << "\n\nFinished!\n\n";
}