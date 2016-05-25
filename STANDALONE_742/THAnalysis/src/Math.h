#ifndef THANALYSIS_MATH_H
#define THANALYSIS_MATH_H


#include <TMath.h>
#include <TLorentzVector.h>

#include <cmath>


// calculate deltaPhi
double deltaPhi(double phi1, double phi2);


// calculate deltaR
double deltaR(double eta1, double phi1, double eta2, double phi2);


// calculate invariant mass of two objects
double mass(double pt1, double eta1, double phi1, double e1,
	    double pt2, double eta2, double phi2, double e2);

#endif
