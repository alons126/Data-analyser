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

#if noFSRatio
#include "../../source/functions/GeneralFunctions.h"
#include "../../source/constants.h"
#include "HistPlotter1D.cpp"
#include "HistPlotterStack.cpp"
#include "HistPlotterStack1.cpp"
#include "HistPlotter2D.cpp"
#include "../../source/classes/hData/hData.cpp"
#endif

using namespace std;

void FSRPlotter(hData &utilities, TCanvas *HistCanvas, TList *MScThesisPlotsList, const char *Sim_filename, const char *Data_filename, const string &SampleName,
                const string &FSTopology, const string &DetRegion, const string &HistName_Denominator, const string &HistName_Numerator, const string &FSRHistName,
                const string &SavePath, const string &SaveName_Denominator, const string &SaveName_Numerator, const string &SaveName_FSR, const int &Num) {
    bool PrintOut = false;

    string Numerator_FS, Denominator_FS;
    bool TLmom = false;

    if (FSTopology == "1N") {
        Numerator_FS = "1n", Denominator_FS = "1p";
    } else if (FSTopology == "2N") {
        Numerator_FS = "nFDpCD", Denominator_FS = "pFDpCD";
    }

    string HistNameFeed_Numerator, HistNameFeed_Denominator;

    if (findSubstring(HistName_Numerator, "electron") || findSubstring(HistName_Numerator, "Electron") ||
        findSubstring(HistName_Denominator, "electron") || findSubstring(HistName_Denominator, "Electron") ||
        (Numerator_FS == "nFDpCD" || Denominator_FS == "pFDpCD")) {
        if (findSubstring(HistName_Numerator, "TL")) {
            HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ", " + DetRegion + ")";
            HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ", " + DetRegion + ")";
        } else if (findSubstring(HistName_Numerator, "W distribution") || findSubstring(HistName_Numerator, "Q^{2}") || findSubstring(HistName_Numerator, "#theta_{e}") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD}") || findSubstring(HistName_Numerator, "#theta_{pFD}") ||
                   findSubstring(HistName_Numerator, "#theta_{pCD}") || findSubstring(HistName_Numerator, "#theta_{tot}") ||
                   findSubstring(HistName_Numerator, "#theta_{rel}") || findSubstring(HistName_Numerator, "#theta_{#vec{P}_{e},#vec{P}_{tot}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{tot}}") || findSubstring(HistName_Numerator, "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pL}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nL}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pR}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nR}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nFD}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pFD}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pCD}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pCD}}") || findSubstring(HistName_Numerator, "#theta_{pFD,pCD}") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD,pCD}") || findSubstring(HistName_Numerator, "E_{e}") ||
                   findSubstring(HistName_Numerator, "#omega around #theta_{e} = 15#circ") || findSubstring(HistName_Numerator, "E_{cal}") ||
                   findSubstring(HistName_Numerator, "#deltaP_{T,tot}") || findSubstring(HistName_Numerator, "#delta#alpha_{T,tot}") ||
                   findSubstring(HistName_Numerator, "#deltaP_{T,tot}") || findSubstring(HistName_Numerator, "#delta#alpha_{T,tot}") ||
                   findSubstring(HistName_Numerator, "#theta_{e} vs.") || findSubstring(HistName_Numerator, "#phi_{e} vs.") || findSubstring(HistName_Numerator, "vs. W") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD} vs.") || findSubstring(HistName_Numerator, "#theta_{pFD} vs.") ||
                   findSubstring(HistName_Numerator, "#theta_{pCD} vs.") || findSubstring(HistName_Numerator, "#phi_{nFD} vs.") ||
                   findSubstring(HistName_Numerator, "#phi_{pFD} vs.") || findSubstring(HistName_Numerator, "#phi_{pCD} vs.")
                ) {
            HistNameFeed_Numerator = HistName_Numerator, HistNameFeed_Denominator = HistName_Denominator;

            if (PrintOut) {
                cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
                cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
            }
        } else {
            HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ")", HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ")";

            if (PrintOut) {
                cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
                cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
            }
        }
    } else {
        HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ", " + DetRegion + ")";
        HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ", " + DetRegion + ")";

        if (PrintOut) {
            cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
            cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
        }
    }

    string FSRatioParticle = utilities.GetParticleName1(FSRHistName), FSRatioType = utilities.GetType(FSRHistName), FSTopo = utilities.GetTopology(FSRHistName), FSRationTemp;

    if (PrintOut) {
//        cout << "HistogramClass = " << HistogramClass << "\n";
        cout << "FSRatioParticle = " << FSRatioParticle << "\n";
        cout << "FSRatioType = " << FSRatioType << "\n";
        cout << "FSTopo = " << FSTopo << "\n";
        cout << "!findSubstring(HistName_Numerator, vs) = " << !findSubstring(HistName_Numerator, "vs") << "\n";
        cout << "!findSubstring(HistName_Numerator, VS) = " << !findSubstring(HistName_Numerator, "vs") << "\n";
    }

    if (findSubstring(HistName_Numerator, "#theta_{tot}")) {
        FSRationTemp = "theta_tot FSRatio ()";
    } else if (findSubstring(HistName_Numerator, "#theta_{rel}")) {
        FSRationTemp = "theta_rel FSRatio ()";
    } else {
        if ((FSRatioParticle == "Neutron" || FSRatioParticle == "Proton") && FSTopology == "2N") {
            FSRationTemp = FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")";
        } else {
            FSRationTemp = " " + FSRatioType + " FSRatio (" + FSTopology + ")";
        }
    }

    if ((HistName_Numerator == "FD TL Neutron momentum AC") && (FSTopology == "1N")) {
        FSRationTemp = "Neutron momentum FSRatio (1N)";
        TLmom = true;
    } else if ((HistName_Numerator == "FD TL Neutron momentum AC" || HistName_Numerator == "CD TL Proton momentum AC") && (FSTopology == "2N")) {
        TLmom = true;
    } else if (findSubstring(HistName_Numerator, "#theta_{nFD}") || findSubstring(HistName_Numerator, "#theta_{pFD}")) {
        FSRationTemp = "Neutron theta FSRatio (2N)";
    } else if (findSubstring(HistName_Numerator, "#theta_{pCD}")) {
        FSRationTemp = "Proton theta FSRatio (2N)";
    }

    if (PrintOut) {
        cout << "\n\nFSRationTemp = " << FSRationTemp << "\n";
        cout << "TLmom = " << TLmom << "\n\n";
    }

    if (PrintOut) { cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n\n"; }
    HistPlotterStack(utilities, HistCanvas, MScThesisPlotsList, Sim_filename, Data_filename, HistNameFeed_Numerator.c_str(), SampleName, SavePath,
                     (to_string(Num) + "a_" + SaveName_Numerator));

    if (PrintOut) { cout << "\nHistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n\n"; }
    HistPlotterStack(utilities, HistCanvas, MScThesisPlotsList, Sim_filename, Data_filename, HistNameFeed_Denominator.c_str(), SampleName, SavePath,
                     (to_string(Num) + "b_" + SaveName_Denominator));

    if (PrintOut) { cout << "\nFSRationTemp = " << FSRationTemp << "\n\n"; }
    HistPlotterStack(utilities, HistCanvas, MScThesisPlotsList, Sim_filename, Data_filename, FSRationTemp.c_str(), SampleName, SavePath,
                     (to_string(Num) + "c_" + SaveName_FSR), TLmom);
}

void FSRPlotterStack(hData &utilities, TCanvas *HistCanvas, TList *MScThesisPlotsList, const char *Sim_filename, const char *Data_filename, const string &SampleName,
                     const string &FSTopology, const string &DetRegion, const string &HistName_Denominator, const string &HistName_Numerator, const string &FSRHistName,
                     const string &SavePath, const string &SaveName_Denominator, const string &SaveName_Numerator, const string &SaveName_FSR, const int &Num) {
    bool PrintOut = false;

    string Numerator_FS, Denominator_FS;
    bool TLmom = false;

    if (FSTopology == "1N") {
        Numerator_FS = "1n", Denominator_FS = "1p";
    } else if (FSTopology == "2N") {
        Numerator_FS = "nFDpCD", Denominator_FS = "pFDpCD";
    }

    string HistNameFeed_Numerator, HistNameFeed_Denominator;

    if (findSubstring(HistName_Numerator, "electron") || findSubstring(HistName_Numerator, "Electron") ||
        findSubstring(HistName_Denominator, "electron") || findSubstring(HistName_Denominator, "Electron") ||
        (Numerator_FS == "nFDpCD" || Denominator_FS == "pFDpCD")) {
        if (findSubstring(HistName_Numerator, "TL")) {
            HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ", " + DetRegion + ")";
            HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ", " + DetRegion + ")";
        } else if (findSubstring(HistName_Numerator, "W distribution") || findSubstring(HistName_Numerator, "Q^{2}") || findSubstring(HistName_Numerator, "#theta_{e}") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD}") || findSubstring(HistName_Numerator, "#theta_{pFD}") ||
                   findSubstring(HistName_Numerator, "#theta_{pCD}") || findSubstring(HistName_Numerator, "#theta_{tot}") ||
                   findSubstring(HistName_Numerator, "#theta_{rel}") || findSubstring(HistName_Numerator, "#theta_{#vec{P}_{e},#vec{P}_{tot}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{tot}}") || findSubstring(HistName_Numerator, "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pL}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nL}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pR}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nR}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nFD}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pFD}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pCD}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pCD}}") || findSubstring(HistName_Numerator, "#theta_{pFD,pCD}") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD,pCD}") || findSubstring(HistName_Numerator, "E_{e}") ||
                   findSubstring(HistName_Numerator, "#omega around #theta_{e} = 15#circ") || findSubstring(HistName_Numerator, "E_{cal}") ||
                   findSubstring(HistName_Numerator, "#deltaP_{T,tot}") || findSubstring(HistName_Numerator, "#delta#alpha_{T,tot}") ||
                   findSubstring(HistName_Numerator, "#deltaP_{T,tot}") || findSubstring(HistName_Numerator, "#delta#alpha_{T,tot}") ||
                   findSubstring(HistName_Numerator, "#theta_{e} vs.") || findSubstring(HistName_Numerator, "#phi_{e} vs.") || findSubstring(HistName_Numerator, "vs. W") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD} vs.") || findSubstring(HistName_Numerator, "#theta_{pFD} vs.") ||
                   findSubstring(HistName_Numerator, "#theta_{pCD} vs.") || findSubstring(HistName_Numerator, "#phi_{nFD} vs.") ||
                   findSubstring(HistName_Numerator, "#phi_{pFD} vs.") || findSubstring(HistName_Numerator, "#phi_{pCD} vs.")
                ) {
            HistNameFeed_Numerator = HistName_Numerator, HistNameFeed_Denominator = HistName_Denominator;

            if (PrintOut) {
                cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
                cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
            }
        } else {
            HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ")", HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ")";

            if (PrintOut) {
                cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
                cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
            }
        }
    } else {
        HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ", " + DetRegion + ")";
        HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ", " + DetRegion + ")";

        if (PrintOut) {
            cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
            cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
        }
    }

    string FSRatioParticle = utilities.GetParticleName1(FSRHistName), FSRatioType = utilities.GetType(FSRHistName), FSTopo = utilities.GetTopology(FSRHistName), FSRationTemp;

    if (PrintOut) {
//        cout << "HistogramClass = " << HistogramClass << "\n";
        cout << "FSRatioParticle = " << FSRatioParticle << "\n";
        cout << "FSRatioType = " << FSRatioType << "\n";
        cout << "FSTopo = " << FSTopo << "\n";
        cout << "!findSubstring(HistName_Numerator, vs) = " << !findSubstring(HistName_Numerator, "vs") << "\n";
        cout << "!findSubstring(HistName_Numerator, VS) = " << !findSubstring(HistName_Numerator, "vs") << "\n";
    }

    if (findSubstring(HistName_Numerator, "#theta_{tot}")) {
        FSRationTemp = "theta_tot FSRatio ()";
    } else if (findSubstring(HistName_Numerator, "#theta_{rel}")) {
        FSRationTemp = "theta_rel FSRatio ()";
    } else {
        if ((FSRatioParticle == "Neutron" || FSRatioParticle == "Proton") && FSTopology == "2N") {
            FSRationTemp = FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")";
        } else {
            FSRationTemp = " " + FSRatioType + " FSRatio (" + FSTopology + ")";
        }
    }

    if ((HistName_Numerator == "FD TL Neutron momentum AC") && (FSTopology == "1N")) {
        FSRationTemp = "Neutron momentum FSRatio (1N)";
        TLmom = true;
    } else if ((HistName_Numerator == "FD TL Neutron momentum AC" || HistName_Numerator == "CD TL Proton momentum AC") && (FSTopology == "2N")) {
        TLmom = true;
    } else if (findSubstring(HistName_Numerator, "#theta_{nFD}") || findSubstring(HistName_Numerator, "#theta_{pFD}")) {
        FSRationTemp = "Neutron theta FSRatio (2N)";
    } else if (findSubstring(HistName_Numerator, "#theta_{pCD}")) {
        FSRationTemp = "Proton theta FSRatio (2N)";
    }

    if (PrintOut) {
        cout << "\n\nFSRationTemp = " << FSRationTemp << "\n";
        cout << "TLmom = " << TLmom << "\n\n";
    }

    if (PrintOut) { cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n\n"; }
    HistPlotterStack1(utilities, HistCanvas, MScThesisPlotsList, Sim_filename, Data_filename, HistNameFeed_Numerator.c_str(), SampleName, SavePath,
                      (to_string(Num) + "a_" + SaveName_Numerator));

    if (PrintOut) { cout << "\nHistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n\n"; }
    HistPlotterStack1(utilities, HistCanvas, MScThesisPlotsList, Sim_filename, Data_filename, HistNameFeed_Denominator.c_str(), SampleName, SavePath,
                      (to_string(Num) + "b_" + SaveName_Denominator));

    if (PrintOut) { cout << "\nFSRationTemp = " << FSRationTemp << "\n\n"; }
    HistPlotterStack(utilities, HistCanvas, MScThesisPlotsList, Sim_filename, Data_filename, FSRationTemp.c_str(), SampleName, SavePath,
                     (to_string(Num) + "c_" + SaveName_FSR), TLmom);
}

void FSRPlotter(hData &utilities, TCanvas *HistCanvas, TList *MScThesisPlotsList, const char *filename, const string &SampleName, const string &FSTopology,
                const string &HistogramClass, const string &DetRegion, const string &HistName_Denominator, const string &HistName_Numerator, const string &FSRHistName,
                const string &SavePath, const string &SaveName_Denominator, const string &SaveName_Numerator, const string &SaveName_FSR, const int &Num,
                const bool &Results_plots = false) {
    bool PrintOut = false;

    string Numerator_FS, Denominator_FS;
    bool TLmom = false;

    if (FSTopology == "1N") {
        Numerator_FS = "1n", Denominator_FS = "1p";
    } else if (FSTopology == "2N") {
        Numerator_FS = "nFDpCD", Denominator_FS = "pFDpCD";
    }

    string HistNameFeed_Numerator, HistNameFeed_Denominator;

    if (findSubstring(HistName_Numerator, "electron") || findSubstring(HistName_Numerator, "Electron") ||
        findSubstring(HistName_Denominator, "electron") || findSubstring(HistName_Denominator, "Electron") ||
        (Numerator_FS == "nFDpCD" || Denominator_FS == "pFDpCD")) {
        if (findSubstring(HistName_Numerator, "TL")) {
            HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ", " + DetRegion + ")";
            HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ", " + DetRegion + ")";
        } else if (findSubstring(HistName_Numerator, "W distribution") || findSubstring(HistName_Numerator, "Q^{2}") || findSubstring(HistName_Numerator, "#theta_{e}") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD}") || findSubstring(HistName_Numerator, "#theta_{pFD}") ||
                   findSubstring(HistName_Numerator, "#theta_{pCD}") || findSubstring(HistName_Numerator, "#theta_{tot}") ||
                   findSubstring(HistName_Numerator, "#theta_{rel}") || findSubstring(HistName_Numerator, "#theta_{#vec{P}_{e},#vec{P}_{tot}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{tot}}") || findSubstring(HistName_Numerator, "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pL}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nL}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pR}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nR}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{nFD}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pFD}}") || findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pCD}}") ||
                   findSubstring(HistName_Numerator, "#theta_{#vec{q},#vec{P}_{pCD}}") || findSubstring(HistName_Numerator, "#theta_{pFD,pCD}") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD,pCD}") || findSubstring(HistName_Numerator, "E_{e}") ||
                   findSubstring(HistName_Numerator, "#omega around #theta_{e} = 15#circ") || findSubstring(HistName_Numerator, "E_{cal}") ||
                   findSubstring(HistName_Numerator, "#deltaP_{T,tot}") || findSubstring(HistName_Numerator, "#delta#alpha_{T,tot}") ||
                   findSubstring(HistName_Numerator, "#deltaP_{T,tot}") || findSubstring(HistName_Numerator, "#delta#alpha_{T,tot}") ||
                   findSubstring(HistName_Numerator, "#theta_{e} vs.") || findSubstring(HistName_Numerator, "#phi_{e} vs.") || findSubstring(HistName_Numerator, "vs. W") ||
                   findSubstring(HistName_Numerator, "#theta_{nFD} vs.") || findSubstring(HistName_Numerator, "#theta_{pFD} vs.") ||
                   findSubstring(HistName_Numerator, "#theta_{pCD} vs.") || findSubstring(HistName_Numerator, "#phi_{nFD} vs.") ||
                   findSubstring(HistName_Numerator, "#phi_{pFD} vs.") || findSubstring(HistName_Numerator, "#phi_{pCD} vs.")
                ) {
            HistNameFeed_Numerator = HistName_Numerator, HistNameFeed_Denominator = HistName_Denominator;

            if (PrintOut) {
                cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
                cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
            }
        } else {
            HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ")", HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ")";

            if (PrintOut) {
                cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
                cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
            }
        }
    } else {
        HistNameFeed_Numerator = HistName_Numerator + " (" + Numerator_FS + ", " + DetRegion + ")";
        HistNameFeed_Denominator = HistName_Denominator + " (" + Denominator_FS + ", " + DetRegion + ")";

        if (PrintOut) {
            cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n";
            cout << "HistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n";
        }
    }

    string FSRatioParticle = utilities.GetParticleName1(FSRHistName), FSRatioType = utilities.GetType(FSRHistName), FSTopo = utilities.GetTopology(FSRHistName), FSRationTemp;

    if (PrintOut) {
        cout << "HistogramClass = " << HistogramClass << "\n";
        cout << "FSRatioParticle = " << FSRatioParticle << "\n";
        cout << "FSRatioType = " << FSRatioType << "\n";
        cout << "FSTopo = " << FSTopo << "\n";
        cout << "!findSubstring(HistName_Numerator, vs) = " << !findSubstring(HistName_Numerator, "vs") << "\n";
        cout << "!findSubstring(HistName_Numerator, VS) = " << !findSubstring(HistName_Numerator, "vs") << "\n";
    }

    if (HistogramClass == "TH1D") {
        if (findSubstring(HistName_Numerator, "#theta_{tot}")) {
            FSRationTemp = "theta_tot FSRatio ()";
        } else if (findSubstring(HistName_Numerator, "#theta_{rel}")) {
            FSRationTemp = "theta_rel FSRatio ()";
        } else {
            if ((FSRatioParticle == "Neutron" || FSRatioParticle == "Proton") && FSTopology == "2N") {
                FSRationTemp = FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")";
            } else {
                FSRationTemp = " " + FSRatioType + " FSRatio (" + FSTopology + ")";
            }
        }

        if ((HistName_Numerator == "FD TL Neutron momentum AC") && (FSTopology == "1N")) {
            FSRationTemp = "Neutron momentum FSRatio (1N)";
            TLmom = true;
        } else if ((HistName_Numerator == "FD TL Neutron momentum AC" || HistName_Numerator == "CD TL Proton momentum AC") && (FSTopology == "2N")) {
            TLmom = true;
        } else if (findSubstring(HistName_Numerator, "#theta_{nFD}") || findSubstring(HistName_Numerator, "#theta_{pFD}")) {
            FSRationTemp = "Neutron theta FSRatio (2N)";
        } else if (findSubstring(HistName_Numerator, "#theta_{pCD}")) {
            FSRationTemp = "Proton theta FSRatio (2N)";
        }
    } else {
        FSRationTemp = FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")";
    }

    if (PrintOut) {
        cout << "\n\nFSRationTemp = " << FSRationTemp << "\n";
        cout << "TLmom = " << TLmom << "\n\n";
    }

    if (HistogramClass == "TH1D") {
        if (PrintOut) { cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n\n"; }
        HistPlotter1D(HistCanvas, MScThesisPlotsList, filename, HistNameFeed_Numerator.c_str(), SampleName, SavePath, (to_string(Num) + "a_" + SaveName_Numerator));

        if (PrintOut) { cout << "\nHistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n\n"; }
        HistPlotter1D(HistCanvas, MScThesisPlotsList, filename, HistNameFeed_Denominator.c_str(), SampleName, SavePath, (to_string(Num) + "b_" + SaveName_Denominator));

        if (PrintOut) { cout << "\nFSRationTemp = " << FSRationTemp << "\n\n"; }
        HistPlotter1D(HistCanvas, MScThesisPlotsList, filename, FSRationTemp.c_str(), SampleName, SavePath, (to_string(Num) + "c_" + SaveName_FSR), TLmom);
    } else if (HistogramClass == "TH2D") {
        if (PrintOut) { cout << "\nHistNameFeed_Numerator = " << HistNameFeed_Numerator << "\n\n"; }
        HistPlotter2D(HistCanvas, MScThesisPlotsList, filename, HistNameFeed_Numerator.c_str(), SampleName, SavePath, (to_string(Num) + "a_" + SaveName_Numerator),
                      Results_plots, HistNameFeed_Denominator, HistNameFeed_Numerator);

        if (PrintOut) { cout << "\nHistNameFeed_Denominator = " << HistNameFeed_Denominator << "\n\n"; }
        HistPlotter2D(HistCanvas, MScThesisPlotsList, filename, HistNameFeed_Denominator.c_str(), SampleName, SavePath, (to_string(Num) + "b_" + SaveName_Denominator),
                      Results_plots, HistNameFeed_Denominator, HistNameFeed_Numerator);

        if (PrintOut) { cout << "\nFSRationTemp = " << FSRationTemp << "\n\n"; }
        HistPlotter2D(HistCanvas, MScThesisPlotsList, filename, FSRationTemp.c_str(), SampleName, SavePath, (to_string(Num) + "c_" + SaveName_FSR), Results_plots);
    }
}
