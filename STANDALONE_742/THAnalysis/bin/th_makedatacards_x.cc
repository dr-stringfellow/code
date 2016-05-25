///////////////////////////////////////////////////////////////////////////////////////
//
// th_makedatacards_x
// ---------------------
//
//                                          10/11/2011 Hauke Held <held@cern.ch>
//                                          02/09/2013 Benedikt Maier <bmaier@cern.ch>
//                 add histograms to dc     24/03/2014 Christian Boeser <christian.boser@cern.ch>
///////////////////////////////////////////////////////////////////////////////////////


#include "../interface/Histogram.h"
#include "../interface/MCSample.h"
#include "../interface/DataSample.h"

#include "../interface/CommandLine.h"
#include "../interface/RootStyle.h"

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
#include <RooHistError.h>


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
void get_from_histo(const string hname, string& hchannel, string& hprocess, string& hsystematic);

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  vector<string> data      = cl.getVector<string>  ("data",                 "");
  vector<string> mcs       = cl.getVector<string>  ("mcs",                  "");
  double         lumi      = cl.getValue <double>  ("lumi");
  string         datapath  = cl.getValue <string>  ("datapath",             "");
  string         mcpath    = cl.getValue <string>  ("mcpath",               "");
  string         dirname   = cl.getValue <string>  ("dirname",           "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",      ".root");
  string         treename  = cl.getValue <string>  ("treename",            "t");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                 "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",               "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",            "");
  string         varname   = cl.getValue <string>  ("varname",              "");
  string         varexpr   = cl.getValue <string>  ("varexpr",              "");
  unsigned       nbinsx    = cl.getValue <unsigned>("nbinsx",                0);
  vector<double> binsx     = cl.getVector<double>  ("binsx",                "");
  double         xmin      = cl.getValue <double>  ("xmin",              -1E10);
  double         xmax      = cl.getValue <double>  ("xmax",              +1E10);
  bool           overflow  = cl.getValue <bool>    ("overflow",           true);
  bool           underflow  = cl.getValue <bool>    ("underflow",           true);
  bool           batch     = cl.getValue <bool>    ("batch",              true);
  vector<string> formats   = cl.getVector<string>  ("formats",              "");
  vector<string> prefix    = cl.getVector<string>  ("prefix",               "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",               "");
  // dummys
  string         datalabel = cl.getValue <string>  ("datalabel",        "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",             "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",             "");
  vector<string> labels    = cl.getVector<string>  ("labels",               "");
  // additions for datacard handling
  string         datacard  = cl.getValue <string>  ("datacard", "datacard.txt");
  string         opath     = cl.getValue <string>  ("opath",               ".");
  string         rootfile  = cl.getValue <string>  ("rootfile","datacard.root");
  string         channel   = cl.getValue <string>  ("channel",          "Wmnu");
  vector<string> processes = cl.getVector<string>  ("processes",            "");
  string         systematic= cl.getValue <string>  ("systematic",           "");
  string         direction = cl.getValue <string>  ("direction",            "");
  unsigned       howmanysigs = cl.getValue <unsigned> ("howmanysigs",        1);
  bool           blind     = cl.getValue <bool>    ("blind",             false);
  // parameter "theta" is only valid when doing blind analysis where we put a fake 
  // data__obs__nominal from MC in the datacard (which we do not use when blinded)
  // combine checks if the integral of this is the same as the number in observation
  // theta checks if the number in observation is an integer ... 
  bool           theta     = cl.getValue <bool>    ("theta",             false);
  string         histo     = cl.getValue <string>  ("histo",                 "");
  string         hpath     = cl.getValue <string>  ("hpath",                 "");
  double         ddscalefactor = cl.getValue<double> ("ddscalefactor",    1);

  if(!cl.check()) return 0;
  cl.print();
  
  if (!systematic.empty()&&direction.empty()) {
    cout<<"Error: direction Up/Down for systematic missing"<<endl; return 0;
  }

  rootfile = opath + "/" + rootfile;

  if (batch) {}


  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //
  TH1::SetDefaultSumw2();

  TH1F* myhist;
  vector<TH1F*> myhist2;
  vector<TH1F*> myhistsys; 
  string tmphist;
  TFile* hfile;
  
  if(!histo.empty()){ 
    string input = hpath+"/"+histo;
    //    cout << "Read " << input.c_str() << endl;
    //    size_t pos = histo.find(".root");
    hfile = new TFile(input.c_str(),"READ");
    if (!hfile->IsOpen()){cout<<"Failed to access "<<input.c_str()<<endl;return 0;}
    TIter next(hfile->GetListOfKeys());
    TKey* key(0);
    while ((key=(TKey*)next())) {
      TH1F* tmphist = (TH1F*)key->ReadObj();
      string tmpprocess, tmpchannel, tmpsys;
      string tmpname = tmphist->GetName();
      // Fixes for Dan's naming
      if(tmpname.find("_e_") != string::npos){
	tmphist->SetName((tmpname.replace(tmpname.find("_e_"), 3, "_el_")).c_str());
      }
      if(tmpname.find("_3t_") != string::npos){
	tmphist->SetName((tmpname.replace(tmpname.find("_3t_"), 4, "_3tt_")).c_str());
      }
      if(tmpname.find("_4t_") != string::npos){
	tmphist->SetName((tmpname.replace(tmpname.find("_4t_"), 4, "_4tt_")).c_str());
      }
      if(tmpname.find("__CMS_tHq_nominal") != string::npos){
	tmphist->SetName((tmpname.replace(tmpname.find("__CMS_tHq_nominal"), 17, "__nominal")).c_str());
      }
      get_from_histo(tmphist->GetName(), tmpchannel, tmpprocess, tmpsys);
      //      cout << "Channel:" << channel << " -> " << tmpchannel << "__"<<tmpprocess << "__"<< tmpsys << endl;
      tmphist->Scale(ddscalefactor);
      if(channel.find(tmpchannel)!=string::npos){
	if(strcmp(tmpsys.c_str(), "nominal")!=0){
	  myhistsys.push_back(tmphist);
	}
	else{
	  myhist2.push_back(tmphist);
	}
      }
    }
    cout << "Found " <<  myhistsys.size() << " syst. shifted templates" << endl;
    cout << "Found " <<  myhist2.size() << " nominal templates" << endl; 
    if(myhist2.size()>=1){
      myhist = myhist2.at(0);
      tmphist = channel+"__TTdd";
    }
    else{
      cout << "Two nominal templates for additional histo!! " << endl;
      return -1;
    }
  }
  cout << "HERE\n";
  //
  // Check DATACARD
  //

  ifstream idcfile (datacard.c_str());
  if (!idcfile.is_open()&&!systematic.empty()) {
    cout<<"Error: systematics called, but datacard "
	<<datacard<<" does not exist"<<endl;
    return 0;
  }
  bool dcwrite = true;
  if (idcfile.is_open()) {
    while (idcfile.good()&&dcwrite) {
      string line;
      getline (idcfile,line);
      if (line.find(systematic.c_str())!=string::npos) dcwrite = false;
    }
  }
  idcfile.close();
  cout << "HERE2\n";
  //
  // OPEN DATA FILE / RETRIEVE TREE
  //
  DataSample datasample=DataSample::load(data,dirname,treename,datapath,filesuffix);
  if (!datasample.empty()) DataSample::printInfo(datasample);

  //
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples;
  cout <<  dirname<< " " <<treename<< " " <<mcpath<< " " <<filesuffix << endl;
  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0) MCSample::printSampleInfos(mcsamples);

  //
  // CREATE HISTOGRAMS DEFINITION
  //
  cout << "HERE3\n";
  //  histogram specification
  map<string,Histogram> name2histo = Histogram::getHistogramMap();
  Histogram histogram;
  const map<string,Histogram>::iterator it = name2histo.find(varname);
  if (it!=name2histo.end()) {
    histogram = it->second;
    if (nbinsx!=0)      histogram.setNbinsx(nbinsx);
    if (binsx.size()>0) histogram.setBinsx(binsx);
    if (xmin>-1E10)     histogram.setXmin(xmin);
    if (xmax<+1E10)     histogram.setXmax(xmax);
  }
  else {
    if (varexpr.empty()) { cout<<"varexpr must be set!"<<endl; return 0; }
    if (nbinsx==0)       { cout<<"nbinsx  must be set!"<<endl; return 0; }
    if (xmin<-1E09)      { cout<<"xmin    must be set!"<<endl; return 0; }
    if (xmax>+1E09)      { cout<<"xmax    must be set!"<<endl; return 0; }
    histogram.setExpr(varexpr);
    histogram.setNbinsx(nbinsx);
    histogram.setBinsx(binsx);
    histogram.setXmin(xmin);
    histogram.setXmax(xmax);
  }
  histogram.setName("");
    
  // overflow bin?
  if (overflow) {
    stringstream ss;
    ss<<"min("<<setprecision(10)<<(histogram.xmax()-1.E-04)
      <<","<<histogram.expr()<<")";
    histogram.setExpr(ss.str());
  }

  if (underflow) {
    stringstream ss;
    ss<<"max("<<setprecision(10)<<(histogram.xmin()+1.E-04)
      <<","<<histogram.expr()<<")";
    histogram.setExpr(ss.str());
  }
  
  // event selection
  string mcsel;
  for (unsigned icut=0;icut<mccuts.size();icut++) {
    if (!mcsel.empty()) mcsel += "&&";
    mcsel += get_cut(mccuts,icut,"expression");
  }
  string selection;
  for (unsigned icut=0;icut<cuts.size();icut++) {
    if (!selection.empty()) selection += "&&";
    selection += get_cut(cuts,icut,"expression");
  }
  mcsel = (mcsel.empty()) ? selection : mcsel + "&&" + selection;
  
  TFile* ifile = new TFile(rootfile.c_str(),"UPDATE");
  if (!ifile->IsOpen()){cout<<"Failed to access "<<rootfile<<endl;return 0;}
  else cout<<rootfile<<" opened."<<endl;

  // save template counts
  vector<float>  mcrates,datarates;
  vector<string> mcprocesses;
  vector<TH1F*>   vec_histos;
  bool ishistobased=0;
  // create MC templates
  for (unsigned imc=0;imc<mcsamples.size();imc++) {
    MCSample mcsample  = mcsamples[imc];
    mcsample.setWeights(mcweights);
    string process = get_process(processes,mcsample.name()); assert (!process.empty());
    string hname = channel+"__"+process;

    // check here if nominal template empty
    // if empty leave the variation empty to avoid errors in theta
    bool isSysEmpty = false;

    if (!systematic.empty()) {
      TH1F* hnominal = (TH1F*) ifile->Get((hname+"__nominal").c_str()); 
      if(hnominal==0){
	cout << "Cannot assert " << (hname+"__nominal").c_str() << endl;
	ishistobased = 1; 
	continue;
      }
      
      // write nominal data-driven histo for systematic
      if (!histo.empty()){
	string tmpchannel, tmpprocess,tmpsys;
	get_from_histo(myhist->GetName(), tmpchannel, tmpprocess,tmpsys);
	string ddname = tmpchannel+"__"+tmpprocess+"__"+systematic+direction;
	myhist->Write(ddname.c_str(),TObject::kOverwrite);
      }


      if (hnominal->Integral()<0.00001) {
	isSysEmpty = true;
	hname +=  "__"+systematic+direction;
	TH1F* htemplate = (TH1F*) hnominal->Clone(hname.c_str());
	cout<<htemplate->GetName()<<": "<<flush;	
	setHBinsNonZero(htemplate);
	htemplate->Write(hname.c_str(),TObject::kOverwrite);
	mcprocesses.push_back(process);
	mcrates.push_back(htemplate->Integral());
	cout<<htemplate->Integral()<<" (empty nominal yield - unchanged)"<<endl;
      }
    }
    
    if (!isSysEmpty) {
      if (systematic.empty()) hname += "__nominal";
      else hname += "__"+systematic+direction;
      TH1F* htemplate = histogram.create(hname.c_str()); 
      cout<<htemplate->GetName()<<": "<<flush;
      mcsample.project(htemplate->GetName(),histogram.expr(),mcsel);
      htemplate->Scale(lumi);
      setHBinsNonZero(htemplate);
      vec_histos.push_back(htemplate);
      htemplate->Write(hname.c_str(),TObject::kOverwrite);
      mcprocesses.push_back(process);
      mcrates.push_back(htemplate->Integral());
      cout<<htemplate->Integral()<<endl;
    }
  }

  if (systematic.empty()&&!datasample.empty()) {
    TH1F* hdata(0);
    string hname = channel+"__data_obs__nominal";
    if(!blind){
      hdata = histogram.create(hname.c_str());
      cout<<hdata->GetName()<<": "<<flush;
      datasample.project(hdata->GetName(),histogram.expr(),selection);
      hdata->Write(hname.c_str(),TObject::kOverwrite);
      datarates.push_back(hdata->Integral());
      cout<<hdata->Integral()<<endl;
    }
    else{
      hdata = histogram.create(hname.c_str());
      cout<<hdata->GetName()<<": "<<flush;
      for(UInt_t i=1; i<vec_histos.size(); i++){
	hdata->Add(vec_histos[i]);
      }
      if(!histo.empty()){
	hdata->Add(myhist);
      }
      hdata->Write(hname.c_str(),TObject::kOverwrite);
      datarates.push_back(hdata->Integral());
      cout<<hdata->Integral()<<endl;
    }
  }

  if(!histo.empty()){
    cout<< myhist->GetName() <<": "<<flush;
    // READ INPUT HISTOS                                                          
    myhist->Write((tmphist+"__nominal").c_str(),TObject::kOverwrite);
    mcrates.push_back(myhist->Integral());
    mcprocesses.push_back("TTdd");
    cout << myhist->Integral() << endl;
  }
  for(int i=0; i< myhistsys.size(); i++){
    cout << "Write " << myhistsys.at(i)->GetName() << " : " << myhistsys.at(i)->Integral()<< endl;
    string tmpprocessdd, tmpchanneldd, tmpsysdd;
    get_from_histo(myhistsys.at(i)->GetName(), tmpchanneldd, tmpprocessdd, tmpsysdd);
    myhistsys.at(i)->Write(myhistsys.at(i)->GetName(),TObject::kOverwrite);
    for(int j=0; j<vec_histos.size(); j++){
      string tmpprocess, tmpchannel, tmpsys;
      get_from_histo(vec_histos.at(j)->GetName(), tmpchannel, tmpprocess, tmpsys);
      TH1F* tmpnom = (TH1F*)ifile->Get((tmpchannel + "__" + tmpprocess + "__nominal").c_str());
      tmpnom->SetName((tmpchannel+"__"+tmpprocess+"__"+tmpsysdd).c_str());
      tmpnom->Write(tmpnom->GetName(),TObject::kOverwrite);
      //vec_histos[i]);
    }
  }

  cout<<rootfile<<" written."<<endl;
  ifile->Close();
  delete ifile;

 
 //
  // UPDATE DATACARD
  //
  
  cout << "#### 1 ####" << endl;

  if (dcwrite) {
    cout << "#### 2 ####" << endl;
    int ws(30),wp(18);

    if (systematic.empty()) {
      cout << "#### 4 ####" << endl;
      cout<<"New datacard "<<datacard<<" is being written... ";
      ofstream dcfile;
      
      dcfile.open(datacard.c_str(),ofstream::trunc); assert (dcfile.is_open());
      stringstream ss;
      
      cout << "#### 5 ####" << endl;
      ss<<left;
      ss<<setw(7)<<"imax"<<"1"<<"   number of channels\n";
      ss<<setw(7)<<"jmax"<<mcprocesses.size()-1<<"   number of backgrounds\n";
      ss<<setw(7)<<"kmax"<<"*"<<"   number of systematics (automatic)\n";
      ss<<setfill('-')<<setw(ws)<<""<<endl<<setfill(' ');
      
      ss<<"shapes"<<" * * "<<rootfile.c_str()
	<<"  $CHANNEL__$PROCESS__nominal  $CHANNEL__$PROCESS__$SYSTEMATIC\n";
      
      ss<<setfill('-')<<setw(ws)<<""<<endl<<setfill(' ');
      ss<<setw(ws)<<"bin"<<"   "<<setw(wp)<<channel<<endl;
      
      
      //  if(!data.empty())
      if(!blind)  ss<<setw(ws)<<"observation"<<"   "<<setw(wp)<<datarates.back()<<endl;
      else {
	double totalbkg = 0;
	for (unsigned i=1;i<mcrates.size();i++) {totalbkg+=mcrates[i];}
	if(theta)
	  ss<<setw(ws)<<"observation"<<"   "<<setw(wp)<<round(totalbkg)<<endl;
	else
	  ss<<setw(ws)<<"observation"<<"   "<<setw(wp)<<totalbkg<<endl;
      }

      ss<<setfill('-')<<setw(ws)<<""<<endl<<setfill(' ');
      ss<<setw(ws)<<"bin"<<"   "; 
      
      for (unsigned i=0;i<mcrates.size();i++) {ss<<setw(wp)<<channel;} ss<<endl;
      ss<<setw(ws)<<"process"<<"   "; 
      for (unsigned i=0;i<mcrates.size();i++) {ss<<setw(wp)<<mcprocesses[i];} ss<<endl;
      ss<<setw(ws)<<"process"<<"   "; 
      for (int i=0;i<mcrates.size();i++) {
	int tmp=int(i-howmanysigs)+1;
	ss<<setw(wp)<<tmp;
      } ss<<endl;
      ss<<setw(ws)<<"rate"<<"   "; 
      for (unsigned i=0;i<mcrates.size();i++) {ss<<setw(wp)<<mcrates[i];} ss<<endl;
      ss<<setfill('-')<<setw(ws)<<""<<endl<<setfill(' ');     
           
      dcfile<<ss.str().c_str();
      dcfile.close();
      cout<<"done."<<endl;
      
    }
    else{
      cout<<"Datacard "<<datacard<<" is being appended... ";
      ofstream dcfile;
      dcfile.open(datacard.c_str(),ofstream::app); assert (dcfile.is_open());
      stringstream ss; ss<<left;
      ss<<setw(ws-7)<<systematic<<setw(7)<<"shape"<<"   ";
      if(!histo.empty()){
	for (unsigned i=0;i<mcrates.size();i++) {ss<<setw(wp)<<"1";} 
	//	ss<<setw(wp)<<"-"<<endl;
	ss<<endl;
      }
      else{
	for (unsigned i=0;i<mcrates.size();i++) {
	  ss<<setw(wp)<<"1";
	}
	if(ishistobased) ss<<setw(wp)<<"-";
	ss<<endl;
      }
      dcfile<<ss.str().c_str();
      dcfile.close();
      cout<<"done."<<endl;
    }

    if( myhistsys.size()!=0 && systematic.empty() ){
      cout<<"Datacard "<<datacard<<" is being appended... ";
      ofstream dcfile;
      dcfile.open(datacard.c_str(),ofstream::app); assert (dcfile.is_open());
      for(int isys=0; isys<myhistsys.size(); isys++){
	TH1F* tmphist = myhistsys.at(isys);
	stringstream ss; ss<<left;
	string tmpprocess, tmpchannel, tmpsys;
	get_from_histo(tmphist->GetName(), tmpchannel, tmpprocess, tmpsys);
	if(tmpsys.find("Down")!=string::npos){
	  //	  cout << "found down" << endl;
	  continue;}
	if(tmpsys.find("Up")!=string::npos){
	  //cout << "found up" << endl;
	  tmpsys.erase(tmpsys.size() - 2);
	}
	ss<<setw(ws-7)<<tmpsys<<setw(7)<<"shape"<<"   ";
	for (unsigned i=0;i<mcrates.size();i++) {ss<<setw(wp)<<"1";}
	//	ss<<setw(wp)<<"1"<<endl;
	ss<<endl;
	dcfile<<ss.str().c_str();
      }
    }
  }
  else cout<<"Datacard "<<datacard<<" and entry already existing"<<endl;

  if(!histo.empty()){
    hfile->Close();
    delete hfile;
  }

  //
  // RUN ROOT APPLICATION
  //
  //if (!batch) app->Run();

  return 0;
}


//______________________________________________________________________________
void setHBinsNonZero(TH1F* h)
{
  // function to set all bins in a histogram to non empty values
  // this is needed to ensure proper working of the likelihood ratio 
  // minimizations inside the >> theta-framework.org

  if (0==h) return;
  
  for (Int_t i=1;i<=h->GetNbinsX();i++) {
    if (h->GetBinContent(i)<FLT_EPSILON) {
      h->SetBinContent(i,FLT_EPSILON); 
      h->SetBinError(i,sqrt(FLT_EPSILON)); 
    }
  }
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
string get_process(const vector<string>& processes,const string& sample)
{
  for (unsigned isample=0;isample<processes.size();isample++) {
    string tmp(processes[isample]);
    size_t pos = tmp.find(":"); assert (pos!=string::npos);
    if (sample==tmp.substr(0,pos)) return tmp.substr(pos+1);
  }
  cout<<"Error: get_process() did not find "<<sample<<endl;
  return "";
}

//______________________________________________________________________________
void get_from_histo(const string hname, string& hchannel, string& hprocess, string& hsystematic){
  string tmp = hname;
  size_t pos = tmp.find("__");
  if (pos!=string::npos) {
    hchannel      = tmp.substr(0,pos);
    tmp = tmp.substr(pos+2);
    size_t pos2 = tmp.find("__");
    if (pos2!=string::npos) {
      hprocess = tmp.substr(0,pos2);
      hsystematic = tmp.substr(pos2+2);
    }
  }
}
