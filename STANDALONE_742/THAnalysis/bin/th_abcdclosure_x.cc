////////////////////////////////////////////////////////////////////////////////////
//
// th_abcdclosure_x
// --------------
//
//            03/02/2014 Christian Boeser <christian.boser@cern.ch>
//          
////////////////////////////////////////////////////////////////////////////////////

#include "../interface/Histogram.h"
#include "../interface/DataSample.h"
#include "../interface/MCSample.h"
#include "../interface/MatrixMethod.h"

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
#include <TLine.h>
#include "TPluginManager.h"
#include <TRandom3.h>

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
  
  vector<string> mcs       = cl.getVector<string>  ("mcs",                 "");
  vector<string> qcds      = cl.getVector<string>  ("qcds",                "");
  string         mcref     = cl.getValue<string>   ("mcref",               "");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  string         mcsubpath = cl.getValue <string>  ("mcsubpath",           "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> a         = cl.getVector<string>  ("a",                   "");
  vector<string> b         = cl.getVector<string>  ("b",                   "");
  vector<string> c         = cl.getVector<string>  ("c",                   "");
  vector<string> d         = cl.getVector<string>  ("d",                   "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
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
  unsigned       nbinshist = cl.getValue <unsigned>("nbinshist",          100);
  double         histmin   = cl.getValue <double>  ("histmin",             -2);
  double         histmax   = cl.getValue <double>  ("histmax",             +2);
  bool           overflow  = cl.getValue <bool>    ("overflow",         false);
  bool           logx      = cl.getValue <bool>    ("logx",             false);
  bool           logy      = cl.getValue <bool>    ("logy",             false);
  double         lumi      = cl.getValue <double>  ("lumi",               0.0);
  int            npseudo   = cl.getValue <int>     ("npseudo",          1000);
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
  bool           drawbias    = cl.getValue <bool>    ("drawbias",               0);
  
  if(!cl.check()) return 0;
  cl.print();
  
  if (mcs.empty()){cout<<"provide -mcs!"<<endl;return 0;}
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
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples;
  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath+mcsubpath,filesuffix);
  if (mcsamples.size()>0) MCSample::printSampleInfos(mcsamples);
  vector<MCSample> qcdsamples;
  qcdsamples=MCSample::loadSamples(qcds,dirname,treename,mcpath+mcsubpath,filesuffix);
  if (qcdsamples.size()>0) MCSample::printSampleInfos(qcdsamples);
  

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
  

  TH1::SetDefaultSumw2();	
  
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

  //
  // tightsel: selection cuts without the 2D abcd cuts
  // a: cuts for region A
  // b: cuts for region B
  // c: cuts for region C
  // d: cuts for region D
  
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
  // MC
  //
  // mchistosX: All non-QCD mc
  // qcdhistosX: All QCD mc 
  //

  vector<TH1F*>  mchistos;
  vector<TH1F*>  mchistosa;
  vector<TH1F*>  mchistosb;
  vector<TH1F*>  mchistosc;
  vector<TH1F*>  mchistosd;
  vector<TH1F*>  qcdhistos;
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
      TH1F* hmc_i = histogram.create(sshmcname_i.str());
      TH1F* hmc_i_a = histogram.create(sshmcname_i.str()+"A");
      TH1F* hmc_i_b = histogram.create(sshmcname_i.str()+"B");
      TH1F* hmc_i_c = histogram.create(sshmcname_i.str()+"C");
      TH1F* hmc_i_d = histogram.create(sshmcname_i.str()+"D"); 
      mcsample.project(hmc_i->GetName(),histogram.expr(),tightsel);
      mcsample.project(hmc_i_a->GetName(),histogram.expr(),abcdsel[0]);
      mcsample.project(hmc_i_b->GetName(),histogram.expr(),abcdsel[1]);
      mcsample.project(hmc_i_c->GetName(),histogram.expr(),abcdsel[2]);
      mcsample.project(hmc_i_d->GetName(),histogram.expr(),abcdsel[3]);

      hmc_i->SetFillStyle(1001);
      hmc_i->SetFillColor(get_color(mccolors,mcsample.name()));
      qcdhistos.push_back(hmc_i);
      hmc_i_a->SetFillStyle(1001);
      hmc_i_a->SetFillColor(get_color(mccolors,mcsample.name()));
      qcdhistosa.push_back(hmc_i_a);
      hmc_i_b->SetFillStyle(1001);
      hmc_i_b->SetFillColor(get_color(mccolors,mcsample.name()));
      qcdhistosb.push_back(hmc_i_b);
      hmc_i_c->SetFillStyle(1001);
      hmc_i_c->SetFillColor(get_color(mccolors,mcsample.name()));
      qcdhistosc.push_back(hmc_i_c);
      hmc_i_d->SetFillStyle(1001);
      hmc_i_d->SetFillColor(get_color(mccolors,mcsample.name()));
      qcdhistosd.push_back(hmc_i_d);

    }
  }
  
  if (mcs.size()>0) {
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample     mcsample = mcsamples[imc];
      stringstream sshmcname_i; sshmcname_i<<"h"<<mcsample.name();
      mcsample.setWeights(mcweights);
      TH1F* hmc_i = histogram.create(sshmcname_i.str());
      TH1F* hmc_i_a = histogram.create(sshmcname_i.str()+"A");
      TH1F* hmc_i_b = histogram.create(sshmcname_i.str()+"B");
      TH1F* hmc_i_c = histogram.create(sshmcname_i.str()+"C");
      TH1F* hmc_i_d = histogram.create(sshmcname_i.str()+"D"); 
      mcsample.project(hmc_i->GetName(),histogram.expr(),tightsel);
      mcsample.project(hmc_i_a->GetName(),histogram.expr(),abcdsel[0]);
      mcsample.project(hmc_i_b->GetName(),histogram.expr(),abcdsel[1]);
      mcsample.project(hmc_i_c->GetName(),histogram.expr(),abcdsel[2]);
      mcsample.project(hmc_i_d->GetName(),histogram.expr(),abcdsel[3]);
      
      hmc_i->SetFillStyle(1001);
      hmc_i->SetFillColor(get_color(mccolors,mcsample.name()));
      mchistos.push_back(hmc_i);
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
  }


  
  //
  // NORMALIZE MC
  //
  if (lumi>0.0){
    for (unsigned i=0;i<mchistos.size();i++){
      mchistos[i]->Scale(lumi);
      mchistosa[i]->Scale(lumi);
      mchistosb[i]->Scale(lumi);
      mchistosc[i]->Scale(lumi);
      mchistosd[i]->Scale(lumi);
    }
    for (unsigned i=0;i<qcdhistos.size();i++){
      qcdhistos[i]->Scale(lumi);
      qcdhistosa[i]->Scale(lumi);
      qcdhistosb[i]->Scale(lumi);
      qcdhistosc[i]->Scale(lumi);
      qcdhistosd[i]->Scale(lumi);
    }
  }

  float mytmp = 0.0; 
  
   //
  // Stack mcs ... 
  //
  THStack* hmc[4]={0,0,0,0};
  float diff[4];
  float differr[4];
  float dataerr[4];
  float MCInt[4] = {0.0,0.0,0.0,0.0};
  float MCErr[4] = {0.0,0.0,0.0,0.0};
  float trueNQCD[4] = {0.0,0.0,0.0,0.0};
  float trueNQCDerr[4] = {0.0,0.0,0.0,0.0};
  float trueresult = 0.0;
  float trueresulterr = 0.0;

  float trueresultfrac[3] = {0.0,0.0,0.0};

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
      trueNQCD[0] += qcdhistosa[i]->IntegralAndError(-1, -2, error);
      trueNQCDerr[0] = sqrt(trueNQCDerr[0]*trueNQCDerr[0] + error*error);
      error = 0.0;
      trueNQCD[1] += qcdhistosb[i]->IntegralAndError(-1, -2, error);
      trueNQCDerr[1] = sqrt(trueNQCDerr[1]*trueNQCDerr[1] + error*error);
      error = 0.0;
      trueNQCD[2] += qcdhistosc[i]->IntegralAndError(-1, -2, error);
      trueNQCDerr[2] = sqrt(trueNQCDerr[2]*trueNQCDerr[2] + error*error);
      error = 0.0;
      trueNQCD[3] += qcdhistosd[i]->IntegralAndError(-1, -2, error);
      trueNQCDerr[3] = sqrt(trueNQCDerr[3]*trueNQCDerr[3] + error*error);
      error = 0.0;
    }
    
    cout << "# QCD MC: " << endl;
    cout << "# True A: " << trueNQCD[0] << " +- " <<  trueNQCDerr[0] << " | B: " << trueNQCD[1] << " +- " << trueNQCDerr[1] << " | C: " << trueNQCD[2] << " +- " << trueNQCDerr[2] << " | D: " << trueNQCD[3] << " +- " <<  trueNQCDerr[3]  << endl;
    trueresult = (float)trueNQCD[1] * trueNQCD[2] / trueNQCD[3];
    trueresulterr = (float)sqrt(pow(trueNQCDerr[1]*trueNQCD[2]/trueNQCD[3], 2) + pow(trueNQCDerr[2]*trueNQCD[1]/trueNQCD[3],2) + pow(trueNQCD[1]*trueNQCD[2]*trueNQCDerr[3]/(trueNQCD[3]*trueNQCD[3]),2));
    trueresultfrac[0] = (trueresult - trueNQCD[0])/trueNQCD[0];
    trueresultfrac[1] = (trueresult + trueresulterr-trueNQCD[0])/trueNQCD[0];
    trueresultfrac[2] = (trueresult - trueresulterr-trueNQCD[0])/trueNQCD[0];
    
  }

  TH1F* truebox= new TH1F("truebox","truebox", 1, trueresultfrac[2], trueresultfrac[1]);
  truebox->SetFillStyle(3354);
  truebox->SetFillColor(632);
  truebox->SetLineColor(0);

    // not good because of nbinsx:
    //  if((trueresult-trueresulterr)*1.1 < histmin)
    //    histmin = (trueresult-trueresulterr)*1.1;
    //  if((trueresult + trueresulterr)*1.1 > histmax)
    //    histmax = (trueresult+trueresulterr)*1.1);
  
  
  //
  // Draw 1000 pseudo datasets  for each region
  //
  
  // Filling results in histograms 
  TH1F* reshist= new TH1F("reshist","", nbinshist, histmin,histmax);
  TH1F* qcdhist= new TH1F("qcdhist","", nbinshist, histmin,histmax);
  //  reshist->SetTitle("");

  reshist->SetFillStyle(1001);
  reshist->SetFillColor(600);
  reshist->SetLineWidth(0);
  reshist->SetLineColor(600);
  reshist->GetXaxis()->SetTitle(xtitle.c_str());
  reshist->GetYaxis()->SetTitle(ytitle.c_str());

  qcdhist->SetLineWidth(2);
  qcdhist->SetLineColor(800);

  TLegend* l = new TLegend(0.68,0.68,0.93,0.93);
  l->SetFillColor(10); l->SetLineColor(10); l->SetBorderSize(0);
  l->SetHeader("rel. diff. between:");
  l->AddEntry(reshist, "#frac{BC}{D} & A", "f");
  l->AddEntry(qcdhist, "A & A_{MC}" , "f");
  if(drawbias)
    l->AddEntry(truebox, "#frac{B_{MC}C_{MC}}{D_{MC}} & A_{MC}","f");
  
  vector<float> resultABCD(0);
  vector<float> resultABCDerr(0);  

  cout << "Starting pseudo experiments ... \n" << endl;

  TRandom3 random=0;

  for(int ips=0; ips<npseudo; ips++){
    if(ips<10 || ips % 100 ==0)
      cout << "Pseudo experiment " << ips << " ... " << endl;
    
    // create new for every pseudo exp
    float pseudo[4]={0.,0.,0.,0.};   // whole pseudo data
    float pseudodiff[4]={0.,0.,0.,0.}; // whole pseudo data without QCD 
    float NQCD[4] = {0.0,0.0,0.0,0.0}; // whole QCD pseudo data 
    // NQCD[i] = pseudo[i] - pseudodiff[i];
    float NQCDerr[4] = {0.0,0.0,0.0,0.0}; // ^ error on it

    Double_t error=0.0;
    Double_t tmp=0.0;
    Double_t scale =0.0;
    for (unsigned i=0;i<mchistos.size();i++){
      // calculate yield and error for each temp before all abcd cuts:
      tmp=mchistos[i]->IntegralAndError(-1, -2, error);
      // draw random yield within uncertainties: 
      scale = random.Poisson(random.Gaus(tmp,error));
      // and calculate a ramdom scale factor which is applied to all regions abcd. 
      scale /= tmp;
      
      cout << "scale " << mchistos[i]->GetName() << " with " << scale << endl;
      
      // fill only pseudo and pseudodiff here
      pseudo[0]+= scale * mchistosa[i]->Integral();
      pseudodiff[0]+= scale * mchistosa[i]->Integral();
      pseudo[1]+= scale * mchistosb[i]->Integral();
      pseudodiff[1]+= scale * mchistosb[i]->Integral();
      pseudo[2]+= scale * mchistosc[i]->Integral();
      pseudodiff[2]+= scale * mchistosc[i]->Integral();
      pseudo[3]+= scale * mchistosd[i]->Integral();
      pseudodiff[3]+= scale * mchistosd[i]->Integral();
      error = 0.0;
      tmp=0.0;
      scale = 0.0;
    }
    
    for (unsigned i=0;i<qcdhistosa.size();i++){
      // same procedure as above
      tmp=qcdhistos[i]->IntegralAndError(-1, -2, error);
      scale = random.Poisson(random.Gaus(tmp,error));
      scale /= tmp;
      // fill only pseudo and NQCD here
      pseudo[0]+= scale *qcdhistosa[i]->Integral();
      NQCD[0] += scale * qcdhistosa[i]->Integral();
      pseudo[1]+= scale *qcdhistosb[i]->Integral();
      NQCD[1] += scale * qcdhistosb[i]->Integral();
      pseudo[2]+= scale *qcdhistosc[i]->Integral();
      NQCD[2] += scale * qcdhistosc[i]->Integral();
      pseudo[3]+= scale *qcdhistosd[i]->Integral();
      NQCD[3] += scale * qcdhistosd[i]->Integral();
      error = 0.0;
      tmp=0.0;
      scale = 0.0;
    }
    
    for(int j=0; j<4; j++){
      // calculate difference between pseudo data and MC
      // the MC to subtract is the same you would subtract in real data, ie. MCInt[i]
      diff[j] = pseudo[j]-MCInt[j];
      dataerr[j]=sqrt(pseudo[j]);
      differr[j] = sqrt(MCErr[j]*MCErr[j] + dataerr[j]*dataerr[j]);
    }

    cout << "MCInt : " << MCInt[0] << " " << MCInt[1] <<  " " << MCInt[2] << " " << MCInt[3] << endl;
    cout << "pseudo: "  << pseudo[0] << " " << pseudo[1] <<  " " << pseudo[2] << " " << pseudo[3] << endl;
    cout << "diff:   " << diff[0] << " " << diff[1] <<  " " << diff[2] << " " << diff[3] << endl;
    cout << "MCQCD:  " << NQCD[0] << " " << NQCD[1] <<  " " << NQCD[2] << " " << NQCD[3] << endl;
    
    // calculate results 
    float result = (float)diff[1] * diff[2] / diff[3];
    float resulterr = (float)sqrt(pow(differr[1]*diff[2]/diff[3], 2) + pow(differr[2]*diff[1]/diff[3],2) + pow(diff[1]*diff[2]*differr[3]/(diff[3]*diff[3]),2));
    cout << "result: " << result << endl;
    cout << "-------------" << endl;
    // fill histograms with relative differences. Here it is open to interpretation what we compare the result with: The "real" amout of QCD NQCD[i] in the pseudo data, or the yield of the QCD MC trueNQCD[i]. We take NQCD... 

    qcdhist->Fill((diff[0]-trueNQCD[0])/trueNQCD[0]);
    reshist->Fill((result-NQCD[0])/NQCD[0]);
    //    cout << "fill truebox [" << trueresultfrac[2] << "," << trueresultfrac[1] << "] with " << trueresultfrac[0] << endl;
    truebox->Fill(trueresultfrac[0]);
    
    resultABCD.push_back(result);
    resultABCDerr.push_back(resulterr);
    //    cout << result << " +- " << resulterr << endl;
  }

  // calculate mean and standard deviation
  float resmean=0.0;
  float sd=0.0;
  for(int ips=0; ips < resultABCD.size(); ips++){
    // cout << NQCD << " " <<  resultABCD.at(ips) << endl;
    resmean += resultABCD.at(ips);
  }

  for(int ips=0; ips < resultABCD.size(); ips++){
    sd += (resultABCD.at(ips) - resmean/resultABCD.size())*(resultABCD.at(ips) - resmean/resultABCD.size());
  }
  sd /= resultABCD.size();
  sd = sqrt( sd );
 
  float maximum =  reshist->GetMaximum()*1.3;
  reshist->SetMaximum(maximum);

  TLine* trueline = new TLine(trueresultfrac[0],-1,trueresultfrac[0], maximum);
  trueline->SetLineColor(632);
  trueline->SetLineWidth(2);
    
  TCanvas* c1 = new TCanvas("c1","c1", 1200,900);
  c1->cd()->SetRightMargin(0.08);

  reshist->Draw("hist");
  if(drawbias){
    truebox->Draw("histsame");
    trueline->Draw("same");
  }
  reshist->Draw("histsame");
  qcdhist->Draw("histsame");
  l->Draw("same");
  c1->RedrawAxis();

  TCanvas* c2 = new TCanvas("c2","c2", 1200,1200);
  c2->cd();
  truebox->Draw("hist");
  trueline->Draw("same");
  c2->RedrawAxis();

  for (unsigned ifmt=0;ifmt<formats.size();ifmt++){
    c1->SaveAs((output+"."+formats[ifmt]).c_str());
    c2->SaveAs("test2.pdf");
  }

  cout << "Result  " <<  resmean/resultABCD.size() << " +- " << sd << "   | true " << trueNQCD[0] << " +- " << trueNQCDerr[0] << endl;
  cout << "Result2 " <<  reshist->GetMean() << " +- " << reshist->GetRMS() << endl; 
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
