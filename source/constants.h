//
// Created by alons on 31/01/2023.
//

#ifndef DETECTORSIMULATIONANALYSER_C_CONSTANTS_H
#define DETECTORSIMULATIONANALYSER_C_CONSTANTS_H

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Physical constants
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Particle masses (in GeV):
double m_n = 0.939565;
double m_p = 0.938272;
double m_Kminus = 0.493677;
double m_Kplus = 0.493677;
double m_Kzero = 0.497614;
double m_piplus = 0.13957;
double m_piminus = 0.13957;
double m_pizero = 0.134977;
double m_e = 0.000510999;

// Speed of light in vacuum (in cm/ns):
const double c = 29.9792458;

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Particle PDG
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

int nPDG = 2112;
int pPDG = 2212;
int KplusPDG = 321;
int KminusPDG = -321;
int KzeroPDG = 311;
int piplusPDG = 211;
int piminusPDG = -211;
int pizeroPDG = 111;
int ePDG = 11;
int nuePDG = 12;
int muPDG = 13;
int numuPDG = 14;
int tauPDG = 15;
int nutauPDG = 16;
int phPDG = 22;

#endif //DETECTORSIMULATIONANALYSER_C_CONSTANTS_H
