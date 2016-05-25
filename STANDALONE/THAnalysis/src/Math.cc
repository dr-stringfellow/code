////////////////////////////////////////////////////////////////////////////////
//
// Math
// ----
//
//            14/04/2010 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/Math.h"


//______________________________________________________________________________
double deltaPhi(double phi1, double phi2)
{
  double result = phi1 - phi2;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}


//______________________________________________________________________________
double deltaR(double eta1, double phi1, double eta2, double phi2)
{
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return std::sqrt(deta*deta + dphi*dphi);
}


//______________________________________________________________________________
double mass(double pt1, double eta1, double phi1, double e1,
	    double pt2, double eta2, double phi2, double e2)
{
  TLorentzVector v1,v2;
  v1.SetPtEtaPhiE(pt1,eta1,phi1,e1);
  v2.SetPtEtaPhiE(pt2,eta2,phi2,e2);
  return (v1+v2).M();
}


double DeltaR(const TLorentzVector & v1, const TLorentzVector & v2) {
  Double_t DeltaR = 0.0;
  Double_t DeltaPhi = TMath::Abs(v1.Phi()-v2.Phi());
  if (DeltaPhi>TMath::Pi())
    DeltaPhi = 2*TMath::Pi() - DeltaPhi;
  Double_t DeltaEta = v1.Eta() - v2.Eta();
  DeltaR = sqrt(TMath::Power(DeltaPhi, 2) + TMath::Power(DeltaEta, 2));
  return DeltaR;
}