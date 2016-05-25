#////////////////////////////////////////////////////////////////////////////////
//
// th_syscomparer_x
// -----------------
//
// tool to check for effects of systematic shape uncertainties
//
// 
//                                    23/09/2013 Benedikt Maier <bmaier@cern.ch>
//                                               
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
#include <TGaxis.h>
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
  
  
  vector<string> signals   = cl.getVector<string>  ("signals",         "");
  vector<string> mcs       = cl.getVector<string>  ("mcs",             "");
  vector<string> mcs_2     = cl.getVector<string>  ("mcs_2",           "");
  vector<string> mcs_3     = cl.getVector<string>  ("mcs_3",           "");
  double         lumi      = cl.getValue <double>  ("lumi"               );
  int            sys       = cl.getValue <int>     ("sys",             -1);
  string         datapath  = cl.getValue <string>  ("datapath",        "");
  string         mcpath    = cl.getValue <string>  ("mcpath",          "");
  string         mcpath2   = cl.getValue <string>  ("mcpath2",         "");
  string         mcpath3   = cl.getValue <string>  ("mcpath3",         "");
  string         dirname   = cl.getValue <string>  ("dirname",      "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix", ".root");
  string         treename  = cl.getValue <string>  ("treename",       "t");
  string         mass      = cl.getValue <string>  ("mass",            "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",            "");
  vector<string> cuts2     = cl.getVector<string>  ("cuts2",           "");
  vector<string> cuts3     = cl.getVector<string>  ("cuts3",           "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",       "");
  vector<string> mcweights2= cl.getVector<string>  ("mcweights2",      "");
  vector<string> mcweights3= cl.getVector<string>  ("mcweights3",      "");
  bool           overflow  = cl.getValue <bool>    ("overflow",     true);
  bool           residuals  = cl.getValue <bool>    ("residuals",     false);
  bool           sig       = cl.getValue <bool>    ("sig",     true);
  bool           batch     = cl.getValue <bool>    ("batch",        false);
  vector<string> prefix    = cl.getVector<string>  ("prefix",          "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",          "");
  string         bdtout    = cl.getValue <string>  ("bdtout",          "mlpout");
  vector<string> formats   = cl.getVector<string>  ("formats",         "");
  int            i_nbinsx  = cl.getValue <int>     ("nbinsx",          15);
  double         i_xmin    = cl.getValue <double>  ("xmin",           -0.1);
  double         i_xmax    = cl.getValue <double>  ("xmax",            1.1);
  // dummys
  string         datalabel = cl.getValue <string>  ("datalabel",   "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",        "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",        "");
  vector<string> labels    = cl.getVector<string>  ("labels",          "");

  if(!cl.check()) return 0;
  cl.print();
  
  if (batch&&formats.size()==0) formats.push_back("pdf");

  string varexpr = bdtout;
  unsigned nbinsx = i_nbinsx;
  double xmin = i_xmin;
  double xmax = i_xmax;

  const char* masspoint = mass.c_str();

  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //

  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app = new TApplication("th_maketemplates_x",&argc,argv);
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
  if(!mcs_2.empty())
    mcsamples2=MCSample::loadSamples(mcs_2,dirname,treename,mcpath2,filesuffix);
  else
    mcsamples2=MCSample::loadSamples(mcs,dirname,treename,mcpath2,filesuffix);
  if (mcsamples2.size()>0) MCSample::printSampleInfos(mcsamples2);
  
  vector<MCSample> mcsamples3;

  if(!mcs_3.empty())
    mcsamples3=MCSample::loadSamples(mcs_3,dirname,treename,mcpath3,filesuffix);
  else
    mcsamples3=MCSample::loadSamples(mcs,dirname,treename,mcpath3,filesuffix);
  if (mcsamples3.size()>0) MCSample::printSampleInfos(mcsamples3);
  
  


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
   
    string selection;
    for (unsigned icut=0;icut<cuts.size();icut++) {
      if (!selection.empty()) selection += "&&";
      selection += get_cut(cuts,icut,"expression");
    }

    string selection2;
    if(cuts2.empty())
      selection2 = selection;
    else{
      for (unsigned icut=0;icut<cuts2.size();icut++) {
	if (!selection2.empty()) selection2 += "&&";
	selection2 += get_cut(cuts2,icut,"expression");
      }
    }

    string selection3;
    if(cuts3.empty())
      selection3 = selection;
    else{
      for (unsigned icut=0;icut<cuts3.size();icut++) {
	if (!selection3.empty()) selection3 += "&&";
	selection3 += get_cut(cuts3,icut,"expression");
      }
    }


    // create MC templates for signal and background

    TH1F* hsignal = new TH1F("hsignal","NN output",15,-0.1,1.1);
    TH1F* hbackground = new TH1F("hbackground","NN output",15,-0.1,1.1);
    
    TH1F* hsignal2 = new TH1F("hsignal2","NN output",15,-0.1,1.1);
    TH1F* hbackground2 = new TH1F("hbackground2","NN output",15,-0.1,1.1);
    
    TH1F* hsignal3 = new TH1F("hsignal3","NN output",15,-0.1,1.1);
    TH1F* hbackground3 = new TH1F("hbackground3","NN output",15,-0.1,1.1);
    
    TH1F* signal = new TH1F("signal","NN output",15,-0.1,1.1);
    TH1F* signal2 = new TH1F("signal2","NN output",15,-0.1,1.1);
    TH1F* signal3 = new TH1F("signal3","NN output",15,-0.1,1.1);

    TH1F* background = new TH1F("background","NN output",15,-0.1,1.1);
    TH1F* background2 = new TH1F("background2","NN output",15,-0.1,1.1);
    TH1F* background3 = new TH1F("background3","NN output",15,-0.1,1.1);


    // NOMINAL

    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample mcsample  = mcsamples[imc];
      mcsample.setWeights(mcweights);

      bool isSignal = false;
	
      for (unsigned i=0;i<signals.size();i++)
	 if (signals[i]==mcsample.name()) isSignal = true;
	
	if (isSignal) {
	  TH1F* htemplates=histogram.create("h"+mcsample.name()+"s");
	  mcsample.project(htemplates->GetName(),histogram.expr(),selection);
	  htemplates->Scale(lumi);
	  hsignal->Add(htemplates);  
	}
	else {
	  TH1F* htemplateb=histogram.create("h"+mcsample.name()+"b");
	  mcsample.project(htemplateb->GetName(),histogram.expr(),selection);
	  htemplateb->Scale(lumi);
	  hbackground->Add(htemplateb);
	}
    }	 	
    
    // UP VARIATION

    for (unsigned imc=0;imc<mcsamples2.size();imc++) {
      MCSample mcsample2  = mcsamples2[imc];
      if(mcweights2.empty())
	mcsample2.setWeights(mcweights);
      else
	mcsample2.setWeights(mcweights2);

      bool isSignal2 = false;
	
      for (unsigned i=0;i<signals.size();i++)
	 if (signals[i]==mcsample2.name()) isSignal2 = true;
	
	if (isSignal2) {
	  TH1F* htemplates2=histogram.create("h"+mcsample2.name()+"s2");
	  mcsample2.project(htemplates2->GetName(),histogram.expr(),selection2);
	  htemplates2->Scale(lumi);
	  hsignal2->Add(htemplates2);  
	}
	else {
	  TH1F* htemplateb2=histogram.create("h"+mcsample2.name()+"b2");
	  mcsample2.project(htemplateb2->GetName(),histogram.expr(),selection2);
	  htemplateb2->Scale(lumi);
	  hbackground2->Add(htemplateb2);
	}
    }	 

    // DOWN VARIATION

    for (unsigned imc=0;imc<mcsamples3.size();imc++) {
      MCSample mcsample3  = mcsamples3[imc];
      if(mcweights3.empty())
	mcsample3.setWeights(mcweights);
      else 	
	mcsample3.setWeights(mcweights3);

      bool isSignal3 = false;
	
      for (unsigned i=0;i<signals.size();i++)
	 if (signals[i]==mcsample3.name()) isSignal3 = true;
	
	if (isSignal3) {
	  TH1F* htemplates3=histogram.create("h"+mcsample3.name()+"s3");
	  mcsample3.project(htemplates3->GetName(),histogram.expr(),selection3);
	  htemplates3->Scale(lumi);
	  hsignal3->Add(htemplates3);  
	}
	else {
	  TH1F* htemplateb3=histogram.create("h"+mcsample3.name()+"b3");
	  mcsample3.project(htemplateb3->GetName(),histogram.expr(),selection3);
	  htemplateb3->Scale(lumi);
	  hbackground3->Add(htemplateb3);
	}
    }	 

    

    signal = (TH1F*)hsignal->Clone("signal");
    signal2 = (TH1F*)hsignal2->Clone("signal2");
    signal3 = (TH1F*)hsignal3->Clone("signal3");

    background = (TH1F*)hbackground->Clone("background");
    background2 = (TH1F*)hbackground2->Clone("background2");
    background3 = (TH1F*)hbackground3->Clone("background3");

    //signal->Scale(1./(signal->Integral()));
    //signal2->Scale(1./(signal2->Integral()));
    //signal3->Scale(1./(signal3->Integral()));

    // draw sic
    
    TCanvas* c1 = new TCanvas("c1","SIC",300,0,1000,800);
    setup_global_style();


    gStyle->SetPalette(1);
    
   
    float max = signal->GetMaximum();
    float max_bkg = background->GetMaximum();
    //pad1->Draw();
    //pad1->cd();

    if(residuals){
      c1->Divide(1,2,0.5,0.0);
      c1->cd(1);
    }
    else
      c1->cd();


    if(sig)
      {
	signal3->SetTitle("");
	signal3->GetXaxis()->SetLimits(-0.1,1.1);
	//signal->GetYaxis()->SetLimits(0,1);
	signal3->SetMaximum(max*1.5);
	signal3->SetMarkerSize(0);
	signal3->SetMarkerStyle(21);
	signal3->SetMarkerColor(8);
	signal3->SetLineWidth(2);
	signal3->SetLineColor(603);
	signal3->Draw("HIST");
	signal3->GetXaxis()->SetTitle("MVA output");
	signal3->GetYaxis()->SetTitle("Events");
	signal3->GetXaxis()->SetLabelFont(42);
	signal3->GetYaxis()->SetTitleSize(0.07);
	signal3->GetYaxis()->SetLabelSize(0.06);
	signal3->GetXaxis()->SetTitleSize(0.07);
	signal3->GetXaxis()->SetLabelSize(0.06);
	signal3->GetYaxis()->SetLabelFont(42);
	signal3->GetXaxis()->SetTitleFont(42);
	signal3->GetYaxis()->SetTitleFont(42);
	
	
	signal2->SetTitle("");
	signal2->SetMarkerSize(0);
	signal2->SetLineWidth(2);
	signal2->SetLineColor(593);
	signal2->Draw("HISTsame");
	
	signal->SetTitle("");
	signal->SetMarkerSize(0);
	signal->SetLineWidth(2);
	signal->SetLineColor(600);
	signal->Draw("HISTsame");
      }
    else
      {
	background3->SetTitle("");
	background3->GetXaxis()->SetLimits(-0.1,1.1);
	//background->GetYaxis()->SetLimits(0,1);
	background3->SetMaximum(max_bkg*1.5);
	background3->SetMarkerSize(0);
	background3->SetMarkerStyle(21);
	background3->SetMarkerColor(800);
	background3->SetLineWidth(2);
	background3->SetLineColor(800);
	background3->Draw("HIST");
	background3->GetXaxis()->SetTitle("MVA output");
	background3->GetYaxis()->SetTitle("Events");
	background3->GetXaxis()->SetLabelFont(42);
	background3->GetYaxis()->SetTitleSize(0.07);
	background3->GetYaxis()->SetLabelSize(0.06);
	background3->GetXaxis()->SetTitleSize(0.07);
	background3->GetXaxis()->SetLabelSize(0.06);
	background3->GetYaxis()->SetLabelFont(42);
	background3->GetXaxis()->SetTitleFont(42);
	background3->GetYaxis()->SetTitleFont(42);
	
	
	background2->SetTitle("");
	background2->SetMarkerSize(0);
	background2->SetLineWidth(2);
	background2->SetLineColor(612);
	background2->Draw("HISTsame");
	
	background->SetTitle("");
	background->SetMarkerSize(0);
	background->SetLineWidth(2);
	background->SetLineColor(632);
	background->Draw("HISTsame");


      }
    
    

    TLatex tex;
    tex.SetTextSize(0.05);
    tex.SetTextFont(42);
    tex.SetNDC(true);

    tex.DrawLatex(0.200,0.95,"20 fb^{-1} (8 TeV)");
    //tex.DrawLatex(0.4,0.95,"L=20 fb^{-1}");
    tex.DrawLatex(0.200,0.88,"Muon + electron channel");
    //tex.DrawLatex(0.200,0.81,"y_{t}=-1, m_{H}=125 GeV");
    tex.DrawLatex(0.33,0.81, masspoint);
    if(sig)
      tex.DrawLatex(0.55,0.34, "tHq");
    else
      tex.DrawLatex(0.28,0.28, "background sum");
    //Texa.DrawLatex(0.385,0.82,"GeV");
 
 


    TLegend* leg1 = new TLegend(0.20,0.59,.64,.75);
    leg1->SetBorderSize(1);
    leg1->SetFillColor(kWhite);
    leg1->SetFillStyle(4000);//transparent
    leg1->SetTextFont(42);
    leg1->AddEntry(signal, "Nominal", "l");
    leg1->AddEntry(signal2, "Down","l");
    leg1->AddEntry(signal3, "Up","l");
    
    leg1->SetTextSize(0.04);
    leg1->SetFillColor(10);
    leg1->SetLineColor(10);
    
    if(sig)
      leg1->Draw();


    TLegend* leg2 = new TLegend(0.55,0.59,.84,.75);
    leg2->SetBorderSize(1);
    leg2->SetFillColor(kWhite);
    leg2->SetFillStyle(4000);//transparent
    leg2->SetTextFont(42);
    leg2->AddEntry(background, "Nominal", "l");
    leg2->AddEntry(background2, "Down","l");
    leg2->AddEntry(background3, "Up","l");
    
    leg2->SetTextSize(0.04);
    leg2->SetFillColor(10);
    leg2->SetLineColor(10);
    if(!sig)
      leg2->Draw();
    
    
   
    gPad->RedrawAxis();
    
    c1   ->Modified();


    if (residuals) {
      c1->cd(2);

      TH1F* res_up = (TH1F*)hsignal->Clone();
      TH1F* res_down = (TH1F*)hsignal->Clone();

      res_up->Reset();
      res_down->Reset();

      res_up->GetYaxis()->CenterTitle();
      res_up->GetYaxis()->SetTitleSize(0.11);
      res_up->GetYaxis()->SetTitleOffset(0.66);
      res_up->GetYaxis()->SetLabelSize(0.16);
      res_up->GetYaxis()->SetNdivisions(505);
      res_up->GetXaxis()->SetTitleSize(0.19);
      res_up->GetXaxis()->SetLabelSize(0.19);
      res_up->GetXaxis()->SetTitleOffset(1);
      res_up->GetXaxis()->SetLabelOffset(0.006);
      res_up->GetXaxis()->SetNdivisions(505);
      res_up->SetLineColor(632);
      res_up->GetYaxis()->SetRangeUser(0.,2.);
      res_up->GetXaxis()->SetTickLength(res_up->GetXaxis()->GetTickLength() * 3.0 );

      res_up->SetYTitle("#frac{syst}{nom}");

      for (int ibin=1;ibin<=res_up->GetNbinsX();ibin++) {

	Float_t bin_up = hsignal3->GetBinContent(ibin);
	Float_t bin_down = hsignal2->GetBinContent(ibin);
	Float_t bin_nom = hsignal->GetBinContent(ibin);
	Float_t ratio_up = bin_up/bin_nom;
	Float_t ratio_down = bin_down/bin_nom;
	res_up->SetBinContent(ibin,ratio_up);
	res_up->SetBinError(ibin,0);
	res_down->SetBinContent(ibin,ratio_down);
	res_down->SetBinError(ibin,0);
      }

      res_up->Draw("HIST");
      res_down->Draw("HIST same");
    }


   

    //
    // SAVE PLOT TO FILE(S)
    //
    string sprefix;
    for (unsigned i=0;i<prefix.size();i++) sprefix += prefix[i] + "_";

    for (unsigned ifmt=0;ifmt<formats.size();ifmt++)
      {
	if(sys==1){
	  if(sig==1)
	    c1->Print((sprefix+"bdt_sig_sysbtag_"+bdtout+"."+formats[ifmt]).c_str());
	  else
	    c1->Print((sprefix+"bdt_bkg_sysbtag_"+bdtout+"."+formats[ifmt]).c_str());
	}
	else if(sys==2){
	  if(sig==1)
	    c1->Print((sprefix+"bdt_sig_sysmistag_"+bdtout+"."+formats[ifmt]).c_str());
	  else
	    c1->Print((sprefix+"bdt_bkg_sysmistag_"+bdtout+"."+formats[ifmt]).c_str());
	}
	else if(sys==3){
	  if(sig==1)
	    c1->Print((sprefix+"bdt_sig_systoppt_"+bdtout+"."+formats[ifmt]).c_str());
	  else
	    c1->Print((sprefix+"bdt_bkg_systoppt_"+bdtout+"."+formats[ifmt]).c_str());
	}
	else if(sys==4){
	  if(sig==1)
	    c1->Print((sprefix+"bdt_sig_sysq2_"+bdtout+"."+formats[ifmt]).c_str());
	  else
	    c1->Print((sprefix+"bdt_bkg_sysq2_"+bdtout+"."+formats[ifmt]).c_str());
	}
	else 
	  c1->Print((sprefix+"bdt_"+bdtout+"."+formats[ifmt]).c_str());
	
      }


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

