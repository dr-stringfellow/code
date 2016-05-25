////////////////////////////////////////////////////////////////////////////////
//
// MatrixMethod
// ------------
//
//            17/01/2011 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/MatrixMethod.h"


#include <TH1F.h>


#include <iostream>
#include <string>
#include <cmath>


using namespace std;


namespace matrixmethod
{


  ////////////////////////////////////////////////////////////////////////////////
  // implementation of functions declared in matrixmethod.h
  ////////////////////////////////////////////////////////////////////////////////
  
  //______________________________________________________________________________
  TH1F* getQcdTight(TH1F*  hloose,     TH1F*  htight,
		    double eps_qcd,    double eps_sig,
		    double err_eps_qcd,double err_eps_sig,
		    int    rebin)
  {
    TH1F* hResult(0);
    TH1F* hSig(0);
    return (getSigAndQcdTight(hSig,hResult,
			      hloose,htight,
			      eps_qcd,eps_sig,
			      err_eps_qcd,err_eps_sig,
			      rebin)) ? hResult : 0;
  }
  
  
  //______________________________________________________________________________
  bool getSigAndQcdTight(TH1F*& hsig, TH1F*& hqcd,
			 TH1F* hloose,TH1F* htight,
			 double eps_qcd,    double eps_sig,
			 double err_eps_qcd,double err_eps_sig,
			 int rebin)
  {
    TH1F* htmploose(0);
    TH1F* htmptight(0);
    
    if (!prepareHistos(hsig,hqcd,htmploose,htmptight,hloose,htight,rebin)) 
      return false;
    
    
    // separate signal from background
    int nbins=htmploose->GetNbinsX();
    for (int i=1;i<=nbins;i++) {
      
      double nl=htmploose->GetBinContent(i);
      double nt=htmptight->GetBinContent(i);
      
      double NTSIG=nt_sig(nl,nt,eps_qcd,eps_sig);
      double NTQCD=nt_qcd(nl,nt,eps_qcd,eps_sig);
      
      double ERRNTSIG=err_nt_sig(nl,nt,eps_qcd,eps_sig,err_eps_qcd,err_eps_sig);
      double ERRNTQCD=err_nt_qcd(nl,nt,eps_qcd,eps_sig,err_eps_qcd,err_eps_sig);
      
      hsig->SetBinContent(i,NTSIG);
      hsig->SetBinError(i,ERRNTSIG);
      
      hqcd->SetBinContent(i,NTQCD);
      hqcd->SetBinError(i,ERRNTQCD);
    }
    
    return true;
  }


  //______________________________________________________________________________
  bool getSigAndQcdLoose(TH1F*& hsig, TH1F*& hqcd,
			 TH1F* hloose,TH1F* htight,
			 double eps_qcd,    double eps_sig,
			 double err_eps_qcd,double err_eps_sig,
			 int rebin)
  {
    TH1F* htmploose(0);
    TH1F* htmptight(0);
    
    if (!prepareHistos(hsig,hqcd,htmploose,htmptight,hloose,htight,rebin))
      return false;
    
    // separate signal from background
    int nbins=htmploose->GetNbinsX();
    for (int i=1;i<=nbins;i++) {
	
      double nl=htmploose->GetBinContent(i);
      double nt=htmptight->GetBinContent(i);
	
      double nsig=n_sig(nl,nt,eps_qcd,eps_sig);
      double nqcd=n_qcd(nl,nt,eps_qcd,eps_sig);
	
      double errnsig=err_n_sig(nl,nt,eps_qcd,eps_sig,err_eps_qcd,err_eps_sig);
      double errnqcd=err_n_qcd(nl,nt,eps_qcd,eps_sig,err_eps_qcd,err_eps_sig);
	
      hsig->SetBinContent(i,nsig);
      hsig->SetBinError(i,errnsig);
	
      hqcd->SetBinContent(i,nqcd);
      hqcd->SetBinError(i,errnqcd);
    }
    
    return true;
  }


  //______________________________________________________________________________
  bool prepareHistos(TH1F*& hsig, TH1F*& hqcd,
		     TH1F*& hloose_tmp,TH1F*& htight_tmp,
		     TH1F* hloose,TH1F* htight,
		     int rebin)
  {
    hsig=0;
    hqcd=0;
    
    if (0==hloose||0==htight) return false;
    
    if (hloose->GetNbinsX()!=htight->GetNbinsX()||
	hloose->GetXaxis()->GetXmin()!=htight->GetXaxis()->GetXmin()||
	hloose->GetXaxis()->GetXmax()!=htight->GetXaxis()->GetXmax()) return false;
    
    hloose_tmp=(TH1F*)hloose->Clone();
    htight_tmp=(TH1F*)htight->Clone();
    hloose_tmp->Rebin(rebin);
    htight_tmp->Rebin(rebin);
    
    string signame(hloose->GetName());
    string qcdname(htight->GetName());
    
    signame+="_sig";
    qcdname+="_qcd";
    
    hsig=(TH1F*)hloose_tmp->Clone(signame.c_str());
    hqcd=(TH1F*)hloose_tmp->Clone(qcdname.c_str());
    
    hsig->Reset();
    hqcd->Reset();
    
    string sigtitle(hloose->GetTitle());
    string qcdtitle(htight->GetTitle());
    
    sigtitle+=" (Signal)";
    qcdtitle+=" (QCD)";
    
    hsig->SetTitle(sigtitle.c_str());
    hqcd->SetTitle(qcdtitle.c_str());
    
    return true;
  }


  //______________________________________________________________________________
  double n_sig(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return (nt-eps_qcd*nl)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double nt_sig(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return eps_sig*n_sig(nl,nt,eps_qcd,eps_sig);
  }


  //______________________________________________________________________________
  double n_qcd(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return (eps_sig*nl-nt)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double nt_qcd(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return eps_qcd*n_qcd(nl,nt,eps_qcd,eps_sig);
  }


  //______________________________________________________________________________
  double err_n_sig(double nl,         double nt,
		   double eps_qcd,    double eps_sig,
		   double err_eps_qcd,double err_eps_sig)
  {
    double dndes=dnsig_desig(nl,nt,eps_qcd,eps_sig);
    double dndeb=dnsig_deqcd(nl,nt,eps_qcd,eps_sig);
    double dndn1=dnsig_dnl(eps_qcd,eps_sig);
    double dndn2=dnsig_dnt(eps_qcd,eps_sig);
    
    double dn12=nl-nt; // square of n1 error
    double dn22=nt;    // square of n2 error
    
    return std::sqrt(dndes*dndes*err_eps_sig*err_eps_sig +
		     dndeb*dndeb*err_eps_qcd*err_eps_qcd +
		     dndn1*dndn1*dn12 +
		     dndn2*dndn2*dn22); 
  }


  //______________________________________________________________________________
  double err_nt_sig(double nl,         double nt,
		    double eps_qcd,    double eps_sig,
		    double err_eps_qcd,double err_eps_sig)
  {
    double dndes=dntsig_desig(nl,nt,eps_qcd,eps_sig);
    double dndeb=dntsig_deqcd(nl,nt,eps_qcd,eps_sig);
    double dndn1=dntsig_dnl(eps_qcd,eps_sig);
    double dndn2=dntsig_dnt(eps_qcd,eps_sig);
    
    double dn12=nl-nt; // square of n1 error
    double dn22=nt;    // square of n2 error
    
    return std::sqrt(dndes*dndes*err_eps_sig*err_eps_sig +
		     dndeb*dndeb*err_eps_qcd*err_eps_qcd +
		     dndn1*dndn1*dn12 +
		     dndn2*dndn2*dn22);
  }


  //______________________________________________________________________________
  double err_n_qcd(double nl,         double nt,
		   double eps_qcd,    double eps_sig,
		   double err_eps_qcd,double err_eps_sig)
  {
    double dndes=dnqcd_desig(nl,nt,eps_qcd,eps_sig);
    double dndeb=dnqcd_deqcd(nl,nt,eps_qcd,eps_sig);
    double dndn1=dnqcd_dnl(eps_qcd,eps_sig);
    double dndn2=dnqcd_dnt(eps_qcd,eps_sig);
    
    double dn12=nl-nt; // square of n1 error
    double dn22=nt;    // square of n2 error
    
    return std::sqrt(dndes*dndes*err_eps_sig*err_eps_sig +
		     dndeb*dndeb*err_eps_qcd*err_eps_qcd +
		     dndn1*dndn1*dn12 +
		     dndn2*dndn2*dn22);   
  }


  //______________________________________________________________________________
  double err_nt_qcd(double nl,         double nt,
		    double eps_qcd,    double eps_sig,
		    double err_eps_qcd,double err_eps_sig)
  {
    double dndes=dntqcd_desig(nl,nt,eps_qcd,eps_sig);
    double dndeb=dntqcd_deqcd(nl,nt,eps_qcd,eps_sig);
    double dndn1=dntqcd_dnl(eps_qcd,eps_sig);
    double dndn2=dntqcd_dnt(eps_qcd,eps_sig);
    
    double dn12=nl-nt; // square of n1 error
    double dn22=nt;    // square of n2 error
    
    return std::sqrt(dndes*dndes*err_eps_sig*err_eps_sig +
		     dndeb*dndeb*err_eps_qcd*err_eps_qcd +
		     dndn1*dndn1*dn12 +
		     dndn2*dndn2*dn22);
  }



  ////////////////////////////////////////////////////////////////////////////////
  // tight
  ////////////////////////////////////////////////////////////////////////////////

  //______________________________________________________________________________
  double dntsig_desig(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return eps_qcd*(eps_qcd*nl-nt)/(eps_sig-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dntsig_deqcd(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return eps_sig*(nt-eps_sig*nl)/(eps_sig-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dntqcd_desig(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return eps_qcd*(nt-eps_qcd*nl)/(eps_sig-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dntqcd_deqcd(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return eps_sig*(eps_sig*nl-nt)/(eps_sig-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dntsig_dnl(double eps_qcd,double eps_sig)
  {
    return -eps_sig*eps_qcd/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dntsig_dnt(double eps_qcd,double eps_sig)
  {
    return eps_sig*(1.0-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dntqcd_dnl(double eps_qcd,double eps_sig)
  {
    return eps_qcd*eps_sig/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dntqcd_dnt(double eps_qcd,double eps_sig)
  {
    return eps_qcd*(eps_sig-1.0)/(eps_sig-eps_qcd);
  }


  ////////////////////////////////////////////////////////////////////////////////
  // loose 
  ////////////////////////////////////////////////////////////////////////////////


  //______________________________________________________________________________
  double dnsig_desig(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return (eps_qcd*nl-nt)/(eps_sig-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dnsig_deqcd(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return (nt-eps_sig*nl)/(eps_sig-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dnqcd_desig(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return (nt-eps_qcd*nl)/(eps_sig-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dnqcd_deqcd(double nl,double nt,double eps_qcd,double eps_sig)
  {
    return (eps_sig*nl-nt)/(eps_sig-eps_qcd)/(eps_sig-eps_qcd);
  }



  //______________________________________________________________________________
  double dnsig_dnl(double eps_qcd,double eps_sig)
  {
    return -eps_qcd/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dnsig_dnt(double eps_qcd,double eps_sig)
  {
    return (1.0-eps_qcd)/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dnqcd_dnl(double eps_qcd,double eps_sig)
  {
    return eps_sig/(eps_sig-eps_qcd);
  }


  //______________________________________________________________________________
  double dnqcd_dnt(double eps_qcd,double eps_sig)
  {
    return (eps_sig-1.0)/(eps_sig-eps_qcd);
  }

}
