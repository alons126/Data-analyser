//
// Created by alons on 06/03/2023.
//

#ifndef PROJECT_DSCUTS_H
#define PROJECT_DSCUTS_H

class DSCuts {
public:
    DSCuts(std::string cv, std::string r, std::string p, std::string ac, double mean, double llim, double ulim);

    DSCuts(std::string cv, std::string ac, double mean, double llim, double ulim);

    DSCuts(std::string cv, std::string r, std::string p, std::string ac);

    DSCuts(std::string cv, std::string ac);

    void SetCutVariable(std::string cv) { CutVariable = cv; }

    void SetRegion(std::string r) { Region = r; }

    void SetParticle(std::string p) { Particle = p; }

    void SetAppliedCuts(std::string ac) { AppliedCuts = ac; }

    double GetMean() { return Cuts.at(0); }

    double GetLowerCut() { return Cuts.at(1); }

    double GetUpperCut() { return Cuts.at(2); }

    std::string GetCutVariable(std::string cv) { return CutVariable; }

    std::string GetRegion(std::string r) { return Region; }

    std::string GetParticle(std::string p) { return Particle; }

    std::string GetAppliedCuts(std::string ac) { return AppliedCuts; }

private:
    std::string CutVariable, Region, Particle, AppliedCuts;
    vector<double> Cuts = {0, -1, -1}; // {mean, lower cut, upper cut}
};

DSCuts::DSCuts(std::string cv, std::string r, std::string p, std::string ac, double mean, double llim, double ulim) {
    CutVariable = cv, Region = r, Particle = p, AppliedCuts = ac;
    Cuts.at(0) = mean, Cuts.at(1) = llim, Cuts.at(2) = ulim;
}

DSCuts::DSCuts(std::string cv, std::string ac, double mean, double llim, double ulim) {
    CutVariable = cv, Region = "CD & FD", AppliedCuts = ac;
    Cuts.at(0) = mean, Cuts.at(1) = llim, Cuts.at(2) = ulim;
}

DSCuts::DSCuts(std::string cv, std::string r, std::string p, std::string ac) { CutVariable = cv, Region = r, Particle = p, AppliedCuts = ac; }

DSCuts::DSCuts(std::string cv, std::string ac) { CutVariable = cv, AppliedCuts = ac; }

#endif //PROJECT_DSCUTS_H
