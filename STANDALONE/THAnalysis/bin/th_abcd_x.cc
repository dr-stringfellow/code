////////////////////////////////////////////////////////////////////////////////////
//
// th_abcd_x
// --------------
//
// Implements the ABCD method to estimate the total contribution from QCD multijet
//  processes, based on the event yields in data sideband regions.
//
//            03/02/2014 Christian Boeser <christian.boser@cern.ch>
//          
////////////////////////////////////////////////////////////////////////////////////

#include "../interface/Histogram.h"
#include "../interface/DataSample.h"
#include "../interface/MCSample.h"
#include "../interface/MatrixMethod.h"
#include "../interface/THStatus.h"

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
#include <THStack.h>
#include <TH1F.h>
#include <TF1.h>
#include <TEventList.h>
#include <TLatex.h>
#include <TList.h>
#include "TPluginManager.h"

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

double get_minimum(const TH1* h);
bool   get_val_and_err(double& val, double& err,const string& expr);
string get_leglabel(const vector<string>&labels,const string& sample);
int    get_color(const vector<string>& colors,const string&sample);
void   draw_stats(unsigned level,TH1* hdata,double xoffset=0.2);
void   draw_labels(const vector<string>& labels,bool leginplot=true,bool res=true);
string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression"); 
float  get_chi2test(TH1F* hh,THStack* hmc);
float  get_kstest(TH1F* hh,THStack* hmc);
void   rm_CMS(vector<string>& labels);
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
  vector<string> qcds      = cl.getVector<string>  ("qcds",                "");
  string         mcref     = cl.getValue<string>   ("mcref",               "");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  string         mcsubpath = cl.getValue <string>  ("mcsubpath",           "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> a         = cl.getVector<string>  ("a",                   "");
  vector<string> b         = cl.getVector<string>  ("b",                   "");
  vector<string> c         = cl.getVector<string>  ("c",                   "");
  vector<string> d         = cl.getVector<string>  ("d",                   "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
  vector<string> datacuts  = cl.getVector<string>  ("datacuts",            "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");
  string         tightcut  = cl.getValue <string>  ("tightcut",         "iso");
  string         epssig    = cl.getValue <string>  ("epssig", "0.99 +- 0.010");
  string         epsqcd    = cl.getValue <string>  ("epsqcd", "0.40 +- 0.025");
  string         varname   = cl.getValue <string>  ("varname",             "");
  string         varexpr   = cl.getValue <string>  ("varexpr",             "");
  string         xtitle    = cl.getValue <string>  ("xtitle",              "");
  string         ytitle    = cl.getValue <string>  ("ytitle",              "");
  unsigned       nbinsx    = cl.getValue <unsigned>("nbinsx",               0);
  vector<double> binsx     = cl.getVector<double>  ("binsx",               "");
  double         xmin      = cl.getValue <double>  ("xmin",             -1E10);
  double         xmax      = cl.getValue <double>  ("xmax",             +1E10);
  double         ymin      = cl.getValue <double>  ("ymin",               -1.);
  bool           overflow  = cl.getValue <bool>    ("overflow",         false);
  bool           fillqcd  = cl.getValue <bool>    ("fillqcd",         false);
  bool           logx      = cl.getValue <bool>    ("logx",             false);
  bool           logy      = cl.getValue <bool>    ("logy",             false);
  bool           residuals = cl.getValue <bool>    ("residuals",         true);
  double         resmax    = cl.getValue <double>  ("resmax",             -1.);
  double         lumi      = cl.getValue <double>  ("lumi",               0.0);
  double         legx      = cl.getValue <double>  ("legx",              0.70);
  double         legy      = cl.getValue <double>  ("legy",              0.94);
  double         legw      = cl.getValue <double>  ("legw",              0.20);
  double         legh      = cl.getValue <double>  ("legh",             0.055);
  string         datalabel = cl.getValue <string>  ("datalabel",       "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");
  vector<string> labels    = cl.getVector<string>  ("labels",              "");
  bool           reversemc = cl.getValue <bool>    ("reversemc",        false);
  bool           batch     = cl.getValue <bool>    ("batch",            false);
  vector<string> formats   = cl.getVector<string>  ("formats",             "");
  string         opath     = cl.getValue <string>  ("opath",               "");
  string         output    = cl.getValue <string>  ("output",              "");
  vector<string> prefix    = cl.getVector<string>  ("prefix",              "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",              "");
  bool           chi2test  = cl.getValue <bool>    ("chi2test",             0);
  bool           kstest    = cl.getValue <bool>    ("kstest",               0);
  bool           mcuncert  = cl.getValue <bool>    ("mcuncert",             0);
  bool           printres  = cl.getValue <bool>    ("printres",          true);
 
  
  if(!cl.check()) return 0;
  cl.print();
  
  if (data.empty() || mcs.empty()){cout<<"provide -data and -mcs!"<<endl;return 0;}
  if (batch&&formats.size()==0) formats.push_back("pdf");
  if (output.empty()) {
    output = (varname.empty()) ? "DataToMC" : varname;
    if (dirname!="utm") output = dirname + "_" + output;
    if (suffix.size()>0) 
      for (int i=(int)suffix.size()-1;i>=0;i--) output = output + "_" + suffix[i];
    if (prefix.size()>0)
      for (int i=(int)prefix.size()-1;i>=0;i--) output = prefix[i] + "_" + output;
    if (!opath.empty()) output = opath + "/" + output;
  }
  if (mcs.size()>0&&!reversemc) std::reverse(mcs.begin(),mcs.end());
 
  if (chi2test||kstest) rm_CMS(labels);

  
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
  vector<MCSample> qcdsamples;
  qcdsamples=MCSample::loadSamples(qcds,dirname,treename,mcpath+mcsubpath,filesuffix);
  if (qcdsamples.size()>0) MCSample::printSampleInfos(qcdsamples);
  
  int tmp_status = 1 + mcsamples.size() + qcdsamples.size();
  THStatus* status = new THStatus (tmp_status);
  
  cout << tmp_status << endl;
  //
  // INITITALIZE HISTOGRAM DATA
  //
  map<string,Histogram> name2histo = Histogram::getHistogramMap();
  Histogram histogram;
  const map<string,Histogram>::iterator it = name2histo.find(varname);
  if (it!=name2histo.end()) {
    histogram = it->second;
    if (!varexpr.empty())histogram.setExpr(varexpr);
    if (!xtitle.empty()) histogram.setXTitle(xtitle);
    if (!ytitle.empty()) histogram.setYTitle(ytitle);
    if (nbinsx!=0)       histogram.setNbinsx(nbinsx);
    if (binsx.size()>0)  histogram.setBinsx(binsx);
    if (xmin>-1E10)      histogram.setXmin(xmin);
    if (xmax< 1E10)      histogram.setXmax(xmax);
    if (logx)            histogram.setLogx(logx);
    if (logy)            histogram.setLogy(logy);
  }
  else {
    if (varexpr.empty()) { cout<<"varexpr must be set!"<<endl; return 0; }
    if (nbinsx==0)       { cout<<"nbinsx  must be set!"<<endl; return 0; }
    if (xmin<-1E09)      { cout<<"xmin    must be set!"<<endl; return 0; }
    if (xmax>+1E09)      { cout<<"xmax    must be set!"<<endl; return 0; }
    if (varname.empty())  histogram.setName("UtmDataToMC");
    else                  histogram.setName(varname);
    histogram.setExpr(varexpr);
    histogram.setXTitle(xtitle);
    histogram.setYTitle(ytitle);
    histogram.setNbinsx(nbinsx);
    histogram.setBinsx(binsx);
    histogram.setXmin(xmin);
    histogram.setXmax(xmax);
    histogram.setLogx(logx);
    histogram.setLogy(logy);
  }
  
  if (overflow) {
    stringstream ss;
    ss<<"min("<<setprecision(10)<<(histogram.xmax()-1.E-04)
      <<","<<histogram.expr()<<")";
    histogram.setExpr(ss.str());
  }
  
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
  string cname   = dirname + "_" + histogram.name();
  int    cwidth  = 800;
  int    cheight = 800;
  TCanvas* can = new TCanvas(cname.c_str(),cname.c_str(),cwidth,cheight);
  can->Divide(2,2);
  
  //
  // CONSTRUCT RELEVANT SELECTIONS
  //
  string loosesel, tightsel, loosenottightsel;
  string abcdsel[4];
  
  for (unsigned icut=0;icut<cuts.size();icut++) {
    string cut=get_cut(cuts,icut,"expression");
    if (!tightsel.empty())         tightsel         += "&&";
    if (!loosenottightsel.empty()) loosenottightsel += "&&";
    if (cut.find(tightcut)!=string::npos) {
      tightsel         += cut;
      loosenottightsel += "!(" + cut + ")";
    }
    else {
      if (!loosesel.empty()) loosesel += "&&";
      loosesel         += cut;
      tightsel         += cut;
      loosenottightsel += cut;
    }
  }
  if (!histogram.selection().empty()) {
    loosesel = (loosesel.empty()) ?
      histogram.selection():histogram.selection()+"&&"+loosesel;
    tightsel = (tightsel.empty()) ?
      histogram.selection():histogram.selection()+"&&"+tightsel;
    loosenottightsel = (loosenottightsel.empty()) ?
      histogram.selection():histogram.selection()+"&&"+loosenottightsel;
  }
  
  if (!tightsel.empty()){
    abcdsel[0] = tightsel;
    abcdsel[1] = tightsel;
    abcdsel[2] = tightsel;
    abcdsel[3] = tightsel;
  }
  
  for(unsigned icut=0;icut<a.size();icut++){
    string cut=get_cut(a,icut,"expression");
    if(!abcdsel[0].empty()) abcdsel[0]+= "&&";
    abcdsel[0] += cut;
  }
  
  for(unsigned icut=0;icut<b.size();icut++){
    string cut=get_cut(b,icut,"expression");
    if(!abcdsel[1].empty()) abcdsel[1]+= "&&";
    abcdsel[1] += cut;
  }
  
  for(unsigned icut=0;icut<c.size();icut++){
    string cut=get_cut(c,icut,"expression");
    if(!abcdsel[2].empty()) abcdsel[2]+= "&&";
    abcdsel[2] += cut;
  }
  
  for(unsigned icut=0;icut<d.size();icut++){
    string cut=get_cut(d,icut,"expression");
    if(!abcdsel[3].empty()) abcdsel[3]+= "&&";
    abcdsel[3] += cut;
  }

  //
  // LEGEND
  //
  if (!mcs.empty()) {
    legh = std::min(legh,0.33/(mcsamples.size()+2.)); 
  }
  double legxmin = legx;
  double legymin = legy; if (residuals) legymin =1.-1.33*(1-legymin);
  double legxmax = legx+legw;
  double legymax = legymin - (mcsamples.size()+1)*1.2*legh;
  if (legymax<0.5) legymax = 0.5;
  TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax);
  l->SetFillColor(10); l->SetLineColor(10); l->SetBorderSize(0);
  
  //
  // DATA
  //
  TH1F* hdata[4]={0,0,0,0};
  double minimum = (-1.!=ymin) ? ymin : (histogram.logy()) ? 0.1 : 0.0; 
  double maximum[4]={0.,0.,0.,0.};
  if (!datasample.empty()) {
    hdata[0] = histogram.create("hDataA");
    status->get_vmem(1);
    hdata[1] = histogram.create("hDataB");
    status->get_vmem(1);
    hdata[2] = histogram.create("hDataC");
    status->get_vmem(1);
    hdata[3] = histogram.create("hDataD");
    status->get_vmem(1);    
    datasample.project(hdata[0]->GetName(),histogram.expr(),abcdsel[0]);
    datasample.project(hdata[1]->GetName(),histogram.expr(),abcdsel[1]);
    datasample.project(hdata[2]->GetName(),histogram.expr(),abcdsel[2]);
    datasample.project(hdata[3]->GetName(),histogram.expr(),abcdsel[3]);

    maximum[0]=1.5*hdata[0]->GetMaximum();
    maximum[1]=1.5*hdata[1]->GetMaximum();
    maximum[2]=1.5*hdata[2]->GetMaximum();
    maximum[3]=1.5*hdata[3]->GetMaximum();
  }
  
  //
  // MC
  //
  vector<TH1F*>  mchistosa;
  vector<TH1F*>  mchistosb;
  vector<TH1F*>  mchistosc;
  vector<TH1F*>  mchistosd;
  vector<TH1F*>  qcdhistosa;
  vector<TH1F*>  qcdhistosb;
  vector<TH1F*>  qcdhistosc;
  vector<TH1F*>  qcdhistosd;
  vector<string> mcleglabels;

  if (qcds.size()>0) {
    for (unsigned imc=0;imc<qcdsamples.size();imc++) {
      MCSample     mcsample = qcdsamples[imc];
      stringstream sshmcname_i; sshmcname_i<<"h"<<mcsample.name();
      mcsample.setWeights(mcweights);
      TH1F* hmc_i_a = histogram.create(sshmcname_i.str()+"A");
      TH1F* hmc_i_b = histogram.create(sshmcname_i.str()+"B");
      TH1F* hmc_i_c = histogram.create(sshmcname_i.str()+"C");
      TH1F* hmc_i_d = histogram.create(sshmcname_i.str()+"D"); 

      //      status->print_status(1+imc);
      status->get_vmem(1);
      mcsample.project(hmc_i_a->GetName(),histogram.expr(),abcdsel[0]);
      status->get_vmem(1);
      mcsample.project(hmc_i_b->GetName(),histogram.expr(),abcdsel[1]);
      status->get_vmem(1);
      mcsample.project(hmc_i_c->GetName(),histogram.expr(),abcdsel[2]);
      status->get_vmem(1);
      mcsample.project(hmc_i_d->GetName(),histogram.expr(),abcdsel[3]);
      status->get_vmem(1);
      
      hmc_i_a->SetFillStyle(1001);
      hmc_i_a->SetFillColor(get_color(mccolors,mcsample.name()));
      if(fillqcd)
	mchistosa.push_back(hmc_i_a);
      else
	qcdhistosa.push_back(hmc_i_a);
      hmc_i_b->SetFillStyle(1001);
      hmc_i_b->SetFillColor(get_color(mccolors,mcsample.name()));
      if(fillqcd)
        mchistosb.push_back(hmc_i_b);
      else 
	qcdhistosb.push_back(hmc_i_b);
      hmc_i_c->SetFillStyle(1001);
      hmc_i_c->SetFillColor(get_color(mccolors,mcsample.name()));
      if(fillqcd)
        mchistosc.push_back(hmc_i_c);
      else
	qcdhistosc.push_back(hmc_i_c);
      hmc_i_d->SetFillStyle(1001);
      hmc_i_d->SetFillColor(get_color(mccolors,mcsample.name()));
      if(fillqcd)
        mchistosd.push_back(hmc_i_d);
      else      
	qcdhistosd.push_back(hmc_i_d);
      if(fillqcd)
	mcleglabels.push_back(get_leglabel(mclabels,mcsample.name()));
    }
    string legstyle = "f";
    if(fillqcd)
      for (int iimc=mchistosa.size()-1;iimc>=0;iimc--)
	l->AddEntry(mchistosa[iimc],mcleglabels[iimc].c_str(),legstyle.c_str());
  }
  
  if (mcs.size()>0) {
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample     mcsample = mcsamples[imc];
      stringstream sshmcname_i; sshmcname_i<<"h"<<mcsample.name();
      mcsample.setWeights(mcweights);
      TH1F* hmc_i_a = histogram.create(sshmcname_i.str()+"A");
      TH1F* hmc_i_b = histogram.create(sshmcname_i.str()+"B");
      TH1F* hmc_i_c = histogram.create(sshmcname_i.str()+"C");
      TH1F* hmc_i_d = histogram.create(sshmcname_i.str()+"D"); 

      status->print_status(1 + qcdsamples.size() + imc);
      mcsample.project(hmc_i_a->GetName(),histogram.expr(),abcdsel[0]);
      mcsample.project(hmc_i_b->GetName(),histogram.expr(),abcdsel[1]);
      mcsample.project(hmc_i_c->GetName(),histogram.expr(),abcdsel[2]);
      mcsample.project(hmc_i_d->GetName(),histogram.expr(),abcdsel[3]);
      
      hmc_i_a->SetFillStyle(1001);
      hmc_i_a->SetFillColor(get_color(mccolors,mcsample.name()));
      mchistosa.push_back(hmc_i_a);
      hmc_i_b->SetFillStyle(1001);
      hmc_i_b->SetFillColor(get_color(mccolors,mcsample.name()));
      mchistosb.push_back(hmc_i_b);
      hmc_i_c->SetFillStyle(1001);
      hmc_i_c->SetFillColor(get_color(mccolors,mcsample.name()));
      mchistosc.push_back(hmc_i_c);
      hmc_i_d->SetFillStyle(1001);
      hmc_i_d->SetFillColor(get_color(mccolors,mcsample.name()));
      mchistosd.push_back(hmc_i_d);
      mcleglabels.push_back(get_leglabel(mclabels,mcsample.name()));
    }
    string legstyle = "f";
    for (int iimc=mchistosa.size()-1;iimc>=0;iimc--)
      l->AddEntry(mchistosa[iimc],mcleglabels[iimc].c_str(),legstyle.c_str());
  }


  
  //
  // NORMALIZE MC
  //
  if (lumi>0.0){
    for (unsigned i=0;i<mchistosa.size();i++){
      mchistosa[i]->Scale(lumi);
      mchistosb[i]->Scale(lumi);
      mchistosc[i]->Scale(lumi);
      mchistosd[i]->Scale(lumi);
    }
    for (unsigned i=0;i<qcdhistosa.size();i++){
      qcdhistosa[i]->Scale(lumi);
      qcdhistosb[i]->Scale(lumi);
      qcdhistosc[i]->Scale(lumi);
      qcdhistosd[i]->Scale(lumi);
    }
  }
  if (0!=hdata[0]) {
    double Idata = hdata[0]->Integral();
    double Imc   = 0.0;
    for (unsigned i=0;i<mchistosa.size();i++) Imc += mchistosa[i]->Integral();
    if(!fillqcd){
      for (unsigned i=0;i<qcdhistosa.size();i++) Imc += qcdhistosa[i]->Integral();
    }
    for (unsigned i=0;i<mchistosa.size();i++) {
      TH1F* ha = mchistosa[i];
      TH1F* hb = mchistosb[i];
      TH1F* hc = mchistosc[i];
      TH1F* hd = mchistosd[i];
      ha->Scale(Idata/Imc);
      hb->Scale(Idata/Imc);
      hc->Scale(Idata/Imc);
      hd->Scale(Idata/Imc);
    }
    for (unsigned i=0;i<qcdhistosa.size();i++){
      TH1F* ha = qcdhistosa[i];
      TH1F* hb = qcdhistosb[i];
      TH1F* hc = qcdhistosc[i];
      TH1F* hd = qcdhistosd[i];
      ha->Scale(Idata/Imc);
      hb->Scale(Idata/Imc);
      hc->Scale(Idata/Imc);
      hd->Scale(Idata/Imc);
    }
  }
  string datadrawopt = (mchistosa.size()>0) ? "ESAME" : "E";
   
  //
  // PREPARE RESIDUALS
  //
  TPad* mainPad[4];

  for(int i=0; i<4; i++){
    mainPad[i] = (TPad*)can->cd(i+1);
    if (residuals) {
      Double_t topMargin    = mainPad[i]->GetTopMargin();
      //Double_t bottomMargin = mainPad[i]->GetBottomMargin();
      Double_t leftMargin   = mainPad[i]->GetLeftMargin();
      Double_t rightMargin  = mainPad[i]->GetRightMargin();
      
      mainPad[i]->Divide(1,2,0.01,0.0);
      
      mainPad[i]->GetPad(1)->SetFillColor(0);
      mainPad[i]->GetPad(2)->SetFillColor(0);
      mainPad[i]->GetPad(1)->SetPad(0.0,0.25,1.0,1.0);
      mainPad[i]->GetPad(2)->SetPad(0.0,0.0,1.0,0.25);
      
      mainPad[i]->cd(1);
      
      if (mainPad[i]->GetLogx()) gPad->SetLogx();
      if (mainPad[i]->GetLogy()) gPad->SetLogy();
      gPad->SetTopMargin(topMargin/0.75);
      gPad->SetLeftMargin(leftMargin);
      gPad->SetRightMargin(rightMargin);
      
      mainPad[i]->cd(2);
      if (mainPad[i]->GetLogx()) gPad->SetLogx();
      gPad->SetBottomMargin(0.375);
      gPad->SetLeftMargin(leftMargin);
      gPad->SetRightMargin(rightMargin);
      
      mainPad[i]->cd(1);
    }
  }
  
  //
  // DRAW MC (PLUS MATRIXMETHOD)
  //
  THStack* hmc[4]={0,0,0,0};

  float MCInt[4] = {0.0,0.0,0.0,0.0};
  float MCErr[4] = {0.0,0.0,0.0,0.0};
  float NQCD = 0.0;
  float NQCDerr = 0.0;

  if (mchistosa.size()>0) {
    if (0==hmc[0]) hmc[0] = new THStack("hMCa","");
    if (0==hmc[1]) hmc[1] = new THStack("hMCb","");
    if (0==hmc[2]) hmc[2] = new THStack("hMCc","");
    if (0==hmc[3]) hmc[3] = new THStack("hMCd","");

    Double_t error=0.0;
    for (unsigned i=0;i<mchistosa.size();i++){
      hmc[0]->Add(mchistosa[i]);
      MCInt[0]+= mchistosa[i]->IntegralAndError(-1, -2, error);
      MCErr[0] =  sqrt(MCErr[0]*MCErr[0] + error*error);
      error = 0.0;
      hmc[1]->Add(mchistosb[i]);
      MCInt[1]+= mchistosb[i]->IntegralAndError(-1, -2, error);
      MCErr[1] =  sqrt(MCErr[1]*MCErr[1] + error*error);
      error = 0.0;
      hmc[2]->Add(mchistosc[i]);
      MCInt[2]+= mchistosc[i]->IntegralAndError(-1, -2, error);
      MCErr[2] =  sqrt(MCErr[2]*MCErr[2] + error*error);
      error = 0.0;
      hmc[3]->Add(mchistosd[i]);
      MCInt[3]+= mchistosd[i]->IntegralAndError(-1, -2, error);
      MCErr[3] =  sqrt(MCErr[3]*MCErr[3] + error*error);
      error = 0.0;
    }

    for (unsigned i=0;i<qcdhistosa.size();i++){
      NQCD += qcdhistosa[i]->IntegralAndError(-1, -2, error);
      NQCDerr = sqrt(NQCDerr*NQCDerr + error*error);
      error = 0.0;
    }
        
    for(unsigned j=0; j<4; j++){
      mainPad[j] = (TPad*)can->cd(j+1);
      mainPad[j]->cd(1);
      hmc[j]->Draw("HIST");
      hmc[j]->GetXaxis()->SetTitle(mchistosa[0]->GetXaxis()->GetTitle());
      hmc[j]->SetMinimum(minimum);
      hmc[j]->SetMaximum(maximum[j]);
      hmc[j]->GetHistogram()->SetMinimum(minimum);
      hmc[j]->GetHistogram()->SetMaximum(maximum[j]);

      // calculate and draw mcuncertainty
      TH1F* herr[4]={0,0,0,0};
      if (mcuncert) {
	if(j==0)
	  herr[j] = (TH1F*) mchistosa[0]->Clone("herra");
	if(j==1)
	  herr[j] = (TH1F*) mchistosb[0]->Clone("herrb");
	if(j==2)
	  herr[j] = (TH1F*) mchistosc[0]->Clone("herrc");
	if(j==3)
	  herr[j] = (TH1F*) mchistosd[0]->Clone("herrd");

	for (unsigned i=1;i<mchistosa.size();i++) {
	  for (int ibin=0;ibin<=mchistosa[i]->GetNbinsX();ibin++) {
	    float val1  = herr[j]->GetBinContent(ibin);
	    float eval1 = herr[j]->GetBinError(ibin);
	    float val2 = 0.;
	    float eval2 = 0.;
	    if(j==0){
	      val2  = mchistosa[i]->GetBinContent(ibin);
	      eval2 = mchistosa[i]->GetBinError(ibin);
	    }
	    if(j==1){
	      val2  = mchistosb[i]->GetBinContent(ibin);
	      eval2 = mchistosb[i]->GetBinError(ibin);
	    }
	    if(j==2){
	      val2  = mchistosc[i]->GetBinContent(ibin);
	      eval2 = mchistosc[i]->GetBinError(ibin);
	    }
	    if(j==3){
	      val2  = mchistosd[i]->GetBinContent(ibin);
	      eval2 = mchistosd[i]->GetBinError(ibin);
	    }	    
	    herr[j]->SetBinContent(ibin,(val1+val2));
	    herr[j]->SetBinError(ibin,sqrt(eval1*eval1+eval2*eval2));
	  }
	}
	herr[j]->SetFillColor(kBlack);
	herr[j]->SetFillStyle(3245);
	herr[j]->SetMarkerStyle(1);
	herr[j]->Draw("error sameE2");
	if(j==0)
	  l->AddEntry(herr[j],"MC stat.","F");
      } // if (mcuncert)
    }
  }
    
  //
  // DRAW DATA and calculate numbers
  //
  int numdata[4];
  float nummc[4];
  float diff[4];
  float differr[4];
  float dataerr[4];
  Double_t dataerror = 0.0;
  for(int j=0; j<4; j++){
    mainPad[j] = (TPad*)can->cd(j+1);
    mainPad[j]->cd(1);
    if (hdata[j]) {
      hdata[j]->Draw(datadrawopt.c_str());
    }
    numdata[j] = hdata[j]->IntegralAndError(-1,-2,dataerror);
    nummc[j] =  MCInt[j];
    diff[j] = numdata[j] - MCInt[j];
    differr[j] = sqrt(MCErr[j]*MCErr[j] + dataerror*dataerror);
    dataerr[j]=dataerror;
    dataerror=0.0;
  }
 
  cout << "A: " <<  numdata[0] << " +- " << dataerr[0] << " | " << nummc[0] << " +- " << MCErr[0] << " | " << diff[0] << " +- " << differr[0] << endl;
  cout << "B: " <<  numdata[1] << " +- " << dataerr[1] << " | " << nummc[1] << " +- " << MCErr[1] << " | " << diff[1] << " +- " << differr[1] << endl;
  cout << "C: " <<  numdata[2] << " +- " << dataerr[2] << " | " << nummc[2] << " +- " << MCErr[2] << " | " << diff[2] << " +- " << differr[2] << endl;
  cout << "D: " <<  numdata[3] << " +- " << dataerr[3] << " | " << nummc[3] << " +- " << MCErr[3] << " | " << diff[3] << " +- " << differr[3] << endl;
  cout << "----------------------------------------------------------" <<endl;
  float result = (float)diff[1] * diff[2] / diff[3];
  float resulterr = (float)sqrt(pow(differr[1]*diff[2]/diff[3], 2) + pow(differr[2]*diff[1]/diff[3],2) + pow(diff[1]*diff[2]*differr[3]/(diff[3]*diff[3]),2));

  cout << "# N(QCD)     = " << result << " +- " << resulterr << endl;
  float result2 = (float)diff[1] * diff[2] *100.0 / (diff[3] * nummc[0]);
  float resulterr2 = 100.0*(float)sqrt(pow(differr[1]*diff[2]/(MCInt[0]*diff[3]), 2) + pow(differr[2]*diff[1]/(MCInt[0]*diff[3]),2) + pow(diff[1]*diff[2]*differr[3]/(MCInt[0]*diff[3]*diff[3]),2) + pow(diff[1]*diff[2]*MCErr[0]/(MCInt[0]*MCInt[0]*diff[3]),2));
  cout << "# Ratio      = (" << result2 << " +- " << resulterr2 << ") %" << endl;
  cout << "# MC: N(QCD) = " << NQCD << " +- " << NQCDerr << endl;
  //
  // DRAW AXIS TITLES, STATS, LEGEND, AND LABEL
  //
  //  TH1F*h=(mchistos.size()==0)?hdata:(stackmc)?(TH1F*)hmc->GetHistogram():mchistos[0];
  // if (hmc&&hdata) {
  //  h->SetXTitle(hdata->GetXaxis()->GetTitle());
  //  h->SetYTitle(hdata->GetYaxis()->GetTitle());
  // }
  //if (0!=hdata&&!ytitle.empty()) {
  //  hdata->GetYaxis()->SetTitle("");
  //  h->SetYTitle(ytitle.c_str());
  // }
  int start= 1;
  if(printres)
    start=2;
  for(int i=start; i<=4; i++){
    (TPad*)can->cd(i)->cd(1);
    l->Draw();
    draw_labels(labels,1,residuals);
  }

  (TPad*)can->cd(1)->cd(1);
  TLatex tex;
  tex.SetTextSize(0.06);
  tex.SetTextFont(42);
  tex.SetNDC(true);
  char name[50];
  char name2[50];
  sprintf(name, "N(QCD) = %.1f +- %.1f", result,resulterr);
  sprintf(name2,"Ratio  = (%2.1f +- %2.1f)%%", result2,resulterr2);
  if(printres){
    tex.DrawLatex(0.4,0.84,name);
    tex.DrawLatex(0.4,0.74,name2);
  }
  //
  // DRAW RESIDUALS
  //
  TH1F* hres[4] = {0,0,0,0};
  for(int j=0; j<4; j++){
    cout << "Draw Res " << j << endl;
    if (residuals) {
      mainPad[j] = (TPad*)can->cd(j+1);
      mainPad[j]->cd(2);
      hres[j] = (TH1F*)hdata[j]->Clone();
      hres[j]->Reset();
      hres[j]->GetYaxis()->CenterTitle();
      hres[j]->GetYaxis()->SetTitleSize(0.1);
      hres[j]->GetYaxis()->SetTitleOffset(0.55);
      hres[j]->GetYaxis()->SetLabelSize(0.1);
      hres[j]->GetYaxis()->SetNdivisions(505);
      hres[j]->GetXaxis()->SetTitleSize(0.16);
      hres[j]->GetXaxis()->SetLabelSize(0.16);
      hres[j]->GetXaxis()->SetTitleOffset(1);
      hres[j]->GetXaxis()->SetLabelOffset(0.006);
      hres[j]->GetXaxis()->SetNdivisions(505);
      hres[j]->GetXaxis()->SetTickLength(hres[j]->GetXaxis()->GetTickLength() * 3.0 );
    
      hres[j]->SetYTitle("#frac{(data-mc)}{data}");
      
      for (int ibin=1;ibin<=hres[j]->GetNbinsX();ibin++) {
	Float_t ndata  = hdata[j]->GetBinContent(ibin);
	Float_t nmc    = ((TH1*)hmc[j]->GetStack()->Last())->GetBinContent(ibin);
	Float_t edata  = hdata[j]->GetBinError(ibin);
	if (ndata>0) {
	  Float_t bin  = (ndata-nmc)/ndata;
	  Float_t err  = nmc/ndata/ndata*edata;
	  hres[j]->SetBinContent(ibin,bin);
	  hres[j]->SetBinError(ibin,err);
	}
	else {
	  hres[j]->SetBinContent(ibin,0.0);
	  hres[j]->SetBinError(ibin,0.0);
	}
      }
      Double_t max=std::max(std::abs(hres[j]->GetMinimum()),
			  std::abs(hres[j]->GetMaximum()));
      max *=1.05; max = std::min(1.0,max);
      if(resmax>0.) max = resmax;
      hres[j]->SetMinimum(-max);
      hres[j]->SetMaximum(max);
      hres[j]->SetMarkerStyle(kFullCircle);
      hres[j]->SetMarkerSize(0.5);
      gPad->SetGridy();
      
      // DRAW MC UNCERTAINTY IN RESIDUAL
      
      TH1F* herr2[4]={0,0,0,0};
      if(j==0)
	herr2[j] = (TH1F*) mchistosa[0]->Clone("herra");
      if(j==1)
	herr2[j] = (TH1F*) mchistosb[0]->Clone("herrb");
      if(j==2)
	herr2[j] = (TH1F*) mchistosc[0]->Clone("herrc");
      if(j==3)
	herr2[j] = (TH1F*) mchistosd[0]->Clone("herrd");

      if (mcuncert) {
	for (unsigned i=1;i<mchistosa.size();i++) {
	  for (int ibin=0;ibin<=mchistosa[i]->GetNbinsX();ibin++) {
	    float valA  = herr2[j]->GetBinContent(ibin);
	    float evalA = herr2[j]->GetBinError(ibin);
	    float valB = 0.;
	    float evalB = 0.;
	    if(j==0){
	      valB  = mchistosa[i]->GetBinContent(ibin);
	      evalB = mchistosa[i]->GetBinError(ibin);
	    }
	    if(j==1){
	      valB  = mchistosb[i]->GetBinContent(ibin);
	      evalB = mchistosb[i]->GetBinError(ibin);
	    }
	    if(j==2){
	      valB  = mchistosc[i]->GetBinContent(ibin);
	      evalB = mchistosc[i]->GetBinError(ibin);
	    }
	    if(j==3){
	      valB  = mchistosd[i]->GetBinContent(ibin);
	      evalB = mchistosd[i]->GetBinError(ibin);
	    }	    
	    herr2[j]->SetBinContent(ibin,(valA+valB));
	    herr2[j]->SetBinError(ibin,sqrt(evalA*evalA+evalB*evalB));
	    	    
	    if((i==mchistosa.size()-1)&&herr2[j]->GetBinContent(ibin)){
	      herr2[j]->SetBinContent(ibin,0.);
	      herr2[j]->SetBinError(ibin,(sqrt(evalA*evalA+evalB*evalB)/(valA+valB)));
	    }
	  }
	}
	
	herr2[j]->SetFillColor(kAzure-4);
	herr2[j]->SetFillStyle(3001);
	herr2[j]->SetMarkerStyle(1);
	hres[j]->Draw("EPHIST");
	herr2[j]->Draw("error sameE2");
	hres[j]->Draw("EPHIST same");
      }
      else{
	hres[j]->Draw("EPHIST");
      }
      
      //
      // LEGEND 2
      //
      if (mcuncert) {
	TLegend* l2 = new TLegend(0.175,0.83,0.275,0.93);
	l2->SetFillColor(10); l2->SetLineColor(10); l2->SetBorderSize(0);
	l2->AddEntry(herr2[0],"MC stat.","F");
	l2->Draw();
      }
    }
  }
    
  //
  // SAVE PLOT TO FILE(S)
  //
  for (unsigned ifmt=0;ifmt<formats.size();ifmt++){
    can->Print((output+"."+formats[ifmt]).c_str());
    can->cd(1)->Print((output+"_A."+formats[ifmt]).c_str());
    can->cd(2)->Print((output+"_B."+formats[ifmt]).c_str());
    can->cd(3)->Print((output+"_C."+formats[ifmt]).c_str());
    can->cd(4)->Print((output+"_D."+formats[ifmt]).c_str());
  }
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
void rm_CMS(vector<string>& labels)
{
  int cms = -1;
  for (unsigned i=0;i<labels.size();i++) 
    if (labels[i].find("CMS")!=string::npos) {cms=i;break;}
  
  if (cms>=0) {
    cout<<"Removing label: "<<labels[cms]<<" for Chi2/KS test output"<<endl;
    labels.erase(labels.begin()+cms);
  }
}

//______________________________________________________________________________
float get_chi2test(TH1F* hh,THStack* hmc)
{
  float result(-999.);
  if (0==hmc||0==hh) return result;
  TIter next(hmc->GetHists());
  TH1F* hist(0);
  TH1F* htmp = (TH1F*)hmc->GetHists()->First()->Clone("hClone");
  htmp->Reset();

  while ((hist = (TH1F*)next())) {
    if (0==hist) continue;
    htmp->Add(hist);
  }
  if (0==htmp) return result;
  result = hh->Chi2Test(htmp,"CHI2");
  delete htmp;
  return result;
}

//______________________________________________________________________________
float get_kstest(TH1F* hh,THStack* hmc)
{
  float result(-999.);
  if (0==hmc||0==hh) return result;
  TIter next(hmc->GetHists());
  TH1F* hist(0);
  TH1F* htmp = (TH1F*)hmc->GetHists()->First()->Clone("hClone");
  htmp->Reset();

  while ((hist = (TH1F*)next())) {
    if (0==hist) continue;
    htmp->Add(hist);
  }
  if (0==htmp) return result;
  result = hh->KolmogorovTest(htmp);
  delete htmp;
  return result;
}


//______________________________________________________________________________
double get_minimum(const TH1* h)
{
  double result(0.1);
  for (int ibin=1;ibin<h->GetNbinsX();ibin++) {
    double bincontent = h->GetBinContent(ibin);
    if (bincontent>0.0&&bincontent<result) result = bincontent;
  }
  return result;
}


//______________________________________________________________________________
bool get_val_and_err(double& val, double& err,const string& expr)
{
  val=0.0; err=0.0;
  string tmp(expr);
  stringstream ssval,sserr; 
  size_t pos = tmp.find('+');
  if (pos==string::npos) { ssval<<expr; ssval>>val; return true; }
  ssval<<tmp.substr(0,pos);
  pos = tmp.find('-');
  if (pos==string::npos) {cout<<"get_val_and_err ERROR!"<<endl; return false; }
  sserr<<tmp.substr(pos+1);
  ssval>>val;
  sserr>>err;
  return true;
}


//______________________________________________________________________________
void draw_stats(unsigned level,TH1* hdata,double xoffset)
{
  if (level==0) return;

  stringstream ssn;
  int ndata = (Histogram::hasVariableBinWidth(hdata)) ?
    (int)hdata->Integral("width"):(int)hdata->Integral();
  ssn<<setw(6)<<left<<"Data:"<<setw(10)<<right<<ndata;
  
  stringstream ssmean;
  ssmean<<setw(6)<<left<<"Mean:"<<setw(9)<<right<<setprecision(3)<<hdata->GetMean();
  
  stringstream ssrms;
  ssrms<<setw(6)<<left<<"RMS:"<<setw(9)<<right<<setprecision(3)<<hdata->GetRMS();

  TLatex stats;
  stats.SetNDC(true);
  stats.SetTextAlign(12);
  stats.SetTextSize(0.04);
  stats.SetTextFont(42);
  
  if (level>0) stats.DrawLatex(xoffset,0.89,ssn.str().c_str());
  if (level>1) stats.DrawLatex(xoffset,0.855,ssmean.str().c_str());
  if (level>2) stats.DrawLatex(xoffset,0.82,ssrms.str().c_str());
  
  return;
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


//______________________________________________________________________________
string  get_leglabel(const vector<string>& labels,const string& sample)
{
  for (int ilabel=(int)labels.size()-1;ilabel>=0;--ilabel) {
    string label = labels[ilabel];
    size_t pos = label.find(":"); assert(pos!=string::npos);
    if (sample==label.substr(0,pos)) return label.substr(pos+1);
  }
  return sample;
}


//______________________________________________________________________________
int get_color(const vector<string>& colors,const string& sample)
{
  for (int icolor=(int)colors.size()-1;icolor>=0;--icolor) {
    string color = colors[icolor];
    size_t pos = color.find(':'); assert(pos!=string::npos);
    string sampleName = color.substr(0,pos); color = color.substr(pos+1);
    if (sample==sampleName) {
      pos = color.find(':');
      if (pos==string::npos) {stringstream ss; ss<<color; int c; ss>>c; return c; }
      else {
	Double_t r,g,b;
	stringstream ssr,ssg,ssb;
	ssr<<color.substr(0,pos); ssr>>r; color = color.substr(pos+1);
	pos = color.find(':'); assert(pos!=string::npos);
	ssg<<color.substr(0,pos); ssg>>g;
	ssb<<color.substr(pos+1); ssb>>b;
	Int_t indexMax=gROOT->GetListOfColors()->GetSize()-1;
	while (gROOT->GetListOfColors()->At(indexMax)==0) indexMax--;
	TColor* newColor = new TColor(indexMax+1,r,g,b);
	return newColor->GetNumber();
      }
    }
  }
  return kYellow;
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
