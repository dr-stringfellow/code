////////////////////////////////////////////////////////////////////////////////
//
// bdt_regprepare_x
// -----------------
//
//            20/08/2012 Christian Boeser 
//
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TEventList.h>
#include <TLorentzVector.h>
#include <TBranch.h>

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
  string         dirname    =  cl.getValue<string> ("dirname", "utm");

  if (!cl.check()) return 0;
  cl.print();

  //
  // loop over all input files
  //

  for (unsigned iinput=0;iinput<inputs.size();iinput++) {
  
    // open input file
    string input = inputs[iinput]; 
    TFile* ifile = new TFile(input.c_str(),"READ");
    if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

    // create temporary output file
    size_t pos = input.find(".root");
    string output = input.substr(0,pos);
    output = output + "_reg.root";
    TFile* ofile = new TFile(output.c_str(),"CREATE");
    if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
    
    // read input directory                                                                                                                                                                   
    TDirectory* idir    = (TDirectory*)ifile->Get(dirname.c_str());
    cout<<input<<"/"<<dirname<<":"<<endl;
    
    // create output directory signal                                                                                                                                                     
    TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
    if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}



    // open input tree
    TTree* itree = (TTree*)idir->Get(treename.c_str());
    if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<endl;return 0;}
    
 

    // create non-arraw branches in output tree
    
    Int_t i_hbbjtidx[3]; itree->SetBranchAddress("hbbjtidx", i_hbbjtidx);
    Int_t i_topjtidx; itree->SetBranchAddress("topjtidx", &i_topjtidx);
    Float_t i_jtpt[140]; itree->SetBranchAddress("jtpt", i_jtpt);
    Float_t i_jtgenpt[140]; itree->SetBranchAddress("jtgenpt", i_jtgenpt);
    Float_t i_jtrawpt[140]; itree->SetBranchAddress("jtrawpt", i_jtrawpt);
    Float_t i_jteta[140]; itree->SetBranchAddress("jteta", i_jteta);
    Float_t i_jte[140]; itree->SetBranchAddress("jte", i_jte);
    Float_t i_jtntracks[140]; itree->SetBranchAddress("jtntracks", i_jtntracks); 
    Float_t i_jtarea[140]; itree->SetBranchAddress("jtarea", i_jtarea);
    Float_t i_jtcharge[140]; itree->SetBranchAddress("jtcharge", i_jtcharge);
    Float_t i_jtchhadmult[140]; itree->SetBranchAddress("jtchhadmult", i_jtchhadmult);
    Float_t i_jtcsvt[140]; itree->SetBranchAddress("jtcsvt",i_jtcsvt);

   
    odir->cd();
    // create output tree
    TTree* otree = new TTree(itree->GetName(),itree->GetTitle());
    cout<<output<<"/"<<treename<<" created (temporary!)"<<endl;
    
    

    // Create branches in the output tree
    
    Float_t o_jtpt;            TBranch* b_jtpt =         otree->Branch("jtpt", &o_jtpt, "jtpt/F");
    Float_t o_jtgenpt;         TBranch* b_jtgenpt =      otree->Branch("jtgenpt", &o_jtgenpt, "jtgenpt/F");
    Float_t o_jtrawpt;         TBranch* b_jtrawpt =      otree->Branch("jtrawpt", &o_jtrawpt, "jtrawpt/F");
    Float_t o_jteta;           TBranch* b_jteta =        otree->Branch("jteta", &o_jteta, "jteta/F");
    Float_t o_jte;             TBranch* b_jte =          otree->Branch("jte", &o_jte, "jte/F");
    Float_t o_jtntracks;       TBranch* b_jtntracks =    otree->Branch("jtntracks", &o_jtntracks, "jtntracks/F");
    Float_t o_jtarea;          TBranch* b_jtarea =       otree->Branch("jtarea", &o_jtarea, "jtarea/F");
    Float_t o_jtcharge;        TBranch* b_jtcharge =     otree->Branch("jtcharge", &o_jtcharge, "jtcharge/F");
    Float_t o_jtchhadmult;     TBranch* b_jtchhadmult =  otree->Branch("jtchhadmult", &o_jtchhadmult,"jtchhadmult/F");
    Float_t o_jtcsvt;          TBranch* b_jtcsvt =       otree->Branch("jtcsvt", &o_jtcsvt, "jtcsvt/F");
    
    
    Int_t evt=0;               TBranch* b_evt =              otree->Branch("evt", &evt,  "evt/I");
    
    // loop over all events and over the 2 jets to fill non-array branches
    
    unsigned nevt = static_cast<unsigned>(itree->GetEntries());
    cout<<nevt<<" events will be processed"<<endl;
    
    


    for (unsigned jetnumber=0; jetnumber <=2; jetnumber++){
      
      if(jetnumber!=2)
	{
	  for (unsigned ievt=0;ievt<nevt;ievt++) 
	    {
	    
	      if(ievt%1000==0)cout << "Event " << ievt << endl;
	      
	      itree->GetEntry(ievt);
	      evt=ievt;
	      
	      o_jtpt = i_jtpt[i_hbbjtidx[jetnumber]];
	      o_jtgenpt = i_jtgenpt[i_hbbjtidx[jetnumber]];
	      o_jtrawpt = i_jtrawpt[i_hbbjtidx[jetnumber]];
	      o_jteta = i_jteta[i_hbbjtidx[jetnumber]];
	      o_jte = i_jte[i_hbbjtidx[jetnumber]];
	      o_jtntracks = i_jtntracks[i_hbbjtidx[jetnumber]];
	      o_jtcharge = i_jtcharge[i_hbbjtidx[jetnumber]];
	      o_jtchhadmult = i_jtchhadmult[i_hbbjtidx[jetnumber]];
	      o_jtarea = i_jtarea[i_hbbjtidx[jetnumber]];
	      o_jtcsvt = i_jtcsvt[i_hbbjtidx[jetnumber]];
	      
	      otree->Fill();
	      
	    } // ievt
	} // Higgs jets
      if(jetnumber==2)
	{
	  for (unsigned ievt=0;ievt<nevt;ievt++) 
	    {
	      
	      if(ievt%1000==0)cout << "Event " << ievt << endl;
	      
	      itree->GetEntry(ievt);
	      evt=ievt;
	      
	      o_jtpt = i_jtpt[i_topjtidx];
	      o_jtgenpt = i_jtgenpt[i_topjtidx];
	      o_jtrawpt = i_jtrawpt[i_topjtidx];
	      o_jteta = i_jteta[i_topjtidx];
	      o_jte = i_jte[i_topjtidx];
	      o_jtntracks = i_jtntracks[i_topjtidx];
	      o_jtcharge = i_jtcharge[i_topjtidx];
	      o_jtchhadmult = i_jtchhadmult[i_topjtidx];
	      o_jtarea = i_jtarea[i_topjtidx];
	      o_jtcsvt = i_jtcsvt[i_topjtidx];

	      otree->Fill();

	    } //ievt
	  
	} // top jet
      
    } // jetnumber
    
    cout<<"Save extended tree to "<<output<<" ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;

    cout<<"DONE!\n"<<endl;
    
  } // ifile
  

  return 0;
}



////////////////////////////////////////////////////////////////////////////////
// helpers
////////////////////////////////////////////////////////////////////////////////
