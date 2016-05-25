////////////////////////////////////////////////////////////////////////////////
//
// th_datatodata_x (single histograms with resolutions)
// -----------------------------------------------------
//
//            12/07/2013 Christian Boser
////////////////////////////////////////////////////////////////////////////////


#include "../interface/Histogram.h"
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
#include <THStack.h>
#include <TH1F.h>
#include <TF1.h>
#include <TEventList.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <TList.h>
#include <TPluginManager.h>

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
void   get_resolution(TH1F* hh, int &loffset,  float& Mean, float& Sigma, float min=-1, float max=-1, int size=-1, bool drawtext=true);
void   print_improvement(float improvement, double histmaximum=-1, float XMIN=-1, float XMAX=-1);
////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo","*", "TStreamerInfo","RIO", "TStreamerInfo()");
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  vector<string> data       = cl.getVector<string>  ("data");
  vector<string> specialcuts= cl.getVector<string>  ("specialcuts",        "");
  
  // string         mcref     = cl.getValue<string>   ("mcref",               "");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  //  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  // string         mcsubpath = cl.getValue <string>  ("mcsubpath",           "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> ignore    = cl.getVector<string>  ("ignore",              "");
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
  double         ymin      = cl.getValue <double>  ("ymin",               0.);
  bool           overflow  = cl.getValue <bool>    ("overflow",         false);
  bool           logx      = cl.getValue <bool>    ("logx",             false);
  bool           logy      = cl.getValue <bool>    ("logy",             false);
  string         fitresids = cl.getValue <string>  ("fitresids",           "");
  bool           leginplot = cl.getValue <bool>    ("leginplot",         true);
  double         legx      = cl.getValue <double>  ("legx",              0.60);
  double         legy      = cl.getValue <double>  ("legy",              0.90);
  double         legw      = cl.getValue <double>  ("legw",              0.30);
  double         legh      = cl.getValue <double>  ("legh",             0.16);
  vector<string> datalabels  = cl.getVector<string>  ("datalabels",            "");
  vector<string> datacolors  = cl.getVector<string>  ("datacolors",            "");
  vector<string> labels    = cl.getVector<string>  ("labels",              "");
  bool           batch     = cl.getValue <bool>    ("batch",            false);
  vector<string> formats   = cl.getVector<string>  ("formats",             "");
  string         opath     = cl.getValue <string>  ("opath",              ".");
  string         output    = cl.getValue <string>  ("output",              "");
  vector<string> prefix    = cl.getVector<string>  ("prefix",              "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",              "");
  bool           resolution = cl.getValue<bool>    ("resolution",       false);
  bool           printtext  = cl.getValue<bool>    ("printtext",         true);
  bool           drawfitsonly = cl.getValue<bool>  ("drawfitsonly",         false);
  float          fitmin     = cl.getValue<float>    ("fitmin",              -1.0);
  float          fitmax     = cl.getValue<float>    ("fitmax",              -1.0);
  
 
  if(!cl.check()) return 0;
  cl.print();
  
  if (batch&&formats.size()==0) formats.push_back("pdf");
  if (output.empty()) {
    output = (varname.empty()) ? "DataToData" : varname;
    if (dirname!="utm") output = dirname + "_" + output;
    if (suffix.size()>0) 
      for (int i=(int)suffix.size()-1;i>=0;i--) output = output + "_" + suffix[i];
    if (prefix.size()>0)
      for (int i=(int)prefix.size()-1;i>=0;i--) output = prefix[i] + "_" + output;
    
    if (!opath.empty()) output = opath + "/" + output;
  }
  
  //
  // OPEN DATA (!) FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> datasamples;
  datasamples=MCSample::loadSamples(data,dirname,treename,"",filesuffix);
  if (datasamples.size()>0) MCSample::printSampleInfos(datasamples);
   
  int datasize = datasamples.size();
  cout <<datasamples.size()<< endl; 

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
    if (varname.empty())  histogram.setName("UtmDataToData");
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
  TApplication* app = new TApplication("utm_datatodata_x",&argc,argv);
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

  string selection;
  for (unsigned icut=0;icut<cuts.size();icut++) {
    string cut=get_cut(cuts,icut,"expression");
    // bool ignore_cut(false);
    //for (unsigned iign=0;iign<ignore.size();iign++)
    //  if (cut.find(ignore[iign])!=string::npos) ignore_cut=true;
    //if (ignore_cut) continue;
    if (!selection.empty())         selection       += "&&";
    selection += cut;
  }
  if (!histogram.selection().empty()) {
    selection = (selection.empty()) ?
      histogram.selection():histogram.selection()+"&&"+selection;
  }
  
  
  //
  // LEGEND
  //
  if (!data.empty()) {
    legh = (leginplot) ? std::min(legh,0.33/(datasamples.size()+2.)) : 
                         std::min(legh,0.90/(datasamples.size()+2.)); 
  }
  double legxmin = (leginplot) ? legx : 0.825;
  double legymin = legy;
  double legxmax = (leginplot) ? legx+legw : 0.995;
  double legymax = legymin - (datasamples.size()+1)*legh;
  if (leginplot&&legymax<0.6) legymax = 0.6;
  TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax);
  l->SetFillColor(10);l->SetFillStyle(0); l->SetLineColor(10); l->SetBorderSize(0);
  

  //
  // DATA
  //
  double minimum = (-1.!=ymin) ? ymin : (histogram.logy()) ? 0.1 : 0.0; 
  double maximum(0.0);
  int loffset = 0;

  //
  // 
  //
  vector<TH1F*>  datahistos;
  vector<string> dataleglabels;

  if (data.size()>0) {
    for (unsigned idata=0;idata<datasamples.size();idata++) {
      MCSample     datasample = datasamples[idata];
      stringstream sshdataname_i; sshdataname_i<<"h"<<datasample.name();
      datasample.setWeights(specialcuts);
      TH1F* hdata_i = histogram.create(sshdataname_i.str());
      datasample.project(hdata_i->GetName(),histogram.expr(),selection);
      hdata_i->SetFillStyle(1001);
      hdata_i->SetFillColor(get_color(datacolors,datasample.name()));
      datahistos.push_back(hdata_i);
      dataleglabels.push_back(get_leglabel(datalabels,datasample.name()));
    }

   for (int iidata=datahistos.size()-1;iidata>=0;iidata--)
     {
       if(!drawfitsonly)
	 l->AddEntry(datahistos[iidata],dataleglabels[iidata].c_str(),"lp");
       else
	 {
	   l->SetLineWidth(3);
	   l->AddEntry(datahistos[iidata],dataleglabels[iidata].c_str(),"lp");
	 }
     }
  }

  
  //
  // NORMALIZE DATA SAMPLES
  //
  for (unsigned i=0;i<datahistos.size();i++) datahistos[i]->Scale(1/datahistos[i]->Integral());

  maximum = (histogram.logy()) ? 100.*maximum : 1.5*maximum;
  string datadrawopt = (datahistos.size()>0) ? "E,HIST" : "E,HIST";
  
  
  // 
  // DRAW DATA 
  //
  float tempMean=-1;
  float tempSigma=-1;
  float tempmean[140];
  float tempsigma[140];
  float improvement=-1;
  //float histmaximum=-1;
  bool displaytext=false;
  displaytext = printtext;
 
  if (datahistos.size()>0) {
      maximum=0.0;
      for (unsigned i=0;i<datahistos.size();i++) {
	TH1F* h = datahistos[i];
	string drawopt = (i==0) ? "HIST" : "HISTSAME";
	if (h->GetEntries()>0) h->Scale(1.0/h->Integral());
	if (h->GetMaximum()>maximum) maximum=h->GetMaximum();
	if (get_minimum(h) <minimum) minimum=get_minimum(h);
	h->SetLineColor(h->GetFillColor());
	if(!drawfitsonly)
	  h->SetLineWidth(3);
	else
	  h->SetLineWidth(0);
	h->SetFillStyle(0);
	h->GetXaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetTitleOffset(1.45);
	h->GetXaxis()->SetLabelSize(0.04);
	h->GetYaxis()->SetLabelSize(0.04);
	h->Draw(drawopt.c_str());
	if(resolution){
	  get_resolution(h, loffset,  tempMean, tempSigma,fitmin,fitmax, datasize, displaytext);
	  tempmean[i]=tempMean;
	  tempsigma[i]=tempSigma;
	  cout << "drawn resolution for " << h->GetName() << endl;
	  //if(h->GetMaximum()>histmaximum)
	  //  histmaximum = h->GetMaximum();
	  
	}
      }
      maximum = (histogram.logy()) ? 50.*maximum : 1.5*maximum;
      if (!histogram.logy()) datahistos[0]->SetMinimum(0.0);
      else if (-1.0!=ymin)   datahistos[0]->SetMinimum(ymin);
      else if (minimum>0.0)  datahistos[0]->SetMinimum(minimum);
      TH1* h = datahistos[0]; h->SetMaximum(maximum);
  }

  if(datasize==2 &&printtext)
    {
      //if(tempsigma[0]<tempsigma[1])
      //{
      //  improvement = 100*(1-tempsigma[0]*tempmean[1]/(tempsigma[1]*tempmean[0]));
      //}
      //if(tempsigma[1]<tempsigma[0])
      //{
	  improvement = 100*(1-tempsigma[1]*tempmean[0]/(tempsigma[0]*tempmean[1]));
	  //}

      print_improvement(improvement, maximum, xmin, xmax);
    }

  

  //
  // DRAW AXIS TITLES, STATS, LEGEND, AND LABEL
  //
  TH1F*h=datahistos[0];
  if (!ytitle.empty()) {
    h->SetYTitle(ytitle.c_str());
  }
  l->Draw();
  draw_labels(labels,0,0);
  
  
  //
  // SAVE PLOT TO FILE(S)
  //
  c->RedrawAxis();
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

//______________________________________________________________________________
void get_resolution(TH1F* h, int &loffset, float  &Mean, float &Sigma, float min, float max, int size, bool drawtext){
  float histmean = h->GetMean();
  float histrms = h->GetRMS();
  float histmeane = h->GetMeanError();
  float histrmse = h->GetRMSError();  

  int color = h->GetLineColor();
  double maxval = h->GetBinContent(h->GetMaximumBin());
  //  int maxbin = h->GetMaximumBin();
  int halfmaxbin1 = h->FindFirstBinAbove(maxval/2);
  int halfmaxbin2 = h->FindLastBinAbove(maxval/2);
  TF1 *fit;
  if(min == -1 && max == -1){
    fit = new TF1("fit", "gaus",h->GetXaxis()->GetBinLowEdge(halfmaxbin1-1),h->GetXaxis()->GetBinUpEdge(halfmaxbin2+1));
  }
  else{
    cout << "Use fixed range for fit\n";
    fit = new TF1("fit", "gaus",min,max);
  }
  h->Fit("fit","RN");
  fit->SetLineWidth(3);
  fit->SetLineColor(color);
  double mean= fit->GetParameter(1);
  Mean = mean;
  double variance =fit->GetParameter(2);
  Sigma = variance;
    fit->Draw("same");
  char name[100];
  sprintf(name,"#splitline{mean: %1.1f GeV}{#sigma:        %1.1f GeV}",mean, variance);
  TLatex* text = new TLatex(0.270061,.8044,name);
  
  text->SetTextAlign(12);
  text->SetTextSize(0.03);
  text->SetTextFont(42);
  text->SetY(0.70-(float)loffset*0.08);
  loffset++;
  text->SetX(0.185);
  text->SetTextColor(color);
  text->SetNDC(true);
  if(drawtext)
    text->Draw("same");


  if(size==2)
    {
    }

  cout << h->GetEntries() << " Events!" << endl;

  cout << "HistMean: " << histmean << " " << histmeane << endl;
  cout << "HistRms: " << histrms << " " << histrmse << endl;
}
//___________________________________________________________________________
void print_improvement(float improvement, double histmaximum, float XMIN, float XMAX){

  char name2[100];
  if(improvement>0)    
    sprintf(name2,"rel impr: +%1.1f %%",improvement);
  else
    sprintf(name2,"rel impr: %1.1f %%",improvement);
  
  TLatex* text2 = new TLatex(0.270061,.8044,name2);
  
  float xpos = (XMAX+XMIN)/2+0.3*(XMAX-XMIN);
  
  text2->SetTextAlign(12);
  text2->SetTextSize(0.025);
  text2->SetTextFont(42);
  text2->SetY(histmaximum*0.5);
  //text->SetY(0.50);
  text2->SetX(xpos-0.10*(XMAX-XMIN)); //0.185
  text2->SetTextColor(kBlack);
  //text2->SetNDC(true);


  TBox *box2 = new TBox(100,100,150,150);
  box2->SetX1(xpos-0.12*(XMAX-XMIN));
  box2->SetX2(xpos+0.13*(XMAX-XMIN));
  box2->SetY1(histmaximum*0.45);
  box2->SetY2(histmaximum*0.55);
  box2->SetFillStyle(1001);
  box2->SetFillColor(kGreen-10);
  box2->SetLineColor(kBlack);
  box2->SetLineWidth(4);
  box2->Draw("same");
  text2->Draw("same");
}
