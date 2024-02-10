#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void replaceSubStr(std::string &str, const std::string &subStr, const std::string &replacement) {
    // Search for the given substring in string
    size_t pos = str.find(subStr);
    // Iterate till index position of substring is valid
    while (pos != std::string::npos) {
        // Replace the first occurrence of substring in string
        // from position pos onwards
        str.replace(pos, subStr.length(), replacement);
        // Get the index position of next occurrence of substring in string
        pos = str.find(subStr, pos + replacement.length());
    }
}

void Replace() {
    string str = "#vec{P}_{tot} - #vec{q}";

    cout << "str = " << str << "\n";

    replaceSubStr(str, "#vec{", "BBB");

    cout << "str = " << str << "\n";
}
