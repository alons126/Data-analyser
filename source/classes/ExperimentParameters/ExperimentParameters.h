
#ifndef EXPERIMENTPARAMETERS_H
#define EXPERIMENTPARAMETERS_H

#include <iostream>

#include "../TargetParameters/TargetParameters.h"
#include "../../functions/GeneralFunctions.h"

class ExperimentParameters : public TargetParameters {
protected:
    std::string SampleName;
    double BeanEnergy;

    bool LocalSample = false;

    bool SimulationSample = false;
    bool DataSample = false;

    bool BeamAt2GeV = false;
    bool BeamAt4GeV = false;
    bool BeamAt6GeV = false;
public:

// ConfigureSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

    string ConfigureSampleName(const string &AnalyseFilePath, const string &AnalyseFileSample);

// ConfigureBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

    double ConfigureBeanEnergy(const string &sn);

// GetBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------------

    double GetBeanEnergy();

// ExperimentParameters function ----------------------------------------------------------------------------------------------------------------------------------------

    ExperimentParameters(const string &AnalyseFilePath, const string &AnalyseFileSample);

// other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    bool SLocal() const { return LocalSample; };

    bool SSample() const { return SimulationSample; };

    bool DSample() const { return DataSample; };

    bool IsBeamAt2GeV() const { return BeamAt2GeV; };

    bool IsBeamAt4GeV() const { return BeamAt4GeV; };

    bool IsBeamAt6GeV() const { return BeamAt6GeV; };
};


#endif //EXPERIMENTPARAMETERS_H
