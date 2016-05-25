#ifndef MATRIXMETHOD_H
#define MATRIXMETHOD_H 1


#include <Rtypes.h>


class TH1F;
class TMatrix;

namespace matrixmethod
{
  //______________________________________________________________________________
  TH1F* getQcdTight(TH1F*  hloose,     TH1F*  htight,
		    double eps_qcd,    double eps_sig,
		    double err_eps_qcd,double err_eps_sig,
		    int    rebin = 1);
  
  //______________________________________________________________________________
  bool getSigAndQcdTight(TH1F*& hsig,       TH1F*& hqcd,
			 TH1F*  hloose,     TH1F*  htight,
			 double eps_qcd,    double eps_sig,
			 double err_eps_qcd,double err_eps_sig,
			 int    rebin = 1);
  
  
  //______________________________________________________________________________
  bool getSigAndQcdLoose(TH1F*& hsig,       TH1F*& hqcd,
			 TH1F*  hloose,     TH1F*  htight,
			 double eps_qcd,    double eps_sig,
			 double err_eps_qcd,double err_eps_sig,
			 int    rebin = 1);
  
  
  //______________________________________________________________________________
  bool prepareHistos(TH1F*& hsig,      TH1F*& h1cd,
		     TH1F*& hloose_tmp,TH1F*& htight_tmp,
		     TH1F*  hloose,    TH1F* htight,
		     int    rebin=1);
  
  
  
  double n_sig(double nl,double nt,double eps_qcd,double eps_sig);
  double nt_sig(double nl,double nt,double eps_qcd,double eps_sig);
  double n_qcd(double nl,double nt,double eps_qcd,double eps_sig);
  double nt_qcd(double nl,double nt,double eps_qcd,double eps_sig);
  
  double err_n_sig(double nl,         double nt,
		   double eps_qcd,    double eps_sig,
		   double err_eps_qcd,double err_eps_sig);
  double err_nt_sig(double nl,         double nt,
		    double eps_qcd,    double eps_sig,
		    double err_eps_qcd,double err_eps_sig);
  double err_n_qcd(double nl,         double nt,
		   double eps_qcd,    double eps_sig,
		   double err_eps_qcd,double err_eps_sig);
  double err_nt_qcd(double nl,         double nt,
		    double eps_qcd,    double eps_sig,
		    double err_eps_qcd,double err_eps_sig);
  
  
  
  double dntsig_desig(double nl,double nt,double eps_qcd,double eps_sig);
  double dntsig_deqcd(double nl,double nt,double eps_qcd,double eps_sig);
  double dntqcd_desig(double nl,double nt,double eps_qcd,double eps_sig);
  double dntqcd_deqcd(double nl,double nt,double eps_qcd,double eps_sig);
  
  double dntsig_dnl(double eps_qcd,double eps_sig);
  double dntsig_dnt(double eps_qcd,double eps_sig);
  double dntqcd_dnl(double eps_qcd,double eps_sig);
  double dntqcd_dnt(double eps_qcd,double eps_sig);
  
  double dnsig_desig(double nl,double nt,double eps_qcd,double eps_sig);
  double dnsig_deqcd(double nl,double nt,double eps_qcd,double eps_sig);
  double dnqcd_desig(double nl,double nt,double eps_qcd,double eps_sig);
  double dnqcd_deqcd(double nl,double nt,double eps_qcd,double eps_sig);
  
  double dnsig_dnl(double eps_qcd,double eps_sig);
  double dnsig_dnt(double eps_qcd,double eps_sig);
  double dnqcd_dnl(double eps_qcd,double eps_sig);
  double dnqcd_dnt(double eps_qcd,double eps_sig);
  
}

#endif
