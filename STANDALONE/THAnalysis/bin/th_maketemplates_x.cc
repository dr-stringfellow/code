////////////////////////////////////////////////////////////////////////////////
//
// th_maketemplates_x
// ---------------------
//
//            24/12/2010 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
//                       Hauke Held             <held@cern.ch>
////////////////////////////////////////////////////////////////////////////////


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
#include <TCanvas.h>
#include <TLegend.h>
#include <TColor.h>
#include <THStack.h>
#include <TH1F.h>
#include <TEventList.h>
#include <TLatex.h>

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

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void setHBinsNonZero(TH1F* h);
string get_cut(const vector<string>& cuts,unsigned icut,const string& mode);

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  vector<string> data      = cl.getVector<string>  ("data",                "");
  vector<string> mcs       = cl.getVector<string>  ("mcs",                 "");
  string         output    = cl.getValue <string>  ("output",              "");
  double         lumi      = cl.getValue <double>  ("lumi");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  string         opath     = cl.getValue <string>  ("opath",       "");
  string         dirname   = cl.getValue <string>  ("dirname",  "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
  string         treename  = cl.getValue <string>  ("treename",   "t");
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
  bool           batch     = cl.getValue <bool>    ("batch",    false);
  vector<string> formats   = cl.getVector<string>  ("formats",     "");
  vector<string> prefix    = cl.getVector<string>  ("prefix",              "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",              "");
  // dummys
  string         datalabel = cl.getValue <string>  ("datalabel",       "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");
  vector<string> labels    = cl.getVector<string>  ("labels",              "");
  bool           mcuncert  = cl.getValue <bool>    ("mcuncert",         false);
  
  if(!cl.check()) return 0;
  cl.print();
  
  if (batch&&formats.size()==0) formats.push_back("pdf");
 

  //
  // CONSTRUCT OUTPUTFILE NAME
  //

  if (output.empty()) {
    output = (varname.empty()) ? "DataToMC" : varname;
    if (dirname!="utm") output = dirname + "_" + output;
    if (suffix.size()>0) 
      for (int i=(int)suffix.size()-1;i>=0;i--) output = output + "_" + suffix[i];
    if (prefix.size()>0)
      for (int i=(int)prefix.size()-1;i>=0;i--) output = prefix[i] + "_" + output;
    if (!opath.empty()) output = opath + "/" + output;
  }
  string ofilename = output;
  if (ofilename.find(".root")==string::npos) ofilename += ".root";
  
  
  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //
  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app = new TApplication("theta_maketemplates_x",&argc,argv);
  TH1::SetDefaultSumw2();
  set_root_style();
  gStyle->SetOptStat(0);
  TColor::SetPalette(1,0);
  
  vector<TCanvas*> c;
  
  //
  // OPEN DATA FILE / RETRIEVE TREE
  //
  DataSample datasample=DataSample::load(data,dirname,treename,datapath,filesuffix);
  if (!datasample.empty()) DataSample::printInfo(datasample);

  //
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples;
  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0) MCSample::printSampleInfos(mcsamples);


  //
  // CREATE HISTOGRAMS FROM MCSAMPLES IF SO REQUESTED
  //
  if (!mcsamples.empty()||!datasample.empty()) {
    
    // open output file
    
    TFile* ofile = new TFile(ofilename.c_str(),"RECREATE");
    if (!ofile->IsOpen()){cout<<"Failed to create "<<ofilename<<endl;return 0;}
    else cout<<ofilename<<" created."<<endl;
    
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
    
    // create MC templates
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample mcsample  = mcsamples[imc];
      mcsample.setWeights(mcweights);
      TH1F*    htemplate = histogram.create("h"+mcsample.name()); 
      mcsample.project(htemplate->GetName(),histogram.expr(),mcsel);
      htemplate->Scale(lumi);
      setHBinsNonZero(htemplate);
      htemplate->Write();
      cout<<htemplate->GetName()<<": "<<htemplate->Integral()<<endl;
    }

    if (!datasample.empty()) {
      TH1F* hdata(0);
      hdata = histogram.create("hData");
      datasample.project(hdata->GetName(),histogram.expr(),selection);
      hdata->Write();
      cout<<hdata->GetName()<<": "<<hdata->Integral()<<endl;
    }
    
    // write and close output file
    //ofile->Write();
    ofile->Close();
    delete ofile;
    cout<<ofilename<<" written."<<endl;
  }  
  

  //
  // PLOT TEMPLATES FROM OUTPUT FILE
  //
  TFile* ofile = new TFile(ofilename.c_str(),"READ");
  if (!ofile->IsOpen()) { cout<<"Failed to open "<<ofilename<<endl;return 0;}
  
  vector<TH1F*> templates;
  TIter next(ofile->GetListOfKeys());
  TKey* key(0);
  while ((key=(TKey*)next())) {
    if ((strcmp(key->GetClassName(),"TH1F")!=0)) continue;
    templates.push_back((TH1F*)key->ReadObj());
    cout<<templates.back()->GetName()<<": "<<templates.back()->Integral()<<endl;
  }
  
  c.push_back(new TCanvas("ThetaTemplates","ThetaTemplates",400,0,800,800));
  UInt_t nx = (UInt_t) std::sqrt(templates.size());
  UInt_t ny = nx;
  if (nx*ny<templates.size())nx++;
  if (nx*ny<templates.size())ny++;
  c.back()->Divide(nx,ny);
  for (unsigned itemplate=0;itemplate<templates.size();itemplate++) {
    c.back()->cd(itemplate+1);
    templates[itemplate]->SetMinimum(0.0);
    templates[itemplate]->SetMaximum(1.33*templates[itemplate]->GetMaximum());
    templates[itemplate]->SetLineWidth(2);
    templates[itemplate]->SetFillStyle(1001);
    templates[itemplate]->SetFillColor(kYellow);
    templates[itemplate]->Draw("EHIST");
    TLatex tex;
    tex.SetNDC();
    stringstream sstex;
    sstex<<templates[itemplate]->GetName()<<": "<<setprecision(4)
	 <<templates[itemplate]->Integral();
    tex.DrawLatex(0.25,0.85,sstex.str().c_str());
  }
  
  
  //
  // SAVE PLOT TO FILE(S)
  //
  for (unsigned int ifmt=0;ifmt<formats.size();ifmt++) {
    for (unsigned int ic=0;ic<c.size();ic++) {
      c[ic]->Print((ofilename.substr(0,ofilename.find(".root"))
		    +"."+formats[ifmt]).c_str());      
    }
  }

  
  
  //
  // RUN ROOT APPLICATION
  //
  if (!batch) app->Run();

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

