//
// Created by alons on 23/12/2023.
//

#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include "CustomPlotsDirectory.cpp"

using namespace std;

class Settings: public CustomPlotsDirectory {
public:

// Default constructor ---------------------------------------------------------------------------------------------------------------------------------------------

    Settings() = default;

// AutoDisableVariables constructor --------------------------------------------------------------------------------------------------------------------------------

    void AutoDisableVariables();

};


#endif //PROJECT_SETTINGS_H
