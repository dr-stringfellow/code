
////////////////////////////////////////////////////////////////////////////////
//
// th_regteacher_x
// ------------
//
// tool to train boosted decision trees (TMVA package) - requires nothin' but root
// this version has been trimmed to only work with MCs
// 
//
//    ::08/15/2012 Christian Boeser
//    ::           Simon Fink
//    ::08/20/2013 Benedikt Maier
////////////////////////////////////////////////////////////////////////////////

#include "../interface/MCSample.h"
//#include "../src/Weights.h"
//#include "SchieferD/AnaUtils/interface/helper-functions.h"
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

  vector<string> mcs           = cl.getVector<string>  ("mcs",                 "");
  string         filesuffix    = cl.getValue <string>  ("filesuffix",     ".root");
  string         treename      = cl.getValue <string>  ("treename",        "t");
  string         datapath      = cl.getValue <string>  ("datapath",            "");
  string         mcpath        = cl.getValue <string>  ("mcpath",              "");
  string         opath         = cl.getValue <string>  ("opath",     "./bdtoutput");
  vector<string> mccuts        = cl.getVector<string>  ("mccuts",              "");
  vector<string> mcweights     = cl.getVector<string>  ("mcweights",           "");
  vector<string> cuts          = cl.getVector<string>  ("cuts",                "");
  vector<string> variables     = cl.getVector<string>  ("variables",           "");
  string         target        = cl.getValue<string>   ("target",              "");
  string         bdtconfig     = cl.getValue<string>   ("bdtconfig",   "!H:V:NTrees=400:nEventsMin=50:MaxDepth=40:BoostType=Bagging:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=20" );
  string         bdtgconfig     = cl.getValue<string>   ("bdtgconfig",   "!H:!V:NTrees=2000::BoostType=Grad:Shrinkage=0.1:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=200:MaxDepth=3:NNodesMax=15");
  
 
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
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples=
    MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0) {
    cout<<endl; MCSample::printSampleInfos(mcsamples); cout<<endl;
  }
  
  //
  // Create a ROOT ouytput file where TMVA will store ntuples, histograms, etc.
  //

  TString outfileName( "Regression.root" );
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

  std::map<std::string,int> Use;


   Use["BDT"]             = 1;
   Use["BDTG"]            = 0;


   std::cout << std::endl;
   std::cout << "==> Start TMVARegression" << std::endl;


  TMVA::Factory *factory = new TMVA::Factory( "TMVARegression", outputFile, "!V:!Silent:Color:DrawProgressBar" );


  // READ IN THE INPUT VARIABLES THAT SHALL BE USED IN THE MVA TRAINING
  
  const unsigned nvar = variables.size();
  
  cout << "used variables: \n"; 
  for (unsigned ivar=0;ivar<nvar;ivar++){
    factory->AddVariable(variables[ivar].c_str());
    cout << variables[ivar].c_str() <<endl;
    }




  cout << "used target:\n";
    factory->AddTarget(target.c_str());
  cout << target.c_str() << endl;
  
 // It is also possible to declare additional targets for multi-dimensional regression, ie:
   // -- factory->AddTarget( "fvalue2" );
   // BUT: this is currently ONLY implemented for MLP

   // Read training and test data (see TMVAClassification for reading ASCII files)
   // load the signal and background event samples from ROOT trees
   //   TFile *input(0);
   //   TString fname = "/storage/9/held/HBB/TEST/WH_125.root";

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

  TCut mycut = (TCut) selection.c_str();
 
  //
  // LOOP AND FEED
  //

  cout<<"Loading Samples:"<<endl;
  for (unsigned imc=0;imc<mcsamples.size();imc++) {

    cout << "sample " << imc << endl;

    MCSample mcsample  = mcsamples[imc];
    // mcsample.setWeights(mcweights);

    for (unsigned itree=0;itree<mcsample.ntrees();itree++) {

      cout<<mcsample.filename(itree)<<endl;

      // retrieve tree
      TTree* tree = mcsample.tree(itree);

      // define weights
      // vector<string> sweights;
      // vector<float>  fweights;

     // setWeights(mcweights,mcsample.name(),sweights,fweights);

    
      //
      // ADD REGRESSION TREES TO FACTORY
      //
      Double_t regWeight = 1.0;
      
      factory->AddRegressionTree(tree , regWeight );
      
      //TChain *input = new TChain("tree");
      //input->Add("/storage/9/held/HBB/lpchbb/dlopes/Step2_5May12/WH_*.root");


   /*
   if (!gSystem->AccessPathName( fname )) 
      input = TFile::Open( fname ); // check if file in local directory exists
   else 
      input = TFile::Open( "http://root.cern.ch/files/tmva_reg_example.root" ); // if not: download from ROOT server
   */
      cout<<"done."<<endl;
    } //itree<ntrees
  } //imc<mcsamples

  cout << "\n--------------------\n " <<  mycut <<endl;

  factory->PrepareTrainingAndTestTree( mycut, "nTrain_Regression=0:nTest_Regression=0:SplitMode=Random:NormMode=NumEvents:!V" );



   // PDE - RS method
   if (Use["PDERS"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERS", 
                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=40:NEventsMax=60:VarTransform=None" );
   // And the options strings for the MinMax and RMS methods, respectively:
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );   
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );   

   if (Use["PDEFoam"])
       factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam", 
			    "!H:!V:MultiTargetRegression=F:TargetSelection=Mpv:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Compress=T:Kernel=None:Nmin=10:VarTransform=None" );

   // K-Nearest Neighbour classifier (KNN)
   if (Use["KNN"])
      factory->BookMethod( TMVA::Types::kKNN, "KNN", 
                           "nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

   // Linear discriminant
   if (Use["LD"])
      factory->BookMethod( TMVA::Types::kLD, "LD", 
                           "!H:!V:VarTransform=None" );

	// Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"]) 
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MC",
                          "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=MC:SampleSize=100000:Sigma=0.1:VarTransform=D" );
   
   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options) .. the formula of this example is good for parabolas
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GA",
                           "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=GA:PopSize=100:Cycles=3:Steps=30:Trim=True:SaveBestGen=1:VarTransform=Norm" );

   if (Use["FDA_MT"]) 
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MT",
                           "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

   if (Use["FDA_GAMT"]) 
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT",
                           "!H:!V:Formula=(0)+(1)*x0+(2)*x1:ParRanges=(-100,100);(-100,100);(-100,100):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

   // Neural network (MLP)
   if (Use["MLP"])
      factory->BookMethod( TMVA::Types::kMLP, "MLP", "!H:!V:VarTransform=Norm:NeuronType=tanh:NCycles=20000:HiddenLayers=N+20:TestRate=6:TrainingMethod=BFGS:Sampling=0.3:SamplingEpoch=0.8:ConvergenceImprove=1e-6:ConvergenceTests=15:!UseRegulator" );

   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

   // Boosted Decision Trees
     if (Use["BDT"])
       factory->BookMethod( TMVA::Types::kBDT, "BDT", bdtconfig.c_str());

// Boosted Decision Trees with gradient boosting
//       factory->BookMethod( TMVA::Types::kBDT, "BDTG","H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:PruneMethod=CostComplexity:PruneStrength=50:NNodesMax=5" );  // training schlecht

// Boosted Decision Trees with bagging
       //    factory->BookMethod( TMVA::Types::kBDT, "BDTB","H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" ); //schlecht


    if (Use["BDTG"])
     factory->BookMethod( TMVA::Types::kBDT, "BDTG", bdtgconfig.c_str());

   //    std::cout << "  !!!!!! " << max(20,NEvtsTrain/NVar^2/10) << " !!!!! " << std::endl;
   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();    

   // --------------------------------------------------------------
   
   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVARegression is done!" << std::endl;   


   delete factory;

   // Launch the GUI for the root macros
   //if (!gROOT->IsBatch()) TMVARegGui( outfileName );

   return 0;
}
