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
    string const jobName("THvsBkg_Global");
    TFile infoFile((string("info") + jobName + ".root").c_str(), "recreate");
    TMVA::Factory factory(jobName, &infoFile, "Color=True");
    
    
    // Specifiy signal and background trees and event weights
    string const filePrefix("../tuples/");
    
    TFile sgnTrainFile((filePrefix + "sgn_train.root").c_str());
    unique_ptr<TTree> sgnTrainTree(dynamic_cast<TTree *>(sgnTrainFile.Get("Vars")));
    
    TFile sgnExamFile((filePrefix + "sgn_exam.root").c_str());
    unique_ptr<TTree> sgnExamTree(dynamic_cast<TTree *>(sgnExamFile.Get("Vars")));
    
    TFile bkgTrainFile((filePrefix + "bkg_train.root").c_str());
    unique_ptr<TTree> bkgTrainTree(dynamic_cast<TTree *>(bkgTrainFile.Get("Vars")));
    
    TFile bkgExamFile((filePrefix + "bkg_exam.root").c_str());
    unique_ptr<TTree> bkgExamTree(dynamic_cast<TTree *>(bkgExamFile.Get("Vars")));
    
    factory.AddSignalTree(sgnTrainTree.get(), 1., TMVA::Types::kTraining);
    factory.AddBackgroundTree(bkgTrainTree.get(), 1., TMVA::Types::kTraining);
    factory.AddSignalTree(sgnExamTree.get(), 1., TMVA::Types::kTesting);
    factory.AddBackgroundTree(bkgExamTree.get(), 1., TMVA::Types::kTesting);
    
    factory.SetWeightExpression("Weight");
    
    
    // Specify the input variables (set v3a)
    factory.AddVariable("glb_Charge_Lep", 'F');
    factory.AddVariable("abs(tHq_Eta_Recoil)", 'F');
    factory.AddVariable("tHq_NumBTag_Higgs", 'F');
    factory.AddVariable("log(tHq_Pt_Higgs)", 'F');
    factory.AddVariable("log(tHq_Pt_Recoil)", 'F');
    factory.AddVariable("tt_DeltaR_Light", 'F');
    factory.AddVariable("log(tt_Mass_TopHad)", 'F');
    factory.AddVariable("tt_NumPassBTag_Light", 'F');
    
    
    // // Specify the input variables (set v3b)
    // factory.AddVariable("glb_Charge_Lep", 'F');
    // factory.AddVariable("log(glb_Pt_J1)", 'F');
    // factory.AddVariable("log(glb_MET)", 'F');
    // factory.AddVariable("log(tHq_Pt_Higgs)", 'F');
    // factory.AddVariable("log(tHq_Pt_Recoil)", 'F');
    // factory.AddVariable("abs(tHq_Eta_Recoil)", 'F');
    // factory.AddVariable("tt_MaxEta_Light", 'F');
    // factory.AddVariable("log(tt_Mass_TopHad)", 'F');
    // factory.AddVariable("log(tt_DMass_TopHadWHad)", 'F');
    // factory.AddVariable("log(tt_MaxMass_BTopHadLight)", 'F');
    // factory.AddVariable("tHq_NumBTag_Higgs", 'F');
    // factory.AddVariable("tt_NumPassBTag_Light", 'F');
    // factory.AddVariable("tHq_DeltaR_BJetsHiggs", 'F');
    // factory.AddVariable("tt_DeltaR_Light", 'F');
    // factory.AddVariable("tt_SumCharge_Light", 'F');
    // factory.AddVariable("tt_Charge_BTopHad", 'F');
    
    
    // // Specify the input variables (set v3c)
    // factory.AddVariable("glb_Charge_Lep", 'F');
    // factory.AddVariable("log(glb_SqrtSHat)", 'F');
    // factory.AddVariable("log(glb_Pt_J1)", 'F');
    // factory.AddVariable("log(glb_MET)", 'F');
    // factory.AddVariable("log(tHq_Pt_Higgs)", 'F');
    // factory.AddVariable("log(tHq_Pt_Recoil)", 'F');
    // factory.AddVariable("tHq_Cos_LepRecoil_TH", 'F');
    // factory.AddVariable("abs(tHq_Eta_Recoil)", 'F');
    // factory.AddVariable("glb_Sphericity", 'F');
    // factory.AddVariable("tt_MaxEta_Light", 'F');
    // factory.AddVariable("log(tt_Mass_TopHad)", 'F');
    // factory.AddVariable("log(tt_DMass_TopHadWHad)", 'F');
    // factory.AddVariable("log(tt_MaxMass_BTopHadLight)", 'F');
    // factory.AddVariable("tHq_NumBTag_Higgs", 'F');
    // factory.AddVariable("tt_NumPassBTag_Light", 'F');
    // factory.AddVariable("tHq_DeltaR_BJetsHiggs", 'F');
    // factory.AddVariable("tt_DeltaR_Light", 'F');
    // factory.AddVariable("tt_SumCharge_Light", 'F');
    // factory.AddVariable("tt_Charge_BTopHad", 'F');
    
    
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
