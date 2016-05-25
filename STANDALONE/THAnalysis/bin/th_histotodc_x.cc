////////////////////////////////////////////////////////////////////////////////
//
// theta_makedatacards_x
// ---------------------
//
//                                          10/11/2011 Hauke Held <held@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "interface/Histogram.h"
#include "interface/MCSample.h"
#include "interface/DataSample.h"

#include "interface/CommandLine.h"
#include "interface/RootStyle.h"

#include <TApplication.h>
#include <TStyle.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TTree.h>
#include <THStack.h>
#include <TH1F.h>
#include <TEventList.h>
#include <TObject.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <cfloat>
#include <fstream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void setHBinsNonZero(TH1F* h);
string get_cut(const vector<string>& cuts,unsigned icut,const string& mode);
string get_process(const vector<string>& processes,const string& sample);

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  /*  
  vector<string> mcs       = cl.getVector<string>  ("mcs",                 "");
  double         lumi      = cl.getValue <double>  ("lumi");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  string         dirname   = cl.getValue <string>  ("dirname",  "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");

  vector<string> cuts      = cl.getVector<string>  ("cuts",        "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",      "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",   "");
  string         varname   = cl.getValue <string>  ("varname",     "");
  string         varexpr   = cl.getValue <string>  ("varexpr",     "");
  unsigned       nbinsx    = cl.getValue <unsigned>("nbinsx",       0);
  vector<double> binsx     = cl.getVector<double>  ("binsx",       "");
  double         xmin      = cl.getValue <double>  ("xmin",     -1E10);
  double         xmax      = cl.getValue <double>  ("xmax",     +1E10);
  bool           overflow  = cl.getValue <bool>    ("overflow", false);
  bool           batch     = cl.getValue <bool>    ("batch",     true);
  vector<string> formats   = cl.getVector<string>  ("formats",     "");
  vector<string> prefix    = cl.getVector<string>  ("prefix",              "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",              "");
  // dummys
  string         datalabel = cl.getValue <string>  ("datalabel",       "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");
  vector<string> labels    = cl.getVector<string>  ("labels",              "");
  // additions for datacard handling
  string         opath     = cl.getValue <string>  ("opath",              ".");
  string         rootfile  = cl.getValue <string>  ("rootfile","datacard.root");
  vector<string> processes = cl.getVector<string>  ("processes",           "");
  string         systematic= cl.getValue <string>  ("systematic",          "");
  string         direction = cl.getValue <string>  ("direction",           "");
  
  if(!cl.check()) return 0;
  cl.print();
  
  if (!systematic.empty()&&direction.empty()) {
    cout<<"Error: direction Up/Down for systematic missing"<<endl; return 0;
  } */
 
  string         datacard  = cl.getValue <string>  ("datacard","datacard.txt");
  string         channel   = cl.getValue <string>  ("channel",         "3t");
  string         sys= cl.getValue <string>  ("sys",          "");
  string         output  = cl.getValue <string>  ("output","test.root");
  bool           setdummy = cl.getValue <bool>   ("setdummy", false);
  
  //  rootfile = opath + "/" + rootfile;

  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //
  TH1::SetDefaultSumw2();
  
  //
  // CHECK DATACARD
  //


  if (datacard == "datacard.txt") datacard = channel+".txt";
  if (output   == "test.root")    output   = channel+".root";
  

  bool sysisempty;
  if (sys==""){ sysisempty = true;}
  else { sysisempty = false;}
  
  string process[6] = {"thq",
		       "Wjets",
		       "t-tWchan",
		       "t-tchan",
		       "ttbar",
		       "tth"};

  string systematic[13] = {"Nominal",
			   "PileUpUp",
			   "PileUpDown",
			   "TagRateUp",
			   "TagRateDown",
			   "MistagRateUp",
			   "MistagRateDown",
			   "JECUp",
			   "JECDown",
			   "JERUp",
			   "JERDown",
			   "METUnclusteredUp",
			   "METUnclusteredDown"};

  Float_t mcrates[6];
  Int_t datarates=0;


  if(sysisempty){
    TFile* ofile = new TFile(output.c_str(),"RECREATE");
    if (!ofile->IsOpen()) { cout<<"Failed to open "<<output<<endl; return 0; }
    
    if(setdummy) {
      TH1F* data_dummy = new TH1F("th3t__data_obs__Nominal","Fake Data Histogram",1,0,1);
      string dummyname = channel+"__data_obs__Nominal";
      data_dummy->SetName(dummyname.c_str());
      //    data_dummy->Write();
    }

    for(Int_t i = 0; i<6; i++){
    
      string input = process[i]+".root";
      TFile* ifile = new TFile(input.c_str(),"READ");
      if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }
      
      ofile->cd();
      
      
      TH1F* histo[13];
      
      histo[0] = (TH1F*)ifile->Get("Nominal");
      histo[1] = (TH1F*)ifile->Get("PileUpUp");
      histo[2] = (TH1F*)ifile->Get("PileUpDown");
      histo[3] = (TH1F*)ifile->Get("TagRateUp");
      histo[4] = (TH1F*)ifile->Get("TagRateDown");
      histo[5] = (TH1F*)ifile->Get("MistagRateUp");
      histo[6] = (TH1F*)ifile->Get("MistagRateDown");
      histo[7] = (TH1F*)ifile->Get("JECUp");
      histo[8] = (TH1F*)ifile->Get("JECDown");
      histo[9] = (TH1F*)ifile->Get("JERUp");
      histo[10] = (TH1F*)ifile->Get("JERDown");
      histo[11] = (TH1F*)ifile->Get("METUnclusteredUp");
      histo[12] = (TH1F*)ifile->Get("METUnclusteredDown");  
      
      mcrates[i] = histo[0]->Integral();
      
      TH1F* o_histo[13];
      string histoname;
      
      for(Int_t j =0; j<13; j++){
	//histo[j]->Write();
	o_histo[j] = histo[j];
	histoname = channel+"__"+process[i]+"__"+systematic[j];
	o_histo[j]->SetName(histoname.c_str());
	o_histo[j]->Write();
      }
    
    }
    ofile->Write();
    for(Int_t i =1; i<6;i++){
      datarates+=mcrates[i];
    }


  }
  int ws(35),wp(12);
  if (sysisempty) {
    
    cout<<"New datacard "<<datacard<<" is being written... ";
    ofstream dcfile;
    dcfile.open(datacard.c_str(),ofstream::trunc); assert (dcfile.is_open());
    stringstream ss;
    ss<<left;
    ss<<setw(7)<<"imax"<<"1"<<"   number of channels\n";
    ss<<setw(7)<<"jmax"<<"5"<<"   number of backgrounds\n";
    ss<<setw(7)<<"kmax"<<"*"<<"   number of systematics (automatic)\n";
    ss<<setfill('-')<<setw(ws)<<""<<endl<<setfill(' ');
    ss<<"shapes"<<" * * "<<output.c_str()
      <<"  $CHANNEL__$PROCESS__Nominal  $CHANNEL__$PROCESS__$SYSTEMATIC\n";
    ss<<setfill('-')<<setw(ws)<<""<<endl<<setfill(' ');
    ss<<setw(ws)<<"bin"<<"   "<<setw(wp)<<channel<<endl;
    ss<<setw(ws)<<"observation"<<"   "<<setw(wp)<<datarates<<endl;
    ss<<setfill('-')<<setw(ws)<<""<<endl<<setfill(' ');
    ss<<setw(ws)<<"bin"<<"   "; 
    for (unsigned i=0;i<6;i++) {ss<<setw(wp)<<channel;} ss<<endl;
    ss<<setw(ws)<<"process"<<"   "; 
    for (unsigned i=0;i<6;i++) {ss<<setw(wp)<<process[i];} ss<<endl;
    ss<<setw(ws)<<"process"<<"   "; 
    for (unsigned i=0;i<6;i++) {ss<<setw(wp)<<i;} ss<<endl;
    ss<<setw(ws)<<"rate"<<"   "; 
    for (unsigned i=0;i<6;i++) {ss<<setw(wp)<<mcrates[i];} ss<<endl;
    ss<<setfill('-')<<setw(ws)<<""<<endl<<setfill(' ');          
    dcfile<<ss.str().c_str();
    dcfile.close();
    cout<<"done."<<endl;
  }
  else {
    cout<<"Datacard "<<datacard<<" is being appended... ";
    ofstream dcfile;
    dcfile.open(datacard.c_str(),ofstream::app); assert (dcfile.is_open());
    stringstream ss; ss<<left;
    ss<<setw(ws-7)<<sys<<setw(7)<<"shape"<<"   ";
    for (unsigned i=0;i<6;i++) {ss<<setw(wp)<<"1";} ss<<endl;
    dcfile<<ss.str().c_str();
    dcfile.close();
    cout<<"done."<<endl;
  } 
  // else cout<<"Datacard "<<datacard<<" and entry already existing"<<endl;
  
  
  
}
