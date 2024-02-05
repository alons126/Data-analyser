#include "TruthLevelAnalyser.c"
#include "DetectorSimulationAnalyser.C"

int main() {
    auto start = std::chrono::system_clock::now(); // Start counting running time

    std::string AnalyserMode;
    string FileType = AnalyseFile.substr(AnalyseFile.find_last_of(".") + 1);

    cout << "\n" << "AnalyseFile:\t" << AnalyseFile << "\n";
    cout << "FileType:\t" << FileType << "\n";

    //<editor-fold desc="AnalyserMode selector">
    if (FileType == "root") {
        AnalyserMode = "Truth level";
    } else if (FileType == "hipo") {
        AnalyserMode = "Detector Simulation";
    } else {
        cout << "\nCould not find file type!" << "\n\n";
    }
    //</editor-fold>

    cout << "Analyser mode:\t'" << AnalyserMode << "'\n";

    if (AnalyserMode == "Truth level") {
        gst g;
        g.Loop();
    } else if (AnalyserMode == "Detector Simulation") {
        EventAnalyser();
    }

    cout << "\n---------------------------------------------------------------------------\n\n";
    cout << "Analysis mode:\t'" << AnalyserMode << "'" << "\n";
    cout << "Code version:\t" << Ver << "\n";

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    if (elapsed_time_seconds.count() < 60) {
        std::cout << "Running time:\t" << elapsed_time_seconds.count() << " seconds\n\n";
    } else {
        std::cout << "Running time:\t" << to_string_with_precision(elapsed_time_minutes, 3) << " minutes\n\n";
    }
}
