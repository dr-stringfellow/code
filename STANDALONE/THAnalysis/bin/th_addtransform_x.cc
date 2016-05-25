////////////////////////////////////////////////////////////////////////////////
//
// th_addtransform_x
// ------------
//
////////////////////////////////////////////////////////////////////////////////


#include "../interface/MCSample.h"
#include "../interface/CommandLine.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <map>

#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TKey.h"
#include "TControlBar.h"
#include "TSystem.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TObjString.h"
#include "TChain.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  string         input     = cl.getValue <string>  ("input");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         ipath     = cl.getValue <string>  ("ipath",              ".");
  string         bdtpath   = cl.getValue <string>  ("bdtpath",            ".");
  bool           expert    = cl.getValue <bool>    ("expert",           false);
  string         ntrees    = cl.getValue<string>   ("ntrees",              "");
  string         nevtmin   = cl.getValue<string>   ("nevtmin",             "");
  // name for historical reasons, can also be mlpout, etc. 
  string         bdtout    = cl.getValue <string>  ("bdtout"             );
  bool           fixinput  = cl.getValue <bool>    ("fixinput",         false); 
  // when fixinput = true always take bdtout (no extensions) from input file
  // output branch is still modified by ntrees and nevtmin, so they can be hijacked for eg. binning studies
 

  if(!cl.check()) return 0;
  cl.print();

  if(ntrees!="") ntrees = "_"+ntrees;
  if(nevtmin!="") nevtmin = "_"+nevtmin;

  double array[50][5];
  int    nbinsx[5];
  double bdtmin[5];
  double bdtmax[5];
  
  for(UInt_t iexp=0; iexp<5; iexp++){
    TString stexp = "";
    switch(iexp){
    case 0: stexp = ""; break;
    case 1: stexp = "_vlight"; break;
    case 2: stexp = "_ttbar"; break;
    case 3: stexp = "_vv"; break;
    case 4: stexp = "_sig"; break;
    default: stexp = "";
    }
    TString bdtexpr = bdtout + stexp;
    ifstream file(((TString)bdtpath+"/"+bdtexpr+ntrees+nevtmin+".txt"));
    int a = 0;
    if(!file)
      {
	cout<<"Error opening input file"<<endl;
	return -1;
      }
    while(!file.eof())
      {
	file >> array[a][iexp];
	a++;
      }
    nbinsx[iexp] = a-4;
    bdtmin[iexp] = array[a-3][iexp];
    bdtmax[iexp] = array[a-2][iexp];
    cout << bdtexpr << " has " << nbinsx[iexp] << " bins from " <<  bdtmin[iexp] << " to " << bdtmax[iexp] << endl;
    if(!expert) break;
  }
  
  TFile* ifile = new TFile((ipath+"/"+input).c_str(),"READ");
  if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }
  
  // create temporary output file
  size_t pos = input.find(".root");
  string output = input.substr(0,pos);
  output = output + ".bdt.root";
  TFile* ofile = new TFile((ipath+"/"+output).c_str(),"RECREATE");
  if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
 
  // read input directory
  TDirectory* idir    = (TDirectory*)ifile->Get(dirname.c_str());
  cout<<input<<"/"<<dirname<<":"<<endl;
  
  // create output directory
  TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
  if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}
  
  // open input tree
  TTree* itree = (TTree*)idir->Get(treename.c_str());
  if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<dirname<<endl;return 0;}
  
  // don't copy bdtout branch if it already exists
  for(UInt_t iexp=0; iexp<5; iexp++){
    TString stexp = "";
    switch(iexp){
    case 0: stexp = ""; break;
    case 1: stexp = "_vlight"; break;
    case 2: stexp = "_ttbar"; break;
    case 3: stexp = "_vv"; break;
    case 4: stexp = "_sig"; break;
    default: stexp = "";
    }
    TString bdtexpr = bdtout + "trans" + stexp + ntrees + nevtmin;
    if (0!=itree->FindBranch(bdtexpr)) itree->SetBranchStatus(bdtexpr,0);
    if(!expert) break;
  }

  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree();
  cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;
  
  Float_t  obdtout[5];
  Float_t  ibdtout;
  TBranch* b_bdtout[5];

  TString bdtexpr = bdtout;
  if(!fixinput)
    bdtexpr += ntrees + nevtmin;

  for(UInt_t iexp=0; iexp<5; iexp++){
    TString stexp = "";
    switch(iexp){
    case 0: stexp = ""; break;
    case 1: stexp = "_vlight"; break;
    case 2: stexp = "_ttbar"; break;
    case 3: stexp = "_vv"; break;
    case 4: stexp = "_sig"; break;
    default: stexp = "";
    }
    //      if (0!=itree->FindBranch(bdtexpr)) itree->SetBranchStatus(bdtexpr2,1);
    TString bdtexpr2 = bdtout + "trans" + stexp + ntrees + nevtmin;
    obdtout[iexp] = 0.0;
    b_bdtout[iexp] = otree->Branch(bdtexpr2,&obdtout[iexp],bdtexpr2+"/F");
    if(!expert) break;
  }
  ibdtout = 0;
  itree->SetBranchAddress(bdtexpr,  &ibdtout);

  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  cout<<nevt<<" events will be processed"<<endl;
  
  //event loop
  
  for (unsigned ievt=0;ievt<nevt;ievt++) {
    
    if (ievt%10000 == 0) 
      std::cout << "--- ... Processing event: " << ievt << std::endl;
    
    itree->GetEntry(ievt);
    
    
    
    for(UInt_t iexp=0; iexp<5; iexp++){
      int tmp_nbinsx = nbinsx[iexp];
      double tmp_bdtmin = bdtmin[iexp];
      double tmp_bdtmax = bdtmax[iexp];
      for(int i=0; i< tmp_nbinsx; i++){
	if(ibdtout >= array[i][iexp] && ibdtout < array[i+1][iexp]){
	  obdtout[iexp]= tmp_bdtmin + (tmp_bdtmax - tmp_bdtmin)*((float)i + 0.5)/tmp_nbinsx;
	}
      }
      b_bdtout[iexp]->Fill();
      if(!expert) break;
    }    
  } //event loop
  
  cout << "End of event loop \n";
  otree->Write();
  ofile->Close();
  ifile->Close();
  cout << "Wrote file *.bdt.root \n";
  delete ifile;
  delete ofile;
  
  string cmd_str = "mv -f " + output + " " + input;
  gSystem->Exec(cmd_str.c_str());
  
  cout << "BDT output transformed!" << endl;
  
  return 0;
  
}

