////////////////////////////////////////////////////////////////////////////////
//
// utm_skim_x
// ----------
//
//            01/12/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"
#include "../interface/RunRangeManager.h"

#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TFormula.h>
#include <TObjString.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "TROOT.h"
#include "TPluginManager.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// helper function
////////////////////////////////////////////////////////////////////////////////
string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression"); 


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char**argv)
{
  gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo","*", "TStreamerInfo","RIO", "TStreamerInfo()");
  
  //
  // evaluate command line / configuration file options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  string         input     = cl.getValue<string> ("input");
  vector<string> cuts      = cl.getVector<string>("cuts");
  vector<string> dirs      = cl.getVector<string>("dirs",           "");
  string         treename  = cl.getValue<string> ("treename",      "t");
  string         output    = cl.getValue<string> ("output","skim.root");
  bool           cutsOR    = cl.getValue<bool>   ("or",          false);
  bool           noeff     = cl.getValue<bool>   ("noeff",       false);

  if (!cl.check()) return 0;
  cl.print();

  
  //
  // open input and output files
  //
  TFile* ifile = new TFile(input.c_str(),"READ");
  if (!ifile->IsOpen()) return 0;
  
  TFile* ofile = new TFile(output.c_str(),"RECREATE");
  if (!ofile->IsOpen()) return 0;


  //
  // skimming according to JSON file?
  //
  string jsonfile;
  RunRangeManager rrm;
  if (cuts.size()==1&&get_cut(cuts,cuts.size()-1,"expression").find(".json")!=string::npos) {
    jsonfile = get_cut(cuts,cuts.size()-1,"expression");
    rrm.readJsonFile(jsonfile);
    cout<<"Skim according to JSON file "<<jsonfile<<": "
	<<rrm.nRuns()<<" runs, "<<rrm.nLbns()<<" lbns"<<endl;
  }
  
  
  //
  // construct selection string and adjust TFormula limits
  //
  string selection;
  for (unsigned icut=0;icut<cuts.size();icut++) {
    if (!selection.empty()) selection += (cutsOR) ? "||" : "&&";
    selection += get_cut(cuts,icut,"expression");
  }
  TFormula::SetMaxima(selection.size(),selection.size(),selection.size());
  
  
  //
  // loop over input directories
  //
  TIter nextDir(ifile->GetListOfKeys());
  TKey* dirKey(0);
  
  while ((dirKey=(TKey*)nextDir())) {
    if (strcmp(dirKey->GetClassName(),"TDirectoryFile")!=0) continue;
    TDirectory* idir    = (TDirectory*)dirKey->ReadObj();
    string      dirname = idir->GetName();
    if (dirs.size()>0&&find(dirs.begin(),dirs.end(),dirname)==dirs.end()) continue;
    TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str()); odir->cd();
  
    // standard skimming
    if (jsonfile.empty()) {
      TTree* itree = (TTree*)idir->Get(treename.c_str());
      TTree* otree = itree->CopyTree(selection.c_str());
      if(!noeff){
	double ntotal     = (double)itree->GetEntries();
	double nselected  = (double)otree->GetEntries();
	double efficiency = nselected / ntotal;
	stringstream ss; ss<<"efficiency:"<<efficiency;
	otree->GetUserInfo()->Add(new TObjString(ss.str().c_str()));
	cout<<dirname<<": "<<nselected<<" / "<<ntotal<<" ("<<efficiency<<")"<<endl;
      }
      else
	cout<<dirname<<"(no efficiency filled!!) "<<endl;
      otree->Write();
	
    }
    // json file skimming
    else {
      TTree* itree = (TTree*)idir->Get(treename.c_str());
      TTree* otree = itree->CloneTree(0);
      unsigned run,lbn;
      itree->SetBranchAddress("run",&run);
      itree->SetBranchAddress("lbn",&lbn);
      unsigned long nevts = itree->GetEntries();
      for (unsigned long ievt=0;ievt<nevts;ievt++) {
	itree->GetEntry(ievt);
	if (rrm.contains(run,lbn)) otree->Fill();
      }
      otree->Write();
      cout<<dirname<<": "<<otree->GetEntries()<<" / "<<itree->GetEntries()<<endl;
    }
  }
  
  
  //
  // close both files and ciao!
  //
  ifile->Close();
  delete ifile;
  ofile->Close();
  delete ofile;
  

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
