////////////////////////////////////////////////////////////////////////////////
//
// th_eventlist_x
// -------------
//
//            01/12/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
//                       Simon Fink
////////////////////////////////////////////////////////////////////////////////


#include "../interface/DataSample.h"
#include "../interface/MCSample.h"

#include "../interface/CommandLine.h"
#include "../interface/RootStyle.h" 

#include <TApplication.h> 
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TEventList.h>
#include <TStyle.h> 
#include <TH1F.h> 
#include <TH1.h> 
#include <TCanvas.h> 
#include <TColor.h> 
#include <TAxis.h> 
#include <TPad.h> 
#include <THStack.h> 
#include <TLegend.h> 
#include <TLatex.h> 


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <cassert> 
#include <algorithm> 

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper function
////////////////////////////////////////////////////////////////////////////////
string get_leglabel(const vector<string>&labels,const string& sample);
int    get_color(const vector<string>& colors,const string&sample);
void   draw_labels(const vector<string>& labels,bool leginplot=true,bool res=true);
string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression"); 
int    get_index_cut(const vector<string>&cuts,const string& cut);


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char**argv)
{
  //
  // evaluate command line / configuration file options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> mcs        = cl.getVector<string>("mcs",            "");
  string         dirname    = cl.getValue<string> ("dirname",     "utm");
  string         treename   = cl.getValue<string> ("treename",      "t");
  string         filesuffix = cl.getValue <string>("filesuffix",".root");
  string         datapath   = cl.getValue<string> ("datapath",       "");
  string         mcpath     = cl.getValue<string> ("mcpath",         "");
  string         filename   = cl.getValue<string> ("filename", "output");
  double         lumi       = cl.getValue<double> ("lumi",          0.0);
  vector<string> cuts       = cl.getVector<string>("cuts");
  vector<string> mclabels   = cl.getVector<string>("mclabels",       "");
  vector<string> mccolors   = cl.getVector<string>("mccolors",       "");
  vector<string> mcweights  = cl.getVector<string>("mcweights",      "");

  if (!cl.check()) return 0;
  cl.print();

  //
  // open mc files, retrieve trees, cross-sections, and k-factors
  //
  vector<MCSample> mcsamples;
  if(mcs.size()>1){
    cout << "Only use one tree for the moment\n"; return 0;
  }

  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0)MCSample::printSampleInfos(mcsamples);

  // one row for each cut
  string selection;
  vector<TEventList*> vec_elist;
  for (unsigned imc=0;imc<mcsamples.size();imc++) {
    MCSample mcsample = mcsamples[imc];
    int ntotal = mcsample.nevents("");
    TTree* itree = mcsample.tree(0);
    mcsample.setWeights(mcweights);
    for (unsigned icut=0;icut<cuts.size();icut++) {
      if (!selection.empty()) selection += "&&";
      selection += get_cut(cuts,icut,"expression");
      double nevts = mcsample.nevents(selection,lumi);
      TEventList* elist = new TEventList("elist", "elist");
      itree->Draw(">>elist", selection.c_str());
      cout << "Step " << icut << ":" << endl;
      cout << itree->GetEntries(selection.c_str()) << " " << elist->GetN() <<  endl;
      ULong64_t evt=0; itree->SetBranchAddress("evt", &evt);
      UInt_t lbn=0; itree->SetBranchAddress("lbn", &lbn);
      UInt_t run=0; itree->SetBranchAddress("run", &run);
      char name[10];
      sprintf(name,"%i",icut);
      ofstream myfile((filename+"_Step"+name+".txt").c_str());
      if (myfile.is_open()){
	//	myfile << "************************************************\n";
	//	myfile << "*    Row   * LumiBlock *    RunNum *    EvtNum *\n";
	//      myfile << "************************************************\n";
	for(int iev=0;iev<elist->GetN();iev++){
	  itree->GetEntry(elist->GetEntry(iev));
	  //	  myfile<< "* " << setw(8) << iev << " *    " << lbn << " *    " << run << " *  " << evt << " *\n";
	  myfile<< run << "  " << lbn <<  "  " <<  evt << endl;
	}
	myfile.close();
      }
      
      delete elist;      
    }
  }
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

//______________________________________________________________________________
int get_index_cut(const vector<string>&cuts,const string& cut)
{
  for (int i=0;i<(int)cuts.size();i++) {
    if ( cuts[i].find(cut.c_str())!=string::npos ) {
      cout<<"Return index: "<<i<<endl;
      return i;
    }
  }
  cout<<"index for cut: "<<cut<<" not found in cuts vector\n";
  return 0;
}
