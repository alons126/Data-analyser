//
// Created by alons on 06/03/2023.
//

#ifndef PROJECT_DSCUTS_H
#define PROJECT_DSCUTS_H

class DSCuts {
public:
    /* Constructors */
    DSCuts(std::string cv = "", std::string r = "", std::string p = "", std::string ac = "", double mean = 0, double llim = -1, double ulim = -1); // Default constructor

    /* Set functions */
//    void InitSetter(std::string cv, std::string r, std::string p, std::string ac, double mean = 0, double llim = -1, double ulim = -1);

    void SetMeanHist(double mh) { MeanFromHistogram = mh; }

    void SetMeanFit(double mf) { MeanFromHistogram = mf; }

    void SetStdFactor(double sf) { FitStdFactor = sf; }

    void SetMean(double mean) { Cuts.at(0) = mean; }

    void SetLowerCut(double lcut) { Cuts.at(1) = lcut; }

    void SetUpperCut(double ucut) { Cuts.at(2) = ucut; }

    void SetCutVariable(std::string cv) { CutVariable = cv; }

    void SetRegion(std::string r) { Region = r; }

    void SetPart(std::string p) { Particle = p; }

    void SetAppliedCuts(std::string ac) { AppliedCuts = ac; }

    /* Get functions */
    double GetMeanHist() { return MeanFromHistogram; }

    double GetMeanFit() { return MeanFromFit; }

    double GetStdFactor() { return FitStdFactor; }

    double GetMean() { return Cuts.at(0); }

    double GetLowerCut() { return Cuts.at(1); }

    double GetUpperCut() { return Cuts.at(2); }

    std::string GetCutVariable() { return CutVariable; }

    std::string GetRegion() { return Region; }

    std::string GetPart() { return Particle; }

    int GetPartPDG() { return particlePDG; }

    std::string GetAppliedCuts() { return AppliedCuts; }

    double MeanFromHistogram, MeanFromFit, FitStdFactor;
    vector<double> Cuts = {0, -1, -1}; // {mean, lower cut, upper cut}
private:
    std::string CutVariable, Region, Particle, AppliedCuts;
    int particlePDG;
//    double MeanFromHistogram, MeanFromFit, FitStdFactor;
//    vector<double> Cuts = {0, -1, -1}; // {mean, lower cut, upper cut}
};

DSCuts::DSCuts(std::string cv, std::string r, std::string p, std::string ac, double mean, double llim, double ulim) { // Default constructor
    CutVariable = cv, Particle = p, AppliedCuts = ac;
    Cuts.at(0) = mean, Cuts.at(1) = llim, Cuts.at(2) = ulim;

    if (r == "") {
        Region = "CD & FD";
    } else {
        Region = r;
    }

    if (r == "CD") {
//        FitStdFactor = 1; // sigma factor for CD cuts
//        FitStdFactor = 1.5; // sigma factor for CD cuts
//        FitStdFactor = 2; // sigma factor for CD cuts
        FitStdFactor = 3; // sigma factor for CD cuts
    } else if (r == "FD") {
//        FitStdFactor = 1; // sigma factor for CD cuts
//        FitStdFactor = 2; // sigma factor for CD cuts
        FitStdFactor = 3; // sigma factor for CD cuts
    } else {
//        FitStdFactor = 1;
        FitStdFactor = 3;
    }

    if (p == "electron" || p == "Electron") {
        particlePDG = 11;
    } else if (p == "proton" || p == "Proton") {
        particlePDG = 2212;
    } else if (p == "kplus" || p == "Kplus") {
        particlePDG = 321;
    } else if (p == "kminus" || p == "Kminus") {
        particlePDG = -321;
    } else if (p == "piplus" || p == "Piplus") {
        particlePDG = 211;
    } else if (p == "piminus" || p == "Piminus") {
        particlePDG = -211;
    }
}

//void DSCuts::InitSetter(std::string cv, std::string r, std::string p, std::string ac, double mean, double llim, double ulim) {
//    CutVariable = cv, Particle = p, AppliedCuts = ac;
//    Cuts.at(0) = mean, Cuts.at(1) = llim, Cuts.at(2) = ulim;
//
//    if (r == "") {
//        Region = "CD & FD";
//    } else {
//        Region = r;
//    }
//
//    if (r == "CD") {
//        FitStdFactor = 1; // sigma factor for CD cuts
////        FitStdFactor = 1.5; // sigma factor for CD cuts
////        FitStdFactor = 2; // sigma factor for CD cuts
//    } else if (r == "FD") {
//        FitStdFactor = 2; // sigma factor for CD cuts
////        FitStdFactor = 3; // sigma factor for CD cuts
//    } else {
//        FitStdFactor = 1;
//    }
//}

#endif //PROJECT_DSCUTS_H