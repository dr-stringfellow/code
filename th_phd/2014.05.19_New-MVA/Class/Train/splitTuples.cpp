/**
 * The programs splits tuples with input variables into training and testing/examination set.
 */

#include <TFile.h>
#include <TTree.h>

#include <boost/filesystem.hpp>

#include <string>
#include <list>
#include <memory>


using namespace std;


/**
 * Splits given ROOT tuples into training and examining sets. An event gets into the training set if
 * (eventID.eventNumber % denominator) <= maxReminder. Event weights are scaled accordingly. The
 * last parameter must be +1 for signal file and -1 for background one. It might also be 0. In the
 * latter case the same file is supposed to contain both signal and background events, and they
 * should be distinguished by IsSignal branch.
 */
void SplitTuples(string const &srcDirectory, list<string> const &srcFileNames,
 string const &outDirectory, string const &outFileBaseName,
 unsigned maxReminder, unsigned denominator, int sgnFlag)
{
    // Factor to correct event weights
    double const trainScaleFactor = double(maxReminder + 1) / denominator;
    
    // Create output files
    TFile trainFile((outDirectory + outFileBaseName + "_train.root").c_str(), "recreate");
    TFile examFile((outDirectory + outFileBaseName + "_exam.root").c_str(), "recreate");
    
    string const treeName("Vars");
    unique_ptr<TTree> sgnTrainTree, bkgTrainTree, sgnExamTree, bkgExamTree;
    //^ When sgnFlag != 0, only "signal" trees are used, no matter if it is signal or background
    
    
    // Loop over the source files
    for (string const &srcFileName: srcFileNames)
    {
        // Open the source file and get the tree
        TFile srcFile((srcDirectory + srcFileName).c_str());
        unique_ptr<TTree> srcTree(dynamic_cast<TTree *>(srcFile.Get(treeName.c_str())));
        unsigned long const nEntries = srcTree->GetEntries();
        
        
        // Set buffers to access event ID and signal flag
        Bool_t isSignal;
        ULong64_t /*run, lumi,*/ event;
        Float_t weight;
        
        if (sgnFlag == 0)
            srcTree->SetBranchAddress("IsSignal", &isSignal);
        else
            isSignal = true;  // a fake value just to pick a specific tree
        
        //srcTree->SetBranchAddress("RunNumber", &run);
        //srcTree->SetBranchAddress("LumiSection", &lumi);
        srcTree->SetBranchAddress("EventNumber", &event);
        srcTree->SetBranchAddress("Weight", &weight);
        
        
        // Create output trees or reset their buffer addresses
        if (not sgnTrainTree)  // this is the first source file
        {
            sgnTrainTree.reset(srcTree->CloneTree(0));
            sgnTrainTree->SetDirectory(&trainFile);
            
            sgnExamTree.reset(srcTree->CloneTree(0));
            sgnExamTree->SetDirectory(&examFile);
            
            if (sgnFlag == 0)
            {
                bkgTrainTree.reset(srcTree->CloneTree(0));
                bkgTrainTree->SetDirectory(&trainFile);
                
                bkgExamTree.reset(srcTree->CloneTree(0));
                bkgExamTree->SetDirectory(&examFile);
            }
        }
        else
        {
            srcTree->CopyAddresses(sgnTrainTree.get());
            srcTree->CopyAddresses(sgnExamTree.get());
            
            if (sgnFlag == 0)
            {
                srcTree->CopyAddresses(bkgTrainTree.get());
                srcTree->CopyAddresses(bkgExamTree.get());
            }
        }
        
        
        // Loop over events in the source tree and fill the output trees
        for (unsigned long ev = 0; ev < nEntries; ++ev)
        {
            srcTree->GetEntry(ev);
            
            if ((event % denominator) <= maxReminder)  // training set
            {
                weight /= trainScaleFactor;
                (isSignal) ? sgnTrainTree->Fill() : bkgTrainTree->Fill();
            }
            else  // examining set
            {
                weight /= (1. - trainScaleFactor);
                (isSignal) ? sgnExamTree->Fill() : bkgExamTree->Fill();
            }
        }
    }
    
    
    // The output trees has been filled. Write them
    trainFile.cd();
    
    if (sgnFlag == 0)
    {
        sgnTrainTree->Write("VarsSgn", TObject::kOverwrite);
        bkgTrainTree->Write("VarsBkg", TObject::kOverwrite);
    }
    else
        sgnTrainTree->Write("Vars", TObject::kOverwrite);
    
    examFile.cd();
    
    if (sgnFlag == 0)
    {
        sgnExamTree->Write("VarsSgn", TObject::kOverwrite);
        bkgExamTree->Write("VarsBkg", TObject::kOverwrite);
    }
    else
        sgnExamTree->Write("Vars", TObject::kOverwrite);
}


int main()
{
    // Define the source files
    string const srcDirectory("../InputVars/tuples/");
    list<string> tHqFileNames{"mu/thq-nc-mg_53X.02.01_Wyg.root", "e/thq-nc-mg_53X.02.01_Wyg.root"};
    list<string> ttbarSemilepFileNames{"mu/ttbar-semilep-mg-p1_53X.02.01_VNz_p1.root",
     "mu/ttbar-semilep-mg-p1_53X.02.01_VNz_p2.root", "mu/ttbar-semilep-mg-p1_53X.02.01_VNz_p3.root",
     "mu/ttbar-semilep-mg-p1_53X.02.01_VNz_p4.root", "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p1.root",
     "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p2.root", "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p3.root",
     "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p4.root", "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p5.root",
     "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p1.root", "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p2.root",
     "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p3.root", "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p4.root",
     "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p5.root",
     "e/ttbar-semilep-mg-p1_53X.02.01_VNz_p1.root",
     "e/ttbar-semilep-mg-p1_53X.02.01_VNz_p2.root", "e/ttbar-semilep-mg-p1_53X.02.01_VNz_p3.root",
     "e/ttbar-semilep-mg-p1_53X.02.01_VNz_p4.root", "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p1.root",
     "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p2.root", "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p3.root",
     "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p4.root", "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p5.root",
     "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p1.root", "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p2.root",
     "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p3.root", "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p4.root",
     "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p5.root"};
    list<string> ttbarDilepFileNames{"mu/ttbar-dilep-mg_53X.02.01_FFe_p1.root",
     "mu/ttbar-dilep-mg_53X.02.01_FFe_p2.root", "e/ttbar-dilep-mg_53X.02.01_FFe_p1.root",
     "e/ttbar-dilep-mg_53X.02.01_FFe_p2.root"};
    list<string> ttHFileNames{"mu/tth_53X.02.01_bVJ.root", "e/tth_53X.02.01_bVJ.root"};
    
    
    // Output directory
    string const outDirectory("tuples/");
    boost::filesystem::create_directory(outDirectory);
    
    
    // Process the samples
    SplitTuples(srcDirectory, tHqFileNames, outDirectory, "tHq-nc-mg", 7, 25, +1);
    SplitTuples(srcDirectory, ttbarSemilepFileNames, outDirectory, "ttbar-semilep-mg", 7, 25, -1);
    SplitTuples(srcDirectory, ttbarDilepFileNames, outDirectory, "ttbar-dilep-mg", 7, 25, -1);
    SplitTuples(srcDirectory, ttHFileNames, outDirectory, "ttH", 7, 25, -1);
    
    
    return 0;
}
