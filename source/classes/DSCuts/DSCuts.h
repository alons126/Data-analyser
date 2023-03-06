//
// Created by alons on 06/03/2023.
//

#ifndef PROJECT_DSCUTS_H
#define PROJECT_DSCUTS_H

class DSCuts {
public:
    /* Constructors */
    DSCuts(std::string cv = "", std::string r = "", std::string p = "", std::string ac = "", double usmean = 0, double llim = -1, double ulim = -1); // Default constructor

//    DSCuts(std::string cv, std::string ac, double usmean, double llim, double ulim);

//    DSCuts(std::string cv, std::string r, std::string p, std::string ac);

//    DSCuts(std::string cv, std::string ac);

    /* Set functions */
    void SetCutVariable(std::string cv) { CutVariable = cv; }

    void SetRegion(std::string r) { Region = r; }

    void SetParticle(std::string p) { Particle = p; }

    void SetAppliedCuts(std::string ac) { AppliedCuts = ac; }

    void SetUSMean(double usmean) { Unsymmetric_Cuts.at(0) = usmean; }

    void SetLowerUSCut(double luscut) { Unsymmetric_Cuts.at(1) = luscut; }

    void SetUpperUSCut(double uuscut) { Unsymmetric_Cuts.at(2) = uuscut; }

    void SetSMean(double smean) { Unsymmetric_Cuts.at(0) = smean; }

    void SetLowerSCut(double lscut) { Unsymmetric_Cuts.at(1) = lscut; }

    void SetUpperSCut(double uscut) { Unsymmetric_Cuts.at(2) = uscut; }

    /* Get functions */
    double GetMean() { return Unsymmetric_Cuts.at(0); }

    double GetLowerUSCut() { return Unsymmetric_Cuts.at(1); }

    double GetUpperUSCut() { return Unsymmetric_Cuts.at(2); }

    double GetLowerSCut() { return Unsymmetric_Cuts.at(1); }

    double GetUpperSCut() { return Unsymmetric_Cuts.at(2); }

    std::string GetCutVariable(std::string cv) { return CutVariable; }

    std::string GetRegion(std::string r) { return Region; }

    std::string GetParticle(std::string p) { return Particle; }

    std::string GetAppliedCuts(std::string ac) { return AppliedCuts; }

private:
    std::string CutVariable, Region, Particle, AppliedCuts;
    vector<double> Unsymmetric_Cuts = {0, -1, -1}; // {mean, lower cut, upper cut}
    vector<double> Symmetric_Cuts = {0, -1}; // {mean, sigma}
};

DSCuts::DSCuts(std::string cv, std::string r, std::string p, std::string ac, double usmean, double llim, double ulim) {
    CutVariable = cv, Particle = p, AppliedCuts = ac;
    Unsymmetric_Cuts.at(0) = usmean, Unsymmetric_Cuts.at(1) = llim, Unsymmetric_Cuts.at(2) = ulim;

    if (r == "") {
        Region = "CD & FD";
    } else {
        Region = r;
    }
}

//DSCuts::DSCuts(std::string cv, std::string ac, double usmean, double llim, double ulim) {
//    CutVariable = cv, Region = "CD & FD", AppliedCuts = ac;
//    Unsymmetric_Cuts.at(0) = usmean, Unsymmetric_Cuts.at(1) = llim, Unsymmetric_Cuts.at(2) = ulim;
//}

//DSCuts::DSCuts(std::string cv, std::string r, std::string p, std::string ac) {
//    CutVariable = cv, Particle = p, AppliedCuts = ac;
//
//    if (r == "") {
//        Region = "CD & FD";
//    } else {
//        Region = r;
//    }
//}

//DSCuts::DSCuts(std::string cv, std::string ac) { CutVariable = cv, AppliedCuts = ac; }

#endif //PROJECT_DSCUTS_H
