////////////////////////////////////////////////////////////////////////////////
//
// th_sigtoback_x
// --------------
//
//            02/12/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


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
  string         mcref     = cl.getValue<string>   ("mcref",               "");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  string         mcsubpath = cl.getValue <string>  ("mcsubpath",           "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");
  vector<string> ignore    = cl.getVector<string>  ("ignore",              "");
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
  bool           logx      = cl.getValue <bool>    ("logx",             false);
  bool           logy      = cl.getValue <bool>    ("logy",             false);
  bool           residuals = cl.getValue <bool>    ("residuals",         true);
  string         fitresids = cl.getValue <string>  ("fitresids",           "");
  unsigned       stats     = cl.getValue <unsigned>("stats",                0);
  double         lumi      = cl.getValue <double>  ("lumi",               0.0);
  bool           leginplot = cl.getValue <bool>    ("leginplot",         true);
  double         legx      = cl.getValue <double>  ("legx",              0.70);
  double         legy      = cl.getValue <double>  ("legy",              0.94);
  double         legw      = cl.getValue <double>  ("legw",              0.20);
  double         legh      = cl.getValue <double>  ("legh",             0.055);
  string         datalabel = cl.getValue <string>  ("datalabel",       "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");
  vector<string> labels    = cl.getVector<string>  ("labels",              "");
  bool           stackmc   = cl.getValue <bool>    ("stackmc",          false);
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
    if (!stackmc) output = output + "_ns";
    if (!opath.empty()) output = opath + "/" + output;
  }
  if (mcs.size()>0&&!reversemc) std::reverse(mcs.begin(),mcs.end());
  if ((data.empty()&&mcref.empty())||mcs.empty()||!stackmc) residuals = false;
  
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
  
  Histogram histogram_tot;
  const map<string,Histogram>::iterator it_tot = name2histo.find(varname);
  if (it_tot!=name2histo.end()) {
    histogram_tot = it->second;
    if (!varexpr.empty())histogram_tot.setExpr(varexpr);
    if (!xtitle.empty()) histogram_tot.setXTitle(xtitle);
    if (!ytitle.empty()) histogram_tot.setYTitle(ytitle);
    if (nbinsx!=0)       histogram_tot.setNbinsx(nbinsx);
    if (binsx.size()>0)  histogram_tot.setBinsx(binsx);
    if (xmin>-1E10)      histogram_tot.setXmin(-100000);
    if (xmax< 1E10)      histogram_tot.setXmax(100000);
    if (logx)            histogram_tot.setLogx(logx);
    if (logy)            histogram_tot.setLogy(logy);
  }
  else {
    if (varexpr.empty()) { cout<<"varexpr must be set!"<<endl; return 0; }
    if (nbinsx==0)       { cout<<"nbinsx  must be set!"<<endl; return 0; }
    if (xmin<-1E09)      { cout<<"xmin    must be set!"<<endl; return 0; }
    if (xmax>+1E09)      { cout<<"xmax    must be set!"<<endl; return 0; }
    if (varname.empty())  histogram_tot.setName("UtmDataToMC");
    else                  histogram_tot.setName(varname);
    histogram_tot.setExpr(varexpr);
    histogram_tot.setXTitle(xtitle);
    histogram_tot.setYTitle(ytitle);
    histogram_tot.setNbinsx(nbinsx);
    histogram_tot.setBinsx(binsx);
    histogram_tot.setXmin(-100000);
    histogram_tot.setXmax(100000);
    histogram_tot.setLogx(logx);
    histogram_tot.setLogy(logy);
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
  
  //
  // Add KIT_green & KIT red
  //

  Int_t ci_green = 6667; // color index
  Int_t ci_red   = 6666;
  TColor *color_green = new TColor(ci_green, 0,151,130);
  TColor *color_red   = new TColor(ci_red,155,29,45);


  
  //
  // LEGEND
  //
  if (!mcs.empty()) {
    legh = (leginplot) ? std::min(legh,0.33/(mcsamples.size()+2.)) : 
                         std::min(legh,0.90/(mcsamples.size()+2.)); 
  }
  double legxmin = (leginplot) ? legx : 0.825;
  double legymin = legy; if (residuals) legymin =1.-1.33*(1-legymin);
  double legxmax = (leginplot) ? legx+legw : 0.995;
  double legymax = legymin - (mcsamples.size()+1)*legh;
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
    datasample.project(hdata->GetName(),histogram.expr(),tightsel);
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
    l->AddEntry(hdata,datalabel.c_str(),"lp");
  }
  
  
  //
  // MC
  //
  string         mcsel;
  vector<TH1F*>  mchistos;
  vector<TH1F*>  tothistos;
  vector<string> mcleglabels;
  
  if (mcs.size()>0) {
    for (unsigned icut=0;icut<mccuts.size();icut++) {
      if (!mcsel.empty()) mcsel += "&&";
      mcsel += get_cut(mccuts,icut,"expression");
    }
    if (!tightsel.empty())
      mcsel = (mcsel.empty()) ? tightsel : mcsel + "&&" + tightsel;
    
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample     mcsample = mcsamples[imc];
      stringstream sshmcname_i; sshmcname_i<<"h"<<mcsample.name();
      stringstream sshmcname_tot; sshmcname_tot<<"htot"<<mcsample.name();
      mcsample.setWeights(mcweights);
      TH1F* hmc_i = histogram.create(sshmcname_i.str());
      mcsample.project(hmc_i->GetName(),histogram.expr(),mcsel);
      
      
      TH1F* forscaling = histogram_tot.create(sshmcname_tot.str()); 
      
      mcsample.project(forscaling->GetName(),histogram_tot.expr(),mcsel);

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
	hmc_i->SetFillStyle(1001);
	hmc_i->SetFillColor(get_color(mccolors,mcsample.name()));
	mchistos.push_back(hmc_i);
	tothistos.push_back(forscaling);
	mcleglabels.push_back(get_leglabel(mclabels,mcsample.name()));
      }
    }

    string legstyle = (stackmc) ? "f" : "l";
    for (int iimc=mchistos.size()-1;iimc>=0;iimc--)
      l->AddEntry(mchistos[iimc],mcleglabels[iimc].c_str(),legstyle.c_str());
  }

  
  //
  // NORMALIZE MC
  //
  if (lumi>0.0) for (unsigned i=0;i<mchistos.size();i++) mchistos[i]->Scale(lumi);
  else if (0!=hdata) {
    double Idata = hdata->Integral();
    double Imc   = 0.0;
    for (unsigned i=0;i<mchistos.size();i++) Imc += mchistos[i]->Integral();
    for (unsigned i=0;i<mchistos.size();i++) {
      TH1F* h = mchistos[i];
      h->Scale(Idata/Imc);
      if (h->GetMaximum()>maximum) maximum = h->GetMaximum();
    }
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
    gPad->SetBottomMargin(0.375);
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
  
  if (mchistos.size()>0) {
    if (stackmc) {
      if (0==hmc) hmc = new THStack("hMC","");
      for (unsigned i=0;i<mchistos.size();i++) hmc->Add(mchistos[i]);
      if (0==hdata)
	maximum = (histogram.logy()) ? 50.*hmc->GetMaximum():1.5*hmc->GetMaximum();
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
	herr->Draw("error sameE2");
	l->AddEntry(herr,"MC stat.","F");


      } // if (mcuncert)
    }
    else {
      maximum=0.0;
      for (unsigned i=0;i<mchistos.size();i++) {
	TH1F* h = mchistos[i];
	TH1F* h_tot = tothistos[i];
	string drawopt = (0!=hmm) ? "HISTSAME" : (i==0) ? "HIST" : "HISTSAME";
	if (h->GetEntries()>0 &&lumi==0.) h->Scale(1.0/h->Integral()); else if(h->GetEntries()>0 &&lumi<0.) h->Scale(1.0/h_tot->Integral());
	cout << "Integral:" << h_tot->Integral() << endl;
	if (h->GetMaximum()>maximum) maximum=h->GetMaximum();
	if (get_minimum(h) <minimum) minimum=get_minimum(h);
	h->SetLineColor(h->GetFillColor());
	h->SetLineWidth(3);
	
	if(i==0) h->SetFillStyle(1001);
	else
	  h->SetFillStyle(3004);
	h->Draw(drawopt.c_str());

      }
      maximum = (histogram.logy()) ? 50.*maximum : 1.5*maximum;
      if (!histogram.logy()) mchistos[0]->SetMinimum(0.0);
      else if (-1.0!=ymin)   mchistos[0]->SetMinimum(ymin);
      else if (minimum>0.0)  mchistos[0]->SetMinimum(minimum);
      TH1* h = (0!=hmm) ? hmm : mchistos[0]; h->SetMaximum(maximum);
    }
  }
  
  
  //
  // DRAW DATA
  //
  if (hdata) {
    //    if (!stackmc&&hdata->GetEntries()>0) hdata->Scale(1.0/hdata->Integral()); now earlier for Chi2 (Hauke)
    hdata->Draw(datadrawopt.c_str());
    if (mchistos.size()==0) hdata->SetMaximum(maximum);
  }
  

  //
  // DRAW AXIS TITLES, STATS, LEGEND, AND LABEL
  //
  if (stats&&hdata) draw_stats(stats,hdata);
  TH1F*h=(mchistos.size()==0)?hdata:(stackmc)?(TH1F*)hmc->GetHistogram():mchistos[0];
  if (hmc&&hdata) {
    h->SetXTitle(hdata->GetXaxis()->GetTitle());
    h->SetYTitle(hdata->GetYaxis()->GetTitle());
  }
  if (0!=hdata&&!ytitle.empty()) {
    hdata->GetYaxis()->SetTitle("");
    h->SetYTitle(ytitle.c_str());
  }
  l->Draw();
  draw_labels(labels,leginplot,residuals);

  cout << "Lalala"<< endl;
  //
  // DRAW RESIDUALS
  //
  if (residuals) {
    mainPad->cd(2);
    TH1F* hres = (TH1F*)hdata->Clone();
    hres->Reset();
    hres->GetYaxis()->CenterTitle();
    hres->GetYaxis()->SetTitleSize(0.1);
    hres->GetYaxis()->SetTitleOffset(0.55);
    hres->GetYaxis()->SetLabelSize(0.1);
    hres->GetYaxis()->SetNdivisions(505);
    hres->GetXaxis()->SetTitleSize(0.16);
    hres->GetXaxis()->SetLabelSize(0.16);
    hres->GetXaxis()->SetTitleOffset(1);
    hres->GetXaxis()->SetLabelOffset(0.006);
    hres->GetXaxis()->SetNdivisions(505);
    hres->GetXaxis()->SetTickLength(hres->GetXaxis()->GetTickLength() * 3.0 );
    hres->SetYTitle("#frac{(data-mc)}{data} [%]");
    
    for (int ibin=1;ibin<=hres->GetNbinsX();ibin++) {
      Float_t ndata  = hdata->GetBinContent(ibin);
      Float_t nmc    = ((TH1*)hmc->GetStack()->Last())->GetBinContent(ibin);
      Float_t edata  = hdata->GetBinError(ibin);
      if (ndata>0) {
	Float_t bin  = (ndata-nmc)/ndata*100.;
	Float_t err  = nmc/ndata/ndata*edata*100.;
	hres->SetBinContent(ibin,bin);
	hres->SetBinError(ibin,err);
      }
      else {
	hres->SetBinContent(ibin,0.0);
	hres->SetBinError(ibin,0.0);
      }
    }
    Double_t max=std::max(std::abs(hres->GetMinimum()),
			  std::abs(hres->GetMaximum()));
    max *=1.05; max = std::min(49.9,max);
    hres->SetMinimum(-max);
    hres->SetMaximum(max);
    hres->SetMarkerStyle(kFullCircle);
    hres->SetMarkerSize(0.5);
    gPad->SetGridy();
    hres->Draw("EPHIST");

    if (!fitresids.empty()) {
      TF1* fResiduals = new TF1("fResiduals",fitresids.c_str(),
				hres->GetXaxis()->GetXmin(),
				hres->GetXaxis()->GetXmax());
      fResiduals->SetLineWidth(1);
      fResiduals->SetLineColor(hres->GetMarkerColor());
      hres->Fit(fResiduals,"Q");
      cout<<"Fit to Residuals ("<<fitresids<<"):"<<endl;
      for (Int_t iPar=0;iPar<fResiduals->GetNpar();iPar++)
	cout<<"iPar="<<iPar
	    <<": "<<fResiduals->GetParameter(iPar)
	    <<" +/- "<<fResiduals->GetParError(iPar)<<endl;
      hres->SetYTitle("#frac{(ref-mc)}{ref} [%]");
      cout<<"=> K-Factor = "<<1./(1.-0.01*fResiduals->GetParameter(0))<<endl;
    }
  }
  cout << "Lululu"<< endl;
  cout << "Lelele" << endl;
  //
  // SAVE PLOT TO FILE(S)
  //
  for (unsigned ifmt=0;ifmt<formats.size();ifmt++)
    {
      cout << "Magic" << endl;
      c->Print((output+"."+formats[ifmt]).c_str());
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
