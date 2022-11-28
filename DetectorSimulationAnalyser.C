
//#include "HipoChain.h"

//#include <HipoHist.h>

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include "clas12reader.h"

//#include "TruthLevelAnalyser.h"
//#include "gst.h"
#include "codeSetup.h"

using namespace clas12;
using namespace std;

/* Copy commend (local -> JLab):
scp -r /home/alon/project/temp/BankHist/Analyser.C -J asportes@ftp.jlab.org:/u/home/asportes/clas12project/
*/

//void Analyser() {
void EventAnalyser(int NumberOfProtons, int NumberOfNeutrons) {

    string AnalyserVersion = "3.1b";

    string loadedInput = AnalyseFile;
//    string loadedInput = "/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo";

    TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
    c1->SetGrid();

    if (NumberOfProtons == 2 && NumberOfNeutrons == 0) {
        clas12reader c12(loadedInput.c_str()); //open file

        c12.addExactPid(2212, 2); //exactly 2 protons
        c12.addExactPid(11, 1); //exactly 1 electron
        c12.addZeroOfRestPid(); //nothing else

        auto *Beta_VS_P_2p = new TH2F("Beta VS P (2p)", "#beta VS P (2p); P [GeV]; #beta [Arbitrary units]", 250, 0, 6, 250, 0, 1.05);
        auto *P_histogram_2p = new TH1F("P Histogram (2p)", "P Histogram (2p)", 100, 0, 6);

        TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
        c1->SetGrid();

        while (c12.next()) { //loop over events (2p)

            auto particles_2p = c12.getDetParticles(); //particles is now a std::vector of particles for this event

            cout << "2p =======================================================================\n";

            for (int i = 0; i < particles_2p.size(); i++) {

                float particlePDG = particles_2p[i]->par()->getPid();
                float Beta = particles_2p[i]->par()->getBeta();
                float P = particles_2p[i]->par()->getP();

                Beta_VS_P_2p->Fill(P, Beta);
                P_histogram_2p->Fill(P);

                cout << "particlePDG[" << i << "] = " << particlePDG << "\n";

            }

            cout << "\n";

        }

        Beta_VS_P_2p->SetStats(0);
        Beta_VS_P_2p->Draw("colz");
        cout << "\n";
        c1->SaveAs("./plots/Beta_VS_P_2p.png");
        c1->Clear();

        P_histogram_2p->Draw("colz");
        cout << "\n";
        c1->SaveAs("./plots/P_histogram_2p.png");
        c1->Clear();

    } else if (NumberOfProtons == 1 && NumberOfNeutrons == 1) {
        clas12reader c12(loadedInput.c_str()); //open file

        c12.addExactPid(2212, 1);    //exactly 2 protons
        c12.addExactPid(2112, 1);    //exactly 2 protons
        c12.addExactPid(11, 1);    //exactly 1 electron
        c12.addZeroOfRestPid();  //nothing else

        auto *Beta_VS_P_1n1p = new TH2F("Beta VS P (1n1p)", "#beta VS P (1n1p); P [GeV]; #beta [Arbitrary units]", 250, 0, 6, 250, 0, 1.05);
        auto *P_histogram_1n1p = new TH1F("P Histogram (1n1p)", "P Histogram (1n1p)", 100, 0, 6);

        while (c12.next()) { //loop over events (1np)

            auto particles_1n1p = c12.getDetParticles(); //particles is now a std::vector of particles for this event

            cout << "1n1p =======================================================================\n";

            for (int i = 0; i < particles_1n1p.size(); i++) {

                float particlePDG = particles_1n1p[i]->par()->getPid();
                float Beta = particles_1n1p[i]->par()->getBeta();
                float P = particles_1n1p[i]->par()->getP();

                Beta_VS_P_1n1p->Fill(P, Beta);
                P_histogram_1n1p->Fill(P);

                cout << "particlePDG[" << i << "] = " << particlePDG << "\n";

            }

            cout << "\n";

        }

        Beta_VS_P_1n1p->SetStats(0);
        Beta_VS_P_1n1p->Draw("colz");
        cout << "\n";
        c1->SaveAs("./plots/Beta_VS_P_1n1p.png");
        c1->Clear();

        P_histogram_1n1p->Draw("colz");
        cout << "\n";
        c1->SaveAs("./plots/P_histogram_1n1p.png");
        c1->Clear();

    }

    cout << "\n";
    cout << "Operation finished (AnalyserVersion = " << AnalyserVersion << ")." << "\n";
    cout << "\n";

}