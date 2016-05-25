/**
 * The program trains an MVA to perform jet assignment for semileptonic decay of a top quark.
 */

#include <TMVA/Factory.h>

#include <TFile.h>

#include <string>
#include <memory>


using namespace std;


int main()
{
    // Create a log file and a factory
    string const jobName("GlobalTHReco");
    TFile infoFile((string("info") + jobName + ".root").c_str(), "recreate");
    TMVA::Factory factory(jobName, &infoFile, "Color=True");
    
    
    // Specifiy signal and background trees and event weights
    TFile trainFile("tuples/thq-nc-mg_train.root");
    unique_ptr<TTree> sgnTrainTree(dynamic_cast<TTree *>(trainFile.Get("VarsSgn")));
    unique_ptr<TTree> bkgTrainTree(dynamic_cast<TTree *>(trainFile.Get("VarsBkg")));
    
    TFile examFile("tuples/thq-nc-mg_exam.root");
    unique_ptr<TTree> sgnExamTree(dynamic_cast<TTree *>(examFile.Get("VarsSgn")));
    unique_ptr<TTree> bkgExamTree(dynamic_cast<TTree *>(examFile.Get("VarsBkg")));
    
    factory.AddSignalTree(sgnTrainTree.get(), 1., TMVA::Types::kTraining);
    factory.AddBackgroundTree(bkgTrainTree.get(), 1., TMVA::Types::kTraining);
    factory.AddSignalTree(sgnExamTree.get(), 1., TMVA::Types::kTesting);
    factory.AddBackgroundTree(bkgExamTree.get(), 1., TMVA::Types::kTesting);
    
    factory.SetWeightExpression("Weight");
    
    
    // Specify the input variables
    factory.AddVariable("log(Mass_Higgs)", 'F');
    factory.AddVariable("log(Mass_BTopLep)", 'F');
    factory.AddVariable("abs(Eta_Recoil)", 'F');
    factory.AddVariable("PassBTag_BTop", 'F');
    factory.AddVariable("NumBTag_Higgs", 'F');
    factory.AddVariable("DeltaR_BJetsHiggs", 'F');
    factory.AddVariable("DeltaR_BTopW", 'F');
    factory.AddVariable("DeltaR_TopHiggs", 'F');
    factory.AddVariable("RelHt", 'F');
    factory.AddVariable("MaxEta_BHiggs", 'F');
    factory.AddVariable("log(MinPt_BHiggs)", 'F');
    factory.AddVariable("Charge_BTop", 'F');
    
    
    // Make the factory copy the trees
    factory.PrepareTrainingAndTestTree("", "NormMode=EqualNumEvents");
    
    
    // Book a BFGS NN
    factory.BookMethod(TMVA::Types::kMLP, jobName + "_BFGS",
     "VarTransform=N:NeuronType=tanh:NCycles=500:HiddenLayers=30:TrainingMethod=BFGS:TestRate=5");
    
    
    // Train and test the MVA
    factory.TrainAllMethods();
    factory.TestAllMethods();
    factory.EvaluateAllMethods();
    
    return 0;
}
