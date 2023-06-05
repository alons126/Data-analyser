//
// Created by alons on 05/06/2023.
//

#ifndef SETTYPE_H
#define SETTYPE_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "../GeneralFunctions.h"

using namespace std;

string SetType(const string &Title) {
    string Type;

    if (findSubstring(Title, "momentum")) {
        Type = "momentum";
    } else if (findSubstring(Title, "#theta")) {
        Type = "theta";
    } else if (findSubstring(Title, "#phi")) {
        Type = "phi";
    }

    return Type;
}

#endif //SETTYPE_H
