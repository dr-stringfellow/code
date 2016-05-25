////////////////////////////////////////////////////////////////////////////////
//
//th_bdttagexpert_x
// ------------
//
// tool to classify a single top higgs event with  boosted decision trees (TMVA package) - requires nothin' but root
//
// 
//
//    
//    ::05/14/2012 Benedikt Maier <bmaier@ekp.uni-karlsruhe.de>::
////////////////////////////////////////////////////////////////////////////////

//#include "SchieferD/AnaUtils/interface/MCSample.h"

#include "../interface/CommandLine.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <map>
#include <TLorentzVector.h>

#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TKey.h"
#include "TControlBar.h"
#include "TSystem.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TObjString.h"
#include "TChain.h"
//#include "/portal/ekpcms5/home/held/code/root_v5.28.00c/tmva/test/TMVAGui.C"
//#include "/portal/ekpcms5/home/held/code/root_v5.28.00c/tmva/test/tmvaglob.C"
//#include "TStopwatch.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
//////////////////////////////////////////////////////////////////////////////////______________________________________________________________________________

struct GreaterByVal{
  vector<float>  pts;
  GreaterByVal(vector<float>  pts_): pts(pts_){}
  bool operator()(size_t i, size_t j) const{
    return pts.at(i) > pts.at(j);
  }
};

struct SmallerByVal{
  vector<float>  pts;
  SmallerByVal(vector<float>  pts_): pts(pts_){}
  bool operator()(size_t i, size_t j) const{
    return pts.at(i) < pts.at(j);
  }
};

double deltaPhi(double phi1, double phi2);
double deltaR(double eta1, double phi1, double eta2, double phi2);


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main(int argc,char**argv)
{
  
  // This loads the library
  
  TMVA::Tools::Instance();
  
  
  // Default MVA methods to be trained + tested
  std::map<std::string,int> Use;
  
  // --- Boosted Decision Trees
  Use["BDT"]             = 1; // uses Adaptive Boost
  Use["BDTG"]            = 0; // uses Gradient Boost
  Use["BDTB"]            = 0; // uses Bagging
  Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
  std::cout << std::endl;
  std::cout << "==> Start TMVAClassificationApplication" << std::endl;
  
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  string         input     = cl.getValue <string>  ("input");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         ipath     = cl.getValue <string>  ("ipath",              ".");
  string         opath     = cl.getValue <string>  ("opath",              ".");
  bool           signal    = cl.getValue <bool>    ("signal",           false);
  string         bdtpath0  = cl.getValue<string>   ("bdtpath0"               );
  string         bdtpath1  = cl.getValue<string>   ("bdtpath1"               );
  string         bdtpath2  = cl.getValue<string>   ("bdtpath2"               );
  string         bdtpath3  = cl.getValue<string>   ("bdtpath3"               );
  string         bdtpath4  = cl.getValue<string>   ("bdtpath4"               );
  int            btagsys   = cl.getValue<int>      ("btagsys",              0);
  
  if(!cl.check()) {};
  cl.print();
  
  assert (std::abs(btagsys)<5);
  
  if      (0==btagsys) cout<<"Nominal mode"<<endl;
  else if (1==btagsys) cout<<"Systematic BTAG UP"<<endl;
  else if (2==btagsys) cout<<"Systematic BTAG DOWN"<<endl;
  else if (3==btagsys) cout<<"Systematic MISTAG UP"<<endl;
  else if (4==btagsys) cout<<"Systematic MISTAG DOWN"<<endl;
  
  // --- Create the Reader object
  
  TMVA::Reader *reader_th = new TMVA::Reader( "!Color:!Silent" );    
  
  // Create a set of variables and declare them to the reader
  // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
  Float_t var1=0., var2=0.;
  Float_t var3=0., var4=0.;
 
  Int_t spec1=0;
  Int_t spec2=0;
  Int_t spec3=0;
  Int_t spec4=0;

  reader_th->AddSpectator("taggerpos[0]",   &spec1);
  reader_th->AddSpectator("taggerpos[1]",   &spec2);
  reader_th->AddSpectator("taggerpos[2]",   &spec3);
  reader_th->AddSpectator("taggerpos[3]",   &spec4);

  reader_th->AddVariable( "jtcsvtcorr[taggerpos[0]][0]",                       &var1 );
  reader_th->AddVariable( "jtcsvtcorr[taggerpos[1]][0]",                       &var2 );
  reader_th->AddVariable( "jtcsvtcorr[taggerpos[2]][0]",                       &var3 );
  reader_th->AddVariable( "jtcsvtcorr[taggerpos[3]][0]",                       &var4 );
 
  string prefix = "TMVAClassification";
   
  // Book method(s)   WH110_BDTD.weights.xml
  
  TString methodName0 = TString("BDTG0") + TString(" method");
  TString methodName1 = TString("BDTG1") + TString(" method");
  TString methodName2 = TString("BDTG2") + TString(" method");
  TString methodName3 = TString("BDTG3") + TString(" method");
  TString methodName4 = TString("BDTG4") + TString(" method");
  
  TString weightfile0_th = TString(bdtpath0) +"/"+ TString("tHnominal_BDTG.weights.xml");
  TString weightfile1_th = TString(bdtpath1) +"/"+ TString("tHnominal_BDTG.weights.xml");
  TString weightfile2_th = TString(bdtpath2) +"/"+ TString("tHnominal_BDTG.weights.xml");
  TString weightfile3_th = TString(bdtpath3) +"/"+ TString("tHnominal_BDTG.weights.xml");
  TString weightfile4_th = TString(bdtpath4) +"/"+ TString("tHnominal_BDTG.weights.xml");
 
  reader_th->BookMVA( methodName0, weightfile0_th ); 
  reader_th->BookMVA( methodName1, weightfile1_th );
  reader_th->BookMVA( methodName2, weightfile2_th ); 
  reader_th->BookMVA( methodName3, weightfile3_th );
  reader_th->BookMVA( methodName4, weightfile4_th );
  
  // read input file
  TFile* ifile = new TFile((ipath+"/"+input).c_str(),"READ");
  if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }
  
  // create temporary output file
  size_t pos = input.find(".root");
  string output = input.substr(0,pos);
  output = output + ".bdt.root";
  TFile* ofile = new TFile((opath+"/"+output).c_str(),"RECREATE");
  if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
  
  // read input directory
  TDirectory* idir    = (TDirectory*)ifile->Get(dirname.c_str());
  cout<<input<<"/"<<dirname<<":"<<endl;
  
  // create output directory
  TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
  if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}
  
  // open input tree
  TTree* itree = (TTree*)idir->Get(treename.c_str());
  if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<dirname<<endl;return 0;}
  
  // don't copy bdtout branch if it already exists
  
  if (0!=itree->FindBranch("tag0_bdtg")) itree->SetBranchStatus("tag0_bdtg",0);
  if (0!=itree->FindBranch("tag1_bdtg")) itree->SetBranchStatus("tag1_bdtg",0);
  if (0!=itree->FindBranch("tag2_bdtg")) itree->SetBranchStatus("tag2_bdtg",0);
  if (0!=itree->FindBranch("tag3_bdtg")) itree->SetBranchStatus("tag3_bdtg",0);
  if (0!=itree->FindBranch("tag4_bdtg")) itree->SetBranchStatus("tag4_bdtg",0);

  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree(0);
  cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;

  // create bdtout branches in output tree
  
  Float_t  tag0_bdtg=-99; otree->Branch("tag0_bdtg",&tag0_bdtg, "tag0_bdtg/F");
  Float_t  tag1_bdtg=-99; otree->Branch("tag1_bdtg",&tag1_bdtg, "tag1_bdtg/F");
  Float_t  tag2_bdtg=-99; otree->Branch("tag2_bdtg",&tag2_bdtg, "tag2_bdtg/F");
  Float_t  tag3_bdtg=-99; otree->Branch("tag3_bdtg",&tag3_bdtg, "tag3_bdtg/F");
  Float_t  tag4_bdtg=-99; otree->Branch("tag4_bdtg",&tag4_bdtg, "tag4_bdtg/F");
 
  //Get relevant information for creating hypothesis
 
  Int_t  taggerpos[140];          itree->SetBranchAddress("taggerpos",     taggerpos);  
  Float_t jtcsvtcorr[40][5];       itree->SetBranchAddress("jtcsvtcorr",   jtcsvtcorr);
  
  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  cout<<nevt<<" events will be processed"<<endl;
  //event loop
  for (unsigned ievt=0;ievt<nevt;ievt++) {
 
    if (ievt%1000 == 0 || ievt <= 10) 
      std::cout << "--- ... Processing event: " << ievt << std::endl;
    
    itree->GetEntry(ievt);

    var1   =  jtcsvtcorr[taggerpos[0]][0];
    var2   =  jtcsvtcorr[taggerpos[1]][0];
    var3   =  jtcsvtcorr[taggerpos[2]][0];
    var4   =  jtcsvtcorr[taggerpos[3]][0];
   
    tag0_bdtg = reader_th->EvaluateMVA( "BDTG0 method"  );
    tag1_bdtg = reader_th->EvaluateMVA( "BDTG1 method"  );
    tag2_bdtg = reader_th->EvaluateMVA( "BDTG2 method"  );
    tag3_bdtg = reader_th->EvaluateMVA( "BDTG3 method"  );
    tag4_bdtg= reader_th->EvaluateMVA( "BDTG4 method"  );
    
    otree->Fill();

  } // event loop
    
  cout << "End of event loop \n";
  
  otree->Write();
  ofile->Close();
  ifile->Close();
  cout << "Wrote file *.bdt.root \n";
  
  delete reader_th;
  delete ifile;
  delete ofile;
  
  string cmd_str = "mv " + opath+"/"+output + " " + opath+"/"+input;
  gSystem->Exec(cmd_str.c_str());
  
  cout << "Override original file... !" << endl;
  cout << "==> TMVAClassificationApplication is done! Hell Yeah!" << endl;

  return 0;

  //  std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;
    
 
}

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

