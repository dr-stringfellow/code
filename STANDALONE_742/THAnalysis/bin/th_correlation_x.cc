////////////////////////////////////////////////////////////////////////////////
//
// th_correlation_x
// -----------------
//
//                                          
////////////////////////////////////////////////////////////////////////////////


#include "../interface/DataSample.h"
#include "../interface/MCSample.h"

#include "../interface/CommandLine.h"
#include "../interface/RootStyle.h"

#include <TApplication.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TColor.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2F.h>
#include <TProfile.h>
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


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper function
////////////////////////////////////////////////////////////////////////////////

string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression");
void draw_labels(const vector<string>& labels,bool leginplot,bool res);

		    
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
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  string         mcsubpath = cl.getValue <string>  ("mcsubpath",           "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");

  string         xvarname  = cl.getValue <string>  ("xvarname",            "");
  string         xvarexpr  = cl.getValue <string>  ("xvarexpr",            "");
  string         yvarname  = cl.getValue <string>  ("yvarname",            "");
  string         yvarexpr  = cl.getValue <string>  ("yvarexpr",            "");
  string         xtitle    = cl.getValue <string>  ("xtitle",              "");
  string         ytitle    = cl.getValue <string>  ("ytitle",              "");
  unsigned       nbinsx    = cl.getValue <unsigned>("nbinsx",               0);
  vector<double> binsx     = cl.getVector<double>  ("binsx",               "");
  double         xmin      = cl.getValue <double>  ("xmin",             -1E10);
  double         xmax      = cl.getValue <double>  ("xmax",             +1E10);
  double         zmax      = cl.getValue <double>  ("zmax",             0.05);
  unsigned       nbinsy    = cl.getValue <unsigned>("nbinsy",               0);
  vector<double> binsy     = cl.getVector<double>  ("binsy",               "");
  double         ymin      = cl.getValue <double>  ("ymin",             -1E10);
  double         ymax      = cl.getValue <double>  ("ymax",             +1E10);
  double         plotymin  = cl.getValue <double>  ("plotymin",         -1E10);
  double         plotymax  = cl.getValue <double>  ("plotymax",         +1E10);

  double         legxmin   = cl.getValue <double>  ("legxmin",            0.7);
  double         legxmax   = cl.getValue <double>  ("legxmax",            0.9);
  double         legymin   = cl.getValue <double>  ("legymin",            0.7);
  double         legymax   = cl.getValue <double>  ("legymax",            0.9);

  bool           batch     = cl.getValue <bool>    ("batch",            false);
  bool           scatter   = cl.getValue <bool>    ("scatter",          false);
  bool           ratioscat = cl.getValue <bool>    ("ratioscat",        false);
  vector<string> formats   = cl.getVector<string>  ("formats",             "");
  string         opath     = cl.getValue <string>  ("opath",               "");
  string         output    = cl.getValue <string>  ("output",              "");
  vector<string> prefix    = cl.getVector<string>  ("prefix",              "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",              "");
  
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");
  vector<string> labels    = cl.getVector<string>  ("labels",              "");

  if(!cl.check()) return 0;
  cl.print();
  
  if (data.empty()&&mcs.empty()){cout<<"provide -data and/or -mcs!"<<endl;return 0;}
  if (batch&&formats.size()==0) formats.push_back("pdf");

  if (xtitle.empty()) xtitle = xvarexpr;
  if (ytitle.empty()) ytitle = yvarexpr;

  string htitle = ";" + xtitle + ";" + ytitle;
  //  string expression = xvarexpr + ":" + yvarexpr;
  string expression = yvarexpr + ":" + xvarexpr;

  //
  // OPEN DATA FILE / RETRIEVE TREE
  //
  DataSample datasample=DataSample::load(data,dirname,treename,datapath,filesuffix);
  if (!datasample.empty()) DataSample::printInfo(datasample);
  
  
  //
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples;
  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath+mcsubpath,filesuffix);
  if (mcsamples.size()>0) MCSample::printSampleInfos(mcsamples);
  
  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //
  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app = new TApplication("utm_datatomc_x",&argc,argv);
  TH1::SetDefaultSumw2();	

  set_root_style();
  gStyle->SetOptStat(0);
  TColor::SetPalette(1,0);
  
  //
  // MAKE THE CANVAS, DRAW THE HISTOGRAM(S)
  //
  string cname   = "correlation";
  int    cwidth  = 800;
  int    cheight = 600;
  TCanvas* c = new TCanvas(cname.c_str(),cname.c_str(),cwidth,cheight);
  //c->Divide(2,2);
  if(scatter)
    c->cd()->SetRightMargin(0.2);
  //
  // CONSTRUCT RELEVANT SELECTIONS
  //
  string selection = "1";
  for (unsigned icut=0;icut<cuts.size();icut++) {
    string cut=get_cut(cuts,icut,"expression");
    if (!selection.empty()) selection += "&&";
    selection += cut;    
  }

  //
  // DATA
  //
  TH2F* hdata(0);
  TProfile* hdata_pfx(0);
  TH2F* hcanvas(0);

  if(!scatter){
  if (!datasample.empty()) {
    hdata = new TH2F("hData",htitle.c_str(),
		     nbinsx,xmin,xmax,nbinsy,ymin,ymax);
    datasample.project2d(hdata->GetName(),expression.c_str(),selection);
    hdata->SetXTitle(xtitle.c_str());
    hdata->SetYTitle(ytitle.c_str());
    hdata_pfx = hdata->ProfileX();
    hdata_pfx->SetXTitle(xtitle.c_str());
    hdata_pfx->SetYTitle(ytitle.c_str());
    hdata_pfx->SetLineWidth(3);\
    hdata_pfx->SetMarkerStyle(33);
    hdata_pfx->Draw();

    float urgh =    0;

    for (int i=1; i<=nbinsy; i++)
      {
	cout << "bin: " << i << "bin content "<< hdata_pfx->GetBinContent(45) << endl;
	urgh += hdata->GetBinContent(45,i);
      }
    cout << "Urgh: " << urgh << endl;

  }
  }
  else{
    hcanvas = new TH2F("hData",htitle.c_str(),
		       nbinsx,xmin,xmax,nbinsy,ymin,ymax);
    hcanvas->SetXTitle(xtitle.c_str());
    hcanvas->SetYTitle(ytitle.c_str());
    hcanvas->Draw("COLZ");
  }
  //
  // MCs
  //

  TH2F* hmc(0);
  TProfile* hmc_pfx(0);
  int scatcolor(2);
  if (mcs.size()>0) {
    hmc = new TH2F("hMC",htitle.c_str(),
		   nbinsx,xmin,xmax,nbinsy,ymin,ymax);
    string mcsel;
    for (unsigned icut=0;icut<mccuts.size();icut++) {
      if (!mcsel.empty()) mcsel += "&&";
      mcsel += get_cut(mccuts,icut,"expression");
    }
    if (!selection.empty())
      mcsel = (mcsel.empty()) ? selection : mcsel + "&&" + selection;
    
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample mcsample = mcsamples[imc];
      stringstream sshmcname_i; sshmcname_i<<"h"<<mcsample.name();
      mcsample.setWeights(mcweights);

      if(scatter){
	mcsample.tree(0)->SetMarkerColor(scatcolor);
	mcsample.tree(0)->Draw(expression.c_str(),selection.c_str(),"same");
	scatcolor++;
      }

      TH2F* hmc_i = new TH2F(sshmcname_i.str().c_str(),
			     htitle.c_str(),
			     nbinsx,xmin,xmax,nbinsy,ymin,ymax);
      mcsample.project2d(hmc_i->GetName(),expression.c_str(),mcsel);
      hmc->Add(hmc_i);
    }

    

    if(!scatter){
    hmc_pfx = hmc->ProfileX();
    hmc_pfx->SetXTitle(xtitle.c_str());
    hmc_pfx->SetYTitle(ytitle.c_str());
    hmc_pfx->GetYaxis()->SetTitleOffset(1.2);
    hmc_pfx->SetFillColor(kRed);
    hmc_pfx->SetFillStyle(3001);
    hmc_pfx->SetLineColor(kRed);
    hmc_pfx->SetLineWidth(3);
    hmc_pfx->GetXaxis()->SetRangeUser(xmin,xmax);
    hmc_pfx->GetYaxis()->SetRangeUser(plotymin,plotymax);
    if (!hdata_pfx) hmc_pfx->Draw("HE2");
    else {hmc_pfx->Draw("E2"); hdata_pfx->Draw("same");}
    }
    else{
      hmc->SetXTitle(xtitle.c_str());
      hmc->SetYTitle(ytitle.c_str());
      hmc->GetYaxis()->SetTitleOffset(1.2);
      hmc->GetXaxis()->SetRangeUser(xmin,xmax);
      hmc->GetYaxis()->SetRangeUser(plotymin,plotymax);
      hmc->Scale(1./hmc->Integral());
      hmc->GetZaxis()->SetRangeUser(0.,zmax);
      //hmc->Draw("COLZ");
    }
  }

  if(!scatter){
  TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax);
  l->SetFillColor(10); l->SetLineColor(10); l->SetBorderSize(0);
  l->AddEntry(hdata_pfx,"Data","lep"); 
  l->AddEntry(hmc_pfx,"MC","f");
  l->Draw("same");
  }
  //
  // DRAW LABELS
  //
  draw_labels(labels,false,false);


  //
  // SAVE PLOT TO FILE(S)
  //
  for (unsigned ifmt=0;ifmt<formats.size();ifmt++)
    c->Print((output+"."+formats[ifmt]).c_str());
  
  
  //
  // RUN ROOT APPLICATION
  //
  if (!batch) app->Run();

  return 0;
}


////////////////////////////////////////////////////////////////////////////////
// implementation of helper functions
////////////////////////////////////////////////////////////////////////////////


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
