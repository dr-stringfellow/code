////////////////////////////////////////////////////////////////////////////////
//
// bdt_overtrain_x
// --------------
//
// tool to check for overtraining of BDT outputs 
//
// Christian Boser
//                                          
////////////////////////////////////////////////////////////////////////////////

#include "../interface/Histogram.h"
#include "../interface/DataSample.h"
#include "../interface/MCSample.h"
#include "../interface/MatrixMethod.h"

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
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
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
void get_resolution(TH1F* h, float min=-1, float max=-1);
string get_cut(const vector<string>& cuts,unsigned icut,const string& mode);
void setup_global_style();
void set_legend(TLegend* leg);
void set_legend1(TLegend* leg);

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  string         input     = cl.getValue <string>  ("input"             );
  string         datapath  = cl.getValue <string>  ("datapath",       "");
  string         filesuffix= cl.getValue <string>  ("filesuffix",".root");
  string         treename  = cl.getValue <string>  ("treename",      "t");
  vector<string> cuts      = cl.getVector<string>  ("cuts",           "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",         "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",      "");
  bool           batch     = cl.getValue <bool>    ("batch",       false);
  vector<string> prefix    = cl.getVector<string>  ("prefix",         "");
  vector<string> suffix    = cl.getVector<string>  ("suffix",         "");
  string         varexpr   = cl.getValue <string>  ("varexpr"           );
  // dummys
  string         datalabel  = cl.getValue <string> ("datalabel",    "data");
  vector<string> mclabels   = cl.getVector<string> ("mclabels",         "");
  vector<string> mccolors   = cl.getVector<string> ("mccolors",         "");
  vector<string> labels     = cl.getVector<string> ("labels",           "");
  bool           regression = cl.getValue<bool>    ("regression",    false);
  bool           reso       = cl.getValue<bool>    ("reso",          false);
  string         xtitle     = cl.getValue <string> ("xtitle", "BDT output");
  double         xmin       = cl.getValue<double>  ("xmin",           -100);
  double         nbinsx     = cl.getValue<double>  ("nbinsx",           40);
  double         xmax       = cl.getValue<double>  ("xmax",            100);
  string         text       = cl.getValue <string> ("text",             "");
  bool           sm         = cl.getValue<bool>    ("sm",            false);	
  bool           lclass     = cl.getValue<bool>    ("lclass",        false);	
  bool           ttbar      = cl.getValue<bool>    ("ttbar",         false);	 
 
  if(!cl.check()) return 0;
  
  //
  // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
  //
  string folder= input.substr(0,input.rfind("/")+1);

  cout << folder << endl;
  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app = new TApplication("th_xxx_x",&argc,argv);
  TH1::SetDefaultSumw2();
  set_root_style();
  gStyle->SetOptStat(0);
  TColor::SetPalette(1,0);

  const char* textstring = text.c_str();


  //
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //

  TFile* file = new TFile(input.c_str(),"READ");
  if (!file->IsOpen()) return 0;
  
  TTree* tree_train = (TTree*)file->Get("TrainTree");
  TTree* tree_test = (TTree*)file->Get("TestTree");
  
  //
  // CREATE HISTOGRAMS FROM MCSAMPLES IF SO REQUESTED
  //

  
  //  histogram specification
  
  Histogram histogram;
  histogram.setExpr(varexpr);
  histogram.setNbinsx(nbinsx);
  histogram.setXmin(xmin);
  histogram.setXmax(xmax);
  histogram.setName("");
 
  
 
    // event selection

    string selection_sig = "(classID==0)*weight";
    string selection_bkg = "(classID==1)*weight";

    // create MC templates for signal even, signal odd, background even, background odd
    // even == Training
    // odd  == Test

    TH1F* hsignaleven = new TH1F("hsignaleven","BDT Output",nbinsx,xmin,xmax);
    TH1F* hsignalodd  = new TH1F("hsignalodd","BDT Output",nbinsx,xmin,xmax);
    TH1F* hbackgroundeven=0;
    TH1F* hbackgroundodd=0; 
    if(!regression){
    hbackgroundeven = new TH1F("hbackgroundeven","BDT Output",nbinsx,xmin,xmax);
    hbackgroundodd = new TH1F("hbackgroundodd","BDT Output",nbinsx,xmin,xmax);
    }

    TH1F* se = new TH1F("se","BDT Output",nbinsx,xmin,xmax);
    TH1F* so = new TH1F("so","BDT Output",nbinsx,xmin,xmax);
    TH1F* sa = new TH1F("sa","BDT Output",nbinsx,xmin,xmax);
    TH1F* be = new TH1F("be","BDT Output",nbinsx,xmin,xmax);
    TH1F* bo = new TH1F("bo","BDT Output",nbinsx,xmin,xmax);
    TH1F* ba = new TH1F("ba","BDT Output",nbinsx,xmin,xmax);

    // create Kolmogorov-Smirnov values

    float kss(0);
    float ksb(0);

    // fill histograms
  
    tree_train->Project(hsignaleven->GetName(),histogram.expr().c_str(),selection_sig.c_str());
    tree_test->Project(hsignalodd->GetName(),histogram.expr().c_str() ,selection_sig.c_str());
    se = (TH1F*)hsignaleven->Clone("se");
    so = (TH1F*)hsignalodd->Clone("so");
    se->Scale(1./(se->Integral()));
    so->Scale(1./(so->Integral()));
    //    kss = hsignaleven->KolmogorovTest(hsignalodd);
    kss = se->KolmogorovTest(so);
    sa->Add(se);
    sa->Add(so);
    sa->Scale(1./(sa->Integral()));

    if(!regression){
    tree_test->Project(hbackgroundodd->GetName(),histogram.expr().c_str() ,selection_bkg.c_str());
    tree_train->Project(hbackgroundeven->GetName(),histogram.expr().c_str(),selection_bkg.c_str());
    be = (TH1F*)hbackgroundeven->Clone("be");
    bo = (TH1F*)hbackgroundodd->Clone("bo");
    be->Scale(1./(be->Integral()));
    bo->Scale(1./(bo->Integral())); 
    //    ksb = hbackgroundeven->KolmogorovTest(hbackgroundodd);
    ksb = be->KolmogorovTest(bo);
    ba->Add(be);
    ba->Add(bo);
    ba->Scale(1./(ba->Integral()));
    }

    int cut = 0;
    for (unsigned i=0; i<nbinsx-2; i++) {
      if (so->GetBinContent(i) > bo->GetBinContent(i))
	if (so->GetBinContent(i+1) > bo->GetBinContent(i+1))
	  if (so->GetBinContent(i+2) > bo->GetBinContent(i+2)) {
	    cut = i-1;
	    break;
	  }
    }

    double cutx=xmin+abs(xmax-xmin)*cut/nbinsx;

    float signi(0);
    if(so->Integral(so->FindBin(0),nbinsx-1)>0 && bo->Integral(bo->FindBin(0),nbinsx-1)>0)
      //      signi = so->Integral(so->FindBin(0),nbinsx-1)/bo->Integral(bo->FindBin(0),nbinsx-1);
      signi = so->Integral(cut,nbinsx-1)/bo->Integral(cut,nbinsx-1);

    // draw various nnouts for signal/background
    
    TCanvas* BDTOutput = new TCanvas("BDTOutput","BDT Output",300,0,800,600);
    setup_global_style();
    
    
    if(regression)
    BDTOutput->Divide(1,2);
    else BDTOutput->Divide(2,2);

    BDTOutput->cd(1);
    hsignaleven->SetTitle("");
    hsignaleven->SetLineColor(93);
    hsignaleven->SetXTitle((xtitle).c_str());
    hsignaleven->SetYTitle("norm. to unit area");
    TLegend *leg1 = new TLegend(0.65,0.8,0.85,0.9);
    if(lclass==false)
      leg1->AddEntry(hsignaleven,"training correct combination","l");
    else 
      leg1->AddEntry(hsignaleven,"training signal sample","l");
    hsignaleven->DrawNormalized("H");
    set_legend(leg1);
    
    BDTOutput->cd(2);
    hsignalodd->SetTitle("");

    hsignalodd->SetLineColor(99);
    hsignalodd->SetXTitle("BDT Output");
    hsignalodd->SetYTitle("norm. to unit area");
    TLegend *leg2 = new TLegend(0.65,0.8,0.85,0.9);
    if(lclass==false)
      leg2->AddEntry(hsignalodd,"test correct combination","l");
    else
      leg2->AddEntry(hsignalodd,"test signal sample","l");
    hsignalodd->DrawNormalized("H");
    set_legend(leg2);
    
    if(!regression){
    BDTOutput->cd(3);
    hbackgroundeven->SetTitle("");
    hbackgroundeven->SetLineColor(30);
    hbackgroundeven->SetXTitle("BDT Output");
    hbackgroundeven->SetYTitle("norm. to unit area");
    TLegend *leg3 = new TLegend(0.65,0.8,0.85,0.9);
    if(lclass==false)
      leg3->AddEntry(hbackgroundeven,"training wrong combination","l");
    else
      leg3->AddEntry(hbackgroundeven,"training background sample","l");
    hbackgroundeven->DrawNormalized("H");
    set_legend(leg3);
    
    BDTOutput->cd(4);
    hbackgroundodd->SetTitle("");
    hbackgroundodd->SetLineColor(60);
    hbackgroundodd->SetXTitle("BDT Output");
    hbackgroundodd->SetYTitle("norm. to unit area");
    TLegend *leg4 = new TLegend(0.65,0.8,0.85,0.9);
    if(lclass==false)
      leg4->AddEntry(hbackgroundodd,"test wrong combination","l");
    else
      leg4->AddEntry(hbackgroundodd,"test background sample","l");
    hbackgroundodd->DrawNormalized("H");
    set_legend(leg4);
    }
    

    // creating the final overtrain plot 
   float maxse,maxso,maxbe,maxbo;
    float MAX=0.;

    maxse = se->GetMaximum();
    maxso = so->GetMaximum();
    if(!regression){
      maxbe = be->GetMaximum();
      maxbo = bo->GetMaximum();
      if(maxbe>MAX) MAX = maxbe;
      if(maxbo>MAX) MAX = maxbo;  
    }
    
    if(maxse>MAX)
      MAX=maxse;
    if(maxso>MAX)
      MAX=maxso;
        
    TCanvas* Overtrain = new TCanvas("Overtrain","Overtrain",300,0,800,600);

    Overtrain->cd();
    //Overtrain->cd()->SetLogy();
    /*
    TPad* mainPad = (TPad*)gPad;

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
 

    //Overtrain->cd(1);
    */
    se->SetMaximum(1.5*MAX);
    se->SetMinimum(0);
    //se->GetYaxis()->SetRange(0.,1.);
    se->SetTitle("");
    se->SetXTitle((xtitle).c_str());
    se->SetYTitle("norm. to unit area");

    if(!regression){
    be->SetMaximum(1.5*MAX);
    be->SetMinimum(0);
    be->SetTitle("");
    be->SetXTitle((xtitle).c_str());
    be->SetYTitle("norm. to unit area");
    
    be->SetLineColor(920);
    be->SetLineWidth(1.5);
    be->SetFillColor(920);
    be->SetFillStyle(1001);
    be->Draw("H");

    bo->SetLineColor(922);
    bo->SetLineWidth(2.5);
    bo->Draw("sameP");
    TLegend *legtrbkg = new TLegend(0.58,0.80,0.80,0.83);
    if(lclass==false)
      legtrbkg->AddEntry(be,"#bf{#bf{training wrong combination}}","f");
    else
      legtrbkg->AddEntry(be,"#bf{#bf{training background sample}}","f");
    set_legend1(legtrbkg);
    
    TLegend *legtebkg = new TLegend(0.58,0.75,0.80,0.78);
    if(lclass==false)
      legtebkg->AddEntry(bo,"#bf{#it{test wrong combination}}","l");
    else
      legtebkg->AddEntry(bo,"#bf{#it{test background sample}}","l");
    set_legend1(legtebkg);

    TLatex KSBBox;
    KSBBox.SetNDC();
    KSBBox.SetTextSize(0.02);
    char KSBtext[255];
    sprintf(KSBtext,"KS_{bkg} = %3.1f %%",float(ksb*100));
    KSBBox.DrawLatex(0.84,0.81,KSBtext);
    
    }
    
    se->SetLineColor(kPink);
    se->SetLineWidth(1.5);
    se->SetFillColor(kPink);
    se->SetFillStyle(3004);
    if(!regression)se->Draw("sameH");
    else se->Draw("H");

    so->SetLineColor(kPink-1);
    so->SetLineWidth(2.5);
    // so->SetXTitle((xtitle).c_str());
    so->Draw("sameP");

    if(reso)
      get_resolution(so);

    TLegend *legtrsig = new TLegend(0.58,0.90,0.80,0.93);
    if(lclass==false)
      legtrsig->AddEntry(se,"#bf{#bf{training correct combination}}","f");
    else
      legtrsig->AddEntry(se,"#bf{#bf{training signal sample}}","f");
    set_legend1(legtrsig);
    TLegend *legtesig = new TLegend(0.58,0.85,0.80,0.88);
    if(lclass==false)
      legtesig->AddEntry(so,"#bf{#it{test correct combination}}","l");
    else
      legtesig->AddEntry(so,"#bf{#it{test signal sample}}","l");
    set_legend1(legtesig);
    
    TLatex KSSBox;
    KSSBox.SetNDC();
    KSSBox.SetTextSize(0.02);
    char KSStext[255];
    sprintf(KSStext,"KS_{sig} = %3.1f %%",float(kss*100));
    KSSBox.DrawLatex(0.84,0.91,KSStext);
    

    TLatex tex;
    tex.SetTextSize(0.04);
    tex.SetTextFont(42);
    tex.SetNDC(true);

    tex.DrawLatex(0.20,0.96,"#sqrt{s}=8 TeV");
    //tex.DrawLatex(0.36,0.96,"L=19.3 fb^{-1}");
    //tex.DrawLatex(0.360,0.96,"CMS Preliminary");
    tex.DrawLatex(0.20,0.89,"t(bl#nu)H(b#bar{b})");
    //tex.DrawLatex(0.20,0.84,"3t Bin (tight WP)");
    if(sm==false)
      tex.DrawLatex(0.20,0.84,"#kappa_{t}=-1, m_{H}=125 GeV");
    else
      tex.DrawLatex(0.20,0.84,"#kappa_{t}=+1, m_{H}=125 GeV");
    if(lclass==false){
      if(ttbar==false)
	tex.DrawLatex(0.20,0.79,"tH reco hypotheses");
      else
	tex.DrawLatex(0.20,0.79,"t#bar{t} reco hypotheses");
    }
    else
      tex.DrawLatex(0.20,0.79,"MVA classification");
    TLatex tex2;
    tex2.SetNDC(true);
    tex2.SetTextSize(0.04);
    tex2.SetTextFont(102);
    //tex2.SetTextFont(102);
    tex2.DrawLatex(0.20,0.79, textstring);

    /*
    
    //Overtrain->cd(2);
    mainPad->cd(2);

    TH1F* herrse = (TH1F*) se->Clone("herrse");
    TH1F* herrbe = (TH1F*) be->Clone("herrbe");
    TH1F* herrso = (TH1F*) so->Clone("herrso"); //treated as "data"
    TH1F* herrbo = (TH1F*) bo->Clone("herrbo"); //treated as "data"
    TH1F* hres = (TH1F*) se->Clone();
    TH1F* hres2 = (TH1F*) se->Clone();
    hres->Reset();
    hres2->Reset();
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



    for (int ibin=1;ibin<=hres->GetNbinsX();ibin++) {
      Float_t ndata  = herrbo->GetBinContent(ibin);
      Float_t nmc    = herrbe->GetBinContent(ibin);
      Float_t edata  = herrbo->GetBinError(ibin);
      if (ndata>0) {
	Float_t bin  = (ndata-nmc)/ndata;
	Float_t err  = nmc/ndata/ndata*edata;
	hres->SetBinContent(ibin,bin);
	hres->SetBinError(ibin,err);
      }
      else {
	hres->SetBinContent(ibin,0.0);
	hres->SetBinError(ibin,0.0);
      }
    }

    for (int ibin=1;ibin<=hres2->GetNbinsX();ibin++) {
      Float_t ndata  = herrso->GetBinContent(ibin);
      Float_t nmc    = herrse->GetBinContent(ibin);
      Float_t edata  = herrso->GetBinError(ibin);
      if (ndata>0) {
	Float_t bin  = (ndata-nmc)/ndata;
	cout << bin << endl;
	Float_t err  = nmc/ndata/ndata*edata;
	hres2->SetBinContent(ibin,bin);
	cout << hres2->GetBinContent(ibin) << endl;
	hres2->SetBinError(ibin,err);
      }
      else {
	hres2->SetBinContent(ibin,0.0);
	hres2->SetBinError(ibin,0.0);
      }
    }

    float mini = 99;
    for (int ibin=1;ibin<=hres2->GetNbinsX();ibin++) {
      cout << hres2->GetBinContent(ibin) << endl;
      if(mini>hres2->GetBinContent(ibin))
	mini = hres2->GetBinContent(ibin);
    }

    cout<< "Mini: "<< mini << endl;
    Double_t max=std::max(std::abs(hres->GetMinimum()),
			  std::abs(hres->GetMaximum()));
    cout<< hres2->GetMinimum() << endl;
    max *=1.05; //max = std::min(49.9,max);
    hres->SetMinimum(-4*max);
    hres->SetMaximum(4*max);
    hres->SetFillColor(60);
    hres->SetLineColor(60);
    hres->SetLineWidth(1);
    hres->SetMarkerStyle(kFullCircle);
    hres->SetMarkerSize(0.5);
    hres->GetYaxis()->SetTitleOffset(0.64);
    hres->SetYTitle("#frac{test-train}{train}");
    gPad->SetGridy();


    hres2->SetFillColor(99);
    hres2->SetLineColor(99);
    hres2->SetLineWidth(1);

    hres->Draw("HIST");
    hres2->Draw("HISTsame");


    TLegend* l2 = new TLegend(0.175,0.83,0.275,0.93);
    l2->SetFillColor(10); l2->SetLineColor(10); l2->SetBorderSize(0);
    l2->SetTextSize(0.06);
    l2->AddEntry(hres,"bkg","F");
    l2->Draw();
    
    
    TLegend* l3 = new TLegend(0.25,0.83,0.35,0.93);
    l3->SetFillColor(10); l3->SetLineColor(10); l3->SetBorderSize(0);
    l3->SetTextSize(0.06);
    l3->AddEntry(hres2,"sig","F");
    l3->Draw();
    */

    Overtrain->RedrawAxis();    

    cout << "--------------------------------------------------------------\n";
    cout << "| KS(s) = " << kss*100 << "% | KS(b) = " << ksb*100 << "% | S/B["<< cutx << "," << xmax <<"] = " << signi << "\n";
    cout << "--------------------------------------------------------------\n";	

  //
  // RUN ROOT APPLICATION
  //
    size_t pos2 = input.find(".root");
    input=input.substr(0,pos2);
    size_t pos = input.find_last_of("/"); 
    input=input.substr(pos+1,-1);

  if (!batch)
     app->Run();
  else
    Overtrain->SaveAs((folder+"overtrain_"+varexpr+".pdf").c_str());

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

//_______________________________________________________________________________
void set_legend(TLegend* leg)
{
  leg->SetFillColor(10);
  leg->SetLineColor(10);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.02);
  leg->Draw();
}

//_______________________________________________________________________________
void set_legend1(TLegend* leg)
{
  leg->SetFillColor(10);
  leg->SetLineColor(10);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.02);
  leg->Draw();
}

//_______________________________________________________________________________
void get_resolution(TH1F* h, float min, float max){
  //  float histmean = h->GetMean();
  //  float histrms = h->GetRMS();
  //  float histmeane = h->GetMeanError();
  //  float histrmse = h->GetRMSError();

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
  fit->SetLineWidth(2);
  fit->SetLineColor(color);
  double mean= fit->GetParameter(1);
  double variance =fit->GetParameter(2);
  fit->Draw("same");
  cout << "mean: " << mean << ", sigma: " << variance << endl;
  /*char name[100];
  sprintf(name,"#splitline{mean: %1.1f}{#sigma:        %1.1f}",mean, variance);
  TLatex* text = new TLatex(1.570061,13.08044,name);
  text->SetTextAlign(12);
  text->SetTextSize(0.03);
  text->SetTextFont(42);
  text->SetY(0.85-(float)loffset*0.08);
  loffset++;
  text->SetX(0.15);
  text->SetTextColor(color);
  text->Draw("same");*/
}
