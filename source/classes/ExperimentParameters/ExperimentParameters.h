
#ifndef EXPERIMENTPARAMETERS_H
#define EXPERIMENTPARAMETERS_H

#include <iostream>

#include "../TargetParameters/TargetParameters.h"
#include "../../functions/GeneralFunctions.h"

class ExperimentParameters : public TargetParameters {
protected:
    std::string SampleName;
    double BeanEnergy;
public:
    string ConfigureSampleName(string AnalyseFilePath, string AnalyseFileSample);

    double ConfigureBeanEnergy(string sn);

    double GetBeanEnergy();

    ExperimentParameters(string AnalyseFilePath, string AnalyseFileSample);
};


#endif //EXPERIMENTPARAMETERS_H
