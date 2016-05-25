////////////////////////////////////////////////////////////////////////////////
//
// th_addvariables_x
// -----------------
//
//            01/10/2013 Benedikt Maier <bmaier@cern.ch>
//                                                    
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"
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

double deltaPhi(double phi1, double phi2);



//Sorting
struct GreaterByCSV{
    float * csv;
    
    GreaterByCSV(float * csv_): csv(csv_){}
    
    bool operator()(size_t i, size_t j) const{
        return csv[i] > csv[j];
       }
   };

struct GreaterByEta{
    float * eta;
    
    GreaterByEta(float * eta_): eta(eta_){}
    
    bool operator()(size_t i, size_t j) const{
        return eta[i] > eta[j];
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
  string         mode_as_str = cl.getValue<string> ("mode","dijetpt");
  bool           clean       = cl.getValue<bool>   ("clean",   false);
  Int_t          isMu        = cl.getValue<int>    ("isMu",    -1);
  //bool           isSemi      = cl.getValue<bool>   ("isSemi",  false);

  if (!cl.check()) return 0;
  cl.print();

  //
  // decode mode
  //
  
  int mode(0);
  if (mode_as_str=="sumbtag") {
    cout<<"Mode: Highest btag sum"<<endl;
    mode=1;
  }
  else if (mode_as_str=="dijetpt") {
    cout<<"Mode: Highest dijet pT"<<endl;
    mode=2;
  }
  else {
    cout<<"ERROR: invalid mode '"<<mode_as_str<<"', pick dijetpt|sumbtag!"<<endl;
    return 0;
  }
  
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
    output = output + ".var.root";
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
      if (0!=itree->FindBranch("lepeta"))      itree->SetBranchStatus("lepeta",     0);
      if (0!=itree->FindBranch("nlep"))      itree->SetBranchStatus("nlep",     0);

 
      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;



      // if cleaning is requested, nothing further!
      if (clean) continue;
      
      // create H->bb branches in output tree

      //Int_t nlep;
      Float_t lepeta[140];

      //TBranch* o_nlep                 =otree->Branch("nlep",       &nlep,    "nlep/I");
      TBranch* o_lepeta                 =otree->Branch("lepeta",       lepeta,    "lepeta[140]/F");

      // setup input branches needed to fill H->bb branches
      
      
      Int_t i_nel={-99};               itree->SetBranchAddress("nel",            &i_nel);
      Float_t i_eleta[10]={-99};       itree->SetBranchAddress("eleta",         i_eleta);
      
      Int_t i_nmu={-99};               itree->SetBranchAddress("nmu",            &i_nmu);
      Float_t i_mueta[10]={-99};       itree->SetBranchAddress("mueta",         i_mueta);
      
       
      // loop over all events and fill H->bb branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {

	itree->GetEntry(ievt);
	
	if (isMu){
	  lepeta[0]=i_mueta[0];
	}
	else
	  lepeta[0]=i_eleta[0];
	  	
	o_lepeta                ->Fill();

      } // ievt

    } // idir
    
    cout<<"Save extended tree to "<<input<<" ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv -f " + output + " " + input;
    gSystem->Exec(cmd.c_str());
    cout<<"DONE!\n"<<endl;
    
  } // ifile
  

  return 0;
}



//______________________________________________________________________________
double deltaPhi(double phi1, double phi2)
{
  double result = phi1 - phi2;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}
