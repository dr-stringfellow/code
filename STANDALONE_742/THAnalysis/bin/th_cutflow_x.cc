////////////////////////////////////////////////////////////////////////////////
//
// th_cutflow_x
// -------------
//
//            01/12/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
//                       Simon Fink
////////////////////////////////////////////////////////////////////////////////


#include "../interface/DataSample.h"
#include "../interface/MCSample.h"

#include "../interface/CommandLine.h"
#include "../interface/RootStyle.h" 

#include <TApplication.h> 
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TEventList.h>
#include <TStyle.h> 
#include <TH1F.h> 
#include <TH1.h> 
#include <TCanvas.h> 
#include <TColor.h> 
#include <TAxis.h> 
#include <TPad.h> 
#include <THStack.h> 
#include <TLegend.h> 
#include <TLatex.h> 


#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <cassert> 
#include <algorithm> 

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper function
////////////////////////////////////////////////////////////////////////////////
string get_leglabel(const vector<string>&labels,const string& sample);
int    get_color(const vector<string>& colors,const string&sample);
void   draw_labels(const vector<string>& labels,bool leginplot=true,bool res=true);
string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression"); 
int    get_index_cut(const vector<string>&cuts,const string& cut);


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char**argv)
{
  //
  // evaluate command line / configuration file options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> data       = cl.getVector<string>("data",           "");
  vector<string> mcs        = cl.getVector<string>("mcs",            "");
  vector<string> vsignals   = cl.getVector<string>("signals",        "");
  string         dirname    = cl.getValue<string> ("dirname",     "utm");
  string         treename   = cl.getValue<string> ("treename",      "t");
  string         filesuffix = cl.getValue <string>("filesuffix",".root");
  string         datapath   = cl.getValue<string> ("datapath",       "");
  string         mcpath     = cl.getValue<string> ("mcpath",         "");
  double         lumi       = cl.getValue<double> ("lumi",          0.0);
  bool           relative   = cl.getValue<bool>   ("relative",    false);
  vector<string> cuts       = cl.getVector<string>("cuts");
  vector<string> mclabels   = cl.getVector<string>("mclabels",       "");
  vector<string> mccolors   = cl.getVector<string>("mccolors",       "");
  vector<string> mcweights  = cl.getVector<string>("mcweights",      "");

  // new options :-)
  vector<string> formats    = cl.getVector<string>("formats",        "");
  bool           leginplot  = cl.getValue<bool>   ("leginplot",    true);
  bool           reversemc  = cl.getValue<bool>   ("reversemc",   false);
  bool           stackmc    = cl.getValue<bool>   ("stackmc",      true);
  bool           logy       = cl.getValue<bool>   ("logy",        false);
  double         legx       = cl.getValue <double>("legx",         0.60);
  double         legy       = cl.getValue <double>("legy",         0.94);
  double         legw       = cl.getValue <double>("legw",         0.20);
  double         legh       = cl.getValue <double>("legh",        0.055);
  string         datalabel  = cl.getValue <string>("datalabel",  "data");
  double         ymin       = cl.getValue <double>("ymin",          -1.);
  double         ymax       = cl.getValue <double>("ymax",          -1.);
  string         xtitle     = cl.getValue <string>("xtitle",         "");
  vector<string> labels     = cl.getVector<string>("labels",         "");
  bool           stats      = cl.getValue <bool>  ("stats",        true);
  bool           batch      = cl.getValue <bool>  ("batch",        true);
  bool           verbose    = cl.getValue <bool>  ("verbose",     false);
  string         skip       = cl.getValue <string>("skip",           "");

  if (!cl.check()) return 0;
  cl.print();

  if (mcs.size()>0&&!reversemc) std::reverse(mcs.begin(),mcs.end());

  set<string> signals;
  for (unsigned i=0;i<vsignals.size();i++) signals.insert(vsignals[i]);

  if (xtitle.empty()) labels.push_back("0.19:0.88:42:Analysis cutflow");

  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //
  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app = new TApplication("utm_cutflow_x",&argc,argv);

  TH1::SetDefaultSumw2();
  set_root_style();
  gStyle->SetOptStat(0);
  TColor::SetPalette(1,0);

  string cname   = dirname + "_cutflow";
  int    cwidth  = (leginplot) ? 800 : 1000;
  int    cheight = 800;
  TCanvas* c = new TCanvas(cname.c_str(),cname.c_str(),cwidth,cheight);

  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(leginplot ? 0.15 : 0.18);
  if (logy) gPad->SetLogy();

  //
  // open data file and retrieve tree
  //
  DataSample datasample=DataSample::load(data,dirname,treename,datapath,filesuffix);
  if (!datasample.empty()) DataSample::printInfo(datasample);
  
  
  //
  // open mc files, retrieve trees, cross-sections, and k-factors
  //
  vector<MCSample> mcsamples;
  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0)MCSample::printSampleInfos(mcsamples);



  //
  // LEGEND
  //

  double legxmin = (leginplot) ? legx : 0.825;
  double legymin = legy; //if (residuals) legymin =1.-1.33*(1-legymin);
  double legxmax = (leginplot) ? legx+legw : 1.03;
  double legymax = legymin - (mcsamples.size()+1)*legh;
  if (leginplot&&legymax<0.6) legymax = 0.6;
  TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax);
  l->SetFillColor(10); l->SetLineColor(10); l->SetBorderSize(0);

    
  //
  // print cutflow table
  //
  unsigned wcut(0);
  for (unsigned icut=0;icut<cuts.size();icut++)
    if ((get_cut(cuts,icut,"label")).length()>wcut) wcut = get_cut(cuts,icut,"label").length()+2;
  wcut = std::max(wcut,(unsigned)5);
  unsigned ncol = (!datasample.empty())+mcsamples.size();
  unsigned wtot = wcut+23*ncol+13*(signals.size()>0&&signals.size()<mcs.size())+3;
  
  
  // headline
  cout<<endl<<endl<<MCSample::printline(wtot)<<endl;
  cout<<setw(wcut)<<left<<"CUT";
  if (!datasample.empty())
    cout<<setw(3)<<left<<" & "<<setw(20)<<right<<"DATA";
  for (unsigned imc=0;imc<mcsamples.size();imc++)
    cout<<setw(3)<<left<<" & "<<setw(20)<<right<<mcsamples[imc].name();
  if (signals.size()>0&&signals.size()<mcs.size())
    cout<<setw(3)<<left<<" & "<<setw(10)<<right<<"S/sqrt(B)";
  cout<<setw(3)<<right<<"\\\\"<<endl<<MCSample::printline(wtot)<<endl;  

  
  //
  // setup histogram for visualization
  //

  TH1F* hdata(0);
  if (!datasample.empty()) {
    hdata = new TH1F("hdata","hdata",(int)cuts.size()+1,0,(float)cuts.size()+1);
    hdata->GetYaxis()->SetNoExponent();
    hdata->SetMarkerStyle(kFullCircle);
    hdata->SetMarkerSize(1.25);
    hdata->SetTitle("");
    hdata->SetXTitle(xtitle.c_str());
    hdata->GetXaxis()->SetLabelFont(42);
    hdata->GetXaxis()->SetLabelSize(0.035);
    hdata->GetYaxis()->SetLabelSize(0.0325);
    l->AddEntry(hdata,datalabel.c_str(),"lp");
  }

  vector<TH1F*>  mchistos;
  vector<string> mcleglabels;
  for (unsigned imc=0;imc<mcsamples.size();imc++) {
    stringstream ss; ss<<"hmc"<<imc;
    mchistos.push_back(new TH1F(ss.str().c_str(),ss.str().c_str(),
			      (int)cuts.size()+1,0,(float)cuts.size()+1));
    mchistos.back()->GetYaxis()->SetNoExponent();
    mchistos.back()->SetMarkerStyle(20);
    mchistos.back()->SetTitle("");
    mchistos.back()->SetFillStyle(1001);
    mchistos.back()->SetFillColor(get_color(mccolors,mcsamples[imc].name()));
    mcleglabels.push_back(get_leglabel(mclabels,mcsamples[imc].name()));
  }
  string legstyle = (stackmc) ? "f" : "l";
  for (int iimc=mchistos.size()-1;iimc>=0;iimc--)
    l->AddEntry(mchistos[iimc],mcleglabels[iimc].c_str(),legstyle.c_str());


  // total number of events
  vector<double> ntotal;
  double nsig(0.0);
  double nbkg(0.0);
  cout<<setw(wcut)<<left<<"TOTAL";
  if (!datasample.empty()) {
    ntotal.push_back(datasample.nevents(""));
    stringstream ss; ss<<"("<<setprecision(3)<<fixed<<1.0<<")";
    cout<<setw(3)<<left<<" & "<<setw(12)<<right<<ntotal.back()<<setw(8)<<ss.str();
    hdata->Fill("Total",ntotal.back());
  }
  for (unsigned imc=0;imc<mcsamples.size();imc++) {
    MCSample mcsample = mcsamples[imc];
    mcsample.setWeights(mcweights);
    ntotal.push_back(mcsample.nevents("",lumi));
    stringstream ss;  ss<<"("<<setprecision(3)<<fixed<<1.0<<")";
    cout<<setw(3)<<left<<" & "
	<<setw(12)<<setprecision(1)<<fixed<<right<<ntotal.back()<<setw(8)<<ss.str();
    if (signals.find(mcsample.name())==signals.end()) nbkg+=ntotal.back();
    else nsig+=ntotal.back();
    mchistos[imc]->Fill("Total",ntotal.back());
  }
  if (signals.size()>0&&signals.size()<mcs.size()) {
    
    cout<<setw(3)<<left<<" & "
	<<setw(10)<<setprecision(2)<<fixed<<right<<nsig/std::sqrt(nbkg);
  }
  cout<<setw(3)<<right<<"\\\\"<<endl;
  
  // one row for each cut
  string selection;
  vector<TEventList*> vec_elist;
  for (unsigned icut=0;icut<cuts.size();icut++) {
    cout<<setw(wcut)<<left<<get_cut(cuts,icut,"label");

    nsig = nbkg = 0.0;
    
    if (!selection.empty()) selection += "&&";
    selection += get_cut(cuts,icut,"expression");

    if (!datasample.empty()) {
      double ndata = datasample.nevents(selection);
      stringstream ss; ss<<"("<<setprecision(3)<<fixed<<ndata/ntotal.front()<<")";
      cout<<setw(3)<<left<<" & "<<setw(12)<<right<<ndata<<setw(8)<<ss.str();
      if (relative) ntotal[0] = ndata;
      hdata->Fill(get_cut(cuts,icut,"label").c_str(),ndata);
    }
    
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample mcsample = mcsamples[imc];
      mcsample.setWeights(mcweights);
      double nevts = mcsample.nevents(selection,lumi);
      /*  if(verbose){
	mcsample.eventlist(vec_elist, selection);
	} */
      unsigned itotal = (!datasample.empty()) + imc;
      stringstream ss; ss<<"("<<setprecision(3)<<fixed<<nevts/ntotal[itotal]<<")";
      cout<<setw(3)<<left<<" & "
	  <<setw(12)<<setprecision(1)<<fixed<<right<<nevts<<setw(8)<<ss.str();
      if (relative) ntotal[itotal] = nevts;
      if (signals.find(mcsample.name())==signals.end()) nbkg+=nevts;
      else nsig+=nevts;
      mchistos[imc]->Fill(get_cut(cuts,icut,"label").c_str(),nevts);
    }
    
    if (signals.size()>0&&signals.size()<mcs.size()) {
      cout<<setw(3)<<left<<" & "
	  <<setw(10)<<setprecision(2)<<fixed<<right<<nsig/std::sqrt(nbkg);
      if (stats&&icut==cuts.size()-1) {
	stringstream ssSoverB;ssSoverB<<"S/#sqrt{B} = "<<nsig/std::sqrt(nbkg);
	labels.push_back(("0.19:0.82:42:"+ssSoverB.str()));
      }
    }
    
    cout<<setw(3)<<right<<"\\\\"<<endl;
  }

  // bottom line
  cout<<MCSample::printline(wtot)<<endl<<endl;
  cout << endl << "size: " << vec_elist.size() << endl;
  for(unsigned i=0; i<vec_elist.size(); i++){
    cout << "EventList " << i << endl;
    TEventList* tmpelist = (TEventList*) vec_elist[i+1];
    tmpelist->Print("all");
    delete tmpelist;
  }



  //
  // DRAW histograms
  //

  if (hdata&&!skip.empty()) hdata->GetXaxis()->SetRange(get_index_cut(cuts,skip)+2,
							hdata->GetNbinsX());

  double minimum = (-1.!=ymin) ? ymin : (logy) ? -1. : 0.0; 
  double maximum = (hdata) ? ((logy) ? 10.* hdata->GetMaximum() : 1.25*hdata->GetMaximum()) 
    : 0.0;

  if (ymax!=-1.) maximum = ymax;

  THStack* hmc(0);  
  if (mchistos.size()>0) {
    if (stackmc) {
      stringstream ss; ss<<"hMC";
      hmc = new THStack(ss.str().c_str(),"");
      for (unsigned i=0;i<mchistos.size();i++) hmc->Add(mchistos[i]);      

      hmc->Draw("HIST"); 

      if (!skip.empty()) 
	hmc->GetXaxis()->SetRange(get_index_cut(cuts,skip)+2,
				  hmc->GetHistogram()->GetNbinsX());
      if (ymax==-1.)
	maximum = std::max(maximum,(logy) ? 10.*hmc->GetMaximum():1.5*hmc->GetMaximum());
       
      hmc->GetXaxis()->SetTitle(xtitle.c_str());
      hmc->GetXaxis()->SetLabelFont(42);
      hmc->GetXaxis()->SetLabelSize(0.04);
      hmc->GetYaxis()->SetLabelSize(0.035);
      if (minimum!=-1.) {
	hmc->SetMinimum(minimum);
	hmc->GetHistogram()->SetMinimum(minimum);
      }
      hmc->SetMaximum(maximum);
      hmc->GetHistogram()->SetMaximum(maximum);
      hmc->GetYaxis()->SetNoExponent();
    }
    else {
      for (unsigned i=0;i<mchistos.size();i++) {
	TH1F* h = mchistos[i];
	string drawopt = (i==0) ? "HIST" : "HIST SAME";	

	if (!skip.empty()) h->GetXaxis()->SetRange(get_index_cut(cuts,skip)+2,
						   h->GetNbinsX());

	if (ymax==-1.) maximum=std::max(maximum,
					(logy) ? 10.*h->GetMaximum() : 1.5*h->GetMaximum());

	h->GetXaxis()->SetTitle(xtitle.c_str());
	h->GetXaxis()->SetLabelFont(42);
	h->GetXaxis()->SetLabelSize(0.04);
	h->GetYaxis()->SetLabelSize(0.035);
	h->SetLineColor(h->GetFillColor());
	h->SetLineWidth(3);
	h->SetFillStyle(0);

	h->Draw(drawopt.c_str());
      }    
      if (-1.0!=minimum) mchistos[0]->SetMinimum(minimum);
      mchistos[0]->SetMaximum(maximum);

    }
  }

  if (hdata) {
    if (!skip.empty()) hdata->GetXaxis()->SetRange(get_index_cut(cuts,skip)+2,
						   hdata->GetNbinsX());
    hdata->Draw( (mchistos.size()>0) ? "P SAME HIST" : "P HIST" );
  }
  l->Draw("SAME");

  draw_labels(labels,leginplot,false);


  for (unsigned i=0;i<formats.size();i++)
    c->Print((cname+"."+formats[i]).c_str());

  //
  // RUN ROOT APPLICATION
  //

  if (!batch) app->Run();
   
  return 0;
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
int get_index_cut(const vector<string>&cuts,const string& cut)
{
  for (int i=0;i<(int)cuts.size();i++) {
    if ( cuts[i].find(cut.c_str())!=string::npos ) {
      cout<<"Return index: "<<i<<endl;
      return i;
    }
  }
  cout<<"index for cut: "<<cut<<" not found in cuts vector\n";
  return 0;
}
