
////////////////////////////////////////////////////////////////////////////////
//
// th_toprecteacher_x
// ------------
//
// tool to train boosted decision trees (TMVA package) - requires nothin' but root
// this version has been trimmed to only work with MCs
// 
//
//    ::08/15/2012 Christian Boeser
//    ::           Simon Fink
//    ::           Benedikt Maier
////////////////////////////////////////////////////////////////////////////////

#include "../interface/CommandLine.h"
#include "../interface/Histogram.h"
#include "../interface/DataSample.h"
#include "../interface/MCSample.h"
//#include "../interface/MatrixMethod.h"


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <map>

#include <TMath.h>
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
#include "TLorentzVector.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"

using namespace std;
//using namespace helpers;



string get_cut(const vector<string>& cuts,unsigned icut,const string& mode)
{
  if (icut>=cuts.size()) return string("");
  string tmp(cuts[icut]);
  size_t pos;
  pos = tmp.find(':');
  if (pos==string::npos) return tmp;
  string cutlabel = tmp.substr(0,pos);
  string cutexpression= tmp.substr(pos+1);
  return (mode.find("label")==string::npos) ? cutexpression : cutlabel;
}

int main(int argc,char**argv)
{
  cout << "Und los gehts\n";
    
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  string         signal         = cl.getValue <string>  ("signal"                     );
  string         test_signal    = cl.getValue <string>  ("test_signal"                );
  string         background     = cl.getValue <string>  ("background"                 );
  string         test_background= cl.getValue <string>  ("test_background"            );
  string         treename       = cl.getValue <string>  ("treename",               "t");
  string         opath          = cl.getValue <string>  ("opath",         "./bdt_reco");
  vector<string> sigcuts        = cl.getVector<string>  ("sigcuts",    "hyp_isbest==1");
  //  vector<string> mcweights     = cl.getVector<string>  ("mcweights",              "");
  vector<string> bckcuts        = cl.getVector<string>  ("bckcuts",    "hyp_isbest==0");
  vector<string> variables      = cl.getVector<string>  ("variables",               "");
  string         bdtconfig      = cl.getValue<string>   ("bdtconfig", "!H:!V:NTrees=200:nEventsMin=100:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.3:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning");
  //  string         bdtgconfig     = cl.getValue<string>  ("bdtgconfig",          "");
  
 
  //  if(!cl.check()) return 0;
  cl.print();
 
  
  TMVA::Tools::Instance();
  // you can read in data and MC files on equal footing
  // and then specify what exactly is supposed to be treated as *signal*
  // density training is not yet supported

  // move all output to a NB working directory
  string cmdmkdir = "mkdir -p " + opath;
  gSystem->Exec(cmdmkdir.c_str());
  string execdir = gSystem->WorkingDirectory();
  if (!gSystem->ChangeDirectory(opath.c_str())) {
    cout<<"Could not open opath: "<<opath<<endl; return 0;
  }

  string dirname = "utm";

  //
  // OPEN SIGNAL HYP FILE
  //
  
  // TRAIN FILE HYP==1
  TFile* sigfile = TFile::Open((signal).c_str(),"READ");
  if (!sigfile->IsOpen()) { cout<<"Failed to open "<<signal<<endl; return 0; }

  TDirectory* sigdir    = (TDirectory*)sigfile->Get(dirname.c_str());
  cout<<signal<<"/"<<dirname<<":"<<endl;
  
  TTree* sigtree = (TTree*)sigdir->Get(treename.c_str());
  if(0==sigtree){cout<<"No "<<treename<<" in "<<signal<<":"<<dirname<<endl;return 0;}


  // TEST FILE HYP==1
  TFile* test_sigfile = TFile::Open((test_signal).c_str(),"READ");
  if (!test_sigfile->IsOpen()) { cout<<"Failed to open "<<test_signal<<endl; return 0; }

  TDirectory* test_sigdir    = (TDirectory*)test_sigfile->Get(dirname.c_str());
  cout<<test_signal<<"/"<<dirname<<":"<<endl;
  
  TTree* test_sigtree = (TTree*)test_sigdir->Get(treename.c_str());
  if(0==test_sigtree){cout<<"No "<<treename<<" in "<<test_signal<<":"<<dirname<<endl;return 0;}


  // TRAIN FILE HYP==0
  TFile* bkgfile = TFile::Open((background).c_str(),"READ");
  if (!bkgfile->IsOpen()) { cout<<"Failed to open "<<background<<endl; return 0; }

  TDirectory* bkgdir    = (TDirectory*)bkgfile->Get(dirname.c_str());
  cout<<background<<"/"<<dirname<<":"<<endl;
  
  TTree* bkgtree = (TTree*)bkgdir->Get(treename.c_str());
  if(0==bkgtree){cout<<"No "<<treename<<" in "<<background<<":"<<dirname<<endl;return 0;}


  // TEST FILE HYP==0
  TFile* test_bkgfile = TFile::Open((test_background).c_str(),"READ");
  if (!test_bkgfile->IsOpen()) { cout<<"Failed to open "<<test_background<<endl; return 0; }

  TDirectory* test_bkgdir    = (TDirectory*)test_bkgfile->Get(dirname.c_str());
  cout<<test_background<<"/"<<dirname<<":"<<endl;
  
  TTree* test_bkgtree = (TTree*)test_bkgdir->Get(treename.c_str());
  if(0==test_bkgtree){cout<<"No "<<treename<<" in "<<test_background<<":"<<dirname<<endl;return 0;}


  //
  // Create a ROOT ouytput file where TMVA will store ntuples, histograms, etc.
  //

  TString outfileName( (opath+"BDTreco.root").c_str() );
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

  std::map<std::string,int> Use;
  Use["BDT"]             = 1;
  Use["BDTG"]            = 0;
  

  std::cout << std::endl;
   std::cout << "==> Start TMVARegression" << std::endl;

   TMVA::Factory *factory = new TMVA::Factory( "BDTreco_expert", outputFile,  // I;D;P;G,D
					       "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );


  // READ IN THE INPUT VARIABLES THAT SHALL BE USED IN THE MVA TRAINING
  
  const unsigned nvar = variables.size();
  
  cout << "used variables: \n"; 

  factory->AddSpectator("tophadjtidx[0]");
  factory->AddSpectator("tophadjtidx[1]");
  factory->AddSpectator("tophadjtidx[2]");
  factory->AddSpectator("toplepjtidx");
  for (unsigned ivar=0;ivar<nvar;ivar++){
    factory->AddVariable(variables[ivar].c_str());
    cout << variables[ivar].c_str() <<endl;
    }
  
  //
  // DEFINE SELECTION
  //

  string sig_selection = "";
  string bck_selection = "";
  for (unsigned icuts=0;icuts<sigcuts.size();icuts++) {
    if (!sig_selection.empty()) sig_selection += "&&";
    sig_selection += get_cut(sigcuts,icuts,"expression");
  }
  for (unsigned icuts=0;icuts<bckcuts.size();icuts++) {
    if (!bck_selection.empty()) bck_selection += "&&";
    bck_selection += get_cut(bckcuts,icuts,"expression");
  }
  
  TCut mycut_sig = (TCut) sig_selection.c_str();
  TCut mycut_bck = (TCut) bck_selection.c_str();
 
  //
  // LOOP AND FEED
  //

  cout<<"Loading Sample:"<<endl;

  TTree *signaltree     =  sigtree;
  factory->AddSignalTree(signaltree, 1.0, "Training");
  cout << "is signal\n";
  
  TTree *backgroundtree =  bkgtree;
  factory->AddBackgroundTree (backgroundtree, 1.0, "Training");
  cout << "is background\n";

  TTree *test_signaltree = test_sigtree;
  factory->AddSignalTree(test_signaltree, 1.0, "Test");
  cout << "is testing signal\n";
  
  TTree *test_backgroundtree = test_bkgtree;
  factory->AddBackgroundTree(test_backgroundtree,1.0,"Test");
  cout << "is testing background\n";

  
  factory->SetBackgroundWeightExpression("hyp_weight");
  factory->SetSignalWeightExpression("hyp_weight");
  
  factory->PrepareTrainingAndTestTree( mycut_sig,mycut_bck,"nTrain_Signal=0:nTrain_Background=0:nTest_Signal=0:nTest_Background=0:SplitMode=Random:NormMode=EqualNumEvents:!V" );

  string BDT_str = bdtconfig;
  cout<<"Evaluating the training"<<endl;
  //factory->BookMethod( TMVA::Types::kBDT, "BDTD", BDTD_str.c_str());                                                                                                                             
  //factory->BookMethod( TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:PruneMethod=ExpectedError:PruneStrength=3");                                                                                                                                                                           
  cout << "0" << endl; 
  factory->BookMethod( TMVA::Types::kBDT, "BDT",  BDT_str.c_str());
  cout << "1" << endl;
  factory->TrainAllMethods();
  cout << "2" << endl;
  factory->TestAllMethods();
  cout << "3" << endl;
  factory->EvaluateAllMethods();
  cout << "4" << endl;

  // Save the output                                                                                                                                                                                 
  outputFile->Close();
  sigfile->Close();
  bkgfile->Close();
  std::cout << "==> Wrote root file: " << opath.c_str()<<"/"<<outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done! Hell Yeah!" << std::endl;

   delete factory;

   // Launch the GUI for the root macros
   //if (!gROOT->IsBatch()) TMVARegGui( outfileName );

   return 0;
}


