////////////////////////////////////////////////////////////////////////////////////
//
// th_datatomc_x
// --------------
//
//            02/12/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
//            Modified by Christian Boeser, Simon Fink, Hauke Held, Benedikt Maier
////////////////////////////////////////////////////////////////////////////////////


#include "../interface/Histogram.h"
#include "../interface/DataSample.h"
#include "../interface/MCSample.h"
#include "../interface/THStatus.h"
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
float  return_sign(Float_t number);
void   rm_CMS(vector<string>& labels);
////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo","*", "TStreamerInfo","RIO", "TStreamerInfo()");
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  vector<string> data      = cl.getVector<string>  ("data",                "");
  vector<string> mcs       = cl.getVector<string>  ("mcs",                 "");
  vector<string> mcs_sysup = cl.getVector<string>  ("mcs_sysup",           "");
  vector<string> mcs_sysdown = cl.getVector<string>("mcs_sysdown",         "");
  vector<string> mcs_2     = cl.getVector<string>  ("mcs_2",               "");
  vector<string> mcs_3     = cl.getVector<string>  ("mcs_3",               "");
  vector<string> signal    = cl.getVector<string>  ("signal",              "");
  double         signalfac = cl.getValue<double>   ("signalfac",           1.);
  string         mcref     = cl.getValue<string>   ("mcref",               "");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  string         mcsubpath = cl.getValue <string>  ("mcsubpath",           "");
  string         mcpath_sysup= cl.getValue <string>("mcpath_sysup",        "");
  string         mcpath_sysdown= cl.getValue <string>("mcpath_sysdown",    "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
  vector<string> datacuts  = cl.getVector<string>  ("datacuts",            "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");
  vector<string> mcweights_2=cl.getVector<string>  ("mcweights_2",         "");
  vector<string> mcweights_3=cl.getVector<string>  ("mcweights_3",         "");
  vector<string> mcweightsup=cl.getVector<string>  ("mcweightsup",         "");
  vector<string> mcweightsdown=cl.getVector<string>("mcweightsdown",       "");
  vector<string> ignore    = cl.getVector<string>  ("ignore",              "");
  string         reslabel  = cl.getValue <string>  ("reslabel","#frac{(syst.-nom.)}{syst.}");
  string         tightcut  = cl.getValue <string>  ("tightcut",         "iso");
  string         mm        = cl.getValue <string>  ("mm",                  "");
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
  bool           drawnormalized  = cl.getValue <bool>("drawnormalized", false);
  bool           underflow = cl.getValue <bool>    ("underflow",        false);
  bool           logx      = cl.getValue <bool>    ("logx",             false);
  bool           logy      = cl.getValue <bool>    ("logy",             false);
  bool           residuals = cl.getValue <bool>    ("residuals",         true);
  double         resmax    = cl.getValue <double>  ("resmax",             -1.);
  string         fitresids = cl.getValue <string>  ("fitresids",           "");
  unsigned       stats     = cl.getValue <unsigned>("stats",                0);
  double         lumi      = cl.getValue <double>  ("lumi",               0.0);
  bool           leginplot = cl.getValue <bool>    ("leginplot",        false);
  double         legx      = cl.getValue <double>  ("legx",              0.70);
  double         legy      = cl.getValue <double>  ("legy",              0.94);
  double         legw      = cl.getValue <double>  ("legw",              0.20);
  double         legh      = cl.getValue <double>  ("legh",              0.065);
  string         datalabel = cl.getValue <string>  ("datalabel",       "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");
  vector<string> labels    = cl.getVector<string>  ("labels",              "");
  bool           stackmc   = cl.getValue <bool>    ("stackmc",           true);
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
  bool           printdiff = cl.getValue <bool>    ("printdiff",            0);
  bool           printint  = cl.getValue <bool>    ("printint",             0);
  bool           plotsys   = cl.getValue <bool>    ("plotsys",              1);
  bool           usesyssamples = cl.getValue <bool>("usesyssamples",        0);
  string         systype   = cl.getValue <string>  ("systype",             "");
  bool           oneonly   = cl.getValue <bool>    ("oneonly",             0);
  
  
  const char* sys_type = systype.c_str();

  if(!cl.check()) return 0;
  cl.print();
  
  if (data.empty()&&mcs.empty()){cout<<"provide -data and/or -mcs!"<<endl;return 0;}
  if (batch&&formats.size()==0) formats.push_back("pdf");
  if (output.empty()) {
    output = (varname.empty()) ? "DataToMC" : varname;
    if (dirname!="utm") output = dirname + "_" + output;
    if (suffix.size()>0) 
      for (int i=(int)suffix.size()-1;i>=0;i--) output = output + "_" + suffix[i];
    if (prefix.size()>0)
      for (int i=(int)prefix.size()-1;i>=0;i--) output = prefix[i] + "_" + output;
    if (!stackmc || drawnormalized) output = output + "_ns";
    if (!opath.empty()) output = opath + "/" + output;
  }
  if (mcs.size()>0&&!reversemc) std::reverse(mcs.begin(),mcs.end());
  //if ((data.empty()&&mcref.empty())||mcs.empty()||!stackmc) residuals = false;
  
  if (chi2test||kstest) rm_CMS(labels);

  //
  // EVALUATE INGNORED CUTS AND PRINT
  //
  for (unsigned i=0;i<ignore.size();i++)
    for (unsigned j=0;j<cuts.size();j++)
      if (get_cut(cuts,j,"expression").find(ignore[i])!=string::npos)
	cout<<"ignore cut: "<<get_cut(cuts,j,"label")<<endl;
  
  
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
  if (mcsamples.size()>5) leginplot=false;

  if(mcs_sysup.empty()&&mcs_sysdown.empty())
    {
      mcs_sysup=mcs;
      mcs_sysdown=mcs;
    }


  vector<MCSample> mcsamples_sysup;
  mcsamples_sysup=MCSample::loadSamples(mcs_sysup,dirname,treename,mcpath_sysup+mcsubpath,filesuffix);
  if (mcsamples_sysup.size()>0) MCSample::printSampleInfos(mcsamples_sysup);

  vector<MCSample> mcsamples_sysdown;
  mcsamples_sysdown=MCSample::loadSamples(mcs_sysdown,dirname,treename,mcpath_sysdown+mcsubpath,filesuffix);
  if (mcsamples_sysdown.size()>0) MCSample::printSampleInfos(mcsamples_sysdown);

  vector<MCSample> mcsamples_2;
  mcsamples_2=MCSample::loadSamples(mcs_2,dirname,treename,mcpath+mcsubpath,filesuffix);
  if (mcsamples_2.size()>0) MCSample::printSampleInfos(mcsamples_2);

  vector<MCSample> mcsamples_3;
  mcsamples_3=MCSample::loadSamples(mcs_3,dirname,treename,mcpath+mcsubpath,filesuffix);
  if (mcsamples_3.size()>0) MCSample::printSampleInfos(mcsamples_3);


  THStatus* status= new THStatus((int)mcsamples.size());
  
  vector<MCSample> signalmc;
  if(!signal.empty()){
    signalmc=MCSample::loadSamples(signal,dirname,treename,mcpath+mcsubpath,filesuffix);
    MCSample::printSampleInfos(signalmc);
  }
  //
  // INITITALIZE HISTOGRAM DATA
  //
  map<string,Histogram> name2histo = Histogram::getHistogramMap();
  Histogram histogram;
  const map<string,Histogram>::iterator it = name2histo.find(varname);
  
  if (drawnormalized) 
    ytitle = "Norm. to unit area";
  else
    ytitle = "Arbitrary units";


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
  
  if (underflow) {
    stringstream ss;
    ss<<"max("<<setprecision(10)<<(histogram.xmin()+1.E-04)
      <<","<<histogram.expr()<<")";
    histogram.setExpr(ss.str());
  }

  cout << "2" << endl;

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
  int    cwidth  = (leginplot) ? 800 : 1000;
  int    cheight = 800;
  TCanvas* c = new TCanvas(cname.c_str(),cname.c_str(),cwidth,cheight);
  if (!leginplot) gPad->SetRightMargin(0.18);
  if (histogram.logx()) gPad->SetLogx();
  if (histogram.logy()) gPad->SetLogy();


  //
  // CONSTRUCT RELEVANT SELECTIONS
  //
  string loosesel, tightsel, loosenottightsel;
  string datasel;
  for(unsigned icut=0;icut<datacuts.size();icut++){
    string cut=get_cut(datacuts,icut,"expression");
    if (!datasel.empty())  datasel         += "&&";
    datasel         += cut;
  }

  for (unsigned icut=0;icut<cuts.size();icut++) {
    string cut=get_cut(cuts,icut,"expression");
    bool ignore_cut(false);
    for (unsigned iign=0;iign<ignore.size();iign++)
      if (cut.find(ignore[iign])!=string::npos) ignore_cut=true;
    if (ignore_cut) continue;
    if (!tightsel.empty())         tightsel         += "&&";
    if (!loosenottightsel.empty()) loosenottightsel += "&&";
    if (cut.find(tightcut)!=string::npos) {
      if (!mm.empty()) cout<<"MM: tight cut = "<<cut<<endl;
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
  
  if (!tightsel.empty())
    datasel = (datasel.empty()) ? tightsel : datasel + "&&" + tightsel;

  //
  // LEGEND
  //
  if (!mcs.empty()) {
    legh = (leginplot) ? std::min(legh,0.33/(mcsamples.size()+2.)) : 
                         std::min(legh,0.90/(mcsamples.size()+2.)); 
  }
  double legxmin = (leginplot) ? legx : 0.825;
  double legymin = legy; if (residuals) legymin =1.-1.33*(1-legymin);
  double legxmax = (leginplot) ? legx+legw : 0.999;
  double legymax = legymin - (mcsamples.size()+2)*legh;
  if (leginplot&&legymax<0.6) legymax = 0.6;
  TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax);
  l->SetFillColor(10); l->SetLineColor(10); l->SetBorderSize(0);
  

  //
  // DATA
  //
  TH1F*  hdata(0);
  double minimum = (-1.!=ymin) ? ymin : (histogram.logy()) ? 0.1 : 0.0; 
  double maximum(0.0);
  if (!datasample.empty()) {
    hdata = histogram.create("hData");
    datasample.project(hdata->GetName(),histogram.expr(),datasel);
    if (histogram.variableBinWidth()) Histogram::divideBinContentByBinWidth(hdata);
  }
  

  //
  // MATRIX METHOD
  //
  TH1F* hmm(0);
  if (!datasample.empty()&&!mm.empty()) {
    double eps_sig, err_eps_sig; get_val_and_err(eps_sig,err_eps_sig,epssig);
    double eps_qcd, err_eps_qcd; get_val_and_err(eps_qcd,err_eps_qcd,epsqcd);
    cout<<"MM: eps_sig = "<<eps_sig<<" +/- "<<err_eps_sig<<endl;
    cout<<"MM: eps_qcd = "<<eps_qcd<<" +/- "<<err_eps_qcd<<endl;
    
    if (mm.find("E")!=string::npos) {
      hmm = histogram.create("hMM");
      TH1F* hDataLoose = histogram.create("hDataLoose");
      TH1F* hDataTight = histogram.create("hDataTight");
      datasample.project(hDataLoose->GetName(),histogram.expr(),loosesel);
      datasample.project(hDataTight->GetName(),histogram.expr(),tightsel);
      if (histogram.variableBinWidth()) {
	Histogram::divideBinContentByBinWidth(hDataLoose);
	Histogram::divideBinContentByBinWidth(hDataTight);
      }
      hmm = matrixmethod::getQcdTight(hDataLoose,hDataTight,
				      eps_qcd,eps_sig,err_eps_qcd,err_eps_sig);
      hmm->SetTitle("");
    }
    else if (mm.find("W")!=string::npos) {
      hmm = histogram.create("hMM");
      size_t pos;
      pos=epssig.find("+-"); if (pos!=string::npos) epssig=epssig.substr(0,pos);
      pos=epsqcd.find("+-"); if (pos!=string::npos) epsqcd=epsqcd.substr(0,pos);
      datasample.projectMM(hmm->GetName(),histogram.expr(),
			   loosenottightsel,tightsel,
			   epssig,epsqcd);
    }
  }
  if (!mm.empty()&&0==hmm) cout<<"mm='"<<mm<<"' invalid, MatrixMethod not applied!"<<endl;
  if (0!=hmm&&mm.find("DATA")!=string::npos) { hdata=hmm; datalabel="QCD (MM)"; }
  if (0!=hdata) {
    hdata->SetMarkerStyle(kFullCircle);
    hdata->SetMarkerSize(1.25);
    maximum = hdata->GetMaximum();
    //l->AddEntry(hdata,datalabel.c_str(),"lp");
  }
  
  
  //
  // MC
  //
  string         mcsel;
  vector<TH1F*>  mchistos;
  vector<TH1F*>  mchistos_2;
  vector<TH1F*>  mchistos_3;
  vector<TH1F*>  mchistos_up;
  vector<TH1F*>  mchistos_down;
  vector<string> mcleglabels;
  vector<string> mcleglabels_2;
  vector<string> mcleglabels_3;
  vector<TH1F*>  signalhistos;


  if (mcs.size()>0) {
    for (unsigned icut=0;icut<mccuts.size();icut++) {
      if (!mcsel.empty()) mcsel += "&&";
      mcsel += get_cut(mccuts,icut,"expression");
    }
    if (!tightsel.empty())
      mcsel = (mcsel.empty()) ? tightsel : mcsel + "&&" + tightsel;
        
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      
      MCSample     mcsample = mcsamples[imc]; // nominal histogram. the LHE weights are mcsample_up and mcsample_down (see below)
      MCSample     mcsample_2 = mcsamples_2[imc]; // separate histograms
      MCSample     mcsample_3 = mcsamples_3[imc]; // separate histograms 
      MCSample     mcsample_up = mcsamples_sysup[imc]; // shaded bands in residuals
      MCSample     mcsample_down = mcsamples_sysdown[imc]; // shaded bands in residuals
      
      cout << "Name: " << mcsample.name() << endl;

      stringstream sshmcname_i; sshmcname_i<<"h"<<mcsample.name();
     

      mcsample.setWeights(mcweights);
      if(mcweights_2.empty())
	mcsample_2.setWeights(mcweights);
      else 
	mcsample_2.setWeights(mcweights_2);
      if(mcweights_3.empty())
	mcsample_3.setWeights(mcweights);
      else
	mcsample_3.setWeights(mcweights_3);

      mcsample_up.setWeights(mcweightsup);
      mcsample_down.setWeights(mcweightsdown);

      


      TH1F* hmc_i = histogram.create(sshmcname_i.str());
      TH1F* hmc_i_2 = histogram.create(sshmcname_i.str()+"_2");
      TH1F* hmc_i_3 = histogram.create(sshmcname_i.str()+"_3");
      TH1F* hmc_i_up = histogram.create(sshmcname_i.str()+"_up");
      TH1F* hmc_i_down = histogram.create(sshmcname_i.str()+"_down");

      
      
      if(usesyssamples)
	{
	  mcsample.project(hmc_i->GetName(),histogram.expr(),mcsel);
	  mcsample_2.project(hmc_i_2->GetName(),histogram.expr(),mcsel);
	  mcsample_3.project(hmc_i_3->GetName(),histogram.expr(),mcsel);
	  mcsample_up.project(hmc_i_up->GetName(),histogram.expr(),mcsel);
	  mcsample_down.project(hmc_i_down->GetName(),histogram.expr(),mcsel);
	}
      
      status->print_status((int)imc);

      //maximum = hmc_i->GetMaximum(); 

      if (0==hdata&&mcsample.name()==mcref) {
	hdata = hmc_i;
	
	if (lumi>0.0) hdata->Scale(lumi); else hdata->Scale(1./hdata->Integral());
	hdata->SetMarkerStyle(kFullSquare);
	hdata->SetMarkerColor(get_color(mccolors,mcsample.name()));
	hdata->SetLineColor(hdata->GetMarkerColor());
	maximum = hdata->GetMaximum();
	l->AddEntry(hdata,get_leglabel(mclabels,mcsample.name()).c_str(),"lp");
      }
      else {
	hmc_i->SetFillStyle(0);
	hmc_i->SetLineWidth(2); 
	hmc_i->SetFillColor(get_color(mccolors,mcsample.name()));
	hmc_i->SetLineColor(get_color(mccolors,mcsample.name()));
	hmc_i_2->SetFillStyle(0);
	hmc_i_2->SetLineWidth(2);
	hmc_i_3->SetFillStyle(0);
	hmc_i_3->SetLineWidth(2);
	hmc_i_2->SetFillColor(get_color(mccolors,mcsample_2.name()));
        hmc_i_2->SetLineColor(get_color(mccolors,mcsample_2.name()));
	hmc_i_3->SetFillColor(get_color(mccolors,mcsample_3.name()));
        hmc_i_3->SetLineColor(get_color(mccolors,mcsample_3.name()));
	mchistos.push_back(hmc_i);
	mchistos_2.push_back(hmc_i_2);
	mchistos_3.push_back(hmc_i_3);
	mchistos_up.push_back(hmc_i_up);
	mchistos_down.push_back(hmc_i_down);
	mcleglabels.push_back(get_leglabel(mclabels,mcsample.name()));
	mcleglabels_2.push_back(get_leglabel(mclabels,mcsample_2.name()));
	if (!oneonly)
	  mcleglabels_3.push_back(get_leglabel(mclabels,mcsample_3.name()));
	
      }
    }
    string legstyle = (stackmc) ? "f" : "l";
    for (int iimc=mchistos.size()-1;iimc>=0;iimc--){
      l->AddEntry(mchistos[iimc],mcleglabels[iimc].c_str(),"l");
      l->AddEntry(mchistos_2[iimc],mcleglabels_2[iimc].c_str(),"l");
      if(!oneonly)
	l->AddEntry(mchistos_3[iimc],mcleglabels_3[iimc].c_str(),"l");
    }
  }
  
  if(!signal.empty()){
    stringstream sshsigname_i; sshsigname_i<<"hsig"<<signalmc[0].name();
    signalmc[0].setWeights(mcweights);
    TH1F* sig_i = histogram.create(sshsigname_i.str());
    signalmc[0].project(sig_i->GetName(),histogram.expr(),mcsel);
    sig_i->SetFillStyle(0);
    sig_i->SetLineColor(2);
    sig_i->SetLineWidth(2);
    signalhistos.push_back(sig_i);
    if(signalfac!=1){
      stringstream ss;
      ss<<signalfac;
      l->AddEntry(signalhistos[0],(ss.str()+"x #sigma("+get_leglabel(mclabels,signalmc[0].name())+")").c_str(),"l");    
    }
    else
      l->AddEntry(signalhistos[0],get_leglabel(mclabels,signalmc[0].name()).c_str(),"l");    
  }

  

    
  //
  // NORMALIZE MC
  //
  float integral(0);
  if (lumi>0.0){
    for (unsigned i=0;i<mchistos.size();i++) 
      {
	mchistos[i]->Scale(lumi);
	integral += mchistos[i]->Integral();
	if(drawnormalized)
	  mchistos[i]->Scale(1./mchistos[i]->Integral());
      }
    for (unsigned i=0;i<mchistos_2.size();i++) 
      {
	mchistos_2[i]->Scale(lumi);
	if(drawnormalized)
          mchistos_2[i]->Scale(1./mchistos_2[i]->Integral());
      }
    for (unsigned i=0;i<mchistos_3.size();i++) 
      {
	mchistos_3[i]->Scale(lumi);
	if(drawnormalized)
          mchistos_3[i]->Scale(1./mchistos_3[i]->Integral());
      }
    for (unsigned i=0;i<mchistos_up.size();i++) 
      {
	mchistos_up[i]->Scale(lumi);
	if(drawnormalized)
          mchistos_up[i]->Scale(1./integral);
      }
    for (unsigned i=0;i<mchistos_down.size();i++) 
      {
	mchistos_down[i]->Scale(lumi);
	if(drawnormalized)
          mchistos_down[i]->Scale(1./integral);
      }
    if(!signal.empty()){
      signalhistos[0]->Scale(signalfac*lumi);      
    }
  }
  else if (0!=hdata) {
    double Idata = hdata->Integral();
    double Imc   = 0.0;
    for (unsigned i=0;i<mchistos.size();i++) Imc += mchistos[i]->Integral();
    for (unsigned i=0;i<mchistos.size();i++) {
      TH1F* h = mchistos[i];
      h->Scale(Idata/Imc);
      TH1F* h_up = mchistos_up[i];
      h_up->Scale(Idata/Imc);
      cout << "Bla" << endl;
      TH1F* h_down = mchistos_down[i];
      h_down->Scale(Idata/Imc);
      TH1F* h_2 = mchistos_2[i];
      h_2->Scale(Idata/Imc);
      TH1F* h_3 = mchistos_3[i];
      h_3->Scale(Idata/Imc);

      if (h->GetMaximum()>maximum) maximum = h->GetMaximum();
    }    
    if(!signal.empty()){
      signalhistos[0]->Scale(signalfac*Idata/Imc);      
    }
  }
  if(printint){
    double scaledInt = 0.0;
    Int_t MCentries = 0;
    for (unsigned i=0;i<mchistos.size();i++){
      scaledInt += mchistos[i]->Integral();
      MCentries += mchistos[i]->GetEntries();
    }    
    cout << "Integral      : " << scaledInt << endl;
    cout << "Average weight: " << scaledInt/MCentries << endl;    
  }

  maximum = (histogram.logy()) ? 100.*maximum : 1.5*maximum;
  string datadrawopt = (mchistos.size()>0) ? "ESAME" : "E";
    
   
  //
  // PREPARE RESIDUALS
  //
  TPad* mainPad = (TPad*)gPad;
  if (residuals) {
    Double_t topMargin    = mainPad->GetTopMargin();
    //Double_t bottomMargin = mainPad->GetBottomMargin();
    Double_t leftMargin   = mainPad->GetLeftMargin();
    Double_t rightMargin  = mainPad->GetRightMargin();
    
    mainPad->Divide(1,2,0.01,0.0);

    mainPad->GetPad(1)->SetFillColor(0);
    mainPad->GetPad(2)->SetFillColor(0);
    mainPad->GetPad(1)->SetPad(0.0,0.25,1.0,1.0);
    mainPad->GetPad(2)->SetPad(0.0,0.0,1.0,0.25);

    mainPad->cd(1);
    
    if (mainPad->GetLogx()) gPad->SetLogx();
    if (mainPad->GetLogy()) gPad->SetLogy();
    gPad->SetTopMargin(topMargin/0.75);
    gPad->SetLeftMargin(leftMargin);
    gPad->SetRightMargin(rightMargin);
    
    mainPad->cd(2);
    if (mainPad->GetLogx()) gPad->SetLogx();
    gPad->SetBottomMargin(0.480);
    gPad->SetLeftMargin(leftMargin);
    gPad->SetRightMargin(rightMargin);
    
    mainPad->cd(1);
  }
  
  //
  // PREPARE DATA
  //  
  if (hdata) {
    if (!stackmc&&hdata->GetEntries()>0) hdata->Scale(1.0/hdata->Integral());
  }

  //
  // DRAW MC (PLUS MATRIXMETHOD)
  //
  THStack* hmc(0);
  THStack* hmc_2(0);
  THStack* hmc_3(0);
  THStack* hmc_up(0);
  THStack* hmc_down(0);

  if (0!=hmm&&mm.find("MC")!=string::npos) {
    if (stackmc) {
      hmm->SetFillStyle(1001);
      hmm->SetFillColor(kMagenta+2);
      hmc = new THStack("hMC","");
      hmc->Add(hmm);
      l->AddEntry(hmm,"QCD (MM)","f");
      if (0==mchistos.size()) {
	hmc->Draw("HIST");
	hmc->GetXaxis()->SetTitle(mchistos[0]->GetXaxis()->GetTitle());
	hmc->SetMinimum(minimum);
	hmc->SetMaximum(maximum);
	hmc->GetHistogram()->SetMinimum(minimum);
	hmc->GetHistogram()->SetMaximum(maximum);
	if (chi2test&&hdata!=0) {
	  stringstream sstest;
	  sstest<<((chi2test&&kstest) ? "0.62" : "0.77")
		<<":0.96:42:#chi^{2}="
		<<setprecision(4)<<get_chi2test(hdata,hmc);
	  labels.push_back(sstest.str());
	}
	if (kstest&&hdata!=0) {
	  stringstream sstest;
	  sstest<<"0.77:0.96:42:KS="
		<<setprecision(3)<<get_kstest(hdata,hmc);
	  labels.push_back(sstest.str());
	}
      }
    }
    else {
      if (hmm->GetEntries()>0) hmm->Scale(1./hmm->Integral());
      hmm->SetMarkerStyle(kOpenCircle);
      hmm->SetMarkerSize(1.25);
      if (hmm->GetMaximum()>maximum) maximum = hmm->GetMaximum();
      if (get_minimum(hmm) <minimum) minimum = get_minimum(hmm);
      hmm->Draw("EP");
      l->AddEntry(hmm,"QCD (MM)","p");
      if (chi2test&&hdata!=0) {
	stringstream sstest;
	sstest<<((chi2test&&kstest) ? "0.62" : "0.77")
	      <<":0.96:42:#chi^{2}="
	      <<setprecision(4)<<hdata->Chi2Test(hmm,"CHI2");
	labels.push_back(sstest.str());
      }
      if (kstest&&hdata!=0) {
	stringstream sstest;
	sstest<<"0.77:0.96:42:KS="
	      <<setprecision(3)<<hdata->KolmogorovTest(hmm);
	labels.push_back(sstest.str());
      }
    }
  }
  float MCInt = 0.0;

  if (mchistos.size()>0) {
    if (stackmc) {
      if (0==hmc) hmc = new THStack("hMC","");
      for (unsigned i=0;i<mchistos.size();i++){
	hmc->Add(mchistos[i]);
	MCInt+= mchistos[i]->Integral();
      }
      if (0==hmc_2) hmc_2 = new THStack("hmc_2","hmc_2");
      for (unsigned i=0;i<mchistos_2.size();i++) hmc_2->Add(mchistos_2[i]);
      if (0==hmc_3) hmc_3 = new THStack("hmc_3","hmc_3");
      for (unsigned i=0;i<mchistos_3.size();i++) hmc_3->Add(mchistos_3[i]);

      if (0==hmc_up) hmc_up = new THStack("hmc_up","hmc_up");
      for (unsigned i=0;i<mchistos_up.size();i++) hmc_up->Add(mchistos_up[i]);
      if (0==hmc_down) hmc_down = new THStack("hmc_down","hmc_down");
      for (unsigned i=0;i<mchistos_down.size();i++) hmc_down->Add(mchistos_down[i]);

      
      maximum = (histogram.logy()) ? 50.*hmc->GetMaximum():1.5*hmc->GetMaximum();
      hmc->SetMaximum(1.5*maximum);
      hmc->Draw("HIST");
      hmc_2->Draw("HIST same");
      if (!oneonly)
	hmc_3->Draw("HIST same");
      hmc->Draw("HIST same");
      hmc->GetXaxis()->SetTitle(mchistos[0]->GetXaxis()->GetTitle());
      hmc->SetMinimum(minimum);
      hmc->SetMaximum(maximum);
      hmc->GetHistogram()->SetMinimum(minimum);
      hmc->GetHistogram()->SetMaximum(maximum);
      if (chi2test&&hdata!=0) {
	stringstream sstest;
	sstest<<((chi2test&&kstest) ? "0.62" : "0.77")
	      <<":0.96:42:#chi^{2}="
	      <<setprecision(4)<<get_chi2test(hdata,hmc);
	labels.push_back(sstest.str());
      }
      if (kstest&&hdata!=0) {
	stringstream sstest;
	sstest<<"0.77:0.96:42:KS="
	      <<setprecision(3)<<get_kstest(hdata,hmc);
	labels.push_back(sstest.str());
      }
      // calculate and draw mcuncertainty
      if (mcuncert) {
	TH1F* herr = (TH1F*) mchistos[0]->Clone("herr");
	for (unsigned i=1;i<mchistos.size();i++) {
	  for (int ibin=0;ibin<=mchistos[i]->GetNbinsX();ibin++) {
	    float val1  = herr->GetBinContent(ibin);
	    float eval1 = herr->GetBinError(ibin);
	    float val2  = mchistos[i]->GetBinContent(ibin);
	    float eval2 = mchistos[i]->GetBinError(ibin);
	    herr->SetBinContent(ibin,(val1+val2));
	    herr->SetBinError(ibin,sqrt(eval1*eval1+eval2*eval2));
	  }
	}
	if (0!=hmm) {
	  for (int ibin=0;ibin<herr->GetNbinsX();ibin++) {
	    float val1  = herr->GetBinContent(ibin);
	    float eval1 = herr->GetBinError(ibin);
	    float val2  = hmm ->GetBinContent(ibin);
	    float eval2 = hmm ->GetBinError(ibin);
	    herr->SetBinContent(ibin,(val1+val2));
	    herr->SetBinError(ibin,sqrt(eval1*eval1+eval2*eval2));
	  }
	}
	herr->SetFillColor(kBlack);
	herr->SetFillStyle(3245);
	herr->SetMarkerStyle(1);
	

      } // if (mcuncert)
    }
    else {
      maximum=0.0;
      for (unsigned i=0;i<mchistos.size();i++) {
	TH1F* h = mchistos[i];
	string drawopt = (0!=hmm) ? "HISTSAME" : (i==0) ? "HIST" : "HISTSAME";
	if (h->GetEntries()>0) h->Scale(1.0/h->Integral());
	if (h->GetMaximum()>maximum) maximum=h->GetMaximum();
	if (get_minimum(h) <minimum) minimum=get_minimum(h);
	h->SetLineColor(h->GetFillColor());
	h->SetLineWidth(3);
	h->SetFillStyle(0);
	h->Draw(drawopt.c_str());
      }
      maximum = (histogram.logy()) ? 50.*maximum : 1.5*maximum;
      if (!histogram.logy()) mchistos[0]->SetMinimum(0.0);
      else if (-1.0!=ymin)   mchistos[0]->SetMinimum(ymin);
      else if (minimum>0.0)  mchistos[0]->SetMinimum(minimum);
      TH1* h = (0!=hmm) ? hmm : mchistos[0]; h->SetMaximum(maximum);
    }
  }
  
  if(!signal.empty()){
    signalhistos[0]->Draw("HISTSAME");
  }
  
  //
  // DRAW DATA
  //
  if (hdata) {
    //    if (!stackmc&&hdata->GetEntries()>0) hdata->Scale(1.0/hdata->Integral()); now earlier for Chi2 (Hadatake)
    //hdata->Draw(datadrawopt.c_str());
    if (mchistos.size()==0) hdata->SetMaximum(maximum);
  }
  if(printdiff){
    cout << "N_Data = " << hdata->Integral() << endl;
    cout << "N_MC = " << MCInt << endl;
    cout << "! N_Data - N_MC = " << hdata->Integral() - MCInt<< endl;
  }
  
  //
  // DRAW AXIS TITLES, STATS, LEGEND, AND LABEL
  //
  if (stats&&hdata) draw_stats(stats,hdata);
  TH1F*h=(mchistos.size()==0)?hdata:(stackmc)?(TH1F*)hmc->GetHistogram():mchistos[0];
  cout << "Here"<<endl;
  if (hmc&&hdata) {
    h->SetXTitle(hdata->GetXaxis()->GetTitle());
    h->SetYTitle(hdata->GetYaxis()->GetTitle());
  }
  if (!ytitle.empty()) {
    if(0!=hdata) hdata->GetYaxis()->SetTitle("");
    h->SetYTitle(ytitle.c_str());
  }
  l->Draw();
  draw_labels(labels,leginplot,residuals);


  //
  // DRAW RESIDUALS
  //
  if (residuals) {
    mainPad->cd(2);
    TH1F* hres_2 = (TH1F*)hdata->Clone();
    TH1F* hres_3 = (TH1F*)hdata->Clone();
    TH1F* hsysup = (TH1F*)hdata->Clone();
    TH1F* hsysdown = (TH1F*)hdata->Clone();
    TH1F* herrges = (TH1F*)hdata->Clone();
    TH1F* herrges2 = (TH1F*)hdata->Clone();
    TH1F* herrgesup = (TH1F*)hdata->Clone();
    TH1F* herrgesdown = (TH1F*)hdata->Clone();
    TH1F* hsysstat = (TH1F*)hdata->Clone();

    hres_2->Reset();
    hres_3->Reset();
    hsysup->Reset();
    hsysdown->Reset();
    herrges->Reset();
    herrges2->Reset();
    herrgesup->Reset();
    herrgesdown->Reset();
    hsysstat->Reset();

    hres_2->GetYaxis()->CenterTitle();
    hres_2->GetYaxis()->CenterTitle();
    hres_2->GetYaxis()->SetTitleSize(0.10);
    hres_2->GetYaxis()->SetTitleOffset(0.72);
    hres_2->GetYaxis()->SetLabelSize(0.16);
    hres_2->GetYaxis()->SetNdivisions(505);
    hres_2->GetXaxis()->SetTitleSize(0.19);
    hres_2->GetXaxis()->SetLabelSize(0.19);
    hres_2->GetXaxis()->SetTitleOffset(1);
    hres_2->GetXaxis()->SetLabelOffset(0.006);
    hres_2->GetXaxis()->SetNdivisions(505);

    //hres_2->GetYaxis()->SetTitleSize(0.1);
    //hres_2->GetYaxis()->SetTitleOffset(0.55);
    //hres_2->GetYaxis()->SetLabelSize(0.1);
    //hres_2->GetYaxis()->SetNdivisions(505);
    //hres_2->GetXaxis()->SetTitleSize(0.16);
    //hres_2->GetXaxis()->SetLabelSize(0.16);
    //hres_2->GetXaxis()->SetTitleOffset(1);
    //hres_2->GetXaxis()->SetLabelOffset(0.006);
    //hres_2->GetXaxis()->SetNdivisions(505);
    hres_2->GetXaxis()->SetTickLength(hres_2->GetXaxis()->GetTickLength() * 3.0 );
    
    hres_2->SetYTitle(reslabel.c_str());
    
    for (int ibin=1;ibin<=hres_2->GetNbinsX();ibin++) {
      Float_t ndata  = hdata->GetBinContent(ibin);
      Float_t nmc    = ((TH1*)hmc->GetStack()->Last())->GetBinContent(ibin);
      Float_t nmc_2  = ((TH1*)hmc_2->GetStack()->Last())->GetBinContent(ibin);
      Float_t nmc_3  = ((TH1*)hmc_3->GetStack()->Last())->GetBinContent(ibin);
      Float_t nmc_up = ((TH1*)hmc_up->GetStack()->Last())->GetBinContent(ibin);
      Float_t nmc_down = ((TH1*)hmc_down->GetStack()->Last())->GetBinContent(ibin);
      Float_t edata  = hdata->GetBinError(ibin);
      if (nmc_2!=0 && nmc_3!=0) {
	Float_t bin  = (nmc_2-nmc)/nmc;
	Float_t bin2 = (nmc_3-nmc)/nmc;  
	//Float_t err  = nmc/ndata/ndata*edata;
	hres_2->SetBinContent(ibin,bin);
	hres_2->SetBinError(ibin,0);
	hres_3->SetBinContent(ibin,bin2);
	hres_3->SetBinError(ibin,0);
	hsysup->SetBinContent(ibin,(nmc_up-nmc)/nmc);
        hsysup->SetBinError(ibin,0.0);
	hsysdown->SetBinContent(ibin,(nmc_down-nmc)/nmc);
        hsysdown->SetBinError(ibin,0.0);
	herrges->SetBinContent(ibin,(hsysdown->GetBinContent(ibin)-hsysup->GetBinContent(ibin))/2+hsysup->GetBinContent(ibin));
	herrges->SetBinError(ibin,abs((hsysdown->GetBinContent(ibin)-hsysup->GetBinContent(ibin)))/2);

	if(return_sign(hsysdown->GetBinContent(ibin))==return_sign(hsysup->GetBinContent(ibin)))
	  {
	    if(abs(hsysdown->GetBinContent(ibin))>abs(hsysup->GetBinContent(ibin)))
	      {
		herrges2->SetBinContent(ibin,hsysdown->GetBinContent(ibin)/2);
		herrges2->SetBinError(ibin,hsysdown->GetBinContent(ibin)/2);
	      }
	    else
	      {
		herrges2->SetBinContent(ibin,hsysup->GetBinContent(ibin)/2);
		herrges2->SetBinError(ibin,hsysup->GetBinContent(ibin)/2);
	      }


	  }
	else
	  {
	    herrges2->SetBinContent(ibin,(hsysdown->GetBinContent(ibin)-hsysup->GetBinContent(ibin))/2+hsysup->GetBinContent(ibin));
	    herrges2->SetBinError(ibin,abs((hsysdown->GetBinContent(ibin)-hsysup->GetBinContent(ibin)))/2);
	  }
      }
      else if(nmc_3!=0 && nmc_2==0){
	hres_2->SetBinContent(ibin,0.0);
	hres_2->SetBinError(ibin,0.0);
	hsysup->SetBinContent(ibin,0.0);
        hsysup->SetBinError(ibin,0.0);
	hsysdown->SetBinContent(ibin,0.0);
        hsysdown->SetBinError(ibin,0.0);
	herrges->SetBinContent(ibin,0.0);
	herrges->SetBinError(ibin,0.0);
	herrges2->SetBinContent(ibin,0.0);
	herrges2->SetBinError(ibin,0.0);
      }
      else if(nmc_2!=0 && nmc_3==0){
        hres_3->SetBinContent(ibin,0.0);
        hres_3->SetBinError(ibin,0.0);
        hsysup->SetBinContent(ibin,0.0);
        hsysup->SetBinError(ibin,0.0);
        hsysdown->SetBinContent(ibin,0.0);
        hsysdown->SetBinError(ibin,0.0);
        herrges->SetBinContent(ibin,0.0);
        herrges->SetBinError(ibin,0.0);
        herrges2->SetBinContent(ibin,0.0);
        herrges2->SetBinError(ibin,0.0);
      }
      else{
	hres_2->SetBinContent(ibin,0.0);
        hres_2->SetBinError(ibin,0.0);
        hres_3->SetBinContent(ibin,0.0);
        hres_3->SetBinError(ibin,0.0);
        hsysup->SetBinContent(ibin,0.0);
        hsysup->SetBinError(ibin,0.0);
        hsysdown->SetBinContent(ibin,0.0);
        hsysdown->SetBinError(ibin,0.0);
        herrges->SetBinContent(ibin,0.0);
        herrges->SetBinError(ibin,0.0);
        herrges2->SetBinContent(ibin,0.0);
        herrges2->SetBinError(ibin,0.0);
      }

    }
    Double_t max=std::max(std::abs(hres_2->GetMinimum()),
			  std::abs(hres_2->GetMaximum()));
    max *=1.05; max = std::min(49.9,max);
    if(resmax>0.) max = resmax;
    hres_2->SetMinimum(-max);
    hres_2->SetMaximum(1.2*max);
    hres_2->SetMarkerStyle(23);
    hres_2->SetMarkerSize(0);
    hres_3->SetMarkerStyle(23);
    hres_3->SetMarkerSize(0);
    gPad->SetGridy();


    


    // DRAW MC UNCERTAINTY IN RESIDUAL

    TH1F* herr2 = (TH1F*) mchistos[0]->Clone("herr");
    if (mcuncert) {
      //TH1F* herr2 = (TH1F*) mchistos[0]->Clone("herr");
      for (unsigned i=1;i<mchistos.size();i++) {
	for (int ibin=1;ibin<=mchistos[i]->GetNbinsX();ibin++) {
	  float valA  = herr2->GetBinContent(ibin);
	  float evalA = herr2->GetBinError(ibin);
	  float valB  = mchistos[i]->GetBinContent(ibin);
	  float evalB = mchistos[i]->GetBinError(ibin);
	  herr2->SetBinContent(ibin,(valA+valB));
	  herr2->SetBinError(ibin,sqrt(evalA*evalA+evalB*evalB));
	  
	  
	  if((i==mchistos.size()-1)&&herr2->GetBinContent(ibin)){
	    herr2->SetBinContent(ibin,0.);
	    herr2->SetBinError(ibin,(sqrt(evalA*evalA+evalB*evalB)/(valA+valB)));
	  }
	}
      }
      
      /*
      
      for (int ibin=1;ibin<=hres_2->GetNbinsX();ibin++) {
	Float_t staterr = herr2->GetBinError(ibin); 
	Float_t syserr_up = (hsysup->GetBinContent(ibin)>hsysdown->GetBinContent(ibin))? hsysup->GetBinContent(ibin) : hsysdown->GetBinContent(ibin);
	Float_t syserr_down = (hsysdown->GetBinContent(ibin)<hsysup->GetBinContent(ibin))? hsysdown->GetBinContent(ibin): hsysup->GetBinContent(ibin);
	
	if(syserr_up<0)
	  syserr_up = 0;
	if(syserr_down>0)
	  syserr_down = 0;
	
	if(staterr==0){
	  herrgesup->SetBinContent(ibin, 0.0);
	  herrgesdown->SetBinContent(ibin, 0.0);
	}
	else{
	  herrgesup->SetBinContent(ibin, 0.0 + sqrt(staterr*staterr+syserr_up*syserr_up));
	  herrgesdown->SetBinContent(ibin, 0.0 - sqrt(staterr*staterr+syserr_down*syserr_down));
	}

      }
      */

      for (int ibin=1;ibin<=hres_2->GetNbinsX();ibin++) {
      
	Float_t staterr = herr2->GetBinError(ibin); 
	//Float_t syserr_up = herrges2->GetBinContent(ibin)+herrges2->GetBinError(ibin);
	//Float_t syserr_down = herrges2->GetBinContent(ibin)-herrges2->GetBinError(ibin);

	hsysstat->SetBinContent(ibin,herrges2->GetBinContent(ibin));
	//hsysstat->SetBinError(ibin,sqrt(herrges2->GetBinError(ibin)*herrges2->GetBinError(ibin)+staterr*staterr));
	hsysstat->SetBinError(ibin,sqrt(herrges2->GetBinError(ibin)*herrges2->GetBinError(ibin)));

      }
      


      herrges->SetLineWidth(0);
      herrges->SetFillColor(kPink-7);
      herrges->SetFillStyle(3001);
      herrges->SetMarkerSize(0);
      herrges2->SetLineWidth(0);
      herrges2->SetFillColor(kPink-7);
      herrges2->SetFillStyle(3001);
      herrges2->SetMarkerSize(0);
      
      hsysstat->SetLineWidth(1);
      hsysstat->SetFillColor(kPink-7);
      hsysstat->SetFillStyle(3003);
      hsysstat->SetMarkerSize(0);
          
      herr2->SetFillColor(kAzure-4);
      herr2->SetFillStyle(3002);
      herr2->SetMarkerStyle(1);
      hres_2->SetLineWidth(2);
      hres_2->SetLineColor(600);
      if(oneonly)
	hres_2->SetLineColor(854);
      hres_3->SetLineColor(419);
      hres_3->SetLineWidth(2);
      hres_2->Draw("HIST");
      if(!oneonly)
	hres_3->Draw("HIST same");
 
      
      if(plotsys || usesyssamples){
	
	if(plotsys) 
	  hsysstat->Draw("error sameE2");
	//herr2->Draw("error sameE2");
	//herrges->Draw("error sameE2");
	//herrges2->Draw("error sameE2");

      }

      hres_2->Draw("EPHIST same");
      if(!oneonly)
	hres_3->Draw("EPHIST same");

    }

    else{
      hres_2->Draw("EPHIST");
    }

    //
    // LEGEND 2
    //
    if (mcuncert) {
      
      if(plotsys || usesyssamples){
	
	TLegend* l2 = new TLegend(0.825,0.73,0.9575,0.93);
	l2->SetFillColor(10); l2->SetLineColor(10); l2->SetBorderSize(0); l2->SetTextSize(0.125);
	//l2->AddEntry(herr2,"MC stat.","F");
	l2->AddEntry(hsysstat,sys_type,"F");
	if(plotsys)
	  l2->Draw();
      }
      else{
	TLegend* l2 = new TLegend(0.8275,0.83,0.9275,0.93);
	l2->SetFillColor(10); l2->SetLineColor(10); l2->SetBorderSize(0);
	l2->AddEntry(herr2,"MC stat.","F");
	//l2->Draw();
      }
    }

    if (!fitresids.empty()) {
      TF1* fResiduals = new TF1("fResiduals",fitresids.c_str(),
				hres_2->GetXaxis()->GetXmin(),
				hres_2->GetXaxis()->GetXmax());
      fResiduals->SetLineWidth(1);
      fResiduals->SetLineColor(hres_2->GetMarkerColor());
      hres_2->Fit(fResiduals,"Q");
      cout<<"Fit to Residuals ("<<fitresids<<"):"<<endl;
      for (Int_t iPar=0;iPar<fResiduals->GetNpar();iPar++)
	cout<<"iPar="<<iPar
	    <<": "<<fResiduals->GetParameter(iPar)
	    <<" +/- "<<fResiduals->GetParError(iPar)<<endl;
      hres_2->SetYTitle("#frac{(ref-mc)}{ref} [%]");
      cout<<"=> K-Factor = "<<1./(1.-0.01*fResiduals->GetParameter(0))<<endl;
    }
  }
  c->cd();

  TPad* b = new TPad("b", "b",0.10501342,0.246621,0.1401047,0.2814685);
  b->SetBorderMode(0);
  //b->SetFillColor(kBlue);                                                                                                                                                                                 
  b->Draw();

  
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
      size_as_str = "0.05";
      text = tmp;
    }
    else {
      font_as_str = tmp.substr(0,pos); tmp = tmp.substr(pos+1);
      pos = tmp.find(':');
      if (pos==string::npos) {
	size_as_str = "0.05";
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

//_____________________________________________________________________________
float return_sign(Float_t number)
{
  float sign=0;
  if(number<0.)
    sign = -1;
  else if (number>0.)
    sign = 1;

  return sign;

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
