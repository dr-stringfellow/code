
 ////////////////////////////////////////////////////////////////////////////////////
 //
 // th_datatomc_x
 // --------------
 //
 // Plotting tool, mainly for data/MC comparisons.
 //
 //            02/12/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
 //            Modified by Christian Boeser, Simon Fink, Kevin Floeh, Hauke Held, Benedikt Maier
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
 #include <TRandom3.h>

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
 #include <sys/stat.h> // for mkdir


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

   vector<string> data      = cl.getVector<string>  ("data",                "");  // name of the data files
   vector<string> mcs       = cl.getVector<string>  ("mcs",                 "");  // name of the mc files
   vector<string> signal    = cl.getVector<string>  ("signal",              "");  // allows you to specify your signal
   double         signalfac = cl.getValue<double>   ("signalfac",           1.);  // scale factor for your signal
   string         mcref     = cl.getValue<string>   ("mcref",               "");  // draw residuals to a specific background
   string         dirname   = cl.getValue <string>  ("dirname",          "utm");  // tree directory name
   string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");  // file suffix
   string         treename  = cl.getValue <string>  ("treename",           "t");  // name of the tree
   string         datapath  = cl.getValue <string>  ("datapath",            "");  // path to your data files
   string         mcpath    = cl.getValue <string>  ("mcpath",              "");  // path to your mc files
   string         mcsubpath = cl.getValue <string>  ("mcsubpath",           "");  // possible sub folder in your mc path
   vector<string> cuts      = cl.getVector<string>  ("cuts",                "");  // applied cuts
   vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");  // cuts only applied to mc
   vector<string> datacuts  = cl.getVector<string>  ("datacuts",            "");  // cuts only applied to data
   vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");  // weights applied to mc
   vector<string> mcweightsup=cl.getVector<string>  ("mcweightsup",         "");  // upward weights applied to mc
   vector<string> mcweightsdown=cl.getVector<string>("mcweightsdown",       "");  // downward weights applied to mc
   vector<string> ignore    = cl.getVector<string>  ("ignore",              "");  // ignore specific cuts
   string         tightcut  = cl.getValue <string>  ("tightcut",         "iso");  // allows to specify a tight selection
   string         mm        = cl.getValue <string>  ("mm",                  "");  // use matrix method (currently unused)
   string         epssig    = cl.getValue <string>  ("epssig", "0.99 +- 0.010");  // parameter for matrix method (currently unused)
   string         epsqcd    = cl.getValue <string>  ("epsqcd", "0.40 +- 0.025");  // parameter for matrix method (currently unused)
   string         varname   = cl.getValue <string>  ("varname",             "");  // name of the plotted variable (see src/Histogram.cc)
   string         varexpr   = cl.getValue <string>  ("varexpr",             "");  // custom variable definition
   string         xtitle    = cl.getValue <string>  ("xtitle",              "");  // title of x axis
   string         ytitle    = cl.getValue <string>  ("ytitle",    "Events/Bin");  // title of y axis
   unsigned       nbinsx    = cl.getValue <unsigned>("nbinsx",               0);  // number of bins
   vector<double> binsx     = cl.getVector<double>  ("binsx",               "");  // set in width
   double         xmin      = cl.getValue <double>  ("xmin",             -1E10);  // minimum on x axis
   double         xmax      = cl.getValue <double>  ("xmax",             +1E10);  // maximum on x axis
   double         ymin      = cl.getValue <double>  ("ymin",               -1.);  // minimum for y axis
   bool           overflow  = cl.getValue <bool>    ("overflow",         false);  // possible overflow bin
   bool           underflow = cl.getValue <bool>    ("underflow",        false);  // possible underflow bin
   bool           logx      = cl.getValue <bool>    ("logx",             false);  // logarithmic x axis
   bool           logy      = cl.getValue <bool>    ("logy",             false);  // logarithmic y axis
   bool           residuals = cl.getValue <bool>    ("residuals",         true);  // draw residuals
   double         resmax    = cl.getValue <double>  ("resmax",             -1.);  // maximum of residuals - i.e. y-axis range for the ratio plot
   string         fitresids = cl.getValue <string>  ("fitresids",           "");  // fit function to residuals
   unsigned       stats     = cl.getValue <unsigned>("stats",                0);  // draw ROOT stats
   double         lumi      = cl.getValue <double>  ("lumi",               0.0);  // set luminosity (lumi 0: scale to data)
   double         log_min   = cl.getValue <double>  ("log_min",           0.01);  // set minimum for logarithmic plot
   int            canv_width = cl.getValue <int>    ("canv_width",          -1);  // canvas width (-1: default values)
   int            canv_height= cl.getValue <int>    ("canv_height",         -1);  // canvas height (-1: default values)
   bool           leginplot = cl.getValue <bool>    ("leginplot",        false);  // draw legend inside plot
   double         legx      = cl.getValue <double>  ("legx",             0.772);  // set x position of legend
   double         legy      = cl.getValue <double>  ("legy",              0.93);  // set y position of legend
   double         legw      = cl.getValue <double>  ("legw",              0.25);  // set width of legend
   double         legh      = cl.getValue <double>  ("legh",             0.155);  // set height of single legend entry
   string         datalabel = cl.getValue <string>  ("datalabel",       "Data");  // set label for data entry in legend
   vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");  // set label for mc entries in legend
   vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");  // set color for mc entries in legend
   vector<string> labels    = cl.getVector<string>  ("labels",              "");  // set labels in plot
   bool           stackmc   = cl.getValue <bool>    ("stackmc",           true);  // draw stacked histogram
   bool           reversemc = cl.getValue <bool>    ("reversemc",        false);  // reverse ordering of mc
   bool           batch     = cl.getValue <bool>    ("batch",            false);  // prevent opening of root display 
   vector<string> formats   = cl.getVector<string>  ("formats",             "");  // output format
   string         opath     = cl.getValue <string>  ("opath",               "");  // output path
   string         output    = cl.getValue <string>  ("output",              "");  // name of output file
   vector<string> prefix    = cl.getVector<string>  ("prefix",              "");  // add prefix to output file
   vector<string> suffix    = cl.getVector<string>  ("suffix",              "");  // add suffix to output file
   bool           chi2test  = cl.getValue <bool>    ("chi2test",             0);  // show chi2test of mc to data agreement
   bool           kstest    = cl.getValue <bool>    ("kstest",               0);  // show kstest of mc to data agreement
   bool           mcuncert  = cl.getValue <bool>    ("mcuncert",             0);  // show statistical mc uncertainties
   bool           printdiff = cl.getValue <bool>    ("printdiff",            0);  // print the total difference of mc to data
   bool           printint  = cl.getValue <bool>    ("printint",             0);  // print integral and scalefactors
   bool           plotsys   = cl.getValue <bool>    ("plotsys",              0);  // plot systematic uncertainties (when using different up and down sys.)
   string         dd_file   = cl.getValue <string>  ("dd_file",             "");  // draw histograms from file
   string         dd_hist   = cl.getValue <string>  ("dd_hist",             "");  // name of histogram from file
   bool           poisson   = cl.getValue <bool>    ("poisson",              0);  // use poissonian errors for error bars
   vector<string> binlabels  = cl.getVector<string> ("binlabels",           "");  // give specific labels to bins
   bool           pseudodata = cl.getValue <bool>   ("pseudodata",       false);  // true if pseudodata should be plotted
   string         up_uncfile= cl.getValue<string>   ("up_uncfile",          "");
   string         down_uncfile= cl.getValue<string> ("down_uncfile",        "");


   if(!cl.check()) return 0;
   cl.print();

   
   new TColor(1101,0.17250549177124488, 0.11951843162770594, 0.24320155229883056);
   new TColor(1102,0.2217962654723154, 0.23841378594571613, 0.39797674055755683);
   new TColor(1103,0.24670155725826665, 0.37340824813905654, 0.49725690696587516);
   new TColor(1104,0.2884627663592704, 0.5163814459748142, 0.5434217716422115);
   new TColor(1105,0.3720866446574984, 0.6378633419526029, 0.5550368905837924);
   new TColor(1106,0.5109365778646006, 0.7367198696575319, 0.5730408794126332);
   new TColor(1107,0.6825187635707243, 0.810691907283208, 0.6352470180118206);
   new TColor(1108,0.8423298817793848, 0.8737404427964184, 0.7524954030731037);  
   
   new TColor(1201,0.1762632810610893, 0.153368459294816, 0.051392720977734827
	      ); 
   new TColor(1202,0.1395347580721531, 0.3211810810337362, 0.17839056229108577
	      );
   new TColor(1203,0.21837465717034935, 0.3933880667740738, 0.46734715937548776
	       );
   new TColor(1204,0.5114059777263529, 0.38716036176669955, 0.6284923910586768
	      );
   new TColor(1205,0.7261878969369367, 0.46229568085632006, 0.5309905852356823
	      );
   new TColor(1206,0.6959560204049361, 0.6608936306892899, 0.4751479973840161
	      );
   new TColor(1207,0.640859268860801, 0.8227315228544844, 0.6842875826128113
	      );
   new TColor(1208,0.7794147710949635, 0.8747147622804028, 0.9188565282197311
	      );
   
   new TColor(1308,0.7686274509803922, 0.3058823529411765, 0.3215686274509804
              );
   new TColor(1307,0.39215686274509803, 0.7098039215686275, 0.803921568627451
              );
   new TColor(1306,0.360784313725490,0.622222222222222,0.766013071895425
	      );
   new TColor(1305,0.329411764705882,0.534640522875817,0.728104575163399
              );
   new TColor(1304,0.2980392156862745, 0.4470588235294118, 0.6901960784313725
              );
   new TColor(1303,0.3333333333333333, 0.6588235294117647, 0.40784313725490196
              );
   new TColor(1302,0.5058823529411764, 0.4470588235294118, 0.6980392156862745
              );
   new TColor(1301,0.8, 0.7254901960784313, 0.4549019607843137
              );



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
     if (!opath.empty()) {
       output = opath + "/" + output;
       mkdir(opath.c_str(), 0777);
     }
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
   TGaxis::SetMaxDigits(3);
   TGaxis::SetMaxDigits(2);
   TGaxis::SetExponentOffset(0.01, 0, "x"); // Y and Y offset for X axis 


   //
   // MAKE THE CANVAS, DRAW THE HISTOGRAM(S)
   //
   string cname   = dirname + "_" + histogram.name();
   int    cwidth  = (leginplot) ? 1000 : 1000;
   int    cheight = 800;
   
   if(canv_width != -1)
     cwidth = canv_width;
   if(canv_height != -1)
     cheight = canv_height;

   TCanvas* c = new TCanvas(cname.c_str(),cname.c_str(),cwidth,cheight);
   if (!leginplot) gPad->SetRightMargin(1-legx+0.01);
   if (histogram.logx()) gPad->SetLogx();
   if (histogram.logy()) gPad->SetLogy();

   // Prepare for external uncertainty file
   double uperror[nbinsx];
   double downerror[nbinsx];


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

   double legxmin = (leginplot) ? legx : legx;// : 0.825;
   double legymin = legy; 
   
   if (residuals)
     legymin = 1. - 1.33*(1-legymin);
   double legxmax = (leginplot) ? legx+legw : 0.9999;
   double legymax = (stackmc) ? legymin - (mcsamples.size()+1)*legh : legymin - (mcsamples.size()-2)*legh;
   if(signalfac>1.)
     legymax = legymin - (mcsamples.size()+2)*legh;
   //if (leginplot&&legymax<0.6) legymax = 0.6;
   TLegend* l = new TLegend(legxmin,legymin,legxmax,legymax);
   l->SetFillColor(10); l->SetLineColor(10); l->SetBorderSize(0); if(stackmc) l->SetTextAlign(11);
   l->SetTextSize(0.054);

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
     hdata->SetMarkerSize(1.);
     maximum = hdata->GetMaximum();
     if (pseudodata == true)  datalabel = "Pseudodata";
     l->AddEntry(hdata,datalabel.c_str(),"pe");
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
	 hmc_i->SetLineColor(12);
	 //	 hmc_i->SetLineColor(get_color(mccolors,mcsample.name()));
	 hmc_i->SetLineWidth(1);
	 mchistos.push_back(hmc_i);
	 mchistos_up.push_back(hmc_i_up);
	 mchistos_down.push_back(hmc_i_down);
	 mcleglabels.push_back(get_leglabel(mclabels,mcsample.name()));
       }
     }
     string legstyle = (stackmc) ? "f" : "l";
     for (int iimc=mchistos.size()-1;iimc>=0;iimc--)
       {
	 l->AddEntry(mchistos[iimc],mcleglabels[iimc].c_str(),legstyle.c_str());
	 if(iimc==mchistos.size()-1 && !dd_hist.empty())
	   {
	     l->AddEntry(tmp1_hist,"d.d. t#bar{t}","f");
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
     cout << endl << "------------------------------" << endl;
     cout << "------------------------------" << endl;
     cout << " Integrals: " << endl;
     
     for (unsigned i=0;i<mchistos.size();i++){
       scaledInt += mchistos[i]->Integral();
       MCentries += mchistos[i]->GetEntries();
       cout << mchistos[i]->GetName() << " : " << mchistos[i]->Integral() << endl;       
     }    
     cout << "------------------------------" << endl;
     cout << "Integral      : " << scaledInt << endl;
     cout << "Data          : " << integraldata << endl;
     cout << "Average weight: " << scaledInt/MCentries << endl; 
     cout << "------------------------------" << endl;
     cout << "------------------------------" << endl;
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
     leftMargin = leftMargin/1.2;

     mainPad->Divide(1,2,0.01,0.0);

     mainPad->GetPad(1)->SetFillColor(0);
     mainPad->GetPad(2)->SetFillColor(0);
     mainPad->GetPad(1)->SetPad(0.0,0.33,1.0,1.0);
     mainPad->GetPad(2)->SetPad(0.0,0.0,1.0,0.3);

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
	   sstest<<"0.60:0.86:42:KS="
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
	 sstest<<"0.60:0.86:42:KS="
	       <<setprecision(3)<<hdata->KolmogorovTest(hmm);
	 labels.push_back(sstest.str());
       }
     }
   }



   float MCInt = 0.0;

   if (mchistos.size()>0) {
     if (stackmc) {
       if (0==hmc) hmc = new THStack("hMC","");
//	generate pseudodata
	int nbins= mchistos[0]->GetNbinsX();
	float bincontent[nbins]={0};
	TRandom3 r;
	r.SetSeed(5);
	for (unsigned i=0;i<mchistos.size();i++){
	 	hmc->Add(mchistos[i]);
		if (pseudodata == true){
			for (int j = 0; j<nbins; j++){
	        		bincontent[j]+= mchistos[i]->GetBinContent(j);
        		        if (i==mchistos.size()-1){
				        bincontent[j]-= mchistos[i]->GetBinContent(j);
					bincontent[j]=r.Poisson(bincontent[j]);
					hdata->SetBinContent(j, bincontent[j]);
					hdata->SetMarkerStyle(30);
					hdata->SetMarkerSize(2);
					
        			}
			}
		}
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
       if (logy) hmc->SetMinimum(log_min);
       hmc->SetMaximum(maximum);
       hmc->GetHistogram()->SetMinimum(minimum);
       if(logy) hmc->GetHistogram()->SetMinimum(log_min);
       hmc->GetHistogram()->SetMaximum(maximum);

       if (binlabels.size()){
	 for (int i=0;i<binlabels.size();i++){
	   hmc->GetXaxis()->SetBinLabel(i+1,binlabels[i].c_str());
	 }
       }

       if (chi2test&&hdata!=0) {
	 stringstream sstest;
	 sstest<<((chi2test&&kstest) ? "0.62" : "0.77")
	       <<":0.96:42:#chi^{2}="
	       <<setprecision(4)<<get_chi2test(hdata,hmc);
	 labels.push_back(sstest.str());
       }
       if (kstest&&hdata!=0) {
	 stringstream sstest;
	 sstest<<"0.60:0.86:42:KS="
	       <<setprecision(3)<<get_kstest(hdata,hmc);
	 labels.push_back(sstest.str());
       }
       // calculate and draw mcuncertainty
       if (mcuncert) {
	 TH1F* herr = (TH1F*) mchistos[0]->Clone("herr");
	 for (unsigned i=1;i<mchistos.size();i++) {
	   for (int ibin=1;ibin<=mchistos[i]->GetNbinsX();ibin++) {
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




	 //
	 // FUNCTIONALITY TO READ IN EXTERNAL UNCERTAINTY FILES -- MUST HAVE SAME BINNING AND CALCULATED W.R.T. THE ENTIRE MC STACK
	 //

	 std::ifstream my_upfile(up_uncfile.c_str());
	 std::ifstream my_downfile(down_uncfile.c_str());
	 std::string line;
	 unsigned linecounter = 0;
	  
	 // up file
	 while (std::getline(my_upfile, line)){
	   if (linecounter!=0)
	     {
	       std::istringstream iss(line);

	       std::string up_col1;
	       std::string up_col2;
	       std::string up_col3;
	       std::getline(iss, up_col1, ' ');
	       std::getline(iss, up_col2, ' ');
	       std::getline(iss, up_col3, ' ');
	             
	       double up_d = atof(up_col3.c_str());
	       uperror[linecounter]=up_d;

	       //       cout << "3rd column " << up_d << endl;
	     }  
	   linecounter++;
	 }

	 linecounter = 0;
	 // down file
	 while (std::getline(my_downfile, line)){
	   if (linecounter!=0)
	     {
	       std::istringstream iss(line);

	       std::string down_col1;
	       std::string down_col2;
	       std::string down_col3;
	       std::getline(iss, down_col1, ' ');
	       std::getline(iss, down_col2, ' ');
	       std::getline(iss, down_col3, ' ');
	             
	       double down_d = atof(down_col3.c_str());
	       downerror[linecounter]=down_d;

	       //cout << "3rd column " << down_d << endl;
	     }  
	   linecounter++;
	 }
	  
	 if (!up_uncfile.empty() && !down_uncfile.empty()){
	      
	   for (int ibin=1;ibin<=herr->GetNbinsX();ibin++){
	     //Float_t tmp_bincontent = herr->GetBinContent(ibin);
	     Float_t tmp_staterror  = herr->GetBinError(ibin);
	     Float_t tmp_systerror  = abs(uperror[ibin]-(uperror[ibin]+downerror[ibin])/2);
	     herr->SetBinContent(ibin,(uperror[ibin]+downerror[ibin])/2);
	     herr->SetBinError(ibin,sqrt(tmp_systerror*tmp_systerror+tmp_staterror*tmp_staterror));
	     //cout << herr->GetBinContent(ibin) << endl;
	   }

	 }

 
	  
	 //
	 //
	 //





	 herr->Draw("error sameE2");

	 if(up_uncfile.empty())
	   l->AddEntry(herr,"MC stat.","F");
	 else
	   l->AddEntry(herr,"Stat.+syst.","F");

	 if(signalfac!=1 && signalfac>0.){
	   stringstream ss;
	   ss<<signalfac;
	   l->AddEntry(signalhistos[0],(ss.str()+ "x tH").c_str(),"l");//+ get_leglabel(mclabels,signalmc[0].name())+")").c_str(),"l");    
	      
	   //l->AddEntry(signalhistos[0],(ss.str()+"x #sigma("+get_leglabel(mclabels,signalmc[0].name())+")").c_str(),"l");    
	 }
	 else if (signalfac==1)
	   l->AddEntry(signalhistos[0],get_leglabel(mclabels,signalmc[0].name()).c_str(),"l");    
      
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
	 h->SetLineWidth(1);
	 h->SetFillStyle(0);
	 h->GetXaxis()->SetLabelSize(0.055);
	 h->GetXaxis()->SetTitleSize(0.055);
	 h->GetYaxis()->SetLabelSize(0.055);
	 h->GetYaxis()->SetTitleSize(0.055);
	 h->GetYaxis()->SetTitleOffset(0.85);
	 h->Draw(drawopt.c_str());
       }
       maximum = (histogram.logy()) ? 50.*maximum : 1.5*maximum;
       if (!histogram.logy()) mchistos[0]->SetMinimum(0.0);
       else if (-1.0!=ymin)   mchistos[0]->SetMinimum(ymin);
       else if (minimum>0.0)  mchistos[0]->SetMinimum(minimum);
       TH1* h = (0!=hmm) ? hmm : mchistos[0]; h->SetMaximum(maximum);
     }
   }

   if(!signal.empty() && signalfac>0.){
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
    h->GetYaxis()->SetNoExponent(true);
    h->GetXaxis()->SetNoExponent(true);
    hdata->GetXaxis()->SetNoExponent(true);
    h->GetYaxis()->SetTitleOffset(0.85);
    if (binlabels.size()){
      for (int i=0;i<binlabels.size();i++){
	h->GetXaxis()->SetBinLabel(i+1,binlabels[i].c_str());
      }
    }
    if (!stackmc) h->GetYaxis()->SetTitleOffset(1.45);
  }
  l->Draw();
  draw_labels(labels,leginplot,residuals);


  //
  // DRAW RESIDUALS
  //
  if (residuals) {
    mainPad->cd(2);
    

    TH1F* hres = (TH1F*)hdata->Clone();
    TH1F* hsysup = (TH1F*)hdata->Clone();
    TH1F* hsysdown = (TH1F*)hdata->Clone();
    TH1F* herrges = (TH1F*)hdata->Clone();
    TH1F* herrges2 = (TH1F*)hdata->Clone();
    TH1F* herrgesup = (TH1F*)hdata->Clone();
    TH1F* herrgesdown = (TH1F*)hdata->Clone();
    TH1F* hsysstat = (TH1F*)hdata->Clone();


    hres->Reset();
    hsysup->Reset();
    hsysdown->Reset();
    herrges->Reset();
    herrges2->Reset();
    herrgesup->Reset();
    herrgesdown->Reset();
    hsysstat->Reset();

    hres->GetYaxis()->CenterTitle();
    hres->GetYaxis()->SetTitleSize(0.11);
    hres->GetYaxis()->SetLabelOffset(0.004);
    hres->GetYaxis()->SetTitleOffset(0.5);
    hres->GetYaxis()->SetLabelSize(0.10);
    hres->GetYaxis()->SetNdivisions(505);
    hres->GetXaxis()->SetTitleSize(0.14);
    hres->GetXaxis()->SetLabelSize(0.13);
    hres->GetXaxis()->SetTitleOffset(1);
    hres->GetXaxis()->SetLabelOffset(0.006);
    hres->GetXaxis()->SetNdivisions(505);
    hres->GetXaxis()->SetTickLength(hres->GetXaxis()->GetTickLength() * 3.0 );
    hres->GetXaxis()->SetNoExponent(true);
    hres->GetYaxis()->SetNoExponent(true);
    
    hres->SetYTitle("#frac{Data-Pred.}{Pred.}");

    if (binlabels.size()){
      for (int i=0;i<binlabels.size();i++){
	hres->GetXaxis()->SetLabelSize(0.14);
	hres->GetXaxis()->SetLabelOffset(0.03);
	hres->GetXaxis()->SetBinLabel(i+1,binlabels[i].c_str());
      }
    }


/*  
    if(hdata&&poisson){
      if (mchistos.size()==0) hdata->SetMaximum(maximum);
      TGraphAsymmErrors * gdata = new TGraphAsymmErrors(hdata);
      for (int i = 0; i < gdata->GetN(); ++i) {
	double y=gdata->GetY()[i];
	double ym, yp;
	double nSigma = 1.;
	RooHistError::instance().getPoissonInterval(y,ym,yp,nSigma);
	double yerrplus = yp - y;
	double yerrminus = y - ym;
	gdata->SetPointEYlow(i,yerrminus);
	gdata->SetPointEYhigh(i,yerrplus);
      }
      gdata->Draw("PZ");
    }
*/


    TGraphAsymmErrors * resdata = new TGraphAsymmErrors(hres);    
    


    for (int ibin=1;ibin<=hres->GetNbinsX();ibin++) {
      Float_t ndata  = hdata->GetBinContent(ibin);
      Float_t nmc    = ((TH1*)hmc->GetStack()->Last())->GetBinContent(ibin);
      Float_t nmc_up = ((TH1*)hmc_up->GetStack()->Last())->GetBinContent(ibin);
      Float_t nmc_down = ((TH1*)hmc_down->GetStack()->Last())->GetBinContent(ibin);
      Float_t edata  = hdata->GetBinError(ibin);
      if (nmc>0) {
	Float_t bin  = (ndata-nmc)/nmc;
	Float_t err  = edata/nmc;
	
	if(ndata!=0&&!poisson) {
	  hres->SetBinContent(ibin,bin);
	  hres->SetBinError(ibin,err);
	  //cout << " i : " << ibin << " bin :  " << bin << " err+ :  " << err << endl;
	}
	else if(ndata>=0&&poisson){
	  double y=ndata;
	  double ym, yp; 
	  double nSigma = 1.;                                                                                                                                                                                                                                  
	  RooHistError::instance().getPoissonInterval(y,ym,yp,nSigma);                                                                                                                                                                                     
	  double yerrplus = yp - y;                                                                                                                                                                                                                     
	  double yerrminus = y - ym;                                                                                                                                                                                                                            
	  resdata->SetPoint(ibin-1,hres->GetBinCenter(ibin),bin);
	  resdata->SetPointEYlow(ibin-1,yerrminus/nmc);                                                                                                                                                                                                             
	  resdata->SetPointEYhigh(ibin-1,yerrplus/nmc);                                 
	  //	  cout << " i : " << ibin << " bin :  " << bin << " err :  " << err << endl;                                                                                                                                                                                  
	  //	  cout << " i : " << ibin << " bin : " << bin << " err- :  " << yerrminus/nmc << " err+ :  " << yerrplus/nmc << endl;
	  resdata->SetPointEXlow(ibin-1,0);
	  resdata->SetPointEXhigh(ibin-1,0);
	  if (ndata==0) resdata->SetPoint(ibin-1,-99,-99);
	}    
	else {
	  hres->SetBinContent(ibin,-999);
	  hres->SetBinError(ibin,0);
	}
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
      else {
	hres->SetBinContent(ibin,-99.0);
	hres->SetBinError(ibin,-99.0);
	hsysup->SetBinContent(ibin,0.0);
        hsysup->SetBinError(ibin,0.0);
	hsysdown->SetBinContent(ibin,0.0);
        hsysdown->SetBinError(ibin,0.0);
	herrges->SetBinContent(ibin,0.0);
	herrges->SetBinError(ibin,0.0);
	herrges2->SetBinContent(ibin,0.0);
	herrges2->SetBinError(ibin,0.0);
	resdata->SetPoint(ibin-1,-99,-99);
      }
    }
    Double_t max=std::max(std::abs(hres->GetMinimum()),
			  std::abs(hres->GetMaximum()));
    max *=1.05; max = std::min(49.9,max);
    if(resmax>0.) max = resmax;
    hres->SetMinimum(-max);
    hres->SetMaximum(1.2*max);
    hres->SetMarkerStyle(kFullCircle);
    hres->SetMarkerSize(1.25);
    //hres->SetMarkerSize(1);
    
    

    gPad->SetGridy();
    
    
    // DRAW MC UNCERTAINTY IN RESIDUAL

    TH1F* herr2 = (TH1F*) mchistos[0]->Clone("herr");

    Float_t tmp_stackcontents[nbinsx];
    Float_t tmp_stackerrors[nbinsx];

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

	  if(i==mchistos.size()-1){
	    tmp_stackcontents[ibin-1]=herr2->GetBinContent(ibin);
	    tmp_stackerrors[ibin-1]=herr2->GetBinError(ibin);
	  }

	  
	  if((i==mchistos.size()-1)&&herr2->GetBinContent(ibin)){
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
      herrges->SetFillColor(kPink-7);
      herrges->SetFillStyle(3001);
      herrges->SetMarkerSize(0);
      herrges2->SetLineWidth(0);
      herrges2->SetFillColor(kPink-7);
      herrges2->SetFillStyle(3001);
      herrges2->SetMarkerSize(0);
      
      hsysstat->SetLineWidth(0);
      hsysstat->SetFillColor(kPink-7);
      hsysstat->SetFillStyle(3001);
      hsysstat->SetMarkerSize(0);
      

    
      herr2->SetFillColor(kMagenta-9);
      herr2->SetFillStyle(3001);
      herr2->SetMarkerStyle(1);
      //hsysup->SetFillColor(kPink-7);
      //hsysup->SetFillStyle(3001);    
      //hsysup->SetLineWidth(0);
      //hsysdown->SetFillColor(kPink-7);
      //hsysdown->SetFillStyle(3001);
      //hsysdown->SetLineWidth(0);
      resdata->SetMarkerStyle(20);
      resdata->SetMarkerSize(0.5);
      gStyle->SetEndErrorSize(6);

      if (!poisson) hres->Draw("EPHIST");
      else {
	//cout << "YEAH ADWAUDGHAKLJDGSGAU{DPAIGUD"<< endl;
	hres->SetMarkerSize(1.3);
	hres->Draw("AXIS");
	hres->Draw("AXIG same");
	resdata->SetMarkerStyle(kFullCircle);
	resdata->Draw("PZ same");
      }

      
      // Plot with external uncertainty file if present
      
      if (!up_uncfile.empty()){
	
	for (int ibin=1;ibin<=herr2->GetNbinsX();ibin++) {
	    
	  Float_t tmp_bincontent = tmp_stackcontents[ibin-1];
	  Float_t tmp_staterror  = tmp_stackerrors[ibin-1];
	  Float_t tmp_error  = abs(uperror[ibin]-(uperror[ibin]+downerror[ibin])/2);
	  herr2->SetBinContent(ibin,((uperror[ibin]+downerror[ibin])/2)/tmp_bincontent-1);
	  herr2->SetBinError(ibin,sqrt(tmp_error*tmp_error+tmp_staterror*tmp_staterror)/tmp_bincontent);
	  //herr->SetBinError(ibin,sqrt(tmp_systerror*tmp_systerror+tmp_staterror*tmp_staterror));     
	    
	}
      }

      herr2->Draw("error sameE2");
      //resdata->Draw("PZ same");
      resdata->Draw("P0");
      resdata->Draw("||");

      if(plotsys){
	hsysstat->Draw("error sameE2");
	//herrges->Draw("error sameE2");
	//herrges2->Draw("error sameE2");

      }

      //if (!poisson) hres->Draw("EPHIST same");
      // else {
      //      hres->Draw("EPHIST same");
      //resdata->Draw("EPHIST same PZ");
      // }
    } // {mcuncert}
    
    else{
      if (!poisson) hres->Draw("EPHIST");
      else {
	//	hres->Draw("AXIS");
	hres->SetMarkerSize(1.3);
        hres->Draw("GRID");
	resdata->SetMarkerStyle(kFullCircle);
	resdata->Draw("PZ same");
      }
    }
    //
    // LEGEND 2
    //
    if (mcuncert) {
      
      if(plotsys){
	
	TLegend* l2 = new TLegend(legxmin,0.73,legxmax,0.90);
	l2->SetFillColor(10); l2->SetLineColor(10); l2->SetBorderSize(0);
	l2->AddEntry(herr2,"MC stat.","F");
	l2->AddEntry(herrges,"MC syst+stat.","F");
	l2->Draw();
      }
      else{
	TLegend* l2 = new TLegend(legxmin,0.73,legxmax,0.90);
	l2->SetFillColor(10); l2->SetLineColor(10); l2->SetBorderSize(0); l2->SetTextSize(0.12);
	l2->AddEntry(herr2,"MC stat.","F");
	l2->Draw();
      }
    }

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
  c->cd(1);

  TPad* b = new TPad("b", "b",0.0501342,0.0,0.121047,0.0714685);
  b->SetBorderMode(0);
  //b->SetFillColor(kBlue);
  if (!logy) b->Draw();
  
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
  return kWhite;
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
