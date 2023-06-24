
#ifndef EXPERIMENTPARAMETERS_H
#define EXPERIMENTPARAMETERS_H

#include <iostream>

#include "../TargetParameters/TargetParameters.h"
#include "../../functions/GeneralFunctions.h"

class ExperimentParameters : public TargetParameters {
protected:
    std::string SampleName;
    double BeanEnergy;

    bool SimulationSample = false;
    bool DataSample = false;

    bool BeamAt2GeV = false;
    bool BeamAt4GeV = false;
    bool BeamAt6GeV = false;
public:

// ConfigureSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

    string ConfigureSampleName(string AnalyseFilePath, string AnalyseFileSample);

// ConfigureBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

    double ConfigureBeanEnergy(string sn);

// GetBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------------

    double GetBeanEnergy();

// ExperimentParameters function ----------------------------------------------------------------------------------------------------------------------------------------

    ExperimentParameters(string AnalyseFilePath, string AnalyseFileSample);

// other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    bool SSample() { return SimulationSample; };

    bool DSample() { return DataSample; };

    bool IsBeamAt2GeV() { return BeamAt2GeV; };
};


#endif //EXPERIMENTPARAMETERS_H
