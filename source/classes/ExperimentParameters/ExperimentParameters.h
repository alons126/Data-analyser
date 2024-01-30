
#ifndef EXPERIMENTPARAMETERS_H
#define EXPERIMENTPARAMETERS_H

#include <iostream>

#include "../TargetParameters/TargetParameters.h"
#include "../DSCuts/DSCuts.h"
#include "../../functions/GeneralFunctions.h"

#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;
using namespace clas12;

class ExperimentParameters : public TargetParameters {
protected:
    string SampleName;
    string VaryingSampleName; // for AMaps, WMaps and nRes
    double BeanEnergy;

    bool LocalSample = false;

    bool SimulationSample = false;
    bool DataSample = false;

    bool BeamAt2GeV = false;
    bool BeamAt4GeV = false;
    bool BeamAt6GeV = false;

    DSCuts Vz_cuts_def = DSCuts("Vertex z component", "", "", "1e cut", 0, -15, 5);
    DSCuts Vz_cuts, Vz_cuts_FD, Vz_cuts_CD, Vz_cuts_FD_def = Vz_cuts_def, Vz_cuts_CD_def = Vz_cuts_def;
    DSCuts dVz_cuts_def = DSCuts("dVz", "", "", "1e cut", 0, -8, 4);
    DSCuts dVz_cuts, dVz_cuts_FD, dVz_cuts_CD, dVz_cuts_FD_def = dVz_cuts_def, dVz_cuts_CD_def = dVz_cuts_def;

public:

// Constructor ----------------------------------------------------------------------------------------------------------------------------------------

    ExperimentParameters(const string &AnalyseFilePath, const string &AnalyseFileSample);

// ConfigureSampleName function -----------------------------------------------------------------------------------------------------------------------------------------

    string ConfigureSampleName(const string &AnalyseFilePath, const string &AnalyseFileSample);

// ConfigureVaryingSampleName function -----------------------------------------------------------------------------------------------------------------------------------

    void ConfigureVaryingSampleName(const string &sn);

// ConfigureVz_cuts function --------------------------------------------------------------------------------------------------------------------------------------------

    void ConfigureVz_cuts(const string &sn);

// ConfiguredVz_cuts function -------------------------------------------------------------------------------------------------------------------------------------------

    void ConfiguredVz_cuts(const string &sn);

// ConfigureBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

    double ConfigureBeanEnergy(const string &sn);

// GetBeanEnergy function -----------------------------------------------------------------------------------------------------------------------------------------------

    double GetBeanEnergy();

// GetVz_cuts function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetVz_cuts() { return Vz_cuts; };

// GetVz_cuts_FD function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetVz_cuts_FD() { return Vz_cuts_FD; };

// GetVz_cuts_CD function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetVz_cuts_CD() { return Vz_cuts_CD; };

// GetdVz_cuts function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetdVz_cuts() { return dVz_cuts; };

// GetdVz_cuts_FD function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetdVz_cuts_FD() { return dVz_cuts_FD; };

// GetdVz_cuts_CD function -----------------------------------------------------------------------------------------------------------------------------------------------

    DSCuts GetdVz_cuts_CD() { return dVz_cuts_CD; };

// AddToHipoChain function ----------------------------------------------------------------------------------------------------------------------------------------

    void AddToHipoChain(HipoChain &chain, const string &sn, const string &AnalyseFilePath, const string &AnalyseFileSample, const string &AnalyseFile);

// other functions ------------------------------------------------------------------------------------------------------------------------------------------------------

    bool SLocal() const { return LocalSample; };

    bool SSample() const { return SimulationSample; };

    bool DSample() const { return DataSample; };

    bool IsBeamAt2GeV() const { return BeamAt2GeV; };

    bool IsBeamAt4GeV() const { return BeamAt4GeV; };

    bool IsBeamAt6GeV() const { return BeamAt6GeV; };

    string GetVaryingSampleName() { return VaryingSampleName; };
};


#endif //EXPERIMENTPARAMETERS_H
