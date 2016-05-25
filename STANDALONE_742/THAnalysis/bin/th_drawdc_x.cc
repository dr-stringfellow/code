///////////////////////////////////////////////////////////////////////////////////////
//
// th_drawdc_x
// ---------------------
//
//          plot histogram from dc     26/05/2014 Christian Boeser <christian.boser@cern.ch>
///////////////////////////////////////////////////////////////////////////////////////


#include "../interface/Histogram.h"
#include "../interface/MCSample.h"
#include "../interface/DataSample.h"

#include "../interface/CommandLine.h"
#include "../interface/RootStyle.h"

#include <TApplication.h>
#include <TStyle.h>
#include <TColor.h>
#include <TROOT.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TTree.h>
#include <THStack.h>
#include <TH1F.h>
#include <TEventList.h>
#include <TObject.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TF1.h>
#include <TEventList.h>
#include <TLatex.h>
#include <TList.h>
#include <RooHistError.h>
#include <TGraphAsymmErrors.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <cfloat>
#include <fstream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////////////////////

bool in_array(const string &value, const std::vector<string> &array)
{
  return std::find(array.begin(), array.end(), value) != array.end();
}


//______________________________________________________________________________
void setHBinsNonZero(TH1F* h);
string get_cut(const vector<string>& cuts,unsigned icut,const string& mode);
string get_process(const vector<string>& processes,const string& sample, int& position);
void get_from_histo(const string hname, string& hchannel, string& hprocess, string& hsystematic);
int get_color(const vector<string>& colors,const string& sample);
float  get_chi2test(TH1F* hh,THStack* hmc);
float  get_kstest(TH1F* hh,THStack* hmc);
void draw_labels(const vector<string>& labels,bool leginplot,bool res);
bool check_if_already_in(const vector<string>& savedhist, string tmpchannel, string tmpprocess, string tmpsys);
string get_leglabel(const vector<string>&labels,const string& sample);
TH1F* combine_bin_change(TH1F* h, int nbinsx, double xmin, double xmax);

struct SmallerByPt{
  int * pts;

  SmallerByPt(int * pts_): pts(pts_){}
  
  bool operator()(size_t i, size_t j) const{
    return pts[i] < pts[j];
  }
};

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> mcs       = cl.getVector<string>  ("mcs",                  "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                 "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",               "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",            "");
  string         varname   = cl.getValue <string>  ("varname",              "");
  string         varexpr   = cl.getValue <string>  ("varexpr",              "");
  unsigned       nbinsx    = cl.getValue <unsigned>("nbinsx",                0);
  vector<double> binsx     = cl.getVector<double>  ("binsx",                "");
  double         xmin      = cl.getValue <double>  ("xmin",              -1E10);
  double         xmax      = cl.getValue <double>  ("xmax",              +1E10);
  double         ymin      = cl.getValue <double>  ("ymin",                -1.);
  bool           overflow  = cl.getValue <bool>    ("overflow",          false);
  bool           batch     = cl.getValue <bool>    ("batch",             false);
  vector<string> formats   = cl.getVector<string>  ("formats",              "");
  vector<string> prefix    = cl.getVector<string>  ("prefix",               "");
  string         suffix    = cl.getValue<string>   ("suffix",               "");
  bool           residuals = cl.getValue <bool>    ("residuals",          true);
  bool           mcuncert = cl.getValue <bool>     ("mcuncert",           true);
  bool           chi2test  = cl.getValue <bool>    ("chi2test",              0);
  bool           kstest    = cl.getValue <bool>    ("kstest",                0);
  bool           reversemc = cl.getValue <bool>    ("reversemc",             0);
  bool           datadriven =cl.getValue <bool>    ("datadriven",            0);   
  string         xtitle    = cl.getValue <string>  ("xtitle",               "");
  string         ytitle    = cl.getValue <string>  ("ytitle",               "");
  double         resmax    = cl.getValue <double>  ("resmax",              -1.);
  string         signal    = cl.getValue<string>   ("signal",               "");
  double         signalfac = cl.getValue<double>   ("signalfac",            1.);
  // dummys
  string         datalabel = cl.getValue <string>  ("datalabel",        "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",             "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",             "");
  vector<string> labels    = cl.getVector<string>  ("labels",               "");
  // additions for datacard handling
  string         datacard  = cl.getValue <string>  ("datacard", "datacard.txt");
  string         ipath     = cl.getValue <string>  ("ipath",               ".");
  string         opath     = cl.getValue <string>  ("opath",               ".");
  string         rootfile  = cl.getValue <string>  ("rootfile","datacard.root");
  string         channel   = cl.getValue <string>  ("channel",        "3tt_mu");
  vector<string> processes = cl.getVector<string>  ("processes",            "");
  string         systematic= cl.getValue <string>  ("systematic",           "");
  string         direction = cl.getValue <string>  ("direction",            "");
  double         legx      = cl.getValue <double>  ("legx",              0.821);
  double         legy      = cl.getValue <double>  ("legy",                0.9);
  double         legw      = cl.getValue <double>  ("legw",              0.174);
  double         legh      = cl.getValue <double>  ("legh",              0.055);
  double         legsize   = cl.getValue <double>  ("legsize",            0.06);
  double         cwidth    = cl.getValue <int>     ("cwidth",             1000);
  double         cheight   = cl.getValue <int>     ("cheight",             800);
  bool           blind     = cl.getValue <bool>    ("blind",              true);
  // parameter "theta" is only valid when doing blind analysis where we put a fake 
  // data__obs__nominal from MC in the datacard (which we do not use when blinded)
  // combine checks if the integral of this is the same as the number in observation
  // theta checks if the number in observation is an integer ... 
  bool           theta     = cl.getValue <bool>    ("theta",             false);
  bool           combine   = cl.getValue <bool>    ("combine",           false);
  bool           bfit      = cl.getValue <bool>    ("bfit",              false);
  string         histo     = cl.getValue <string>  ("histo",                 "");
  string         hpath     = cl.getValue <string>  ("hpath",                 "");
  bool           logy      = cl.getValue <bool>    ("logy",             false);
  bool           poisson   = cl.getValue <bool>    ("poisson",             false);

  if(!cl.check()) return 0;
  cl.print();
  
  if (!systematic.empty()&&direction.empty()) {
    cout<<"Error: direction Up/Down for systematic missing"<<endl; return 0;
  }

  rootfile = ipath + "/" + rootfile;

  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //
  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app = new TApplication("utm_datatomc_x",&argc,argv);

  TH1::SetDefaultSumw2();

  vector<TH1F*> tmpmchist;
  vector<TH1F*> tmpmchist_up;
  vector<TH1F*> tmpmchist_down;
  vector<string> tmpmcleglabels;

  vector<TH1F*> mcsignal;
  vector<TH1F*> mcsignalname;
  vector<string> mchistnames;
  vector<string> savedhist;
  vector<string> savedsys;
  vector<int> mcpos;
  vector<TH1F*> datahist; 
  //  string tmphist;
  TFile* hfile;

  string signal2;
  int signalpos=0;

  if(!signal.empty()){
    signal2 = get_process(processes, signal, signalpos);
  }
  cout << "signal: " <<signal << " => " << signal2 << endl;

  hfile = new TFile(rootfile.c_str(),"READ");
  if (!hfile->IsOpen()){cout<<"Failed to access "<<rootfile.c_str()<<endl;return 0;}

  TIter next = hfile->GetListOfKeys();
  TDirectory* idir;
  if(combine)
    { 
      if(bfit)
	idir = (TDirectory*)hfile->Get(("shapes_fit_b/" + channel).c_str());
      else
	idir = (TDirectory*)hfile->Get(("shapes_fit_s/" + channel).c_str());
      if (0==idir) { cout<<"No dir  found"<<endl; return 0; }
      next =  idir->GetListOfKeys();
    }

  TKey* key(0);
  while ((key=(TKey*)next())) {
    TH1F* tmphist = (TH1F*)key->ReadObj();
    string tmpprocess, tmpchannel, tmpsys;
    string tmpname = tmphist->GetName();
    //cout << tmpname << endl;
    if(tmpname.find("_e_") != string::npos){
      tmphist->SetName((tmpname.replace(tmpname.find("_e_"), 3, "_el_")).c_str());
    }
    if(combine){
      tmpchannel = channel;
      tmpprocess = tmphist->GetName();
      //cout << tmpprocess  << endl;
      if(tmpprocess.find("total") != string::npos ) continue;
      tmphist->SetName((tmpchannel+"__"+tmpprocess+"__nominal").c_str());
    }     
    get_from_histo(tmphist->GetName(), tmpchannel, tmpprocess, tmpsys);
    cout << tmpchannel << "|" << tmpprocess << "|" <<tmpsys << "|" <<endl;
    if(check_if_already_in(savedhist,tmpchannel, tmpprocess, tmpsys)==false){
      if(channel.find(tmpchannel)!=string::npos){
	if(theta){
	  //cout << tmphist->GetName() << endl;
	  tmpmchist.push_back(tmphist);
	  savedhist.push_back(tmpchannel + "__" + tmpprocess + "__" );
	}
	else{
	  if(strcmp(tmpsys.c_str(), "nominal")==0 ){
	    if( tmpprocess.find("data") != string::npos){
              datahist.push_back(tmphist);
              savedhist.push_back(tmpchannel + "__" + tmpprocess + "__" +tmpsys);
	    }
	    else{
	      tmpmchist.push_back(tmphist);	  
	      savedhist.push_back(tmpchannel + "__" + tmpprocess + "__" + tmpsys);	    
	      if(!signal.empty() &&  tmpprocess.find(signal2) != string::npos){
	       mcsignal.push_back(tmphist);
	      }
	    }
	  }
	  else if(tmpsys.find("Up")!=string::npos){
	    tmpmchist_up.push_back(tmphist);	  
	    savedhist.push_back(tmpchannel + "__" + tmpprocess + "__" + tmpsys);	    
	    tmpsys.replace(tmpsys.find("Up"),2,"");
	    //	  cout << "Check " << tmpsys << endl;
	    if(check_if_already_in(savedsys,"","", tmpsys)==false){
	      savedsys.push_back("____"+tmpsys);  
	    }
	  }
	  else if(tmpsys.find("Down")!=string::npos){
	    tmpmchist_down.push_back(tmphist);	  
	    savedhist.push_back(tmpchannel + "__" + tmpprocess + "__" + tmpsys);	    
	  }
	}
      }
    }
  }
  if(theta){
   TFile* hfile2 = new TFile((hpath+histo).c_str(),"READ");
   string channel2 = channel;
   //if(channel2.find("ttbar_cr")!=string::npos)
   //  channel2.replace(0,11, "tt");
   //cout << "!!!!!!!!!!!!\n" << channel2 << endl;
   datahist.push_back((TH1F*) hfile2->Get((channel2+"__data_obs__nominal").c_str()));



   TFile* hfile3 = new TFile((hpath+histo).c_str(),"READ");
   string channel3 = channel;
   //if(channel3.find("ttbar_cr")!=string::npos)
   //  channel3.replace(0,11, "tt");
   //cout << "!!!!!!!!!!!!\n" << channel3 << endl;
   mcsignal.push_back((TH1F*) hfile3->Get((channel3+"__tH__nominal").c_str()));

  }
  
  cout << "Found " <<  setw(6) << tmpmchist.size() << " mc templates" << endl;
  cout << "Found " <<  setw(6) << tmpmchist_up.size() << " mc up variation templates" << endl;
  cout << "Found " <<  setw(6) << tmpmchist_down.size() << " mc down variation templates" << endl; 
  cout << "Found " <<  setw(6) << datahist.size() << " data templates" << endl;



  for(int i=0; i< savedsys.size(); i++){
    cout << savedsys[i] << endl;    
  }


  
  for(int i=0; i  < tmpmchist.size(); i++){
    if(combine){
      //cout << "bins: " << mchist[i]->GetNbinsX() << " | xmin: " <<  mchist[i]->GetXaxis()->GetBinLowEdge(mchist[i]->GetXaxis()->GetFirst()) 	<< " | xmax: " <<	mchist[i]->GetXaxis()->GetBinUpEdge(mchist[i]->GetXaxis()->GetLast()) <<	endl;
      tmpmchist[i] = combine_bin_change(tmpmchist[i], 
					datahist[0]->GetNbinsX(), 
					datahist[0]->GetXaxis()->GetBinLowEdge(datahist[0]->GetXaxis()->GetFirst()), 
					datahist[0]->GetXaxis()->GetBinUpEdge(datahist[0]->GetXaxis()->GetLast()));
    }
    string tmpprocess, tmpchannel, tmpsys, process;
    get_from_histo(tmpmchist[i]->GetName(), tmpchannel, tmpprocess, tmpsys);
    int position;
    process = get_process(processes, tmpprocess, position);
    mcpos.push_back(position);
    mchistnames.push_back(process);
    int col = get_color(mccolors, mchistnames[i]);
    tmpmchist[i]->SetFillColor(col);
    //    tmpmchist[i]->SetLineColor(col);
    tmpmcleglabels.push_back(get_leglabel(mclabels,mchistnames[i]));
  }

  //cout << "here\n" << endl;
  int tmp[100]; 
  
  vector<size_t> indices;
  indices.resize(tmpmchist.size());
  for(int i=0; i<tmpmchist.size(); ++i){
    tmp[i] = mcpos[i];
    indices[i] = i;
  }
  sort(indices.begin(), indices.end(), SmallerByPt(tmp));

  vector<TH1F*> mchist;
  vector<TH1F*> mchist_up;
  vector<TH1F*> mchist_down;
  vector<string> mcleglabels;
  
  for(int i=0; i<tmpmchist.size(); i++)
    {
      mchist.push_back(tmpmchist[indices[i]]);
      mcleglabels.push_back(tmpmcleglabels[indices[i]]);
    }
  
  /*   for(int i=0; i<mchist.size(); i++){
    cout << tmpmchist[i]->GetName() << " " << mchist[i]->GetName() << endl;
    cout << tmpmchist[i]->Integral() << " " << mchist[i]->Integral() << endl;
    cout << i << " : " << tmpmcleglabels[i] << " " <<  mcleglabels[i] << endl;
  }
  */

  set_root_style();
  gStyle->SetOptStat(0);
  TColor::SetPalette(1,0);
  TGaxis::SetMaxDigits(3);

  string cname   = "MVAcanvas";
  TCanvas* c = new TCanvas(cname.c_str(),cname.c_str(),cwidth,cheight);
  gPad->SetRightMargin(1-legx+0.01);

  TPad* mainPad = (TPad*)gPad;
  if (residuals) {
    Double_t topMargin    = mainPad->GetTopMargin();
    //Double_t bottomMargin = mainPad->GetBottomMargin();                                                                                                                                                        
    Double_t leftMargin   = mainPad->GetLeftMargin();
    Double_t rightMargin  = mainPad->GetRightMargin();
    leftMargin = leftMargin/1.2;

    mainPad->Divide(1,2,0.01,0.0);

    mainPad->GetPad(1)->SetFillColor(0);
    mainPad->GetPad(2)->SetFillColor(0);
    mainPad->GetPad(1)->SetPad(0.0,0.33,1.0,1.0);
    mainPad->GetPad(2)->SetPad(0.0,0.0,1.0,0.3);

    mainPad->cd(1);

    if (mainPad->GetLogx()) gPad->SetLogx();
    if (logy) gPad->SetLogy();
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
  
  if (!mchist.empty()) {
    legh = std::min(legh,0.90/(mchist.size()+2.));
  }
  double legxmin = legx;
  double legymin = legy; if (residuals) legymin =1.-1.33*(1-legymin);
  double legxmax = legxmin + legw;
  double legymax = legymin - (mchist.size()+1)*legh;
  TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax);
  l->SetFillColor(10); l->SetLineColor(10); l->SetBorderSize(0); l->SetTextAlign(11);
  l->SetTextSize(legsize);
  //cout << "HERE"<< endl;
  
  TH1F* hdata;
  hdata = datahist[0];
  
  l->AddEntry(hdata, "Data","pe");

  double minimum = (-1.!=ymin) ? ymin : 0.0;
  double maximum(0.0);
  
  maximum = (!logy) ?1.5*hdata->GetMaximum(): 10*hdata->GetMaximum();
 
  if(blind && !theta){
    for(int j=0; j<=hdata->GetNbinsX(); j++){
      if(j > (int)hdata->GetNbinsX()/2 + 1)
	hdata->SetBinContent(j,-1);
    }
  }
 


  /*  for(int j=0; j<=hdata->GetNbinsX(); j++){
    if(hdata->GetBinContent(j)==0)
      hdata->SetBinContent(j,-1);
  }
  */

  hdata->SetMarkerStyle(kFullCircle);
  hdata->SetMarkerColor(kBlack);
  hdata->SetLineColor(kBlack);
  hdata->SetMarkerSize(1.25);
  //cout << "HERE2"<< endl;
  
  if(!xtitle.empty()){
    if(residuals){
      hdata->GetXaxis()->SetTitle(xtitle.c_str());
      hdata->GetYaxis()->SetTitle(ytitle.c_str());
    }
    else{
      mchist[0]->GetXaxis()->SetTitle(xtitle.c_str());
      mchist[0]->GetYaxis()->SetTitle(ytitle.c_str());
    }
  }
    
  

  THStack* hmc(0);

  // Fill for each systematic one stack
  /*  for (unsigned i=0;i<mchist_up.size();i++){
    string tmpprocess, tmpchannel, tmpsys, process;
    get_from_histo(mchist_up[i]->GetName(), tmpchannel, tmpprocess, tmpsys);
    if(tmpsys.find("Up")!=string::npos){

      if(check_if_already_in(savedsys,tmpchannel, tmpprocess, tmpsys)==false){
	THStack
	savedsys.push_back(tmpchannel + "__" + tmpprocess + "__" +tmpsys);
	hmc_up-
	for(int j=0; j < mchist_down.size(); j++){
	  get_from_histo(mchist_down[i]->GetName(), tmpchannel, tmpprocess, tmpsys);
	}
  }
  if (mchist_up.size()>0) {
    if (0==hmc_up) hmc_up = new THStack("hMC_up","");
    for (unsigned i=0;i<mchist_up.size();i++){
      hmc_up->Add(mchist_up[i]);
    }
  }
  */
  //  datahist[0]->Draw("HIST");
    
  //for (int iimc=mchist.size()-1;iimc>=0;iimc--)
  //  l->AddEntry(mchist[iimc],mcleglabels[iimc].c_str(),"F");
  

  bool border[30];
  vector<string> already_in_legend;
  already_in_legend.push_back("dummy");
  for (int iimc=mchist.size()-1;iimc>=0;iimc--){
    if (not in_array(mcleglabels[iimc].c_str(), already_in_legend)) {
      l->AddEntry(mchist[iimc],mcleglabels[iimc].c_str(),"F");
      already_in_legend.push_back(mcleglabels[iimc]);
      border[iimc] = true;
    }
    else{
      border[iimc] = false;
    }
  }

  int MCInt = 0;
  if (mchist.size()>0) {
    if (0==hmc) hmc = new THStack("hMC","");
    for (int i=0;i<mchist.size();i++){
      if(border[i]) mchist[i]->SetLineColor(kBlack);
      else mchist[i]->SetLineColorAlpha(kBlack,1); 
      //else mchist[i]->SetLineColor(mchist[i]->GetFillColor());
      mchist[0]->SetYTitle("Events/Bin");
      hmc->Add(mchist[i]);
      cout << mchist[i]->GetName() << " " << mchist[i]->GetName() << endl;
      cout << mchist[i]->Integral() << " " << mchist[i]->Integral() << endl;

      MCInt+= mchist[i]->Integral();
    }

    hmc->Draw("HIST");
    hmc->GetHistogram()->SetYTitle("Events/Bin");
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
    // cout << "here" << endl;
    if (mcuncert) {
      TH1F* herr = (TH1F*) mchist[0]->Clone("herr");
      for (int i=1;i<mchist.size();i++) {
	for (int ibin=0;ibin<=mchist[i]->GetNbinsX();ibin++) {
	  float val1  = herr->GetBinContent(ibin);
	  float eval1 = herr->GetBinError(ibin);
	  float val2  = mchist[i]->GetBinContent(ibin);
	  float eval2 = mchist[i]->GetBinError(ibin);
	  herr->SetBinContent(ibin,(val1+val2));
	  herr->SetBinError(ibin,sqrt(eval1*eval1+eval2*eval2));
	}
      }

      herr->SetLineColor(kBlack);
      herr->SetFillColor(kBlack);
      herr->SetFillStyle(3245);
      herr->SetMarkerStyle(1);
      herr->SetMarkerColor(kBlack);
      herr->Draw("error sameE2");
      if(!theta)
	l->AddEntry(herr,"MC stat.","F");
      else 
	l->AddEntry(herr,"Stat.+syst.","F");
    }
  }
  //cout << "here" << endl;
  if(!mcsignal.empty()){
    TH1F* sig_i = mcsignal[0];
    sig_i->SetFillStyle(0);
    sig_i->SetLineColor(2);
    sig_i->SetLineWidth(2);
    sig_i->Scale(signalfac);
    if(signalfac!=1){
      stringstream ss;
      ss<<signalfac;
      l->AddEntry(sig_i,(ss.str()+"x tHq #scale[0.8]{(^{}#it{C}_{t}=-1)}").c_str(),"l");
    
    
      sig_i->Draw("HISTSAME");
    }
  }
    

  if(hdata&&poisson){
    TGraphAsymmErrors * gdata = new TGraphAsymmErrors(hdata);
    for (int i = 0; i < gdata->GetN(); ++i) {
      double y=gdata->GetY()[i];
      if (gdata->GetY()[i]==0)
	gdata->SetPoint(i,-99,-99);
      double ym, yp;
      double nSigma = 1.;
      RooHistError::instance().getPoissonInterval(y,ym,yp,nSigma);
      double yerrplus = yp - y;
      double yerrminus = y - ym;
      gdata->SetPointEYlow(i,yerrminus);
      gdata->SetPointEYhigh(i,yerrplus);
      gdata->SetPointEXlow(i,0);
      gdata->SetPointEXhigh(i,0);
      
    }
    //    cout << "Data integral: " << gdata->Integral() << endl; 
    gdata->Draw("PZ"); 
  }

  hdata->GetXaxis()->SetLabelOffset(999);
  hmc->GetXaxis()->SetLabelOffset(999);
  //  TPad * box1 = new TPad("b", "b",0.075,0.0,0.12,0.09);
  //box1->SetBorderMode(0);
  //box1->SetFillColor(kBlack);
  //box1->Draw();


  cout << "Data integral: " << hdata->Integral() <<endl;
  
  if(!poisson) hdata->Draw("ESame");
  l->Draw("same");
  draw_labels(labels,false,residuals);
  

  //
  // DRAW RESIDUALS
  //
  if (residuals) {
    mainPad->cd(2);
    TH1F* hres = (TH1F*)hdata->Clone();
    TH1F* herrges = (TH1F*)hdata->Clone();
    TH1F* herrges2 = (TH1F*)hdata->Clone();
    TH1F* hsysstat = (TH1F*)hdata->Clone();

    hres->Reset();
    herrges->Reset();
    herrges2->Reset();
    hsysstat->Reset();

    hres->GetYaxis()->CenterTitle();
    hres->GetYaxis()->SetTitleSize(0.11);
    hres->GetYaxis()->SetTitleOffset(0.5);
    hres->GetYaxis()->SetLabelSize(0.12);
    hres->GetYaxis()->SetNdivisions(505);
    hres->GetXaxis()->SetTitleSize(0.14);
    hres->GetXaxis()->SetLabelSize(0.13);
    hres->GetXaxis()->SetTitleOffset(1);
    hres->GetXaxis()->SetLabelOffset(0.006);
    hres->GetXaxis()->SetNdivisions(505);
    hres->GetXaxis()->SetTickLength(hres->GetXaxis()->GetTickLength() * 3.0 );
    
    if(!datadriven)
      //      hres->SetYTitle("#frac{Data-MC}{MC}")
      hres->SetYTitle("#frac{Data-Pred.}{Pred.}");
    else
      hres->SetYTitle("#frac{Data-Pred.}{Pred.}");

    //    TGraphAsymmErrors * resdata = new TGraphAsymmErrors((TH1F*)hres->Clone());
    TGraphAsymmErrors * resdata = new TGraphAsymmErrors(0);
    resdata->Set(0);

    for (int ibin=1;ibin<=hres->GetNbinsX();ibin++) {
      Float_t ndata  = hdata->GetBinContent(ibin);
      Float_t nmc    = ((TH1*)hmc->GetStack()->Last())->GetBinContent(ibin);
      Float_t edata  = hdata->GetBinError(ibin);
      if (ndata>=0) {
	Float_t bin  = (ndata-nmc)/nmc;
	Float_t err  = edata/nmc;
	if (ndata!=0){
	  hres->SetBinContent(ibin,bin);
	  hres->SetBinError(ibin,err);
	}
	if(poisson)
          {
            double y=ndata;
            double ym, yp;
            double nSigma = 1.;
	    RooHistError::instance().getPoissonInterval(y,ym,yp,nSigma);
            double yerrplus = yp - y;
            double yerrminus = y - ym;
	    double xbin = hres->GetBinCenter(ibin);
	    //	    cout << " ndata : " << y  << "|  ym : " << ym << "|  yp :  " << yp << " | ym-val :  " << yerrminus << " | ypval : " << yerrplus << " | bin  : " << bin << " | ym-val/nmc :  " << yerrminus/nmc << " | ypval/nmc : " << yerrplus/nmc << endl;
	    resdata->SetPoint(ibin-1,xbin,bin);
	    resdata->SetPointEYlow(ibin-1,(yerrminus/nmc));
	    resdata->SetPointEYhigh(ibin-1,(yerrplus/nmc));
	    //resdata->SetPointEXlow(ibin-1,hres->GetBinWidth(ibin)/2);
	    //resdata->SetPointEXhigh(ibin-1,hres->GetBinWidth(ibin)/2);
	    if(ndata==0)
	      resdata->SetPoint(ibin-1,-99,-99);
	   }

       }
       else {
	 hres->SetBinContent(ibin,0.0);
	 hres->SetBinError(ibin,0.0);
	 herrges->SetBinContent(ibin,0.0);
	 herrges->SetBinError(ibin,0.0);
	 herrges2->SetBinContent(ibin,0.0);
	 herrges2->SetBinError(ibin,0.0);
       }
     }
     Double_t max=std::max(std::abs(hres->GetMinimum()),
			   std::abs(hres->GetMaximum()));
     max *=1.05; max = std::min(49.9,max);
     if(resmax>0.) max = resmax;
     hres->SetMinimum(-max);
     hres->SetMaximum(1.2*max);
     hres->SetMarkerStyle(kFullCircle);
     hres->SetMarkerSize(0.5);
     //     gPad->SetGridy();

     // DRAW MC UNCERTAINTY IN RESIDUAL

     TH1F* herr2 = (TH1F*) mchist[0]->Clone("herr");
     if (mcuncert) {
       //TH1F* herr2 = (TH1F*) mchist[0]->Clone("herr");
       for (int i=1;i<mchist.size();i++) {
	 for (int ibin=1;ibin<=mchist[i]->GetNbinsX();ibin++) {
	   float valA  = herr2->GetBinContent(ibin);
	   float evalA = herr2->GetBinError(ibin);
	   float valB  = mchist[i]->GetBinContent(ibin);
	   float evalB = mchist[i]->GetBinError(ibin);
	   herr2->SetBinContent(ibin,(valA+valB));
	   herr2->SetBinError(ibin,sqrt(evalA*evalA+evalB*evalB));


	   if((i==mchist.size()-1)&&herr2->GetBinContent(ibin)){
	     herr2->SetBinContent(ibin,0.);
	     herr2->SetBinError(ibin,(sqrt(evalA*evalA+evalB*evalB)/(valA+valB)));
	   }
	 }
       }

       for (int ibin=1;ibin<=hres->GetNbinsX();ibin++) {
	 Float_t staterr = herr2->GetBinError(ibin); 
	 //Float_t syserr_up = herrges2->GetBinContent(ibin)+herrges2->GetBinError(ibin);
	 //Float_t syserr_down = herrges2->GetBinContent(ibin)-herrges2->GetBinError(ibin);
	 hsysstat->SetBinContent(ibin,herrges2->GetBinContent(ibin));
	 hsysstat->SetBinError(ibin,sqrt(herrges2->GetBinError(ibin)*herrges2->GetBinError(ibin)+staterr*staterr));
       }

       herrges->SetLineWidth(0);
       herrges->SetFillColor(kMagenta-9);
       herrges->SetFillStyle(3001);
       herrges->SetMarkerSize(0);
       herrges2->SetLineWidth(0);
       herrges2->SetFillColor(kMagenta-9);
       herrges2->SetFillStyle(3001);
       herrges2->SetMarkerSize(0);

       hsysstat->SetLineWidth(0);
       hsysstat->SetFillColor(kMagenta-9);
       hsysstat->SetFillStyle(3001);
       hsysstat->SetMarkerSize(0);

       herr2->SetLineColor(kBlack);
       herr2->SetFillColor(kMagenta-9);
       herr2->SetFillStyle(3001);
       herr2->SetMarkerStyle(1);
       //hsysup->SetFillColor(kPink-7);
       //hsysup->SetFillStyle(3001);    
       //hsysup->SetLineWidth(0);
       //hsysdown->SetFillColor(kPink-7);
       //hsysdown->SetFillStyle(3001);
       //hsysdown->SetLineWidth(0);
       if(poisson){
	 hres->SetMarkerSize(0);
	 gPad->SetGridy();
	 hres->Draw("AXIS");
	 hres->Draw("AXIG same");
	 herr2->Draw("E2 error same");
	 resdata->SetMarkerStyle(kFullCircle);
	 resdata->SetMarkerSize(0.5);
	 gStyle->SetEndErrorSize(6);
	 //	hres->Draw("EPHIST same");
	 resdata->Draw("P0");
	 resdata->Draw("||");
	 

       }
       else{
	 hres->Draw("EPHIST");
	 herr2->Draw("error sameE2");
	 hres->Draw("EPHIST same");
       }
     }
     
     else{
       hres->Draw("EPHIST");
     }

     
     //     hdata->GetYaxis()->SetTitleOffset(0.3);
     hmc->GetYaxis()->SetTitleOffset(0.85);
     //     hres->GetYaxis()->SetTitleOffset(0.035);
     
    //
    // LEGEND 2
    //
    if (mcuncert) {
   
	TLegend* l2 = new TLegend(legxmin,0.73,legxmax,0.90);
	l2->SetTextSize(0.12);
	l2->SetFillColor(10); l2->SetLineColor(10); l2->SetBorderSize(0);
	l2->SetTextAlign(11);
     
	if(!theta)
	  l2->AddEntry(herr2,"MC stat.","F");
	else
	  l2->AddEntry(herr2,"Stat.+syst.","F");
	l2->Draw();
    }
    
    mainPad->cd(1);
    TPad* b = new TPad("b", "b",0.0501342,0.0,0.1201047,0.0814685);
    
    b->SetBorderMode(0);
    b->SetFillColor(kWhite);
    b->Draw();

  
  }

  

  
  if(theta){
    c->SaveAs(("postfit2D_MVA_"+channel+"_"+suffix+".pdf").c_str());
    c->SaveAs(("postfit2D_MVA_"+channel+"_"+suffix+".png").c_str());
  }
  else{
    c->SaveAs(("prefit2D_MVA_"+channel+".pdf").c_str());
    c->SaveAs(("prefit2D_MVA_"+channel+".png").c_str());
  }
    
  if (!batch) app->Run();
  
  return 0;
}


//______________________________________________________________________________
void setHBinsNonZero(TH1F* h)
{
  // function to set all bins in a histogram to non empty values
  // this is needed to ensure proper working of the likelihood ratio 
  // minimizations inside the >> theta-framework.org

  if (0==h) return;
  
  for (Int_t i=1;i<=h->GetNbinsX();i++) {
    if (h->GetBinContent(i)<FLT_EPSILON) {
      h->SetBinContent(i,FLT_EPSILON); 
      h->SetBinError(i,sqrt(FLT_EPSILON)); 
    }
  }
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
string get_process(const vector<string>& processes,const string& sample, int& position)
{
  for (int isample=0;isample<processes.size();isample++) {
    string tmp(processes[isample]);
    size_t pos = tmp.find(":"); assert (pos!=string::npos);
    //  if (sample==tmp.substr(0,pos)) return tmp.substr(pos+1);
    if (sample==tmp.substr(pos+1)){
      position = isample;
      return tmp.substr(0,pos);
    }
  }
  cout<<"Error: get_process() did not find "<<sample<<endl;
  return "";
}

//______________________________________________________________________________
void get_from_histo(const string hname, string& hchannel, string& hprocess, string& hsystematic){
  string tmp = hname;
  size_t pos = tmp.find("__");
  if (pos!=string::npos) {
    hchannel      = tmp.substr(0,pos);
    tmp = tmp.substr(pos+2);
    size_t pos2 = tmp.find("__");
    if (pos2!=string::npos) {
      hprocess = tmp.substr(0,pos2);
      hsystematic = tmp.substr(pos2+2);
    }
    else{
      hprocess = tmp;
    }
  }
}
//________________
TH1F* combine_bin_change(TH1F* h, int nbinsx, double xmin, double xmax)
{
  TH1F* new_h = new TH1F(h->GetName(), h->GetName(), nbinsx, xmin, xmax);
  for(int i=0; i < nbinsx; i++){
    new_h->SetBinContent(i+1, h->GetBinContent(i+1));
    new_h->SetBinError(i+1, h->GetBinError(i+1));
  }
  return new_h;
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
void draw_labels(const vector<string>& labels,bool leginplot,bool res)
{
  for (int ilabel=0;ilabel<labels.size();ilabel++) {
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


bool check_if_already_in(const vector<string>& savedhist, string tmpchannel, string tmpprocess, string tmpsys)
{
  for(int i=0; i< savedhist.size(); i++){
    if(savedhist[i]==tmpchannel+"__"+tmpprocess+"__"+tmpsys)
      return true;
  }
  return false;
}


string  get_leglabel(const vector<string>& labels,const string& sample)
{
  for (int ilabel=(int)labels.size()-1;ilabel>=0;--ilabel) {
    string label = labels[ilabel];
    size_t pos = label.find(":"); assert(pos!=string::npos);
    if (sample==label.substr(0,pos)) return label.substr(pos+1);
  }
  return sample;
}




