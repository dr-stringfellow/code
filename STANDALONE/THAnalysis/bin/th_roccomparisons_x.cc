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
  
  
  vector<string> input     = cl.getVector<string>  ("input"              );
  vector<string> mcs       = cl.getVector<string>  ("mcs",             "");
  double         lumi      = cl.getValue <double>  ("lumi"               );
  string         dirname   = cl.getValue <string>  ("dirname",      "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix", ".root");
  string         treename  = cl.getValue <string>  ("treename",       "t");
  string         mass      = cl.getValue <string>  ("mass",            "");
  bool           overflow  = cl.getValue <bool>    ("overflow",     false);
  bool           underflow = cl.getValue <bool>    ("underflow",    false);
  bool           batch     = cl.getValue <bool>    ("batch",        false);
  vector<string> prefix    = cl.getVector<string>  ("prefix",          "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",          "");
  string         varexpr   = cl.getValue <string>  ("varexpr",         "");
  string         varexpr2  = cl.getValue <string>  ("varexpr2",   varexpr);
  unsigned       nbinsx    = cl.getValue <unsigned>("nbinsx",        3000);
  double         legxmin   = cl.getValue <double>  ("legxmin",        0.2);
  double         legxmax   = cl.getValue <double>  ("legxmax",       0.64);
  double         legymin   = cl.getValue <double>  ("legymin",       0.34);
  double         legymax   = cl.getValue <double>  ("legymax",       0.54);
  
  vector<string> formats   = cl.getVector<string>  ("formats",         "");
  // dummys
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
  // CREATE HISTOGRAMS FROM ROOT FILES
  //

  vector<TH1F*> vec_hist;
  vector<double> h_int;
  vector<TFile*> file;

  for(unsigned i=0;i<input.size();i++){
    cout << "file " << i << endl;
    TFile* tmp = new TFile(input[i].c_str(),"READ");
    file.push_back(tmp);

    if (!tmp->IsOpen()) {
      cout<<"Failed to open File"<<endl;
    }

    cout << "hist " << i << endl;
    TH1F* tmp2 = (TH1F*)file[i]->Get("Method_BDT/BDTG/MVA_BDTG_trainingRejBvsS");
    //TH1F* tmp2 = (TH1F*)file[i]->Get("Method_BDT/BDT/MVA_BDT_trainingRejBvsS");
    vec_hist.push_back(tmp2);
  }



  for(unsigned i=0; i<vec_hist.size(); i++)
    {
      h_int.push_back(vec_hist[i]->Integral("width"));
      cout << "Area under ROC curve for case" << i << " is: " << h_int[i] << endl;
    }
        
    // draw sic
    
    TCanvas* c1 = new TCanvas("c1","SIC",300,0,1600,1200);
    setup_global_style();
    gStyle->SetPalette(1);

   
    

    c1->cd();
    //c1->SetLogx();
    vec_hist[0]->SetTitle("");
    vec_hist[0]->GetXaxis()->SetLimits(0,1);
    vec_hist[0]->GetXaxis()->SetLimits(0.0,1);
    vec_hist[0]->GetYaxis()->SetLimits(0.4,1);
    //vec_hist[1]->GetXaxis()->SetLimits(0.3,1);
    //vec_hist[1]->GetYaxis()->SetLimits(0.4,1);
    //vec_hist[0]->SetMinimum(0.5);
    //vec_hist[0]->SetMinimum(0.3);
    //ec_hist[0]->SetMinimum(0.3);
    vec_hist[0]->GetYaxis()->SetRangeUser(0.775,0.9);
    vec_hist[0]->GetXaxis()->SetRangeUser(0.5,0.7);
    vec_hist[0]->SetMarkerSize(1);
    vec_hist[0]->SetMarkerStyle(21);
    vec_hist[0]->SetMarkerColor(8);
    vec_hist[0]->SetLineWidth(2);
    vec_hist[0]->SetLineColor(8);
    vec_hist[0]->Draw("HIST");
    vec_hist[0]->GetXaxis()->SetTitle("");
    vec_hist[0]->GetYaxis()->SetTitle("");
    vec_hist[0]->GetXaxis()->SetLabelFont(42);
    vec_hist[0]->GetYaxis()->SetLabelFont(42);
    vec_hist[0]->GetXaxis()->SetTitleFont(42);
    vec_hist[0]->GetYaxis()->SetTitleFont(42);
    

    for(unsigned i=1;i<vec_hist.size();i++)
      {
	if (i==1) vec_hist[i]->SetLineColor(kAzure);
	vec_hist[i]->SetLineWidth(2);
	vec_hist[i]->Draw("same");
      }
    
    //ROC2->SetTitle("");
    //ROC2->SetLineColor(kAzure);
    //ROC2->SetLineWidth(2.0);
    //ROC2->Draw("same");

    //ROC2->SetTitle("");
    //ROC2->SetLineColor(kAzure-7);
    //if(drawthree)
    //  ROC2->Draw("same");


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
    leg1->AddEntry(vec_hist[0], "training without m_{b#bar{b}}", "l");
    leg1->AddEntry(vec_hist[1], "training with m_{b#bar{b}}","l");
    // if(drawthree)
    //  leg1->AddEntry(ROC3, "#it{lalala}","l");
    leg1->SetTextSize(0.04);
    leg1->SetFillColor(10);
    leg1->SetLineColor(10);
    //leg1->Draw();
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
      c1->Print((sprefix+"th_roc_"+varexpr+"."+formats[ifmt]).c_str());

  
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
