//
// Created by alons on 04/05/2023.
//

#ifndef DATA_ANALYSER_FINDSUBSTRING_H
#define DATA_ANALYSER_FINDSUBSTRING_H

// findSubstring function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="findSubstring function">
/* Usage: in getBeanE */

bool findSubstring(string string1, string string2) {
    if (string1.find(string2) != string::npos) {
        return true;
    } else {
        return false;
    }
}
//</editor-fold>

#endif //DATA_ANALYSER_FINDSUBSTRING_H
