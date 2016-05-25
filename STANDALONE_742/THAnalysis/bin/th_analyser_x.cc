////////////////////////////////////////////////////////////////////////////////
//
// th_analyser_x
// -----------------
//
//            24/05/2013              Benedikt Maier 
//                                    Christian Boeser
//                                    Simon Fink
///////////////////5/////////////////////////////////////////////////////////////

#include "../interface/CommandLine.h"
#include "../interface/TriggerWeight.h"
#include "../interface/KITAObjects.hpp"
#include "../interface/MCWeighters.h"
#include "../interface/BTagReshaping.h"
#include "../interface/BTagWeight.h"
#include "../interface/BTagSF.h"
#include "../interface/TTbarHFFinder.h"
#include "../interface/THStatus.h"

//include "EGamma/EGammaAnalysisTools/interface/EGammaMvaEleEstimator.h"
//#include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimator.h"
//#include "../interface/FactorizedJetCorrector.h"
#include "../../JetMETObjects/interface/FactorizedJetCorrector.h"
#include "../../JetMETObjects/interface/JetCorrectorParameters.h" 
#include "../../JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "KITA/KITAObjects/interface/KITAObject.h"
#include "KITA/KITAObjects/src/classes.h"

#include "boost/shared_ptr.hpp"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TKey.h>
#include <TEventList.h>
#include <TVector.h>
#include <TLorentzVector.h>
#include <TH1F.h>
#include "Cintex/Cintex.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>



using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////////////////////

// BTag SF per jet
void      fillScaleFactors(float pt, float eta, int flv, Float_t &csvl, Float_t &csvm, Float_t& ccsvt);

//Math
double    deltaR(double eta1, double phi1, double eta2, double phi2); 
double    deltaPhi(double phi1, double phi2);

//Trigger
Bool_t    trig_finder(string triggername);

//Lepton Isolation
double    muAEff(double eta);
float     eleAEff(float SCEta);
 
//KITA Definitions
KITAHlt*     kitahlt = new KITAHlt();
KITAGenInfo* kitaGenInfo = new KITAGenInfo();

//Object Definitions
bool PFJetID_2012(const KITAJet & jet, const double raw_e);
bool EleID_2012(const KITAElectron & lep, float relIso, float elemvaid);
bool EleVetoID_2012(const KITAElectron & lep, float relIso, float elemvaid);
bool MuID_2012(const KITAMuon & lep, float relIso, float pv_z, int pv_size);
bool MuVetoID_2012(const KITAMuon & lep, float relIso);
bool checkElectronMVAIDPreselection(const KITAElectron & ele);
bool PrimaryVertexCut(const KITAPrimaryVertices & pv);
int  NGoodPV(const KITAPrimaryVertices & pv);
bool Preselection(const KITAMisc & kitamisc, const KITADataCleaning & kitadatacleaning, bool isdata);

//Sorting
struct GreaterByPt{
  float * pts;
    
  GreaterByPt(float * pts_): pts(pts_){}
    
  bool operator()(size_t i, size_t j) const{
    return pts[i] > pts[j];
  }
};

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  // to make dictionary available
  ROOT::Cintex::Cintex::Enable();

  //
  // evaluate command line options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  string         input       = cl.getValue<string> ("input");
  string         output      = cl.getValue<string> ("output",           "test.root");
  string         itreename   = cl.getValue<string> ("itreename",         "KITATree");
  string         otreename   = cl.getValue<string> ("otreename",                "t");  
  string         odirname    = cl.getValue<string> ("odirname",               "utm");
  bool           isSignal    = cl.getValue<bool>   ("isSignal",                   0);
  bool           isTop       = cl.getValue<bool>   ("isTop",                      0);
  bool           isData      = cl.getValue<bool>   ("isData",                     0);
  bool           debug       = cl.getValue<bool>   ("debug",                      0);
  vector<int>    debugevent  = cl.getVector<int>   ("events",                    "");

  if (!cl.check()) return 0;
  cl.print();

  TFile* ifile = TFile::Open(input.c_str(),"READ");
  if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

  // create temporary output file
  TFile* ofile = new TFile(output.c_str(),"RECREATE");
  if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
    
  // create output directory
  TDirectory* odir = (TDirectory*)ofile->mkdir(odirname.c_str());
  if(0==odir){cout<<"Failed to make "<<odirname<<" in "<<output<<endl;return 0;}
  
  // open input tree
  TTree* itree = (TTree*)ifile->Get(itreename.c_str());
  if(0==itree){cout<<"No "<<itreename<<endl;return 0;}

  odir->cd();

  TTree* otree = new TTree(otreename.c_str(), otreename.c_str());
  
  cout<<output<<"/"<<odirname<<"/"<<otreename<<" created (temporary!)"<<endl;
          
  // create utm branches in output tree (in principle every branch set in a standard UTM root file)
  // --------------------------------------------------------------------------------------------------   
  
  //GENJETS
  Int_t   ngenjt=0;                   otree->Branch("ngenjt",           &ngenjt,                         "ngenjt/I");
  Int_t   ngenjt5=0;                  otree->Branch("ngenjt5",          &ngenjt5,                   "ngenjt5/I");
  Int_t   ngenjt15=0;                 otree->Branch("ngenjt15",         &ngenjt15,                 "ngenjt15/I");
  Float_t genjtpt[140]={-99};         otree->Branch("genjtpt",          genjtpt,                 "genjtpt[ngenjt5]/F");
  Float_t genjteta[140]={-99};        otree->Branch("genjteta",         genjteta,               "genjteta[ngenjt5]/F");
  Float_t genjtphi[140]={-99};        otree->Branch("genjtphi",         genjtphi,               "genjtphi[ngenjt5]/F");
  Float_t genjte[140]={-99};          otree->Branch("genjte",           genjte,                   "genjte[ngenjt5]/F");

  //JETS
  Int_t   njt=0;                   otree->Branch("njt",           &njt,                         "njt/I");
  Int_t   njt15=0;                 otree->Branch("njt15",           &njt15,                   "njt15/I");
  Float_t jtpt[140]={-99};         otree->Branch("jtpt",          jtpt,                 "jtpt[njt15]/F");
  Float_t jtrawpt[140]={-99};      otree->Branch("jtrawpt",       jtrawpt,           "jtrawpt[njt15]/F");
  Float_t jteta[140]={-99};        otree->Branch("jteta",         jteta,               "jteta[njt15]/F");
  Float_t jtphi[140]={-99};        otree->Branch("jtphi",         jtphi,               "jtphi[njt15]/F");
  Float_t jte[140]={-99};          otree->Branch("jte",           jte,                   "jte[njt15]/F");
  Float_t sumHt = -99;             otree->Branch("sumHt",         &sumHt,                     "sumHt/F");
  Short_t jtgenflv[140]={-99};     otree->Branch("jtgenflv",      jtgenflv,         "jtgenflv[njt15]/S");
  Float_t jtgenpt[140]={-99};      otree->Branch("jtgenpt",       jtgenpt,           "jtgenpt[njt15]/F");
  Float_t jtgeneta[140]={-99};     otree->Branch("jtgeneta",      jtgeneta,         "jtgeneta[njt15]/F");
  Float_t jtgenphi[140]={-99};     otree->Branch("jtgenphi",      jtgenphi,         "jtgenphi[njt15]/F");
  Float_t jtgene[140]={-99};       otree->Branch("jtgene",        jtgene,             "jtgene[njt15]/F");
  Float_t jtcsvt[140]={-99};       otree->Branch("jtcsvt",        jtcsvt,             "jtcsvt[njt15]/F");
  Float_t jtcsvtcorr[140][5];      otree->Branch("jtcsvtcorr",    jtcsvtcorr,  "jtcsvtcorr[njt15][5]/F"); 
  Float_t jtcsvtcorr4p[140][4];    otree->Branch("jtcsvtcorr4p",  jtcsvtcorr4p,"jtcsvtcorr4p[njt15][4]/F"); 
  Float_t jtntracks[140]={-99};    otree->Branch("jtntracks",     jtntracks,       "jtntracks[njt15]/F");
  Float_t jtarea[140]={-99};       otree->Branch("jtarea",        jtarea,             "jtarea[njt15]/F");
  Float_t jtpull[140]={-99};       otree->Branch("jtpull",        jtpull,             "jtpull[njt15]/F");
  Float_t jtcharge[140]={-99};     otree->Branch("jtcharge",      jtcharge,         "jtcharge[njt15]/F");
  Bool_t  jtid[140]={0};           otree->Branch("jtid",          jtid,                 "jtid[njt15]/O");
  Float_t jtpuidmva[140]={-99};    otree->Branch("jtpuidmva",     jtpuidmva,       "jtpuidmva[njt15]/F");
  Bool_t  jtpumvaidl[140]={-99};   otree->Branch("jtpumvaidl",    jtpumvaidl,     "jtpumvaidl[njt15]/O");
  Bool_t  jtpumvaidm[140]={-99};   otree->Branch("jtpumvaidm",    jtpumvaidm,     "jtpumvaidm[njt15]/O");
  Bool_t  jtpumvaidt[140]={-99};   otree->Branch("jtpumvaidt",    jtpumvaidt,     "jtpumvaidt[njt15]/O");
  Bool_t  jtpucutidl[140]={-99};   otree->Branch("jtpucutidl",    jtpucutidl,     "jtpucutidl[njt15]/O");
  Bool_t  jtpucutidm[140]={-99};   otree->Branch("jtpucutidm",    jtpucutidm,     "jtpucutidm[njt15]/O");
  Bool_t  jtpucutidt[140]={-99};   otree->Branch("jtpucutidt",    jtpucutidt,     "jtpucutidt[njt15]/O");
  Float_t jtjer[140]={-99};        otree->Branch("jtjer",         jtjer,               "jtjer[njt15]/F");
  // Saving stuff for jet id 
  Int_t   jtndaughters[140]={-99}; otree->Branch("jtndaughters",  jtndaughters,     "jtndaughters[njt15]/I");
  Int_t   jtchmult[140]={-99};     otree->Branch("jtchmult",      jtchmult,             "jtchmult[njt15]/I");
  Float_t jtnhadronfrac[140]={-99};  otree->Branch("jtnhadronfrac", jtnhadronfrac,   "jtnhadronfrac[njt15]/F");
  Float_t jthfhadronfrac[140]={-99}; otree->Branch("jthfhadronfrac",jthfhadronfrac, "jthfhadronfrac[njt15]/F");
  Float_t jtnemfrac[140]={-99};      otree->Branch("jtnemfrac",     jtnemfrac,           "jtnemfrac[njt15]/F");
  Float_t jtchemfrac[140]={-99};     otree->Branch("jtchemfrac",    jtchemfrac,         "jtchemfrac[njt15]/F");
  Float_t jtchhadronfrac[140]={-99}; otree->Branch("jtchhadronfrac",jtchhadronfrac, "jtchhadronfrac[njt15]/F");
  Float_t jtchhadmult[140]={-99};  otree->Branch("jtchhadmult",   jtchhadmult,   "jtchhadmult[njt15]/F");
  Float_t jtcsvsf_l[140]={-99};    otree->Branch("jtcsvsf_l",     jtcsvsf_l,       "jtcsvsf_l[njt15]/F");
  Float_t jtcsvsf_m[140]={-99};    otree->Branch("jtcsvsf_m",     jtcsvsf_m,       "jtcsvsf_m[njt15]/F");
  Float_t jtcsvsf_t[140]={-99};    otree->Branch("jtcsvsf_t",     jtcsvsf_t,       "jtcsvsf_t[njt15]/F");
  Float_t jtcsvsf_lerr[140]={-99}; otree->Branch("jtcsvsf_lerr",  jtcsvsf_lerr, "jtcsvsf_lerr[njt15]/F");
  Float_t jtcsvsf_merr[140]={-99}; otree->Branch("jtcsvsf_merr",  jtcsvsf_merr, "jtcsvsf_merr[njt15]/F");
  Float_t jtcsvsf_terr[140]={-99}; otree->Branch("jtcsvsf_terr",  jtcsvsf_terr, "jtcsvsf_terr[njt15]/F");

  //LEPTONS: FIRST LOOSE THEN ISOLATED.

  Int_t   nlmu;                    otree->Branch("nlmu",          &nlmu,                     "nlmu/I");
  Float_t lmupt[110]={-99};        otree->Branch("lmupt",         lmupt,              "lmupt[nlmu]/F");
  Float_t lmueta[110]={-99};       otree->Branch("lmueta",        lmueta,            "lmueta[nlmu]/F");
  Float_t lmuphi[110]={-99};       otree->Branch("lmuphi",        lmuphi,            "lmuphi[nlmu]/F");
  Float_t lmuiso[110]={-99};       otree->Branch("lmuiso",        lmuiso,            "lmuiso[nlmu]/F");
  Float_t lmuiso_dBeta[110]={-99}; otree->Branch("lmuiso_dBeta",  lmuiso_dBeta,"lmuiso_dBeta[nlmu]/F");
  Float_t lmue[110]={-99};         otree->Branch("lmue",          lmue,                "lmue[nlmu]/F");
  Float_t lmucharge[110]={-99};    otree->Branch("lmucharge",     lmucharge,      "lmucharge[nlmu]/F");
  Float_t musigid[110]={-99};      otree->Branch("musigid",       musigid,          "musigid[nlmu]/F");
  Float_t muvetoid[110]={-99};     otree->Branch("muvetoid",      muvetoid,        "muvetoid[nlmu]/F");
  
  Int_t   nvetomu={-99};           otree->Branch("nvetomu",       &nvetomu,               "nvetomu/I");
  Int_t   nmu={-99};               otree->Branch("nmu",           &nmu,                       "nmu/I");
  Float_t mupt[110]={-99};         otree->Branch("mupt",          mupt,                 "mupt[nmu]/F");
  Float_t mueta[110]={-99};        otree->Branch("mueta",         mueta,               "mueta[nmu]/F");
  Float_t muphi[110]={-99};        otree->Branch("muphi",         muphi,               "muphi[nmu]/F");
  Float_t muiso[110]={-99};        otree->Branch("muiso",         muiso,               "muiso[nmu]/F");
  Float_t mue[110]={-99};          otree->Branch("mue",           mue,                   "mue[nmu]/F");
  Float_t mucharge[110]={-99};     otree->Branch("mucharge",      mucharge,         "mucharge[nmu]/F");

  Int_t   nlel;                    otree->Branch("nlel",          &nlel,                     "nlel/I");
  Float_t lelpt[110]={-99};        otree->Branch("lelpt",         lelpt,              "lelpt[nlel]/F");
  Float_t leleta[110]={-99};       otree->Branch("leleta",        leleta,            "leleta[nlel]/F");
  Float_t lelphi[110]={-99};       otree->Branch("lelphi",        lelphi,            "lelphi[nlel]/F");
  Float_t leliso[110]={-99};       otree->Branch("leliso",        leliso,            "leliso[nlel]/F");
  Float_t leliso2[110]={-99};      otree->Branch("leliso2",       leliso2,          "leliso2[nlel]/F");
  Float_t lele[110]={-99};         otree->Branch("lele",          lele,                "lele[nlel]/F");
  Float_t lelcharge[110]={-99};    otree->Branch("lelcharge",     lelcharge,      "lelcharge[nlel]/F");
  Float_t lelmvaid[110]={-99};     otree->Branch("lelmvaid",      lelmvaid,        "lelvmaid[nlel]/F");
  Float_t  lelconv[110]={-99};     otree->Branch("lelconv",       lelconv,          "lelconv[nlel]/F");

  Int_t   nvetoel={-99};           otree->Branch("nvetoel",       &nvetoel,               "nvetoel/I");
  Int_t   nel={-99};               otree->Branch("nel",           &nel,                       "nel/I");
  Float_t elpt[110]={-99};         otree->Branch("elpt",          elpt,                 "elpt[nel]/F");
  Float_t eleta[110]={-99};        otree->Branch("eleta",         eleta,               "eleta[nel]/F");
  Float_t elphi[110]={-99};        otree->Branch("elphi",         elphi,               "elphi[nel]/F");
  Float_t eliso[110]={-99};        otree->Branch("eliso",         eliso,               "eliso[nel]/F");
  Float_t ele[110]={-99};          otree->Branch("ele",           ele,                   "ele[nel]/F");
  Float_t elcharge[110]={-99};     otree->Branch("elcharge",      elcharge,         "elcharge[nel]/F");
  Float_t elmvaid[110]={-99};      otree->Branch("elmvaid",       elmvaid,           "elvmaid[nel]/F");
  Float_t elsigid[110]={-99};      otree->Branch("elsigid",       elsigid,          "elsigid[nlel]/F");  
  Float_t elvetoid[110]={-99};     otree->Branch("elvetoid",      elvetoid,        "elvetoid[nlel]/F");
  
  //MC TRUTH INFORMATION     

  // Higgs part
  Float_t Hpt={-99};               otree->Branch("Hpt",           &Hpt,                       "Hpt/F");
  Float_t Heta={-99};              otree->Branch("Heta",          &Heta,                     "Heta/F");
  Float_t Hphi={-99};              otree->Branch("Hphi",          &Hphi,                     "Hphi/F");
  Float_t Hm={-99};                otree->Branch("Hm",            &Hm,                         "Hm/F");

  Float_t Wpt={-99};               otree->Branch("Wpt",           &Wpt,                       "Wpt/F");
  Float_t Weta={-99};              otree->Branch("Weta",          &Weta,                     "Weta/F");
  Float_t Wphi={-99};              otree->Branch("Wphi",          &Wphi,                     "Wphi/F");
  Float_t Wm={-99};                otree->Branch("Wm",            &Wm,                         "Wm/F");

  Float_t tpt={-99};               otree->Branch("tpt",           &tpt,                       "tpt/F");
  Float_t teta={-99};              otree->Branch("teta",          &teta,                     "teta/F");
  Float_t tphi={-99};              otree->Branch("tphi",          &tphi,                     "tphi/F");
  Float_t tm={-99};                otree->Branch("tm",            &tm,                         "tm/F");

  Float_t btoppt={-99};            otree->Branch("btoppt",        &btoppt,                 "btoppt/F");
  Float_t btopeta={-99};           otree->Branch("btopeta",       &btopeta,               "btopeta/F");
  Float_t btopphi={-99};           otree->Branch("btopphi",       &btopphi,               "btopphi/F");
  Float_t btopm={-99};             otree->Branch("btopm",         &btopm,                   "btopm/F");
  
  Float_t sbpt={-99};              otree->Branch("sbpt",          &sbpt,                     "sbpt/F");
  Float_t sbeta={-99};             otree->Branch("sbeta",         &sbeta,                   "sbeta/F");
  Float_t sbphi={-99};             otree->Branch("sbphi",         &sbphi,                   "sbphi/F");
  Float_t sbm={-99};               otree->Branch("sbm",           &sbm,                       "sbm/F");

  Float_t lqpt={-99};              otree->Branch("lqpt",          &lqpt,                     "lqpt/F");
  Float_t lqeta={-99};             otree->Branch("lqeta",         &lqeta,                   "lqeta/F");
  Float_t lqphi={-99};             otree->Branch("lqphi",         &lqphi,                   "lqphi/F");
  Float_t lqm={-99};               otree->Branch("lqm",           &lqm,                       "lqm/F");

  // 0: bottom, 1: anti-bottom
  Float_t Hdaupt[2]={-99};         otree->Branch("Hdaupt",        Hdaupt,               "Hdaupt[2]/F");
  Float_t Hdaueta[2]={-99};        otree->Branch("Hdaueta",       Hdaueta,             "Hdaueta[2]/F");
  Float_t Hdauphi[2]={-99};        otree->Branch("Hdauphi",       Hdauphi,             "Hdauphi[2]/F");
  Float_t Hdaum[2]={-99};          otree->Branch("Hdaum",         Hdaum,                 "Hdaum[2]/F");

  // 0: lepton, 1: neutrino
  Float_t Wdaupt[2]={-99};         otree->Branch("Wdaupt",        Wdaupt,               "Wdaupt[2]/F");
  Float_t Wdaueta[2]={-99};        otree->Branch("Wdaueta",       Wdaueta,             "Wdaueta[2]/F");
  Float_t Wdauphi[2]={-99};        otree->Branch("Wdauphi",       Wdauphi,             "Wdauphi[2]/F");
  Float_t Wdaum[2]={-99};          otree->Branch("Wdaum",         Wdaum,                 "Wdaum[2]/F");
 
  //Top part

  Float_t top_tpt={-99};           otree->Branch("top_tpt",       &top_tpt,               "top_tpt/F");
  Float_t top_teta={-99};          otree->Branch("top_teta",      &top_teta,             "top_teta/F");
  Float_t top_tphi={-99};          otree->Branch("top_tphi",      &top_tphi,             "top_tphi/F");
  Float_t top_te={-99};            otree->Branch("top_te",        &top_te,                 "top_te/F");
  
  Float_t top_tbarpt={-99};        otree->Branch("top_tbarpt",    &top_tbarpt,         "top_tbarpt/F");
  Float_t top_tbareta={-99};       otree->Branch("top_tbareta",   &top_tbareta,       "top_tbareta/F");
  Float_t top_tbarphi={-99};       otree->Branch("top_tbarphi",   &top_tbarphi,       "top_tbarphi/F");
  Float_t top_tbare={-99};         otree->Branch("top_tbare",     &top_tbare,           "top_tbare/F");

  Float_t top_twpt={-99};          otree->Branch("top_twpt",      &top_twpt,             "top_twpt/F");
  Float_t top_tweta={-99};         otree->Branch("top_tweta",     &top_tweta,           "top_tweta/F");
  Float_t top_twphi={-99};         otree->Branch("top_twphi",     &top_twphi,           "top_twphi/F");
  Float_t top_twe={-99};           otree->Branch("top_twe",       &top_twe,               "top_twe/F");


  Float_t top_tbarwpt={-99};       otree->Branch("top_tbarwpt",   &top_tbarwpt,       "top_tbarwpt/F");
  Float_t top_tbarweta={-99};      otree->Branch("top_tbarweta",  &top_tbarweta,     "top_tbarweta/F");
  Float_t top_tbarwphi={-99};      otree->Branch("top_tbarwphi",  &top_tbarwphi,     "top_tbarwphi/F");
  Float_t top_tbarwe={-99};        otree->Branch("top_tbarwe",    &top_tbarwe,         "top_tbarwe/F");

  Float_t top_twdaupt[2]={-99};          otree->Branch("top_twdaupt",      top_twdaupt,             "top_twdaupt[2]/F");
  Float_t top_twdaueta[2]={-99};         otree->Branch("top_twdaueta",     top_twdaueta,           "top_twdaueta[2]/F");
  Float_t top_twdauphi[2]={-99};         otree->Branch("top_twdauphi",     top_twdauphi,           "top_twdauphi[2]/F");
  Float_t top_twdaue[2]={-99};           otree->Branch("top_twdaue",       top_twdaue,               "top_twdaue[2]/F");

  Float_t top_tbarwdaupt[2]={-99};          otree->Branch("top_tbarwdaupt",      top_tbarwdaupt,             "top_tbarwdaupt[2]/F");
  Float_t top_tbarwdaueta[2]={-99};         otree->Branch("top_tbarwdaueta",     top_tbarwdaueta,           "top_tbarwdaueta[2]/F");
  Float_t top_tbarwdauphi[2]={-99};         otree->Branch("top_tbarwdauphi",     top_tbarwdauphi,           "top_tbarwdauphi[2]/F");
  Float_t top_tbarwdaue[2]={-99};           otree->Branch("top_tbarwdaue",       top_tbarwdaue,               "top_tbarwdaue[2]/F");
  

  Float_t top_tbpt={-99};          otree->Branch("top_tbpt",      &top_tbpt,             "top_tbpt/F");
  Float_t top_tbeta={-99};         otree->Branch("top_tbeta",     &top_tbeta,           "top_tbeta/F");
  Float_t top_tbphi={-99};         otree->Branch("top_tbphi",     &top_tbphi,           "top_tbphi/F");
  Float_t top_tbe={-99};           otree->Branch("top_tbe",       &top_tbe,               "top_tbe/F");

  Float_t top_tbarbpt={-99};          otree->Branch("top_tbarbpt",      &top_tbarbpt,             "top_tbarbpt/F");
  Float_t top_tbarbeta={-99};         otree->Branch("top_tbarbeta",     &top_tbarbeta,           "top_tbarbeta/F");
  Float_t top_tbarbphi={-99};         otree->Branch("top_tbarbphi",     &top_tbarbphi,           "top_tbarbphi/F");
  Float_t top_tbarbe={-99};           otree->Branch("top_tbarbe",       &top_tbarbe,               "top_tbarbe/F");
  
  Float_t top_lepcharge={-99};     otree->Branch("top_lepcharge", &top_lepcharge,   "top_lepcharge/F");

  //MET + W BOSON

  Float_t met=-99;               otree->Branch("met",           &met,                       "met/F");
  Float_t uncorrmet=-99;         otree->Branch("uncorrmet",     &uncorrmet,           "uncorrmet/F");
  Float_t metphi=-99;            otree->Branch("metphi",        &metphi,                 "metphi/F");
  Float_t uncorrmetphi=-99;      otree->Branch("uncorrmetphi",  &uncorrmetphi,     "uncorrmetphi/F");
  Float_t meteta=-99;            otree->Branch("meteta",        &meteta,                 "meteta/F");
  Float_t metx=-99;              otree->Branch("metx",          &metx,                     "metx/F");
  Float_t mety=-99;              otree->Branch("mety",          &mety,                     "mety/F");     
  Float_t metsig=-99;              otree->Branch("metsig",        &metsig,                 "metsig/F");
  Float_t metsig_old=-99;          otree->Branch("metsig_old",    &metsig_old,         "metsig_old/F");
  Float_t metsumet=-99;            otree->Branch("metsumet",      &metsumet,             "metsumet/F");
  // unclustered met shift [0]=up, [1]=down:
  Float_t metsysx[2]={-99,-99};    otree->Branch("metsysx",       metsysx,             "metsysx[2]/F");
  Float_t metsysy[2]={-99,-99};    otree->Branch("metsysy",       metsysy,             "metsysy[2]/F");
  Float_t metsys[2]={-99,-99};     otree->Branch("metsys",        metsys,               "metsys[2]/F");

  UChar_t nlepw={-99};             otree->Branch("nlepw",         &nlepw,                   "nlepw/b");
  Float_t lepwm={-99};             otree->Branch("lepwm",         &lepwm,                   "lepwm/F");
  Float_t lepwpt={-99};            otree->Branch("lepwpt",        &lepwpt,                 "lepwpt/F");
  Float_t lepwphi={-99};           otree->Branch("lepwphi",       &lepwphi,               "lepwphi/F");
  Float_t lepweta={-99};           otree->Branch("lepweta",       &lepweta,               "lepweta/F");
  Float_t lepwet={-99};            otree->Branch("lepwet",        &lepwet,                 "lepwet/F");
  Float_t lepwmt={-99};            otree->Branch("lepwmt",        &lepwmt,                 "lepwmt/F");
  Float_t lepwdphi={-99};          otree->Branch("lepwdphi",      &lepwdphi,             "lepwdphi/F");
  Float_t sphericity=-99;          otree->Branch("sphericity",    &sphericity,         "sphericity/F");
  Float_t aplanarity=-99;          otree->Branch("aplanarity",    &aplanarity,         "aplanarity/F");

  Float_t MassDeviation={-99};     otree->Branch("MassDeviation", &MassDeviation,   "MassDeviation/F");
  
  Float_t hbbm={-99};              otree->Branch("hbbm",          &hbbm,                     "hbbm/F");
  Float_t hbbpt={-99};             otree->Branch("hbbpt",         &hbbpt,                   "hbbpt/F");
  Float_t hbbphi={-99};            otree->Branch("hbbphi",        &hbbphi,                 "hbbphi/F");
  Float_t hbbeta={-99};            otree->Branch("hbbeta",        &hbbeta,                 "hbbeta/F");
  Float_t hbbdr={-99};             otree->Branch("hbbdr",         &hbbdr,                   "hbbdr/F");
  Int_t   hbbjtidx[3]={-99};       otree->Branch("hbbjtidx",      hbbjtidx,           "hbbjtidx[3]/I");


  Float_t topm={-99};              otree->Branch("topm",          &topm,                     "topm/F");
  Float_t toppt={-99};             otree->Branch("toppt",         &toppt,                   "toppt/F");
  Float_t topphi={-99};            otree->Branch("topphi",        &topphi,                 "topphi/F");
  Float_t topeta={-99};            otree->Branch("topeta",        &topeta,                 "topeta/F");
  Int_t   topjtidx={-99};          otree->Branch("topjtidx",      &topjtidx,             "topjtidx/I");

  Float_t costst={-99};            otree->Branch("costst",        &costst,                 "costst/F");
  Float_t coststh={-99};           otree->Branch("coststh",       &coststh,               "coststh/F");

  Bool_t  hlt_isomu24_eta2p1=0;     otree->Branch("hlt_isomu24_eta2p1",&hlt_isomu24_eta2p1,"hlt_isomu24_eta2p1/O");

  Bool_t  hlt_isomu24=0;            otree->Branch("hlt_isomu24",       &hlt_isomu24,              "hlt_isomu24/O");

  Bool_t  hlt_ele27_wp80=0;         otree->Branch("hlt_ele27_wp80",    &hlt_ele27_wp80,        "hlt_ele27_wp80/O");
  Bool_t  hlt_mu24_centralpfjet30_centralpfjet25=0;         otree->Branch("hlt_mu24_centralpfjet30_centralpfjet25",    &hlt_mu24_centralpfjet30_centralpfjet25,        "hlt_mu24_centralpfjet30_centralpfjet25/O");
  Bool_t  hlt_mu24_eta2p1_centralpfjet30_centralpfjet25=0;         otree->Branch("hlt_mu24_eta2p1_centralpfjet30_centralpfjet25",    &hlt_mu24_eta2p1_centralpfjet30_centralpfjet25,        "hlt_eta2p1_mu24_eta2p1_centralpfjet30_centralpfjet25/O");
  Bool_t  hlt_mu18_centralpfjet30_centralpfjet25=0;         otree->Branch("hlt_mu18_centralpfjet30_centralpfjet25",    &hlt_mu18_centralpfjet30_centralpfjet25,        "hlt_mu18_centralpfjet30_centralpfjet25/O");
  


  Float_t weightTrig=1;            otree->Branch("weightTrig",    &weightTrig,         "weightTrig/F");
  Float_t weightPU=1;              otree->Branch("weightPU",      &weightPU,             "weightPU/F");
  Float_t weightPU_up=1;           otree->Branch("weightPU_up",   &weightPU_up,       "weightPU_up/F");
  Float_t weightPU_down=1;         otree->Branch("weightPU_down", &weightPU_down,   "weightPU_down/F");

  // MISCALLENEOUS

  UChar_t npv={-99};               otree->Branch("npv",           &npv,                       "npv/b");
  Int_t   run={-99};               otree->Branch("run",           &run,                       "run/i");
  ULong64_t evt={-99};             otree->Branch("evt",           &evt,                       "evt/l");
  UInt_t  lbn={-99};               otree->Branch("lbn",           &lbn,                       "lbn/i");
  Float_t poissonmean=1;           otree->Branch("poissonmean",   &poissonmean,       "poissonmean/F");
  Float_t evtrho={-99};            otree->Branch("evtrho",        &evtrho,                 "evtrho/F");

  Int_t hfcontent = -99;           otree->Branch("hfcontent",     &hfcontent,           "hfcontent/i");

  //For HBHE, Scraping, OptionalMETFilters
  Bool_t passpresel=1;             otree->Branch("passpresel",    &passpresel,        "passpresel/O");

  float btag2MSF=1.;               otree->Branch("btag2MSF",      &btag2MSF,        "btag2MSF/F");
  float btag2TSF=1.;               otree->Branch("btag2TSF",      &btag2TSF,        "btag2TSF/F");
  float btag3MSF=1.;               otree->Branch("btag3MSF",      &btag3MSF,        "btag3MSF/F");
  float btag3TSF=1.;               otree->Branch("btag3TSF",      &btag3TSF,        "btag3TSF/F");
  float btag4MSF=1.;               otree->Branch("btag4MSF",      &btag4MSF,        "btag4MSF/F");
  float btag4TSF=1.;               otree->Branch("btag4TSF",      &btag4TSF,        "btag4TSF/F");


  // Set branch addresses in input tree where necessary
  // ------------------------------------------------------------------------------------------------------

  KITAJets* jets = new KITAJets();
  itree->SetBranchAddress("KITAJets", &jets);

  KITAGenJets* gen_jets = new KITAGenJets();
  itree->SetBranchAddress("KITAGenJets", &gen_jets);
   
  KITAPFMet*  mykitapfmet = new KITAPFMet();
  itree->SetBranchAddress("KITAPFMet",&mykitapfmet);

  KITAPrimaryVertices* kitapvs = new KITAPrimaryVertices();
  itree->SetBranchAddress("KITAPrimaryVertices", &kitapvs);

  KITAMuons* mykitamuonsloose = new KITAMuons();
  itree->SetBranchAddress("KITAMuonsLoose", &mykitamuonsloose);

  KITAElectrons* mykitaelectronsloose = new KITAElectrons();
  itree->SetBranchAddress("KITAElectronsLoose", &mykitaelectronsloose);

  itree->SetBranchAddress("KITAHlt", &kitahlt);
 
  if(!isData)
    itree->SetBranchAddress("KITAGenInfo", &kitaGenInfo);


  KITAGenTopEvent* mykitagentopevent = new KITAGenTopEvent();
  if(isTop)
    {
      itree->SetBranchAddress("KITAGenTopEvent", &mykitagentopevent);
    }

  //  KITAGenParticles* mykitagenstatus2 = new KITAGenParticles();
  //  vector<KITAGenParticle*> mykitagenstatus2;
  //if(isTop)
  //  {
  //    itree->SetBranchAddress("KITAGenFinalState_status2", &mykitagenstatus2);
  //  }
  

  KITAMisc* mykitamisc = new KITAMisc();
  itree->SetBranchAddress("KITAMisc", &mykitamisc);

  KITADataCleaning* mykitadatacleaning = new KITADataCleaning();
  itree->SetBranchAddress("KITADataCleaning", &mykitadatacleaning);

  TriggerWeight triggerWeight;

  //  TTbarHFFinder* hffinder = new TTbarHFFinder(itree);

  
  TTbarHFFinder* hffinder = 0; 
  if(isTop){
    hffinder = new TTbarHFFinder(itree, jets); 
  }


  //Btag Reshaping
  //-----------------------------------------------------------------------------------------------------
  BTagShapeInterface * nominalShape=0;
  BTagShapeInterface * downBCShape=0;
  BTagShapeInterface * upBCShape=0;
  BTagShapeInterface * downLShape=0;
  BTagShapeInterface * upLShape=0;
  
  BTagShapeInterface * nominalShape4p=0;
  BTagShapeInterface * downBCShape4p=0;
  BTagShapeInterface * upBCShape4p=0;
  BTagShapeInterface * nominalShape1Bin=0;
  
  char* standalonedir = getenv("STANDALONE_DIR");
  string btag = string(standalonedir);
  const string btagshape = btag + "/THAnalysis/csvdiscr.root";

  if(!isData) 
    {
      nominalShape = new BTagShapeInterface(btagshape.c_str(), 0.0, 0.0); 
      upBCShape = new BTagShapeInterface(btagshape.c_str(), 1.0, 0.0); 
      downBCShape = new BTagShapeInterface(btagshape.c_str(), -1.0, 0.0); 
      upLShape = new BTagShapeInterface(btagshape.c_str(), 0.0, 1.0); 
      downLShape = new BTagShapeInterface(btagshape.c_str(), 0.0, -1.0); 
      nominalShape4p = new BTagShapeInterface(btagshape.c_str(), 0.0, 0.0,true,1.003,1.003); 
      upBCShape4p = new BTagShapeInterface(btagshape.c_str(), 0.0, 0.0,true,1.001,1.001); 
      downBCShape4p = new BTagShapeInterface(btagshape.c_str(), 0.0, 0.0,true,1.005,1.005); 
      nominalShape1Bin = new BTagShapeInterface(btagshape.c_str(), 0.0, 0.0,true,1.001,1.001,1); 
    }

  //edm::ESHandle<BtagPerformance> bTagSF_CSVL_;

  
  // Ele MVA ID
  // ------------------------------------------------------------------------------------------------------

  // EGammaMvaEleEstimator* myMVANonTrig = new EGammaMvaEleEstimator();
  //  EGammaMvaEleEstimator* myMVATrig = new EGammaMvaEleEstimator();
  
  //  const string weight_dir = "/afs/cern.ch/cms/data/CMSSW/RecoEgamma/ElectronIdentification/data";
  /*  char* res = getenv("CMSSW_BASE");
      if (res) printf("CMSSW_BASE (via getenv) is %s\n",res);
  
      string cmssw_base = string(res);
      const string weight_dir = cmssw_base + "/src/EGamma/EGammaAnalysisTools/data/";
      bool manualCat = true;


      std::vector<std::string> myManualCatWeigthsTrig;
      myManualCatWeigthsTrig.push_back(weight_dir+"/Electrons_BDTG_TrigV0_Cat1.weights.xml");
      myManualCatWeigthsTrig.push_back(weight_dir+"/Electrons_BDTG_TrigV0_Cat2.weights.xml");
      myManualCatWeigthsTrig.push_back(weight_dir+"/Electrons_BDTG_TrigV0_Cat3.weights.xml");
      myManualCatWeigthsTrig.push_back(weight_dir+"/Electrons_BDTG_TrigV0_Cat4.weights.xml");
      myManualCatWeigthsTrig.push_back(weight_dir+"/Electrons_BDTG_TrigV0_Cat5.weights.xml");
      myManualCatWeigthsTrig.push_back(weight_dir+"/Electrons_BDTG_TrigV0_Cat6.weights.xml");
  
      myMVATrig->initialize("BDT",
      EGammaMvaEleEstimator::kTrig,
      manualCat,
      myManualCatWeigthsTrig);
  */

  string syst_shift = "none";
  string syst_shift_up = "up";
  string syst_shift_down = "down";
  string pathtocfg = "/portal/ekpcms5/home/bmaier/thanalysis/STANDALONE/THAnalysis/sample-info.cfg";
  string datasetpath = "MC_*";
  SampleInfoWeighter* pile_up = new SampleInfoWeighter(pathtocfg,datasetpath, syst_shift);
  SampleInfoWeighter* pile_up_up = new SampleInfoWeighter(pathtocfg,datasetpath, syst_shift_up);
  SampleInfoWeighter* pile_up_down = new SampleInfoWeighter(pathtocfg,datasetpath, syst_shift_down);

 
  //Jet Energy Corrections
  //-------------------------------------------------------------------------------------------------------

  string jecpath = "/portal/ekpcms5/home/cboeser/thanalysis/TEST/THAnalysis/jetcorrs/";
  string jec_uncertainty = "";
  
  vector<JetCorrectorParameters> pars;
  vector<JetCorrectorParameters> l1pars;
  
  if(isData)
    {
      l1pars.push_back(JetCorrectorParameters(jecpath + "FT53_V21A_AN6_L1FastJet_AK5PFchs.txt"));
      pars.push_back(JetCorrectorParameters(jecpath + "FT53_V21A_AN6_L1FastJet_AK5PFchs.txt"));
      pars.push_back(JetCorrectorParameters(jecpath + "FT53_V21A_AN6_L2Relative_AK5PFchs.txt"));
      pars.push_back(JetCorrectorParameters(jecpath + "FT53_V21A_AN6_L3Absolute_AK5PFchs.txt"));
      pars.push_back(JetCorrectorParameters(jecpath + "FT53_V21A_AN6_L2L3Residual_AK5PFchs.txt"));
    }
  else
    {
      l1pars.push_back(JetCorrectorParameters(jecpath + "START53_V27_L1FastJet_AK5PFchs.txt"));
      pars.push_back(JetCorrectorParameters(jecpath + "START53_V27_L1FastJet_AK5PFchs.txt"));
      pars.push_back(JetCorrectorParameters(jecpath + "START53_V27_L2Relative_AK5PFchs.txt"));
      pars.push_back(JetCorrectorParameters(jecpath + "START53_V27_L3Absolute_AK5PFchs.txt"));
      jec_uncertainty = "START53_V27_Uncertainty_AK5PFchs.txt"; // WARNING Unsure if that of data shouldn't be used instead
    }
  
  const boost::shared_ptr<FactorizedJetCorrector> corrector(new FactorizedJetCorrector(pars));
  const boost::shared_ptr<FactorizedJetCorrector> l1corrector(new FactorizedJetCorrector(l1pars));
  
  // For PUID
  //  Int_t i_pumvaidl[80][2]={{},{}}; itree->SetBranchAddress("KITAJets.puIdL[2]", i_pumvaidl);
  //Int_t i_pumvaidm[80][2]={{},{}}; itree->SetBranchAddress("KITAJets.puIdM[2]", i_pumvaidm);
  //Int_t i_pumvaidt[80][2]={{},{}}; itree->SetBranchAddress("KITAJets.puIdT[2]", i_pumvaidt);

  TLeaf* i_pumvaidl = itree->GetLeaf("KITAJets.puIdL");
  TLeaf* i_pumvaidm = itree->GetLeaf("KITAJets.puIdM");
  TLeaf* i_pumvaidt = itree->GetLeaf("KITAJets.puIdT");

  // loop over all events and fill UTM branches
  //---------------------------------------------------------------------------------------------------------
  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  THStatus* status = new THStatus(nevt); 

  cout<<"\033[1m" << nevt<<" events are processed.\033[0m\n";
  
  for (unsigned ievt=0;ievt<nevt;ievt++) {

    if(debugevent.empty()){
      status->print_status(ievt);
      // For debugging:
      // status->get_vmem(ievt%100==0);
    }
    if(debug) cout << "Starting new Event: Nr. " << ievt << endl;

    itree->GetEntry(ievt);

    if(debug) cout << "After GetEvent" << endl;

    //MISCALLENEOUS
    
    run = itree->GetLeaf("nRun")->GetValue(0);
    evt = itree->GetLeaf("nEvent")->GetValue(0);
    lbn = itree->GetLeaf("luminosityBlock")->GetValue(0);
    poissonmean = kitaGenInfo->poisson_mean;


    //Find Heavy-Flavour content for TTbar splitting
    if(debug) cout << "Before HFFinder" << endl;
    if(isTop){
      hffinder->FindHF();
      hfcontent  = hffinder->GetDecision();
      // isUnknown         : 0
      // isTwoBottom       : 1
      // isDoubleBottomJet : 2
      // isSingleBottom    : 3
      // isTwoCharm        : 4
      // isDoubleCharmJet  : 5
      // isSingleCharm     : 6
      // isUnmatchedBottom : 7
      // isUnmatchedCharm  : 8
      // isNoHF            : 9
    }

    //KITAGenParticle & test = mykitagenstatus2->at(0);
    
    // cout << "FEST " << test.pid << "  of total " << mykitagenstatus2->size() << endl;
    
    bool debevt = true; 
    if(!debugevent.empty()){
      debevt = false;
      for(UInt_t deb=0; deb<debugevent.size(); deb++){
	if(evt == (UInt_t)debugevent.at(deb) || debugevent.at(deb)< 0){
	  debevt = true;
	  cout << "----------------------------------------------------------" << endl;
	  cout << "Debugging Event " << evt << ", Run " << run << ", LumiSection " << lbn << endl;
	}
      }
    }

    if(!debevt) continue;

    if(!debugevent.empty()){
      KITAPrimaryVertex & tmpvertex = kitapvs->at(0);
      cout << "1.PV : isFake " << tmpvertex.isFake << " : ndof " << tmpvertex.ndof << " : Z " << fabs(tmpvertex.z) << " : Rho " << tmpvertex.rho << endl;
    }
    
    passpresel=true;

    if(debug) cout << "Before Preselection" << endl;

    // HBHE, OptionalMETFilters, Scraping
    passpresel=Preselection(*mykitamisc, *mykitadatacleaning, isData);
        
    if(PrimaryVertexCut(*kitapvs)==false)   {
      //cout << "PV TEST: " << PrimaryVertexCut(*kitapvs) <<endl;
      npv=0;
    }
    else 
      npv = NGoodPV(*kitapvs);

    //PUWEIGHTS
    if (!isData) 
      {
	weightPU      = pile_up->getWeights1D();
	weightPU_up   = pile_up_up->getWeights1D();
	weightPU_down = pile_up_down->getWeights1D();
      }
    else 
      {
	weightPU      = 1;
	weightPU_up   = 1;
	weightPU_down = 1;
      }
    
    if(debug) cout << "Before TriggerFinder" << endl;

    hlt_isomu24_eta2p1=trig_finder("HLT_IsoMu24_eta2p1_v");
    hlt_isomu24=false;
    hlt_isomu24=trig_finder("HLT_IsoMu24_v");
    hlt_ele27_wp80=trig_finder("HLT_Ele27_WP80_v");


    hlt_mu24_centralpfjet30_centralpfjet25=trig_finder("HLT_Mu24_CentralPFJet30_CentralPFJet25_v");
    hlt_mu24_eta2p1_centralpfjet30_centralpfjet25=trig_finder("HLT_Mu24_eta2p1_CentralPFJet30_CentralPFJet25_v");
    hlt_mu18_centralpfjet30_centralpfjet25=trig_finder("HLT_Mu18_CentralPFJet30_CentralPFJet25_v");
    
    //MET

    metsig = itree->GetLeaf("METsig")->GetValue(0);
    metsumet = itree->GetLeaf("sumEt")->GetValue(0);
    met = mykitapfmet->vec.Pt(); // Sqrt(x2 + y2) 
    uncorrmet = mykitapfmet->uncorr_vec.Pt();
    metphi= mykitapfmet->vec.Phi();
    uncorrmetphi = mykitapfmet->uncorr_vec.Phi();
    meteta= mykitapfmet->vec.Eta();
    metx = mykitapfmet->vec.Px();
    mety = mykitapfmet->vec.Py();
    double save_met = met;
    double save_metphi = metphi;
    double save_metx = metx;
    double save_mety = mety;
    KITA4Vector* save_mykitapfmet= new KITA4Vector();
    save_mykitapfmet->SetPxPyPzE( mykitapfmet->vec.Px(), mykitapfmet->vec.Py(), mykitapfmet->vec.Pz(), mykitapfmet->vec.E());

    //JETS
    if(debug) cout << "Before Jets" << endl;
    std::vector<std::vector<BTagWeight::JetInfo> > btagJetInfos;
    BTagSampleEfficiency btagEff( "/portal/ekpcms5/home/cboeser/thanalysis/TEST/THAnalysis/btag_generic.txt" ); 
    
    Int_t n_jet=0;
    
    njt=0;

    double rho;
   
    if(mykitamisc->rhos.find(KITAMisc::pf_nopileup_kt6)!=mykitamisc->rhos.end())
      {
	rho = mykitamisc->rhos[KITAMisc::pf_nopileup_kt6];

      }
    else
      {
	if(mykitamisc->rhos.find(KITAMisc::pf_kt6)==mykitamisc->rhos.end())
	  {
	    throw string("JetRecorrector: no rho available!");
	  }
	rho = mykitamisc->rhos[KITAMisc::pf_kt6];
       
      }

    evtrho = rho;

    //FILL GenJets
    if(gen_jets->size()>0){
      Float_t pt_temp[140],eta_temp[140],e_temp[140],phi_temp[140];
      Int_t temp_ngenjt15=0;
      Int_t temp_ngenjt=0;
      Int_t temp_ngenjt5=0;
      for(unsigned int i=0; i<gen_jets->size(); i++){
	KITA4Vector helper = gen_jets->at(i).vec;
	pt_temp[i]=helper.Pt();
	eta_temp[i]=helper.Eta();
	phi_temp[i]=helper.Phi();
	e_temp[i]=helper.E();
	temp_ngenjt++;
	if(pt_temp[i] > 15.) temp_ngenjt15++;
	if(pt_temp[i] > 5.) temp_ngenjt5++;
      }
      ngenjt15=temp_ngenjt15;
      ngenjt5=temp_ngenjt5;
      ngenjt=temp_ngenjt;
     
      vector<size_t> indices;
      indices.resize(ngenjt5);
      for(Int_t i=0; i<ngenjt5; ++i){
	indices[i] = i;
      }
      sort(indices.begin(), indices.end(), GreaterByPt(pt_temp));
      
      for(int i=0; i<ngenjt5; i++)
	{
	  genjtpt[i]       =   pt_temp[indices[i]];
	  genjteta[i]      =   eta_temp[indices[i]];
	  genjtphi[i]      =   phi_temp[indices[i]];
	  genjte[i]        =   e_temp[indices[i]];
	}
    }
    else{
      ngenjt15=0;
      ngenjt5=0;
      ngenjt=0;
    }


    // For MET Sys
    vector<TVector2> clusteredmet; 

    if(jets->size()>0)
      {
	Float_t rawpt_temp[140],pt_temp[140],eta_temp[140],e_temp[140],phi_temp[140],csvt_temp[140],area_temp[140],pull_temp[140],ntracks_temp[140],charge_temp[140],id_temp[140],chhadmult_temp[140],genflv_temp[140],genpt_temp[140],geneta_temp[140],genphi_temp[140],gene_temp[140], jerfac_temp[140], pumva_temp[140],  ndaughters_temp[140],chmult_temp[140],nhadronfrac_temp[140],hfhadronfrac_temp[140],nemfrac_temp[140],chemfrac_temp[140],chhadronfrac_temp[140]; 
	Bool_t puid_temp[6][140];
	Float_t jerfac=0.;

	sumHt=0;

	for(unsigned int i=0; i<jets->size(); i++)
	  {
	    //RECONSTRUCTED JETS

	    //JET ENERGY CORRECTIONS

	    KITAJet & jet = jets->at(i);
	    
	    const KITA4Vector  jetvec_raw = jet.vec * jet.jec_factors[KITAJet::RAW];	    
	    
	    const KITA4Vector jetvec_old_l1 = jet.vec * jet.jec_factors[KITAJet::L1];
	    	    
	    // Handle Type-I corrections;
	    // see https://twiki.cern.ch/twiki/bin/view/CMS/METType1Type2Formulae
	    const bool jet_was_used_for_type1 = jet.vec.Pt() > 10;
	    
	    if(jet_was_used_for_type1)
	      {
		// subtract correction for this jet from MET, in form corresponding to type-I MET correction
		// (difference between fully corrected form and L1)
		metx += jet.vec.Px() - jetvec_old_l1.Px();
		mety += jet.vec.Py() - jetvec_old_l1.Py();
		met = TMath::Sqrt(metx*metx+mety*mety);
	      }
	    corrector->setJetPt(jetvec_raw.Pt());
	    corrector->setJetEta(jetvec_raw.Eta());
	    corrector->setJetE(jetvec_raw.E());
	    corrector->setJetA(jet.area);
	    corrector->setRho(rho);
	    	    	    
	    const float factor = corrector->getCorrection();	    	    
	    const KITA4Vector jetvec_corrected = jetvec_raw * factor;
	    KITA4Vector helper_smearedjet;

	    jet.vec = jetvec_corrected;
	    jet.jec_factors.clear();
	    jet.jec_factors[KITAJet::RAW] = 1.0/factor;
	    
	    // perform l1 correction by itself for type-I met correction
	    l1corrector->setJetPt(jetvec_raw.Pt());
	    l1corrector->setJetEta(jetvec_raw.Eta());
	    l1corrector->setJetE(jetvec_raw.E());
	    l1corrector->setJetA(jet.area);
	    l1corrector->setRho(rho);
	    const float l1factor = l1corrector->getCorrection();
	    const KITA4Vector jetvec_l1corrected = jetvec_raw * l1factor;
	    jet.jec_factors[KITAJet::L1] = l1factor/factor;
	    
	    // Again, handle Type-I corrections
	    const bool jet_should_be_used_for_type1 = jet.vec.Pt() > 10;
	    if(jet_should_be_used_for_type1)
	      {
		// add back correction for this jet to MET, in form corresponding to type-I MET correction
		// (difference between fully corrected form and L1)
		metx -= jet.vec.Px() - jetvec_l1corrected.Px();
		mety -= jet.vec.Py() - jetvec_l1corrected.Py();
		met = TMath::Sqrt(metx*metx+mety*mety);
	      }
	    
	    helper_smearedjet.SetPxPyPzE(jet.vec.Px(),jet.vec.Py(),jet.vec.Pz(),jet.vec.E());

	    //JER SHIFT / SMEARING

	    bool do_jetsmearing = true;
	    
	    if(!isData&&do_jetsmearing)
	      {
		const float gen_pt = jet.genJetVec.Pt();
		const float gen_E = jet.genJetVec.E();
		const float recopt = jet.vec.Pt();
		const float recoE = jet.vec.E();

		if(gen_pt>0 && recopt>=10.0)
		  {
		    
		    KITA4Vector jetvec_raw_JER = jet.vec * jet.jec_factors[KITAJet::RAW];
		    KITA4Vector jetvec_l1_JER = jet.vec * jet.jec_factors[KITAJet::L1];
		    
		    // subtract this jet from MET, in uncorrected form
		    metx += jetvec_raw_JER.Px();
		    mety += jetvec_raw_JER.Py();
		    // Handle Type-I corrections;
		    // see https://twiki.cern.ch/twiki/bin/view/CMS/METType1Type2Formulae
		    const bool jet_was_used_for_type1_JER = jet.vec.Pt() > 10;
		    
		    if(jet_was_used_for_type1_JER)
		      {
			// also subtract difference between L3/L2L3 and L1 correction levels
			metx += jet.vec.Px() - jetvec_l1_JER.Px();
			mety += jet.vec.Py() - jetvec_l1_JER.Py();
		      }
		    
		    double factor_JER = -10;
		    const double abseta_JER = fabs(jet.vec.Eta());
		    
		    // see recommendation https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution#JER_Uncertainty

		    if(abseta_JER < 0.5)
		      factor_JER = 0.052;
		    else if(abseta_JER <1.1)
		      factor_JER = 0.057;
		    else if(abseta_JER <1.7)
		      factor_JER = 0.096;
		    else if(abseta_JER <2.3)
		      factor_JER = 0.134;
		    else
		      factor_JER = 0.288;

		    // different smearing according to https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_11/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h#L273
		    double dEn = recoE - gen_E;
		    const float ptscale_JER = (1. + (factor_JER)*dEn/TMath::Max(jetvec_raw_JER.E(), recoE));
		    //   const float deltapt_JER = (recopt - gen_pt) * factor_JER;
		    //   const float ptscale_JER = std::max(0.0f, (recopt + deltapt_JER) / recopt);		    

		    jet.vec = ptscale_JER * jet.vec;
		    jerfac = ptscale_JER;
		    
		    // add back the new jet to MET, in uncorrected form
		    metx -= jetvec_raw_JER.Px() * ptscale_JER;
		    mety -= jetvec_raw_JER.Py() * ptscale_JER;
		    
		    // Again, handle Type-I corrections
		    const bool jet_should_be_used_for_type1_JER = jet.vec.Pt() > 10;
		    if(jet_should_be_used_for_type1_JER)
		      {
			// add back difference between L3/L2L3 and L1 correction levels
			// (we assume all levels are smeared by the same ptscale - because we don't know any better, really)
			metx -= jet.vec.Px() - ptscale_JER * jetvec_l1_JER.Px();
			mety -= jet.vec.Py() - ptscale_JER * jetvec_l1_JER.Py();
			met = TMath::Sqrt(metx*metx+mety*mety);
		      }
		  }
       	      } // end smearing

	    if(!debugevent.empty()){
	      if(jet.vec.Pt()>10.){
		TLorentzVector deb_helper;
		deb_helper.SetXYZT(itree->GetLeaf("KITAJets.vec.fX")->GetValue(i), itree->GetLeaf("KITAJets.vec.fY")->GetValue(i), itree->GetLeaf("KITAJets.vec.fZ")->GetValue(i),itree->GetLeaf("KITAJets.vec.fT")->GetValue(i));
		if(isData)
		  cout << "jet " << i << " : pt " <<  jet.vec.Pt() << " : eta " << (jet.vec.Eta()) << " : phi " << (jet.vec.Phi()) << " : mass " << deb_helper.M() << endl;
		else
		  cout << "jet " << i << " : smeared pt " <<  jet.vec.Pt() << " : (pt " << helper_smearedjet.Pt() << ") : eta " << (jet.vec.Eta()) << " : phi " << (jet.vec.Phi()) << " : mass " << deb_helper.M() << endl;
		cout << "      : CSV " << itree->GetLeaf("KITAJets.btag_combSV")->GetValue(i) << " : rawPt " << jetvec_raw.Pt() << " : rawE " << jetvec_raw.E() << endl;
		cout << "      : constit " << jet.pflowvars[0].ChHadronMult + jet.pflowvars[0].NHadronMult + jet.pflowvars[0].EleMult + jet.pflowvars[0].PhMult + jet.pflowvars[0].MuMult + jet.pflowvars[0].HFEmMult + jet.pflowvars[0].HFHadronMult << " : HadFrac " << (jet.pflowvars[0].NHadronEnergy+jet.pflowvars[0].HFHadronEnergy)/jetvec_raw.E() << " : NEmFrac " << jet.pflowvars[0].NEmEnergy/jetvec_raw.E();
		if(fabs(jet.vec.Eta()) < 2.4) 
		  cout<< " : ChEmFrac  " << jet.pflowvars[0].ChEmEnergy/jetvec_raw.E() << " : ChHadFrac " << jet.pflowvars[0].ChHadronEnergy/jetvec_raw.E() << " : ChMult " << jet.pflowvars[0].ChMult;
		cout << " : ID " << PFJetID_2012(jet,jetvec_raw.E()) <<  endl;
		cout << endl;
	      }
	    }
	    
	    // Store jets(pt>10.) for unclustered MET systematic
	    if(jet.vec.Pt()>10.){
	      TVector2 clustmethelper(jet.vec.Px(),jet.vec.Py());
	      clusteredmet.push_back(clustmethelper);
	    }
	    
	    // Fill right raw E for JET_ID
	    //	    rawe_temp[140] = jetvec_raw.E();

	    //CHECK FOR PFJETID_2012
	    if(PFJetID_2012(jet,jetvec_raw.E()))
	      {
		//	    cout << "KITA JET#" << i << " with good ID" << endl;
		TLorentzVector KITAJets_helper;
		
		KITAJets_helper.SetXYZT(itree->GetLeaf("KITAJets.vec.fX")->GetValue(i), itree->GetLeaf("KITAJets.vec.fY")->GetValue(i), itree->GetLeaf("KITAJets.vec.fZ")->GetValue(i),itree->GetLeaf("KITAJets.vec.fT")->GetValue(i));

		rawpt_temp[n_jet]=jetvec_raw.Pt();
		pt_temp[n_jet]=jet.vec.Pt();
		eta_temp[n_jet]=jet.vec.Eta();
		phi_temp[n_jet]=jet.vec.Phi();
		e_temp[n_jet]=jet.vec.E();
		//		cout << evt << " " << i << " => mvaidl: " << i_pumvaidt->GetValue(1) << " | cutidl: " << i_pumvaidt->GetValue(0) <<  endl;
		puid_temp[0][n_jet]=i_pumvaidl->GetValue(0);
		puid_temp[1][n_jet]=i_pumvaidl->GetValue(1);
		puid_temp[2][n_jet]=i_pumvaidm->GetValue(0);
		puid_temp[3][n_jet]=i_pumvaidm->GetValue(1);
		puid_temp[4][n_jet]=i_pumvaidt->GetValue(0);
		puid_temp[5][n_jet]=i_pumvaidt->GetValue(1);
		pumva_temp[n_jet] = itree->GetLeaf("KITAJets.puIdMVA")->GetValue(i);
		jerfac_temp[n_jet]= jerfac;
		csvt_temp[n_jet]=itree->GetLeaf("KITAJets.btag_combSV")->GetValue(i);
		ntracks_temp[n_jet]=itree->GetLeaf("KITAJets.nTracks")->GetValue(i);
		area_temp[n_jet]=jet.area;
		pull_temp[n_jet]=jet.pullAngle;
		charge_temp[n_jet]=itree->GetLeaf("KITAJets.jetCharge")->GetValue(i);
		id_temp[n_jet]=PFJetID_2012(jets->at(i),jetvec_raw.E());
		chhadmult_temp[n_jet]=jets->at(i).pflowvars[0].ChHadronMult;

		ndaughters_temp[n_jet]=jets->at(i).nDaughters;
		chmult_temp[n_jet]=jets->at(i).pflowvars[0].ChMult;
		nhadronfrac_temp[n_jet]=(jets->at(i).pflowvars[0].NHadronEnergy)/jetvec_raw.E();
		hfhadronfrac_temp[n_jet]=(jets->at(i).pflowvars[0].HFHadronEnergy)/jetvec_raw.E();
		nemfrac_temp[n_jet]=(jets->at(i).pflowvars[0].NEmEnergy)/jetvec_raw.E();
		chemfrac_temp[n_jet]=(jets->at(i).pflowvars[0].ChEmEnergy)/jetvec_raw.E();
		chhadronfrac_temp[n_jet]=(jets->at(i).pflowvars[0].ChHadronEnergy)/jetvec_raw.E();

		sumHt += pt_temp[n_jet];		
		
		//GENERATED JETS
		genflv_temp[n_jet]=itree->GetLeaf("KITAJets.partonFlav")->GetValue(i);
		
		if(itree->GetLeaf("KITAJets.genJetVec.fX")->GetValue(i)!=0)
		  {
		    TLorentzVector KITAGenJets_helper;
		    
		    KITAGenJets_helper.SetXYZT(itree->GetLeaf("KITAJets.genJetVec.fX")->GetValue(i), itree->GetLeaf("KITAJets.genJetVec.fY")->GetValue(i), itree->GetLeaf("KITAJets.genJetVec.fZ")->GetValue(i),itree->GetLeaf("KITAJets.genJetVec.fT")->GetValue(i));   
		    genpt_temp[n_jet]=KITAGenJets_helper.Pt();
		    geneta_temp[n_jet]=KITAGenJets_helper.Eta();
		    genphi_temp[n_jet]=KITAGenJets_helper.Phi();
		    gene_temp[n_jet]=KITAGenJets_helper.E();
		  }
		
		else	  
		  {
		    genpt_temp[n_jet]=0;
		    geneta_temp[n_jet]=0;
		    genphi_temp[n_jet]=0;
		    gene_temp[n_jet]=0;		    
		  }	
		
		n_jet++;
	      }
	    
	  }
	njt15=n_jet;
		
	vector<size_t> indices;
	indices.resize(njt15);
	for(Int_t i=0; i<njt15; ++i){
	  indices[i] = i;
	}

	sort(indices.begin(), indices.end(), GreaterByPt(pt_temp));



	//cout << "Here 4" << endl;

	//FINAL FILL FOR JETS
	
	for(int i=0; i<njt15; i++)
	  {
	    jtrawpt[i]        =   rawpt_temp[indices[i]];
	    jtpt[i]           =   pt_temp[indices[i]];
	    if(jtpt[i]>20)
	      njt++;
	    jteta[i]          =   eta_temp[indices[i]];
	    jtphi[i]          =   phi_temp[indices[i]];
	    jte[i]            =   e_temp[indices[i]];
	    jtcsvt[i]         =   csvt_temp[indices[i]];
	    jtntracks[i]      =   ntracks_temp[indices[i]];
	    jtarea[i]         =   area_temp[indices[i]];
	    jtpull[i]         =   pull_temp[indices[i]];
	    jtcharge[i]       =   charge_temp[indices[i]];
	    jtpumvaidl[i]     =   puid_temp[1][indices[i]];
	    jtpumvaidm[i]     =   puid_temp[3][indices[i]];
	    jtpumvaidt[i]     =   puid_temp[5][indices[i]];
	    jtpucutidl[i]     =   puid_temp[0][indices[i]];
	    jtpucutidm[i]     =   puid_temp[2][indices[i]];
	    jtpucutidt[i]     =   puid_temp[4][indices[i]];
	    jtpuidmva[i]      =   pumva_temp[indices[i]];
 
	    jtid[i]           =   id_temp[indices[i]];
	    jtchhadmult[i]    =   chhadmult_temp[indices[i]];
	    jtndaughters[i]   =  ndaughters_temp[indices[i]];
	    jtchmult[i]       = chmult_temp[indices[i]];
	    jtnhadronfrac[i]  = nhadronfrac_temp[indices[i]];
	    jthfhadronfrac[i] = hfhadronfrac_temp[indices[i]];
	    jtnemfrac[i]      = nemfrac_temp[indices[i]];
	    jtchemfrac[i]     = chemfrac_temp[indices[i]];
	    jtchhadronfrac[i] = chhadronfrac_temp[indices[i]]; 
	    jtjer[i]          =   jerfac_temp[indices[i]];     
	    
	    jtgenflv[i]       =   genflv_temp[indices[i]];
	    jtgenpt[i]        =   genpt_temp[indices[i]];
	    jtgeneta[i]       =   geneta_temp[indices[i]];
	    jtgenphi[i]       =   genphi_temp[indices[i]];
	    jtgene[i]         =   gene_temp[indices[i]];

	    if(!isData)
	      {
		jtcsvtcorr[i][0]  =   nominalShape->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
		jtcsvtcorr[i][1]  =   upBCShape->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
		jtcsvtcorr[i][2]  =   downBCShape->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
		jtcsvtcorr[i][3]  =   upLShape->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
		jtcsvtcorr[i][4]  =   downLShape->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
		
		jtcsvtcorr4p[i][0]=   nominalShape4p->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
		jtcsvtcorr4p[i][1]=   upBCShape4p->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
		jtcsvtcorr4p[i][2]=   downBCShape4p->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
		jtcsvtcorr4p[i][3]=   nominalShape1Bin->reshape(jteta[i],jtpt[i],jtcsvt[i],jtgenflv[i]);
	      }
	    else
	      {
		jtcsvtcorr[i][0]  = jtcsvt[i];
		jtcsvtcorr[i][1]  = jtcsvt[i];
		jtcsvtcorr[i][2]  = jtcsvt[i];
		jtcsvtcorr[i][3]  = jtcsvt[i];
		jtcsvtcorr[i][4]  = jtcsvt[i];

		jtcsvtcorr4p[i][0]  = jtcsvt[i];
		jtcsvtcorr4p[i][1]  = jtcsvt[i];
		jtcsvtcorr4p[i][2]  = jtcsvt[i];
		jtcsvtcorr4p[i][3]  = jtcsvt[i];
	      }
	    float sfcsvl=-1;
	    float sfcsvm=-1;
	    float sfcsvt=-1;

	    fillScaleFactors(jtpt[i],jteta[i],jtgenflv[i], sfcsvl, sfcsvm, sfcsvt);
	    if(sfcsvl==-1 || sfcsvm==-1 || sfcsvt==-1){return 0;}
	    btagJetInfos.push_back(btagEff.jetInfo(jtgenflv[i], sfcsvm, sfcsvt));
	  }
		
      }
    else 
      {
	njt15=0;
	njt=0;
      }
    if(debug) cout << "Befor Btag" << endl;
    BTagWeight btag(2);
    if(!isData)
      {
	if ( njt15 < 11) 
	  {
	    btag2MSF = btag.weight<BTag2MediumFilter>(btagJetInfos);
	    btag2TSF = btag.weight<BTag2TightFilter>(btagJetInfos);
	    btag3MSF = btag.weight<BTag3MediumFilter>(btagJetInfos);
	    btag3TSF = btag.weight<BTag3TightFilter>(btagJetInfos);
	    btag4MSF = btag.weight<BTag4MediumFilter>(btagJetInfos);
	    btag4TSF = btag.weight<BTag4TightFilter>(btagJetInfos);
	  }
	else
	  {
	    if(debug) std::cout << "WARNING:  combinatorics for " << njt15 << " jets is too high (>=11). use SF=1 " << std::endl;
	    //TODO: revert to random throw  for this cases
	    btag2MSF = 1.;
	    btag2TSF = 1.;
	    btag3MSF = 1.;
	    btag3TSF = 1.;
	    btag4MSF = 1.;
	    btag4TSF = 1.;
	  }
      }

    //METPhiCorrection

    uint nvertex = 0;

    for(uint i=0; i<kitapvs->size(); ++i)
      {
	const KITAPrimaryVertex & vertex = kitapvs->at(i);
	if( vertex.isFake ) continue;
	if( vertex.ndof < 4 ) continue;
	if( vertex.redChi2 <= 0 ) continue; // reduced-chisquare = chisquare/ndof. we can use it directly and
	// instead of chisquare because we compare against zero - where it
	// doesn't make a difference
	if( vertex.ntracks <= 0 ) continue;
	if( fabs(vertex.z) >= 24 ) continue;
	if( vertex.rho >= 2 ) continue;
	
	++nvertex;
      }

    //    cout << met << endl; 
    //cout << metphi << endl;
    //cout << metx << endl;
    //cout << mety << endl;
    if(!debugevent.empty()){
      cout << "MET before phi correction: " <<  met << " : phi : " << metphi << " : x : " << metx <<  " : y : " << mety << endl << endl;
    }
    // changed according to https://github.com/cms-sw/cmssw/blob/CMSSW_7_0_X/JetMETCorrections/Type1MET/python/pfMETsysShiftCorrections_cfi.py

    double met_temp;
    met_temp  = met;

    if(isData)
      { // similar to pfMEtSysShiftCorrParameters_2012runABCDvsNvtx_data
	metx -= +4.83642e-02 + (2.48870e-01 * nvertex);
	mety -= -1.50135e-01 - (8.27917e-02 * nvertex);
	met = TMath::Sqrt(metx*metx+mety*mety);
      }
    else
      { // similar to pfMEtSysShiftCorrParameters_2012runABCDvsNvtx_mc
	metx -= +1.62861e-01 - (2.38517e-02 * nvertex);
	mety -= +3.60860e-01 - (1.30335e-01 * nvertex);
	met = TMath::Sqrt(metx*metx+mety*mety);
      }

    mykitapfmet->vec.SetPx(metx);
    mykitapfmet->vec.SetPy(mety);
    metphi=mykitapfmet->vec.Phi();
    
    if(!debugevent.empty()){
      cout << "Rho : " << rho << endl << endl;
      cout << "MET : " << uncorrmet << " : phi : " << uncorrmetphi/TMath::Pi()*180 << endl << endl;
      cout << "MET type-0, type 1 corrected : " << save_met << " : phi : " << save_metphi/TMath::Pi()*180 << " : x : " << save_metx <<  " : y : " << save_mety <<  " : sumet : " << metsumet << endl;
      cout << "MET type-0, type 1 and phi corrected : " <<  met << " : phi : " << metphi/TMath::Pi()*180 << " : x : " << metx <<  " : y : " << mety << endl << endl;
      cout << "old metsig: " << metsig << " : met factor : " << met/met_temp << " : new metsig : " << metsig*met/met_temp << endl << endl; 
    }

    metsig_old = metsig;
    metsig = metsig*met/met_temp;

    //Unclustered MET (Met - Jets(Pt>10.)) Shift
    metsysx[0] = metx;
    metsysy[0] = mety;
    metsysx[1] = metx;
    metsysy[1] = mety;

    for(size_t i=0; i<clusteredmet.size(); i++){
      metsysx[0] += clusteredmet.at(i).X();
      metsysy[0] += clusteredmet.at(i).Y();
      metsysx[1] += clusteredmet.at(i).X();
      metsysy[1] += clusteredmet.at(i).Y();
    }

    metsysx[0] *= 1.10;
    metsysy[0] *= 1.10;
    metsysx[1] *= 0.90;
    metsysy[1] *= 0.90;     
  

    for(size_t i=0; i<clusteredmet.size(); i++){
      metsysx[0] -= clusteredmet.at(i).X();
      metsysy[0] -= clusteredmet.at(i).Y();
      metsysx[1] -= clusteredmet.at(i).X();
      metsysy[1] -= clusteredmet.at(i).Y();
    }

    metsys[0] = TMath::Sqrt(metsysx[0]*metsysx[0]+metsysy[0]*metsysy[0]);
    metsys[1] = TMath::Sqrt(metsysx[1]*metsysx[1]+metsysy[1]*metsysy[1]);
    

    //LEPTONS 
    if(debug) cout << "Before Leptons" << endl;

    if(mykitamisc->rhos.find(KITAMisc::pf_neutral_kt6_eta25)==mykitamisc->rhos.end())
      {
	throw std::string("RelIso Mu: Needed rho not available.");
      }
    
    if(debug) cout << "Leptons: Before calculating definitions" << endl;
    //Calculate necessary quantities to define muons
    const float rho_mu = std::max(0.f,mykitamisc->rhos[KITAMisc::pf_neutral_kt6_eta25]);
    float pv_z=kitapvs->at(0).z;
    int pv_size=kitapvs->size();

    if(debug) cout << "Leptons: Before getting muons" << endl;    
    nlmu = itree->GetLeaf("KITAMuonsLoose.vec.fX")->GetNdata();
    Int_t posnvetomu = -1;
    

    
    for(Int_t i=0; i<nlmu; i++)
      {

	if(debug) cout << "Leptons: Before filling muon quantities" << endl;
	//cout << "PF Muon: " << mykitamuonsloose->at(i).isPFMuon << endl;
	TLorentzVector KITAMuons_helper;
	KITAMuons_helper.SetXYZT(itree->GetLeaf("KITAMuonsLoose.vec.fX")->GetValue(i), itree->GetLeaf("KITAMuonsLoose.vec.fY")->GetValue(i), itree->GetLeaf("KITAMuonsLoose.vec.fZ")->GetValue(i),itree->GetLeaf("KITAMuonsLoose.vec.fT")->GetValue(i));
	lmupt[i]=KITAMuons_helper.Pt();
	lmueta[i]=KITAMuons_helper.Eta();
	lmuphi[i]=KITAMuons_helper.Phi();
	lmue[i]=KITAMuons_helper.E();

	// Muon relative isolation, including delta beta correction. The TreeRunnerRecord is only for API consistent with electron case.
	// compare https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId#Muon_Isolation
	lmuiso[i]=(std::max(0.,(itree->GetLeaf("KITAMuonsLoose.pf_iso_photon")->GetValue(i)+itree->GetLeaf("KITAMuonsLoose.pf_iso_neutrals")->GetValue(i)- rho_mu *muAEff(lmueta[i])))+itree->GetLeaf("KITAMuonsLoose.pf_iso_charged")->GetValue(i))/lmupt[i];

	lmuiso_dBeta[i]=(std::max(0.,(itree->GetLeaf("KITAMuonsLoose.pf_iso_photon")->GetValue(i)+itree->GetLeaf("KITAMuonsLoose.pf_iso_neutrals")->GetValue(i)- 0.5 *itree->GetLeaf("KITAMuonsLoose.pf_iso_sumPUPt")->GetValue(i)))+itree->GetLeaf("KITAMuonsLoose.pf_iso_charged")->GetValue(i))/lmupt[i];


	lmucharge[i]=mykitamuonsloose->at(i).charge;
	musigid[i]=0;
	muvetoid[i]=0;
	musigid[i]=MuID_2012(mykitamuonsloose->at(i),lmuiso_dBeta[i],pv_z,pv_size);
	muvetoid[i]=MuVetoID_2012(mykitamuonsloose->at(i),lmuiso_dBeta[i]);

	if(!debugevent.empty()){
	  cout << "muon " << i << " : pt " <<  lmupt[i] << " : eta " << lmueta[i] << " : phi " << lmuphi[i] << " : mass " << itree->GetLeaf("KITAMuonsLoose.mass")->GetValue(i)  << endl;
	  cout << "       : reliso " << lmuiso_dBeta[i] << " : ChHad " << itree->GetLeaf("KITAMuonsLoose.pf_iso_charged")->GetValue(i)  << " : NHad " << itree->GetLeaf("KITAMuonsLoose.pf_iso_neutrals")->GetValue(i)  <<" : Photons " << itree->GetLeaf("KITAMuonsLoose.pf_iso_photon")->GetValue(i)  << " : ChPU " << itree->GetLeaf("KITAMuonsLoose.pf_iso_sumPUPt")->GetValue(i)  << " : ID " << musigid[i] << " : vetoID " << muvetoid[i] << endl;
	}
      }
    
    Int_t niso_mu = 0;
    Int_t nveto_mu = 0;

    for(Int_t i=0; i<nlmu; i++)
      {
	if(musigid[i])
	  {
	    mupt[niso_mu]=lmupt[i];
	    mueta[niso_mu]=lmueta[i];
	    muphi[niso_mu]=lmuphi[i];
	    mue[niso_mu]=lmue[i];
	    mucharge[niso_mu]=lmucharge[i];
	    muiso[niso_mu]=lmuiso_dBeta[i];
	    niso_mu++;
	  }
	if(muvetoid[i]){
	  nveto_mu++;
	  // Save position of hardest nvetomu 
	  if(posnvetomu == -1)
	    posnvetomu = i;
	}
      }
    
    nmu=niso_mu;
    nvetomu = nveto_mu;

    if(mykitamisc->rhos.find(KITAMisc::pf_kt6)==mykitamisc->rhos.end())
      {
	throw std::string("RelIso Ele: Needed rho not available.");
      }
    if(debug) cout << "Leptons: Before getting Electrons" << endl;
    const float rho_el = mykitamisc->rhos[KITAMisc::pf_kt6];
    nlel = itree->GetLeaf("KITAElectronsLoose.vec.fX")->GetNdata();
    
    Int_t posnvetoel = -1;
    for(Int_t i=0; i<nlel; i++)
      {
	if(debug) cout << "Leptons: Before filling electron quantities" << endl;
	TLorentzVector KITAElectrons_helper;
	KITAElectrons_helper.SetXYZT(itree->GetLeaf("KITAElectronsLoose.vec.fX")->GetValue(i), itree->GetLeaf("KITAElectronsLoose.vec.fY")->GetValue(i), itree->GetLeaf("KITAElectronsLoose.vec.fZ")->GetValue(i),itree->GetLeaf("KITAElectronsLoose.vec.fT")->GetValue(i));
        if(debug) cout << "Leptons: Before filling electron pt" << endl;
	lelpt[i]=KITAElectrons_helper.Pt();
	leleta[i]=KITAElectrons_helper.Eta();
	lelphi[i]=KITAElectrons_helper.Phi();
	lele[i]=KITAElectrons_helper.E();
	lelcharge[i]=mykitaelectronsloose->at(i).charge;
	// Electron relative isolation. Needs effective area correction according to
	// https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaEARhoCorrection
        if(debug) cout << "Leptons: Before filling electron iso" << endl;
	leliso[i]=(std::max(0.,itree->GetLeaf("KITAElectronsLoose.pf_iso_photon")->GetValue(i)+itree->GetLeaf("KITAElectronsLoose.pf_iso_neutrals")->GetValue(i)-1.0*itree->GetLeaf("KITAElectronsLoose.pf_iso_user")->GetValue(i))+itree->GetLeaf("KITAElectronsLoose.pf_iso_charged")->GetValue(i))/lelpt[i];
	leliso2[i]=(std::max(0.,itree->GetLeaf("KITAElectronsLoose.pf_iso_photon")->GetValue(i)+itree->GetLeaf("KITAElectronsLoose.pf_iso_neutrals")->GetValue(i)-rho_el*eleAEff(itree->GetLeaf("KITAElectronsLoose.sclEta")->GetValue(i)))+itree->GetLeaf("KITAElectronsLoose.pf_iso_charged")->GetValue(i))/lelpt[i];
	if(debug) cout << "Leptons: Before filling electron mva" << endl;
	if(checkElectronMVAIDPreselection(mykitaelectronsloose->at(i))){	  
	  lelmvaid[i]=itree->GetLeaf("KITAElectronsLoose.mvaTrigV0")->GetValue(i);
	  /*lelmvaid[i]= myMVATrig->mvaValue(itree->GetLeaf("KITAElectronsLoose.myMVAVar_fbrem")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_kfchi2")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_kfhits")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_gsfchi2")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_deta")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_dphi")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_detacalo")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_see")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_spp")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_etawidth")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_phiwidth")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_e1x5e5x5")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_R9")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_HoE")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_EoP")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_IoEmIoP")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_eleEoPout")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_PreShowerOverRaw")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_d0")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_ip3d")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_eta")->GetValue(i),
	    itree->GetLeaf("KITAElectronsLoose.myMVAVar_pt")->GetValue(i),
	    false // debug yes/no
	    );*/
	}
	else { 
	  lelmvaid[i]=-99; 
	}
	const double probMin = 1e-6;
	const double lxyMin = 2.0;
	const int nHitsBeforeVtxMax = 0;
	
	KITAConversionInfos::const_iterator it = mykitaelectronsloose->at(i).conversionInfos.begin();
	KITAConversionInfos::const_iterator end = mykitaelectronsloose->at(i).conversionInfos.end();
	
	if(mykitaelectronsloose->at(i).conversionInfos.size()==0){   
	  lelconv[i]=1; 
	}
	if(debug) cout << "Leptons: Before ele iterator conversionINfo" << endl;
	for (   ; it!=end; it++) {
	  if (!(it->vtxProb>probMin &&  it->lxy>lxyMin && it->nHitsMax<=nHitsBeforeVtxMax)) { 
	    lelconv[i]=1;
	  }
	  else {
	    lelconv[i]=0;  
	  }
	}
	if(debug) cout << "Leptons: Before asking ConversionVeto" << endl;
	if(lelconv[i] != mykitaelectronsloose->at(i).passConversionVeto){
	  //cout << lelconv[i] << " != " << mykitaelectronsloose->at(i).passConversionVeto << endl;
	}
	
	if(!debugevent.empty()){
	  cout << "elec " << i << " : pt " <<  lelpt[i] << " : eta " << leleta[i] << " : phi " << lelphi[i] << " : mass " << itree->GetLeaf("KITAElectronsLoose.mass")->GetValue(i) << endl;
	  cout << "       : reliso " << leliso[i] << " : ChHad " << itree->GetLeaf("KITAElectronsLoose.pf_iso_charged")->GetValue(i)  << " : NHad " << itree->GetLeaf("KITAElectronsLoose.pf_iso_neutrals")->GetValue(i)  <<" : Photons " << itree->GetLeaf("KITAElectronsLoose.pf_iso_photon")->GetValue(i)  << " : PU Corr " << itree->GetLeaf("KITAElectronsLoose.pf_iso_user")->GetValue(i)  << endl;
	  cout << "       : TrigPreSel " << checkElectronMVAIDPreselection(mykitaelectronsloose->at(i)) << " : TkSumPt/pt " << (mykitaelectronsloose->at(i).dr03TkSumPt)/lelpt[i] << " : HcalTowerSumEt/pt " << (mykitaelectronsloose->at(i).dr03HcalTowerSumEt)/lelpt[i] << " : EcalRecHitSumEt/pt " << (mykitaelectronsloose->at(i).dr03EcalRecHitSumEt)/lelpt[i] << " : corrECALIso/pt " << (mykitaelectronsloose->at(i).dr03EcalCorrected)/lelpt[i] << endl;
	  cout << "       : NoLostHits " << (mykitaelectronsloose->at(i).trInnerNoOfLostHits) << " : sclEta " << mykitaelectronsloose->at(i).sclEta << " : sigmaIetaIeta " << mykitaelectronsloose->at(i).sigmaIetaIeta << " : hadOverEm " << mykitaelectronsloose->at(i).hadOverEm << endl;
	  cout << "       : MVA " << lelmvaid[i] << endl << endl;
	}
	
      }

    //Calculate number of isolated & loose electrons
    Int_t niso_el = 0; 
    Int_t nveto_el = 0;

    for(Int_t i=0; i<nlel; i++)
      {
	
	elvetoid[i]=0;
	elsigid[i]=0;
	
	elvetoid[i]=EleVetoID_2012(mykitaelectronsloose->at(i),leliso[i], lelmvaid[i]);
	elsigid[i]=EleID_2012(mykitaelectronsloose->at(i),leliso[i], lelmvaid[i]);
	
	if(elsigid[i])
	  {
	    elpt[niso_el]=lelpt[i];
	    eleta[niso_el]=leleta[i];
	    elphi[niso_el]=lelphi[i];
	    ele[niso_el]=lele[i];
	    eliso[niso_el]=leliso[i];
	    elmvaid[niso_el]=lelmvaid[i];
	    elcharge[niso_el]=lelcharge[i];
	    niso_el++;
	  }
	if(elvetoid[i]){
	  nveto_el++;
	  // Save position of hardest nvetoel
	  if(posnvetoel == -1)
	    posnvetoel = i;
	}
      }
    
    nvetoel=nveto_el;
    nel=niso_el;

    Float_t weightMu=1, weightEl=1, weightScaleEl=1, weightScaleMu=1, weightMuFinal=1, weightElFinal=1;

    weightMu = triggerWeight.singleMuon2012A(mupt[0],mueta[0]);
    weightScaleMu = triggerWeight.scaleMuID(mupt[0],mueta[0]);
    weightEl = triggerWeight.singleEle2012Awp80(elpt[0],eleta[0]);
    weightScaleEl = triggerWeight.eleId2012Awp80(mupt[0],mueta[0]);

    weightElFinal = weightEl * weightScaleEl;
    weightMuFinal = weightMu * weightScaleMu;


    if(nmu!=0){
      weightTrig = weightMuFinal;
    }
    else { weightMu = 1; weightScaleMu = 1; weightMuFinal =1;}

    if(nel!=0){
      weightTrig = weightElFinal;
    }
    else { weightEl = 1; weightScaleEl = 1; weightElFinal =1; }
    
    if( nel==0 && nmu == 0) weightTrig = 1; 

    
    //MC TRUTH
    
    //Higgs part
    if(debug) cout << "Before MC Truth" << endl;
    if(isSignal)
      {
	TLorentzVector Higgs_helper; //Higgs boson
	Higgs_helper.SetXYZT(itree->GetLeaf("higgs.vec.fX")->GetValue(0), itree->GetLeaf("higgs.vec.fY")->GetValue(0), itree->GetLeaf("higgs.vec.fZ")->GetValue(0),itree->GetLeaf("higgs.vec.fT")->GetValue(0));
	
	Hpt = Higgs_helper.Pt();
	Heta = Higgs_helper.Eta();
	Hphi = Higgs_helper.Phi();
	Hm = Higgs_helper.M();
	
	TLorentzVector GenW_helper; //W boson
	GenW_helper.SetXYZT(itree->GetLeaf("Wtop.vec.fX")->GetValue(0), itree->GetLeaf("Wtop.vec.fY")->GetValue(0), itree->GetLeaf("Wtop.vec.fZ")->GetValue(0),itree->GetLeaf("Wtop.vec.fT")->GetValue(0));
	
	Wpt = GenW_helper.Pt();
	Weta = GenW_helper.Eta();
	Wphi = GenW_helper.Phi();
	Wm = GenW_helper.M();
	
	TLorentzVector GenTop_helper; //top quark
	GenTop_helper.SetXYZT(itree->GetLeaf("top.vec.fX")->GetValue(0), itree->GetLeaf("top.vec.fY")->GetValue(0), itree->GetLeaf("top.vec.fZ")->GetValue(0),itree->GetLeaf("top.vec.fT")->GetValue(0));
	
	tpt = GenTop_helper.Pt();
	teta = GenTop_helper.Eta();
	tphi = GenTop_helper.Phi();
	tm = GenTop_helper.M();
	
	TLorentzVector GenbTop_helper; //b from top decay
	GenbTop_helper.SetXYZT(itree->GetLeaf("btop.vec.fX")->GetValue(0), itree->GetLeaf("btop.vec.fY")->GetValue(0), itree->GetLeaf("btop.vec.fZ")->GetValue(0),itree->GetLeaf("btop.vec.fT")->GetValue(0));
	
	btoppt = GenbTop_helper.Pt();
	btopeta = GenbTop_helper.Eta();
	btopphi = GenbTop_helper.Phi();
	btopm = GenbTop_helper.M();
	
	TLorentzVector Gensb_helper; //second b quark
	Gensb_helper.SetXYZT(itree->GetLeaf("secb.vec.fX")->GetValue(0), itree->GetLeaf("secb.vec.fY")->GetValue(0), itree->GetLeaf("secb.vec.fZ")->GetValue(0),itree->GetLeaf("secb.vec.fT")->GetValue(0));
	
	sbpt = Gensb_helper.Pt();
	sbeta = Gensb_helper.Eta();
	sbphi = Gensb_helper.Phi();
	sbm = Gensb_helper.M();
	
	TLorentzVector Genlq_helper; //light quark
	Genlq_helper.SetXYZT(itree->GetLeaf("q2.vec.fX")->GetValue(0), itree->GetLeaf("q2.vec.fY")->GetValue(0), itree->GetLeaf("q2.vec.fZ")->GetValue(0),itree->GetLeaf("q2.vec.fT")->GetValue(0));
	
	lqpt = Genlq_helper.Pt();
	lqeta = Genlq_helper.Eta();
	lqphi = Genlq_helper.Phi();
	lqm = Genlq_helper.M();
	
	for(Int_t i=0; i<2; i++) //Higgs daughters
	  {
	    TLorentzVector Hdau_helper;
	    if(i==0)
	      {
		Hdau_helper.SetXYZT(itree->GetLeaf("b1higgs.vec.fX")->GetValue(0), itree->GetLeaf("b1higgs.vec.fY")->GetValue(0), itree->GetLeaf("b1higgs.vec.fZ")->GetValue(0),itree->GetLeaf("b1higgs.vec.fT")->GetValue(0));
	      }
	    if(i==1)
	      {
		Hdau_helper.SetXYZT(itree->GetLeaf("b2higgs.vec.fX")->GetValue(0), itree->GetLeaf("b2higgs.vec.fY")->GetValue(0), itree->GetLeaf("b2higgs.vec.fZ")->GetValue(0),itree->GetLeaf("b2higgs.vec.fT")->GetValue(0));
	      }
	    
	    Hdaupt[i] = Hdau_helper.Pt();
	    Hdaueta[i] = Hdau_helper.Eta();
	    Hdauphi[i] = Hdau_helper.Phi();
	    Hdaum[i] = Hdau_helper.M();
	  }
	
	
	for(Int_t i=0; i<2; i++) //W daughters
	  {
	    TLorentzVector Wdau_helper;
	    if(i==0)
	      {
		Wdau_helper.SetXYZT(itree->GetLeaf("lep.vec.fX")->GetValue(0), itree->GetLeaf("lep.vec.fY")->GetValue(0), itree->GetLeaf("lep.vec.fZ")->GetValue(0),itree->GetLeaf("lep.vec.fT")->GetValue(0));
	      }
	    if(i==1)
	      {
		Wdau_helper.SetXYZT(itree->GetLeaf("nu.vec.fX")->GetValue(0), itree->GetLeaf("nu.vec.fY")->GetValue(0), itree->GetLeaf("nu.vec.fZ")->GetValue(0),itree->GetLeaf("nu.vec.fT")->GetValue(0));
	      }
	    
	    Wdaupt[i] = Wdau_helper.Pt();
	    Wdaueta[i] = Wdau_helper.Eta();
	    Wdauphi[i] = Wdau_helper.Phi();
	    Wdaum[i] = Wdau_helper.M();
	  }
      }

    //Top part

    if(isTop)
      {
	
	KITAGenParticle top1 = mykitagentopevent->tops.at(0);
	KITAGenParticle top2 = mykitagentopevent->tops.at(1);

	if(top1.pid==6) //sanity check. in fact, at(0) is always the top, at(1) the antitop
	  {
	    top_tpt   = top1.vec.Pt();
	    top_teta  = top1.vec.Eta();
	    top_tphi  = top1.vec.Phi();
	    top_te    = top1.vec.E();
	    
	    top_tbarpt = top2.vec.Pt();
	    top_tbareta = top2.vec.Eta();
	    top_tbarphi = top2.vec.Phi();
	    top_tbare   = top2.vec.E(); 

	    
	  }

	else
	  {
	    
	    top_tpt   = top2.vec.Pt();
	    top_teta  = top2.vec.Eta();
	    top_tphi  = top2.vec.Phi();
	    top_te    = top2.vec.E();
	    
	    top_tbarpt = top1.vec.Pt();
	    top_tbareta = top1.vec.Eta();
	    top_tbarphi = top1.vec.Phi();
	    top_tbare   = top1.vec.E(); 
	  }

	


	if(!debugevent.empty()){
	  cout << endl << "=============== Top Properties =================" << endl << endl;
	  cout << "KITA:" << endl;
	  cout <<" Top1.Pt = "<< top1.vec.Pt() << endl;
	  cout <<" Top1.Eta = "<< top1.vec.Eta() << endl;
	  cout <<" Top1.Phi = "<< top1.vec.Phi() << endl << endl;
	  cout <<" TopBar.Pt = "<< top2.vec.Pt() << endl;
	  cout <<" TopBar.Eta = "<< top2.vec.Eta() << endl;
	  cout <<" TopBar.Phi = "<< top2.vec.Phi() << endl;

	  cout << "Analyser:" << endl;
	  cout <<" Top1.Pt = "<< top_tpt << endl;
          cout <<" Top1.Eta = "<< top_teta << endl;
          cout <<" Top1.Phi = "<< top_tphi << endl << endl;
          cout <<" TopBar.Pt = "<< top_tbarpt << endl;
          cout <<" TopBar.Eta = "<< top_tbareta << endl;
          cout <<" TopBar.Phi = "<< top_tbarphi << endl;
	}

	if (mykitagentopevent->leptons.size()==1){

	  KITAGenParticle lep = mykitagentopevent->leptons.at(0);

	  KITAGenParticle neutrino = mykitagentopevent->neutrinos.at(0);
	
	  top_lepcharge = lep.Charge;
	

	  if(!debugevent.empty()) cout << " Lepton charge = " << top_lepcharge << endl;
	
	  KITAGenParticle w1 = mykitagentopevent->ws.at(0); 
	  KITAGenParticle w2 = mykitagentopevent->ws.at(1);
	  //cout << "Here 15c" << endl;
	  if(w1.pid==-24 &&w1.MoID==-6) //sanity check. in fact, at(0) is always W-, at(1) W+
	    { 
	      top_tbarwpt = w1.vec.Pt();
	      top_tbarweta = w1.vec.Eta();
	      top_tbarwphi = w1.vec.Phi();
	      top_tbarwe   = w1.vec.E(); 
	    
	      top_twpt   = w2.vec.Pt();
	      top_tweta  = w2.vec.Eta();
	      top_twphi  = w2.vec.Phi();
	      top_twe    = w2.vec.E();	   
	      // cout << "Here 15d" << endl;
	    }
	
	  else
	    {
	      top_tbarwpt = w2.vec.Pt();
	      top_tbarweta = w2.vec.Eta();
	      top_tbarwphi = w2.vec.Phi();
	      top_tbarwe   = w2.vec.E(); 
	    
	      top_twpt   = w1.vec.Pt();
	      top_tweta  = w1.vec.Eta();
	      top_twphi  = w1.vec.Phi();
	      top_twe    = w1.vec.E();	    
	    }		


	  if(!debugevent.empty()){
	    cout << endl << "=============== Top - W Properties =================" << endl << endl;
	    cout <<" TopW.Pt = "<< top_twpt << endl;
	    cout <<" TopW.Eta = "<< top_tweta << endl;
	    cout <<" TopW.Phi = "<< top_twphi << endl << endl;
	    cout <<" TopBarW.Pt = "<< top_tbarwpt << endl;
	    cout <<" TopBarW.Eta = "<< top_tbarweta << endl;
	    cout <<" TopBarW.Phi = "<< top_tbarwphi << endl;
	  }


	  KITAGenParticle q1 = mykitagentopevent->quarks.at(0); //checked in KITA: always bottom quark (PID 5), mother always top quark (PID 6)
	  KITAGenParticle q2 = mykitagentopevent->quarks.at(1); //checked in KITA: always bottom quark (PID -5), mother always top quark (PID -6)
	  KITAGenParticle q3 = mykitagentopevent->quarks.at(2);
	  KITAGenParticle q4 = mykitagentopevent->quarks.at(3);

	  if(q3.MoID!=q4.MoID)
	    cout << "PID: "<< q3.MoID << "Mo ID: " << q3.MoID << endl;

	  top_tbpt = q1.vec.Pt();
	  top_tbeta = q1.vec.Eta();
	  top_tbphi = q1.vec.Phi();
	  top_tbe   = q1.vec.E();

	  top_tbarbpt = q2.vec.Pt();
	  top_tbarbeta = q2.vec.Eta();
	  top_tbarbphi = q2.vec.Phi();
	  top_tbarbe   = q2.vec.E();

	  if(q3.MoID==24 && q4.MoID==24)
	    {
	      top_twdaupt[0]=q3.vec.Pt();
	      top_twdaueta[0]=q3.vec.Eta();
	      top_twdauphi[0]=q3.vec.Phi();
	      top_twdaue[0]=q3.vec.Eta();

	      top_twdaupt[1]=q4.vec.Pt();
	      top_twdaueta[1]=q4.vec.Eta();
	      top_twdauphi[1]=q4.vec.Phi();
	      top_twdaue[1]=q4.vec.Eta();

	      top_tbarwdaupt[0]=lep.vec.Pt();
	      top_tbarwdaueta[0]=lep.vec.Eta();
	      top_tbarwdauphi[0]=lep.vec.Phi();
	      top_tbarwdaue[0]=lep.vec.Eta();

	      top_tbarwdaupt[1]=neutrino.vec.Pt();
	      top_tbarwdaueta[1]=neutrino.vec.Eta();
	      top_tbarwdauphi[1]=neutrino.vec.Phi();
	      top_tbarwdaue[1]=neutrino.vec.Eta();
	      //  cout << top_twdaupt[0] << " || " << top_twdaupt[1] << endl;


	    }
	  else
	    {
	      top_twdaupt[0]=lep.vec.Pt();
	      top_twdaueta[0]=lep.vec.Eta();
	      top_twdauphi[0]=lep.vec.Phi();
	      top_twdaue[0]=lep.vec.Eta();

	      top_twdaupt[1]=neutrino.vec.Pt();
	      top_twdaueta[1]=neutrino.vec.Eta();
	      top_twdauphi[1]=neutrino.vec.Phi();
	      top_twdaue[1]=neutrino.vec.Eta();

	      top_tbarwdaupt[0]=q3.vec.Pt();
	      top_tbarwdaueta[0]=q3.vec.Eta();
	      top_tbarwdauphi[0]=q3.vec.Phi();
	      top_tbarwdaue[0]=q3.vec.Eta();

	      top_tbarwdaupt[1]=q4.vec.Pt();
	      top_tbarwdaueta[1]=q4.vec.Eta();
	      top_tbarwdauphi[1]=q4.vec.Phi();
	      top_tbarwdaue[1]=q4.vec.Eta();

	    }
	
	  if(!debugevent.empty()){
	    cout << endl << "=============== Top - Quarks Properties =================" << endl << endl;

	    cout <<" TopWDau0.Pt = "<< top_twdaupt[0] << endl;
	    cout <<" TopWDau0.Eta = "<< top_twdaueta[0] << endl;
	    cout <<" TopWDau0.Phi = "<< top_twdauphi[0] << endl << endl;

	    cout <<" TopWDau1.Pt = "<< top_twdaupt[1] << endl;
	    cout <<" TopWDau1.Eta = "<< top_twdaueta[1] << endl;
	    cout <<" TopWDau1.Phi = "<< top_twdauphi[1] << endl << endl;

	    cout <<" TopBarWDau0.Pt = "<< top_tbarwdaupt[0] << endl;
	    cout <<" TopBarWDau0.Eta = "<< top_tbarwdaueta[0] << endl;
	    cout <<" TopBarWDau0.Phi = "<< top_tbarwdauphi[0] << endl;

	    cout <<" TopBarWDau1.Pt = "<< top_tbarwdaupt[1] << endl;
	    cout <<" TopBarWDau1.Eta = "<< top_tbarwdaueta[1] << endl;
	    cout <<" TopBarWDau1.Phi = "<< top_tbarwdauphi[1] << endl;
	  }

	//	cout << top_twdaupt[0] << " || " << top_twdaupt[1] << endl;
	}
	
      }
    
    if(debug) cout << "Before Reconstruction" << endl;

    //cout << "Here 16" << endl;

    //
    // RECONSTRUCTION PART
    //
     
    //bool has_wenu  = niso_el==1&&nveto_el==1&&niso_mu==0&&nveto_mu==0;
    //bool has_wmunu = niso_mu==1&&nveto_mu==1&&niso_el==0&&nveto_el==0;
    bool has_lel = nveto_el==1;
    bool has_lmu = nveto_mu==1;
    
    TLorentzVector p4nu;
    TLorentzVector lep;
    TLorentzVector p4W;
    
    // Extended the acceptance of this part for the QCD studies, since we ask explicitly for nveto_el == 1 && niso_el == 0
    if (has_lmu||has_lel) {
      if (has_lmu && has_lel){
	if(lelpt[posnvetoel]>lmupt[posnvetomu]) lep.SetPtEtaPhiE(lelpt[posnvetoel],leleta[posnvetoel],lelphi[posnvetoel],lele[posnvetoel]);
	else lep.SetPtEtaPhiE(lmupt[posnvetomu],lmueta[posnvetomu],lmuphi[posnvetomu],lmue[posnvetomu]);
      }
      else if (has_lel) lep.SetPtEtaPhiE(lelpt[posnvetoel],leleta[posnvetoel],lelphi[posnvetoel],lele[posnvetoel]);
      else if (has_lmu) lep.SetPtEtaPhiE(lmupt[posnvetomu],lmueta[posnvetomu],lmuphi[posnvetomu],lmue[posnvetomu]);
      
      float nu_e  = met;
      float nu_px = metx;
      float nu_py = mety;
      
      float lep_e  = lep.E();
      float lep_pt = lep.Pt();
      float lep_px = lep.Px();
      float lep_py = lep.Py();
      float lep_pz = lep.Pz();

      float mw  = 80.43;
      
      float pt  = std::sqrt((lep_px+nu_px)*(lep_px+nu_px)+
			    (lep_py+nu_py)*(lep_py+nu_py));
      
      float mtsq= ((lep_pt+nu_e)*(lep_pt+nu_e) - 
		   (lep_px+nu_px)*(lep_px+nu_px) - 
		   (lep_py+nu_py)*(lep_py+nu_py));
      
      float mt  = (mtsq>0.0) ? std::sqrt(mtsq) : 0.0;
            
      float A, B, Csq, C, S1, S2;
      float scf(1.0);
      
      if (mt < mw) {
	A = mw*mw/2.0;
      }
      else {
	A = mt*mt/2.0;
	float k = nu_e*lep_pt - nu_px*lep_px - nu_py*lep_py;
	if (k <0.0001) k = 0.0001;
	scf = 0.5*(mw*mw)/k;
	nu_px *= scf;
	nu_py *= scf;
	nu_e  = sqrt(nu_px*nu_px + nu_py*nu_py);
      }
      
      B  = nu_px*lep_px + nu_py*lep_py;
      Csq= 1.0 + nu_e*nu_e*(lep_pz*lep_pz-lep_e*lep_e)/(A+B)/(A+B);
      C  = (Csq>0.0) ? std::sqrt(Csq) : 0.0;
      S1 = (-(A+B)*lep_pz + (A+B)*lep_e*C)/(lep_pz*lep_pz-lep_e*lep_e);
      S2 = (-(A+B)*lep_pz - (A+B)*lep_e*C)/(lep_pz*lep_pz-lep_e*lep_e);
      
      float nu_pz = (std::abs(S1) < std::abs(S2)) ? S1 : S2;
            
      TVector3 nu;   nu.SetXYZ(nu_px,nu_py,nu_pz);
      TVector3 wlnu; wlnu.SetXYZ(nu_px+lep_px,nu_py+lep_py,nu_pz+lep_pz);
      
      nlepw       = 1;
      
      lepwpt   = pt;
      lepweta  = wlnu.Eta();
      lepwphi  = wlnu.Phi();
      lepwet   = nu_e + lep_pt;
      lepwmt   = mt;
      lepwdphi = lep.Vect().DeltaPhi(nu);

      float nupt=std::sqrt((nu_px)*(nu_px)+
			   (nu_py)*(nu_py));
      float nueta=nu.Eta();
      float nuphi=nu.Phi();

      p4nu.SetPtEtaPhiM(nupt,nueta,nuphi,0);
      p4W=lep+p4nu;
      
      //p4W.SetPtEtaPhiM(lepwpt,lepweta,lepwphi,80.43);
      
      lepwm=p4W.M();  
      
      
      //Calculate sphericity and aplanarity

      double denominator=0;
      double numXX=0;
      double numYY=0;
      double numZZ=0;
      double numXY=0;
      double numXZ=0;
      double numYZ=0;
      
      denominator+=pow(lep.P(),2) + pow(p4nu.P(),2);
      numXX+=pow(lep.X(), 2)+pow(p4nu.X(), 2);
      numYY+=pow(lep.Y(), 2)+pow(p4nu.Y(), 2);
      numZZ+=pow(lep.Z(), 2)+pow(p4nu.Z(), 2);
      numXY+=lep.X()*lep.Y()+p4nu.X()*p4nu.Y();
      numXZ+=lep.X()*lep.Z()+p4nu.X()*p4nu.Z();
      numYZ+=lep.Y()*lep.Z()+p4nu.Y()*p4nu.Z();

       
      for (Int_t iJet=0; iJet<njt; ++iJet)
	{
	  TLorentzVector Jet;
	  Jet.SetPtEtaPhiE(jtpt[iJet],jteta[iJet],jtphi[iJet],jte[iJet]);
	  denominator += pow(Jet.P(),2);
	  numXX+=pow(Jet.X(), 2);
	  numYY+=pow(Jet.Y(), 2);
	  numZZ+=pow(Jet.Z(), 2);
	  numXY+=Jet.X()*Jet.Y();
	  numXZ+=Jet.X()*Jet.Z();
	  numYZ+=Jet.Y()*Jet.Z();
	}

      TMatrix spherTens(3, 3);
      spherTens(0, 0) = numXX/denominator;
      spherTens(1, 1) = numYY/denominator; //cout << numYY/denominator << endl;
      spherTens(2, 2) = numZZ/denominator; //cout << numZZ/denominator << endl;
      spherTens(0, 1) = numXY/denominator; //cout << numXY/denominator << endl;
      spherTens(1, 0) = numXY/denominator; //cout << numXX/denominator << endl;
      spherTens(0, 2) = numXZ/denominator; //cout << numXZ/denominator << endl;
      spherTens(2, 0) = numXZ/denominator; //cout << numXX/denominator << endl;
      spherTens(1, 2) = numYZ/denominator; //cout << numYZ/denominator << endl;
      spherTens(2, 1) = numYZ/denominator; //cout << numXX/denominator << endl;


      TVector eigenval(3);

      spherTens.EigenVectors(eigenval);
	
      sphericity=0;
      aplanarity=0;

      if (eigenval(0)<=eigenval(1) && eigenval(1)<=eigenval(2))
	{
	  sphericity = 3.0*(eigenval(0)+eigenval(1))/2.0;
	  aplanarity = 3.0*eigenval(0)/2.0;
	}
      if (eigenval(0)<=eigenval(2) && eigenval(2)<=eigenval(1))
	{
	  sphericity = 3.0*(eigenval(0)+eigenval(2))/2.0;
	  aplanarity = 3.0*eigenval(0)/2.0;
	}
      if (eigenval(1)<=eigenval(0) && eigenval(0)<=eigenval(2))
	{
	  sphericity = 3.0*(eigenval(1)+eigenval(0))/2.0;
	  aplanarity = 3.0*eigenval(1)/2.0;
	}
      if (eigenval(1)<=eigenval(2) && eigenval(2)<=eigenval(0))
	{
	  sphericity = 3.0*(eigenval(1)+eigenval(2))/2.0;
	  aplanarity = 3.0*eigenval(1)/2.0;
	}
      if (eigenval(2)<=eigenval(0) && eigenval(0)<=eigenval(1))
	{
	  sphericity = 3.0*(eigenval(2)+eigenval(0))/2.0;
	  aplanarity = 3.0*eigenval(2)/2.0;
	}
      if (eigenval(2)<=eigenval(1) && eigenval(1)<=eigenval(0))
	{
	  sphericity = 3.0*(eigenval(2)+eigenval(1))/2.0;
	  aplanarity = 3.0*eigenval(2)/2.0;
	}


      
    }
    else
      {
	nlepw    = 0;
	lepwpt   = -999.;
	lepweta  = -999.;
	lepwphi  = -999.;
	lepwet   = -999.;
	lepwmt   = -999.;
	lepwdphi = -999.;
	lepwm    = -999.;
	sphericity=-999.;
	aplanarity=-999.;
      }    
    

    // JET ASSIGNMENT
 
    Int_t threefoundat=0;
    Int_t ntaggedjets=0;
    threefoundat=threefoundat;
    //THREE HARDEST B JETS = RECO JETS

    Int_t foundcounter=0;
    Int_t foundarray[3];
    
    for(Int_t i=0; i<njt; i++)
      {    

	if(jtcsvt[i]>0.898) 
	  {
	    ntaggedjets++;
	    foundarray[foundcounter]=i;
	    foundcounter++;
	  }
	if(ntaggedjets==3) 
	  {
	    threefoundat = i;
	    break;
	  }
      }
    
    if(ntaggedjets==3&&nlepw==1)
      {

	//SET LORENTZVECTORS FOR THREE HARDEST FOUND B JETS
	TLorentzVector B1Vector;
	B1Vector.SetPtEtaPhiE(jtpt[foundarray[0]],jteta[foundarray[0]],jtphi[foundarray[0]],jte[foundarray[0]]);
	TLorentzVector B2Vector;
	B2Vector.SetPtEtaPhiE(jtpt[foundarray[1]],jteta[foundarray[1]],jtphi[foundarray[1]],jte[foundarray[1]]);
	TLorentzVector B3Vector;
	B3Vector.SetPtEtaPhiE(jtpt[foundarray[2]],jteta[foundarray[2]],jtphi[foundarray[2]],jte[foundarray[2]]);
		
	//TOP&HIGGS RECONSTRUCTION WITH CHI2
	
	float const sigmaTop2 = 30. * 30., sigmaHiggs2 = 15. * 15.;
	float const mTrueTop = 173., mTrueHiggs = 125.;
	
	TLorentzVector p4Top;
	TLorentzVector p4Higgs;
		
	MassDeviation = 1e10;
	
	topjtidx=-999;
	hbbjtidx[0]=-999;
	hbbjtidx[1]=-999;

	for(Int_t i=0; i<3; i++)
	  {    
	    
	    TLorentzVector BVector;
	    BVector.SetPtEtaPhiE(jtpt[foundarray[i]],jteta[foundarray[i]],jtphi[foundarray[i]],jte[foundarray[i]]);
	    p4Top=p4W+BVector;
	    p4Higgs=B1Vector*(foundarray[0]!=foundarray[i])+B2Vector*(foundarray[1]!=foundarray[i])+B3Vector*(foundarray[2]!=foundarray[i]);
	    
	    //	    cout << "Top Mass: " << p4Top.M() << endl;
	    
	    float const curDeviation =std::pow(p4Top.M()-mTrueTop,2)/sigmaTop2+std::pow(p4Higgs.M()-mTrueHiggs,2)/sigmaHiggs2;
	    //	    cout<< "Deviation: "<<curDeviation << endl;
	    if(curDeviation<MassDeviation)
	      {
		MassDeviation=curDeviation;
		topjtidx=foundarray[i];
		hbbjtidx[0]=foundarray[i+1]*(i==0)+foundarray[i-1]*(i==1)+foundarray[i-2]*(i==2);
		hbbjtidx[1]=foundarray[i+2]*(i==0)+foundarray[i+1]*(i==1)+foundarray[i-1]*(i==2);
	      }
	  }

	//SET FINAL LORENTZVECTORS AFTER JET ASSIGNMENT

	TLorentzVector topBJet;
	topBJet.SetPtEtaPhiE(jtpt[topjtidx],jteta[topjtidx],jtphi[topjtidx],jte[topjtidx]);
	TLorentzVector p4Top_final=topBJet+p4W;

	TLorentzVector B1HiggsJet;
	TLorentzVector B2HiggsJet;
	B1HiggsJet.SetPtEtaPhiE(jtpt[hbbjtidx[0]],jteta[hbbjtidx[0]],jtphi[hbbjtidx[0]],jte[hbbjtidx[0]]);
	B2HiggsJet.SetPtEtaPhiE(jtpt[hbbjtidx[1]],jteta[hbbjtidx[1]],jtphi[hbbjtidx[1]],jte[hbbjtidx[1]]);
	TLorentzVector p4Higgs_final=B1HiggsJet+B2HiggsJet;

	hbbm   =  p4Higgs_final.M();
	hbbpt  =  p4Higgs_final.Pt();
	hbbeta =  p4Higgs_final.Eta();
	hbbphi =  p4Higgs_final.Phi();
	hbbdr  =  deltaR(jteta[hbbjtidx[0]],jtphi[hbbjtidx[0]],jteta[hbbjtidx[1]],jtphi[hbbjtidx[1]]);
	

	topm   =  p4Top_final.M();
	//	cout << "Final Top Mass: " << topm<< endl;
	toppt  =  p4Top_final.Pt();
	topeta =  p4Top_final.Eta();
	topphi =  p4Top_final.Phi();

      }
    else
      {
	hbbm   = -999;
	hbbpt  = -999;
	hbbeta = -999;
	hbbphi = -999;
	hbbdr  = -999;

	topm   = -999;
	toppt  = -999;
	topeta = -999;
	topphi = -999;
      }

    /*
      
    Float_t smallestdr=999.;
    bool hdau0found=false;
    Int_t hdau0jetid;
    Int_t hdau1jetid;
    Int_t topbjetid;


    for(Int_t i=0; i<njt15; i++)
    {
    if(deltaR(Hdaueta[0],Hdauphi[0],jtgeneta[firstgenfoundat],jtgenphi[firstgenfoundat])<0.5 && deltaR(jtgeneta[firstgenfoundat],jtgenphi[firstgenfoundat],jteta[i],jtphi[i])<0.3 && jtcsvt[i]>0.679) 
    {
    if(smallestdr>deltaR(Hdaueta[0],Hdauphi[0],jtgeneta[firstgenfoundat],jtgenphi[firstgenfoundat]))
    {
    smallestdr=deltaR(Hdaueta[0],Hdauphi[0],jtgeneta[firstgenfoundat],jtgenphi[firstgenfoundat]);
    hdau0jetid=i;
    hdau0found=true;
    }
    }
    if(deltaR(Hdaueta[0],Hdauphi[0],jtgeneta[secondgenfoundat],jtgenphi[secondgenfoundat])<0.5 && deltaR(jtgeneta[secondgenfoundat],jtgenphi[secondgenfoundat],jteta[i],jtphi[i])<0.3 && jtcsvt[i]>0.679) continue;
    if(deltaR(Hdaueta[0],Hdauphi[0],jtgeneta[thirdgenfoundat],jtgenphi[thirdgenfoundat])<0.5 && deltaR(jtgeneta[thirdgenfoundat],jtgenphi[thirdgenfoundat],jteta[i],jtphi[i])<0.3 && jtcsvt[i]>0.679) continue;

    }
    


    */
    // Cos(theta*)
    if(debug) cout << "Before CosThetaStar" << endl;
    TLorentzVector lep4vec;
    lep4vec.SetPtEtaPhiM(Wdaupt[0],Wdaueta[0],Wdauphi[0],Wdaum[0]);
    TLorentzVector lq4vec;
    lq4vec.SetPtEtaPhiM(lqpt,lqeta,lqphi,lqm);
    TLorentzVector genhiggs4vec;
    genhiggs4vec.SetPtEtaPhiM(Hpt,Heta,Hphi,Hm);
    TLorentzVector gentop4vec;
    gentop4vec.SetPtEtaPhiM(tpt,teta,tphi,tm);

    TLorentzVector boost_topcm = gentop4vec;
    TLorentzVector boost_tophiggscm = gentop4vec+genhiggs4vec;
    TVector3 boost3_topcm = boost_topcm.BoostVector();
    TVector3 boost3_tophiggscm = boost_tophiggscm.BoostVector();

    lep4vec.Boost(-boost3_topcm);
    lq4vec.Boost(-boost3_topcm);

    costst = TMath::Cos (lep4vec.Vect().Angle(lq4vec.Vect()));

    lep4vec.Boost(boost3_topcm-boost3_tophiggscm);
    lq4vec.Boost(boost3_topcm-boost3_tophiggscm);

    coststh = TMath::Cos (lep4vec.Vect().Angle(lq4vec.Vect()));
    if(debug) cout << "Before Fill" << endl;
    otree->Fill();
           
  } // ievt

  delete hffinder;

  //  TH1F* count = (TH1F*)ifile->Get("Count");
  //double ntotal = count->GetEntries(); 
  double ntotal = otree->GetEntries();
  double efficiency = 1.0;
  double kfac = 1.0;
  double xsec = 1.0;

  stringstream ss; ss<<"efficiency:"<<efficiency;
  stringstream ss1; ss1<<"ntotal:"<<ntotal;
  stringstream ss2; ss2<<"xsection:"<<xsec;
  stringstream ss3; ss3<<"kfactor:"<<kfac;
 
  otree->GetUserInfo()->Add(new TObjString(ss.str().c_str()));
  otree->GetUserInfo()->Add(new TObjString(ss1.str().c_str()));
  otree->GetUserInfo()->Add(new TObjString(ss2.str().c_str()));
  otree->GetUserInfo()->Add(new TObjString(ss3.str().c_str()));
  // Write files 
  // ------------------------------------------------------------------------------
  ifile->Close();
  delete ifile;
  ofile->Write();
  ofile->Close();
  delete ofile;
  
  cout<<endl << "Save extended tree to "<< output <<" ... "<<flush;
  //  string cmd = "mv" + output + " " + input;
  //  gSystem->Exec(cmd.c_str());
  cout<<"DONE!\n"<<endl;
  // ifile
  


  return 0;
}



////////////////////////////////////////////////////////////////////////////////
// helpers
////////////////////////////////////////////////////////////////////////////////
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
Bool_t trig_finder(string triggername)
{
  std::map<std::string, uint64_t>::const_iterator iter = kitahlt->triggerInfos.lower_bound(triggername);
  if(iter == kitahlt->triggerInfos.end() || iter->first.find(triggername)!=0){
    cout << "error: trigger " + triggername + " not found!" << endl;
    return false; 
  }
  return KITAHlt::status(iter->second) == 1;
}
//______________________________________________________________________________
// Effective area for muon with dR=0.4 in 2012 data. Taken from:
// https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=188494
// (slide 9, column PF combined DR<0.4)
double muAEff(double eta){
  double ea = -999;
  
  if(false) ;
  else if ( fabs(eta) < 1.0 ) ea = 0.674;
  else if ( fabs(eta) < 1.5 ) ea = 0.564;
  else if ( fabs(eta) < 2.0 ) ea = 0.442;
  else if ( fabs(eta) < 2.2 ) ea = 0.515;
  else if ( fabs(eta) < 2.3 ) ea = 0.821;
  else if ( fabs(eta) < 2.4 ) ea = 0.660;
  else 
    {
      //cerr << "Warning: Demanded effective area for muon with eta > 2.4. Using value at 2.4 instead" << endl;
      ea = 0.660;
    }
  
  return ea;
}

//______________________________________________________________________________
float eleAEff(float SCEta){
  float ea = -999;
  
  if(false) ;
  else if ( fabs(SCEta) < 1.0 ) ea = 0.130;
  else if ( fabs(SCEta) < 1.479 ) ea = 0.137;
  else if ( fabs(SCEta) < 2.0 ) ea = 0.067;
  else if ( fabs(SCEta) < 2.2 ) ea = 0.089;
  else if ( fabs(SCEta) < 2.3 ) ea = 0.107;
  else if ( fabs(SCEta) < 2.4 ) ea = 0.110;
  else ea = 0.138;
  
  return ea;
}

//_____________________________________________________________________

bool PFJetID_2012(const KITAJet & jet,const double raw_e) {

  Float_t jetptmin=15.; //originally: 10
  Float_t jetetamax=4.7;
 
  if(jet.vec.Pt()<=jetptmin) return false;
  if(fabs(jet.vec.Eta()) >= jetetamax) return false;
  if(jet.pflowvars.size() ==0) return false;  
  
  //  const int constituents = jet.pflowvars[0].ChHadronMult + jet.pflowvars[0].NHadronMult
  //    + jet.pflowvars[0].EleMult + jet.pflowvars[0].PhMult + jet.pflowvars[0].MuMult
  //  + jet.pflowvars[0].HFEmMult + jet.pflowvars[0].HFHadronMult;
  //const double raw_energy = jet.vec.E() * jet.jec_factors.find(KITAJet::RAW)->second; 
  const double raw_energy = raw_e; 
  if(jet.nDaughters<=1)
    return false;
  
  if(((jet.pflowvars[0].NHadronEnergy+jet.pflowvars[0].HFHadronEnergy)/raw_energy)>=0.99 ) return false; 
  if((jet.pflowvars[0].NEmEnergy/raw_energy)>=0.99) return false;

  if(fabs(jet.vec.Eta())<2.4)
    {
      if((jet.pflowvars[0].ChEmEnergy/raw_energy)>=0.99 ) return false;
      if((jet.pflowvars[0].ChHadronEnergy/raw_energy)==0) return false;
      if(jet.pflowvars[0].ChMult==0)return false;
    }
  //cout << " nach jet ID!\n";
  return true;
}

//____________________________________________

bool EleID_2012(const KITAElectron & lep, float relIso, float elemvaid) {
  Float_t eleptmin=30;
  Float_t eleetamax=2.5;
  Float_t relisomax=0.1;
  //  if(fabs(lep.distEleVtxPV)>=1) return false;
  if(fabs(lep.vec.Pt())<=eleptmin) return false;
  if(fabs(lep.vec.Eta())>=eleetamax) return false;
  if(fabs(lep.sclEta)>1.4442 && fabs(lep.sclEta)<1.5660) return false; // EB-EE transition region
  //if(fabs(lep.dxyPV)>=0.02) return false; // transverse IP of GSF track with regard to first offline PV
  if(lep.trNoOfECL_Inner > 0) return false;
  
  if(elemvaid <= 0.9 || elemvaid>1) return false;    
  // conversion veto
  // WARNING values might change. look at https://twiki.cern.ch/twiki/bin/viewauth/CMS/ConversionTools
  /*  const double probMin = 1e-6;
      const double lxyMin = 2.0;
      const int nHitsBeforeVtxMax = 0;
      KITAConversionInfos::const_iterator it = lep.conversionInfos.begin();
      KITAConversionInfos::const_iterator end = lep.conversionInfos.end();
      for (; it!=end; it++) {
      if (!(it->vtxProb>probMin &&  it->lxy>lxyMin && it->nHitsMax<=nHitsBeforeVtxMax)) continue;
      else return false;
      }*/
  if(lep.passConversionVeto == false) return false;
  if(relIso >= relisomax) return false;
  return true;
}
//____________________________________________

bool EleVetoID_2012(const KITAElectron & lep, float relIso, float elemvaid) {
  Float_t eleptmin=20;
  Float_t eleetamax=2.5;
  Float_t relisomax=0.15;

  if(fabs(lep.vec.Pt())<=eleptmin) return false;
  if(fabs(lep.vec.Eta())>=eleetamax) return false;  
  //  if(elemvaid<=0. || elemvaid>1) return false;      no MVA ID for loose electrons, information still filled
  if(relIso >= relisomax) return false;
  return true;
}

//____________________________________________

bool MuID_2012(const KITAMuon & lep, float relIso, float pv_z, int pv_size)
{
  Float_t muptmin=26;
  Float_t muetamax=2.1;
  Float_t murelisomax=0.12;

  if(!lep.isGlobalMuon)                   return false; 
  if(!lep.isPFMuon)                       return false;
  if(!lep.isGlobalMuonPromptTight)        return false; // normChi2<10, numberofvalidmuonhits>0
  if(lep.vec.Pt()<=muptmin)               return false;
  if(fabs(lep.vec.Eta())>=muetamax)       return false;
  if(lep.trackerLayersWithMeasurement<=5) return false;
  
  if(pv_size==0) return false;

  if(fabs(lep.distMuBestTrVtxdZ) >= 0.5) return false;
  
  if(lep.trackerNoOfValidPixelHits<1)  return false; // muon.innerTrack()->hitPattern().numberOfValidPixelHits() 
  if(fabs(lep.distMuBestTrVtxdXY)>=0.2)  return false;

  if(lep.numberOfMatchedStations <= 1)  return false;
  
  if(relIso >= murelisomax)  return false;
      
  return true;
}
//____________________________________________

bool MuVetoID_2012(const KITAMuon & lep, float relIso)
{
  Float_t muptmin=10.;
  Float_t muetamax=2.5;
  Float_t murelisomax=0.2;

  if(!lep.isPFMuon) return false;
  if(!lep.isGlobalMuon && !lep.isTrackerMuon) return false;
  if(lep.vec.Pt()<=muptmin) return false;
  if(fabs(lep.vec.Eta())>=muetamax) return false;
  if(relIso >= murelisomax) return false;
      
  return true;
}

//___________________________________________

bool checkElectronMVAIDPreselection(const KITAElectron & ele)
{
  
  // Check preconditions for "triggering electrons" as defined in
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentification#Training_of_the_MVA

  const double pt = ele.vec.Pt();
  //cout << pt << endl;
  //ONLY FOR TESTNG ...
  //  const double pt = ele.ptnoecal;
  //  cout << "uncorr: "<< ele.dr03HcalTowerSumEt << " | Corr: " << ele.dr03EcalCorrected << endl;

  if(fabs(ele.sclEta) < 1.479)
    {
      if( ele.sigmaIetaIeta < 0.014 &&
	  ele.hadOverEm < 0.15 &&
	  ele.dr03TkSumPt / pt < 0.2 &&
	  ele.dr03EcalCorrected / pt < 0.2 &&
	  //	ele.dr03EcalRecHitSumEt / pt < 0.2 &&
	  ele.dr03HcalTowerSumEt / pt < 0.2 && // WARNING at time of writing, recipe actually wants this non-BC value
	  ele.trInnerNoOfLostHits == 0
	  ){
      
	return true;
      }
      else
	return false;
    }
  else
    {
      if( ele.sigmaIetaIeta < 0.035 &&
	  ele.hadOverEm < 0.10 &&
	  ele.dr03TkSumPt / pt < 0.2 &&
	  ele.dr03EcalCorrected / pt < 0.2 &&
	  // ele.dr03EcalRecHitSumEt / pt < 0.2 &&
	  ele.dr03HcalTowerSumEt / pt < 0.2 && // WARNING at time of writing, recipe actually wants this non-BC value
	  ele.trInnerNoOfLostHits == 0
	  )
	return true;
      else
	return false;
    }

}



//______________________________________________________

bool PrimaryVertexCut(const KITAPrimaryVertices & pv){
  // assert(rec.PrimaryVertices);
  //cout << " _____________ START ________" << endl;
  Float_t min_ndof = 4;
  Float_t max_z = 24;
  Float_t max_rho = 2; 
  if(pv.size()==0) return false;
  //cout << "PV SIZE: " << pv.size() << endl;
  const KITAPrimaryVertex & vertex = pv.at(0);
  if(vertex.isFake) return false;
  //cout << "IS FAKE : " << vertex.isFake << endl;
  if(vertex.ndof < min_ndof) return false;
  //cout << "NDOF : " << vertex.ndof << endl;
  if(fabs(vertex.z) >= max_z) return false;
  // cout << "Z : " << fabs(vertex.z) << endl;
  if(vertex.rho >= max_rho) return false;
  //cout << " RHO : " << vertex.rho << endl;
  //cout << " %%%%%%%%% FOUND %%%%%%%" << endl;
  return true;
}

int NGoodPV(const KITAPrimaryVertices & pv){
  Float_t min_ndof = 4;
  Float_t max_z = 24;
  Float_t max_rho = 2; 
  int npv = 0;
  if(pv.size()==0) return 0;
  for(UInt_t i=0; i<pv.size(); i++){
    const KITAPrimaryVertex & vertex = pv.at(i);
    if(!vertex.isFake && vertex.ndof >= min_ndof && fabs(vertex.z) < max_z && vertex.rho < max_rho)
      npv++;
  }
  return npv;
}

//_____________________________________________________
void fillScaleFactors(float pt, float eta, int flv, Float_t& csvl, Float_t& csvm, Float_t& csvt){
  if(fabs(eta)<2.4){
    if(TMath::Abs(flv) == 4 or TMath::Abs(flv) == 5)
      {
	csvl = GetSFTagCSVL(pt);
	csvm = GetSFTagCSVM(pt);
	csvt = GetSFTagCSVT(pt);
      }
    else{
      csvl = GetSFMistagCSVLMean(pt,fabs(eta));
      csvm = GetSFMistagCSVMMean(pt,fabs(eta));
      csvt = GetSFMistagCSVTMean(pt,fabs(eta));
    }
  }
  else{
    csvl=1.0;
    csvm=1.0;
    csvt=1.0;
  }

}

//_____________________________________________________
bool Preselection(const KITAMisc& kitamisc, const KITADataCleaning& kitadatacleaning, bool isdata){
  if(kitamisc.filteredByScraping)                                    return false;
  if(kitadatacleaning.filteredByHBHENoiseFilter)                     return false;
  if(kitadatacleaning.filteredByTightCSCBeamHaloFilter)              return false;
  if(kitadatacleaning.filteredByEcalDeadCellTriggerPrimitiveFilter)  return false;
  if(kitadatacleaning.filteredByTrackingFailureFilter)               return false;
  if(kitadatacleaning.filteredByEeBadScFilter)                       return false;
  if(kitadatacleaning.filteredByEcalLaserCorrFilter)                 return false;
  if(kitadatacleaning.filteredByTrkPOGFilters)                       return false;
  return true;
}
