////////////////////////////////////////////////////////////////////////////////
//
// DataSample
// ----------
//
//            01/09/2010 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/DataSample.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TEventList.h>
#include <TH1F.h>
#include <TH2F.h>

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>


using namespace std;


////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTION / DESTRUCTION
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
DataSample::DataSample()
{

}


//______________________________________________________________________________
DataSample::~DataSample()
{

}


////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void DataSample::addTree(const string& filename,TTree* t)
{
  filenames_.push_back(filename);
  trees_.push_back(t);
  selections_.push_back("1");
  prescales_.push_back(1.0);
}



//______________________________________________________________________________
void DataSample::addTree(const string& filename,TTree* t,
			 const string& selection)
{
  filenames_.push_back(filename);
  trees_.push_back(t);
  selections_.push_back(selection);
  prescales_.push_back(1.0);
}


//______________________________________________________________________________
void DataSample::addTree(const string& filename,TTree* t,
			 const string& selection,float prescale)
{
  filenames_.push_back(filename);
  trees_.push_back(t);
  selections_.push_back(selection);
  prescales_.push_back(prescale);
}

  
//______________________________________________________________________________
double DataSample::nevents(const std::string& selection) const
{
  double result(0.0);
  for (unsigned i=0;i<ntrees();i++) {
    TTree* tree_i     = this->tree(i);
    string sel_i      = this->selection(i);
    float  prescale_i = this->prescale(i);
    if (!selection.empty()) sel_i += "&&"+selection;
    TEventList* el = new TEventList("elist","elist");
    tree_i->Draw(">>elist",sel_i.c_str());
    double result_i = el->GetN();
    delete el;
    result_i*=prescale_i;
    result += result_i;
  }
  return result;
}


//______________________________________________________________________________
void DataSample::project(const string& hname,
			 const string& expr,
			 const string& presel)
{
  TH1F* result= (TH1F*)gROOT->FindObject(hname.c_str());
  if (0==result) {
    cout<<"DataSample::project() ERROR: histogram "<<hname<<" not found!"<<endl;
    return;
    }

  TH1F* hclone = (TH1F*)result->Clone("hClone");

  for (unsigned i=0;i<trees_.size();i++) {
    string selection(selections_[i]);
    if (!presel.empty()) selection += "&&" + presel;
    stringstream sssel; sssel<<prescales_[i]<<"*("<<selection<<")";
    TH1F* htmp = (TH1F*)hclone->Clone("hTmp");
    trees_[i]->Project(htmp->GetName(), expr.c_str(),sssel.str().c_str());
    result->Add(htmp);
    delete htmp;
  }
  delete hclone;
  return;
}

//______________________________________________________________________________
void DataSample::project2d(const string& hname,
			 const string& expr,
			 const string& presel)
{
  TH2F* result = (TH2F*)gROOT->FindObject(hname.c_str());
  if (0==result) {
    cout<<"DataSample::project() ERROR: histogram "<<hname<<" not found!"<<endl;
    return;
  }
  
  TH2F* hclone = (TH2F*)result->Clone("hClone");

  for (unsigned i=0;i<trees_.size();i++) {
    string selection(selections_[i]);
    if (!presel.empty()) selection += "&&" + presel;
    stringstream sssel; sssel<<prescales_[i]<<"*("<<selection<<")";
    TH2F* htmp = (TH2F*)hclone->Clone("hTmp");
    trees_[i]->Project(htmp->GetName(),expr.c_str(),sssel.str().c_str());
    result->Add(htmp);
    delete htmp;
  }
  delete hclone;
  return;
}

//______________________________________________________________________________
void DataSample::projectMM(const string& hname,
			   const string& expr,
			   const string& loosesel,
			   const string& tightsel,
			   string        eps_sig,
			   string        eps_qcd)
{
  string wloose = "("+eps_qcd+")*("+eps_sig+")/(("+eps_sig+")-("+eps_qcd+"))";
  string wtight = "("+eps_qcd+")*(("+eps_sig+")-1.0)/(("+eps_sig+")-("+eps_qcd+"))";
  
  //cout<<"wloose: "<<wloose<<endl;
  //cout<<"wtight: "<<wtight<<endl;

  for (unsigned i=0;i<trees_.size();i++) {
    stringstream ssloose,sstight;
    ssloose<<prescales_[i]<<"*"<<wloose<<"*("<<selections_[i]<<"&&"<<loosesel<<")";
    sstight<<prescales_[i]<<"*"<<wtight<<"*("<<selections_[i]<<"&&"<<tightsel<<")";
    trees_[i]->Project(("+"+hname).c_str(),expr.c_str(),ssloose.str().c_str());
    trees_[i]->Project(("+"+hname).c_str(),expr.c_str(),sstight.str().c_str());
  }
}


//______________________________________________________________________________
DataSample DataSample::load(const vector<string>& data,
			    const string&         dirname,
			    const string&         treename,
			    const string&         datapath,
			    const string&         filesuffix)
{
  DataSample result;
  
  for (unsigned i=0;i<data.size();i++) {
    string filename;
    string selection;
    float  prescale;
    decodeDescriptor(data[i],filename,selection,prescale);
    if (!datapath.empty()) filename = datapath+"/"+filename;
    if (filename.find(filesuffix)==string::npos) filename += filesuffix;
    TFile* file = new TFile(filename.c_str(),"READ");
    if (!file->IsOpen()) {
      stringstream sserr;
      sserr<<"DataSample::load() ERROR: "<<"failed to open file "<<filename<<endl;
      throw std::runtime_error(sserr.str());
    }
    TDirectory* dir = (TDirectory*)file->Get(dirname.c_str());
    if (0==dir) {
      stringstream sserr;
      sserr<<"DataSample::load() ERROR: "<<"can't find directory "<<dirname<<endl;
      throw std::runtime_error(sserr.str());
    }
    TTree* tree = (TTree*)dir->Get(treename.c_str());
    if (0==tree) {
      stringstream sserr;
      sserr<<"MCSample::load() ERROR: "<<"can't find tree "<<treename<<endl;
    }
    result.addTree(filename,tree,selection,prescale);
  }
  return result;
}

//______________________________________________________________________________
DataSample DataSample::loadsingle(const string& data,
			    const string&         dirname,
			    const string&         treename,
			    const string&         datapath,
			    const string&         filesuffix)
{
  DataSample result;
  
  string filename;
  string selection;
  float  prescale;
  decodeDescriptor(data,filename,selection,prescale);
  if (!datapath.empty()) filename = datapath+"/"+filename;
  if (filename.find(filesuffix)==string::npos) filename += filesuffix;
  TFile* file = new TFile(filename.c_str(),"READ");
  if (!file->IsOpen()) {
    stringstream sserr;
    sserr<<"DataSample::load() ERROR: "<<"failed to open file "<<filename<<endl;
    throw std::runtime_error(sserr.str());
  }
  TDirectory* dir = (TDirectory*)file->Get(dirname.c_str());
  if (0==dir) {
    stringstream sserr;
    sserr<<"DataSample::load() ERROR: "<<"can't find directory "<<dirname<<endl;
    throw std::runtime_error(sserr.str());
  }
  TTree* tree = (TTree*)dir->Get(treename.c_str());
  if (0==tree) {
    stringstream sserr;
    sserr<<"MCSample::load() ERROR: "<<"can't find tree "<<treename<<endl;
  }
  result.addTree(filename,tree,selection,prescale);

  return result;
}


//______________________________________________________________________________
void DataSample::decodeDescriptor(const string& descriptor,
				  string&       filename,
				  string&       selection,
				  float&        prescale)
{
  string tmp(descriptor);

  size_t pos = tmp.find(':');
  filename = tmp.substr(0,pos);
  selection = "1";
  prescale  = 1.0;

  if (pos==string::npos) return;

  tmp = tmp.substr(pos+1);
  pos = tmp.find(':');
  selection = tmp.substr(0,pos);

  if (pos==string::npos) return;

  tmp = tmp.substr(pos+1);
  pos = tmp.find(':');
  stringstream ssps;
  ssps<<tmp.substr(0,pos);
  ssps>>prescale;
  assert(pos==string::npos);

  return;
}


//______________________________________________________________________________
void DataSample::printInfo(const DataSample& datasample)
{
  unsigned wfilename(0);
  for (unsigned i=0;i<datasample.ntrees();i++) {
    unsigned wtmp = datasample.filename(i).length();
    if (wtmp>wfilename) wfilename = wtmp;
  }
  wfilename = std::max((unsigned)10,wfilename);
  
  unsigned wselection(0);
  for (unsigned i=0;i<datasample.ntrees();i++) {
    unsigned wtmp = datasample.selection(i).length();
    if (wtmp>wselection) wselection = wtmp;
  }
  wselection = std::max((unsigned)15,wselection);
  
  unsigned wtot = wfilename+15+wselection+10;
  
  cout<<endl<<printline(wtot)<<endl
      <<setw(wfilename) <<left <<"FILENAME"
      <<setw(15)        <<right<<"EVENTS"
      <<setw(wselection)<<right<<"SELECTION"
      <<setw(10)        <<right<<"PRESCALE"
      <<endl<<printline(wtot)<<endl;
  
  for (unsigned i=0;i<datasample.ntrees();i++)
    cout<<setw(wfilename) <<left <<datasample.filename(i)
	<<setw(15)        <<right<<datasample.tree(i)->GetEntries()
	<<setw(wselection)<<right <<datasample.selection(i)
	<<setw(10)        <<right<<datasample.prescale(i)
	<<endl;
  cout<<printline(wtot)<<endl;
  
  return;
}


//______________________________________________________________________________
string DataSample::printline(unsigned length,const char& c)
{
  stringstream result;
  for (unsigned i=0;i<length;i++) result<<c;
  return result.str();
}
