
#ifndef TARGETPARAMETERS_H
#define TARGETPARAMETERS_H

class TargetParameters {
protected:
    string TargetElement;
    int TargetElementPDG, ProbePDG;
    double BindingEnergyToNucleus;

public:
    void SetTargetElement(string TargetElementName) { TargetElement = TargetElementName; }

    void SetTargetElementPDG(int TargetElementPDGNumber) { TargetElementPDG = TargetElementPDGNumber; }

    void SetProbePDG(int ProbePDGNumber) { ProbePDG = ProbePDGNumber; }

    void SetBindingEnergyToNucleus(double BindingEnergy) { BindingEnergyToNucleus = BindingEnergy; }

    string GetTargetElement() { return TargetElement; }

    int GetTargetElementPDG() { return TargetElementPDG; }

    int GetProbePDG() { return ProbePDG; }

    double GetBindingEnergyToNucleus() { return BindingEnergyToNucleus; }
};

#endif //TARGETPARAMETERS_H
