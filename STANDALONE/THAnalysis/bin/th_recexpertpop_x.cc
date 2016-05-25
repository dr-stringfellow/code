///////////////////////////////////////////////////////////////////////////////////
//
//th_recexpert_x
// ------------
//
// tool to reconstuct a single top higgs event with popov's weights
//
// 
//
//    
//    ::05/14/2012 Benedikt Maier <bmaier@ekp.uni-karlsruhe.de>:: Simon :: Christian
/////////////////////////////////////////////////////////////////////////////////////

//#include "SchieferD/AnaUtils/interface/MCSample.h"

#include "../interface/CommandLine.h"
#include "../interface/THStatus.h"


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <map>
#include <TLorentzVector.h>
#include <math.h>

#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TKey.h"
#include "TControlBar.h"
#include "TSystem.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TObjString.h"
#include "TChain.h"
//#include "/portal/ekpcms5/home/held/code/root_v5.28.00c/tmva/test/TMVAGui.C"
//#include "/portal/ekpcms5/home/held/code/root_v5.28.00c/tmva/test/tmvaglob.C"
//#include "TStopwatch.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
//////////////////////////////////////////////////////////////////////////////////______________________________________________________________________________

struct GreaterByVal{
  vector<float>  pts;
  GreaterByVal(vector<float>  pts_) : pts(pts_){
    //void
  }
  bool operator()(size_t i, size_t j) const{
    return pts.at(i) > pts.at(j);
  }
};

struct SmallerByVal{
  vector<float>  pts;
  SmallerByVal(vector<float>  pts_) : pts(pts_) {
    //void
  }
  bool operator()(size_t i, size_t j) const{
    return pts.at(i) < pts.at(j);
  }
};

double deltaPhi(double phi1, double phi2);
double deltaR(double eta1, double phi1, double eta2, double phi2);
void master_reader_th(TMVA::Reader *reader_th, string weight, TString weightfile);

// Create a set of variables and declare them to the reader
  // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
Float_t var1=0., var2=0.;
Float_t var3=0., var4=0.;
Float_t var5=0., var6=0.;
Float_t var7=0., var8=0.;
Float_t var9=0., var10=0.;
Float_t var11=0., var12=0.;
Float_t var13=0., var14=0.;
Float_t var15=0., var16=0.;
Float_t var17=0., var18=0.;

Int_t spec1=0;
Int_t spec2=0;
Int_t spec3=0;
Int_t spec4=0;
Float_t spec5=0.0;
Float_t spec6=0.0;
Float_t spec7=0.0;
Float_t spec8=0.0;

// Book method(s)   
TString methodName = TString("BDT") + TString(" method");


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main(int argc,char**argv)
{
  
  // This loads the library
  TMVA::Tools::Instance();
  
  
  // Default MVA methods to be trained + tested
  std::map<std::string,int> Use;
  
  // --- Boosted Decision Trees
  Use["BDT"]             = 1; // uses Adaptive Boost
  Use["BDTG"]            = 0; // uses Gradient Boost
  Use["BDTB"]            = 0; // uses Bagging
  Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
  std::cout << std::endl;
  std::cout << "==> Start TMVAClassificationApplication" << std::endl;
  
  CommandLine cl;
  if (!cl.parse(argc,argv))
    return 0;
  
  string         input     = cl.getValue <string>  ("input");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         ipath     = cl.getValue <string>  ("ipath",              ".");
  string         opath     = cl.getValue <string>  ("opath",              ".");
  bool           signal  = cl.getValue <bool>      ("signal",         false);
  vector<string> bdtpath   = cl.getVector<string>  ("bdtpath"              ""); // was empty before (why?)
  bool           takerandom = cl.getValue<bool>    ("takerandom",       false);
  float          workingpoint=cl.getValue<float>   ("workingpoint",     0.935);


  if(!cl.check()){};
  cl.print();
    
  
  // How many different BDTs will be evaluated?

  Int_t nbdts = bdtpath.size();

  string cvcfmap[51]={"weights_syst[0]/weights_syst[0]",
		      "weights_syst[446]/weights_syst[0]",
		      "weights_syst[447]/weights_syst[0]",
		      "weights_syst[448]/weights_syst[0]",
		      "weights_syst[449]/weights_syst[0]",
		      "weights_syst[450]/weights_syst[0]",
		      "weights_syst[451]/weights_syst[0]",
		      "weights_syst[452]/weights_syst[0]",
		      "weights_syst[453]/weights_syst[0]",
		      "weights_syst[454]/weights_syst[0]",
		      "weights_syst[455]/weights_syst[0]",
		      "weights_syst[456]/weights_syst[0]",
		      "weights_syst[457]/weights_syst[0]",
		      "weights_syst[458]/weights_syst[0]",
		      "weights_syst[459]/weights_syst[0]",
		      "weights_syst[460]/weights_syst[0]",
		      "weights_syst[461]/weights_syst[0]",
		      "weights_syst[462]/weights_syst[0]",
		      "weights_syst[463]/weights_syst[0]",
		      "weights_syst[464]/weights_syst[0]",
		      "weights_syst[465]/weights_syst[0]",
		      "weights_syst[466]/weights_syst[0]",
		      "weights_syst[467]/weights_syst[0]",
		      "weights_syst[468]/weights_syst[0]",
		      "weights_syst[469]/weights_syst[0]",
		      "weights_syst[470]/weights_syst[0]",
		      "weights_syst[471]/weights_syst[0]",
		      "weights_syst[472]/weights_syst[0]",
		      "weights_syst[473]/weights_syst[0]",
		      "weights_syst[474]/weights_syst[0]",
		      "weights_syst[475]/weights_syst[0]",
		      "weights_syst[476]/weights_syst[0]",
		      "weights_syst[477]/weights_syst[0]",
		      "weights_syst[478]/weights_syst[0]",
		      "weights_syst[479]/weights_syst[0]",
		      "weights_syst[480]/weights_syst[0]",
		      "weights_syst[481]/weights_syst[0]",
		      "weights_syst[482]/weights_syst[0]",
		      "weights_syst[483]/weights_syst[0]",
		      "weights_syst[484]/weights_syst[0]",
		      "weights_syst[485]/weights_syst[0]",
		      "weights_syst[486]/weights_syst[0]",
		      "weights_syst[487]/weights_syst[0]",
		      "weights_syst[488]/weights_syst[0]",
		      "weights_syst[489]/weights_syst[0]",
		      "weights_syst[490]/weights_syst[0]",
		      "weights_syst[491]/weights_syst[0]",
		      "weights_syst[492]/weights_syst[0]",
		      "weights_syst[493]/weights_syst[0]",
		      "weights_syst[494]/weights_syst[0]",
		      "weights_syst[495]/weights_syst[0]"};


  

  TMVA::Reader *reader_th_0 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_1 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_2 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_3 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_4 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_5 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_6 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_7 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_8 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_9 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_10 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_11 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_12 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_13 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_14 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_15 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_16 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_17 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_18 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_19 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_20 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_21 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_22 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_23 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_24 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_25 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_26 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_27 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_28 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_29 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_30 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_31 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_32 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_33 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_34 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_35 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_36 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_37 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_38 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_39 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_40 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_41 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_42 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_43 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_44 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_45 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_46 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_47 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_48 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_49 = new TMVA::Reader( "!Color:!Silent" );
  TMVA::Reader *reader_th_50 = new TMVA::Reader( "!Color:!Silent" );


  TString weightfiles_th[51];
  
  for (Int_t z=0;z<nbdts;z++)
    weightfiles_th[z]=TString(bdtpath[z]) +"/"+ TString("BDTreco_expert_BDT.weights.xml");


    
  master_reader_th(reader_th_0,cvcfmap[0],weightfiles_th[0]);
  master_reader_th(reader_th_1,cvcfmap[1],weightfiles_th[1]);
  master_reader_th(reader_th_2,cvcfmap[2],weightfiles_th[2]);
  master_reader_th(reader_th_3,cvcfmap[3],weightfiles_th[3]);
  master_reader_th(reader_th_4,cvcfmap[4],weightfiles_th[4]);
  master_reader_th(reader_th_5,cvcfmap[5],weightfiles_th[5]);
  master_reader_th(reader_th_6,cvcfmap[6],weightfiles_th[6]);
  master_reader_th(reader_th_7,cvcfmap[7],weightfiles_th[7]);
  master_reader_th(reader_th_8,cvcfmap[8],weightfiles_th[8]);
  master_reader_th(reader_th_9,cvcfmap[9],weightfiles_th[9]);
  master_reader_th(reader_th_10,cvcfmap[10],weightfiles_th[10]);
  master_reader_th(reader_th_11,cvcfmap[11],weightfiles_th[11]);
  master_reader_th(reader_th_12,cvcfmap[12],weightfiles_th[12]);
  master_reader_th(reader_th_13,cvcfmap[13],weightfiles_th[13]);
  master_reader_th(reader_th_14,cvcfmap[14],weightfiles_th[14]);
  master_reader_th(reader_th_15,cvcfmap[15],weightfiles_th[15]);
  master_reader_th(reader_th_16,cvcfmap[16],weightfiles_th[16]);
  master_reader_th(reader_th_17,cvcfmap[17],weightfiles_th[17]);
  master_reader_th(reader_th_18,cvcfmap[18],weightfiles_th[18]);
  master_reader_th(reader_th_19,cvcfmap[19],weightfiles_th[19]);
  master_reader_th(reader_th_20,cvcfmap[20],weightfiles_th[20]);
  master_reader_th(reader_th_21,cvcfmap[21],weightfiles_th[21]);
  master_reader_th(reader_th_22,cvcfmap[22],weightfiles_th[22]);
  master_reader_th(reader_th_23,cvcfmap[23],weightfiles_th[23]);
  master_reader_th(reader_th_24,cvcfmap[24],weightfiles_th[24]);
  master_reader_th(reader_th_25,cvcfmap[25],weightfiles_th[25]);
  master_reader_th(reader_th_26,cvcfmap[26],weightfiles_th[26]);
  master_reader_th(reader_th_27,cvcfmap[27],weightfiles_th[27]);
  master_reader_th(reader_th_28,cvcfmap[28],weightfiles_th[28]);
  master_reader_th(reader_th_29,cvcfmap[29],weightfiles_th[29]);
  master_reader_th(reader_th_30,cvcfmap[30],weightfiles_th[30]);
  master_reader_th(reader_th_31,cvcfmap[31],weightfiles_th[31]);
  master_reader_th(reader_th_32,cvcfmap[32],weightfiles_th[32]);
  master_reader_th(reader_th_33,cvcfmap[33],weightfiles_th[33]);
  master_reader_th(reader_th_34,cvcfmap[34],weightfiles_th[34]);
  master_reader_th(reader_th_35,cvcfmap[35],weightfiles_th[35]);
  master_reader_th(reader_th_36,cvcfmap[36],weightfiles_th[36]);
  master_reader_th(reader_th_37,cvcfmap[37],weightfiles_th[37]);
  master_reader_th(reader_th_38,cvcfmap[38],weightfiles_th[38]);
  master_reader_th(reader_th_39,cvcfmap[39],weightfiles_th[39]);
  master_reader_th(reader_th_40,cvcfmap[40],weightfiles_th[40]);
  master_reader_th(reader_th_41,cvcfmap[41],weightfiles_th[41]);  
  master_reader_th(reader_th_42,cvcfmap[42],weightfiles_th[42]);
  master_reader_th(reader_th_43,cvcfmap[43],weightfiles_th[43]);
  master_reader_th(reader_th_44,cvcfmap[44],weightfiles_th[44]);
  master_reader_th(reader_th_45,cvcfmap[45],weightfiles_th[45]);
  master_reader_th(reader_th_46,cvcfmap[46],weightfiles_th[46]);
  master_reader_th(reader_th_47,cvcfmap[47],weightfiles_th[47]);
  master_reader_th(reader_th_48,cvcfmap[48],weightfiles_th[48]);
  master_reader_th(reader_th_49,cvcfmap[49],weightfiles_th[49]);
  master_reader_th(reader_th_50,cvcfmap[50],weightfiles_th[50]);
  
  

  
  // --- Create the Reader object
  		      
  string prefix = "TMVAClassification";
  

  
  // read input file
  TFile* ifile = new TFile((ipath+"/"+input).c_str(),"READ");
  if (!ifile->IsOpen()) {
    cout<<"Failed to open "<<input<<endl;
    return 0;
  }
  
  // create temporary output file
  size_t pos = input.find(".root");
  string output = input.substr(0,pos);
  output = output + ".bdt.root";
  TFile* ofile = new TFile((opath+"/"+output).c_str(),"RECREATE");
  if (!ofile->IsOpen()) {
    cout<<"Failed to create "<<output<<endl;
    return 0;
  }
  
  // read input directory
  TDirectory* idir = (TDirectory*)ifile->Get(dirname.c_str());
  cout << "Input file: " << input << "/" << dirname << ": (@" << idir << ")" << endl;
  
  // create output directory
  TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
  if(0==odir){
    cout << "Failed to make " << dirname << " in " << output << endl;
    return 0;
  }
  cout << "Output file: " << output << "/" << dirname << ": (@" << odir << ")" << endl;

  // open input tree
  TTree* itree = (TTree*)idir->Get(treename.c_str());
  if(0==itree){
    cout << "No " << treename << " in " << output << ":" << dirname << endl;
    return 0;
  }
  
  // don't copy popout branch if it already exists
  if (0!=itree->FindBranch("best_recbdtout"))           itree->SetBranchStatus("best_recbdtout",0);
  if (0!=itree->FindBranch("hyp_posbdt"))               itree->SetBranchStatus("hyp_posbdt",0);
  if (0!=itree->FindBranch("hyp_posdR"))                itree->SetBranchStatus("hyp_posdR",0);
  if (0!=itree->FindBranch("hbbm"))                     itree->SetBranchStatus("hbbm",0); 
  if (0!=itree->FindBranch("hbbpt"))                    itree->SetBranchStatus("hbbpt",0);
  if (0!=itree->FindBranch("hbbphi"))                   itree->SetBranchStatus("hbbphi",0);
  if (0!=itree->FindBranch("hbbeta"))                   itree->SetBranchStatus("hbbeta",0);
  if (0!=itree->FindBranch("hbbdr"))                    itree->SetBranchStatus("hbbdr",0); 
  if (0!=itree->FindBranch("hbbjtidx"))                 itree->SetBranchStatus("hbbjtidx",0);
  if (0!=itree->FindBranch("topm"))                     itree->SetBranchStatus("topm",0);   
  if (0!=itree->FindBranch("toppt"))                    itree->SetBranchStatus("toppt",0);  
  if (0!=itree->FindBranch("topphi"))                   itree->SetBranchStatus("topphi",0); 
  if (0!=itree->FindBranch("topeta"))                   itree->SetBranchStatus("topeta",0); 
  if (0!=itree->FindBranch("topjtidx"))                 itree->SetBranchStatus("topjtidx",0);
  if (0!=itree->FindBranch("ljtidx"))                   itree->SetBranchStatus("ljtidx",0);
  

  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree(0);
  cout << "Output file: " << output << "/" << dirname << "/" << treename << ": (@" << otree << ")" << endl;

  if (0!=itree->FindBranch("hbbm"))                   itree->SetBranchStatus("hbbm",1);
  if (0!=itree->FindBranch("hbbpt"))                  itree->SetBranchStatus("hbbpt",1);
  if (0!=itree->FindBranch("hbbphi"))                 itree->SetBranchStatus("hbbphi",1);
  if (0!=itree->FindBranch("hbbeta"))                 itree->SetBranchStatus("hbbeta",1);
  if (0!=itree->FindBranch("hbbdr"))                  itree->SetBranchStatus("hbbdr",1);
  if (0!=itree->FindBranch("hbbjtidx"))               itree->SetBranchStatus("hbbjtidx",1);
  if (0!=itree->FindBranch("topm"))                   itree->SetBranchStatus("topm",1);
  if (0!=itree->FindBranch("toppt"))                  itree->SetBranchStatus("toppt",1);
  if (0!=itree->FindBranch("topphi"))                 itree->SetBranchStatus("topphi",1);
  if (0!=itree->FindBranch("topeta"))                 itree->SetBranchStatus("topeta",1);
  if (0!=itree->FindBranch("topjtidx"))               itree->SetBranchStatus("topjtidx",1);
  if (0!=itree->FindBranch("ljtidx"))                 itree->SetBranchStatus("ljtidx",1);

  // create bdtout branches in output tree
  Float_t  best_recbdtout[51]={-99};      otree->Branch("best_recbdtout", best_recbdtout, "best_recbdtout[51]/F");
  Int_t  hyp_posdR=-99;             otree->Branch("hyp_posdR",     &hyp_posdR,           "hyp_posdR/I");
  Int_t  hyp_posbdt=-99;            otree->Branch("hyp_posbdt",    &hyp_posbdt,         "hyp_posbdt/I");

  Float_t hbbm[51]={-99};                 otree->Branch("hbbm",          hbbm,                     "hbbm[51]/F");
  Float_t hbbpt[51]={-99};                otree->Branch("hbbpt",         hbbpt,                   "hbbpt[51]/F");
  Float_t hbbphi[51]={-99};               otree->Branch("hbbphi",        hbbphi,                 "hbbphi[51]/F");
  Float_t hbbeta[51]={-99};               otree->Branch("hbbeta",        hbbeta,                 "hbbeta[51]/F");
  Float_t hbbdr[51]={-99};                otree->Branch("hbbdr",         hbbdr,                   "hbbdr[51]/F");
  Int_t hbbjtidx[3][51]={-99,-99,-99};  otree->Branch("hbbjtidx",      hbbjtidx,           "hbbjtidx[3][51]/I");
  Float_t topm[51]={-99};                 otree->Branch("topm",          topm,                     "topm[51]/F");
  Float_t toppt[51]={-99};                otree->Branch("toppt",         toppt,                   "toppt[51]/F");
  Float_t topphi[51]={-99};               otree->Branch("topphi",        topphi,                 "topphi[51]/F");
  Float_t topeta[51]={-99};               otree->Branch("topeta",        topeta,                 "topeta[51]/F");
  Int_t topjtidx[51]={-99};               otree->Branch("topjtidx",      topjtidx,             "topjtidx[51]/I");
  Int_t ljtidx[51]={-99};                 otree->Branch("ljtidx",        ljtidx,                 "ljtidx[51]/I");
  
  //Get relevant information for creating hypothesis
  Int_t i_njt=-99;                 itree->SetBranchAddress("njt",            &i_njt);
  Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
  Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta",         i_jteta);
  Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
  Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);
  Float_t i_jtcsvt[40]={-99};      itree->SetBranchAddress("jtcsvt",       i_jtcsvt);
  Float_t i_jtcharge[40]={-99};    itree->SetBranchAddress("jtcharge",   i_jtcharge);
  Float_t i_jtcostheta_l[40]={-99};itree->SetBranchAddress("jtcostheta_l",   i_jtcostheta_l);
  
  Int_t i_nlepw={-99};           itree->SetBranchAddress("nlepw",        &i_nlepw);
  Float_t i_lepwm={-99};           itree->SetBranchAddress("lepwm",        &i_lepwm);
  Float_t i_lepwpt={-99};          itree->SetBranchAddress("lepwpt",      &i_lepwpt);
  Float_t i_lepwphi={-99};         itree->SetBranchAddress("lepwphi",    &i_lepwphi);
  Float_t i_lepweta={-99};         itree->SetBranchAddress("lepweta",    &i_lepweta);
  
  Int_t i_nel={-99};               itree->SetBranchAddress("nel",            &i_nel);
  Float_t i_elpt[10]={-99};        itree->SetBranchAddress("elpt",           i_elpt);
  Float_t i_eleta[10]={-99};       itree->SetBranchAddress("eleta",         i_eleta);
  Float_t i_elphi[10]={-99};       itree->SetBranchAddress("elphi",         i_elphi);
  Float_t i_ele[10]={-99};         itree->SetBranchAddress("ele",             i_ele);
  Float_t i_elcharge[10]={-99};    itree->SetBranchAddress("elcharge",   i_elcharge);  
  
  Int_t i_nmu={-99};               itree->SetBranchAddress("nmu",            &i_nmu);
  Float_t i_mupt[10]={-99};        itree->SetBranchAddress("mupt",           i_mupt);
  Float_t i_mueta[10]={-99};       itree->SetBranchAddress("mueta",         i_mueta);
  Float_t i_muphi[10]={-99};       itree->SetBranchAddress("muphi",         i_muphi);
  Float_t i_mue[10]={-99};         itree->SetBranchAddress("mue",             i_mue);
  Float_t i_mucharge[10]={-99};    itree->SetBranchAddress("mucharge",   i_mucharge);
  
  Float_t i_sumHt={-99};           itree->SetBranchAddress("sumHt",        &i_sumHt);
  Float_t i_met={-99};             itree->SetBranchAddress("met",            &i_met);

  Float_t i_Hpt={-99};
  Float_t i_Heta={-99};          
  Float_t i_Hphi={-99};
  Float_t i_Hm={-99}; 

  Float_t i_tpt={-99}; 
  Float_t i_teta={-99};       
  Float_t i_tphi={-99};
  Float_t i_tm={-99};        

  Float_t i_lqpt={-99};
  Float_t i_lqeta={-99};
  Float_t i_lqphi={-99};
  Float_t i_lqm={-99};

  if(signal){
    itree->SetBranchAddress("Hpt",            &i_Hpt);
    itree->SetBranchAddress("Heta",          &i_Heta);
    itree->SetBranchAddress("Hphi",          &i_Hphi);
    itree->SetBranchAddress("Hm",              &i_Hm);
    itree->SetBranchAddress("tpt",            &i_tpt);
    itree->SetBranchAddress("teta",          &i_teta);
    itree->SetBranchAddress("tphi",          &i_tphi);
    itree->SetBranchAddress("tm",              &i_tm);
    itree->SetBranchAddress("lqpt",          &i_lqpt);
    itree->SetBranchAddress("lqeta",        &i_lqeta);
    itree->SetBranchAddress("lqphi",        &i_lqphi);
    itree->SetBranchAddress("lqm",            &i_lqm);
  }

  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  THStatus * status= new THStatus(nevt);
  cout << nevt << " events will be processed" << endl;




  Float_t rndm_recbdtout[51];
  Int_t best_i[51];
  Int_t best_j[51];
  Int_t best_k[51];
  Int_t best_l[51];
  TLorentzVector p4jets[100];
  Float_t jetscsv[100];
  vector<float> vec_bdtout;
  vector<float> vec_deltaR;
  Float_t recbdtout[51];


  //event loop
  for (unsigned int ievt=0; ievt<nevt; ievt++) {

    if (ievt %20 == 0 || ievt <= 10 || nevt - ievt < 10){
      cout << "\r  > Processing events from input tree:  (\033[33m" << ievt+1 << "/" << nevt << "\033[0m)  -> \033[32m";
      cout << setprecision(3) << setiosflags(std::ios_base::left) << setfill('0') << setw(2)\
	   << ((ievt*1.0+1))/nevt*100.0 << "\033[0m%    \b\b\b" << flush;
    }
    //status->print_status(ievt);
    
    itree->GetEntry(ievt);

    Int_t rndm_i=-1;
    Int_t rndm_j=-1;
    Int_t rndm_k=-1;
    Int_t rndm_l=-1;
 
    // unnecessary because they get overwritten either way, and nothing checks for the default value of -1
    //std::fill_n(best_i, 51, -1);
    //std::fill_n(best_j, 51, -1);
    //std::fill_n(best_k, 51, -1);
    //std::fill_n(best_l, 51, -1);
    
    for(int z=0;z<51;z++)
      {
	ljtidx[z]=-1;
	topjtidx[z]=-1;
	hbbjtidx[0][z]=-1;
	hbbjtidx[1][z]=-1;
	best_recbdtout[z] = -99; 
	rndm_recbdtout[z] = -99; 
      }
	  

    vec_bdtout.clear();
    vec_deltaR.clear();

    // unused:
    //vector<int>  vec_i;
    //vector<int>  vec_j;
    //vector<int>  vec_k;
    //vector<int>  vec_l;

    // unnecessary because they get overwritten either way, and nothing checks for the default value of -1
    //std::fill_n(best_i, 51, -1);
    //std::fill_n(best_j, 51, -1);
    //std::fill_n(best_k, 51, -1);
    //std::fill_n(best_l, 51, -1);
    
    for(int z=0;z<51;z++)
      {
	ljtidx[z]=-1;
	topjtidx[z]=-1;
	hbbjtidx[0][z]=-1;
	hbbjtidx[1][z]=-1;
	best_recbdtout[z] = -99; 
	rndm_recbdtout[z] = -99; 
      }
	  

    vec_bdtout.clear();
    vec_deltaR.clear();

    // unused:
    //vector<int>  vec_i;
    //vector<int>  vec_j;
    //vector<int>  vec_k;
    //vector<int>  vec_l;

    Bool_t isForbidden = false;
    Int_t hyp_counter=0;
    Int_t n_central=0;
    for (int jets=0; jets<i_njt; jets++){
      if (abs(i_jteta[jets])<2.4)
        n_central++;
    }
    if (i_nlepw == 1 && i_njt>=4 && n_central>2) {
      TLorentzVector p4top, p4higgs, p4light;
      TLorentzVector p4lepw;
      for (int jets=0; jets<i_njt; jets++){
	p4jets[jets].SetPtEtaPhiE(i_jtpt[jets],i_jteta[jets],i_jtphi[jets],i_jte[jets]);
	jetscsv[jets]=i_jtcsvt[jets];
      }

      p4lepw.SetPtEtaPhiM(i_lepwpt,i_lepweta,i_lepwphi,i_lepwm);
     
      bool has_wenu = (i_nel==1);
      TLorentzVector p4lepton;
      float lepcharge = -99; 

      if (has_wenu){
	      p4lepton.SetPtEtaPhiE(i_elpt[0],i_eleta[0],i_elphi[0],i_ele[0]);
	      lepcharge = i_elcharge[0];
      }else{
	      p4lepton.SetPtEtaPhiE(i_mupt[0],i_mueta[0],i_muphi[0],i_mue[0]);
	      lepcharge = i_mucharge[0];
      }

      if(signal){
	      p4higgs.SetPtEtaPhiM(i_Hpt,i_Heta,i_Hphi,i_Hm);
	      p4top.SetPtEtaPhiM(i_tpt,i_teta,i_tphi,i_tm);
	      p4light.SetPtEtaPhiM(i_lqpt, i_lqeta,i_lqphi,i_lqm);
      } 

      


      int facul = i_njt ;
      int resultfac = 1;
      for(int f=0; f<4; f++){
        resultfac *= facul;
	facul -= 1;
      }
      resultfac /= 2;

      for (Int_t i=0; i<i_njt; i++) {  // b from top
	if(fabs(p4jets[i].Eta())>2.4 || p4jets[i].Pt()<30)
	  continue;

	for (Int_t j=0; j<i_njt; j++) {  // b1 from Higgs
          if (j!=i) {
            if(fabs(p4jets[j].Eta())>2.4 || p4jets[j].Pt()<30)
              continue;

            for (Int_t k=0; k<i_njt; k++) {  // b2 from Higgs
              if ((k!=i )&&(k>j)) {  // switching of b1 & b2 does not change Higgs, so take only one
                if(fabs(p4jets[k].Eta())>2.4 || p4jets[k].Pt()<30)
                  continue;

                TLorentzVector rechiggs = p4jets[j] + p4jets[k];
                for (Int_t l=0; l<i_njt; l++) {  // light jet
                  if(l!=k && l!=j && l!=i) {
                    if((i_jtcsvt[l] > workingpoint && fabs(p4jets[l].Eta())<2.4) || (fabs(p4jets[l].Eta())>2.4 && p4jets[l].Pt()<40)){
                      continue;}
                    hyp_counter++;
                  }
                }//for ...  light jet
              }
            }//for ... b2 from Higgs
          }
        }//for ... b1 from Higgs
      }//for ... b from top
      if(hyp_counter==0)continue;
      TRandom3 random = 0;
      int randomhyp = random.Integer(hyp_counter-1);
      hyp_counter =0 ;
      for (Int_t i=0; i<i_njt; i++) {  // b from top
        if(fabs(p4jets[i].Eta())>2.4 || p4jets[i].Pt()<30)
          continue;

        TLorentzVector rectop = p4lepw + p4jets[i];

        for (Int_t j=0; j<i_njt; j++){  // b1 from Higgs
          if (j!=i){
            if(fabs(p4jets[j].Eta())>2.4 || p4jets[j].Pt()<30)
              continue;


            for (Int_t k=0; k<i_njt; k++) {  // b2 from Higgs
	      
              if ((k!=i )&&(k>j)) {  // switching of b1 & b2 does not change Higgs, so take only one

		if(fabs(p4jets[k].Eta())>2.4 || p4jets[k].Pt()<30)
                  continue;


                TLorentzVector rechiggs = p4jets[j] + p4jets[k];

                for (Int_t l=0; l<i_njt; l++) {  // light jet

                  if(l!=k && l!=j && l!=i) {
                    if( (i_jtcsvt[l] > workingpoint && fabs(p4jets[l].Eta())<2.4) ||  (fabs(p4jets[l].Eta())>2.4 && p4jets[l].Pt()<40))
                      continue;

                    if(hyp_counter == randomhyp){
                      rndm_i  =  i;
                      rndm_j  =  j;
                      rndm_k  =  k;
                      rndm_l  =  l;
                    }

                    TLorentzVector reclight=p4jets[l];

                    /*Lookup:
                     * i -> topjtidx
                     * j -> hbbjtidx[0]
                     * k -> hbbjtidx[1]
                     * l -> ljtidx
                     */
                    var1  =  i_jtcsvt[i];
                    var2  =  TMath::Abs(i_jteta[l]);
                    var3  =  i_jtcsvt[j];
		    var4  =  i_jtcsvt[k];
                    var5  =  (rectop.M() > 0) ? TMath::Log(rectop.M()) : -1 ;
                    var6  =  (rechiggs.M() > 0) ? TMath::Log(rechiggs.M()) : -1;
                    var7  =  TMath::Log(TMath::Min(i_jtpt[j],i_jtpt[k]));
                    var8  =  deltaR(i_jteta[j],i_jtphi[j], i_jteta[k], i_jtphi[k]);
                    var9  =  TMath::Max(TMath::Abs(i_jteta[j]),TMath::Abs(i_jteta[k]));
		    var10 =   deltaR(i_jteta[i], i_jtphi[i], p4lepw.Eta(), p4lepw.Phi());
                    var11 =  (rectop.Pt()+rechiggs.Pt()+i_jtpt[l])/(i_sumHt + i_met + (i_nel==1 ? i_elcharge[0] : i_mucharge[0]));
                    var12 =  deltaR(rectop.Eta(), rectop.Phi(), rechiggs.Eta(), rechiggs.Phi());
                    var13 =  i_jtcharge[i]*(i_nel==1 ? i_elcharge[0] : i_mucharge[0]);
                    var14 =  TMath::Abs(i_jteta[l]-i_jteta[i]);
                    var15 =  i_jte[l]-i_jte[i];
                    var16 =  TMath::Abs(i_jteta[i]);
                    var17 =  TMath::Abs(rectop.Eta()-rechiggs.Eta());
		    var18 =  i_jtcostheta_l[i];

		    if(isnan(var5))
		      cout << "Higgs jets: " << p4jets[j].Pt() << " " << p4jets[k].Pt() << endl;
		    if(isnan(var4))
		      cout << "top jets: " << p4jets[i].Pt() << " W: " << p4lepw.Pt() << endl;
	
		    recbdtout[0]=reader_th_0->EvaluateMVA( methodName  );
		    recbdtout[1]=reader_th_1->EvaluateMVA( methodName  );
		    recbdtout[2]=reader_th_2->EvaluateMVA( methodName  );
		    recbdtout[3]=reader_th_3->EvaluateMVA( methodName  );
		    recbdtout[4]=reader_th_4->EvaluateMVA( methodName  );
		    recbdtout[5]=reader_th_5->EvaluateMVA( methodName  );
		    recbdtout[6]=reader_th_6->EvaluateMVA( methodName  );
		    recbdtout[7]=reader_th_7->EvaluateMVA( methodName  );
		    recbdtout[8]=reader_th_8->EvaluateMVA( methodName  );
		    recbdtout[9]=reader_th_9->EvaluateMVA( methodName  );
		    recbdtout[10]=reader_th_10->EvaluateMVA( methodName  );
		    recbdtout[11]=reader_th_11->EvaluateMVA( methodName  );
		    recbdtout[12]=reader_th_12->EvaluateMVA( methodName  );
		    recbdtout[13]=reader_th_13->EvaluateMVA( methodName  );
		    recbdtout[14]=reader_th_14->EvaluateMVA( methodName  );
		    recbdtout[15]=reader_th_15->EvaluateMVA( methodName  );
		    recbdtout[16]=reader_th_16->EvaluateMVA( methodName  );
		    recbdtout[17]=reader_th_17->EvaluateMVA( methodName  );
		    recbdtout[18]=reader_th_18->EvaluateMVA( methodName  );
		    recbdtout[19]=reader_th_19->EvaluateMVA( methodName  );
		    recbdtout[20]=reader_th_20->EvaluateMVA( methodName  );
		    recbdtout[21]=reader_th_21->EvaluateMVA( methodName  );
		    recbdtout[22]=reader_th_22->EvaluateMVA( methodName  );
		    recbdtout[23]=reader_th_23->EvaluateMVA( methodName  );
		    recbdtout[24]=reader_th_24->EvaluateMVA( methodName  );
		    recbdtout[25]=reader_th_25->EvaluateMVA( methodName  );
		    recbdtout[26]=reader_th_26->EvaluateMVA( methodName  );
		    recbdtout[27]=reader_th_27->EvaluateMVA( methodName  );
		    recbdtout[28]=reader_th_28->EvaluateMVA( methodName  );
		    recbdtout[29]=reader_th_29->EvaluateMVA( methodName  );
		    recbdtout[30]=reader_th_30->EvaluateMVA( methodName  );
		    recbdtout[31]=reader_th_31->EvaluateMVA( methodName  );
		    recbdtout[32]=reader_th_32->EvaluateMVA( methodName  );
		    recbdtout[33]=reader_th_33->EvaluateMVA( methodName  );
		    recbdtout[34]=reader_th_34->EvaluateMVA( methodName  );
		    recbdtout[35]=reader_th_35->EvaluateMVA( methodName  );
		    recbdtout[36]=reader_th_36->EvaluateMVA( methodName  );
		    recbdtout[37]=reader_th_37->EvaluateMVA( methodName  );
		    recbdtout[38]=reader_th_38->EvaluateMVA( methodName  );
		    recbdtout[39]=reader_th_39->EvaluateMVA( methodName  );
		    recbdtout[40]=reader_th_40->EvaluateMVA( methodName  );
		    recbdtout[41]=reader_th_41->EvaluateMVA( methodName  );
		    recbdtout[42]=reader_th_42->EvaluateMVA( methodName  );
		    recbdtout[43]=reader_th_43->EvaluateMVA( methodName  );
		    recbdtout[44]=reader_th_44->EvaluateMVA( methodName  );
		    recbdtout[45]=reader_th_45->EvaluateMVA( methodName  );
		    recbdtout[46]=reader_th_46->EvaluateMVA( methodName  );
		    recbdtout[47]=reader_th_47->EvaluateMVA( methodName  );
		    recbdtout[48]=reader_th_48->EvaluateMVA( methodName  );
		    recbdtout[49]=reader_th_49->EvaluateMVA( methodName  );
		    recbdtout[50]=reader_th_50->EvaluateMVA( methodName  );


		    for (Int_t z=0;z<nbdts;z++){
		    			      
		      vec_bdtout.push_back(recbdtout[0]);
		      if(signal){
			Float_t DRtop = deltaR(rectop.Eta(),rectop.Phi(),p4top.Eta(),p4top.Phi()) + fabs(rectop.Pt() - p4top.Pt())/p4top.Pt();
			Float_t DRhiggs = deltaR(rechiggs.Eta(),rechiggs.Phi(),p4higgs.Eta(),p4higgs.Phi()) + fabs(rechiggs.Pt() - p4higgs.Pt())/p4higgs.Pt();
			Float_t DRlight = deltaR(reclight.Eta(),reclight.Phi(),p4light.Eta(),p4light.Phi()) + fabs(reclight.Pt() - p4light.Pt())/p4light.Pt();
			Float_t DeltaRval = DRtop + DRhiggs + DRlight;
			vec_deltaR.push_back(DeltaRval);
		      }
		      if(hyp_counter == randomhyp)
			rndm_recbdtout[z] = recbdtout[z];
		      
		      
		      if(recbdtout[z] > best_recbdtout[z]){
			best_recbdtout[z]  =  recbdtout[z];
			best_i[z]  =  i;
			best_j[z]  =  j;
			best_k[z]  =  k;
			best_l[z]  =  l;
		      }

		    } // number of BDTs evaluated: for (Int_t z=0;i<nbdts;i++){ ...

                    hyp_counter++;
		    
                  }
                }//end light jet
              }
            }//end b2 from Higgs
          }
        }//end b1 from Higgs
      }  // end b from top
      
      // Fill in which position max(BDT) and min(deltaR) ar
      if (signal){
        vec_bdtout.resize(hyp_counter);
        vec_deltaR.resize(hyp_counter);
        vector<size_t> indices_bdt;
        vector<size_t> indices_dR;
        indices_bdt.resize(hyp_counter);
        indices_dR.resize(hyp_counter);
        for(Int_t i=0; i<hyp_counter; ++i) {
          indices_bdt[i] = i;
          indices_dR[i] = i;
        }
        sort(indices_bdt.begin(),indices_bdt.end(),GreaterByVal(vec_bdtout));
        sort(indices_dR.begin(),indices_dR.end(),SmallerByVal(vec_deltaR));

        hyp_posbdt = std::find(indices_dR.begin(), indices_dR.end(), indices_bdt.at(0)) - indices_dR.begin();
        hyp_posdR = std::find(indices_bdt.begin(), indices_bdt.end(), indices_dR.at(0)) - indices_bdt.begin();
      }

      // !!! Take random as "best" !!
      if (takerandom) {
	for (Int_t z=0;z<nbdts;z++){
	  best_recbdtout[z] = rndm_recbdtout[z];
	  best_i[z] = rndm_i;
	  best_j[z] = rndm_j;
	  best_k[z] = rndm_k;
	  best_l[z] = rndm_l;
	}
      }

      for (Int_t z=0;z<nbdts;z++){
	TLorentzVector rectop    =  p4lepw + p4jets[best_i[z]];
	TLorentzVector rechiggs  =  p4jets[best_j[z]] + p4jets[best_k[z]];
	TLorentzVector reclight  =  p4jets[best_l[z]];
	ljtidx[z]      =    best_l[z];
	hbbm[z]        =    rechiggs.M();
	hbbpt[z]       =    rechiggs.Pt();
	hbbphi[z]      =    rechiggs.Phi();
	hbbeta[z]      =    rechiggs.Eta();
	hbbdr[z]       =    p4jets[best_j[z]].DeltaR(p4jets[best_k[z]]);
	hbbjtidx[0][z] =    best_j[z];
	hbbjtidx[1][z] =    best_k[z];
	topm[z]        =    rectop.M();
	toppt[z]       =    rectop.Pt();
	topphi[z]      =    rectop.Phi();
	topeta[z]      =    rectop.Eta();
	topjtidx[z]    =    best_i[z];
      }

      otree->Fill();

    }else{ //FROM: if (i_nlep == 1 && i_njt > 4 && n_central > 2)
      //      cout << "No valid hypothesis found!\n";
      for (Int_t z=0;z<nbdts;z++){

	ljtidx[z]      =  -99;
	hbbm[z]        =   -99;
	hbbpt[z]       =   -99;
	hbbphi[z]      =   -99; 
	hbbeta[z]      =   -99;
	hbbdr[z]       =   -99;
	hbbjtidx[0][z] =   -99;
	hbbjtidx[1][z] =   -99;
	topm[z]        =   -99;
	toppt[z]       =   -99;
	topphi[z]      =   -99;
	topeta[z]      =   -99;
	topjtidx[z]    =   -99;
	best_recbdtout[z] = -99;
      }
    }
  } // event loop
    
  cout << "End of event loop \n";
  
  otree->Write();
  ofile->Close();
  ifile->Close();
  cout << "Wrote file *.bdt.root \n";
  delete reader_th_0;  
  delete reader_th_1;  
  delete reader_th_2;  
  delete reader_th_3;  
  delete reader_th_4;  
  delete reader_th_5;  
  delete reader_th_6;  
  delete reader_th_7;  
  delete reader_th_8;  
  delete reader_th_9;  
  delete reader_th_10;  
  delete reader_th_11;  
  delete reader_th_12;  
  delete reader_th_13;  
  delete reader_th_14;  
  delete reader_th_15;  
  delete reader_th_16;  
  delete reader_th_17;  
  delete reader_th_18;  
  delete reader_th_19;  
  delete reader_th_20;  
  delete reader_th_21;  
  delete reader_th_22;  
  delete reader_th_23;  
  delete reader_th_24;  
  delete reader_th_25;  
  delete reader_th_26;  
  delete reader_th_27;  
  delete reader_th_28;  
  delete reader_th_29;  
  delete reader_th_30;
  delete reader_th_31;  
  delete reader_th_32;  
  delete reader_th_33;  
  delete reader_th_34;  
  delete reader_th_35;  
  delete reader_th_36;  
  delete reader_th_37;  
  delete reader_th_38;  
  delete reader_th_39;  
  delete reader_th_40;  
  delete reader_th_41;  
  delete reader_th_42;  
  delete reader_th_43;
  delete reader_th_44;  
  delete reader_th_45;  
  delete reader_th_46;  
  delete reader_th_47;  
  delete reader_th_48;  
  delete reader_th_49;  
  delete reader_th_50;  
  delete ifile;
  delete ofile;
  
  string cmd_str = "mv -f " + opath + "/" + output + " " + opath + "/" + input;
  gSystem->Exec(cmd_str.c_str());
  
  cout << "Override original file... !" << endl;
  cout << "==> TMVAClassificationApplication is done! Hell Yeah!" << endl;

  return 0;

  //  std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;
    
 
}

//______________________________________________________________________________
double deltaPhi(double phi1, double phi2)
{
  double result = phi1 - phi2;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}

//______________________________________________________________________________
double deltaR(double eta1, double phi1, double eta2, double phi2)
{
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return std::sqrt(deta*deta + dphi*dphi);
}

//______________________________________
void master_reader_th(TMVA::Reader *reader_th, string weight, TString weightfile)
{
  
  reader_th->AddSpectator("topjtidx", &spec1);
  reader_th->AddSpectator("ljtidx", &spec2);
  reader_th->AddSpectator("hbbjtidx[0]", &spec3);
  reader_th->AddSpectator("hbbjtidx[1]", &spec4);
  reader_th->AddSpectator(weight, &spec5);
  

  
  reader_th->AddVariable("jtcsvt[topjtidx]", 			 &var1 );
  reader_th->AddVariable("abs(jteta[ljtidx])",           &var2 );
  reader_th->AddVariable("jtcsvt[hbbjtidx[0]]", 			 &var3 );
  reader_th->AddVariable("jtcsvt[hbbjtidx[1]]", 			 &var4 );
  reader_th->AddVariable("log(topm)",                    &var5 );
  reader_th->AddVariable("log(hbbm)",                    &var6 );
  reader_th->AddVariable("log(min(jtpt[hbbjtidx[0]],jtpt[hbbjtidx[1]]))", 			 &var7 );
  reader_th->AddVariable("deltaR(jteta[hbbjtidx[0]],jtphi[hbbjtidx[0]],jteta[hbbjtidx[1]],jtphi[hbbjtidx[1]])", 			 &var8 );
  //  reader_th->AddVariable("max(abs(jteta[hbbjtidx[0]]),abs(jteta[hbbjtidx[1]]))", 			 &var9 );
  reader_th->AddVariable("deltaR(jteta[topjtidx],jtphi[topjtidx],lepweta,lepwphi)", 			 &var10 );
  reader_th->AddVariable("(toppt+hbbpt+jtpt[ljtidx])/(sumHt+met+(nel==1 && nmu==0)*Alt$(elpt[0],0)"
			 "+(nel==0 && nmu==1)*Alt$(mupt[0],0))",				 &var11 );
  //  reader_th->AddVariable("deltaR(topeta,topphi,hbbeta,hbbphi)",				 &var12 );
  //  reader_th->AddVariable("jtcharge[topjtidx]*(((nel==1 && nmu==0)*Alt$(elcharge[0],0))"
  //			 "+((nel==0 && nmu==1)*Alt$(mucharge[0],0)))",				 &var13 );
  reader_th->AddVariable("abs(jteta[ljtidx]-jteta[topjtidx])",				 &var14 );
  reader_th->AddVariable("jte[ljtidx]-jte[topjtidx]",				 &var15 );
  reader_th->AddVariable("abs(jteta[topjtidx])",				 &var16 );
  reader_th->AddVariable("abs(topeta-hbbeta)",					 &var17 ); //new
  reader_th->AddVariable("jtcostheta_l[topjtidx]",					 &var18 ); //new

  
  reader_th->BookMVA( methodName, weightfile );

}
