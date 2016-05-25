
////////////////////////////////////////////////////////////////////////////////
//
// th_roc_x
// -----------------
//
// tool to check for best signal/background ratio after BDT application
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
void   draw_labels(const vector<string>& labels,bool leginplot=true,bool res=true);
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
  string         mcpath2   = cl.getValue <string>  ("mcpath2",         "");
  string         mcpath3   = cl.getValue <string>  ("mcpath3",         "");
  string         mcpath4   = cl.getValue <string>  ("mcpath4",         "");
  string         dirname   = cl.getValue <string>  ("dirname",      "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix", ".root");
  string         treename  = cl.getValue <string>  ("treename",       "t");
  string         mass      = cl.getValue <string>  ("mass",            "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",            "");
  vector<string> cuts2     = cl.getVector<string>  ("cuts2",           "");
  vector<string> cuts3     = cl.getVector<string>  ("cuts3",           "");
  vector<string> cuts4     = cl.getVector<string>  ("cuts4",           "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",          "");
  vector<string> mccuts2   = cl.getVector<string>  ("mccuts2",         "");
  vector<string> mccuts3   = cl.getVector<string>  ("mccuts3",         "");
  vector<string> mccuts4   = cl.getVector<string>  ("mccuts4",         "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",       "");
  vector<string> mcweights2= cl.getVector<string>  ("mcweights2",      "");
  vector<string> mcweights3= cl.getVector<string>  ("mcweights3",      "");
  vector<string> mcweights4= cl.getVector<string>  ("mcweights4",      "");
  bool           overflow  = cl.getValue <bool>    ("overflow",     false);
  bool           underflow = cl.getValue <bool>    ("underflow",    false);
  bool           batch     = cl.getValue <bool>    ("batch",        false);
  vector<string> prefix    = cl.getVector<string>  ("prefix",          "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",          "");
  string         varexpr   = cl.getValue <string>  ("varexpr",         "");
  string         varexpr2  = cl.getValue <string>  ("varexpr2",   varexpr);
  string         varname   = cl.getValue <string>  ("varname",         "");
  unsigned       nbinsx    = cl.getValue <unsigned>("nbinsx",        3000);
  int            nbinsx_iter  = cl.getValue <int>("nbinsx_iter",  -1);
  double         xmin      = cl.getValue <double>  ("xmin",            -3);
  double         xmax      = cl.getValue <double>  ("xmax",             3);
  vector<string> formats   = cl.getVector<string>  ("formats",         "");
  double         legxmin   = cl.getValue <double>  ("legxmin",        0.2);
  double         legxmax   = cl.getValue <double>  ("legxmax",       0.64);
  double         legymin   = cl.getValue <double>  ("legymin",       0.34);
  double         legymax   = cl.getValue <double>  ("legymax",       0.54);

  // Dummyseeeeee
  bool           leginplot = cl.getValue <bool>    ("leginplot",    false);
  string         datalabel = cl.getValue <string>  ("datalabel",   "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",        "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",        "");
  vector<string> labels    = cl.getVector<string>  ("labels",          "");
  bool           compvar   = cl.getValue <bool>    ("compvar",      false);
  bool           drawthree = cl.getValue <bool>    ("drawthree",    false);
  bool           residuals = cl.getValue <bool>    ("residuals",    false);

  if(!cl.check()) return 0;
  cl.print();
  
  if (batch&&formats.size()==0) formats.push_back("pdf");

  //double xmin = -1;
  //double xmax = 1;
  //unsigned nbinsx = 1000;

  
  const char* masspoint = mass.c_str();

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

  vector<MCSample> mcsamples2;
  mcsamples2=MCSample::loadSamples(mcs,dirname,treename,mcpath2,filesuffix);
  if (mcsamples2.size()>0) MCSample::printSampleInfos(mcsamples2);
  
  //  vector<MCSample> mcsamples3;
  // mcsamples3=MCSample::loadSamples(mcs,dirname,treename,mcpath3,filesuffix);
  //if (mcsamples3.size()>0) MCSample::printSampleInfos(mcsamples3);



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
    
    
    
    Histogram histogram2;
    histogram2.setExpr(varexpr2);
    histogram2.setNbinsx(nbinsx);
    histogram2.setXmin(xmin);
    histogram2.setXmax(xmax);
    histogram2.setName("");
    
    
    /*
    Histogram histogram3;
    histogram3.setExpr(varexpr3);
    histogram3.setNbinsx(nbinsx);
    histogram3.setXmin(xmin);
    histogram3.setXmax(xmax);
    histogram3.setName("");

    */


    // overflow bin?

    if (overflow) {
      stringstream ss;
      ss<<"min("<<setprecision(10)<<(histogram.xmax()-1.E-04)
	<<","<<histogram.expr()<<")";
      histogram.setExpr(ss.str());
      histogram2.setExpr(ss.str());
    }
   
    if (underflow) {
      stringstream ss;
      ss<<"max("<<setprecision(10)<<(histogram.xmin()+1.E-04)
	<<","<<histogram.expr()<<")";
      histogram.setExpr(ss.str());
      histogram2.setExpr(ss.str());
    }

    // 1

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
       
    cout << "HIER" << endl;


    // 2

    string mcsel2;
    for (unsigned icut=0;icut<mccuts2.size();icut++) {
      if (!mcsel2.empty()) mcsel2 += "&&";
      mcsel2 += get_cut(mccuts2,icut,"expression");
    }
    string selection2;
    for (unsigned icut=0;icut<cuts2.size();icut++) {
      if (!selection2.empty()) selection2 += "&&";
      selection2 += get_cut(cuts2,icut,"expression");
    }
    mcsel2 = (mcsel2.empty()) ? selection2 : mcsel2 + "&&" + selection2;

   
    // 3
    /*
    string mcsel3;
    for (unsigned icut=0;icut<mccuts3.size();icut++) {
      if (!mcsel3.empty()) mcsel3 += "&&";
      mcsel3 += get_cut(mccuts3,icut,"expression");
    }
    string selection3;
    for (unsigned icut=0;icut<cuts3.size();icut++) {
      if (!selection3.empty()) selection3 += "&&";
      selection3 += get_cut(cuts3,icut,"expression");
    }
    mcsel3 = (mcsel3.empty()) ? selection3 : mcsel3 + "&&" + selection3;

    */

    // create MC templates for signal and background

    TH1F* hsignal = new TH1F("hsignal","BDTs Output",nbinsx,xmin,xmax);
    TH1F* hbackground = new TH1F("hbackground","BDTs Output",nbinsx,xmin,xmax);
    
    TH1F* hsignal2 = new TH1F("hsignal2","BDTs Output",nbinsx,xmin,xmax);
    TH1F* hbackground2 = new TH1F("hbackground2","BDTs Output",nbinsx,xmin,xmax);
    
    // TH1F* hsignal3 = new TH1F("hsignal3","BDTs Output",1000,-1,1);
    //TH1F* hbackground3 = new TH1F("hbackground3","BDTs Output",1000,-1,1);


    // FIRST

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
    
    // SECOND

    for (unsigned imc=0;imc<mcsamples2.size();imc++) {
      MCSample mcsample2  = mcsamples2[imc];
      mcsample2.setWeights(mcweights);
      bool isSignal2 = false;
	
      if(!compvar)
	{
	  for (unsigned i=0;i<signals.size();i++)
	    if (signals[i]==mcsample2.name()) isSignal2 = true;
	  if (isSignal2) {
	    TH1F* htemplates2=histogram.create("h"+mcsample2.name()+"s2");
	    mcsample2.project(htemplates2->GetName(),histogram.expr(),mcsel2);
	    htemplates2->Scale(lumi);
	    hsignal2->Add(htemplates2);  
	  }
	  
	  else {
	    TH1F* htemplateb2=histogram.create("h"+mcsample2.name()+"b2");
	    mcsample2.project(htemplateb2->GetName(),histogram.expr(),mcsel2);
	    htemplateb2->Scale(lumi);
	    hbackground2->Add(htemplateb2);
	  }
	}
      if(compvar)
	{
	  for (unsigned i=0;i<signals.size();i++)
	    if (signals[i]==mcsample2.name()) isSignal2 = true;
	  if (isSignal2) {
	    TH1F* htemplates2=histogram2.create("h"+mcsample2.name()+"s2");
	    mcsample2.project(htemplates2->GetName(),histogram2.expr(),mcsel2);
	    htemplates2->Scale(lumi);
	    hsignal2->Add(htemplates2);  
	  }
	  else {
	    TH1F* htemplateb2=histogram2.create("h"+mcsample2.name()+"b2");
	    mcsample2.project(htemplateb2->GetName(),histogram2.expr(),mcsel2);
	    htemplateb2->Scale(lumi);
	    hbackground2->Add(htemplateb2);
	  }
	}


    }	 


    // THIRD
    /*
    for (unsigned imc=0;imc<mcsamples3.size();imc++) {
      MCSample mcsample3  = mcsamples3[imc];
      mcsample3.setWeights(mcweights);
      cout << "3d " << endl;
      bool isSignal3 = false;
	
      if(!compvar)
	{
	  //cout << "3a " << endl;
	  for (unsigned i=0;i<signals.size();i++)
	    if (signals[i]==mcsample3.name()) isSignal3 = true;
	  //cout << "3b " << endl;
	  if (isSignal3) {
	    TH1F* htemplates3=histogram.create("h"+mcsample3.name()+"s3");
	    mcsample3.project(htemplates3->GetName(),histogram.expr(),mcsel3);
	    htemplates3->Scale(lumi);
	    hsignal3->Add(htemplates3);  
	  }
	  
	  else {
	    TH1F* htemplateb3=histogram.create("h"+mcsample3.name()+"b3");
	    mcsample3.project(htemplateb3->GetName(),histogram.expr(),mcsel3);
	    htemplateb3->Scale(lumi);
	    hbackground3->Add(htemplateb3);
	  }
	  cout << "3c " << endl;
	}
      if(compvar)
	{
	  //cout << "3e " << endl;
	  for (unsigned i=0;i<signals.size();i++)
	    if (signals[i]==mcsample3.name()) isSignal3 = true;
	  //cout << "3f " << endl;
	  if (isSignal3) {
	    TH1F* htemplates3=histogram3.create("h"+mcsample3.name()+"s3");
	    mcsample3.project(htemplates3->GetName(),histogram3.expr(),mcsel3);
	    htemplates3->Scale(lumi);
	    hsignal3->Add(htemplates3);  
	  }
	  else {
	    TH1F* htemplateb3=histogram3.create("h"+mcsample3.name()+"b3");
	    mcsample3.project(htemplateb3->GetName(),histogram3.expr(),mcsel3);
	    htemplateb3->Scale(lumi);
	    hbackground3->Add(htemplateb3);
	    }
	  }
  

	}	 


    */


    cout << "HIER 4" << endl;
    // calculate significance and errors



    float x = xmin;

    



    TGraph* ROC = new TGraph(0);
    TGraph* ROC2 = new TGraph(0);
    // TGraph* ROC3 = new TGraph(0);
    
    float totalsig=0;
    float totalbkg=0;
    float totalsig2=0;
    float totalbkg2=0;
    // float totalsig3=0;
    //float totalbkg3=0;
    

    // 1

    totalsig=hsignal->Integral(hsignal->FindBin(xmin),hsignal->FindBin(xmax));
    totalbkg=hbackground->Integral(hbackground->FindBin(xmin),hbackground->FindBin(xmax));
    
    // 2

    totalsig2=hsignal2->Integral(hsignal2->FindBin(xmin),hsignal2->FindBin(xmax));
    totalbkg2=hbackground2->Integral(hbackground2->FindBin(xmin),hbackground2->FindBin(xmax));

    // 3

    //totalsig3=hsignal3->Integral(hsignal3->FindBin(x),hsignal3->FindBin(1.0));
    //totalbkg3=hbackground3->Integral(hbackground3->FindBin(x),hbackground3->FindBin(1.0));
   

    cout << totalsig << " " << totalsig2 << " " <<endl;
    float signeff=0;
    float bkgeff=0;
    float bkgrej=0;
    float signeff2=0;
    float bkgeff2=0;
    float bkgrej2=0;
    //    float signeff3=0;
    //float bkgeff3=0;
    //float bkgrej3=0;

    while (x<xmax) {
      float integrals = 
	hsignal->Integral(hsignal->FindBin(x),hsignal->FindBin(xmax));
    
      float integralb = 
	hbackground->Integral(hbackground->FindBin(x),hbackground->FindBin(xmax));
      
      
      float integrals2 = 
	hsignal2->Integral(hsignal2->FindBin(x),hsignal2->FindBin(xmax));
    
      float integralb2 = 
	hbackground2->Integral(hbackground2->FindBin(x),hbackground2->FindBin(xmax));
      


      //cout << "Integral1: "<<integrals<< " and integral2: "<< integrals2 << " bei X = " << x << endl; 

	//hbackground3->Integral(hbackground3->FindBin(x),hbackground3->FindBin(1.0));


      //CALCULATIONS FOR BKGEFF OVER SIGEFF -> ROC CURVE
      int nroc = ROC->GetN();
      signeff = integrals/totalsig;
      bkgeff = integralb/totalbkg;
      bkgrej = 1-bkgeff;

      int nroc2 = ROC2->GetN();
      signeff2 = integrals2/totalsig2;
      bkgeff2 = integralb2/totalbkg2;
      bkgrej2 = 1-bkgeff2;


      //int nroc3 = ROC3->GetN();
      //signeff3 = integrals3/totalsig3;
      //bkgeff3 = integralb3/totalbkg3;
      //bkgrej3 = 1-bkgeff3;


      //SET POINTS FOR PLOTS

      cout << "sigeff: "<<signeff<< " and sigeff2: "<< signeff2 << " bei X = " << x << endl; 
 

      ROC->SetPoint(nroc,signeff,bkgrej);
      ROC2->SetPoint(nroc2,signeff2,bkgrej2);
      //ROC3->SetPoint(nroc3,signeff3,bkgrej3);
      
      if(nbinsx_iter>0)
	x += (xmax-xmin)/nbinsx_iter;
      else
	x += (xmax-xmin)/nbinsx;

    }
        
    // draw sic
    
    TCanvas* c1 = new TCanvas("c1","SIC",300,0,1600,1200);
    setup_global_style();


    gStyle->SetPalette(1);

   


    c1->cd();
    //c1->SetLogx();
    ROC->SetTitle("");
    ROC->GetXaxis()->SetLimits(0,1);
    ROC->GetYaxis()->SetLimits(0,1);
    ROC->SetMinimum(0.0);
    ROC->SetMarkerSize(1);
    ROC->SetMarkerStyle(21);
    ROC->SetMarkerColor(8);
    ROC->SetLineWidth(2);
    ROC->SetLineColor(8);
    ROC->Draw("AL");
    ROC->GetXaxis()->SetTitle("Signal Efficiency");
    ROC->GetYaxis()->SetTitle("Background Rejection");
    ROC->GetXaxis()->SetLabelFont(42);
    ROC->GetYaxis()->SetLabelFont(42);
    ROC->GetXaxis()->SetTitleFont(42);
    ROC->GetYaxis()->SetTitleFont(42);
    

    ROC2->SetTitle("");
    ROC2->SetLineColor(kAzure);
    ROC2->SetLineWidth(2.0);
    //ROC2->SetLineWidth(0.0);
    ROC2->SetMarkerStyle(3);
    ROC2->SetMarkerStyle(20);
    ROC2->Draw("same");

    ROC2->SetTitle("");
    ROC2->SetLineColor(kAzure-7);
    if(drawthree)
      ROC2->Draw("same");


    TLatex tex;
    tex.SetTextSize(0.04);
    tex.SetTextFont(42);
    tex.SetNDC(true);

    //tex.DrawLatex(0.20,0.96,"#sqrt{s}=8 TeV");
    //tex.DrawLatex(0.36,0.96,"L=19.3 fb^{-1}");
    //tex.DrawLatex(0.20,0.74,"t(bl#nu)H(b#bar{b})");
    //tex.DrawLatex(0.20,0.68,"3t Bin (tight WP)");
    //tex.DrawLatex(0.20,0.62,"#kappa_{t}=-1, m_{H}=");
    //tex.DrawLatex(0.325,0.62, masspoint);
    //tex.DrawLatex(0.40,0.62,"GeV");
    //tex.DrawLatex(0.20,0.56,"#bf{ROC} curve of diff. MVA outputs");

    TLegend* leg1 = new TLegend(legxmin,legymin,legxmax,legymax);
    leg1->SetBorderSize(1);
    leg1->SetFillColor(kWhite);
    leg1->SetFillStyle(4000);//transparent
    leg1->SetTextFont(42);
    leg1->AddEntry(ROC, "m_{b#bar{b}} in reco training ", "l");
    leg1->AddEntry(ROC2, "m_{b#bar{b}} not used in reco ","l");
    // if(drawthree)
    //  leg1->AddEntry(ROC3, "#it{lalala}","l");
    leg1->SetTextSize(0.04);
    leg1->SetFillColor(10);
    leg1->SetLineColor(10);
    leg1->Draw();
    draw_labels(labels,leginplot,residuals);
    
    gPad->RedrawAxis();
    //c1   ->SetGridx();
    //c1   ->SetGridy();
    c1   ->Modified();

   

    //
    // SAVE PLOT TO FILE(S)
    //
    string sprefix;
    for (unsigned i=0;i<prefix.size();i++) sprefix += prefix[i] + "_";

    for (unsigned ifmt=0;ifmt<formats.size();ifmt++)
      c1->Print((sprefix+"th_roc_"+varname+"."+formats[ifmt]).c_str());
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

//______________________________________________________________________________
void draw_labels(const vector<string>& labels,bool leginplot,bool res)
{
  for (unsigned ilabel=0;ilabel<labels.size();ilabel++) {
    string tmp(labels[ilabel]);
    size_t pos;
    pos = tmp.find(':'); assert(pos!=string::npos);
    string x_as_str = tmp.substr(0,pos); tmp = tmp.substr(pos+1);
    pos = tmp.find(':'); assert(pos!=string::npos);
    string y_as_str = tmp.substr(0,pos); tmp = tmp.substr(pos+1);
    pos = tmp.find(':');
    string font_as_str;
    string size_as_str;
    string text;
    if (pos==string::npos) {
      font_as_str = "42";
      size_as_str = "0.04";
      text = tmp;
    }
    else {
      font_as_str = tmp.substr(0,pos); tmp = tmp.substr(pos+1);
      pos = tmp.find(':');
      if (pos==string::npos) {
	size_as_str = "0.04";
	text = tmp; 
      }
      else {
	size_as_str = tmp.substr(0,pos);
	text = tmp.substr(pos+1);
      }
    }
    
    stringstream ssx; ssx<<x_as_str;    double x; ssx>>x;
    stringstream ssy; ssy<<y_as_str;    double y; ssy>>y;
    stringstream ssf; ssf<<font_as_str; int    f; ssf>>f;
    stringstream sss; sss<<size_as_str; double s; sss>>s;
    
    if (!leginplot) { double scale = 800.0*0.93/1000./0.82;  x *= scale; }

    if (res) { y = 1.-1.33*(1.-y); s/=0.8; }

    TLatex tex;
    tex.SetTextSize(s);
    tex.SetTextFont(f);
    tex.SetNDC(true);
    tex.DrawLatex(x,y,text.c_str());
  }
}
