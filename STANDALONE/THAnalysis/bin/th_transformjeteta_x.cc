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
    output = output + ".jeteta.root";
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
      if (0!=itree->FindBranch("njt"))                 itree->SetBranchStatus("njt",         0);
      

      // copy the tree
      odir->cd();
      cout<<"Cloning tree in "<<input<<" ==> "<<output<<" ... "<<flush;
      TTree* otree = itree->CloneTree();
      cout<<"done.\n";

      // set branch stati valid again...!
      
      if (0!=itree->FindBranch("jteta"))               itree->SetBranchStatus("jteta",       1);
      if (0!=itree->FindBranch("njt"))                 itree->SetBranchStatus("njt",         1);

      // create the branches in the output tree
            
      
      Float_t o_jteta[100];
      Float_t o_jteta_old[100];
      
      // setup input branches                 
      

      TBranch* b_jteta = otree->Branch("jteta",      o_jteta,      "jteta[njt]/F");
      TBranch* b_jteta_old = otree->Branch("jteta_old",      o_jteta_old,      "jteta_old[njt]/F");
      
      // setup input branches                                                                                                         

      
      Float_t i_jteta[100];       itree->SetBranchAddress("jteta",  i_jteta);
      Int_t   i_njt;              itree->SetBranchAddress("njt", &i_njt);

      unsigned nevt= static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;

      THStatus* status = new THStatus(nevt);

      for (unsigned ievt=0;ievt<nevt;ievt++) {
	status->print_status(ievt);

	itree->GetEntry(ievt);
	
	for (int ijt=0;ijt < i_njt;ijt++) {
	  o_jteta_old[ijt] = i_jteta[ijt];
	  if(fabs(i_jteta[ijt])< 2.4 ) {
	    o_jteta[ijt] = i_jteta[ijt];
	  }
	  else if (fabs(i_jteta[ijt]) > 3.2){
	    o_jteta[ijt] = (i_jteta[ijt] >= 0) ? 3.5 : -3.5;
	  }
	  else {
	    o_jteta[ijt] = (i_jteta[ijt] >= 0) ? 2.8 : -2.8; 
	  }
	} // jet loop

	
	b_jteta -> Fill();
	b_jteta_old ->Fill();

      }

    } // while in keys



    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv -f " + output + " " 
      + output.substr(0,output.find(".jeteta.root")) + ".root";
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
