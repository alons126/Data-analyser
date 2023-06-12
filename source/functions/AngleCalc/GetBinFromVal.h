//
// Created by alons on 12/06/2023.
//

#ifndef GETBINFROMVAL_H
#define GETBINFROMVAL_H

using namespace std;

int GetBinFromVal(double Angle, double AngleBins, double AngleMax, double AngleMin) {
    int Bin = -1;

    double Delta = (AngleMax - AngleMin) / AngleBins;

    for (int i = 0; i < AngleBins; i++) {
        double min_i, max_i;

        min_i = AngleMin + i * Delta;
        max_i = min_i + Delta;

        cout << "\n\n\nAngle = " << Angle << "\n";
        cout << "max_i = " << max_i << "\n";
        cout << "min_i = " << min_i << "\n";
        cout << "i = " << i << "\n";
        cout << "Delta_x = " << Delta << "\n\n\n";

        if (Angle >= min_i && Angle <= max_i) {
            cout << "\n\n\nAngle = " << Angle << "\n";
            cout << "max_i = " << max_i << "\n";
            cout << "min_i = " << min_i << "\n";
            cout << "i = " << i << "\n";
            cout << "Delta_x = " << Delta << "\n\n\n";
            Bin = i;
            break;
        }
    } // end of loop over electrons vector

    if (Bin == -1) { cout << "\n\nGetBinFromVal: Bin not found! Exiting...\n\n", exit(EXIT_FAILURE); }

    return Bin;
}

#endif //GETBINFROMVAL_H
