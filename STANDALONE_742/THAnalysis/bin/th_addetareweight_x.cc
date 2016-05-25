////////////////////////////////////////////////////////////////////////////////////
//
// th_etareweight_x
// --------------
//
//            05/03/2014 Simon Fink <simon.fink@cern.ch>
// 
////////////////////////////////////////////////////////////////////////////////////


#include "../interface/Histogram.h"
#include "../interface/DataSample.h"
#include "../interface/MCSample.h"
#include "../interface/CommandLine.h"
#include "../interface/THStatus.h"

#include <TMath.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TKey.h>
#include <TH1F.h>
#include "TPluginManager.h"


#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <stdexcept>

using namespace std;

//////////////////////////////////////////////////////////////////////////////// 
// helper function   
//////////////////////////////////////////////////////////////////////////////// 

string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression");


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo","*", "TStreamerInfo","RIO", "TStreamerInfo()");
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  vector<string> data      = cl.getVector<string>  ("data",                "");
  vector<string> mcs       = cl.getVector<string>  ("mcs",                 "");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
  vector<string> datacuts  = cl.getVector<string>  ("datacuts",            "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");
  string         datalabel = cl.getValue <string>  ("datalabel",       "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");
  string         opath     = cl.getValue <string>  ("opath",               "");
  string         output    = cl.getValue <string>  ("output",              "");
  string         filename   = cl.getValue <string>  ("filename",        "");
  
  if(!cl.check()) return 0;
  cl.print();
  
  if (mcs.empty()){cout<<"provide -data and/or -mcs!"<<endl;return 0;}

    
  //
  // OPEN DATA FILE / RETRIEVE TREE
  //
  string filesuffix = "";
  
  
  //
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples;
  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0) MCSample::printSampleInfos(mcsamples);
    
  
  //
  // INITITALIZE HISTOGRAM DATA
  //

  Float_t corrfac[5][51];
  Float_t test;
  Int_t a=0;
  Int_t b=0;
  Int_t nbins;
  Float_t xmax, xmin;
  
  filename = filename + ".txt";
 
  ifstream file(filename.c_str());

  if(!file)
    {
      cout<<"Error opening input file"<<endl;
      return -1;
    }
  file >> nbins >> xmin >> xmax;
  //  cout << nbins << " " << xmin << " " << xmax << endl << endl;
  while(!file.eof()) {
    file >> test;
    corrfac[b][a] = test;
    a++; 
    //    cout << std::setw(10) << test;
    if (a == 51) {
      a = 0;
      b++;
      //cout << endl << endl;
    }
  }
  cout << endl;



  Float_t binsize = (xmax-xmin)/nbins;
  
  // loop over all MC input files
  
  for (unsigned iinput=0;iinput<mcsamples.size();iinput++) {
    for (unsigned j=0;j<mcsamples[iinput].ntrees();j++) {
    // open input file                                                                                                                
      string input = mcsamples[iinput].filename(j);//mcs[iinput]; //input = datapath + "/" + input;                                                                  
      TFile* ifile = new TFile(input.c_str(),"READ");
      if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }
      
      // create temporary output file                                                                                                   
      size_t pos = input.find(".root");
      string output = input.substr(0,pos);
      output = output + ".rew.root";
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
	if (0==itree->FindBranch("jteta")){ cout<<"no jteta branch stored!"<<endl; return 0; }
	//if (0==itree->FindBranch("nlepz")){ cout<<"no Z->ll branches stored!"<<endl; return 0; }                                      
	
	// check wether sample is considered MC (weights to be stored)                                                                  
	if (0!=itree->FindBranch("weight_lqeta"))      itree->SetBranchStatus("weight_lqeta",     0);
	
	// clone input tree into output directory                                                                                       
	odir->cd();
	TTree* otree = itree->CloneTree();
	cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;
	
	Float_t weight_lqeta=-99;
	TBranch* b_weight_lqeta                =otree->Branch("weight_lqeta",       &weight_lqeta,    "weight_lqeta/F");
	
	
	
	//	Float_t popvar8=-99;      itree->SetBranchAddress("popvar8",   &popvar8);
	Float_t jteta[140]={-99};      itree->SetBranchAddress("jteta",   jteta);
	Int_t njt=-99;            itree->SetBranchAddress("njt", &njt);

	unsigned nevt = static_cast<unsigned>(itree->GetEntries());
	THStatus * status = new THStatus(nevt);
	cout<<nevt<<" events will be processed"<<endl;
	for (unsigned ievt=0;ievt<nevt;ievt++) {
	  
	  itree->GetEntry(ievt);
	  
	  status->print_status(ievt);
	  
	  weight_lqeta = -99;
	  
	  for (Int_t ijt=0; ijt<njt; ijt++){
	    //	    cout << "Number of Jets : " << njt << "    Current ijt: " << ijt << "     Current weight: " << weight_lqeta << endl;
	    for (Int_t ibin=1; ibin<nbins; ibin++){
	      if (jteta[ijt] >= (xmin+(ibin-1)*binsize) && jteta[ijt] < (xmin+ibin*binsize))  {
		if (weight_lqeta == -99) weight_lqeta = corrfac[iinput][ibin];
		else weight_lqeta = weight_lqeta * corrfac[iinput][ibin];
		//		cout << "In Bin " << ibin << " and getting weight " << corrfac[iinput][ibin] << "   with jteta = " << jteta[ijt] << "   lieing between " << (xmin+(ibin-1)*binsize) << "   and  " << (xmin+ibin*binsize) << endl;
	      }
	    }
	    if (weight_lqeta==-99) cout << "Not any bin found! JetEta: " << jteta[ijt] << endl;
	  } // njt
	  
	  //cout << "#########################" << endl << "### FINAL WEIGHT: " << weight_lqeta << "         ########" << endl;
	  b_weight_lqeta -> Fill();
	}  // ievt
      } //idir
      
      
      cout<<"Save extended tree to "<<input<<" ... "<<flush;
      ifile->Close();
      delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv -f " + output + " " + input;
    gSystem->Exec(cmd.c_str());
    cout<<"DONE!\n"<<endl;

    } // mcfile
  } // mcsample
  
  
  return 0;
}



//______________________________________________________________________________                                                      
string get_cut(const vector<string>& cuts,unsigned icut,const string& mode)
{
  if (icut>=cuts.size()) return string("");
  string tmp(cuts[icut]);
  size_t pos;
  pos = tmp.find(':');
  if (pos==string::npos) return tmp;
  string cutlabel = tmp.substr(0,pos);
  string cutexpression= tmp.substr(pos+1);
  return (mode.find("label")==string::npos) ? cutexpression : cutlabel;
}

