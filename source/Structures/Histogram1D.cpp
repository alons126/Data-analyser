//
// Created by alons on 10/04/2023.
//

struct Histogram1D {
    std::string Proccess = "";
    std::string FinalState;
    std::string DetectorRegion = "";
    TH1D *Histogram1d;
    int NumOfXBins = 100;
    std::string HistogramTitle;
    std::string SaveDirectory;
    std::string SaveName;
};