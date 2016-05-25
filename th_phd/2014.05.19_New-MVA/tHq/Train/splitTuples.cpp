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


int main()
{
    // Define the source files
    string const srcDirectory("../InputVars/tuples/");
    list<string> srcFileNames{"mu/thq-nc-mg.root", "e/thq-nc-mg.root"};
    string const treeName("Vars");
    
    
    // Define how the samples should be split
    unsigned const maxReminder = 5;
    unsigned const denominator = 25;
    
    
    // Names of output files and directory
    string const outDirectory("tuples/");
    string const outFileBaseName("thq-nc-mg");
    
    
    // Create the output files
    boost::filesystem::create_directory(outDirectory);
    
    TFile trainFile((outDirectory + outFileBaseName + "_train.root").c_str(), "recreate");
    TFile examFile((outDirectory + outFileBaseName + "_exam.root").c_str(), "recreate");
    
    unique_ptr<TTree> sgnTrainTree, bkgTrainTree, sgnExamTree, bkgExamTree;
    
    
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
        
        srcTree->SetBranchAddress("IsSignal", &isSignal);
        //srcTree->SetBranchAddress("RunNumber", &run);
        //srcTree->SetBranchAddress("LumiSection", &lumi);
        srcTree->SetBranchAddress("EventNumber", &event);
        
        
        // Create output trees or reset their buffer addresses
        if (not sgnTrainTree)  // this is the first source file
        {
            sgnTrainTree.reset(srcTree->CloneTree(0));
            sgnTrainTree->SetDirectory(&trainFile);
            
            bkgTrainTree.reset(srcTree->CloneTree(0));
            bkgTrainTree->SetDirectory(&trainFile);
            
            sgnExamTree.reset(srcTree->CloneTree(0));
            sgnExamTree->SetDirectory(&examFile);
            
            bkgExamTree.reset(srcTree->CloneTree(0));
            bkgExamTree->SetDirectory(&examFile);
        }
        else
        {
            srcTree->CopyAddresses(sgnTrainTree.get());
            srcTree->CopyAddresses(bkgTrainTree.get());
            srcTree->CopyAddresses(sgnExamTree.get());
            srcTree->CopyAddresses(bkgExamTree.get());
        }
        
        
        // Loop over events in the source tree and fill the output trees
        for (unsigned long ev = 0; ev < nEntries; ++ev)
        {
            srcTree->GetEntry(ev);
            
            if ((event % denominator) <= maxReminder)
                (isSignal) ? sgnTrainTree->Fill() : bkgTrainTree->Fill();
            else
                (isSignal) ? sgnExamTree->Fill() : bkgExamTree->Fill();
        }
    }
    
    
    // The output trees has been filled. Write them
    trainFile.cd();
    sgnTrainTree->Write("VarsSgn", TObject::kOverwrite);
    bkgTrainTree->Write("VarsBkg", TObject::kOverwrite);
    
    examFile.cd();
    sgnExamTree->Write("VarsSgn", TObject::kOverwrite);
    bkgExamTree->Write("VarsBkg", TObject::kOverwrite);
    
    
    return 0;
}