////////////////////////////////////////////////////////////////////////////////
//
// th_transform_x
// -----------------
//
// Tool to transform MVA output as explained in 
// https://hypernews.cern.ch/HyperNews/CMS/get/AUX/2012/10/29/17:28:30-36373-VHbb-HCP-unblind-v6.pdf
// 
// How to optimize the number of bins and stat uncertainty is explained here: 
// https://nmohr.web.cern.ch/nmohr/forVHbb/Talks/BinningTrafo.pdf
// 
//                                    19.02.2014 Christian Boeser
//                                          
////////////////////////////////////////////////////////////////////////////////

#include "../interface/DataSample.h"
#include "../interface/MCSample.h"
#include "../interface/Histogram.h"

#include "../interface/CommandLine.h"
#include "../interface/RootStyle.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <map>
#include <iomanip>
#include <cfloat>

#include <TFile.h>
#include <TSystem.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TEventList.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TArrow.h>
#include <TLine.h>
#include <TAxis.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TPaveText.h>
#include <TText.h>
#include <TLegend.h>
#include <TColor.h>
#include <THStack.h>
#include <TH1F.h>
#include <TF1.h>

#include <TEventList.h>
#include <TLatex.h>
#include <TList.h>
#include <TKey.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
const int LINE_WIDTH = 2;
string get_cut(const vector<string>& cuts,unsigned icut,const string& mode);
void setup_global_style();

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> mcs        = cl.getVector<string>  ("mcs",             "");
  string         mcpath     = cl.getValue <string>  ("mcpath",          "");
  string         dirname    = cl.getValue <string>  ("dirname",      "utm");
  string         filesuffix = cl.getValue <string>  ("filesuffix", ".root");
  string         treename   = cl.getValue <string>  ("treename",       "t");
  vector<string> cuts       = cl.getVector<string>  ("cuts",            "");
  vector<string> mccuts     = cl.getVector<string>  ("mccuts",          "");
  vector<string> mcweights  = cl.getVector<string>  ("mcweights",       "");
  bool           overflow   = cl.getValue <bool>    ("overflow",      true);
  bool           batch      = cl.getValue <bool>    ("batch",        false);
  vector<string> prefix     = cl.getVector<string>  ("prefix",          "");
  vector<string> suffix     = cl.getVector<string>  ("suffix",          "");
  string         bdtout     = cl.getValue <string>  ("bdtout"             );
  vector<string> formats    = cl.getVector<string>  ("formats",         "");
  string         filename   = cl.getValue <string>  ("filename",        "");
  // dummys
  string         datalabel  = cl.getValue <string>  ("datalabel",   "data");
  vector<string> mclabels   = cl.getVector<string>  ("mclabels",        "");
  vector<string> mccolors   = cl.getVector<string>  ("mccolors",        "");
  vector<string> labels     = cl.getVector<string>  ("labels",          "");
  int            bdtbins    = cl.getValue<int>      ("bdtbins",         15);
  double         bdtmin     = cl.getValue<double>   ("bdtmin",        -1.0);
  double         bdtmax     = cl.getValue<double>   ("bdtmax",         1.0);
  double         fitmin     = cl.getValue<double>   ("fitmin",        -2.0);
  double         fitmax     = cl.getValue<double>   ("fitmax",         2.0);
  double         maxstat    = cl.getValue<double>   ("maxstat",       0.25);

  if(!cl.check()) return 0;
  cl.print();

  if (batch&&formats.size()==0) formats.push_back("pdf");

  string varexpr = bdtout;
  unsigned nbinsx = 20000;
  double xmin = fitmin;
  double xmax = fitmax;

  //
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //

  vector<MCSample> mcsamples;
  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0) MCSample::printSampleInfos(mcsamples);

  //
  // CREATE HISTOGRAMS FROM MCSAMPLES IF SO REQUESTED
  //

  if (!mcsamples.empty()) {

    //  histogram specification

    Histogram histogram;
    histogram.setExpr(varexpr);
    histogram.setNbinsx(nbinsx);
    histogram.setXmin(xmin);
    histogram.setXmax(xmax);
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
    // create MC templates for signal and background
    TH1F* hbackground = new TH1F("hbackground","BDT Output",nbinsx,xmin,xmax);
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample mcsample  = mcsamples[imc];
      cout << "read: " << mcsample.name() << endl;
      TH1F* htemplateb=histogram.create("h"+mcsample.name()+"b");
      mcsample.projectNoWeight(htemplateb->GetName(),histogram.expr(),mcsel);
      hbackground->Add(htemplateb);
      delete htemplateb;
    }
    // calculate significance and errors
    float newbdt[bdtbins+1];
    float bdt[bdtbins+1];
    bool foundleft=false;
    bool foundright=false;

    bdt[0]=-1.0; bdt[bdtbins]=1.0;
    for(int i=1; i<bdtbins; i++){
      bdt[i]=bdt[i-1]+(2.0)/bdtbins;
    }
    newbdt[0] = -1.0; newbdt[bdtbins]=1.0;

    // coming from left
    for(unsigned i=0; i<=nbinsx; i++){
      if(i%100==0) cout << "Check bin " << i << endl;
      float integralb = hbackground->Integral(0,i);
      //cout << 1/sqrt(integralb) << " "  << maxstat << " " << integralb << endl; 
      if( 1/sqrt(integralb) < maxstat && integralb!=0){
	cout << "Best bin: " << i << endl;
	foundleft = true;
	newbdt[1]=hbackground->GetXaxis()->GetBinUpEdge(i);
	if(newbdt[1]<bdt[1])
	  newbdt[1]=bdt[1];
	break;
      }
    }

    // coming from right
    for(unsigned i=nbinsx-1; i>=0; i--){
      if(i%100==0) cout << "Check bin " << i << endl;
      float integralb = hbackground->Integral(i,nbinsx-1);
      if( 1/sqrt(integralb) < maxstat && integralb!=0){
	cout << "Best bin: " << i << endl;
	foundright = true;
	newbdt[bdtbins-1]=hbackground->GetXaxis()->GetBinLowEdge(i);
	if(newbdt[bdtbins-1]>bdt[bdtbins-1])
	  newbdt[bdtbins-1]=bdt[bdtbins-1];
	break;
      }
    }
   
    if(!foundright || !foundleft){
      cout << "Error\n";
      return 0;
    }

    // remaining aquidistant bins
    float steps= (newbdt[bdtbins-1] - newbdt[1])/(bdtbins-2);
    for(int i=2; i < bdtbins-1; i++){
      newbdt[i]=newbdt[i-1]+steps;	
    }
    
    ofstream ofile;
    if(filename == "")
      ofile.open ((varexpr+".txt").c_str());
    else
      ofile.open ((filename+".txt").c_str());
    for(int i=0; i<bdtbins+1; i++) ofile << newbdt[i] << " ";
    ofile << bdtmin << " " << bdtmax << endl;
    ofile.close();
       
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
void setup_global_style()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
    
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.2);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);

  gStyle->SetPalette(1);

  gStyle->SetLineWidth(1);
  gStyle->SetFrameLineWidth(1);
}

