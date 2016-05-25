 ////////////////////////////////////////////////////////////////////////////////////
 //
 // th_mctomc_x
 // --------------
 //
 //            04/28/2015 Benedikt Maier <benedikt.maier>@cern.ch
 //           
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
 #include <TGaxis.h>
 #include "TPluginManager.h"
 #include <TKey.h>
 #include <TList.h>
 #include <TText.h>
 #include <TPaveText.h>


//#include <RooHistError.h>

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


 using namespace std;



 ////////////////////////////////////////////////////////////////////////////////
 // helper function
 ////////////////////////////////////////////////////////////////////////////////

 double get_minimum(const TH1* h);
 bool   get_val_and_err(double& val, double& err,const string& expr);
 string get_leglabel(const vector<string>&labels,const string& sample);
 int    get_color(const vector<string>& colors,const string&sample);
 void   draw_stats(unsigned level,TH1* hdata,double xoffset=0.2);
 void   draw_labels(const vector<string>& labels,bool leginplot=true,bool res=true, bool draw_aMCatNLO=true, bool draw_mcresiduals=false);
 string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression"); 
 double  get_chi2test(TH1F* hh,THStack* hmc);
 double  get_kstest(TH1F* hh,THStack* hmc);
 double  return_sign(Double_t number);
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
   vector<string> signal    = cl.getVector<string>  ("signal",              "");
   double         signalfac = cl.getValue<double>   ("signalfac",           1.);
   string         mcref     = cl.getValue<string>   ("mcref",               "");
   string         mcstackname = cl.getValue<string> ("mcstackname",         "");
   string         mcrefname = cl.getValue<string>   ("mcrefname",           "");
   string         dirname   = cl.getValue <string>  ("dirname",          "utm");
   string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
   string         treename  = cl.getValue <string>  ("treename",           "t");
   string         datapath  = cl.getValue <string>  ("datapath",            "");
   string         mcpath    = cl.getValue <string>  ("mcpath",              "");
   string         mcsubpath = cl.getValue <string>  ("mcsubpath",           "");
   vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
   vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
   vector<string> datacuts  = cl.getVector<string>  ("datacuts",            "");
   vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");
   vector<string> mcweightsup=cl.getVector<string>  ("mcweightsup",         "");
   vector<string> mcweightsdown=cl.getVector<string>("mcweightsdown",       "");
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
   bool           overflow  = cl.getValue <bool>    ("overflow",          false);
   bool           underflow = cl.getValue <bool>    ("underflow",         false);
   bool           logx      = cl.getValue <bool>    ("logx",             false);
   bool           logy      = cl.getValue <bool>    ("logy",             false);
   bool           residuals = cl.getValue <bool>    ("residuals",         true);
   double         resmax    = cl.getValue <double>  ("resmax",             -1.);
   string         fitresids = cl.getValue <string>  ("fitresids",           "");
   unsigned       stats     = cl.getValue <unsigned>("stats",                0);
   double         lumi      = cl.getValue <double>  ("lumi",               0.0);
   bool           leginplot = cl.getValue <bool>    ("leginplot",        false);
   string         legtitle  = cl.getValue <string>  ("legtitle",            "");
   double         legx      = cl.getValue <double>  ("legx",              0.70);
   double         legy      = cl.getValue <double>  ("legy",              0.94);
   double         legw      = cl.getValue <double>  ("legw",              0.20);
   double         legh      = cl.getValue <double>  ("legh",             0.060);
   string         datalabel = cl.getValue <string>  ("datalabel",       "Data");
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
   bool           plotsys   = cl.getValue <bool>    ("plotsys",              0);
   string         dd_file   = cl.getValue <string>  ("dd_file",             "");
   string         dd_hist   = cl.getValue <string>  ("dd_hist",             "");
   bool           draw_aMCatNLO=cl.getValue<bool>   ("draw_aMCatNLO",        1);
   bool           poisson   = cl.getValue <bool>    ("poisson",              0);
   bool           normalized   = cl.getValue <bool> ("normalized",           0);
   bool           mc_residuals = cl.getValue <bool> ("mc_residuals",         0);
   string         restitle    = cl.getValue <string>("restitle",            "");
   string         subtract_1 = cl.getValue<string>  ("subtract_1",          "");
   string         subtract_2 = cl.getValue<string>  ("subtract_2",          "");
   bool           subtract   = cl.getValue<bool>    ("subtract",            "");
   bool           masterbool = cl.getValue<bool>    ("masterbool",           0);
   string         datstring = cl.getValue<string>("datstring",    "");
   string         datname = cl.getValue<string>("datname",    "");
   string         subtract_label = cl.getValue<string>("subtract_label",    "");
  

   if(!cl.check()) return 0;
   cl.print();

   if (data.empty()&&mcs.empty()){cout<<"provide -data and/or -mcs!"<<endl;return 0;}
   if (batch&&formats.size()==0) formats.push_back("pdf");
   if (output.empty()) {
     output = (varname.empty()) ? "DataToMC" : varname;
     output = (varname.empty() && data.empty()) ? "MC" : varname;

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

   //if (!mc_residuals) residuals = false;
   
   if(mc_residuals)
     residuals =mc_residuals;

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
     if (varname.empty())  histogram.setName("histogram");
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


   //
   // CREATE ROOT APPLICATION OBJECT, INITIALIZE STYLE
   //
   argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
   TApplication* app = new TApplication("utm_datatomc_x",&argc,argv);
   TH1::SetDefaultSumw2();	

   set_root_style();
   gStyle->SetOptStat(0);
   //gStyle->SetErrorX(0.);
   TColor::SetPalette(1,0);


   //
   // MAKE THE CANVAS, DRAW THE HISTOGRAM(S)
   //
   string cname   = dirname + "_" + histogram.name();
   int    cwidth  = (leginplot) ? 840 : 1000;
   int    cheight = 840;
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
   double legymin = (legtitle.empty()) ? legy : legy-0.02; if (residuals || mc_residuals) legymin =1.-1.33*(1-legymin);
   //double legymin = legy; if (residuals) legymin =1.-1.33*(1-legymin);
   double legxmax = (leginplot) ? legx+legw : 0.9999;
   double legymax = legymin - (mcsamples.size()+1)*legh;
   if(signalfac>1.)
     legymax = legymin - (mcsamples.size()+2)*legh;
   
   
   //TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax);
   
   TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax,legtitle.c_str());
   l->SetFillStyle(0); l->SetLineColor(10); l->SetBorderSize(0); if(stackmc) l->SetTextAlign(11);


   //
   // DATA
   //
   TH1F*  hdata(0);
   TH1F*  hreference(0);
   TH1F*  mcstack(0);
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
     l->AddEntry(hdata,datalabel.c_str(),"lp");
   }


   //
   // MC
   //
   string         mcsel;
   vector<TH1F*>  mchistos;
   vector<TH1F*>  mchistos_up;
   vector<TH1F*>  mchistos_down;
   vector<string> mcleglabels;
   vector<TH1F*>  signalhistos;

   // data-driven ttbar template
   TH1F* tmp1_hist(0);
   if(!dd_hist.empty())
     {
       TFile* dd_hfile = new TFile(dd_file.c_str());
       tmp1_hist=(TH1F*)dd_hfile->Get(dd_hist.c_str());
       tmp1_hist->SetFillColor(797);
       tmp1_hist->SetLineColor(kBlack);
       tmp1_hist->SetFillStyle(1001);

     }



   if (mcs.size()>0) {
     for (unsigned icut=0;icut<mccuts.size();icut++) {
       if (!mcsel.empty()) mcsel += "&&";
       mcsel += get_cut(mccuts,icut,"expression");
     }
     if (!tightsel.empty())
       mcsel = (mcsel.empty()) ? tightsel : mcsel + "&&" + tightsel;

     for (unsigned imc=0;imc<mcsamples.size();imc++) {
       MCSample     mcsample = mcsamples[imc];
       MCSample     mcsample_up = mcsamples[imc];
       MCSample     mcsample_down = mcsamples[imc];


       stringstream sshmcname_i; sshmcname_i<<"h"<<mcsample.name();



       // Comment B.Maier
       // Adding functionality to loop over weights vector to write out histogram entries for several weights (e.g. scale variations)


       //       for (int i = 0; i<mcweights.size(); i++)
       
       mcsample.setWeights(mcweights);



       if(plotsys)
	 mcsample.setSFup(mcweightsup);
       if(plotsys)
	 mcsample.setSFdown(mcweightsdown);


       TH1F* hmc_i = histogram.create(sshmcname_i.str());
       TH1F* hmc_i_up = histogram.create(sshmcname_i.str()+"_up");
       TH1F* hmc_i_down = histogram.create(sshmcname_i.str()+"_down");


       mcsample.projectSys(hmc_i->GetName(),hmc_i_up->GetName(),hmc_i_down->GetName(),histogram.expr(),mcsel);

       status->print_status((int)imc);

       if (histogram.variableBinWidth()) {
	 Histogram::divideBinContentByBinWidth(hmc_i);
	 Histogram::divideBinContentByBinWidth(hmc_i_up);
	 Histogram::divideBinContentByBinWidth(hmc_i_down);
       }

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
	 mchistos_up.push_back(hmc_i_up);
	 mchistos_down.push_back(hmc_i_down);
	 mcleglabels.push_back(get_leglabel(mclabels,mcsample.name()));
       }
     }
     string legstyle = (stackmc) ? "f" : "l";

     if(!reversemc){
       for (int iimc=mchistos.size()-1;iimc>=0;iimc--)
	 {
	   l->AddEntry(mchistos[iimc],mcleglabels[iimc].c_str(),legstyle.c_str());
	   if(iimc==mchistos.size()-1 && !dd_hist.empty())
	     {
	       l->AddEntry(tmp1_hist,"d.d. t#bar{t}","f");
	     }
	 }
     }
     else {
       for (int iimc=0;iimc<=mchistos.size()-1;iimc++)
	 {
	   l->AddEntry(mchistos[iimc],mcleglabels[iimc].c_str(),legstyle.c_str());
	   if(iimc==mchistos.size()-1 && !dd_hist.empty())
	     {
	       l->AddEntry(tmp1_hist,"d.d. t#bar{t}","f");
	     }
	 }
     }
   }

   if(!signal.empty()){
     stringstream sshsigname_i; sshsigname_i<<"hsig"<<signalmc[0].name();
     signalmc[0].setWeights(mcweights);
     TH1F* sig_i = histogram.create(sshsigname_i.str());
     signalmc[0].project(sig_i->GetName(),histogram.expr(),mcsel);

     if (histogram.variableBinWidth())  Histogram::divideBinContentByBinWidth(sig_i);

     sig_i->SetFillStyle(0);
     sig_i->SetLineColor(2);
     sig_i->SetLineWidth(2);
     signalhistos.push_back(sig_i);
     if(signalfac!=1){
       stringstream ss;
       ss<<signalfac;
       //l->AddEntry(signalhistos[0],(ss.str()+"x #sigma("+get_leglabel(mclabels,signalmc[0].name())+")").c_str(),"l");    
       l->AddEntry(signalhistos[0],(ss.str()+ "x tHq").c_str(),"l");//+ get_leglabel(mclabels,signalmc[0].name())+")").c_str(),"l");    
     }
     else
       l->AddEntry(signalhistos[0],get_leglabel(mclabels,signalmc[0].name()).c_str(),"l");    
   }


   double integraldata = 0;
   if (0!=hdata)
     integraldata = hdata->Integral();

   //
   // NORMALIZE MC
   //
   if (lumi>0.0){
     for (unsigned i=0;i<mchistos.size();i++) mchistos[i]->Scale(lumi);
     for (unsigned i=0;i<mchistos_up.size();i++) mchistos_up[i]->Scale(lumi);
     for (unsigned i=0;i<mchistos_down.size();i++) mchistos_down[i]->Scale(lumi);
     if(!signal.empty()){
       signalhistos[0]->Scale(signalfac*lumi);      
     }
   }
   else if (0!=hdata) {
     double Idata = hdata->Integral();
     double Imc   = 0.0;
     for (unsigned i=0;i<mchistos.size();i++) {
       if(dd_hist.empty())
	 Imc += mchistos[i]->Integral();
       else
	 {
	   mchistos[i]->Scale(19700);
	   Imc += mchistos[i]->Integral();
	 }
     }

     if(!dd_hist.empty()){
       Imc += tmp1_hist->Integral();
       TH1F* hdd = tmp1_hist;
       hdd->Scale(Idata/Imc);
       if (hdd->GetMaximum()>maximum) maximum = hdd->GetMaximum();
     }
     for (unsigned i=0;i<mchistos.size();i++) {
       TH1F* h = mchistos[i];
       h->Scale(Idata/Imc);
       TH1F* h_up = mchistos_up[i];
       h_up->Scale(Idata/Imc);
       TH1F* h_down = mchistos_down[i];
       h_down->Scale(Idata/Imc);
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
     cout << "Data          : " << integraldata << endl;
     cout << "Average weight: " << scaledInt/MCentries << endl;    
   }

   maximum = (histogram.logy()) ? 100.*maximum : 1.5*maximum;
   string datadrawopt = (mchistos.size()>0) ? "ESAME" : "E";


   //
   // PREPARE RESIDUALS
   //
   TPad* mainPad = (TPad*)gPad;
   if (residuals || mc_residuals) {
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
     gPad->SetBottomMargin(0.02);
     gPad->SetLeftMargin(leftMargin);
     gPad->SetRightMargin(rightMargin);

     mainPad->cd(2);
     if (mainPad->GetLogx()) gPad->SetLogx();
     gPad->SetBottomMargin(0.470);
     gPad->SetTopMargin(0.03);
     gPad->SetLeftMargin(leftMargin);
     gPad->SetRightMargin(rightMargin);
     gPad->SetFillStyle(0);
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



   double MCInt = 0.0;

   if (mchistos.size()>0) {
     if (stackmc) {
       if (0==hmc) hmc = new THStack("hMC","");
       for (unsigned i=0;i<mchistos.size();i++){
	 hmc->Add(mchistos[i]);
	 if(i==mchistos.size()-2 && !dd_hist.empty())
	   hmc->Add(tmp1_hist); //adding data-driven ttbar template
	 MCInt+= mchistos[i]->Integral();
       }
       if (0==hmc_up) hmc_up = new THStack("hmc_up","hmc_up");
       for (unsigned i=0;i<mchistos_up.size();i++) hmc_up->Add(mchistos_up[i]);
       if (0==hmc_down) hmc_down = new THStack("hmc_down","hmc_down");
       for (unsigned i=0;i<mchistos_down.size();i++) hmc_down->Add(mchistos_down[i]);

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
	     double val1  = herr->GetBinContent(ibin);
	     double eval1 = herr->GetBinError(ibin);
	     double val2  = mchistos[i]->GetBinContent(ibin);
	     double eval2 = mchistos[i]->GetBinError(ibin);
	     herr->SetBinContent(ibin,(val1+val2));
	     herr->SetBinError(ibin,sqrt(eval1*eval1+eval2*eval2));
	   }
	 }
	 if (0!=hmm) {
	   for (int ibin=0;ibin<herr->GetNbinsX();ibin++) {
	     double val1  = herr->GetBinContent(ibin);
	     double eval1 = herr->GetBinError(ibin);
	     double val2  = hmm ->GetBinContent(ibin);
	     double eval2 = hmm ->GetBinError(ibin);
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

       if(normalized){
       maximum=0.0;
       double h_integral=0;
       for (unsigned i=0;i<mchistos.size();i++) {
	 TH1F* h = mchistos[i];
	 string drawopt = (0!=hmm) ? "HISTSAME" : (i==0) ? "HIST E" : "HISTSAME E";
	 string samplename = mchistos[i]->GetName();
	 
	 cout << "Name: "<<samplename.c_str()<<endl;

	 

	 if(!(samplename.find("_sub")!=string::npos)){
	   //if(i==0){
	   if (h->GetEntries()>0) {
	     h_integral=h->Integral();
	     h->Scale(1.0/h->Integral());
	   }
	 }
	 else{
	 
	   if (h->GetEntries()>0) h->Scale(1.0/h_integral);
	 }
	 if (h->GetMaximum()>maximum) maximum=h->GetMaximum();
	 if (get_minimum(h) <minimum) minimum=get_minimum(h);
	 h->SetLineColor(h->GetFillColor());
	 h->SetLineWidth(1);
	 if(!(samplename.find("_sub")!=string::npos)){
	   h->SetLineStyle(0);
	 }
	 else{
	   
	   h->SetLineStyle(4);
	 }
	 
	 h->SetFillStyle(0);
	 h->GetXaxis()->SetLabelSize(0.055);
	 h->GetXaxis()->SetTitleSize(0.055);
	 h->GetYaxis()->SetLabelSize(0.055);
	 h->GetYaxis()->SetTitleSize(0.055);
	 h->GetYaxis()->SetTitleOffset(1.3);
	 h->Draw(drawopt.c_str());
       }
       maximum = (histogram.logy()) ? 50.*maximum : 1.5*maximum;
       if (!histogram.logy()) mchistos[0]->SetMinimum(0.0);
       else if (-1.0!=ymin)   mchistos[0]->SetMinimum(ymin);
       else if (minimum>0.0)  mchistos[0]->SetMinimum(minimum);
       TH1* h = (0!=hmm) ? hmm : mchistos[0]; h->SetMaximum(maximum);

       }
       

       else{
       
	 maximum=0.0;
	 double mcref_integral = 0;
	 double mcstack_integral = 0;
	 double ratio = 0;
	 double tth = 0;
	 double twhb = 0;
	 double wwh = 0;
	 
	 TH1F* hsubtract(0);
	 TH1F* hsubtract1(0);
	 TH1F* hsubtract2(0);
	 
       //double h_integral=0;
       for (unsigned i=0;i<mchistos.size();i++) {
	 TH1F* h = mchistos[i];
	 string drawopt = (0!=hmm) ? "HISTSAME" : (i==0) ? "HIST" : "HISTSAME";
	 string samplename = mchistos[i]->GetName();
	 
	 cout << samplename.c_str()<<endl;

	 
	 if(subtract && samplename.find(subtract_1.c_str())!=string::npos){
	   hsubtract=(TH1F*)h->Clone();
	   hsubtract1=(TH1F*)h->Clone();
	 }

	 if(subtract && samplename.find(subtract_2.c_str())!=string::npos)
           hsubtract2=(TH1F*)h->Clone();
	
	 double integral = h->Integral(1,h->GetNbinsX());
	 cout << "Integral: " << integral << endl;

	 if (h->GetMaximum()>maximum) maximum=h->GetMaximum();
	 if (get_minimum(h) <minimum) minimum=get_minimum(h);
	 h->SetLineColor(h->GetFillColor());
	 h->SetLineWidth(1);
	 if(!(samplename.find("_sub")!=string::npos)){
	   h->SetLineStyle(0);
	 }
	 else{
	   h->SetLineStyle(4);
	 }
	 
	 // Pick the right historgam for residuals
	 if(samplename.find(mcrefname.c_str())!=string::npos)
	   {
	     hreference=(TH1F*)h->Clone();
	     mcref_integral = hreference->Integral();	 
	   }


	 if(samplename.find(mcstackname.c_str())!=string::npos)
	   {
	     mcstack=(TH1F*)h->Clone();
	     mcstack_integral = mcstack->Integral();
	   }

	 if(samplename.find("hWWH_sub")!=string::npos)
	   wwh=h->Integral();
	     
	 if(samplename.find("htWHb_sub")!=string::npos)
           twhb=h->Integral();  
	 
	 if(samplename.find("httH_sub")!=string::npos)
           tth=h->Integral();



	 ratio = (mcref_integral>0. && mcstack_integral>0.) ? mcstack_integral/mcref_integral : ratio;
	 
	 h->SetFillStyle(0);
	 h->GetXaxis()->SetLabelSize(0.055);
	 h->GetXaxis()->SetTitleSize(0.055);
	 h->GetYaxis()->SetLabelSize(0.055);
	 h->GetYaxis()->SetTitleSize(0.055);
	 h->GetYaxis()->SetTitleOffset(1.3);
	 h->GetYaxis()->SetTitle("Events");

	 if (mc_residuals || residuals)
	   {
	     h->GetXaxis()->SetLabelOffset(99);
             h->GetXaxis()->SetTitleOffset(99);
	   }
	 h->Draw(drawopt.c_str());

  // NEW F. Demartin
  // mods to save histo's data to file
         string samp_name = samplename;
         samp_name.erase(samp_name.begin());
         string my_str = "histogram";
         size_t str_position = samp_name.find(my_str);
         samp_name.erase(str_position,my_str.length());
         cerr << "Processing " << samp_name << varexpr << " ... \n";

         
	 
	 string  outfilename = "mydata/" + datname + "_" + samp_name + ".dat";

	 if (!datstring.empty())
	   outfilename = "mydata/" + datname + "_" + samp_name + "_" + datstring + ".dat";


         ofstream out_stream;
         out_stream.open(outfilename.c_str());
         cerr << "Storing data in " << outfilename << endl;

         out_stream << "# Bin data format: xmin, xmax, y, yerr \n";
         // bin #0 is the underflow, bin #1-#nbins are the proper histo, bin #nbins+1 is the overflow
         // GetSize gets the total number of entries, including the two underflow and overflow bins
         int h_nbins = h->GetSize() - 2;  
         for( int ibin = 1 ; ibin <= h_nbins ; ibin++ )
         { 
           out_stream << setprecision(9) << scientific
                      << h->GetXaxis()->GetBinLowEdge(ibin) << "\t" 
                      << h->GetXaxis()->GetBinUpEdge(ibin)  << "\t"
                      << h->GetBinContent(ibin)  << "\t" 
                      << h->GetBinError(ibin) << "\n";
         }
         out_stream.close();
         cerr << " done. \n";
  // 
  //
 
	 if(i==mchistos.size()-1 && subtract){
	   hsubtract->Add(hsubtract2,-1); 
	   hsubtract->SetFillStyle(0);
	   hsubtract->SetLineStyle(3);
	   hsubtract->SetLineColor(kGray+2);
	   hsubtract->Draw("HISTSAME");
	   if(masterbool)
	     l->AddEntry(hsubtract,subtract_label.c_str(),"l");
	 }

	 char line[2555];
	 sprintf(line,"#bf{HWWbb: } %3.1f",mcref_integral);
	 
	 if(ratio>0 && masterbool){
	   TLatex tex;
	   tex.SetNDC(true);
	   tex.SetTextAlign(31);
	   tex.SetTextSize(0.04);
	   tex.SetTextFont(42);	 
	   tex.DrawLatex(0.77,0.84,line);
	   char line_2[2555];
	   sprintf(line_2,"#bf{Sum: } %3.1f",mcstack_integral);
	   TLatex tex_2;
           tex_2.SetNDC(true);
           tex_2.SetTextAlign(31);
           tex_2.SetTextSize(0.04);
           tex_2.SetTextFont(42);
           tex_2.DrawLatex(0.77,0.79,line_2);
	   char line_3[2555];
           sprintf(line_3,"#bf{Sum/HWWbb: } %3.3f",ratio);
           TLatex tex_3;
           tex_3.SetNDC(true);
           tex_3.SetTextAlign(31);
           tex_3.SetTextSize(0.04);
           tex_3.SetTextFont(42);
           tex_3.DrawLatex(0.77,0.74,line_3);

	 }
       }
       maximum = (histogram.logy()) ? 50.*maximum : 1.5*maximum;
       if (!histogram.logy()) mchistos[0]->SetMinimum(0.0);
       else if (-1.0!=ymin)   mchistos[0]->SetMinimum(ymin);
       else if (minimum>0.0)  mchistos[0]->SetMinimum(minimum);
       TH1* h = (0!=hmm) ? hmm : mchistos[0]; h->SetMaximum(maximum);



       char line_4[2555];
       sprintf(line_4,"#bf{ttH: } %3.3f, #bf{tWHb: } %3.3f, #bf{n.r.: } %3.3f",tth,twhb,wwh);
       TLatex tex_4;
       tex_4.SetNDC(true);
       tex_4.SetTextAlign(31);
       tex_4.SetTextSize(0.04);
       tex_4.SetTextFont(42);

       if(masterbool)
	 tex_4.DrawLatex(0.77,0.69,line_4);


       }
     
     
     }



   }

   if(!signal.empty()){
     signalhistos[0]->Draw("HISTSAME");
   }

   //
   // DRAW DATA
   //


   if(hdata&&poisson){
     if (mchistos.size()==0) hdata->SetMaximum(maximum);
     TGraphAsymmErrors * gdata = new TGraphAsymmErrors(hdata);
    for (int i = 0; i < gdata->GetN(); ++i) {
      double y=gdata->GetY()[i]; 
      double ym, yp;
      double nSigma = 1.;
      //RooHistError::instance().getPoissonInterval(y,ym,yp,nSigma);
      double yerrplus = yp - y;
      double yerrminus = y - ym;
      gdata->SetPointEYlow(i,yerrminus);
      gdata->SetPointEYhigh(i,yerrplus);
    }
    
    gdata->Draw("PZ"); 
    
  }
  
  if (hdata&&!poisson) {
    //    if (!stackmc&&hdata->GetEntries()>0) hdata->Scale(1.0/hdata->Integral()); now earlier for Chi2 (Hauke)
    hdata->Draw(datadrawopt.c_str());
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
  if (hmc&&hdata) {
    h->SetXTitle(hdata->GetXaxis()->GetTitle());
    h->SetYTitle(hdata->GetYaxis()->GetTitle());
  }
  if (!ytitle.empty()) {
    if(0!=hdata) hdata->GetYaxis()->SetTitle("");
    h->SetYTitle(ytitle.c_str());
  }
  l->Draw();
  draw_labels(labels,leginplot,residuals,draw_aMCatNLO,mc_residuals);


  //
  // DRAW RESIDUALS
  //

  //cout << residuals << endl;
  if (residuals) {

    


    mainPad->cd(2);
    TH1F* hres = (TH1F*)mcstack->Clone();
    

    hres->Reset();
    

    hres->GetYaxis()->CenterTitle();
    hres->GetYaxis()->SetTitleSize(0.11);
    hres->GetYaxis()->SetTitleOffset(0.66);
    hres->GetYaxis()->SetLabelSize(0.12);
    hres->GetYaxis()->SetNdivisions(505);
    hres->GetXaxis()->SetTitleSize(0.19);
    hres->GetXaxis()->SetLabelSize(0.18);
    hres->GetXaxis()->SetTitleOffset(1);
    hres->GetXaxis()->SetLabelOffset(0.006);
    hres->GetXaxis()->SetNdivisions(505);
    hres->GetXaxis()->SetTickLength(hres->GetXaxis()->GetTickLength() * 3.0 );
    hres->SetYTitle(restitle.c_str());
    if(!dd_hist.empty())
      hres->SetYTitle("#frac{Data-Pred.}{Pred.}");
    

    for (int ibin=1;ibin<=hres->GetNbinsX();ibin++) {
      Double_t nmc1  = mcstack->GetBinContent(ibin);
      Double_t nmc2   = hreference->GetBinContent(ibin);
      Double_t emc1  = mcstack->GetBinError(ibin);

      cout << "Referenz: " << nmc2 << " Approximation: " << nmc1 << endl;
      if (nmc2>0) {
	Double_t bin  = (nmc1)/nmc2;
	Double_t err  = emc1/nmc1;
	
	cout << "bin: "<< bin << endl;

	if(nmc2!=0)
	  {
	    hres->SetBinContent(ibin,bin);
	    hres->SetBinError(ibin,err);
	  }
	else
	  {
	    hres->SetBinContent(ibin,-999);
	    hres->SetBinError(ibin,0);
	  }
	

      }
      else {
	hres->SetBinContent(ibin,-99.0);
	hres->SetBinError(ibin,-99.0);

      }
    }
    Double_t max=std::max(std::abs(hres->GetMinimum()),
			  std::abs(hres->GetMaximum()));
    max *=1.05; max = std::min(49.9,max);
    if(max=49.9) max = -99;
    if(resmax>0.) max = resmax;
    double helpmax=max - 1;
    hres->SetMinimum(1-std::abs(helpmax));
    hres->SetMaximum(1+std::abs(helpmax)*1.2);
    hres->SetMarkerStyle(kFullCircle);
    hres->SetMarkerSize(0);
    //hres->SetMarkerSize(1);
    gPad->SetGridy();

    
 
    hres->Draw("EPHIST");
      
  


  }
  c->cd();

  TPad* b = new TPad("b", "b",0.10501342,0.246621,0.1401047,0.2814685);
  b->SetBorderMode(0);
  
  //b->Draw();
  
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
double get_chi2test(TH1F* hh,THStack* hmc)
{
  double result(-999.);
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
double get_kstest(TH1F* hh,THStack* hmc)
{
  double result(-999.);
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
void draw_labels(const vector<string>& labels,bool leginplot,bool res, bool draw_aMCatNLO, bool draw_mcresiduals)
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
    
    TLatex tex2;
    tex2.SetTextSize(0.03);
    tex2.SetTextAngle(90);
    tex2.SetNDC(true);
    if (draw_aMCatNLO) 
      {
	if(!draw_mcresiduals)
	  tex2.DrawLatex(0.985,0.23,"MadGraph5_aMC@NLO");  
	else 
	  tex2.DrawLatex(0.985,0.02,"MadGraph5_aMC@NLO");
      }

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
double return_sign(Double_t number)
{
  double sign=0;
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
