
#ifndef EXPERIMENTPARAMETERS_H
#define EXPERIMENTPARAMETERS_H

#include <stdio.h>
#include <unistd.h>

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
#include <sstream>

#include <cstdlib>
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
#include <iomanip>

#include "clas12reader.h"

#include "../TargetParameters/TargetParameters.h"

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
