
#ifndef TARGETPARAMETERS_H
#define TARGETPARAMETERS_H

class TargetParameters {
protected:
    std::string TargetElement;
    int TargetElementPDG; // Target PDG used in GENIE
    int ProbePDG; // Incident lepton PDG

    double BindingEnergyToNucleus;

    /* Target PDG in GENIE code - 10LZZZAAAI */
    int TotalBaryonNumber_A;
    int TotalChargeNumber_Z;
    int StrangeQuarksNumber_L;
    int IsomerNumber_I;
//    10LZZZAAAI
//    1000080160

public:
    void SetTargetElement(std::string TargetElementName) { TargetElement = TargetElementName; }

    void SetTargetElementPDG(int TargetElementPDGNumber) { TargetElementPDG = TargetElementPDGNumber; }

    void SetProbePDG(int ProbePDGNumber) { ProbePDG = ProbePDGNumber; }

    void SetBindingEnergyToNucleus(double BindingEnergy) { BindingEnergyToNucleus = BindingEnergy; }

    std::string GetTargetElement() { return TargetElement; }

    int GetTargetElementPDG() { return TargetElementPDG; }

    int GetProbePDG() { return ProbePDG; }

    double GetBindingEnergyToNucleus() { return BindingEnergyToNucleus; }
};

#endif //TARGETPARAMETERS_H
