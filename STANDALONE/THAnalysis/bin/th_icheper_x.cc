////////////////////////////////////////////////////////////////////////////////
//
// th_icheper_x
// --------------
//
// This program creates a new tree with only those jets that fit some specific
// requirements on pt and eta.
//
//
// 19/05/2014             Benedikt Maier <benedikt.maier@cern.ch>        
//                        
////////////////////////////////////////////////////////////////////////////////



#include "../interface/CommandLine.h"
#include "../interface/THStatus.h"
#include "../interface/Math.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TRandom3.h>


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

struct GreaterByPt{
  float * pts;

  GreaterByPt(float * pts_): pts(pts_){}

  bool operator()(size_t i, size_t j) const{
    return pts[i] > pts[j];
  }
};

////////////////////////////////////////////////////////////////////////////////
// HELPER DEFINITIONS
////////////////////////////////////////////////////////////////////////////////
//bool largerthan (Float_t i,Float_t j) { return (i>j); }
//void fill_jtrank(const Int_t njt,Float_t* ojtpt,Int_t* ojtrank);


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
  string         opath     = cl.getValue<string> ("opath",                   "");
  string         output    = cl.getValue<string> ("output",                  "");
  bool           isData      = cl.getValue<bool> ("isData",                  false);

  if (!cl.check()) return 0;
  cl.print();


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
    output = output + ".ichep.root";
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

      // don't copy any jet or met branches if they exist

      if (0!=itree->FindBranch("jteta"))               itree->SetBranchStatus("jteta",       0);
      if (0!=itree->FindBranch("jtpt"))                itree->SetBranchStatus("jtpt",        0);
      if (0!=itree->FindBranch("jte"))                 itree->SetBranchStatus("jte",         0);
      if (0!=itree->FindBranch("jtjer"))               itree->SetBranchStatus("jtjer",       0);
      if (0!=itree->FindBranch("jtphi"))               itree->SetBranchStatus("jtphi",       0);
      if (0!=itree->FindBranch("jtrawpt"))             itree->SetBranchStatus("jtrawpt",     0);
      if (0!=itree->FindBranch("jtcsvt"))              itree->SetBranchStatus("jtcsvt",      0);
      if (0!=itree->FindBranch("jtntracks"))           itree->SetBranchStatus("jtntracks",   0);
      if (0!=itree->FindBranch("jtndaughters"))        itree->SetBranchStatus("jtndaughters",0);
      if (0!=itree->FindBranch("jtchmult"))            itree->SetBranchStatus("jtchmult",    0);
      if (0!=itree->FindBranch("jtarea"))              itree->SetBranchStatus("jtarea",      0);
      if (0!=itree->FindBranch("jtpull"))              itree->SetBranchStatus("jtpull",      0);
      if (0!=itree->FindBranch("jtcharge"))            itree->SetBranchStatus("jtcharge",    0);
      if (0!=itree->FindBranch("jtid"))                itree->SetBranchStatus("jtid",        0);
      if (0!=itree->FindBranch("jtchhadmult"))         itree->SetBranchStatus("jtchhadmult", 0);
      if (0!=itree->FindBranch("jtgenflv"))            itree->SetBranchStatus("jtgenflv",    0);
      if (0!=itree->FindBranch("jtgenpt"))             itree->SetBranchStatus("jtgenpt",     0);
      if (0!=itree->FindBranch("jtgeneta"))            itree->SetBranchStatus("jtgeneta",    0);
      if (0!=itree->FindBranch("jtgenphi"))            itree->SetBranchStatus("jtgenphi",    0);
      if (0!=itree->FindBranch("jtgene"))              itree->SetBranchStatus("jtgene",      0);
      if (0!=itree->FindBranch("jtpuidmva"))           itree->SetBranchStatus("jtpuidmva",   0);
      if (0!=itree->FindBranch("jtpumvaidl"))          itree->SetBranchStatus("jtpumvaidl",  0);
      if (0!=itree->FindBranch("jtpumvaidm"))          itree->SetBranchStatus("jtpumvaidm",  0);
      if (0!=itree->FindBranch("jtpumvaidt"))          itree->SetBranchStatus("jtpumvaidt",  0);
      if (0!=itree->FindBranch("jtpucutidl"))          itree->SetBranchStatus("jtpucutidl",  0);
      if (0!=itree->FindBranch("jtpucutidm"))          itree->SetBranchStatus("jtpucutidm",  0);
      if (0!=itree->FindBranch("jtpucutidt"))          itree->SetBranchStatus("jtpucutidt",  0);
      if (0!=itree->FindBranch("njt"))                 itree->SetBranchStatus("njt",         0);
      if (0!=itree->FindBranch("sumHt"))               itree->SetBranchStatus("sumHt",       0);
      

      

      // copy the tree
      odir->cd();
      cout<<"Cloning tree in "<<input<<" ==> "<<output<<" ... "<<flush;
      TTree* otree = itree->CloneTree();
      cout<<"done.\n";

      // set branch stati valid again...!
      
      if (0!=itree->FindBranch("jteta"))               itree->SetBranchStatus("jteta",       1);
      if (0!=itree->FindBranch("jtpt"))                itree->SetBranchStatus("jtpt",        1);
      if (0!=itree->FindBranch("jte"))                 itree->SetBranchStatus("jte",         1);
      if (0!=itree->FindBranch("jtjer"))               itree->SetBranchStatus("jtjer",       1);
      if (0!=itree->FindBranch("jtphi"))               itree->SetBranchStatus("jtphi",       1);
      if (0!=itree->FindBranch("jtrawpt"))             itree->SetBranchStatus("jtrawpt",     1);
      if (0!=itree->FindBranch("jtcsvt"))              itree->SetBranchStatus("jtcsvt",      1);
      if (0!=itree->FindBranch("jtntracks"))           itree->SetBranchStatus("jtntracks",   1);
      if (0!=itree->FindBranch("jtndaughters"))        itree->SetBranchStatus("jtndaughters",1);
      if (0!=itree->FindBranch("jtchmult"))            itree->SetBranchStatus("jtchmult",    1);
      if (0!=itree->FindBranch("jtarea"))              itree->SetBranchStatus("jtarea",      1);
      if (0!=itree->FindBranch("jtpull"))              itree->SetBranchStatus("jtpull",      1);
      if (0!=itree->FindBranch("jtcharge"))            itree->SetBranchStatus("jtcharge",    1);
      if (0!=itree->FindBranch("jtid"))                itree->SetBranchStatus("jtid",        1);
      if (0!=itree->FindBranch("jtchhadmult"))         itree->SetBranchStatus("jtchhadmult", 1);
      if (0!=itree->FindBranch("jtgenflv"))            itree->SetBranchStatus("jtgenflv",    1);
      if (0!=itree->FindBranch("jtgenpt"))             itree->SetBranchStatus("jtgenpt",     1);
      if (0!=itree->FindBranch("jtgeneta"))            itree->SetBranchStatus("jtgeneta",    1);
      if (0!=itree->FindBranch("jtgenphi"))            itree->SetBranchStatus("jtgenphi",    1);
      if (0!=itree->FindBranch("jtgene"))              itree->SetBranchStatus("jtgene",      1);
      if (0!=itree->FindBranch("jtpuidmva"))           itree->SetBranchStatus("jtpuidmva",   1);
      if (0!=itree->FindBranch("jtpumvaidl"))          itree->SetBranchStatus("jtpumvaidl",  1);
      if (0!=itree->FindBranch("jtpumvaidm"))          itree->SetBranchStatus("jtpumvaidm",  1);
      if (0!=itree->FindBranch("jtpumvaidt"))          itree->SetBranchStatus("jtpumvaidt",  1);
      if (0!=itree->FindBranch("jtpucutidl"))          itree->SetBranchStatus("jtpucutidl",  1);
      if (0!=itree->FindBranch("jtpucutidm"))          itree->SetBranchStatus("jtpucutidm",  1);
      if (0!=itree->FindBranch("jtpucutidt"))          itree->SetBranchStatus("jtpucutidt",  1);
      if (0!=itree->FindBranch("njt"))                 itree->SetBranchStatus("njt",         1);
      if (0!=itree->FindBranch("sumHt"))               itree->SetBranchStatus("sumHt",       1);      
      

      // create the branches in the output tree
            
      
      Float_t o_jtpt[100];
      Float_t o_jtphi[100];
      Float_t o_jte[100];
      Float_t o_jteta[100];
      Float_t o_jtjer[100];
      Float_t o_jtrawpt[100];
      Float_t o_jtcsvt[100];
      Float_t o_jtntracks[100];
      Int_t o_jtndaughters[100];
      Float_t o_jtarea[100];
      Float_t o_jtpull[100];
      Float_t o_jtcharge[100];
      Int_t   o_jtchmult[100];
      Bool_t  o_jtid[100];
      Float_t o_jtchhadmult[100];
      Short_t o_jtgenflv[100];
      Float_t o_jtgenpt[100];
      Float_t o_jtgeneta[100];
      Float_t o_jtgenphi[100];
      Float_t o_jtgene[100];
      Float_t  o_jtpuidmva[100];
      Bool_t  o_jtpumvaidl[100];
      Bool_t  o_jtpumvaidm[100];
      Bool_t  o_jtpumvaidt[100];
      Bool_t  o_jtpucutidl[100];
      Bool_t  o_jtpucutidm[100];
      Bool_t  o_jtpucutidt[100];
      Int_t o_njt  = 0;
      Float_t o_sumHt = 0;

      
      
      // setup input branches                 
      
      
      
      
      TBranch* b_njt   = otree->Branch("njt",       &o_njt,                 "njt/I");
      TBranch* b_jteta = otree->Branch("jteta",      o_jteta,      "jteta[njt]/F");
      TBranch* b_sumHt = otree->Branch("sumHt",     &o_sumHt,             "sumHt/F");
      TBranch* b_jtpt  = otree->Branch("jtpt",       o_jtpt,        "jtpt[njt]/F");
      TBranch* b_jte   = otree->Branch("jte",        o_jte,          "jte[njt]/F");
      TBranch* b_jtphi = otree->Branch("jtphi",      o_jtphi,      "jtphi[njt]/F");
      TBranch* b_jtjer = otree->Branch("jtjer",      o_jtjer,      "jtjer[njt]/F");
      TBranch* b_jtrawpt = otree->Branch("jtrawpt",         o_jtrawpt,           "jtrawpt[njt]/F");
      TBranch* b_jtcsvt = otree->Branch("jtcsvt",          o_jtcsvt,             "jtcsvt[njt]/F");
      TBranch* b_jtntracks = otree->Branch("jtntracks",       o_jtntracks,       "jtntracks[njt]/F");
      TBranch* b_jtndaughters = otree->Branch("jtndaughters",       o_jtndaughters,       "jtndaughters[njt]/I");
      TBranch* b_jtarea = otree->Branch("jtarea",          o_jtarea,             "jtarea[njt]/F");
      TBranch* b_jtpull = otree->Branch("jtpull",          o_jtpull,             "jtpull[njt]/F");
      TBranch* b_jtcharge = otree->Branch("jtcharge",        o_jtcharge,         "jtcharge[njt]/F");
      TBranch* b_jtchmult = otree->Branch("jtchmult",       o_jtchmult,       "jtchmult[njt]/I");
      TBranch* b_jtid = otree->Branch("jtid",            o_jtid,                 "jtid[njt]/O");
      TBranch* b_jtchhadmult = otree->Branch("jtchhadmult",     o_jtchhadmult,   "jtchhadmult[njt]/F");
      TBranch* b_jtgenflv = otree->Branch("jtgenflv",        o_jtgenflv,         "jtgenflv[njt]/S");
      TBranch* b_jtgenpt = otree->Branch("jtgenpt",         o_jtgenpt,           "jtgenpt[njt]/F");
      TBranch* b_jtgeneta = otree->Branch("jtgeneta",        o_jtgeneta,         "jtgeneta[njt]/F");
      TBranch* b_jtgenphi = otree->Branch("jtgenphi",        o_jtgenphi,         "jtgenphi[njt]/F");
      TBranch* b_jtgene = otree->Branch("jtgene",          o_jtgene,             "jtgene[njt]/F");
      TBranch* b_jtpuidmva = otree->Branch("jtpuidmva",            o_jtpuidmva,                 "jtpuidmva[njt]/O");
      TBranch* b_jtpumvaidl = otree->Branch("jtpumvaidl",            o_jtpumvaidl,                 "jtpumvaidl[njt]/O");
      TBranch* b_jtpumvaidm = otree->Branch("jtpumvaidm",            o_jtpumvaidm,                 "jtpumvaidm[njt]/O");
      TBranch* b_jtpumvaidt = otree->Branch("jtpumvaidt",            o_jtpumvaidt,                 "jtpumvaidt[njt]/O");
      TBranch* b_jtpucutidl = otree->Branch("jtpucutidl",            o_jtpucutidl,                 "jtpucutidl[njt]/O");
      TBranch* b_jtpucutidm = otree->Branch("jtpucutidm",            o_jtpucutidm,                 "jtpucutidm[njt]/O");
      TBranch* b_jtpucutidt = otree->Branch("jtpucutidt",            o_jtpucutidt,                 "jtpucutidt[njt]/O");
      
      // setup input branches                                                                                                         

      
      Float_t i_jteta[100];       itree->SetBranchAddress("jteta",  i_jteta);
      Float_t i_jtpt[100];       itree->SetBranchAddress("jtpt",  i_jtpt);
      Int_t  i_njt15;       itree->SetBranchAddress("njt15", &i_njt15);
      //Float_t i_sumHt;       itree->SetBranchAddress("sumHt", i_sumHt);
      Float_t i_jte[100];       itree->SetBranchAddress("jte",  i_jte);
      Float_t i_jtphi[100];       itree->SetBranchAddress("jtphi",  i_jtphi);
      Float_t i_jtjer[100];       itree->SetBranchAddress("jtjer",  i_jtjer);
      Float_t i_jtrawpt[100];       itree->SetBranchAddress("jtrawpt",  i_jtrawpt);
      Float_t i_jtcsvt[100];       itree->SetBranchAddress("jtcsvt",  i_jtcsvt);
      Float_t i_jtntracks[100];       itree->SetBranchAddress("jtntracks",  i_jtntracks);
      Int_t i_jtndaughters[100];       itree->SetBranchAddress("jtndaughters",  i_jtndaughters);
      Float_t i_jtarea[100];       itree->SetBranchAddress("jtarea",  i_jtarea);
      Float_t i_jtpull[100];       itree->SetBranchAddress("jtpull",  i_jtpull);
      Float_t i_jtcharge[100];       itree->SetBranchAddress("jtcharge",  i_jtcharge);
      Int_t i_jtchmult[100];       itree->SetBranchAddress("jtchmult",  i_jtchmult);
      Bool_t i_jtid[100];           itree->SetBranchAddress("jtid", i_jtid);
      Float_t i_jtchhadmult[100];    itree->SetBranchAddress("jtchhadmult", i_jtchhadmult);
      Short_t i_jtgenflv[100];    itree->SetBranchAddress("jtgenflv", i_jtgenflv);
      Float_t i_jtgenpt[100];    itree->SetBranchAddress("jtgenpt", i_jtgenpt);
      Float_t i_jtgeneta[100];    itree->SetBranchAddress("jtgeneta", i_jtgeneta);
      Float_t i_jtgenphi[100];    itree->SetBranchAddress("jtgenphi", i_jtgenphi);
      Float_t i_jtgene[100];    itree->SetBranchAddress("jtgene", i_jtgene);
      Bool_t i_jtpuidmva[100];    itree->SetBranchAddress("jtpuidmva", i_jtpuidmva);
      Bool_t i_jtpumvaidl[100];    itree->SetBranchAddress("jtpumvaidl", i_jtpumvaidl);
      Bool_t i_jtpumvaidm[100];    itree->SetBranchAddress("jtpumvaidm", i_jtpumvaidm);
      Bool_t i_jtpumvaidt[100];    itree->SetBranchAddress("jtpumvaidt", i_jtpumvaidt);
      Bool_t i_jtpucutidl[100];    itree->SetBranchAddress("jtpucutidl", i_jtpucutidl);
      Bool_t i_jtpucutidm[100];    itree->SetBranchAddress("jtpucutidm", i_jtpucutidm);
      Bool_t i_jtpucutidt[100];    itree->SetBranchAddress("jtpucutidt", i_jtpucutidt);






      // loop over all events and drop all jets that do not fit our requirements; no reordering!
      unsigned nevt= static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;

      THStatus* status = new THStatus(nevt);

      for (unsigned ievt=0;ievt<nevt;ievt++) {
	status->print_status(ievt);

	itree->GetEntry(ievt);
	
	o_sumHt=0;
	o_njt=0;

	//	cout << "1!!!" << endl;
		      
	for (int ijt=0;ijt < i_njt15;ijt++) {
	  if((i_jtpt[ijt]>20.&&abs(i_jteta[ijt])<2.4)||i_jtpt[ijt]>40) {
	    o_jtrawpt[o_njt]        =   i_jtrawpt[ijt];
	    o_jtpt[o_njt]           =   i_jtpt[ijt];
	    o_jteta[o_njt]          =   i_jteta[ijt];
	    o_jtphi[o_njt]          =   i_jtphi[ijt];
	    o_jte[o_njt]            =   i_jte[ijt];
	    o_jtjer[o_njt]          =   i_jtjer[ijt];
	    o_jtcsvt[o_njt]         =   i_jtcsvt[ijt];
	    o_jtntracks[o_njt]      =   i_jtntracks[ijt];
	    o_jtndaughters[o_njt]   =   i_jtndaughters[ijt];
	    o_jtarea[o_njt]         =   i_jtarea[ijt];
	    o_jtpull[o_njt]         =   i_jtpull[ijt];
	    o_jtcharge[o_njt]       =   i_jtcharge[ijt];
	    o_jtid[o_njt]           =   i_jtid[ijt];
	    o_jtchmult[o_njt]       =   i_jtchmult[ijt];
	    o_jtchhadmult[o_njt]    =   i_jtchhadmult[ijt];
	    o_jtgenflv[o_njt]       =   i_jtgenflv[ijt];
	    o_jtgenpt[o_njt]        =   i_jtgenpt[ijt];
	    o_jtgeneta[o_njt]       =   i_jtgeneta[ijt];
	    o_jtgenphi[o_njt]       =   i_jtgenphi[ijt];
	    o_jtgene[o_njt]         =   i_jtgene[ijt];
	    o_jtjer[o_njt]          =   i_jtjer[ijt];
	    o_jtpuidmva[o_njt]      =   i_jtpuidmva[ijt];
	    o_jtpumvaidl[o_njt]     =   i_jtpumvaidl[ijt];
	    o_jtpumvaidm[o_njt]     =   i_jtpumvaidm[ijt];
	    o_jtpumvaidt[o_njt]     =   i_jtpumvaidt[ijt];
	    o_jtpucutidl[o_njt]     =   i_jtpucutidl[ijt];
	    o_jtpucutidm[o_njt]     =   i_jtpucutidm[ijt];
	    o_jtpucutidt[o_njt]     =   i_jtpucutidt[ijt];
	  
	    
	    o_sumHt                +=   o_jtpt[o_njt];

	    //cout << "2!!!" << endl;

	    o_njt++;
	 	    
	  }
	} // jet loop

	b_njt->Fill();
	b_jtrawpt -> Fill();
	b_jtpt -> Fill();
	b_jteta -> Fill();
	b_jtphi -> Fill();
	b_jte -> Fill();
	b_jtjer -> Fill();
	b_jtcsvt -> Fill();
	b_jtntracks -> Fill();
	b_jtndaughters ->Fill();
	b_jtarea -> Fill();
	b_jtpull -> Fill();
	b_jtcharge -> Fill();
	b_jtid -> Fill();
	b_jtchmult -> Fill();
	b_jtchhadmult -> Fill();
	b_jtgenflv -> Fill();
	b_jtgenpt -> Fill();
	b_jtgeneta -> Fill();
	b_jtgenphi -> Fill();
	b_jtgene -> Fill();
	b_jtpuidmva -> Fill();
	b_jtpumvaidl -> Fill();
	b_jtpumvaidm -> Fill();
	b_jtpumvaidt -> Fill();
	b_jtpucutidl -> Fill();
	b_jtpucutidm -> Fill();
	b_jtpucutidt -> Fill();
	b_sumHt -> Fill();
	

      }

    } // while in keys



    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv -f " + output + " " 
      + output.substr(0,output.find(".ichep.root")) + ".root";
    //    string cmd2 = "cp " + output.substr(0,output.find(".pur.root")) + ".root " + "BACKUP_" + output.substr(0,output.find(".pur.root")) + ".root"; 
    //cout << endl <<  "Backupping old file to BACKUP_" << output.substr(0,output.find(".pur.root")) << ".root ..." << endl;
    //gSystem->Exec(cmd2.c_str());
     
    cout<<"Save extended tree to "<<output.substr(0,output.find(".pur.root"))<<".root ... "<<flush;
    gSystem->Exec(cmd.c_str());
  } // for iinput

  cout<<"DONE!\n"<<endl;

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// HELPER IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////




//______________________________________________________________________________

/*
void fill_jtrank(const Int_t njt,Float_t* ojtpt,Int_t* ojtrank)
{
  std::vector<Float_t> vv_pts;
  std::multimap<Float_t,Int_t> mm_pts;

  for (Int_t i=0;i<njt;++i) {
    vv_pts.push_back(ojtpt[i]);
    mm_pts.insert(pair<Float_t,Int_t>(ojtpt[i],i));
  }

  std::sort (vv_pts.begin(),vv_pts.end(),largerthan);

  Int_t ijt(0);
  std::vector<Float_t>::const_iterator itvv = vv_pts.begin();
  for (;itvv!=vv_pts.end();++itvv) {
    std::multimap<Float_t,Int_t>::iterator itmm = mm_pts.find(*itvv);
    assert (itmm!=mm_pts.end());
    ojtrank[ itmm->second ] = (ijt);
    mm_pts.erase(itmm);
    ++ijt;
  }
}
*/
