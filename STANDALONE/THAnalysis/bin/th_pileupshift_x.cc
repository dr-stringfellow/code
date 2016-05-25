////////////////////////////////////////////////////////////////////////////////
//
// th_pileupshift_x
// --------------
// calculate up/down PU weights according to
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupSystematicErrors
//
// Further information can be found on
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJSONFileforData#Calculating_Your_Pileup_Distribu
//
//                              09/09/2013     Benedikt Maier <bmaier@cern.ch>
////////////////////////////////////////////////////////////////////////////////

#include "../interface/CommandLine.h"
#include "../interface/MCWeighters.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TLorentzVector.h>

#include "boost/shared_ptr.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cassert>

using namespace std;

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

  vector<string> inputs    = cl.getVector<string>("inputs");
  string         datapath  = cl.getValue<string> ("datapath",               ".");
  string         treename  = cl.getValue<string> ("treename",               "t");
  string         opath     = cl.getValue<string> ("opath",                  ".");
  string         output    = cl.getValue<string> ("output",                  "");
  int            systematic= cl.getValue<int>    ("systematic",               0);
  string         jesfile   = cl.getValue<string> ("jesfile",                 "");

  if (!cl.check()) return 0;
  cl.print();

  if      (-1==systematic) cout<<"Down type systematic scaling!"<<endl;
  else if (+1==systematic) cout<<"Up type systematic scaling!"<<endl;
  else               {cout<<"Error in variable systematic: "<<systematic<<endl;return 0;}

  

  //
  // loop over all input files
  //
  for (unsigned iinput=0;iinput<inputs.size();iinput++) {
  
    // open input file
    string input = inputs[iinput]; input = datapath + "/" + input;
    TFile* ifile = new TFile(input.c_str(),"READ");
    if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

    // create temporary output file
    if (output.empty()) output = input.substr(0,input.find(".root"));
    else output = output.substr(0,output.find(".root"));
    if (!opath.empty()){
      if (output.find("/")==string::npos)
	output = opath + "/" + output;
      else
	output = opath + output.substr(output.find_last_of("/"));
    }
    output = output + ".jes.root";
    TFile* ofile = new TFile(output.c_str(),"RECREATE");
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

      bool doHBB = itree->FindBranch("hbbm");

      // don't copy any jet or met branches if they exist
      
      // jt:
      if (0!=itree->FindBranch("weightPU")) itree->SetBranchStatus("weightPU",       0);
   

      // copy the tree
      odir->cd();
      cout<<"Cloning tree in "<<input<<" ==> "<<output<<" ... "<<flush;
      TTree* otree = itree->CloneTree();
      cout<<"done.\n";

      // set branch stati valid again...!
      
      // jt:
      if (0!=itree->FindBranch("weightPU"))     itree->SetBranchStatus("weightPU",       1);
     

 
      // create the branches in the output tree
      
     
      Float_t o_weightPU=1;              
      TBranch* b_weightPU = otree->Branch("weightPU",      &o_weightPU,             "weightPU/F");
   
     
      // setup input branches
      Int_t njt;
      //Int_t   njt;
      Float_t jtpt[100];     
      Float_t jteta[100];     
      Float_t jtphi[100];     
      Float_t jte[100];   
      Float_t jtjes[100];
   
      itree->SetBranchAddress("njt",   &njt);
      itree->SetBranchAddress("jtpt",   jtpt);  
      itree->SetBranchAddress("jteta",  jteta);  
      itree->SetBranchAddress("jtphi",  jtphi);  
      itree->SetBranchAddress("jte",    jte); 
      itree->SetBranchAddress("jtjes",  jtjes); 
      
      string syst_shift = "none";
      
      if(systematic==1)
	syst_shift = "up";
      if(systematic==-1)
	syst_shift = "down";
      string pathtocfg = "/portal/ekpcms5/home/sfink/analysis/STANDALONE/THAnalysis/sample-info.cfg";
      string datasetpath = "MC_*";
      SampleInfoWeighter* pile_up = new SampleInfoWeighter(pathtocfg,datasetpath, syst_shift);




      // loop over all events and recalculate jet pT and energy; no reordering!
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;

      for (unsigned ievt=0;ievt<nevt;ievt++) {
	
	itree->GetEntry(ievt);


	o_weightPU = pile_up->getWeights();


	o_weightPU = 4;






	// jt:
	b_weightPU ->Fill();


	
      } // for ievt<nevt


    } // while in keys

   cout<<"Save extended tree to "<<output.substr(0,output.find(".jer.root"))<<".root ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv " + output + " " 
      + output.substr(0,output.find(".jes.root")) + ".root";
    gSystem->Exec(cmd.c_str());
  } // for iinput

  cout<<"DONE!\n"<<endl;

  return 0;
}
