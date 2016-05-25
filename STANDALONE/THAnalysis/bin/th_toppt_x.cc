////////////////////////////////////////////////////////////////////////////////
//
// th_toppt_x
// -----------------
//
// Reweights ttbar MC as a function of the top quark pt,
//  c.f. https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopPtReweighting
//
//            01/10/2013 Benedikt Maier <bmaier@cern.ch>
//                                                    
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"
#include "../interface/Math.h"
//#include "SchieferD/HiggsAnalysis/interface/VHBBMultiThresholdEfficiency.h"
//#include "../src/Weights.cc"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TEventList.h>
#include <TLorentzVector.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
////////////////////////////////////////////////////////////////////////////////
//-------


// For sorting by b-tagger value
struct GreaterByCSV{
    float * csv;
    
    GreaterByCSV(float * csv_): csv(csv_){}
    
    bool operator()(size_t i, size_t j) const{
        return csv[i] > csv[j];
       }
   };


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  //
  // evaluate command line options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs      = cl.getVector<string>("inputs");
  string         treename    = cl.getValue<string> ("treename",  "t");
  string         datapath    = cl.getValue<string> ("datapath",  "/");
  bool           clean       = cl.getValue<bool>   ("clean",   false);
  bool           isFull      = cl.getValue<bool>   ("isFull",  false);
  bool           isSemi      = cl.getValue<bool>   ("isSemi",  false);
  bool           isIncl      = cl.getValue<bool>   ("isIncl",  false);

  if (!cl.check()) return 0;
  cl.print();

  
  //
  // loop over all input files
  //
  for (unsigned iinput=0;iinput<inputs.size();iinput++) {
  
    // open input file
    string input = inputs[iinput]; //input = datapath + "/" + input;
    TFile* ifile = new TFile(input.c_str(),"READ");
    if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

    // create temporary output file
    size_t pos = input.find(".root");
    string output = input.substr(0,pos);
    output = output + ".toppt.root";
    TFile* ofile = new TFile(output.c_str(),"CREATE");
    if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
    
    // loop over input directories
    TIter next(ifile->GetListOfKeys());
    TKey* key(0);
    while ((key=(TKey*)next())) {

      if (strcmp(key->GetClassName(),"TDirectoryFile")!=0) continue;
      
      // read input directory
      TDirectory* idir    = (TDirectory*)key->ReadObj();
      string      dirname = idir->GetName();
      cout<<input<<"/"<<dirname<<":"<<endl;
      
      // create output directory
      TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
      if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}
      
      // open input tree
      TTree* itree = (TTree*)idir->Get(treename.c_str());
      if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<dirname<<endl;return 0;}
      
      // check wether relevant branches are present
      if (0==itree->FindBranch("nlepw")){ cout<<"no W->lnu branches stored!"<<endl; return 0; }
      //if (0==itree->FindBranch("nlepz")){ cout<<"no Z->ll branches stored!"<<endl; return 0; }

      // check wether sample is considered MC (weights to be stored)
      bool isMC = itree->FindBranch("npv");
      if (isMC) cout<<"Treating as MC"<<endl; else cout<<"Treating as DATA"<<endl;


      // don't copy H->bb branches if they already exist
      if (0!=itree->FindBranch("weight_toppt"))      itree->SetBranchStatus("weight_toppt",     0);
      
 
      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;


      // if cleaning is requested, nothing further!
      if (clean) continue;
      
      // create H->bb branches in output tree
       
      Float_t weight_toppt;
      

      TBranch* b_weight_toppt        =otree->Branch("weight_toppt",      &weight_toppt,           "weight_toppt/F");
      

      // setup input branches needed to fill H->bb branches
      


      

      Float_t top_tpt;       itree->SetBranchAddress("top_tpt",       &top_tpt);
      Float_t top_tbarpt;    itree->SetBranchAddress("top_tbarpt", &top_tbarpt);
      
     
    
      // loop over all events and fill H->bb branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {

	itree->GetEntry(ievt);
	
	weight_toppt = 1;
	if(isFull)
	  {
	    Float_t f_firsttop = TMath::Exp(0.148 - 0.00129 * top_tpt);
	    Float_t f_secondtop = TMath::Exp(0.148 - 0.00129 * top_tbarpt);
	    weight_toppt = TMath::Sqrt(f_firsttop * f_secondtop);
	  }
	else if(isSemi)
	  {
	    Float_t f_firsttop = TMath::Exp(0.159 - 0.00141 * top_tpt);
	    Float_t f_secondtop = TMath::Exp(0.159 - 0.00141 * top_tbarpt);
	    weight_toppt = TMath::Sqrt(f_firsttop * f_secondtop);
	  }
	else if(isIncl)
	  {
	    Float_t f_firsttop = TMath::Exp(0.156 - 0.00137 * top_tpt);
	    Float_t f_secondtop = TMath::Exp(0.156 - 0.00137 * top_tbarpt);
	    weight_toppt = TMath::Sqrt(f_firsttop * f_secondtop);
	  }
	else
	  weight_toppt = 1;

	b_weight_toppt           ->Fill();

      } // ievt

    } // idir
    
    cout<<"Save extended tree to "<<input<<" ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv " + output + " " + input;
    gSystem->Exec(cmd.c_str());
    cout<<"DONE!\n"<<endl;
    
  } // ifile
  

  return 0;
}


