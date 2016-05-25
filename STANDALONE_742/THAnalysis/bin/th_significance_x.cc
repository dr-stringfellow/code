////////////////////////////////////////////////////////////////////////////////
//
// th_significance_x
// -----------------
//
// tool to check for best signal/background ratio after NN application
//
// 
//                                    19/01/2012 Hauke Held       <held@cern.ch>
//                                               Benedikt Maier <bmaier@cern.ch>
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

//#include "NeuroBayesTeacher.hh"

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
  
  
  vector<string> signals   = cl.getVector<string>  ("signals",         "");
  vector<string> mcs       = cl.getVector<string>  ("mcs",             "");
  double         lumi      = cl.getValue <double>  ("lumi"               );
  string         datapath  = cl.getValue <string>  ("datapath",        "");
  string         mcpath    = cl.getValue <string>  ("mcpath",          "");
  string         dirname   = cl.getValue <string>  ("dirname",      "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix", ".root");
  string         treename  = cl.getValue <string>  ("treename",       "t");
  vector<string> cuts      = cl.getVector<string>  ("cuts",            "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",          "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",       "");
  bool           overflow  = cl.getValue <bool>    ("overflow",     false);
  bool           batch     = cl.getValue <bool>    ("batch",        false);
  vector<string> prefix    = cl.getVector<string>  ("prefix",          "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",          "");
  string         nnout     = cl.getValue <string>  ("nnout",           "");
  vector<string> formats   = cl.getVector<string>  ("formats",         "");
  // dummys
  string         datalabel = cl.getValue <string>  ("datalabel",   "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",        "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",        "");
  vector<string> labels    = cl.getVector<string>  ("labels",          "");

  if(!cl.check()) return 0;
  cl.print();
  
  if (batch&&formats.size()==0) formats.push_back("pdf");

  string varexpr = nnout;
  unsigned nbinsx = 10000;
  double xmin = -1;
  double xmax = +1;
  

  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //

  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app = new TApplication("theta_maketemplates_x",&argc,argv);
  TH1::SetDefaultSumw2();
  set_root_style();
  gStyle->SetOptStat(0);
  TColor::SetPalette(1,0);
  
  
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

    TH1F* hsignal = new TH1F("hsignal","NN Network Output",10000,-1,1);
    TH1F* hbackground = new TH1F("hbackground","NN Network Output",10000,-1,1);
    
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample mcsample  = mcsamples[imc];
      mcsample.setWeights(mcweights);

      bool isSignal = false;
	
      for (unsigned i=0;i<signals.size();i++)
	 if (signals[i]==mcsample.name()) isSignal = true;
	
	if (isSignal) {
	  TH1F* htemplates=histogram.create("h"+mcsample.name()+"s");
	  mcsample.project(htemplates->GetName(),histogram.expr(),mcsel);
	  htemplates->Scale(lumi);
	  hsignal->Add(htemplates);  
	}
	else {
	  TH1F* htemplateb=histogram.create("h"+mcsample.name()+"b");
	  mcsample.project(htemplateb->GetName(),histogram.expr(),mcsel);
	  htemplateb->Scale(lumi);
	  hbackground->Add(htemplateb);
	}
    }	 	
    
    
    
    // calculate significance and errors

    float significance;
    float x = -1.;
    float bestsig = 0;
    float xbest = -1;
    float bestsigevts = 0;
    float bestbkgevts = 0;

    TGraphErrors* grerr = new TGraphErrors(0);

    while (x<1.0) {
      float integrals = 
	hsignal->Integral(hsignal->FindBin(x),hsignal->FindBin(1.0));
    
      float integralb = 
	hbackground->Integral(hbackground->FindBin(x),hbackground->FindBin(1.0));
    
      significance = integrals/(1.5+sqrt(integralb));
      //if(integralb!=0)
      //significance = integrals/(sqrt(integralb));
	//significance = integrals/integralb;
      //      else
      //significance = 0;

      int   n = grerr->GetN();
      float y = significance;   

      float error = sqrt((integrals/(1.5+pow(sqrt(integralb),2)))+(pow(integrals,2)/(4*pow(1.5+sqrt(integralb),4))));

      grerr->SetPoint(n,x,y);
      //grerr->SetPointError(n,0,error);
            
      if(y>bestsig) {
	bestsig = y;
	xbest = x;
	bestsigevts = integrals;
	bestbkgevts = integralb;
	
      }

      x += 0.01;
      
    }
    
    // draw significance

  

    TCanvas* c1 = new TCanvas("c1","Significance",300,0,800,600);
    setup_global_style();

    c1->cd();


    grerr->SetTitle("");
    grerr->GetXaxis()->SetLimits(-1,1);
    grerr->SetMaximum(1.5*bestsig);
    grerr->SetMinimum(0.0);
    grerr->SetMarkerSize(1);
    grerr->SetMarkerStyle(21);
    grerr->SetMarkerColor(kAzure-8);
    grerr->SetLineWidth(2);
    //grerr->SetLineColor(kAzure+7);
    grerr->SetLineColor(kRed-5);

    grerr->Draw("AL");
    grerr->GetXaxis()->SetTitle("MVA Output Cut");
    grerr->GetYaxis()->SetTitle("significance");



    TLine* linex = new TLine(xbest,0,xbest,bestsig);
    
    linex->SetLineWidth(2);
    linex->SetLineStyle(3);
    linex->Draw();

    TLine* liney = new TLine(-1,bestsig,xbest,bestsig);

    liney->SetLineWidth(2);
    liney->SetLineStyle(3);
    liney->Draw();

    float t = grerr->GetYaxis()->GetXmax();

    TPaveText* text = new TPaveText(-0.1,0.15*t,-0.3,0.45*t);
    text->SetTextSize(0.03);
    text->SetFillColor(10);
    text->SetLineColor(10);
    text->SetBorderSize(0);
    char line[2555];
    sprintf(line,"sign. = #frac{S}{1.5+#sqrt{B}}");
    //sprintf(line,"sign. = #frac{S}{#sqrt{B}}");
    //sprintf(line,"sign. = #frac{S}{B}");
    text->AddText(line);
    sprintf(line,"  ");
    text->AddText(line);
    sprintf(line,"  ");
    text->AddText(line);
    sprintf(line,"  ");
    text->AddText(line);
    sprintf(line,"#bf{#it{best significance: }} %3.4f",bestsig);
    text->AddText(line);
    sprintf(line,"  ");
    text->AddText(line);
    sprintf(line,"#bf{#it{best MVA cut: }} %3.2f",xbest);
    text->AddText(line);
    sprintf(line,"  ");
    text->AddText(line);
    sprintf(line,"  ");
    text->AddText(line);
    
    sprintf(line,"#bf{#it{signal events: }} %3.2f",bestsigevts);
    text->AddText(line);
    sprintf(line,"  ");
    text->AddText(line);
    sprintf(line,"#bf{#it{background events: }} %3.2f",bestbkgevts);
    text->AddText(line);
    text->Draw();    

    TLatex tex;
    tex.SetTextSize(0.04);
    tex.SetTextFont(42);
    tex.SetNDC(true);

    tex.DrawLatex(0.20,0.96,"#sqrt{s}=8 TeV");
    tex.DrawLatex(0.36,0.96,"L=19.3 fb^{-1}");
    //tex.DrawLatex(0.360,0.96,"CMS Preliminary");
    tex.DrawLatex(0.20,0.89,"t(bl#nu)H(b#bar{b})");
    tex.DrawLatex(0.20,0.84,"4t Bin (tight WP)");
    tex.DrawLatex(0.20,0.79,"#kappa_{t}=-1, m_{H}=125.5 GeV");

    //
    // SAVE PLOT TO FILE(S)
    //
    string sprefix;
    for (unsigned i=0;i<prefix.size();i++) sprefix += prefix[i] + "_";

    for (unsigned ifmt=0;ifmt<formats.size();ifmt++)
      c1->Print((sprefix+"th_signi_"+nnout+"."+formats[ifmt]).c_str());
  }  
  
  //
  // RUN ROOT APPLICATION
  //
  if (!batch) app->Run();

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

