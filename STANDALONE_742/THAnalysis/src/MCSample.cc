////////////////////////////////////////////////////////////////////////////////
//
// MCSample
// --------
//
//            01/09/2010 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/MCSample.h"


#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TEventList.h>
#include <TObjString.h>
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
MCSample::MCSample()
{

}


//______________________________________________________________________________
MCSample::MCSample(const string&name)
  : name_(name)
{

}


//______________________________________________________________________________
MCSample::~MCSample()
{
  trees_.clear();
}


////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void MCSample::addTree(const std::string& filename,
		       TTree* tree,double xsec,double eff,double kfac)
{
  filenames_.push_back(filename);
  trees_.push_back(tree);
  xsecs_.push_back(xsec);
  efficiencies_.push_back(eff);
  kfactors_.push_back(kfac);
  
  double xsec_from_tree;
  double kfac_from_tree;
  double eff_from_tree;
  
  if (getXsection(tree,xsec_from_tree)) {
    if (xsec!=1.0)
      cout<<filename<<": replace xsec "<<xsec<<" with "<<xsec_from_tree<<endl;
    xsecs_.pop_back();
    xsecs_.push_back(xsec_from_tree);
  }
  if (getKfactor(tree,kfac_from_tree)) {
    if (kfac!=1.0)
      cout<<filename<<": replace kfac "<<kfac<<" with "<<kfac_from_tree<<endl;
    kfactors_.pop_back();
    kfactors_.push_back(kfac_from_tree);
  }
  if (getEfficiency(tree,eff_from_tree)) {
    if (eff!=1.0)
      cout<<filename<<": replace eff "<<eff<<" with "<<eff_from_tree<<endl;
    efficiencies_.pop_back();
    efficiencies_.push_back(eff_from_tree);
  }
}


//______________________________________________________________________________
void MCSample::project(const std::string& hname,
		       const std::string& expr,
		       const std::string& presel)
{
  TH1* result = (TH1*)gROOT->FindObject(hname.c_str());
  if (0==result) {
    cout<<"MCSample::project() ERROR: histogram "<<hname<<" not found!"<<endl;
    return;
  }
  TH1* hclone = (TH1*)result->Clone("hClone");

  vector<string>::const_iterator it;
  for (unsigned itree=0;itree<ntrees();itree++) {
    TTree* tree = trees_[itree];
    double xsec   = xsecs_[itree];
    double eff    = efficiencies_[itree];
    double kfac   = kfactors_[itree];
    double weight = xsec*kfac*eff/(double)tree->GetEntries();
    stringstream sssel;  sssel<<weight;
    for (it=weights_.begin();it!=weights_.end();++it) sssel<<"*("<<*it <<")";
    if (!presel.empty()) sssel<<"*("+presel+")";
    TH1* htmp = (TH1*)hclone->Clone("hTmp");
    tree->Project(htmp->GetName(),expr.c_str(),sssel.str().c_str());
    result->Add(htmp);
    delete htmp;
    // delete tree;
  }
  delete hclone;
  
  return;
}

//______________________________________________________________________________
void MCSample::projectSys(const std::string& hname, const std::string& hnameup,const std::string& hnamedown,
			  const std::string& expr,
			  const std::string& presel)
{
  TH1* result = (TH1*)gROOT->FindObject(hname.c_str());
  TH1* resultup = (TH1*)gROOT->FindObject(hnameup.c_str());
  TH1* resultdown = (TH1*)gROOT->FindObject(hnamedown.c_str());

  if (0==result) {
    cout<<"MCSample::project() ERROR: histogram "<<hname<<" not found!"<<endl;
    return;
  }
  if (0==resultup) {
    cout<<"MCSample::project() ERROR: histogram "<<hnameup<<" not found!"<<endl;
    return;
  }
  if (0==resultdown) {
    cout<<"MCSample::project() ERROR: histogram "<<hnamedown<<" not found!"<<endl;
    return;
  }

  TH1* hclone     = (TH1*)result->Clone("hClone");
  TH1* hcloneup   = (TH1*)resultup->Clone("hCloneup");
  TH1* hclonedown = (TH1*)resultdown->Clone("hClonedown");

  vector<string>::const_iterator it;
  for (unsigned itree=0;itree<ntrees();itree++) {
    TTree* tree = trees_[itree];
    double xsec   = xsecs_[itree];
    double eff    = efficiencies_[itree];
    double kfac   = kfactors_[itree];
    double weight = xsec*kfac*eff/(double)tree->GetEntries();
    stringstream sssel;  sssel<<weight;
    stringstream sssel_up;  sssel_up<<weight;
    stringstream sssel_down;  sssel_down<<weight;

    for (it=weights_.begin();it!=weights_.end();++it){
      sssel<<"*("<<*it <<")";
      sssel_up<<"*("<<*it <<")";
      sssel_down<<"*("<<*it <<")";
    }
    for (it=sfup_.begin();it!=sfup_.end();++it) sssel_up<<"*("<<*it <<")";
    for (it=sfdown_.begin();it!=sfdown_.end();++it) sssel_down<<"*("<<*it <<")";
    if (!presel.empty()){
      sssel<<"*("+presel+")";
      sssel_up<<"*("+presel+")";
      sssel_down<<"*("+presel+")";
    }
    TH1* htmp     = (TH1*)hclone->Clone("hTmp");
    TH1* htmpup   = (TH1*)hcloneup->Clone("hTmpup");
    TH1* htmpdown = (TH1*)hclonedown->Clone("hTmpdown");
	    
    tree->Project(htmp->GetName(),expr.c_str(),sssel.str().c_str());
    tree->Project(htmpup->GetName(),expr.c_str(),sssel_up.str().c_str());
    tree->Project(htmpdown->GetName(),expr.c_str(),sssel_down.str().c_str());
    result->Add(htmp);
    resultup->Add(htmpup);
    resultdown->Add(htmpdown);
    delete htmp;
    delete htmpup;
    delete htmpdown;
    delete tree;
  }
  delete hclone;
  delete hcloneup;
  delete hclonedown;
  
  return;
}

//______________________________________________________________________________
void MCSample::project2d(const std::string& hname,
		       const std::string& expr,
		       const std::string& presel)
{
  TH2* result = (TH2*)gROOT->FindObject(hname.c_str());
  if (0==result) {
    cout<<"MCSample::project() ERROR: histogram "<<hname<<" not found!"<<endl;
    return;
  }
  TH2* hclone = (TH2*)result->Clone("hClone");

  vector<string>::const_iterator it;
  
  for (unsigned itree=0;itree<ntrees();itree++) {
    TTree* tree   = trees_[itree];
    double xsec   = xsecs_[itree];
    double eff    = efficiencies_[itree];
    double kfac   = kfactors_[itree];
    double weight = xsec*kfac*eff/(double)tree->GetEntries();
    stringstream sssel;  sssel<<weight;
    for (it=weights_.begin();it!=weights_.end();++it) sssel<<"*"<<*it;
    if (!presel.empty()) sssel<<"*("+presel+")";
    TH2* htmp = (TH2*)hclone->Clone("hTmp");
    tree->Project(htmp->GetName(),expr.c_str(),sssel.str().c_str());
    result->Add(htmp);
    delete htmp;
    delete tree;
  }
  delete hclone;
  
  return;
}


//______________________________________________________________________________
void MCSample::projectNoWeight(const std::string& hname,
		       const std::string& expr,
		       const std::string& presel)
{
  TH1* result = (TH1*)gROOT->FindObject(hname.c_str());
  if (0==result) {
    cout<<"MCSample::project() ERROR: histogram "<<hname<<" not found!"<<endl;
    return;
  }
  TH1* hclone = (TH1*)result->Clone("hClone");

  vector<string>::const_iterator it;
  
  for (unsigned itree=0;itree<ntrees();itree++) {
    TTree* tree   = trees_[itree];
    stringstream sssel;
    if (!presel.empty()) sssel<<"("+presel+")";
    TH1* htmp = (TH1*)hclone->Clone("hTmp");
    tree->Project(htmp->GetName(),expr.c_str(),sssel.str().c_str());
    result->Add(htmp);
    delete htmp;
    delete tree;
  }
  delete hclone;

  return;
}


//______________________________________________________________________________
void MCSample::setWeights(const vector<string>& weights)
{
  for (vector<string>::const_iterator it=weights.begin();it!=weights.end();++it) {
    string weight(*it);
    size_t pos = weight.find(":");
    if (pos==string::npos) weights_.push_back(weight);
    else if (weight.substr(0,pos)==name()) weights_.push_back(weight.substr(pos+1));
  }
  return;
}

//______________________________________________________________________________
void MCSample::setSFup(const vector<string>& sfup)
{
  for (vector<string>::const_iterator it=sfup.begin();it!=sfup.end();++it) {
    string weight(*it);
    size_t pos = weight.find(":");
    if (pos==string::npos) sfup_.push_back(weight);
    else if (weight.substr(0,pos)==name()) sfup_.push_back(weight.substr(pos+1));
  }
  return;
}

//______________________________________________________________________________
void MCSample::setSFdown(const vector<string>& sfdown)
{
  for (vector<string>::const_iterator it=sfdown.begin();it!=sfdown.end();++it) {
    string weight(*it);
    size_t pos = weight.find(":");
    if (pos==string::npos) sfdown_.push_back(weight);
    else if (weight.substr(0,pos)==name()) sfdown_.push_back(weight.substr(pos+1));
  }
  return;
}


//______________________________________________________________________________
bool MCSample::getXsection(TTree* t,double& xsection)
{
  TIter next(t->GetUserInfo());
  while (TObject* obj = next()) {
    TObjString* objstr = dynamic_cast<TObjString*>(obj); if (0==obj) continue;
    string str = objstr->String().Data();
    size_t pos = str.find(':'); assert(pos!=string::npos);
    string key = str.substr(0,pos);
    if (key=="xsection") {
      stringstream ssxsec;
      ssxsec<<str.substr(pos+1);
      ssxsec>>xsection;
      return true;
    }
  }
  xsection = 1.0;
  return false;
}


//______________________________________________________________________________
bool MCSample::getKfactor(TTree* t,double& kfactor)
{
  TIter next(t->GetUserInfo());
  while (TObject* obj = next()) {
    TObjString* objstr = dynamic_cast<TObjString*>(obj); if (0==obj) continue;
    string str = objstr->String().Data();
    size_t pos = str.find(':'); assert(pos!=string::npos);
    string key = str.substr(0,pos);
    if (key=="kfactor") {
      stringstream sskfac;
      sskfac<<str.substr(pos+1);
      sskfac>>kfactor;
      return true;
    }
  }
  kfactor = 1.0;
  return false;
}


//______________________________________________________________________________
bool MCSample::getEfficiency(TTree* t,double& efficiency)
{
  efficiency = 1.0;
  unsigned neff(0);
  TIter next(t->GetUserInfo());
  while (TObject* obj = next()) {
    TObjString* objstr = dynamic_cast<TObjString*>(obj); if (0==obj) continue;
    string str = objstr->String().Data();
    size_t pos = str.find(':'); assert(pos!=string::npos);
    string key = str.substr(0,pos);
    if (key=="efficiency") {
      stringstream sseff;
      sseff<<str.substr(pos+1);
      double eff_i;
      sseff>>eff_i;
      efficiency *= eff_i;
      neff++;
    }
  }
  return (neff>0) ? true : false;
}
  

//______________________________________________________________________________
bool MCSample::getWeight(TTree* t,double& weight)
{
  bool result(false);
  double xsection;   if (getXsection(t,xsection)) result = true;
  double efficiency; getEfficiency(t,efficiency);
  double kfactor;    getKfactor(t,kfactor);
  weight = xsection*kfactor*efficiency/(double)t->GetEntries();
  return result;
}


//______________________________________________________________________________
double MCSample::nevents(const std::string& selection,double lumi) const
{
  return nentries("1",selection,lumi);
}


//______________________________________________________________________________
double MCSample::nentries(const std::string& expr,
			  const std::string& selection,
			  double lumi) const
{
  double result(0.0);
  // if lumi - use weights
  if (lumi>0.0) {
    for (unsigned i=0;i<ntrees();i++) {
      TTree* t=tree(i);
      double weight; getWeight(t,weight);
      stringstream sssel; sssel<<weight;      
      vector<string>::const_iterator it;
      for (it=weights_.begin();it!=weights_.end();++it) sssel<<"*"<<*it;
      if (!selection.empty()) sssel<<"*("<<selection<<")";
      t->Project("htmp_MCSample(100)",expr.c_str(),sssel.str().c_str());
      TH1* h=(TH1*)gROOT->FindObject("htmp_MCSample");
      double nevts=h->Integral();
      delete h;
      nevts  *= lumi;
      result += nevts;
    }
    //    return result;
  }
  // if not lumi - use full MC statistics
  else {
    for (unsigned i=0;i<ntrees();i++) {
      TTree* t=tree(i);
      // however reweight according to internal e.g. PU weights
      stringstream sssel;sssel<<"1";
      vector<string>::const_iterator it;
      for (it=weights_.begin();it!=weights_.end();++it) sssel<<"*"<<*it;
      if (!selection.empty()) sssel<<"*("<<selection<<")";
      t->Project("htmp_MCSample(100)",expr.c_str(),sssel.str().c_str());
      TH1* h=(TH1*)gROOT->FindObject("htmp_MCSample");
      double nevts=h->Integral();
      delete h;
      result += nevts;
    }
  }
  return result;
}


//______________________________________________________________________________
string MCSample::printline(unsigned length,const char& c)
{
  stringstream result;
  for (unsigned i=0;i<length;i++) result<<c;
  return result.str();
}


//______________________________________________________________________________
vector<MCSample> MCSample::loadSamples(const vector<string>& mcs,
				       const string& dirname,         
				       const string& treename,
				       const string& mcpath,
				       const string& filesuffix)
{
  vector<MCSample> result;
  
  vector<string>       mcsample_names;
  map<string,MCSample> mcsample_map;

  for (unsigned imc=0;imc<mcs.size();imc++) {
    string tmp(mcs[imc]);

    string mcsamplename;
    string mcfilename;
    double mcxsec;
    double mceff;
    double mckfac;
    decodeDescriptor(mcs[imc],mcsamplename,mcfilename,mcxsec,mceff,mckfac);
    if (!mcpath.empty()) mcfilename=mcpath+"/"+mcfilename;
    if (mcfilename.find(filesuffix)==string::npos) mcfilename+=filesuffix;
    TFile* fmc=new TFile(mcfilename.c_str(),"READ");
    if (!fmc->IsOpen()) {
      stringstream sserr;
      sserr<<"MCSample::loadSamples() ERROR: "
	   <<"failed to open file "<<mcfilename<<endl;
      throw std::runtime_error(sserr.str());
    }
    TDirectory* dmc=(TDirectory*)fmc->Get(dirname.c_str());
    if (0==dmc) {
      stringstream sserr;
      sserr<<"MCSample::loadSamples() ERROR: "
	   <<"can't find directory "<<dirname<<endl;
      throw std::runtime_error(sserr.str());
    }
    TTree* tmc=(TTree*)dmc->Get(treename.c_str());
    if (0==tmc) {
      stringstream sserr;
      sserr<<"MCSample::loadSamples() ERROR: "
	   <<"can't find tree "<<treename<<endl;
    }
    map<string,MCSample>::iterator it = mcsample_map.find(mcsamplename);
    if (it==mcsample_map.end()) {
      mcsample_names.push_back(mcsamplename);
      MCSample mcsample(mcsamplename);
      mcsample_map[mcsamplename]=mcsample;
    }
    mcsample_map[mcsamplename].addTree(mcfilename,tmc,mcxsec,mceff,mckfac);
  }
 
  
  vector<string>::const_iterator it=mcsample_names.begin();
  for (;it!=mcsample_names.end();++it) result.push_back(mcsample_map[*it]);
  
  return result;
}


//______________________________________________________________________________
void MCSample::decodeDescriptor(const string& descriptor,
				string& name,string& filename,
				double& xsec,double& eff,double& kfac)
{
  string tmp(descriptor);
  
  size_t pos = tmp.find(':');
  name=tmp.substr(0,pos); filename=name+".root"; xsec=1.0; eff=1.0; kfac=1.0;
  if (pos==string::npos) return;
  
  tmp = tmp.substr(pos+1);
  pos = tmp.find(':');
  filename = tmp.substr(0,pos);
  if (filename.find(".root")==string::npos) filename += ".root";
  if (pos==string::npos) return;
  
  tmp = tmp.substr(pos+1);
  pos = tmp.find(':');
  stringstream ssxsec;
  ssxsec<<tmp.substr(0,pos);
  ssxsec>>xsec;
  if (pos==string::npos) return;
  
  tmp = tmp.substr(pos+1);
  pos = tmp.find(':');
  stringstream sseff;
  sseff<<tmp;
  sseff>>eff;
  if (pos==string::npos) return;
  
  tmp = tmp.substr(pos+1);
  stringstream sskfac;
  sskfac<<tmp;
  sskfac>>kfac;
  
  return;
}


//______________________________________________________________________________
unsigned MCSample::printSampleInfos(const vector<MCSample>& mcsamples)
{
  
  unsigned wfilename(0);
  for (unsigned i=0;i<mcsamples.size();i++) {
    for (unsigned j=0;j<mcsamples[i].ntrees();j++) {
      unsigned wtmp = (unsigned)mcsamples[i].filename(j).length();
      if (wtmp>wfilename) wfilename = wtmp;
    }
  }
  wfilename = std::max((unsigned)10,wfilename);
  unsigned wtot = wfilename+15+10+10+15+6+12;

  cout<<endl<<printline(wtot)<<endl
      <<setw(15)       <<left <<"MC SAMPLE"
      <<setw(wfilename)<<left <<"FILENAME"
      <<setw(10)       <<right<<"EVENTS"
      <<setw(10)       <<right<<"XSEC[PB]"
      <<setw(15)       <<right<<"EFF"
      <<setw(6)        <<right<<"KFAC"
      <<setw(12)       <<right<<"INTLUMI"
      <<endl<<printline(wtot)<<endl;
  
  for (unsigned i=0;i<mcsamples.size();i++)
    for (unsigned j=0;j<mcsamples[i].ntrees();j++)
      cout<<setw(15)       <<left <<mcsamples[i].name()
	  <<setw(wfilename)<<left <<mcsamples[i].filename(j)
	  <<setw(10)       <<right<<mcsamples[i].tree(j)->GetEntries()
	  <<setw(10)       <<right<<mcsamples[i].xsec(j)
	  <<setw(15)       <<right<<mcsamples[i].efficiency(j)
	  <<setw(6)        <<right<<mcsamples[i].kfactor(j)
	  <<setw(12)       <<right<<mcsamples[i].tree(j)->GetEntries()/
	                            mcsamples[i].xsec(j)/
	                            mcsamples[i].kfactor(j)/
	                            mcsamples[i].efficiency(j)
	  <<endl;
  cout<<printline(wtot)<<endl<<endl;
  
  return wtot;
}
