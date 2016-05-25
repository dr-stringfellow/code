////////////////////////////////////////////////////////////////////////////////
//
// th_bdtteacher_x
// ------------
//
// tool to train boosted decision trees (TMVA package) - requires nothin' but root
// this version has been trimmed to only work with MCs
// 
//
//   
//    ::08/22/2013 Benedikt Maier <bmaier@ekp.uni-karlsruhe.de>::
////////////////////////////////////////////////////////////////////////////////

#include "../interface/MCSample.h"
//#include "../src/Weights.h"

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


#include "TMath.h"
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
#include "/portal/ekpcms5/home/cboeser/roots/root_5.34/tmva/test/TMVAGui.C"


#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
////////////////////////////////////////////////////////////////////////////////
double deltaPhi(double phi1, double phi2);
double deltaR(double eta1, double phi1, double eta2, double phi2);

/// get sample weights
double sampleWeight(TTree* t);

/// decompose variables and prepro configurations
bool decompose_nbvariables(vector<string>& vvin,
			   vector<string>& vars,vector<int>& prepros);

/// write decomposed variables into pseudocodegen file
bool write_codegen(const vector<string>& vars,const vector<int>& prepros);


/// check if a file is to be considered signal sample
/// if no signals specified returns true (assuming signal cuts later on)
bool check_signal(const string& mcname,const vector<string>& signals);

string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression");

/// slightly awkward function to retrieve and decode the mc event weights
void setWeights(const vector<string>& mcweights,const string& name,
		vector<string>& sweights,vector<float>&  fweights);

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  
    
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> data          = cl.getVector<string>  ("data",                "");
  vector<string> mcs           = cl.getVector<string>  ("mcs",                 "");
  string         dirname       = cl.getValue <string>  ("dirname",          "utm");
  string         filesuffix    = cl.getValue <string>  ("filesuffix",     ".root");
  string         treename      = cl.getValue <string>  ("treename",           "t");
  string         datapath      = cl.getValue <string>  ("datapath",            "");
  string         mcpath        = cl.getValue <string>  ("mcpath",              "");
  string         mcpath_train  = cl.getValue <string>  ("mcpath_train",        "");
  string         opath         = cl.getValue <string>  ("opath",     "./nboutput");
  string         mass          = cl.getValue <string>  ("mass",                "");
  string         prunestrength = cl.getValue<string>   ("prunestrength",      "0");
  string         prunemethod   = cl.getValue<string>   ("prunemethod","NoPruning");
  string         shrinkage     = cl.getValue<string>   ("shrinkage",       "0.30");
  string         methods       = cl.getValue<string>   ("methods",          "all");
  vector<string> mccuts        = cl.getVector<string>  ("mccuts",              "");
  vector<string> mcweights     = cl.getVector<string>  ("mcweights",           "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> signals   = cl.getVector<string>  ("signals",             "");
  vector<string> variables = cl.getVector<string>  ("variables",           "");

  bool           doanalysis= cl.getValue<bool>     ("doanalysis",        true);

  //int            niter     = cl.getValue<int>      ("niter",              100);
  //float          sigweight = cl.getValue<float>    ("sigweight",          1.0);
  //float          bkgweight = cl.getValue<float>    ("bkgweight",          1.0);
  float          lumi      = cl.getValue<float>    ("lumi",            10000.);
  string         ntrees    = cl.getValue<string>   ("ntrees",           "100");
  string         nevtmin   = cl.getValue<string>   ("nevtmin",          "100");
  string     adaboostbeta  = cl.getValue<string>   ("adaboostbeta",       "1");
  //float          nbevtfraction= cl.getValue<float> ("nbevtfraction",      1.0);
  //int            nbweightupdate=cl.getValue<int>   ("nbweightupdate",     100);
  //int            addlayer  = cl.getValue <int>     ("addlayer",             1);

  vector<string> mclabels  = cl.getVector<string>  ("mclabels",     "");//dummy
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",     "");//dummy
  vector<string> labels    = cl.getVector<string>  ("labels",       "");//dummy



  if(!cl.check()) return 0;
  cl.print();
  TMVA::Tools::Instance();
  // you can read in data and MC files on equal footing
  // and then specify what exactly is supposed to be treated as *signal*
  // density training is not yet supported

  if (signals.size()==0) {
    cout<<"Error: No target defined!"<<endl; return 0;
  }

  // consider also individual MC weights (eg QCDFlat samples)
  //mcweights.push_back("mcweight");

  // move all output to a working directory
  string cmdmkdir = "mkdir -p " + opath;
  gSystem->Exec(cmdmkdir.c_str());
  string execdir = gSystem->WorkingDirectory();
  if (!gSystem->ChangeDirectory(opath.c_str())) {
    cout<<"Could not open opath: "<<opath<<endl; return 0;
  }

  //
  // OPEN MC FILES (TEST AND TRAIN)/ RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples=
    MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0) {
    cout<<endl << "Test/Evaluation Samples:\n";
    cout<<endl; MCSample::printSampleInfos(mcsamples); cout<<endl;
  }
  
  vector<MCSample> mcsamples_train=
    MCSample::loadSamples(mcs,dirname,treename,mcpath_train,filesuffix);
  if (mcsamples_train.size()>0) {
    cout<<endl << "Training Samples:\n";
    cout<<endl; MCSample::printSampleInfos(mcsamples_train); cout<<endl;
  }

  //
  // Create a ROOT ouytput file where TMVA will store ntuples, histograms, etc.
  //

  TString outfileName( "BDT_tH" + mass + ".root" );
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );


  cout<<"======================"<<endl
      <<"Start TMVA Setup"<<endl
      <<"======================"<<endl
      <<endl<<endl;

  // The second argument is the output file for the training results
  // All TMVA output can be suppressed by removing the "!" (not) in
  // front of the "Silent" argument in the option string
  TMVA::Factory *factory = new TMVA::Factory( ("tH"+mass).c_str(), outputFile,  // I;D;P;G,D
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D:AnalysisType=Classification" );


  const unsigned nvar = variables.size();
 
  // READ IN THE INPUT VARIABLES THAT SHALL BE USED IN THE MVA TRAINING

  cout << "used variables: \n"; 


  

  //Nominal Analysis

    factory->AddSpectator("topjtidx");
    factory->AddSpectator("ljtidx");
    factory->AddSpectator("hbbjtidx[0]");
    factory->AddSpectator("hbbjtidx[1]");
    
    factory->AddSpectator("toplepjtidx");
    factory->AddSpectator("tophadjtidx[0]");
    factory->AddSpectator("tophadjtidx[1]");
    factory->AddSpectator("tophadjtidx[2]");
    
  

  /*

  // Fancy Analysis

  factory->AddSpectator("taggerpos[0]");
  factory->AddSpectator("taggerpos[1]");
  factory->AddSpectator("taggerpos[2]");
  factory->AddSpectator("taggerpos[3]");

  */

  for (unsigned ivar=0;ivar<nvar;ivar++){
    factory->AddVariable(variables[ivar].c_str());
  }

  //
  // DEFINE SELECTION
  //

  string selection = "";
  for (unsigned icuts=0;icuts<mccuts.size();icuts++) {
    if (!selection.empty()) selection += "&&";
    selection += get_cut(mccuts,icuts,"expression");
  }
  for (unsigned icuts=0;icuts<cuts.size();icuts++) {
    if (!selection.empty()) selection += "&&";
    selection += get_cut(cuts,icuts,"expression");
  }

  TCut cut_s = (TCut) selection.c_str();
  TCut cut_b = cut_s;

  //int signalevents=0;
  //int backgroundevents=0;
  //
  // LOOP AND FEED
  //
  for (unsigned icuts=0;icuts<mcweights.size();icuts++) {
    cout << "WEIGHT " << icuts << " : " << mcweights.at(icuts)<<endl;
     }
 
  cout<<"Loading Samples Training:"<<endl;
  for (unsigned imc=0;imc<mcsamples_train.size();imc++) {

    cout << "sample " << imc << endl;

    MCSample mcsample  = mcsamples_train[imc];
    mcsample.setWeights(mcweights);
    bool isSignal = false;
    for (unsigned i=0;i<signals.size();i++) 
      if (signals[i]==mcsample.name()) isSignal = true;
    

    for (unsigned itree=0;itree<mcsample.ntrees();itree++) {

      cout<<mcsample.filename(itree)<<endl;

      // retrieve tree
      TTree* tree = mcsample.tree(itree);

      // define weights
      vector<string> sweights;
      vector<float>  fweights;

      setWeights(mcweights,mcsample.name(),sweights,fweights);

      double sampleweight = sampleWeight(tree); // sampleweight is event independent!!!
      sampleweight*=lumi;
      for (unsigned i=0;i<fweights.size();i++) sampleweight *= fweights[i];
          
      //
      // ADD SIGNAL AND BACKGROUND TREES TO FACTORY
      //

      if(isSignal){
	TTree *signal     =  tree;
	//	sampleweight *= makeeven_b/makeeven_s;
	factory->AddSignalTree(signal, sampleweight, "Training");
	cout << "is signal\n";
      }

      else{
	TTree *background =  tree;
	factory->AddBackgroundTree (background, sampleweight, "Training");
	cout << "is background\n";
      }
      cout<<"done."<<endl;
    } //itree<ntrees
  } //imc<mcsamples

  cout<<"Loading Samples Test/Evaluation:"<<endl;
  for (unsigned imc=0;imc<mcsamples.size();imc++) {

    cout << "sample " << imc << endl;

    MCSample mcsample  = mcsamples[imc];
    mcsample.setWeights(mcweights);
    bool isSignal = false;
    for (unsigned i=0;i<signals.size();i++) 
      if (signals[i]==mcsample.name()) isSignal = true;
    

    for (unsigned itree=0;itree<mcsample.ntrees();itree++) {

      cout<<mcsample.filename(itree)<<endl;

      // retrieve tree
      TTree* tree = mcsample.tree(itree);

      // define weights
      vector<string> sweights;
      vector<float>  fweights;

      setWeights(mcweights,mcsample.name(),sweights,fweights);

      double sampleweight = sampleWeight(tree); // sampleweight is event independent!!!
      sampleweight*=lumi;
      for (unsigned i=0;i<fweights.size();i++) sampleweight *= fweights[i];
          
      //
      // ADD SIGNAL AND BACKGROUND TREES TO FACTORY
      //

      if(isSignal){
	TTree *signal     =  tree;
	//	sampleweight *= makeeven_b/makeeven_s;
	factory->AddSignalTree(signal, sampleweight, "Test");
	cout << "is signal\n";
      }

      else{
	TTree *background =  tree;
	//if(backgroundchecker==0)sampleweight *=1000000000000000;
	factory->AddBackgroundTree (background, sampleweight, "Test");
	cout << "is background\n";
      }
      cout<<"done."<<endl;
    } //itree<ntrees
  } //imc<mcsamples

  

  //
  // PERFORM THE TRAINING
  // 
/*
  backgroundevents=0.5*backgroundevents;
  signalevents=0.5*signalevents;

  std::ostringstream s;
  s << signalevents;
  std::ostringstream b;
  b << backgroundevents;

  string sigevt=s.str();
  string bkgevt=b.str();

  cout << "!!!!!!!!!!!!!JFKLSFLKJSFLKJSFLK\n sig: " << sigevt << " back: " << bkgevt << endl;
   */

  string weight_expr="";
  for(unsigned iweights=0;iweights<mcweights.size();iweights++) {
    if (!weight_expr.empty()) weight_expr += "*";
    weight_expr += mcweights[iweights];
  }

  cout << "#############################"<< endl;
  cout << "weight expression:" << endl;
  cout << weight_expr << endl;
  cout << "#############################"<< endl;


  //  factory->SetBackgroundWeightExpression("weightPU*weightTrig");
  //  factory->SetSignalWeightExpression("weightPU*weightTrig");

  factory->SetBackgroundWeightExpression(weight_expr);
  factory->SetSignalWeightExpression(weight_expr);


  //perform training
  factory->PrepareTrainingAndTestTree( cut_s,cut_b,"nTrain_Signal=0:nTrain_Background=0:nTest_Signal=0:nTest_Background=0:SplitMode=Random:NormMode=EqualNumEvents:!V" );
  
  if (doanalysis) {
    string BDT_str = "!H:!V:NTrees="+ntrees+":nEventsMin="+nevtmin+":MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta="+adaboostbeta+":SeparationType=GiniIndex:nCuts=20:PruneMethod="+prunemethod+":PruneStrength="+prunestrength;
    cout<<"Evaluating the training"<<endl;
    
    


    //METHODS
    
    if(methods=="all")
      {
	factory->BookMethod( TMVA::Types::kBDT, "BDT",  BDT_str.c_str());
	factory->BookMethod( TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees="+ntrees+":BoostType=Grad:Shrinkage="+shrinkage+":UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:PruneMethod=ExpectedError:PruneStrength=3");
	factory->BookMethod( TMVA::Types::kBDT, "BDTB", "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
	factory->BookMethod( TMVA::Types::kBDT, "BDTD", "!H:!V:NTrees="+ntrees+":MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );
      }

    //if(methods=="BDT")
    //  factory->BookMethod( TMVA::Types::kBDT, "BDT",  BDT_str.c_str());

    if(methods=="BDTG")
      factory->BookMethod( TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees="+ntrees+":BoostType=Grad:Shrinkage="+shrinkage+":UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:PruneMethod=ExpectedError:PruneStrength=3");
    
    if(methods=="BDTD")
      factory->BookMethod( TMVA::Types::kBDT, "BDTD", "!H:!V:NTrees="+ntrees+":MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

    if (methods=="SVM")
      factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

    if (methods=="MLPBFGS")
      factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

    //factory->BookMethod( TMVA::Types::kBDT, "BDTB", "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
    //factory->BookMethod( TMVA::Types::kMLP, "MLP", "!H:!V:VarTransform=Norm:NeuronType=tanh:NCycles=20000:HiddenLayers=N+20:TestRate=6:TrainingMethod=BFGS:Sampling=0.3:SamplingEpoch=0.8:ConvergenceImprove=1e-6:ConvergenceTests=15:!UseRegulator" );


    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
  }
  
  // Save the output
  outputFile->Close();
  
  std::cout << "==> Wrote root file: " << opath.c_str()<<"/"<<outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done! Hell Yeah!" << std::endl;
  
  delete factory;
  
  // Launch the GUI for the root macros
  if (!gROOT->IsBatch()) cout << "TMVA GUI PART" << endl; 
    TMVAGui( outfileName );
  
  
  cout<<"DONE."<<endl;

  //================
  //TMVA GUI PART



  //================

  gSystem->ChangeDirectory(execdir.c_str());
  return 0;
}





////////////////////////////////////////////////////////////////////////////////
// help implementations
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
bool check_signal(const string& mcname,const vector<string>& signals)
{
  for (unsigned i=0;i<signals.size();i++) 
    if (signals[i]==mcname) return true;
  return false;
}


//______________________________________________________________________________
bool decompose_nbvariables(vector<string>& vvin,
			   vector<string>& vars,vector<int>& prepros)
{
  if (0==vvin.size()) return false;

  for (unsigned itin=0;itin<vvin.size();itin++) {

    string tmp(vvin[itin]);
    size_t pos = tmp.find(":");
    string varname = tmp.substr(0,pos);
    vars.push_back(varname);
    if (pos==string::npos) {
      prepros.push_back(34);
      continue;
    }
    
    tmp = tmp.substr(pos+1);

    if (tmp.find_first_not_of("0123456789")!=string::npos) return false;
    stringstream sstmp; sstmp<<tmp; int iprepro; sstmp>>iprepro;
    prepros.push_back(iprepro);

  }
  return true;
}



//______________________________________________________________________________
bool write_codegen(const vector<string>& vars,const vector<int>& prepros)
{
  
  if (vars.size()!=prepros.size()) return false;
  if (vars.size()==0) return false;

  stringstream ss;
  ss<<"VARSET 1"<<endl;
  for (unsigned i=0;i<vars.size();i++) ss<<vars[i]<<" "<<prepros[i]<<endl;
  ss<<"ENDVARSET"<<endl
    <<"NETWORK classify 1"<<endl;

  ofstream file;
  file.open("pseudocodegen",ofstream::trunc);
  if (!file.is_open()) return false;

  file<<ss.str().c_str();
  file.close();

  cout<<"Created NB file: pseudocodegen"<<endl;

  return true;
}


//______________________________________________________________________________
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

//______________________________________________________________________________
void setWeights(const vector<string>& mcweights,
		const string& name,
		vector<string>& sweights,
		vector<float>&  fweights)
{
  for (vector<string>::const_iterator it=mcweights.begin();it!=mcweights.end();++it) {
    string weight(*it);
    size_t pos = weight.find(":");
    if (pos==string::npos) {
      float ff; stringstream ss; ss<<weight;
      if (ss>>ff) {assert(!isnan(ff));fweights.push_back(ff);}
      else        {sweights.push_back(weight);}
    }
    else if (weight.substr(0,pos)==name) {
      weight = weight.substr(pos+1);
      float ff; stringstream ss; ss<<weight;
      if (ss>>ff) {assert(!isnan(ff));fweights.push_back(ff);}
      else        {sweights.push_back(weight);}
    }
  }
  return;
}

//__________________________________________________________________________

double sampleWeight(TTree* t)
{
  double weight;
  bool result(false);
  double xsection;   if (MCSample::getXsection(t,xsection)) result = true;
  double efficiency; MCSample::getEfficiency(t,efficiency);
  double kfactor;    MCSample::getKfactor(t,kfactor);
  weight = xsection*kfactor*efficiency/(double)t->GetEntries();
  return weight;
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


//TMVA GUI PART
