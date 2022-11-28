//#include "gst.C"
#include "TruthLevelAnalyser.c"
//#include "TruthLevelAnalyser.h"
#include "Analyser.C"

int main() {
    auto start = std::chrono::system_clock::now(); // Start counting running time

    std::string AnalyserMode;
    string FileType = AnalyseFile.substr(AnalyseFile.find_last_of(".") + 1);
    cout << "FileType:\t" << FileType << "\n";


    //<editor-fold desc="AnalyserMode selector">
    if (FileType == "root") {
        AnalyserMode = "Truth level";
    } else if (FileType == "Detector Simulation") {
        AnalyserMode = "Detector Simulation";
    } else {
        cout << "\n";
        cout << "Could not find file type!" << "\n";
        cout << "\n";
    }
    //</editor-fold>

    if (AnalyserMode == "Truth level") {
        cout << "\n";
        cout << "\n";
        cout << "Running analysis on '" << AnalyserMode << "' mode..." << "\n";
        cout << "\n";

        gst g;
        g.Loop();

        cout << "\n";
        cout << "---------------------------------------------------------------------------\n";
        cout << "\n";
        cout << "Analysis mode:\t'" << AnalyserMode << "'" << "\n";
        cout << "Code version:\t" << Ver << "\n";
    } else if (AnalyserMode == "Detector Simulation") {
        cout << "\n";
        cout << "\n";
        cout << "Running analysis on '" << AnalyserMode << "' mode..." << "\n";
        cout << "\n";

        EventAnalyser(2,0);

        cout << "\n";
        cout << "---------------------------------------------------------------------------\n";
        cout << "\n";
        cout << "Analysis mode:\t'" << AnalyserMode << "'" << "\n";
        cout << "Code version:\t" << Ver << "\n";
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    std::cout << "Running time:\t" << elapsed_time_seconds.count() << " seconds\n";
    cout << "\n";
}
