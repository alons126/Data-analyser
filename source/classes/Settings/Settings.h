//
// Created by alons on 23/12/2023.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include "CustomPlotsDirectory.cpp"

using namespace std;

class Settings: public CustomPlotsDirectory {
private:

public:

// Default constructor ---------------------------------------------------------------------------------------------------------------------------------------------

    Settings() = default;

// AutoDisableVariables constructor --------------------------------------------------------------------------------------------------------------------------------

    void AutoDisableVariables();

};


#endif //SETTINGS_H
