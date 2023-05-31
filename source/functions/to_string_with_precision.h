//
// Created by alons on 04/05/2023.
//

#ifndef DATA_ANALYSER_TO_STRING_WITH_PRECISION_H
#define DATA_ANALYSER_TO_STRING_WITH_PRECISION_H

// to_string_with_precision function ------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="to_string_with_precision function">
/* Usage: convert a number to string with n figures after the decimal point in the plotting functions */

template<typename T>
string to_string_with_precision(const T a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
//</editor-fold>

#endif //DATA_ANALYSER_TO_STRING_WITH_PRECISION_H
