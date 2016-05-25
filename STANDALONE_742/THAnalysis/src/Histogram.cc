////////////////////////////////////////////////////////////////////////////////
//
// Histogram
// ---------
//
//            01/09/2010 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/Histogram.h"

#include <iostream>
#include <cmath>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTION / DESTRUCTION
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
Histogram::Histogram()
  : nbinsx_(0)
  , xmin_(0.0)
  , xmax_(0.0)
  , logx_(false)
  , logy_(false)
{

}


//______________________________________________________________________________
Histogram::Histogram(const string& name,
		     const string& title,
		     const string& expr,
		     unsigned      nbinsx,
		     double        xmin,
		     double        xmax,
		     bool          logx,
		     bool          logy)
  : name_(name)
  , expr_(expr)
  , nbinsx_(nbinsx)
  , xmin_(xmin)
  , xmax_(xmax)
  , logx_(logx)
  , logy_(logy)
{
  setXYTitle(title);
}


//______________________________________________________________________________
Histogram::Histogram(const string& name,
		     const string& title,
		     const string& expr,
		     const string& selection,
		     unsigned      nbinsx,
		     double        xmin,
		     double        xmax,
		     bool          logx,
		     bool          logy)
  : name_(name)
  , expr_(expr)
  , selection_(selection)
  , nbinsx_(nbinsx)
  , xmin_(xmin)
  , xmax_(xmax)
  , logx_(logx)
  , logy_(logy)
{
  setXYTitle(title);
}

//______________________________________________________________________________
Histogram::Histogram(const string& name,
		     const string& title,
		     const string& expr,
		     std::vector<double> binsx,
		     bool          logx,
		     bool          logy)
  : name_(name)
  , expr_(expr)
  , binsx_(binsx)
  , logx_(logx)
  , logy_(logy)
{
  setXYTitle(title);
}


//______________________________________________________________________________
Histogram::Histogram(const string& name,
		     const string& title,
		     const string& expr,
		     const string& selection,
		     std::vector<double> binsx,
		     bool          logx,
		     bool          logy)
  : name_(name)
  , expr_(expr)
  , selection_(selection)
  , binsx_(binsx)
  , logx_(logx)
  , logy_(logy)
{
  setXYTitle(title);
}



////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void Histogram::setXYTitle(const std::string& title)
{
  xtitle_="";
  ytitle_="";
  string tmp(title);
  size_t pos = tmp.find(';');
  if (pos==string::npos) return;
  tmp     = tmp.substr(pos+1);
  pos     = tmp.find(';');
  xtitle_ = tmp.substr(0,pos);
  if (pos==string::npos) return;
  ytitle_ = tmp.substr(pos+1);
}


//______________________________________________________________________________
TH1F* Histogram::create(const string& prefix)
{
  if (binsx_.size()==0)
    return new TH1F((prefix+name_).c_str(),title().c_str(),nbinsx_,xmin_,xmax_);
  
  double* bx = new double[binsx_.size()];
  for (unsigned i=0;i<binsx_.size();i++) bx[i]=binsx_[i];
  return new TH1F((prefix+name_).c_str(),title().c_str(),binsx_.size()-1,bx);
}

//______________________________________________________________________________
void Histogram::divideBinContentByBinWidth(TH1F* h)
{
  for (int ibin=1;ibin<=h->GetNbinsX();ibin++) { 
    float binwidth   = h->GetBinWidth(ibin);
    float bincontent = h->GetBinContent(ibin);
    float binerror   = h->GetBinError(ibin);
    h->SetBinContent(ibin,bincontent/binwidth);
    h->SetBinError(ibin,binerror/binwidth); 
  }
  return;
}


//______________________________________________________________________________
bool Histogram::hasVariableBinWidth(TH1* h)
{
  for (int ibin=2;ibin<=h->GetNbinsX();ibin++)
    if (h->GetBinWidth(ibin)!=h->GetBinWidth(1)) return true;
  return false;
}


//______________________________________________________________________________
map<string,Histogram> Histogram::getHistogramMap()
{
  unsigned NJTPT     =    100;
  unsigned NJTETA    =     50;
  unsigned NJTPHI    =     30;
  double   JTPTMIN   =   10.0;
  double   JTPTMAX   =  600.0;
  double   JTETAMAX  =    5.0;
  double   JTPHIMAX  =  3.142;
  
  double x[] = {-JTETAMAX, -3.2, -2.4, -2.2, -2.0, -1.8, -1.6, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 3.2, JTETAMAX};
  std::vector<double> listEta(x, x + sizeof x / sizeof x[0]);

  double x2[] = { 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 3.2, JTETAMAX};
  std::vector<double> listEta2(x2, x2 + sizeof x2 / sizeof x2[0]);

  unsigned NMET      =    100;
  unsigned NMETPHI   =     30;
  double   METMIN    =    0.0;
  double   METMAX    =  100.0;
  double   METPHIMAX =  3.142;

  unsigned NLEPPT    =     50;
  unsigned NLEPETA   =     40;
  unsigned NLEPPHI   =     20;
  double   LEPPTMIN  =   10.0;
  double   LEPPTMAX  =  200.0;
  double   LEPETAMAX =    2.5;
  double   LEPPHIMAX =  3.142;
  
  unsigned NFRAC     =     30;
  double   FRACMAX   = 1.0001;


  
  std::map<std::string,Histogram> result;
  
  //
  // PU DENSITY
  //
  result["Rho"]=Histogram("Rho",";PU-density #rho [GeV/A];Events","rho",26,0.0,25.);
  result["Rho25"]=Histogram("Rho25",";PU-density #rho [GeV/A];Events","rho25",26,0.0,25.);

  //
  // JETS
  //
  
  // JET MULTIPLICITY
  result["NJet15"]=Histogram("NJet15",";N_{jets} (p_{T}>15 GeV);Events","Sum$(jtpt>15.)",9,-0.5,8.5);
  result["NJet20"]=Histogram("NJet20",";N_{jets} (p_{T}>20 GeV);Events","Sum$(jtpt>20.)",9,-0.5,8.5);
  result["NJet25"]=Histogram("NJet25",";N_{jets} (p_{T}>25 GeV);Events","Sum$(jtpt>25.)",9,-0.5,8.5);
  result["NJet30"]=Histogram("NJet30",";N_{jets} (p_{T}>30 GeV);Events","Sum$(jtpt>30.)",9,-0.5,8.5);
  result["NJet15_25"]=Histogram("NJet15_25",";N_{jets} (p_{T}>15 GeV,|#eta|<4.7);Events","Sum$(jtpt>15.&&abs(jteta)<4.7)",9,-0.5,8.5);
  result["NJet20_25"]=Histogram("NJet20_25",";N_{jets} (p_{T}>20 GeV,|#eta|<4.7);Events","Sum$(jtpt>20.&&abs(jteta)<4.7)",9,-0.5,8.5);
  result["NJet25_25"]=Histogram("NJet25_25",";N_{jets} (p_{T}>25 GeV,|#eta|<4.7);Events","Sum$(jtpt>25.&&abs(jteta)<4.7)",9,-0.5,8.5);
  result["NJet30_25"]=Histogram("NJet30_25",";N_{jets} (p_{T}>30 GeV,|#eta|<4.7);Events","Sum$(jtpt>30.&&abs(jteta)<4.7)",9,-0.5,8.5);
  

  // JET ENERGY
  result["JetRawE"]=Histogram("JetRawE",";jet E^{raw} [GeV];Jets","jte*jtrawpt/jtpt",NJTPT,JTPTMIN,JTPTMAX);
  result["JetE"]=Histogram("JetE",";jet E [GeV];Jets","jte",NJTPT,JTPTMIN,JTPTMAX);
  
  // APLANARITY & SPHERICITY
  result["Aplanarity"]=Histogram("Aplanarity",";aplanarity;Events","aplanarity",30,0,80);
  result["Sphericity"]=Histogram("Sphericity",";sphericity;Events","sphericity",30,0,280);


  // JET ID

  result["JetNEMFraction"]=Histogram("JetNEMFraction",";neutral-e.m. energy fraction;Jets","jtnemfrac",50,0.0,1.0);
  result["JetChEMFraction"]=Histogram("JetChEMFraction",";charged-e.m. energy fraction;Jets","jtchemfrac",50,0.0,1.0);
  result["JetNHadFraction"]=Histogram("JetNHadFraction",";neutral-hadron energy fraction;Jets","jtnhadronfrac",50,0.0,1.0);
  result["JetChHadFraction"]=Histogram("JetChHadFraction",";charged-hadron energy fraction;Jets","jtchhadronfrac",50,0.0,1.0);
  result["JetHFHadFraction"]=Histogram("JetHFHadFraction",";HF hadron energy fraction;Jets","jthfhadronfrac",50,0.0,1.0);
  result["JetNEMEnergy"]=Histogram("JetNEMEnergy",";neutral-e.m. energy;Jets","jtnemfrac*(jte*jtrawpt/jtpt)",NJTPT,JTPTMIN,JTPTMAX);
  result["JetChEMEnergy"]=Histogram("JetChEMEnergy",";charged-e.m. energy;Jets","jtchemfrac*(jte*jtrawpt/jtpt)",50,0,50);
  result["JetNHadEnergy"]=Histogram("JetNHadEnergy",";neutral-hadron energy;Jets","jtnhadronfrac*(jte*jtrawpt/jtpt)",50,0,200);
  result["JetChHadEnergy"]=Histogram("JetChHadEnergy",";charged-hadron energy;Jets","jtchhadronfrac*(jte*jtrawpt/jtpt)",50,0,300);
  result["JetHadFraction"]=Histogram("JetHadFraction",";neutral+HF hadron energy fraction;Jets","jthfhadronfrac+jtnhadronfrac",50,0,1);
  result["JetHadEnergy"]=Histogram("JetHadEnergy",";neutral+HF hadron energy;Jets","(jthfhadronfrac+jtnhadronfrac)*(jte*jtrawpt/jtpt)",50,0,300);
  


  // JET AREA
  result["JetA"]=Histogram("JetA",";jet area;Jets","jtarea",50,0.0,1.5);
  result["Jet1A"]=Histogram("Jet1A",";1. jet area;Events","jtarea[0]",50,0.0,1.5);
  result["JetAxRho"]=Histogram("JetAxRho",";jet A#times#rho [GeV];Jets","jtarea*rho",32,0.0,15.);
  result["Jet1AxRho"]=Histogram("Jet1AxRho",";leading jet A#times#rho [GeV];Events","jtarea[0]*rho",32,0.0,15.);
  
  // JET PT
  result["JetRawPt"]=Histogram("JetRawPt",";jet p^{raw}_{T}[GeV];Jets","jtpt/jtjec",NJTPT,JTPTMIN,JTPTMAX);
  result["JetPt"]=Histogram("JetPt",";jet p_{T} [GeV];Jets","jtpt",NJTPT,JTPTMIN,JTPTMAX);
  result["Jet12Pt"]=Histogram("Jet12Pt",";jet p_{T} [GeV];Jets","jtpt","jtrank<2",NJTPT,JTPTMIN,JTPTMAX);
  result["Jet1Pt"]=Histogram("Jet1Pt",";1.jet p_{T} [GeV];Jets","jtpt[0]",NJTPT,JTPTMIN,JTPTMAX);
  result["Jet2Pt"]=Histogram("Jet2Pt",";2.jet p_{T} [GeV];Jets","jtpt[1]",NJTPT,JTPTMIN,JTPTMAX);
  result["Jet3Pt"]=Histogram("Jet3Pt",";3.jet p_{T} [GeV];Jets","jtpt[2]",NJTPT,JTPTMIN,JTPTMAX);
  result["Jet3RelPt"]=Histogram("Jet3RelPt",";3.jet p_{T} fraction;Events","jtpt[2]*2./(jtpt[0]+jtpt[1])",NFRAC,0.,FRACMAX);
  result["Jet3RawPt"]=Histogram("Jet3RawPt",";3.jet p^{raw}_{T} [GeV];Events","jtpt[2]/jtjec[2]",NJTPT,JTPTMIN,JTPTMAX);
  result["Jet3RSoft"]=Histogram("Jet3RSoft",";3.jet p^{raw}_{T}/p_{T};Events","(1./jtjec[2])",NJTPT,0.0,1.0);
  result["LightJetPt"]=Histogram("LightJetPt","; p_{T}(light jet) [GeV];Events","jtpt[ljtidx]",30,0,300);
  
  // JET ETA
  result["JetEta2"]=Histogram("JetEta",";jet #eta;Jets","jteta", listEta);
  result["Jet12Eta2"]=Histogram("Jet12Eta",";jet #eta;Jets","jteta","jtrank<2", listEta);
  result["Jet1Eta2"]=Histogram("Jet1Eta",";1.jet #eta;Jets","jteta[0]", listEta);
  result["Jet2Eta2"]=Histogram("Jet2Eta",";2.jet #eta;Jets","jteta[1]", listEta);
  result["Jet3Eta2"]=Histogram("Jet3Eta",";3.jet #eta;Jets","jteta[2]", listEta);
  result["JetAbsEta2"]=Histogram("JetAbsEta",";jet |#eta|;Jets","abs(jteta)", listEta2);
  result["Jet12AbsEta2"]=Histogram("Jet12AbsEta",";jet |#eta|;Jets","abs(jteta)","jtrank<2", listEta2);
  result["Jet1AbsEta2"]=Histogram("Jet1AbsEta",";jet |#eta|;Jets","abs(jteta[0])", listEta2);
  result["Jet2AbsEta2"]=Histogram("Jet2AbsEta",";jet |#eta|;Jets","abs(jteta[1])", listEta2);
  result["Jet3AbsEta2"]=Histogram("Jet3AbsEta",";jet |#eta|;Jets","abs(jteta[2])", listEta2);
  result["LightJetEta2"]=Histogram("LightJetEta2","; #eta(light jet);Jets","jteta[ljtidx]", listEta2);
  
  result["JetEta"]=Histogram("JetEta",";jet #eta;Jets","jteta",NJTETA+1,-JTETAMAX,JTETAMAX);
  result["Jet12Eta"]=Histogram("Jet12Eta",";jet #eta;Jets","jteta","jtrank<2",NJTETA,-JTETAMAX,JTETAMAX);
  result["Jet1Eta"]=Histogram("Jet1Eta",";1.jet #eta;Jets","jteta[0]",NJTETA+1,-JTETAMAX,JTETAMAX);
  result["Jet2Eta"]=Histogram("Jet2Eta",";2.jet #eta;Jets","jteta[1]",NJTETA+1,-JTETAMAX,JTETAMAX);
  result["Jet3Eta"]=Histogram("Jet3Eta",";3.jet #eta;Jets","jteta[2]",NJTETA+1,-JTETAMAX,JTETAMAX);
  result["JetAbsEta"]=Histogram("JetAbsEta",";jet |#eta|;Jets","abs(jteta)",NJTETA,0,JTETAMAX);
  result["Jet12AbsEta"]=Histogram("Jet12AbsEta",";jet |#eta|;Jets","abs(jteta)","jtrank<2",NJTETA,0,JTETAMAX);
  result["Jet1AbsEta"]=Histogram("Jet1AbsEta",";jet |#eta|;Jets","abs(jteta[0])",NJTETA,0,JTETAMAX);
  result["Jet2AbsEta"]=Histogram("Jet2AbsEta",";jet |#eta|;Jets","abs(jteta[1])",NJTETA,0,JTETAMAX);
  result["Jet3AbsEta"]=Histogram("Jet3AbsEta",";jet |#eta|;Jets","abs(jteta[2])",NJTETA,0,JTETAMAX);
  result["LightJetEta"]=Histogram("LightJetEta","; #eta(light jet);Jets","jteta[ljtidx]",NJTETA+1,-JTETAMAX,JTETAMAX);

  // JET PHI
  result["JetPhi"]=Histogram("JetPhi",";jet #varphi;Jets","jtphi",NJTPHI+1,-JTPHIMAX,JTPHIMAX);
  result["Jet12Phi"]=Histogram("Jet12Phi",";jet #varphi;Jets","jtphi","jtrank<2",NJTPHI+1,-JTPHIMAX,JTPHIMAX);
  result["Jet1Phi"]=Histogram("Jet1Phi",";1.jet #varphi;Jets","jtphi[0]",NJTPHI+1,-JTPHIMAX,JTPHIMAX);
  result["Jet2Phi"]=Histogram("Jet2Phi",";2.jet #varphi;Jets","jtphi[1]",NJTPHI+1,-JTPHIMAX,JTPHIMAX);
  result["Jet3Phi"]=Histogram("Jet3Phi",";3.jet #varphi;Jets","jtphi[2]",NJTPHI+1,-JTPHIMAX,JTPHIMAX);

  // JET CSV
  result["JetCSVT"]=Histogram("JetCSVT",";jet CSV;Jets","jtcsvt",NFRAC,0.,FRACMAX);
  result["JetCSVT1"]=Histogram("JetCSVT1",";1. jet CSV;Jets","jtcsvt[0]",NFRAC,0.,FRACMAX);
  result["JetCSVT2"]=Histogram("JetCSVT2",";2. jet CSV;Jets","jtcsvt[1]",NFRAC,0.,FRACMAX);
  result["JetCSVT3"]=Histogram("JetCSVT3",";3. jet CSV;Jets","jtcsvt[2]",NFRAC,0.,FRACMAX);
  result["JetCSVTCorr"]=Histogram("JetCSVTCorr",";jet corr. CSV;Jets","jtcsvtcorr[][0]",NFRAC,0.,FRACMAX);
  result["JetCSVT1Corr"]=Histogram("JetCSVT1Corr",";1. jet corr. CSV;Jets","jtcsvtcorr[0][0]",NFRAC,0.,FRACMAX);
  result["JetCSVT2Corr"]=Histogram("JetCSVT2Corr",";2. jet corr. CSV;Jets","jtcsvtcorr[1][0]",NFRAC,0.,FRACMAX);
  result["JetCSVT3Corr"]=Histogram("JetCSVT3Corr",";3. jet corr. CSV;Jets","jtcsvtcorr[2][0]",NFRAC,0.,FRACMAX);



  // JET MUON
  result["Jet1MuPt"]=Histogram("Jet1MuPt",";p_{T}(#mu in 1.jet) [GeV];Events","jtmupt[0]","jtmupt[0]>0.0",100,0.,25.);
  
  // DELTA-PHI(JET,MET)
  result["JetDPhiMET"]=Histogram("JetDPhiMET",";#Delta#varphi(jet,#slash{E}_{T});Jets","abs(deltaPhi(jtphi,metphi))",NJTPHI,0,JTPHIMAX);
  result["Jet12DPhiMET"]=Histogram("Jet12DPhiMET",";#Delta#varphi(jet,#slash{E}_{T});Jets","abs(deltaPhi(jtphi,metphi))","jtrank<2",NJTPHI,0,JTPHIMAX);


  //DELTA R (TOP, LIGHT JET)
  result["DRTopLight"]=Histogram("DRTopLight",";#Delta R(t,light jet);Events","deltaR(jteta[topjtidx],jtphi[topjtidx],jteta[ljtidx],jtphi[ljtidx])",30,0,7.);

  // JET COMPOSITION
  result["JetN"]=Histogram("JetN",";number of jet constituents;Jets","jtn",41,-0.5,40.5);
  result["Jet12N"]=Histogram("Jet12N",";number of jet constituents;Jets","jtn","jtrank<2",41,-0.5,40.5);
  result["JetN90"]=Histogram("JetN90",";jet n90;Jets","jtn90",41,-0.5,40.5);
  result["Jet12N90"]=Histogram("Jet12N90",";jet n90;Jets","jtn90","jtrank<2",41,-0.5,40.5);
  result["JetNTrk"]=Histogram("JetNTrk",";jet track multiplicity;Jets","jtntrk",21,-0.5,20.5);
  result["Jet12NTrk"]=Histogram("Jet12NTrk",";jet track multiplicity;Jets","jtntrk","jtrank<2",21,-0.5,20.5);
  
  // CALOJET COMPOSITION
  result["CaloJetEMF"]=Histogram("CaloJetEMF",";jet EMF;Jets","jtemf",NFRAC,0,FRACMAX);
  result["CaloJet12EMF"]=Histogram("CaloJet12EMF",";jet EMF;Jets","jtemf","jtrank<2",NFRAC,0,FRACMAX);
  result["CaloJetCHF"]=Histogram("CaloJetCHF",";jet CHF;Jets","jtchf",NFRAC,0,1.01);
  result["CaloJet12CHF"]=Histogram("CaloJet12CHF",";jet CHF;Jets","jtchf","jtrank<2",NFRAC,0,FRACMAX);
  result["CaloJetFRBX"]=Histogram("CaloJetFRBX",";jet f_{RBX};Jets","jtfrbx",NFRAC,0,1.01);
  result["CaloJet12FRBX"]=Histogram("CaloJet12FRBX",";jet f_{RBX};Jets","jtfrbx","jtrank<2",NFRAC,0,FRACMAX);
  result["CaloJetFHPD"]=Histogram("CaloJetFHPD",";jet f_{HPD};Jets","jtfhpd",NFRAC,0,1.01);
  result["CaloJet12FHPD"]=Histogram("CaloJet12FHPD",";jet f_{HPD};Jets","jtfhpd","jtrank<2",NFRAC,0,FRACMAX);
  result["CaloJetN90Hits"]=Histogram("CaloJetN90Hits",";jet n90hits;Jets","jtn90hits",51,-0.5,50.5);
  result["CaloJet12N90Hits"]=Histogram("CaloJet12N90Hits",";jet n90hits;Jets","jtn90cells","jtrank<2",51,-0.5,50.5);

  // PFJET COMPOSITION
  result["PFJetCHF"]=Histogram("PFJetCHF",";charged hadron fraction CHF;Jets","jtchf",NFRAC,0,FRACMAX);
  result["PFJet12CHF"]=Histogram("PFJet12CHF",";charged hadron fraction CHF;Jets","jtchf","jtrank<2",NFRAC,0,FRACMAX);
  result["PFJetNHF"]=Histogram("PFJetNHF",";neutral hadron fraction NHF;Jets","jtnhf",NFRAC,0,FRACMAX);
  result["PFJet12NHF"]=Histogram("PFJet12NHF",";neutral hadron fraction NHF;Jets","jtnhf","jtrank<2",NFRAC,0,FRACMAX);
  result["PFJetNEF"]=Histogram("PFJetNEF",";photon fraction NEF;Jets","jtnef",NFRAC,0,FRACMAX);
  result["PFJet12NEF"]=Histogram("PFJet12NEF",";photon fraction NEF;Jets","jtnef","jtrank<2",NFRAC,0,FRACMAX);
  result["PFJetCEF"]=Histogram("PFJetCEF",";electron fraction CEF;Jets","jtcef",NFRAC,0,FRACMAX);
  result["PFJet12CEF"]=Histogram("PFJet12CEF",";electron fraction CEF;Jets","jtcef","jtrank<2",NFRAC,0,FRACMAX);
  result["PFJetMUF"]=Histogram("PFJetMUF",";muon fraction MUF","jtmuf;Jets",NFRAC,0,FRACMAX);
  result["PFJet12MUF"]=Histogram("PFJet12MUF",";muon fraction MUF","jtmuf;Jets","jtrank<2",NFRAC,0,FRACMAX);

  result["PFJetNCH"]=Histogram("PFJetNCH",";number of charged hadrons NCH;Jets","jtnch",21,-0.5,20.5);
  result["PFJet12NCH"]=Histogram("PFJet12NCH",";number of charged hadrons NCH;Jets","jtnch","jtrank<2",21,-0.5,20.5);
  result["PFJetNNH"]=Histogram("PFJetNNH",";number of neutral hadrons NNH;Jets","jtnnh",21,-0.5,20.5);
  result["PFJet12NNH"]=Histogram("PFJet12NNH",";number of neutral hadrons NNH;Jets","jtnnh","jtrank<2",21,-0.5,20.5);
  result["PFJetNNE"]=Histogram("PFJetNNE",";number of photons NNE;Jets","jtnne",21,-0.5,20.5);
  result["PFJet12NNE"]=Histogram("PFJet12NNE",";number of photons NNE;Jets","jtnne","jtrank<2",21,-0.5,20.5);
  result["PFJetNCE"]=Histogram("PFJetNCE",";number of electrons NCE;Jets","jtnce",5,-0.5,4.5);
  result["PFJet12NCE"]=Histogram("PFJet12NCE",";number of electrons NCE;Jets","jtnce","jtrank<2",5,-0.5,4.5);
  result["PFJetNMU"]=Histogram("PFJetNMU",";number of muons NMU;Jets","jtnmu",5,-0.5,4.5);
  result["PFJet12NMU"]=Histogram("PFJet12NMU",";number of muons NMU;Jets","jtnmu","jtrank<2",5,-0.5,4.5);

  result["PFJetCHPT"]=Histogram("PFJetCHPT",";PF jet charged hadron p_{T} [GeV];Jets","jtchf*jtpt",50,0.0,100.);
  result["PFJet12CHPT"]=Histogram("PFJet12CHPT",";PF jet charged hadron p_{T} [GeV];Jets","jtchf*jtpt","jtrank<2",50,0.,100.);
  result["PFJetNHPT"]=Histogram("PFJetNHPT",";PF jet neutral hadron p_{T} [GeV];Jets","jtnhf*jtpt",50,0.,100.);
  result["PFJet12NHPT"]=Histogram("PFJet12NHPT",";PF jet neutral hadron p_{T} [GeV];Jets","jtnhf*jtpt","jtrank<2",50,0.,100.);
  result["PFJetNEPT"]=Histogram("PFJetNEPT",";PF jet photon p_{T} [GeV];Jets","jtnef*jtpt",50,0.,100.);
  result["PFJet12NEPT"]=Histogram("PFJet12NEPT",";PF jet photon p_{T} [GeV];Jets","jtnef*jtpt","jtrank<2",50,0.,100.);
  result["PFJetCEPT"]=Histogram("PFJetCEPT",";PF jet electron p_{T} [GeV];Jets","jtcef*jtpt",50,0.,100.);
  result["PFJet12CEPT"]=Histogram("PFJet12CEPT",";PF jet electron p_{T} [GeV];Jets","jtcef*jtpt","jtrank<2",50,0.,100.);
  result["PFJetMUPT"]=Histogram("PFJetMUPT",";PF jet muon p_{T} [GeV];Jets","jtmuf*jtpt",50,0.,100.);
  result["PFJet12MUPT"]=Histogram("PFJet12MUPT",";PF jet muon p_{T} [GeV];Jets","jtmuf*jtpt","jtrank<2",50,0.,100.);

  
  // DIJETS
  result["DijetPtAve"]=Histogram("DijetPtAve",";dijet p^{ave}_{T} [GeV];Events","djptave",NJTPT,JTPTMIN,JTPTMAX);
  result["DijetDPhi"]=Histogram("DijetDPhi",";#Delta#varphi(j_{1},j_{2});Events","abs(deltaPhi(jtphi[0],jtphi[1]))",30,0,3.142);
  result["DijetDPhi13"]=Histogram("DijetDPhi13",";#Delta#varphi(j_{1},j_{3});Events","abs(deltaPhi(jtphi[0],jtphi[2]))",30,0,3.142);
  result["DijetDPhi23"]=Histogram("DijetDPhi23",";#Delta#varphi(j_{2},j_{3});Events","abs(deltaPhi(jtphi[1],jtphi[2]))",30,0,3.142);
  result["DijetDR13"]=Histogram("DijetDR13",";#Delta R(j_{1},j_{3});Events","deltaR(jteta[0],jtphi[0],jteta[2],jtphi[2])",50,0,10.);
  result["DijetDR23"]=Histogram("DijetDR23",";#Delta R(j_{2},j_{3});Events","deltaR(jteta[1],jtphi[1],jteta[2],jtphi[2])",50,0,10.);
  result["DijetA"]=Histogram("DijetA",";(p^{j2}_{T}-p^{j1}_{T})/(p^{j2}_{T}+p^{j1}_{T});Events","(evt%2==0)*(jtpt[1]-jtpt[0])/(jtpt[0]+jtpt[1])+(evt%2==1)*(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",21,-1.0,1.0);
  result["DijetM"]=Histogram("DijetM",";m_{jj} [GeV];Events","djm",100,0.,1000.);
  result["DijetM13"]=Histogram("DijetM13",";m(j_{1}j_{3}) [GeV];Events","mass(jtpt[0],jteta[0],jtphi[0],jte[0],jtpt[2],jteta[2],jtphi[2],jte[2])",100,0.,1000.);
  result["DijetM23"]=Histogram("DijetM23",";m(j_{2}j_{3}) [GeV];Events","mass(jtpt[1],jteta[1],jtphi[1],jte[1],jtpt[2],jteta[2],jtphi[2],jte[2])",100,0.,1000.);

   
  
  //
  // MET
  //
  result["MET"]=Histogram("MET",";#slash{E}_{T} [GeV];Events","met",NMET,METMIN,METMAX);
  result["METx"]=Histogram("METx",";#slash{E}_{X} [GeV];Events","metx",NMET+1,-0.25*METMAX,0.25*METMAX);
  result["METy"]=Histogram("METy",";#slash{E}_{Y} [GeV];Events","mety",NMET+1,-0.25*METMAX,0.25*METMAX);
  result["METphi"]=Histogram("METphi",";#varphi(#slash{E}_{T});Events","metphi",NMETPHI,-METPHIMAX,METPHIMAX);
  result["METphiNP"]=Histogram("METphiNP",";#varphi(#slash{E}_{T});Events","metphinopu",NMETPHI,-METPHIMAX,METPHIMAX);
  result["SumET"]=Histogram("SumET",";#Sigma E_{T} [GeV];Events","metsumet",100,0,1000);
  result["METovSumET"]=Histogram("METovSumET",";#slash{E}_{T}/#Sigma E_{T};Events","met/metsumet",NFRAC,0,FRACMAX);
  result["METovSqrtSumET"]=Histogram("METovSqrtSumET",";#slash{E}_{T} significance;Events","met/sqrt(metsumet)",50,0,25.);
  result["METsig"]=Histogram("METsig",";#slash{E}_{T} significance;Events","metsig",50,0.,25.);
  
  //
  // ELECTRONS
  //
  result["NEl"]=Histogram("NEl",";number of electrons;Events","nel",5,-0.5,4.5);
  result["ElE"]=Histogram("ElE",";E(electron) [GeV];electrons","ele",NLEPPT,LEPPTMIN,LEPPTMAX);
  result["ElPt"]=Histogram("ElPt",";p_{T}(electron) [GeV];electrons","elpt",NLEPPT,LEPPTMIN,LEPPTMAX);
  result["ElEta"]=Histogram("ElEta",";#eta(electron);leptons","eleta",NLEPETA+1,-LEPETAMAX,LEPETAMAX);
  result["ElPhi"]=Histogram("ElPhi",";#phi(electron);leptons","elphi",NLEPPHI+1,-LEPPHIMAX,LEPPHIMAX);
  result["ElIso"]=Histogram("ElIso",";electron isolation;electrons","eliso",36,0,0.35);
  result["ElIsoPUS"]=Histogram("ElIsoPUS",";electron isolation (after PU-subt.);electrons","max(0.0,eliso-rho*.503/elpt)",36,0,0.35);
  result["ElDPhiMET"]=Histogram("ElDPhiMET",";#Delta#varphi(electron,#slash{E}_{T});electrons","abs(deltaPhi(elphi,metphi))",NLEPPHI,0,LEPPHIMAX);
  result["ElDRJet15"]=Histogram("ElDRJet15",";#Delta R(electron,jet);electrons","MinIf$(deltaR(eleta,elphi,jteta,jtphi),jtpt>15.)","nel>0",50,1E-04,5.0);
  

  //
  // MUONS
  //
  result["NMu"]=Histogram("NMu",";number of muons;Events","nmu",5,-0.5,4.5);
  result["MuE"]=Histogram("MuE",";E(#mu) [GeV];muons","mue",NLEPPT,LEPPTMIN,LEPPTMAX);
  result["MuPt"]=Histogram("MuPt",";p_{T}(#mu) [GeV];muons","mupt",NLEPPT,LEPPTMIN,LEPPTMAX);
  result["MuEta"]=Histogram("MuEta",";#eta(#mu);muons","mueta",NLEPETA+1,-LEPETAMAX,LEPETAMAX);
  result["MuPhi"]=Histogram("MuPhi",";#phi(#mu);muons","muphi",NLEPPHI+1,-LEPPHIMAX,LEPPHIMAX);
  result["MuIso"]=Histogram("MuIso",";#mu isolation;muons","muiso",36,0,0.35);
  result["MuIsoPUS"]=Histogram("MuIsoPUS",";#mu isolation (after PU-subt.);muons","max(0.0,muiso-rho*.503/mupt)",36,0,0.35);
  result["MuAbsIso"]=Histogram("MuAbsIso",";#mu absolute isolation;muons","muiso*mupt",50,0,25.);
  result["MuAbsIsoPUS"]=Histogram("MuAbsIsoPUS",";#mu absolute isolation (after PU-subt.);muons","max(0.0,muiso*mupt-rho*0.503)",50,0,25.);
  result["MuD0"]=Histogram("MuD0",";#mu impact parameter d_{0} [cm];muons","mud0",50,0,0.2);
  result["MuD0Sig"]=Histogram("MuD0Sig",";#mu impact parameter significance;muons","mud0sig",50,0,3.);
  result["MuDPhiMET"]=Histogram("MuDPhiMET",";#Delta#varphi(#mu,#slash{E}_{T});muons","abs(deltaPhi(muphi,metphi))",NLEPPHI,0,LEPPHIMAX);
  result["MuDRJet15"]=Histogram("MuDRJet15",";#Delta R(#mu,jet);muons","MinIf$(deltaR(mueta,muphi,jteta,jtphi),jtpt>15.)","nmu>0",50,1E-04,5.0);
  
  
  //
  // W BOSONS
  //
  result["NW"]=Histogram("NW",";number of W bosons;Events","nlepw",3,-0.5,2.5);
  result["WMt"]=Histogram("WMt","; m_{T}(W) [GeV];Events","lepwmt",50,0.,150.);
  result["WM"]=Histogram("WM","; m(W) [GeV];Events","lepwm",50,0.,150.);
  result["WPt"]=Histogram("WPt",";p_{T}(W) [GeV];Events","lepwpt",NLEPPT,LEPPTMIN,LEPPTMAX);
  result["WEta"]=Histogram("WEta",";#eta(W);Events","lepweta",51,-3.0,3.0);
  result["WPhi"]=Histogram("WPhi",";#varphi(W);Events","lepwphi",NLEPPHI+1,-LEPPHIMAX,LEPPHIMAX);
  
  
  //
  // Z BOSONS
  //
  result["NZ"]=Histogram("NZ",";number of Z bosons;Events","nlepz",3,-0.5,2.5);
  result["ZM"]=Histogram("ZM","; m(Z) [GeV];Events","lepzm",50,60.,120.);
  result["ZPt"]=Histogram("ZPt",";p_{T}(Z) [GeV];Events","lepzpt",NLEPPT,LEPPTMIN,LEPPTMAX);
  result["ZEta"]=Histogram("ZEta",";#eta(Z);Events","lepzeta",NLEPETA+1,-LEPETAMAX,LEPETAMAX);
  result["ZPhi"]=Histogram("ZPhi",";#varphi(Z);Events","lepzphi",NLEPPHI+1,-LEPPHIMAX,LEPPHIMAX);
  result["ZDPhi"]=Histogram("ZDPhi",";#Delta#varphi(l,l);Events","abs(lepzdphi)",40,0.,3.142);
  result["ZDR"]=Histogram("ZDR",";#DeltaR(l,l);Events","lepzdr",40,0.,6.);
  
  
  //
  // Zbb
  //
  result["ZbbNjt10"]=Histogram("ZbbNjt10",";N_{jet} (p_{T}>10 GeV);Events","Sum$(jtpt>10.&&abs(jteta)<2.1)",9,-0.5,8.5);
  result["ZbbNjt15"]=Histogram("ZbbNjt15",";N_{jet} (p_{T}>15 GeV);Events","Sum$(jtpt>15.&&abs(jteta)<2.1)",9,-0.5,8.5);
  result["ZbbNjt20"]=Histogram("ZbbNjt20",";N_{jet} (p_{T}>20 GeV);Events","Sum$(jtpt>20.&&abs(jteta)<2.1)",9,-0.5,8.5);
  result["ZbbNjt25"]=Histogram("ZbbNjt25",";N_{jet} (p_{T}>25 GeV);Events","Sum$(jtpt>25.&&abs(jteta)<2.1)",9,-0.5,8.5);

  result["ZbbPt"]=Histogram("ZbbPt",";p_{T}(Z#rightarrow b#bar{b}) [GeV];Events","zbbpt",50,0.0,200.);
  result["ZbbEta"]=Histogram("ZbbEta",";#eta(Z#rightarrow b#bar{b});Events","zbbeta",51,-3.,3.);
  result["ZbbPhi"]=Histogram("ZbbPhi",";#varphi(Z#rightarrow b#bar{b});Events","zbbphi",51,-3.142,3.142);
  result["ZbbM"]=Histogram("ZbbM",";m(Z#rightarrow b#bar{b}) [GeV];Events","zbbm",100,0.,200.);
  result["ZbbDR"]=Histogram("ZbbDR",";#Delta R(b,#bar{b});Events","zbbdr",100,0.,6.);
  result["ZbbDPhi"]=Histogram("ZbbDPhi",";#Delta#varphi(b,#bar{b});Events","zbbdphi",100,0.,3.142);
  result["ZbbDPhiJet3"]=Histogram("ZbbDPhiJet3",";#Delta#varphi(Z,jet);Events","zbbdphijt","zbbidx[0][2]!=-1",100,0.,3.142);
  
  result["ZbbBJetRank"]=Histogram("ZbbBJetRank",";rank;b-jets","jtrank+1","(jtrank==zbbidx[0][0]||jtrank==zbbidx[0][1])",4,0.5,4.5);
  result["ZbbBJetPt"]=Histogram("ZbbBJetPt",";p_{T}(b) [GeV];b-jets","jtpt","(jtrank==zbbidx[0][0]||jtrank==zbbidx[0][1])",100,0.0,200.);
  result["ZbbBJetEta"]=Histogram("ZbbBJetEta",";#eta(b);b-jets","jteta","(jtrank==zbbidx[0][0]||jtrank==zbbidx[0][1])",41,-3.0,3.0);
  result["ZbbBJetPhi"]=Histogram("ZbbBJetPhi",";#varphi(b);b-jets","jtphi","(jtrank==zbbidx[0][0]||jtrank==zbbidx[0][1])",41,-3.142,3.142);

  result["ZbbBMuJetRank"]=Histogram("ZbbBMuJetRank",";#mu-tagged jet rank;Events","jtrank+1","jtrank==zbbidx[0][0]",4,0.5,4.5);
  result["ZbbBMuJetPt"]=Histogram("ZbbBMuJetPt",";p_{T}(#mu-tagged jet) [GeV];Events","jtpt","jtrank==zbbidx[0][0]",100,0.0,200.);
  result["ZbbBMuJetEta"]=Histogram("ZbbBMuJetEta",";#eta(#mu-tagged jet);Events","jteta","jtrank==zbbidx[0][0]",41,-3.0,3.0);
  result["ZbbBMuJetPhi"]=Histogram("ZbbBMuJetPhi",";#varphi(#mu-tagged jet);Events","jtphi","jtrank==zbbidx[0][0]",41,-3.142,3.142);

  result["ZbbBLtJetRank"]=Histogram("ZbbBLtJetRank",";lifetime-tagged jet rank;Events","jtrank+1","jtrank==zbbidx[0][1]",4,0.5,4.5);
  result["ZbbBLtJetPt"]=Histogram("ZbbBLtJetPt",";p_{T}(lifetime-tagged jet) [GeV];Events","jtpt","jtrank==zbbidx[0][1]",100,0.0,200.);
  result["ZbbBLtJetEta"]=Histogram("ZbbBLtJetEta",";#eta(lifetime-tagged jet);Events","jteta","jtrank==zbbidx[0][1]",41,-3.0,3.0);
  result["ZbbBLtJetPhi"]=Histogram("ZbbBLtJetPhi",";#varphi(lifetime-tagged jet);Events","jtphi","jtrank==zbbidx[0][1]",41,-3.142,3.142);
  result["ZbbBLtJetBTag"]=Histogram("ZbbBLtJetBTag",";btag-discriminator (lifetime-tagged jet);Events","jtesvt","jtrank==zbbidx[0][1]",100,0.,8.);

  result["ZbbJet3Rank"]=Histogram("ZbbJet3Rank",";extra jet rank;Events","jtrank+1","jtrank==zbbidx[0][2]",3,0.5,3.5);
  result["ZbbJet3Pt"]=Histogram("ZbbJet3Pt",";p_{T}(extra-jet) [GeV];Events","jtpt","jtrank==zbbidx[0][2]",100,0.0,200.);
  result["ZbbJet3RelPt"]=Histogram("ZbbJet3RelPt",";p_{T}(extra-jet)/p_{T}^{ave};Events","jtpt*2./(jtpt[0]+jtpt[1])","jtrank==zbbidx[0][2]",100,0.0,1.0);
  result["ZbbJet3Eta"]=Histogram("ZbbJet3Eta",";#eta(extra-jet);Events","jteta","jtrank==zbbidx[0][2]",41,-3.0,3.0);
  result["ZbbJet3Phi"]=Histogram("ZbbJet3Phi",";#varphi(extra-jet);Events","jtphi","jtrank==zbbidx[0][2]",41,-3.142,3.142);

  result["ZbbMuPtRel"]=Histogram("ZbbMuPtRel",";p_{T}^{rel}(#mu)","jtmuptrel","jtrank==zbbidx[0][0]",50,0.,4.);
  result["ZbbMuRelPt"]=Histogram("ZbbMuRelPt",";p_{T}(#mu)/p_{T}(jet)","jtmupt/jtpt","jtrank==zbbidx[0][0]",50,0.0,1.);
  result["ZbbMuPt"]=Histogram("ZbbMuPt",";p_{T}(#mu) [GeV]","jtmupt","jtrank==zbbidx[0][0]",50,0.0,100.);
  result["ZbbMuEta"]=Histogram("ZbbMuEta",";#eta(#mu)","jtmueta","jtrank==zbbidx[0][0]",41,-3.0,3.0);
  result["ZbbMuPhi"]=Histogram("ZbbMuPhi",";#varphi(#mu)","jtmuphi","jtrank==zbbidx[0][0]",41,-3.142,3.142);
  


  //
  // SUBJET/FILTER
  //

  result["FltPt"]=Histogram("FltPt",";filter jet p_{T} [GeV];Jets","fltpt[0][]",40,0.,200.);
  result["FltCorr"]=Histogram("FltCorr",";filter jet corrfactor;Jets","  fltcorr[0][]",40,0.,4.);
  result["FltEta"]=Histogram("FltEta",";filter jet #eta;Jets","  flteta[0][]",60,-3.0,3.0); 
  result["FltPhi"]=Histogram("FltPhi",";filter jet #phi;Jets","  fltphi[0][]", 80,-4.0,4.0); 
  result["FltCsvt"]=Histogram("FltCsvt",";filter jet CSVT;Jets","  fltcsvt[0][]", 40,0.,4.);
  result["FltCsvivf"]=Histogram("FltCsvivf",";filter jet CSVIVF;Jets","fltcsvivf[0][]", 40,0.,4.);
  result["FltCmva"]=Histogram("FltCmva",";filter jet CMVA;Jets","  fltcmva[0][]", 40,0.,4.);
  result["FltVtx3dL"]=Histogram("FltVtx3dL",";filter jet Vtx3dL;Jets","fltvtx3dL[0][]", 40,0.,4.);
  result["FltVtx3deL"]=Histogram("FltVtx3deL",";filter jet Vtx3deL;Jets","  fltvtx3deL[0][]", 40,0.,4.);
  result["FltChf"]=Histogram("FltChf",";filter jet Chf;Jets","fltchf[0][]", 40,0.,4.);

  result["Flt1Pt"]=Histogram("FltPt",";filter jet p_{T} [GeV];Jets","fltpt[0][0]",40,0.,200.);
  result["Flt1Corr"]=Histogram("FltCorr",";filter jet corrfactor;Jets","  fltcorr[0][0]",40,0.,4.);
  result["Flt1Eta"]=Histogram("FltEta",";filter jet #eta;Jets","  flteta[0][0]",60,-3.0,3.0); 
  result["Flt1Phi"]=Histogram("FltPhi",";filter jet #phi;Jets","  fltphi[0][0]", 80,-4.0,4.0); 
  result["Flt1Csvt"]=Histogram("FltCsvt",";filter jet CSVT;Jets","  fltcsvt[0][0]", 40,0.,4.);
  result["Flt1Csvivf"]=Histogram("FltCsvivf",";filter jet CSVIVF;Jets","fltcsvivf[0][0]", 40,0.,4.);
  result["Flt1Cmva"]=Histogram("FltCmva",";filter jet CMVA;Jets","  fltcmva[0][0]", 40,0.,4.);
  result["Flt1Vtx3dL"]=Histogram("FltVtx3dL",";filter jet Vtx3dL;Jets","fltvtx3dL[0][0]", 40,0.,4.);
  result["Flt1Vtx3deL"]=Histogram("FltVtx3deL",";filter jet Vtx3deL;Jets","  fltvtx3deL[0][0]", 40,0.,4.);
  result["Flt1Chf"]=Histogram("FltChf",";filter jet Chf;Jets","fltchf[0][0]", 40,0.,4.);

  result["Flt2Pt"]=Histogram("FltPt",";filter jet p_{T} [GeV];Jets","fltpt[0][1]",40,0.,200.);
  result["Flt2Corr"]=Histogram("FltCorr",";filter jet corrfactor;Jets","  fltcorr[0][1]",40,0.,4.);
  result["Flt2Eta"]=Histogram("FltEta",";filter jet #eta;Jets","  flteta[0][1]",60,-3.0,3.0); 
  result["Flt2Phi"]=Histogram("FltPhi",";filter jet #phi;Jets","  fltphi[0][1]", 80,-4.0,4.0); 
  result["Flt2Csvt"]=Histogram("FltCsvt",";filter jet CSVT;Jets","  fltcsvt[0][1]", 40,0.,4.);
  result["Flt2Csvivf"]=Histogram("FltCsvivf",";filter jet CSVIVF;Jets","fltcsvivf[0][1]", 40,0.,4.);
  result["Flt2Cmva"]=Histogram("FltCmva",";filter jet CMVA;Jets","  fltcmva[0][1]", 40,0.,4.);
  result["Flt2Vtx3dL"]=Histogram("FltVtx3dL",";filter jet Vtx3dL;Jets","fltvtx3dL[0][1]", 40,0.,4.);
  result["Flt2Vtx3deL"]=Histogram("FltVtx3deL",";filter jet Vtx3deL;Jets","  fltvtx3deL[0][1]", 40,0.,4.);
  result["Flt2Chf"]=Histogram("FltChf",";filter jet Chf;Jets","fltchf[0][1]", 40,0.,4.);

  result["Flt3Pt"]=Histogram("FltPt",";filter jet p_{T} [GeV];Jets","fltpt[0][2]",40,0.,200.);
  result["Flt3Corr"]=Histogram("FltCorr",";filter jet corrfactor;Jets","  fltcorr[0][2]",40,0.,4.);
  result["Flt3Eta"]=Histogram("FltEta",";filter jet #eta;Jets","  flteta[0][2]",60,-3.0,3.0); 
  result["Flt3Phi"]=Histogram("FltPhi",";filter jet #phi;Jets","  fltphi[0][2]", 80,-4.0,4.0); 
  result["Flt3Csvt"]=Histogram("FltCsvt",";filter jet CSVT;Jets","  fltcsvt[0][2]", 40,0.,4.);
  result["Flt3Csvivf"]=Histogram("FltCsvivf",";filter jet CSVIVF;Jets","fltcsvivf[0][2]", 40,0.,4.);
  result["Flt3Cmva"]=Histogram("FltCmva",";filter jet CMVA;Jets","  fltcmva[0][2]", 40,0.,4.);
  result["Flt3Vtx3dL"]=Histogram("FltVtx3dL",";filter jet Vtx3dL;Jets","fltvtx3dL[0][2]", 40,0.,4.);
  result["Flt3Vtx3deL"]=Histogram("FltVtx3deL",";filter jet Vtx3deL;Jets","  fltvtx3deL[0][2]", 40,0.,4.);
  result["Flt3Chf"]=Histogram("FltChf",";filter jet Chf;Jets","fltchf[0][2]", 40,0.,4.);

  result["FatPt"]=Histogram("FatPt",";fat jet p_{T} [GeV];Events","fatpt[0]",50,0.,500.);
  result["SubPt"]=Histogram("SubPt",";sub jet p_{T} [GeV];Jets","subpt[0][]",40,0.,200.);
  result["Sub1Pt"]=Histogram("Sub1Pt",";1. sub jet p_{T} [GeV];Jets","subpt[0][0]",40,0.,200.);
  result["Sub2Pt"]=Histogram("Sub2Pt",";2. sub jet p_{T} [GeV];Jets","subpt[0][1]",40,0.,200.);
  // result["Flt1Pt"]=Histogram("Flt1Pt",";1. flt jet p_{T} [GeV];Jets","fltpt[0][0]",40,0.,200.);
  // result["Flt2Pt"]=Histogram("Flt2Pt",";2. flt jet p_{T} [GeV];Jets","fltpt[0][1]",40,0.,200.);
  //result["Flt3Pt"]=Histogram("Flt3Pt",";3. flt jet p_{T} [GeV];Jets","fltpt[0][2]",40,0.,200.);
  result["FatEta"]=Histogram("FatEta",";fat jet #eta;Jets","fateta[0]",21,-2.5,2.5);
  result["SubEta"]=Histogram("SubEta",";sub jet #eta;Jets","subeta[0][]",21,-2.5,2.5);
  result["FltEta"]=Histogram("FltEta",";filter jet #eta;Jets","flteta[0][]",21,-2.5,2.5);
  result["FatPhi"]=Histogram("FatPhi",";fat jet #varphi;Jets","fatphi[0]",NJTPHI+1,-JTPHIMAX,JTPHIMAX);
  result["SubPhi"]=Histogram("SubPhi",";sub jet #varphi;Jets","subphi[0][]",NJTPHI+1,-JTPHIMAX,JTPHIMAX);
  result["FltPhi"]=Histogram("FltPhi",";filter jet #varphi;Jets","fltphi[0][]",NJTPHI+1,-JTPHIMAX,JTPHIMAX);
  result["SubCSVT1"]=Histogram("SubCSVT1",";max(CSV1,CSV2);Jets","max(subcsvt[0][0],subcsvt[0][1])","",NFRAC,0.,FRACMAX);
  result["SubCSVT2"]=Histogram("SubCSVT2",";min(CSV1,CSV2);Jets","min(subcsvt[0][0],subcsvt[0][1])","",NFRAC,0.,FRACMAX);
  result["FltCSVT1"]=Histogram("FltCSVT1",";max(CSV1,CSV2);Jets","max(fltcsvt[0][0],fltcsvt[0][1])","",NFRAC,0.,FRACMAX);
  result["FltCSVT2"]=Histogram("FltCSVT2",";min(CSV1,CSV2);Jets","min(fltcsvt[0][0],fltcsvt[0][1])","",NFRAC,0.,FRACMAX);
  result["FltCSVTCorr1"]=Histogram("FltCSVTCorr1",";corr. max(CSV1,CSV2);Jets","max(fltcsvtcorr[0][0],fltcsvtcorr[1][0])","",NFRAC,0.,FRACMAX);
  result["FltCSVTCorr2"]=Histogram("FltCSVTCorr2",";corr. min(CSV1,CSV2);Jets","min(fltcsvtcorr[0][0],fltcsvtcorr[1][0])","",NFRAC,0.,FRACMAX);
  result["FltCSVTnn1"]=Histogram("FltCSVTnn1",";corr. CSV1;Jets","fltcsvtcorr[0][0]","",NFRAC,0.,FRACMAX);
  result["FltCSVTnn2"]=Histogram("FltCSVTnn2",";corr. CSV2;Jets","fltcsvtcorr[0][1]","",NFRAC,0.,FRACMAX);
  result["FatJet1DR"]=Histogram("FatJet1DR",";#Delta R(jet^{fat},jet^{1.});Events","deltaR(jteta[0],jtphi[0],fateta,fatphi)",25,0.,10.);
  result["FatJetDREle"]=Histogram("FatJetDREle",";#Delta R(jet^{fat},electron);Events","deltaR(eleta[0],elphi[0],fateta,fatphi)",25,0.,10.);
  result["DiSubDREle"]=Histogram("DiSubDREle",";#Delta R(di-subjet system,electron);Events","deltaR(eleta[0],elphi[0],disubeta[0],disubphi[0])",25,0.,10.);

  result["DiSubMinDREle"]=Histogram("DiSubMinDREle",";min(#Delta R(subJets,electron));Events","min(deltaR(eleta[0],elphi[0],subeta[0][0],subphi[0][0]),deltaR(eleta[0],elphi[0],subeta[0][1],subphi[0][1]))",25,0.,10.);
  result["SubJet1DREle"]=Histogram("SubJet1DREle",";min(#Delta R(1. subjet,electron));Events","deltaR(eleta[0],elphi[0],subeta[0][0],subphi[0][0])",25,0.,10.);
  result["SubJet2DREle"]=Histogram("SubJet2DREle",";min(#Delta R(2. subjet,electron));Events","deltaR(eleta[0],elphi[0],subeta[0][1],subphi[0][1])",25,0.,10.);
  result["TriFltDREle"]=Histogram("TriFltDREle",";#Delta R(tri-filterjet system,electron);Events","deltaR(eleta[0],elphi[0],triflteta[0],trifltphi[0])",25,0.,10.);
  result["SubLTBTag"]=Histogram("SubLTBTag",";sub jet b-tagging discriminant;Jets","subltbtag[0][]",NFRAC,0,FRACMAX);
  result["FltLTBTag"]=Histogram("FltLTBTag",";filter jet b-tagging discriminant;Jets","fltltbtag[0][]",NFRAC,0,FRACMAX);
  result["SubNLTBTag02"]=Histogram("SubNLTBTag02",";lt-tagged sub jets (dsc>0.2);Events","Sum$(subltbtag[0][]>0.2)",3,-0.5,2.5);
  result["FltNLTBTag02"]=Histogram("FltNLTBTag02",";lt-tagged filter jets (dsc>0.2);Events","Sum$(fltltbtag[0][]>0.2)",4,-0.5,3.5);
  result["SubNLTBTag04"]=Histogram("SubNLTBTag04",";lt-tagged sub jets (dsc>0.4);Events","Sum$(subltbtag[0][]>0.4)",3,-0.5,2.5);
  result["FltNLTBTag04"]=Histogram("FltNLTBTag04",";lt-tagged filter jets (dsc>0.4);Events","Sum$(fltltbtag[0][]>0.4)",4,-0.5,3.5);
  result["SubNLTBTag05"]=Histogram("SubNLTBTag05",";lt-tagged sub jets (dsc>0.5);Events","Sum$(subltbtag[0][]>0.5)",3,-0.5,2.5);
  result["FltNLTBTag05"]=Histogram("FltNLTBTag05",";lt-tagged filter jets (dsc>0.5);Events","Sum$(fltltbtag[0][]>0.5)",4,-0.5,3.5);
  result["SubNLTBTag07"]=Histogram("SubNLTBTag07",";lt-tagged sub jets (dsc>0.7);Events","Sum$(subltbtag[0][]>0.7)",3,-0.5,2.5);
  result["FltNLTBTag07"]=Histogram("FltNLTBTag07",";lt-tagged filter jets (dcs>0.7);Events","Sum$(fltltbtag[0][]>0.7)",4,-0.5,3.5);
  result["SubMuTag"]=Histogram("SubMuTag",";sub jet #mu-tagging discriminant;Jets","subslbtag[0][]",NFRAC,0,FRACMAX);
  result["FltMuTag"]=Histogram("FltMuTag",";filter jet #mu-tagging discriminant;Jets","fltslbtag[0][]",NFRAC,0,FRACMAX);
  result["SubNMuTag"]=Histogram("SubNMuTag",";#mu-tagged sub jets;Events","Sum$(submubtag[0][]>0.0)",3,-0.5,2.5);
  result["FltNMuTag"]=Histogram("FltNMuTag",";#mu-tagged filter jets;Events","Sum$(fltmubtag[0][]>0.0)",4,-0.5,3.5);
  result["SubNBTag"]=Histogram("SubNBTag",";b-tagged sub jets;Events","Sum$(subltbtag[0][]>0.7||submubtag[0][]>0.0)",3,-0.5,2.5);
  result["FltNBTag"]=Histogram("FltNBTag",";b-tagged filter jets;Events","Sum$(fltltbtag[0][]>0.7||fltmubtag[0][]>0.0)",4,-0.5,3.5);
  result["DiSubPt"]=Histogram("DiSubPt",";p_{T,jj}^{sub} [GeV];Events","disubpt[0]",50,0.,500.);
  result["TriFltPt"]=Histogram("TriFltPt",";p_{T,jj}^{filter} [GeV];Events","trifltpt[0]",50,0.,500.);
  result["TriFltPtAlt"]=Histogram("TriFltPt",";p_{T,jj}^{filter} [GeV];Events","Alt$(trifltpt[0]*(nflt>1),0)",50,0.,500.);
  result["DiSubDR"]=Histogram("DiSubDR",";#Delta R(j^{sub}_{1},j^{sub}_{2});Events","disubdr[0]",25,0.,2.);
  result["DiSubDEta"]=Histogram("DiSubDEta",";#Delta#eta(j^{sub}_{1},j^{sub}_{2});Events","abs(subeta[0][0]-subeta[0][1])",10,0.,5.);
  result["TriFltDR12"]=Histogram("TriFltDR12",";#Delta R(j^{flt}_{1},j^{flt}_{2});Events","deltaR(flteta[0][0],fltphi[0][0],flteta[0][1],fltphi[0][1])",25,0.,2.);
  result["TriFltDEta12"]=Histogram("TriFltDEta12",";#Delta#eta(j^{flt}_{1},j^{flt}_{2});Events","abs(flteta[0][0]-flteta[0][1])",10,0.,5.);
  //  result["TriFltDR13"]=Histogram("TriFltDR13",";#Delta R(j^{flt}_{1},j^{flt}_{3});Events","deltaR(flteta[0][0],fltphi[0][0],flteta[0][2],fltphi[0][2])",25,0.,2.);
  result["TriFltDR13"]=Histogram("TriFltDR13",";#Delta R(j^{flt}_{1},j^{flt}_{3});Events","deltaR(flteta[0][0],fltphi[0][0],flteta[0][2],fltphi[0][2])","nflt==3",25,0.,2.);
  //  result["TriFltDEta13"]=Histogram("TriFltDEta13",";#Delta#eta(j^{flt}_{1},j^{flt}_{3});Events","abs(flteta[0][0]-flteta[0][2])",10,0.,5.);
  result["TriFltDEta13"]=Histogram("TriFltDEta13",";#Delta#eta(j^{flt}_{1},j^{flt}_{3});Events","abs(flteta[0][0]-flteta[0][2])","nflt==3",10,0.,5.);
  result["FatDPhiW"]=Histogram("FatDPhiW",";#Delta#varphi(H_{fat},W);Events","abs(fatdphilepw[0])",NJTPHI,0.,JTPHIMAX);
  result["DiSubDPhiW"]=Histogram("DiSubDPhiW",";#Delta#varphi(H_{sub},W);Events","abs(disubdphilepw[0])",NJTPHI,0.,JTPHIMAX);
  result["TriFltDPhiW"]=Histogram("TriFltDPhiW",";#Delta#varphi(H_{filter},W);Events","abs(trifltdphilepw[0])",NJTPHI,0.,JTPHIMAX);
  result["FatDEtaW"]=Histogram("FatDEtaW",";#Delta#eta(H_{fat},W);Events","abs(lepweta[0]-fateta[0])",10,0.,5.);
  result["DiSubDEtaW"]=Histogram("DiSubDEtaW",";#Delta#eta(H_{sub},W);Events","abs(lepweta[0]-disubeta[0])",10,0.,5.);
  result["TriFltDEtaW"]=Histogram("TriFltDEtaW",";#Delta#eta(H_{filter},W);Events","abs(lepweta[0]-triflteta[0])",10,0.,5.);
  result["FatDPhiZ"]=Histogram("FatDPhiZ",";#Delta#varphi(H_{fat},Z);Events","abs(fatdphilepz[0])",NJTPHI,0.,JTPHIMAX);
  result["DiSubDPhiZ"]=Histogram("DiSubDPhiZ",";#Delta#varphi(H_{sub},Z);Events","abs(disubdphilepz[0])",NJTPHI,0.,JTPHIMAX);
  result["TriFltDPhiZ"]=Histogram("TriFltDPhiZ",";#Delta#varphi(H_{filter},Z);Events","abs(trifltdphilepz[0])",NJTPHI,0.,JTPHIMAX);
  result["FatDPhiMET"]=Histogram("FatDPhiMET",";#Delta#varphi(H_{fat},#slash{E}_{T});Events","abs(fatdphimet[0])",NJTPHI,0.,JTPHIMAX);
  result["DiSubDPhiMET"]=Histogram("DiSubDPhiMET",";#Delta#varphi(H_{sub},#slash{E}_{T});Events","abs(disubdphimet[0])",NJTPHI,0.,JTPHIMAX);
  result["TriFltDPhiMET"]=Histogram("TriFltDPhiMET",";#Delta#varphi(H_{filter},#slash{E}_{T});Events","abs(trifltdphimet[0])",NJTPHI,0.,JTPHIMAX);
  result["FatM"]=Histogram("FatM",";m_{jj}^{fat} [GeV];Events","fatm[0]",25,0.,200.);
  result["DiSubM"]=Histogram("DiSubM",";m_{jj}^{sub} [GeV];Events","disubm[0]",25,0.,200.);
  result["TriFltM"]=Histogram("TriFltM",";m_{jj}^{filter} [GeV];Events","trifltm[0]",25,0.,200.);
  result["TriFltMAlt"]=Histogram("TriFltM",";m_{jj}^{filter} [GeV];Events","Alt$(trifltm[0]*(nflt>1),0)",25,0.,200.);
  result["DiSubTriFltDM"]=Histogram("DiSubTriFltDM",";|m_{jj}^{sub}-m_{jj}^{filter}| [GeV];Events","abs(disubm[0]-trifltm[0])",20,0.,100.);
  result["DiSubTriFltDMSign"]=Histogram("DiSubTriFltDMSign",";m_{jj}^{sub}-m_{jj}^{filter} [GeV];Events","disubm[0]-trifltm[0]",20,0.,100.);
  result["NExtraJet20"]=Histogram("NExtraJet20",";number of extra jets w/ p_{T}>20 GeV;Events","Sum$(jtdrsub>disubdr[0]&&jtpt>20.)",5,-0.5,4.5);
  result["NExtraJet30"]=Histogram("NExtraJet30",";number of extra jets w/ p_{T}>30 GeV;Events","Sum$(jtdrsub>disubdr[0]&&jtpt>30.)",5,-0.5,4.5);
  result["NExtraJet40"]=Histogram("NExtraJet40",";number of extra jets w/ p_{T}>40 GeV;Events","Sum$(jtdrsub>disubdr[0]&&jtpt>40.)",5,-0.5,4.5);
  result["NExtraJet50"]=Histogram("NExtraJet50",";number of extra jets w/ p_{T}>50 GeV;Events","Sum$(jtdrsub>disubdr[0]&&jtpt>50.)",5,-0.5,4.5);
  result["ExtraJetPt"]=Histogram("ExtraJetPt",";extra jet p_{T} [GeV];Extra Jets","MaxIf$(jtpt,jtdrsub>disubdr[0])",40,0.,150.);
  result["ExtraJetEta"]=Histogram("ExtraJetEta",";extra jet #eta (p_{T}>20GeV);Extra Jets","jteta","(jtdrsub>disubdr[0]&&jtpt>20.)",31,-5.,5.);
  result["ExtraJetDR"]=Histogram("ExtraJetDR",";#Delta R(extra jet,sub jet);Extra Jets","jtdrsub","(jtdrsub>disubdr[0]&&jtpt>20.)",40,0.,5.);
  result["SJFExtraJetN"]=Histogram("SJFExtraJetN",";N(extra jets);Events","Sum$(jtpt>20.&&abs(jteta)<2.5&&jtdrsub>disubdr[0]&&jtdrsub<9.9)",7,-0.5,6.5);
  result["NaddFlt"]=Histogram("NaddFlt",";N(extra jets);Events","naddflt",7,-0.5,6.5);
  result["DiffM"]=Histogram("DiffM",";|m_{jj} - m_{jj}^{filter}| [GeV];Events","Alt$(abs((hbbm-trifltm[0]))*(nfltjt>1),0)",25,0.,200.);

  //
  // HBB (STANDARD JET RECONSTRUCTION)
  //
  result["HbbM"] =Histogram("HbbM", ";m_{H_{rec}} [GeV];Events","hbbm",25,0.,200.);
  result["HbbM2"] =Histogram("HbbM", ";m_{H_{rec}} [GeV];Events","hbbm",25,0.,200.);
  result["HbbPt"]=Histogram("HbbPt",";p_{T}(H_{rec}) [GeV];Events","hbbpt",60,0.,400.);
  result["HbbDR"]=Histogram("HbbDR",";#Delta R(j,j);Events","hbbdr",45,0.,6.);
  result["HbbEta"]=Histogram("HbbEta",";#eta(H_{rec});Events","hbbeta",25,-6.,6.);
  result["HbbDEta"]=Histogram("HbbDEta",";#Delta #eta(j,j);Events","abs(jteta[hbbjtidx[0]]-jteta[hbbjtidx[1]])",45,0.,6.);
  result["HbbDEtaW"]=Histogram("HbbDEtaW",";#Delta #eta(H,W);Events","abs(hbbeta-lepweta[0])",10,0.,5.);
  result["HbbDPhiZ"]=Histogram("HbbDPhiZ",";#Delta#varphi(H,Z);Events","abs(deltaPhi(hbbphi,lepzphi[0]))",30,0.,3.15);
  result["HbbDPhiMET"]=Histogram("HbbDPhiMET",";#Delta#varphi(H,#slash{E}_{T});Events","abs(deltaPhi(hbbphi,metphi))",30,0.,3.15);
  result["HbbDPhiW"]=Histogram("HbbDPhiW",";#Delta#varphi(H,W);Events","abs(deltaPhi(hbbphi,lepwphi[0]))",30,0.,3.15);
  result["HbbESVT"]=Histogram("HbbESVT",";jet btag-discriminant (ESVT);Jets","jtesvt","(jtrank==hbbjtidx[0]||jtrank==hbbjtidx[1])",NFRAC,0.0,6.0);
  result["HbbPSVT"]=Histogram("HbbPSVT",";jet btag-discriminant (PSVT);Jets","jtpsvt","(jtrank==hbbjtidx[0]||jtrank==hbbjtidx[1])",NFRAC,0.0,6.0);
  result["HbbCSVT"]=Histogram("HbbCSVT",";jet btag-discriminant (CSVT);Jets","jtcsvt","(jtrank==hbbjtidx[0]||jtrank==hbbjtidx[1])",NFRAC,0.,FRACMAX);
  result["HbbCSVT"]=Histogram("HbbCSVT",";jet btag-discriminant (CSVT);Jets","jtcsvt","(jtrank==hbbjtidx[0]||jtrank==hbbjtidx[1])",NFRAC,0.,FRACMAX);
  result["HbbCSVTCorr"]=Histogram("HbbCSVTCorr",";corr. jet btag-discriminant (CSVT);Jets","jtcsvtcorr[][0]","(jtrank==hbbjtidx[0]||jtrank==hbbjtidx[1])",NFRAC,0.,FRACMAX);
  result["HbbCSVT1"]=Histogram("HbbCSVT1",";max(CSV1,CSV2);Jets","max(jtcsvt[hbbjtidx[0]],jtcsvt[hbbjtidx[1]])","",NFRAC,0.,FRACMAX);
  result["HbbCSVT2"]=Histogram("HbbCSVT2",";min(CSV1,CSV2);Jets","min(jtcsvt[hbbjtidx[0]],jtcsvt[hbbjtidx[1]])","",NFRAC,0.,FRACMAX);
  result["HbbCSVT11"]=Histogram("HbbCSVT11",";CSV1_{H};Jets","jtcsvt[hbbjtidx[0]]","",NFRAC,0.,FRACMAX);
  result["HbbCSVT22"]=Histogram("HbbCSVT22",";CSV2_{H};Jets","jtcsvt[hbbjtidx[1]]","",NFRAC,0.,FRACMAX);
  result["HbbCSVTCorr1"]=Histogram("HbbCSVTCorr1",";corr. max(CSV1_{H},CSV2_{H});Jets","max(jtcsvtcorr[hbbjtidx[0]][0],jtcsvtcorr[hbbjtidx[1]][0])","",NFRAC,0.,FRACMAX);
  result["HbbCSVTCorr2"]=Histogram("HbbCSVTCorr2",";corr. min(CSV1_{H},CSV2_{H});Jets","min(jtcsvtcorr[hbbjtidx[0]][0],jtcsvtcorr[hbbjtidx[1]][0])","",NFRAC,0.,FRACMAX);
  result["HbbCSVTCorr11"]=Histogram("HbbCSVTCorr11",";CSV1_{H};Jets","jtcsvtcorr[hbbjtidx[0]][0]","",NFRAC,0.,FRACMAX);
  result["HbbCSVTCorr22"]=Histogram("HbbCSVTCorr22",";CSV2_{H};Jets","jtcsvtcorr[hbbjtidx[1]][0]","",NFRAC,0.,FRACMAX);
  result["TopCSVTCorr"]=Histogram("TopCSVTCorr",";corr. CSV_{t};Jets","jtcsvtcorr[topjtidx][0]","",NFRAC,0.,FRACMAX);
  result["LightCSVTCorr"]=Histogram("LightCSVTCorr",";corr. CSV_{light jet};Jets","jtcsvtcorr[ljtidx][0]","",NFRAC,0.,FRACMAX);
  result["TopCSVT"]=Histogram("TopCSVT",";CSV_{t};Jets","jtcsvt[topjtidx]","",NFRAC,0.,FRACMAX);
  result["LightCSVT"]=Histogram("LightCSVT",";CSV_{light jet};Jets","jtcsvt[ljtidx]","",NFRAC,0.,FRACMAX);
  result["HbbCSVTnn1"]=Histogram("HbbCSVTnn1",";corr. CSV1;Jets","jtcsvtcorr[hbbjtidx[0]][0]","",NFRAC,0.,FRACMAX);
  result["HbbCSVTnn2"]=Histogram("HbbCSVTnn2",";corr. CSV2;Jets","jtcsvtcorr[hbbjtidx[1]][0]","",NFRAC,0.,FRACMAX);
  result["HbbCSVTdisc1"]=Histogram("HbbCSVTdisc1",";discr. CSV1_{H};Jets","0+(jtcsvtcorr[hbbjtidx[0]][0]>0.244 &&jtcsvtcorr[hbbjtidx[0]][0]<0.679)+2*(jtcsvtcorr[hbbjtidx[0]][0]>0.679 &&jtcsvtcorr[hbbjtidx[0]][0]<0.898)+3*(jtcsvtcorr[hbbjtidx[0]][0]>0.898)","",NFRAC,0.,FRACMAX);
  result["HbbCSVTdisc2"]=Histogram("HbbCSVTdisc2",";discr. CSV2_{H};Jets","0+(jtcsvtcorr[hbbjtidx[1]][0]>0.244 &&jtcsvtcorr[hbbjtidx[1]][0]<0.679)+2*(jtcsvtcorr[hbbjtidx[1]][0]>0.679 &&jtcsvtcorr[hbbjtidx[1]][0]<0.898)+3*(jtcsvtcorr[hbbjtidx[1]][0]>0.898)","",NFRAC,0.,FRACMAX);
  result["LightCSVTdisc"]=Histogram("LightCSVTdisc",";discr. CSV_{light jet};Jets","0+(jtcsvtcorr[ljtidx][0]>0.244 &&jtcsvtcorr[ljtidx][0]<0.679)+2*(jtcsvtcorr[ljtidx][0]>0.679 &&jtcsvtcorr[ljtidx][0]<0.898)+3*(jtcsvtcorr[ljtidx][0]>0.898)","",NFRAC,0.,FRACMAX);
  result["TopCSVTdisc"]=Histogram("TopCSVTdisc",";discr. CSV_{t};Jets","0+(jtcsvtcorr[topjtidx][0]>0.244 &&jtcsvtcorr[topjtidx][0]<0.679)+2*(jtcsvtcorr[topjtidx][0]>0.679 &&jtcsvtcorr[topjtidx][0]<0.898)+3*(jtcsvtcorr[topjtidx][0]>0.898)","",NFRAC,0.,FRACMAX);
  result["HbbETCT"]=Histogram("HbbETCT",";jet btag-discriminant (ETCT);Jets","jtetct","(jtrank==hbbjtidx[0]||jtrank==hbbjtidx[1])",100,-50.,100.);
  result["HbbPTCT"]=Histogram("HbbPTCT",";jet btag-discriminant (PTCT);Jets","jtptct","(jtrank==hbbjtidx[0]||jtrank==hbbjtidx[1])",100,-50.,100.);
  result["HbbJet1Pt"]=Histogram("HbbJet1Pt",";p_{T}(j_{1}(H)) [GeV];Events","jtpt[hbbjtidx[0]]","1",100,0.,300.);
  result["HbbJet2Pt"]=Histogram("HbbJet2Pt",";p_{T}(j_{2}(H)) [GeV];Events","jtpt[hbbjtidx[1]]","1",100,0.,300.);
  result["HbbExtraJetRank"]=Histogram("HbbExtraJetRank",";extra jet rank;Events","hbbjtidx[2]",6,-1.5,4.5);  
  result["HbbExtraJetPt"]=Histogram("HbbExtraJetPt",";extra jet p_{T} [GeV];Events","jtpt","(jtrank==hbbjtidx[2])",40,0.0,150.);
  result["HbbExtraJetN"]=Histogram("HbbExtraJetN",";N(extra jets);Events","Sum$(jtpt>20.&&abs(jteta)<2.5&&jtdrhbb>0.0&&jtdrhbb<9.9)",7,-0.5,6.5);
  result["HbbExtraJetN_25"]=Histogram("HbbExtraJetN_25",";N(extra jets, 25);Events","Sum$(jtpt>25.&&abs(jteta)<2.5&&jtdrhbb>0.0&&jtdrhbb<9.9)",7,-0.5,6.5);
  result["HbbExtraJetN_30"]=Histogram("HbbExtraJetN_30",";N(extra jets, 30);Events","Sum$(jtpt>30.&&abs(jteta)<2.5&&jtdrhbb>0.0&&jtdrhbb<9.9)",7,-0.5,6.5);
  result["HbbExtraJetN_35"]=Histogram("HbbExtraJetN_35",";N(extra jets, 35);Events","Sum$(jtpt>35.&&abs(jteta)<2.5&&jtdrhbb>0.0&&jtdrhbb<9.9)",7,-0.5,6.5);
  result["HbbExtraJetN_40"]=Histogram("HbbExtraJetN_40",";N(extra jets, 40);Events","Sum$(jtpt>40.&&abs(jteta)<2.5&&jtdrhbb>0.0&&jtdrhbb<9.9)",7,-0.5,6.5);
  result["HbbExtraJetDR"]=Histogram("HbbExtraJetDR",";#Delta R(extra jet,H jets);Events","jtdrhbb","jtdrhbb>0.0",25,0.0,10.);  

  
  //
  // TOP RECO
  //
  result["TopHadM"] =Histogram("TopHadM", ";m_{t_{had}} [GeV];Events","tophadm",30,80.,280.);
  result["TopHadPt"] =Histogram("TopHadPt", ";p_{T,t_{had}} [GeV];Events","tophadpt",30,0.,520.);
  result["TopLepM"] =Histogram("TopLepM", ";m_{t_{lep}} [GeV];Events","toplepm",30,80.,450.); 
  result["TopLepPt"] =Histogram("TopLepPt", ";p_{T,t_{lep}} [GeV];Events","topleppt",30,0.,520.);
  result["TopHadEta"] =Histogram("TopHadEta", ";#eta_{T,t_{had}};Events","tophadeta",NFRAC,0.,FRACMAX);
  result["TopLepEta"] =Histogram("TopLepEta", ";#eta_{T,t_{lep}};Events","toplepeta",NFRAC,0.,FRACMAX);
  result["TopHadWm"] =Histogram("TopHadWm", ";m_{W_{had}} [GeV];Events","tophadwm",30,0.,350.);
  result["TopHadWPt"] =Histogram("TopHadWPt", ";p_{T,W_{had}} [GeV];Events","tophadwpt",30,0.,350.);
  result["TopHadWDR"] =Histogram("TopHadWDR", ";#Delta R(j1_W_{had},j2_W_{had});Events","tophaddr",30,0.,5);
  result["TopCSVT1"]=Histogram("TopCSVT1",";CSV_{b(t_{had})};Jets","jtcsvt[tophadjtidx[2]]","",NFRAC,0.,FRACMAX);
  result["TopCSVT2"]=Histogram("TopCSVT2",";CSV1_{W_{had}};Jets","jtcsvt[tophadjtidx[0]]","",NFRAC,0.,FRACMAX);
  result["TopCSVT3"]=Histogram("TopCSVT3",";CSV2_{W_{had}};Jets","jtcsvt[tophadjtidx[1]]","",NFRAC,0.,FRACMAX);
  result["TopLepCSVT"]=Histogram("TopLepCSVT",";CSV_{b(t_{lep})};Jets","jtcsvt[toplepjtidx]","",NFRAC,0.,FRACMAX);


  //
  // MCTRUTH DISTRIBUTIONS
  //
  result["NPU"]=Histogram("NPU",";Number of PU interactions;Events","npu[nnpu/2]",25,-0.5,24.5);
  result["BProd"]=Histogram("BProd",";b production","bprod",4,-0.5,3.5);
  
  result["GenVbbPt"]=Histogram("GenVbbPt",";true pT(H/Z#rightarrow b#bar{b}) [GeV];Events","Alt$((nh==0)*zpt[0],0.)+Alt$((nh==1)*hpt[0],0.)","nh+nz==2",35,0.,500.);
  result["GenZllPt"]=Histogram("GenZllPt",";true pT(Z#rightarrow ll) [GeV];Events","zpt","Sum$(abs(zdauid[])==11||abs(zdauid[]==13)||abs(zdauid[])==15)==2",35,0.,500.);
  result["GenZnunuPt"]=Histogram("GenZnunuPt",";true pT(Z#rightarrow#nu#nu) [GeV];Events","zpt","Sum$(abs(zdauid[])==12||abs(zdauid[]==14)||abs(zdauid[])==16)==2",35,0.,500.);
  result["GenWlnuPt"]=Histogram("GenWlnuPt",";true pT(W#rightarrow l#nu) [GeV];Events","wpt","Sum$(abs(wdauid[])==11||abs(wdauid[]==13)||abs(wdauid[])==15)==1",35,0.,500.);
  result["GenSubNb"]=Histogram("GenSubNb",";b-matched sub jets;Events","Sum$(abs(subgenflv[0][])==5)",3,-0.5,2.5);
  result["GenFltNb"]=Histogram("GenFltNb",";b-matched filter jets;Events","Sum$(abs(fltgenflv[0][])==5)",4,-0.5,3.5);
  result["GenSubNc"]=Histogram("GenSubNc",";c-matched sub jets;Events","Sum$(abs(subgenflv[0][])==4)",3,-0.5,2.5);
  result["GenFltNc"]=Histogram("GenFltNc",";c-matched filter jets;Events","Sum$(abs(fltgenflv[0][])==4)",4,-0.5,3.5);
  result["GenZM"]=Histogram("GenZM",";m_{Z}^{GEN} [GeV];Events","zm[0]",50,60.,120.);
  result["GenWM"]=Histogram("GenWM",";m_{W}^{GEN} [GeV];Events","wm[0]",50,40.,120.);
  
  
  //
  // PV
  //
  result["NPV"]=Histogram("NPV",";number of prim. vertex candidates;Events","npv",31,-0.5,30.5);
  result["PVz"]=Histogram("PVz",";Prim. Vertex z [cm];Events","pvz",21,-15.,15.);
  result["PVntrk"]=Histogram("PVntrk",";Prim. Vertex N_{trk};Events","pvntrk",50,-0.5,100.5);
  result["PVndof"]=Histogram("PVndof",";Prim. Vertex NDF;Events","pvndof",50,-0.5,100.5);
  result["PVchsq"]=Histogram("PVchsq",";Prim. Vertex #chi^{2}/NDF;Events","pvchisq",30,0,5.);

  
  //
  // NB
  //
  result["NBout"]=Histogram("NBout",";Neural Network output;Events","nnout",50,-1.,1.);
  result["NN110"]=Histogram("NN110",";Neural Network output;Events","nnout110",50,-1.,1.);
  result["NN115"]=Histogram("NN115",";Neural Network output;Events","nnout115",50,-1.,1.);
  result["NN120"]=Histogram("NN120",";Neural Network output;Events","nnout120",50,-1.,1.);
  result["NN125"]=Histogram("NN125",";Neural Network output;Events","nnout125",50,-1.,1.);
  result["NN130"]=Histogram("NN130",";Neural Network output;Events","nnout130",50,-1.,1.);
  result["NN135"]=Histogram("NN135",";Neural Network output;Events","nnout135",50,-1.,1.);

  result["transNBout"]=Histogram("transNBout",";NN output;Events","nbouttrans",15,-1.,1);

  result["RecMLPout"]=Histogram("RecMLPout",";best MVA output under tHq hypothesis;Events","best_recbdtout",50,-1.,1.);
  result["TopRecMLPout"]=Histogram("TopRecMLPout",";best MVA output under t#bar{t} hypothesis;Events","top_best_recbdtout",50,-1.,1.);
  //For randomly hyp
  result["RecMLPout2"]=Histogram("RecMLPout",";MVA output of randomly chosen tHq hypothesis;Events","best_recbdtout",50,-1.,1.);
  result["TopRecMLPout2"]=Histogram("TopRecMLPout",";MVA output of randomly chosen t#bar{t} hypothesis;Events","top_best_recbdtout",50,-1.,1.);

  //
  // BDT
  //
  result["BDTout"]=Histogram("BDTout",";BDT output;Events","bdtout",50,-1.,1.);
  result["BDTGout"]=Histogram("BDTGout",";BDT_{grad} output;Events","bdtgout",50,-1.,1.);
  result["transBDTGout"]=Histogram("transBDTGout",";BDT_{grad} output;Events","bdtgouttrans",15,-1.,1.);

  result["BDT110_nom"]=Histogram("BDT110",";BDT output;Events","bdtout110",50,-1.,1.);
  result["BDT115_nom"]=Histogram("BDT115",";BDT output;Events","bdtout115",50,-1.,1.);
  result["BDT120_nom"]=Histogram("BDT120",";BDT output;Events","bdtout120",50,-1.,1.);
  result["BDT125_nom"]=Histogram("BDT125",";BDT output;Events","bdtout125",50,-1.,1.);
  result["BDT130_nom"]=Histogram("BDT130",";BDT output;Events","bdtout130",50,-1.,1.);
  result["BDT135_nom"]=Histogram("BDT135",";BDT output;Events","bdtout135",50,-1.,1.);
  result["BDT110"]=Histogram("BDT110",";BDT output;Events","bdtout110trans",25,-1.,1.);
  result["BDT115"]=Histogram("BDT115",";BDT output;Events","bdtout115trans",25,-1.,1.);
  result["BDT120"]=Histogram("BDT120",";BDT output;Events","bdtout120trans",25,-1.,1.);
  result["BDT125"]=Histogram("BDT125",";BDT output;Events","bdtout125trans",25,-1.,1.);
  result["BDT130"]=Histogram("BDT130",";BDT output;Events","bdtout130trans",25,-1.,1.);
  result["BDT135"]=Histogram("BDT135",";BDT output;Events","bdtout135trans",25,-1.,1.);


  //
  // MLP
  //


  result["transMLPout"]=Histogram("transMLPout",";NN output;Events","mlpouttrans",25,-1.,1);

  result["MLPout"]=Histogram("MLPout",";NN output;Events","mlpout",50,-1.,1.5);


  


  //
  // Top
  //
  result["TopM"]=Histogram("TopM",";m_{t_{rec}} [GeV];Events","topm",60,100.,400.);
  result["TopPt"]=Histogram("TopPt",";p_{T,t_{rec}} [GeV];Events","toppt",40,0.,200.);
  result["TopEta"]=Histogram("TopPt",";#eta(t_{rec});Events","topeta",25,-6.,6.);
  result["FltTopM"]=Histogram("FltTopM",";m^{flt}_{t_{had}} [GeV];Events","flttopm",60,100.,400.);
  result["FltTopPt"]=Histogram("FltTopPt",";p^{flt}_{T,t_{had}} [GeV];Events","flttoppt",40,0.,200.);
  result["AltFltTopM"]=Histogram("AltFltTopM",";m^{flt}_{t_{had}} [GeV];Events","altflttopm",60,100.,400.);
  result["AltFltTopPt"]=Histogram("AltFltTopPt",";p^{flt}_{T,t_{had}} [GeV];Events","altflttoppt",40,0.,200.);

  result["SingleTopM"]=Histogram("SingleTopM",";m_{t_{sgl}} [GeV];Events","singletopm",60,100.,400.);
  result["SingleTopPt"]=Histogram("SingleTopPt",";p_{T,t_{sgl}} [GeV];Events","singletoppt",40,0.,200.);
  result["FltSingleTopM"]=Histogram("FltSingleTopM",";m^{flt}_{t_{sgl}} [GeV];Events","fltsingletopm",60,100.,400.);
  result["FltSingleTopPt"]=Histogram("FltSingleTopPt",";p^{flt}_{T,t_{sgl}} [GeV];Events","fltsingletoppt",40,0.,200.);

  



  //
  // MC TRUTH
  //
  result["HiggsM"]=Histogram("HiggsM",";m_{H_{gen}} [GeV];Events","Hm",60,80.,150.);
  result["HiggsPt"]=Histogram("HiggsPt",";p_{T,H_{gen}} [GeV];Events","Hpt",30,0.,400.);
  result["HiggsEta"]=Histogram("HiggsEta",";#eta_{H_{gen}};Events","Heta",40,-3.14,3.14);
  result["HiggsPhi"]=Histogram("HiggsPhi",";#phi_{H_{gen}};Events","Hphi",40,-5.,5.);
  result["TM"]=Histogram("TopM",";m_{t_{gen}} [GeV];Events","tm",60,100.,400.);
  result["TPt"]=Histogram("TopPt",";p_{T,t_{gen}} [GeV];Events","tpt",40,0.,300.);
  result["TEta"]=Histogram("TopEta",";#eta_{t_{gen}};Events","teta",40,-5.,5.);
  result["TPhi"]=Histogram("TopPhi",";#phi_{t_{gen}};Events","tphi",40,-3.14,3.14);
  result["MW"]=Histogram("MW",";m_{W_{gen}} [GeV];Events","Wm",60,100.,400.);
  result["PtW"]=Histogram("PtW",";p_{T,W_{gen}} [GeV];Events","Wpt",40,0.,300.);
  result["EtaW"]=Histogram("EtaW",";#eta_{W_{gen}};Events","Weta",40,-5.,5.);
  result["PhiW"]=Histogram("PhiW",";#phi_{W_{gen}};Events","Wphi",40,-3.14,3.14);
  result["BTopPt"]=Histogram("BTopPt",";p_{T,b(t)} [GeV];Events","btoppt",40,0.,300.);
  result["BTopEta"]=Histogram("BTopEta",";#eta_{b(t)};Events","btopeta",40,-5.,5.);
  result["BTopPhi"]=Histogram("BTopPhi",";#phi_{b(t)};Events","btopphi",40,-3.14,3.14);
  result["SBPt"]=Histogram("SBPt",";p_{T,add. b} [GeV];Events","sbpt",40,0.,300.);
  result["SBEta"]=Histogram("SBEta",";#eta_{add. b};Events","sbeta",40,-5.,5.);
  result["SBPhi"]=Histogram("SBPhi",";#phi_{add. b};Events","sbphi",40,-3.14,3.14);
  result["LQPt"]=Histogram("LQPt",";p_{T,q} [GeV];Events","lqpt",40,0.,300.);
  result["LQEta"]=Histogram("LQEta",";#eta_{q};Events","lqeta",40,-5.,5.);
  result["LQPhi"]=Histogram("LQPhi",";#phi_{q};Events","lqphi",40,-3.14,3.14);
  result["LepPt"]=Histogram("LepPt",";p_{T,l} [GeV];Events","Wdaupt[0]",40,0.,300.);
  result["LepEta"]=Histogram("LepEta",";#eta_{l};Events","Wdaueta[0]",40,-5.,5.);
  result["LepPhi"]=Histogram("LepPhi",";#phi_{l};Events","Wdauphi[0]",40,-3.14,3.14);
  result["NuPt"]=Histogram("NuPt",";p_{T,#nu} [GeV];Events","Wdaupt[1]",40,0.,300.);
  result["NuEta"]=Histogram("NuEta",";#eta_{#nu};Events","Wdaueta[1]",40,-5.,5.);
  result["NuPhi"]=Histogram("NuPhi",";#phi_{#nu};Events","Wdauphi[1]",40,-3.14,3.14);
  result["Hb1Pt"]=Histogram("Hb1Pt",";p_{T,b(H)} [GeV];Events","Hdaupt[0]",40,0.,500.);
  result["Hb1Eta"]=Histogram("Hb1Eta",";#eta_{b(H)};Events","Hdaueta[0]",40,-5.,5.);
  result["Hb1Phi"]=Histogram("Hb1Phi",";#phi_{b(H)};Events","Hdauphi[0]",40,-3.14,3.14);
  result["Hb2Pt"]=Histogram("Hb2Pt",";p_{T,#bar{b}(H)} [GeV];Events","Hdaupt[1]",40,0.,500.);
  result["Hb2Eta"]=Histogram("Hb2Eta",";#eta_{#bar{b}(H)};Events","Hdaueta[1]",40,-5.,5.);
  result["Hb2Phi"]=Histogram("Hb2Phi",";#phi_{#bar{b}(H)};Events","Hdauphi[1]",40,-3.14,3.14);

  result["PullAngle"]=Histogram("PullAngle",";pull angle;Events","jtpull[hbbjtidx[0]]-jtpull[hbbjtidx[1]]+3.14*((jtpull[hbbjtidx[0]]-jtpull[hbbjtidx[1]])<-3.14)-3.14*((jtpull[hbbjtidx[0]]-jtpull[hbbjtidx[1]])>3.14)",30,-3.14,3.14);
  result["Costst"]=Histogram("Costst",";cos(#theta*)_{t};Events","costst",25,-1,1);
  result["Coststh"]=Histogram("Coststh",";cos(#theta*)_{t+H};Events","coststh",25,-1,1);
  result["CosThetaStar"]=Histogram("CosThetaStar",";cos(#theta*)_{t};Events","costst_rec",25,-1,1);
  result["CosThetaStarHiggs"]=Histogram("CosThetaStarHiggs",";cos(#theta*)_{t+H};Events","coststh_rec",25,-1,1);
  result["CosZetaStar"]=Histogram("CosZetaStar",";cos(#zeta*)_{t+H};Events","coszetastar",25,-1,1);
  result["Twist"]=Histogram("Twist",";Twist;Events","twist",25,-1.58,1.58);
  result["Helicity1"]=Histogram("Helicity1",";Heliciy j_{1};Events","helicity_h1",25,-1,1);
  result["Helicity2"]=Histogram("Helicity2",";Heliciy j_{2};Events","helicity_h2",25,-1,1);  
  result["HelicityLight"]=Histogram("HelicityLight",";Heliciy j_{light};Events","helicity_ljt",25,-1,1);
  result["HelicityTop"]=Histogram("HelicityTop",";Heliciy j_{top};Events","helicity_topjt",25,-1,1);




  result["CosZetaStar"]=Histogram("CosZetaStar",";cos(#zeta*)_{t+H};Events","coszetastar",25,-1,1);
  result["Twist"]=Histogram("Twist",";Twist;Events","twist",25,-1.58,1.58);
  result["Helicity1"]=Histogram("Helicity1",";Heliciy j_{1};Events","helicity_h1",25,-1,1);
  result["Helicity2"]=Histogram("Helicity2",";Heliciy j_{2};Events","helicity_h2",25,-1,1);  
  result["HelicityLight"]=Histogram("HelicityLight",";Heliciy j_{light};Events","helicity_ljt",25,-1,1);
  result["HelicityTop"]=Histogram("HelicityTop",";Heliciy j_{top};Events","helicity_topjt",25,-1,1);




  // PULL ANLGES AND COLOUR CONNECTED STUFF

  result["TrackRatio"]=Histogram("TrackRatio",";n^{1st Higgs jet}_{tracks}/n^{top jet}_{tracks}, p_{T} weighted;Events","(jtntracks[hbbjtidx[0]]/jtpt[hbbjtidx[0]])/(jtntracks[topjtidx]/jtpt[topjtidx])",25,0,10);
  

  return result;
}
