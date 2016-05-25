////////////////////////////////////////////////////////////////////////////////
//
//th_bdtexpert_x
// ------------
//
// tool to classify a single top higgs event with  boosted decision trees (TMVA package) - requires nothin' but root
//
// 
//
//    
//    ::05/14/2012 Benedikt Maier <bmaier@ekp.uni-karlsruhe.de>::
////////////////////////////////////////////////////////////////////////////////

//#include "SchieferD/AnaUtils/interface/MCSample.h"

#include "../interface/CommandLine.h"

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
  GreaterByVal(vector<float>  pts_): pts(pts_){}
  bool operator()(size_t i, size_t j) const{
    return pts.at(i) > pts.at(j);
  }
};

struct SmallerByVal{
  vector<float>  pts;
  SmallerByVal(vector<float>  pts_): pts(pts_){}
  bool operator()(size_t i, size_t j) const{
    return pts.at(i) < pts.at(j);
  }
};

double deltaPhi(double phi1, double phi2);
double deltaR(double eta1, double phi1, double eta2, double phi2);
void master_reader_th(TMVA::Reader *reader_th, TString weightfile, string cvcf_weight_idx );



Float_t var1=0., var2=0.;
Float_t var3=0., var4=0.;
Float_t var5=0., var6=0.;
Float_t var7=0., var8=0.;
Float_t var9=0., var10=0.;
Float_t var11=0., var12=0.;
Float_t var13=0., var14=0.;
Float_t var15=0.;



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
  if (!cl.parse(argc,argv)) return 0;
  
  string         input     = cl.getValue <string>  ("input");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         ipath     = cl.getValue <string>  ("ipath",              ".");
  string         opath     = cl.getValue <string>  ("opath",              ".");
  string         cvcf          = cl.getValue <string>  ("cvcf",                "");
  string         cvcf_weight_idx = cl.getValue <string>("cvcf_weight_idx",   ""); 
  bool           signal  = cl.getValue <bool>      ("signal",         false);
  vector<string> bdtpath   = cl.getVector<string>  ("bdtpath"                );
  int            btagsys   = cl.getValue<int>      ("btagsys",              0);
  
  if(!cl.check()) {};
  cl.print();

// How many different BDTs will be evaluated?
  Int_t nbdts = bdtpath.size();

  cout << "nbdts: " << nbdts << endl;
  string cvcfmap[51]={"weights_syst[0]",
                      "weights_syst[446]",
                      "weights_syst[447]",
                      "weights_syst[448]",
                      "weights_syst[449]",
                      "weights_syst[450]",
                      "weights_syst[451]",
                      "weights_syst[452]",
                      "weights_syst[453]",
                      "weights_syst[454]",
                      "weights_syst[455]",
                      "weights_syst[456]",
                      "weights_syst[457]",
                      "weights_syst[458]",
                      "weights_syst[459]",
                      "weights_syst[460]",
                      "weights_syst[461]",
                      "weights_syst[462]",
                      "weights_syst[463]",
                      "weights_syst[464]",
                      "weights_syst[465]",
                      "weights_syst[466]",
                      "weights_syst[467]",
                      "weights_syst[468]",
                      "weights_syst[469]",
                      "weights_syst[470]",
                      "weights_syst[471]",
                      "weights_syst[472]",
                      "weights_syst[473]",
                      "weights_syst[474]",
                      "weights_syst[475]",
                      "weights_syst[476]",
                      "weights_syst[477]",
                      "weights_syst[478]",
                      "weights_syst[479]",
                      "weights_syst[480]",
                      "weights_syst[481]",
                      "weights_syst[482]",
                      "weights_syst[483]",
                      "weights_syst[484]",
                      "weights_syst[485]",
                      "weights_syst[486]",
                      "weights_syst[487]",
                      "weights_syst[488]",
                      "weights_syst[489]",
                      "weights_syst[490]",
                      "weights_syst[491]",
                      "weights_syst[492]",
                      "weights_syst[493]",
                      "weights_syst[494]",
                      "weights_syst[495]"};


  TMVA::Reader *reader_th[51];

  for (Int_t z = 0; z< 51; z++)
    reader_th[z]= new TMVA::Reader( "!Color:!Silent" );

  TString weightfiles_th[51];


  for (Int_t z=0;z<nbdts;z++){
    weightfiles_th[z]=TString(bdtpath[z]) +"/"+ TString("tH_BDT.weights.xml");
    }


  for (Int_t z = 0; z< 51; z++)
    master_reader_th(reader_th[z],weightfiles_th[z], to_string(z));

  /*
  master_reader_th(reader_th_0,cvcfmap[0],weightfiles_th[0], cvcf_weight_idx);
  master_reader_th(reader_th_1,cvcfmap[1],weightfiles_th[1], cvcf_weight_idx);
  master_reader_th(reader_th_2,cvcfmap[2],weightfiles_th[2], cvcf_weight_idx);
  master_reader_th(reader_th_3,cvcfmap[3],weightfiles_th[3], cvcf_weight_idx);
  master_reader_th(reader_th_4,cvcfmap[4],weightfiles_th[4], cvcf_weight_idx);
  master_reader_th(reader_th_5,cvcfmap[5],weightfiles_th[5], cvcf_weight_idx);
  master_reader_th(reader_th_6,cvcfmap[6],weightfiles_th[6], cvcf_weight_idx);
  master_reader_th(reader_th_7,cvcfmap[7],weightfiles_th[7], cvcf_weight_idx);
  master_reader_th(reader_th_8,cvcfmap[8],weightfiles_th[8], cvcf_weight_idx);
  master_reader_th(reader_th_9,cvcfmap[9],weightfiles_th[9], cvcf_weight_idx);
  master_reader_th(reader_th_10,cvcfmap[10],weightfiles_th[10], cvcf_weight_idx);
  master_reader_th(reader_th_11,cvcfmap[11],weightfiles_th[11], cvcf_weight_idx);
  master_reader_th(reader_th_12,cvcfmap[12],weightfiles_th[12], cvcf_weight_idx);
  master_reader_th(reader_th_13,cvcfmap[13],weightfiles_th[13], cvcf_weight_idx);
  master_reader_th(reader_th_14,cvcfmap[14],weightfiles_th[14], cvcf_weight_idx);
  master_reader_th(reader_th_15,cvcfmap[15],weightfiles_th[15], cvcf_weight_idx);
  master_reader_th(reader_th_16,cvcfmap[16],weightfiles_th[16], cvcf_weight_idx);
  master_reader_th(reader_th_17,cvcfmap[17],weightfiles_th[17], cvcf_weight_idx);
  master_reader_th(reader_th_18,cvcfmap[18],weightfiles_th[18], cvcf_weight_idx);
  master_reader_th(reader_th_19,cvcfmap[19],weightfiles_th[19], cvcf_weight_idx);
  master_reader_th(reader_th_20,cvcfmap[20],weightfiles_th[20], cvcf_weight_idx);
  master_reader_th(reader_th_21,cvcfmap[21],weightfiles_th[21], cvcf_weight_idx);
  master_reader_th(reader_th_22,cvcfmap[22],weightfiles_th[22], cvcf_weight_idx);
  master_reader_th(reader_th_23,cvcfmap[23],weightfiles_th[23], cvcf_weight_idx);
  master_reader_th(reader_th_24,cvcfmap[24],weightfiles_th[24], cvcf_weight_idx);
  master_reader_th(reader_th_25,cvcfmap[25],weightfiles_th[25], cvcf_weight_idx);
  master_reader_th(reader_th_26,cvcfmap[26],weightfiles_th[26], cvcf_weight_idx);
  master_reader_th(reader_th_27,cvcfmap[27],weightfiles_th[27], cvcf_weight_idx);
  master_reader_th(reader_th_28,cvcfmap[28],weightfiles_th[28], cvcf_weight_idx);
  master_reader_th(reader_th_29,cvcfmap[29],weightfiles_th[29], cvcf_weight_idx);
  master_reader_th(reader_th_30,cvcfmap[30],weightfiles_th[30], cvcf_weight_idx);
  master_reader_th(reader_th_31,cvcfmap[31],weightfiles_th[31], cvcf_weight_idx);
  master_reader_th(reader_th_32,cvcfmap[32],weightfiles_th[32], cvcf_weight_idx);
  master_reader_th(reader_th_33,cvcfmap[33],weightfiles_th[33], cvcf_weight_idx);
  master_reader_th(reader_th_34,cvcfmap[34],weightfiles_th[34], cvcf_weight_idx);
  master_reader_th(reader_th_35,cvcfmap[35],weightfiles_th[35], cvcf_weight_idx);
  master_reader_th(reader_th_36,cvcfmap[36],weightfiles_th[36], cvcf_weight_idx);
  master_reader_th(reader_th_37,cvcfmap[37],weightfiles_th[37], cvcf_weight_idx);
  master_reader_th(reader_th_38,cvcfmap[38],weightfiles_th[38], cvcf_weight_idx);
  master_reader_th(reader_th_39,cvcfmap[39],weightfiles_th[39], cvcf_weight_idx);
  master_reader_th(reader_th_40,cvcfmap[40],weightfiles_th[40], cvcf_weight_idx);
  master_reader_th(reader_th_41,cvcfmap[41],weightfiles_th[41], cvcf_weight_idx);
  master_reader_th(reader_th_42,cvcfmap[42],weightfiles_th[42], cvcf_weight_idx);
  master_reader_th(reader_th_43,cvcfmap[43],weightfiles_th[43], cvcf_weight_idx);
  master_reader_th(reader_th_44,cvcfmap[44],weightfiles_th[44], cvcf_weight_idx);
  master_reader_th(reader_th_45,cvcfmap[45],weightfiles_th[45], cvcf_weight_idx);
  master_reader_th(reader_th_46,cvcfmap[46],weightfiles_th[46], cvcf_weight_idx);
  master_reader_th(reader_th_47,cvcfmap[47],weightfiles_th[47], cvcf_weight_idx);
  master_reader_th(reader_th_48,cvcfmap[48],weightfiles_th[48], cvcf_weight_idx);
  master_reader_th(reader_th_49,cvcfmap[49],weightfiles_th[49], cvcf_weight_idx);
  master_reader_th(reader_th_50,cvcfmap[50],weightfiles_th[50], cvcf_weight_idx);
  */

  

  string prefix = "TMVAClassification";
  
  // read input file
  TFile* ifile = new TFile((ipath+"/"+input).c_str(),"READ");
  if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }
  
  // create temporary output file
  size_t pos = input.find(".root");
  string output = input.substr(0,pos);
  output = output + ".bdt.root";
  TFile* ofile = new TFile((opath+"/"+output).c_str(),"RECREATE");
  if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
  
  // read input directory
  TDirectory* idir    = (TDirectory*)ifile->Get(dirname.c_str());
  cout<<input<<"/"<<dirname<<":"<<endl;
  
  // create output directory
  TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
  if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}
  
  // open input tree
  TTree* itree = (TTree*)idir->Get(treename.c_str());
  if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<dirname<<endl;return 0;}
  
  // don't copy bdtout branch if it already exists
  if (0!=itree->FindBranch("bdtout")) itree->SetBranchStatus("bdtout",0);
  if (0!=itree->FindBranch("bdtgout")) itree->SetBranchStatus("bdtgout",0);

  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree(0);
  cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;


  // create bdtout branches in output tree
  Float_t  bdtout[51]={-99}; otree->Branch("bdtout",&bdtout, "bdtout[51]/F");
  
  //Get relevant information for creating hypothesis
  Int_t hbbjtidx[3][51];           itree->SetBranchAddress("hbbjtidx",       hbbjtidx);
  Int_t topjtidx[51];              itree->SetBranchAddress("topjtidx",       topjtidx);
  Int_t ljtidx[51];                itree->SetBranchAddress("ljtidx",           ljtidx);
  Int_t tophadjtidx[3];            itree->SetBranchAddress("tophadjtidx", tophadjtidx);
  Int_t toplepjtidx;               itree->SetBranchAddress("toplepjtidx",&toplepjtidx);

  Float_t hbbm[51];            itree->SetBranchAddress("hbbm",               hbbm);
  Float_t hbbpt[51];           itree->SetBranchAddress("hbbpt",             hbbpt);
  Float_t hbbphi[51];          itree->SetBranchAddress("hbbphi",           hbbphi);
  Float_t hbbeta[51];          itree->SetBranchAddress("hbbeta",           hbbeta);
  Float_t hbbdr[51];           itree->SetBranchAddress("hbbdr",             hbbdr);
  Float_t topm[51];            itree->SetBranchAddress("topm",               topm);
  Float_t topeta[51];          itree->SetBranchAddress("topeta",           topeta);
  Float_t toppt[51];           itree->SetBranchAddress("toppt",             toppt);
  Float_t topphi[51];          itree->SetBranchAddress("topphi",           topphi);

  Float_t met=-99;                 itree->SetBranchAddress("met",                &met);
  Int_t nel={-99};                 itree->SetBranchAddress("nel",                &nel);
  Int_t nmu={-99};                 itree->SetBranchAddress("nmu",                &nmu);
  Float_t elcharge[10]={-99};      itree->SetBranchAddress("elcharge",       elcharge);
  Float_t mucharge[10]={-99};      itree->SetBranchAddress("mucharge",       mucharge);

  Int_t   njt=-99;                 itree->SetBranchAddress("njt",                 &njt);
  Float_t jtpt[40]={-99};          itree->SetBranchAddress("jtpt",                jtpt);
  Float_t jteta[40]={-99};         itree->SetBranchAddress("jteta",              jteta);
  Float_t jtphi[40]={-99};         itree->SetBranchAddress("jtphi",              jtphi);
  Float_t jtcsvt[40]={-99};        itree->SetBranchAddress("jtcsvt",            jtcsvt);
  Float_t jtcostheta_l[40]={-99};  itree->SetBranchAddress("jtcostheta_l",jtcostheta_l);


  Float_t lepwm={-99};             itree->SetBranchAddress("lepwm",            &lepwm);
  Float_t lepwpt={-99};            itree->SetBranchAddress("lepwpt",          &lepwpt);
  Float_t lepwphi={-99};           itree->SetBranchAddress("lepwphi",        &lepwphi);
  Float_t lepweta={-99};           itree->SetBranchAddress("lepweta",        &lepweta);
  
  Float_t tophadm={-99};           itree->SetBranchAddress("tophadm",        &tophadm);
  Float_t toplepm={-99};           itree->SetBranchAddress("toplepm",        &toplepm);
  Float_t tophadpt={-99};          itree->SetBranchAddress("tophadpt",      &tophadpt);
  Float_t tophadwm={-99};          itree->SetBranchAddress("tophadwm",      &tophadwm);

  Float_t wolframh1={-99};           itree->SetBranchAddress("wolframh1",  &wolframh1);
  Float_t m3={-99};                  itree->SetBranchAddress("m3",                &m3);
  Float_t aplanarity={-99};          itree->SetBranchAddress("aplanarity",&aplanarity);


  Float_t sumHt={-99};             itree->SetBranchAddress("sumHt",            &sumHt);

  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  cout<<nevt<<" events will be processed"<<endl;
  //event loop
  for (unsigned ievt=0;ievt<nevt;ievt++) {
 
    
    itree->GetEntry(ievt);
    if (ievt %10 == 0 || ievt <= 10 || nevt-ievt <= 10){
      cout << "\r  > Processing events from input tree:  (\033[33m" << ievt+1 << "/" << nevt << "\033[0m)  -> \033[32m";
      cout << setprecision(4) << setiosflags(std::ios_base::left) << setfill('0') << setw(5) \
	   << ((ievt+1)*100.0)/nevt << "\033[0m%    \b\b\b" << flush;
    }


    for (Int_t z = 0; z< 51; z++){

      var1   =  (nmu==1)? mucharge[0]:elcharge[0];
      var2   =  log(tophadm);
      var3   =  jtcsvt[hbbjtidx[0][z]];
      var4   =  jtcsvt[hbbjtidx[1][z]];
      var5   =  log(hbbpt[z]);
      var6   =  deltaR(jteta[tophadjtidx[0]],jtphi[tophadjtidx[0]],jteta[tophadjtidx[1]],jtphi[tophadjtidx[1]]);
      var7   =  jtcsvt[tophadjtidx[0]];
      var8   =  jtcsvt[tophadjtidx[1]];
      var9   =  log(jtpt[ljtidx[z]]);
      var10  =  abs(jteta[ljtidx[z]]);
      var11  =  deltaR(topeta[z],topphi[z],hbbeta[z],hbbphi[z]);
      var12  =  jtcostheta_l[topjtidx[z]];
      var13  =  wolframh1;
      var14  =  log(m3);
      var15  =  aplanarity;

      bdtout[z]=reader_th[z]->EvaluateMVA( "BDT method"  );
    }
    
    otree->Fill();

  } // event loop
    
  cout << "End of event loop \n";
  
  otree->Write();
  ofile->Close();
  ifile->Close();
  cout << "Wrote file *.bdt.root \n";
  
  for (Int_t z = 0; z< 51; z++)
    delete reader_th[z];

  delete ifile;
  delete ofile;
  
  string cmd_str = "mv " + opath+"/"+output + " " + opath+"/"+input;
  gSystem->Exec(cmd_str.c_str());
  
  cout << "Override original file... !" << endl;
  cout << "==> TMVAClassificationApplication is done! Hell Yeah!" << endl;

  return 0;
 
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
void master_reader_th(TMVA::Reader *reader_th, TString weightfile, string cvcf_weight_idx)
{


  string specs1 ="topjtidx[" + cvcf_weight_idx + "]";
  string specs2 ="ljtidx[" + cvcf_weight_idx + "]";
  string specs3 ="hbbjtidx[0][" + cvcf_weight_idx + "]";
  string specs4 ="hbbjtidx[1][" + cvcf_weight_idx + "]";

  Float_t spec1=0;
  Float_t spec2=0;
  Float_t spec3=0;
  Float_t spec4=0;
  Float_t spec5=0;
  Float_t spec6=0;
  Float_t spec7=0;
  Float_t spec8=0;
 
  reader_th->AddSpectator(specs1.c_str(),   &spec1);
  reader_th->AddSpectator(specs2.c_str(),   &spec2);
  reader_th->AddSpectator(specs3.c_str(),   &spec3);
  reader_th->AddSpectator(specs4.c_str(),   &spec4);
  reader_th->AddSpectator("toplepjtidx",    &spec5);
  reader_th->AddSpectator("tophadjtidx[0]", &spec6);
  reader_th->AddSpectator("tophadjtidx[1]", &spec7);
  reader_th->AddSpectator("tophadjtidx[2]", &spec8);

  string loghbbpt        = "log(hbbpt[" + cvcf_weight_idx + "])";
  string jtcsvthbbjtidx1 = "jtcsvt[" + specs3 + "]";
  string jtcsvthbbjtidx2 = "jtcsvt[" + specs4 + "]";
  string logjtptljt      = "log(jtpt[ljtidx[" + cvcf_weight_idx + "]])";
  string absetaljt       = "abs(jteta[ljtidx[" + cvcf_weight_idx + "]])";
  
  string drtophiggs      = "deltaR(topeta["+ cvcf_weight_idx +"],topphi["+ cvcf_weight_idx +"],hbbeta["+ cvcf_weight_idx +"],hbbphi["+ cvcf_weight_idx +"])";
  string costheta        = "jtcostheta_l[topjtidx["+ cvcf_weight_idx +"]]";


  reader_th->AddVariable("(nel==1 && nmu==0)*Alt$(elcharge[0],0)+(nel==0 && nmu==1)*Alt$(mucharge[0],0)", \
                         &var1 );
  reader_th->AddVariable("log(tophadm)",           &var2 );
  reader_th->AddVariable(jtcsvthbbjtidx1.c_str(),	\
                         &var3 );
  reader_th->AddVariable(jtcsvthbbjtidx2.c_str(),	\
                         &var4 );
  reader_th->AddVariable(loghbbpt.c_str(),                    &var5 );
  reader_th->AddVariable("deltaR(jteta[tophadjtidx[0]],jtphi[tophadjtidx[0]],jteta[tophadjtidx[1]],jtphi[tophadjtidx[1]])"
,                    &var6 );
  reader_th->AddVariable("jtcsvt[tophadjtidx[0]]", \
                         &var7 );
  reader_th->AddVariable("jtcsvt[tophadjtidx[1]]", \
                         &var8 );
  reader_th->AddVariable(logjtptljt.c_str(),	\
                         &var9 );
  reader_th->AddVariable(absetaljt.c_str(),	\
                         &var10 );

  reader_th->AddVariable(drtophiggs.c_str(),	\
                         &var11 );
  reader_th->AddVariable(costheta.c_str(),	\
                         &var12 );
  reader_th->AddVariable("wolframh1", \
                         &var13 );
  reader_th->AddVariable("log(m3)", \
                         &var14 );
  reader_th->AddVariable("aplanarity", \
                         &var15 );


  reader_th->BookMVA( "BDT method", weightfile );

}

