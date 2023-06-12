//
// Created by alons on 12/06/2023.
//

#ifndef GETBINFROMANG_H
#define GETBINFROMANG_H

using namespace std;

int GetBinFromAng(double Angle, double AngleBins, double AngleMin, double AngleMax, bool printOut = false, const string &AngleType = "") {
    int Bin = 0;
//    int Bin = -1;

    double Delta = (AngleMax - AngleMin) / AngleBins, min, max;

    for (int i = 0; i < AngleBins; i++) {
        double min_i, max_i;

        min_i = AngleMin + i * Delta, max_i = min_i + Delta;
        min = min_i, max = max_i;

        if (Angle >= min_i && Angle <= max_i) {
            if (printOut) {
                cout << "\n\n\nAngleType = " << AngleType << "\n";
                cout << "Angle = " << Angle << "\n";
                cout << "max_i = " << max_i << "\n";
                cout << "min_i = " << min_i << "\n";
                cout << "i = " << i << "\n";
                cout << "Delta = " << Delta << "\n\n\n";
            }

            Bin = i;
            break;
        }
    } // end of loop over electrons vector

    return Bin;
}

#endif //GETBINFROMANG_H
