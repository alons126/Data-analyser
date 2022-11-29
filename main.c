//#include "gst.C"
#include "TruthLevelAnalyser.c"
//#include "TruthLevelAnalyser.h"
//#include "Analyser.C"
#include "DetectorSimulationAnalyser.C"

int main() {
    auto start = std::chrono::system_clock::now(); // Start counting running time

    std::string AnalyserMode;
    string FileType = AnalyseFile.substr(AnalyseFile.find_last_of(".") + 1);

    cout << "AnalyseFile:\t" << AnalyseFile << "\n";
    cout << "FileType:\t" << FileType << "\n";

    //<editor-fold desc="AnalyserMode selector">
    if (FileType == "root") {
        AnalyserMode = "Truth level";
    } else if (FileType == "hipo") {
        AnalyserMode = "Detector Simulation";
    } else {
        cout << "\n";
        cout << "Could not find file type!" << "\n";
        cout << "\n";
    }
    //</editor-fold>

    cout << "Analyser mode:\t'" << AnalyserMode << "'\n";

    if (AnalyserMode == "Truth level") {
        gst g;
        g.Loop();

        cout << "\n";
        cout << "---------------------------------------------------------------------------\n";
        cout << "\n";
        cout << "Analysis mode:\t'" << AnalyserMode << "'" << "\n";
        cout << "Code version:\t" << Ver << "\n";
    } else if (AnalyserMode == "Detector Simulation") {
//        EventAnalyser(2,0);
//        EventAnalyser(1,1);

        EventAnalyser();

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
