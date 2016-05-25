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
  // bool           signal  = cl.getValue <bool>    ("signal",         false);
  string         bdtpath   = cl.getValue<string>   ("bdtpath"                );
  int            btagsys   = cl.getValue<int>      ("btagsys",              0);
  
  if(!cl.check()) {};
  cl.print();
  
  assert (std::abs(btagsys)<5);
  
  if      (0==btagsys) cout<<"Nominal mode"<<endl;
  else if (1==btagsys) cout<<"Systematic BTAG UP"<<endl;
  else if (2==btagsys) cout<<"Systematic BTAG DOWN"<<endl;
  else if (3==btagsys) cout<<"Systematic MISTAG UP"<<endl;
  else if (4==btagsys) cout<<"Systematic MISTAG DOWN"<<endl;
  
  // --- Create the Reader object
  
  TMVA::Reader *reader_th = new TMVA::Reader( "!Color:!Silent" );    
  

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
  Float_t var19=0.; /*var20=0.;
  Float_t var21=0., var22=0.;
  Float_t var23=0., var24=0.;
  Float_t var25=0., var26=0.;
  Float_t var27=0., var28=0.;
  Float_t var29=0., var30=0.;
  Float_t var31=0., var32=0.;
  Float_t var33=0., var34=0.;
  Float_t var35=0., var36=0.;
		    
  Int_t spec1=0;
  Int_t spec2=0;
  Int_t spec3=0;
  Int_t spec4=0;
  Int_t spec5=0;
  Int_t spec6=0;
  Int_t spec7=0;
  Int_t spec8=0;
  

		    */

  reader_th->AddVariable( "glb_Charge_Lep",                                   &var1 );
  //  reader_th->AddVariable( "log(glb_SqrtSHat)",                                &var2 );
  //reader_th->AddVariable( "log(glb_Pt_J1)",                                   &var3 );
  //reader_th->AddVariable( "log(glb_MET)",                                     &var4 );
  //reader_th->AddVariable( "log(thq_Pt_Higgs)",                                &var5 );
  //reader_th->AddVariable( "log(thq_Pt_Recoil)",                               &var6 );
  //reader_th->AddVariable( "thq_Cos_LepRecoil_TH",                             &var7 );
  reader_th->AddVariable( "abs(thq_Eta_Recoil)",                              &var2 );
  //  reader_th->AddVariable( "glb_Sphericity" ,                                  &var9 );
  //reader_th->AddVariable( "tt_MaxEta_Light",                                  &var9);
  //reader_th->AddVariable( "log(tt_Mass_TopHad)",                              &var10);
  //reader_th->AddVariable( "log(tt_DMass_TopHadWHad)" ,                        &var11);
  //reader_th->AddVariable( "log(tt_MaxMass_BTopHadLight)",                     &var12);
  reader_th->AddVariable( "thq_NumBTag_Higgs" ,                               &var3);
  reader_th->AddVariable( "log(thq_Pt_Higgs)",                                &var4);
  reader_th->AddVariable( "log(thq_Pt_Recoil)",                               &var5);
  reader_th->AddVariable( "tt_DeltaR_Light",                                  &var6);
  reader_th->AddVariable( "log(tt_Mass_TopHad)",                              &var7);
  reader_th->AddVariable( "tt_NumPassBTag_Light",                             &var8);
  //reader_th->AddVariable( "thq_DeltaR_BJetsHiggs",                            &var15);
  //reader_th->AddVariable( "tt_DeltaR_Light",                                  &var16);
  //reader_th->AddVariable( "tt_SumCharge_Light" ,                              &var17);
  //reader_th->AddVariable( "tt_Charge_BTopHad",                                &var18);
  
 
  /*  reader_th->AddVariable( "jtcsvtcorr[ljtidx][0]",                                  &var18);
  reader_th->AddVariable( "jtcsvtcorr[hbbjtidx[0]][0]",                             &var19);
  reader_th->AddVariable( "jtcsvtcorr[hbbjtidx[1]][0]" ,                            &var20);
  reader_th->AddVariable( "jtcsvtcorr[topjtidx][0]",                                &var21);
  reader_th->AddVariable( "tophadm",                                                &var22);
  reader_th->AddVariable( "tophadpt",                                               &var23);
  reader_th->AddVariable( "jtcsvtcorr[tophadjtidx[2]][0]",                          &var24);
  reader_th->AddVariable( "jtcsvtcorr[toplepjtidx][0]",                             &var25);
  reader_th->AddVariable( "jtcsvtcorr[tophadjtidx[0]][0]",                          &var26);
  reader_th->AddVariable( "jtcsvtcorr[tophadjtidx[1]][0]",                          &var27);
  reader_th->AddVariable( "toplepm",                                                &var28);
  reader_th->AddVariable( "tophadwm",                                               &var29); */
  //reader_th->AddVariable( "twist",                                                  &var30);
  //reader_th->AddVariable( "costhetastarhiggs",                                      &var31);
  //reader_th->AddVariable( "costhetastar",                                           &var32);
  //reader_th->AddVariable( "coszetastar",                                            &var33);
  //reader_th->AddVariable( "helicity_h1",                                            &var34);
  //reader_th->AddVariable( "helicity_ljt",                                           &var35);
  //reader_th->AddVariable( "helicity_topjt",                                          &var36);

  /*
  reader_th->AddVariable( "0+(jtcsvtcorr[ljtidx][0]>0.244 &&jtcsvtcorr[ljtidx][0]<0.679)+2*(jtcsvtcorr[ljtidx][0]>0.679 &&jtcsvtcorr[ljtidx][0]<0.898)+3*(jtcsvtcorr[ljtidx][0]>0.898)",                                  &var24);
  reader_th->AddVariable( "0+(jtcsvtcorr[hbbjtidx[0]][0]>0.244 &&jtcsvtcorr[hbbjtidx[0]][0]<0.679)+2*(jtcsvtcorr[hbbjtidx[0]][0]>0.679 &&jtcsvtcorr[hbbjtidx[0]][0]<0.898)+3*(jtcsvtcorr[hbbjtidx[0]][0]>0.898)",                             &var25);
  reader_th->AddVariable( "0+(jtcsvtcorr[hbbjtidx[1]][0]>0.244 &&jtcsvtcorr[hbbjtidx[1]][0]<0.679)+2*(jtcsvtcorr[hbbjtidx[1]][0]>0.679 &&jtcsvtcorr[hbbjtidx[1]][0]<0.898)+3*(jtcsvtcorr[hbbjtidx[1]][0]>0.898)" ,                            &var26);
  reader_th->AddVariable( "0+(jtcsvtcorr[topjtidx][0]>0.244 &&jtcsvtcorr[topjtidx][0]<0.679)+2*(jtcsvtcorr[topjtidx][0]>0.679 &&jtcsvtcorr[topjtidx][0]<0.898)+3*(jtcsvtcorr[topjtidx][0]>0.898)",                                &var27);
  */





  string prefix = "TMVAClassification";
   
  // Book method(s)   WH110_BDTD.weights.xml
    
  //TString methodName = TString("BDT") + TString(" method");
  TString methodName = TString("MLPBFGS") + TString(" method");
  //TString weightfile_th = TString(bdtpath) +"/"+ TString("tHnominal_BDT.weights.xml");
  TString weightfile_th = TString(bdtpath) +"/"+ TString("THvsBkg_Global_THvsBkg_Global_BFGS.weights.xml");
 

  reader_th->BookMVA( methodName, weightfile_th );

  
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
  
  // don't copy mlpout branch if it already exists
  if (0!=itree->FindBranch("mlpout")) itree->SetBranchStatus("mlpout",0);
  
  /* if (0!=itree->FindBranch("var15")) itree->SetBranchStatus("var15",0);
  if (0!=itree->FindBranch("popvar1")) itree->SetBranchStatus("popvar1",0);
  if (0!=itree->FindBranch("popvar2")) itree->SetBranchStatus("popvar2",0);
  if (0!=itree->FindBranch("popvar3")) itree->SetBranchStatus("popvar3",0);
  if (0!=itree->FindBranch("popvar4")) itree->SetBranchStatus("popvar4",0);
  if (0!=itree->FindBranch("popvar5")) itree->SetBranchStatus("popvar5",0);
  if (0!=itree->FindBranch("popvar6")) itree->SetBranchStatus("popvar6",0);
  if (0!=itree->FindBranch("popvar7")) itree->SetBranchStatus("popvar7",0);
  if (0!=itree->FindBranch("popvar8")) itree->SetBranchStatus("popvar8",0);
  if (0!=itree->FindBranch("popvar9")) itree->SetBranchStatus("popvar9",0);
  if (0!=itree->FindBranch("popvar10")) itree->SetBranchStatus("popvar10",0);
  if (0!=itree->FindBranch("popvar11")) itree->SetBranchStatus("popvar11",0);
  if (0!=itree->FindBranch("popvar12")) itree->SetBranchStatus("popvar12",0);
  if (0!=itree->FindBranch("popvar13")) itree->SetBranchStatus("popvar13",0);
  if (0!=itree->FindBranch("popvar14")) itree->SetBranchStatus("popvar14",0);
  if (0!=itree->FindBranch("popvar15")) itree->SetBranchStatus("popvar15",0);
  if (0!=itree->FindBranch("popvar16")) itree->SetBranchStatus("popvar16",0);
  if (0!=itree->FindBranch("popvar17")) itree->SetBranchStatus("popvar17",0);
  if (0!=itree->FindBranch("popvar18")) itree->SetBranchStatus("popvar18",0);
  if (0!=itree->FindBranch("popvar19")) itree->SetBranchStatus("popvar19",0);
  */
 

  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree(0);
  cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;
  /*
  if (0!=itree->FindBranch("popvar1")) itree->SetBranchStatus("popvar1",1);
  if (0!=itree->FindBranch("popvar2")) itree->SetBranchStatus("popvar2",1);
  if (0!=itree->FindBranch("popvar3")) itree->SetBranchStatus("popvar3",1);
  if (0!=itree->FindBranch("popvar4")) itree->SetBranchStatus("popvar4",1);
  if (0!=itree->FindBranch("popvar5")) itree->SetBranchStatus("popvar5",1);
  if (0!=itree->FindBranch("popvar6")) itree->SetBranchStatus("popvar6",1);
  if (0!=itree->FindBranch("popvar7")) itree->SetBranchStatus("popvar7",1);
  if (0!=itree->FindBranch("popvar8")) itree->SetBranchStatus("popvar8",1);
  if (0!=itree->FindBranch("popvar9")) itree->SetBranchStatus("popvar9",1);
  if (0!=itree->FindBranch("popvar10")) itree->SetBranchStatus("popvar10",1);
  if (0!=itree->FindBranch("popvar11")) itree->SetBranchStatus("popvar11",1);
  if (0!=itree->FindBranch("popvar12")) itree->SetBranchStatus("popvar12",1);
  if (0!=itree->FindBranch("popvar13")) itree->SetBranchStatus("popvar13",1);
  if (0!=itree->FindBranch("popvar14")) itree->SetBranchStatus("popvar14",1);
  if (0!=itree->FindBranch("popvar15")) itree->SetBranchStatus("popvar15",1);
  if (0!=itree->FindBranch("popvar16")) itree->SetBranchStatus("popvar16",1);
  if (0!=itree->FindBranch("popvar17")) itree->SetBranchStatus("popvar17",1);
  if (0!=itree->FindBranch("popvar18")) itree->SetBranchStatus("popvar18",1);
  if (0!=itree->FindBranch("popvar19")) itree->SetBranchStatus("popvar19",1);
  */
  // create mlpout branches in output tree
  Float_t  mlpout=-99; otree->Branch("mlpout",&mlpout, "mlpout/F");
 
  

  //Get relevant information for creating hypothesis
 
  Int_t hbbjtidx[3];               itree->SetBranchAddress("hbbjtidx",       hbbjtidx);
  Int_t topjtidx;                  itree->SetBranchAddress("topjtidx",      &topjtidx);
  Int_t ljtidx;                    itree->SetBranchAddress("ljtidx",          &ljtidx);
  Int_t tophadjtidx[3];            itree->SetBranchAddress("tophadjtidx", tophadjtidx);
  Int_t toplepjtidx;               itree->SetBranchAddress("toplepjtidx",&toplepjtidx);

  Float_t hbbm=-99;                itree->SetBranchAddress("hbbm",              &hbbm);
  Float_t hbbpt=-99;               itree->SetBranchAddress("hbbpt",            &hbbpt);
  Float_t hbbphi=-99;              itree->SetBranchAddress("hbbphi",          &hbbphi);
  Float_t hbbeta=-99;              itree->SetBranchAddress("hbbeta",          &hbbeta);
  Float_t hbbdr=-99;               itree->SetBranchAddress("hbbdr",            &hbbdr);
  Float_t topm=-99;                itree->SetBranchAddress("topm",              &topm);
  Float_t topeta=-99;              itree->SetBranchAddress("topeta",          &topeta);
  Float_t toppt=-99;               itree->SetBranchAddress("toppt",            &toppt);
  Float_t topphi=-99;              itree->SetBranchAddress("topphi",          &topphi);

  Float_t sphericity=-99;          itree->SetBranchAddress("sphericity",  &sphericity);
  Float_t aplanarity=-99;          itree->SetBranchAddress("aplanarity",  &aplanarity);
  Float_t met=-99;                 itree->SetBranchAddress("met",                &met);

  Int_t   njt=-99;                 itree->SetBranchAddress("njt",                &njt);
  Float_t jtpt[40]={-99};          itree->SetBranchAddress("jtpt",               jtpt);
  Float_t jteta[40]={-99};         itree->SetBranchAddress("jteta",             jteta);
  Float_t jtphi[40]={-99};         itree->SetBranchAddress("jtphi",             jtphi);
  Float_t jtcsvtcorr[40][5];       itree->SetBranchAddress("jtcsvtcorr",   jtcsvtcorr);
  Float_t jtntracks[40]={-99};     itree->SetBranchAddress("jtntracks",     jtntracks);
  Float_t jtcsvt[40];              itree->SetBranchAddress("jtcsvt",           jtcsvt);
  Float_t jtcharge[40]={-99};      itree->SetBranchAddress("jtcharge",       jtcharge);
  Float_t jte[40]={-99};           itree->SetBranchAddress("jte",                 jte);

  Float_t lepwm={-99};             itree->SetBranchAddress("lepwm",            &lepwm);
  Float_t lepwpt={-99};            itree->SetBranchAddress("lepwpt",          &lepwpt);
  Float_t lepwphi={-99};           itree->SetBranchAddress("lepwphi",        &lepwphi);
  Float_t lepweta={-99};           itree->SetBranchAddress("lepweta",        &lepweta);
  

  Float_t tophadm={-99};           itree->SetBranchAddress("tophadm",        &tophadm);
  Float_t toplepm={-99};           itree->SetBranchAddress("toplepm",        &toplepm);
  Float_t tophadpt={-99};          itree->SetBranchAddress("tophadpt",      &tophadpt);
  Float_t tophadwm={-99};          itree->SetBranchAddress("tophadwm",      &tophadwm);

  
  Float_t sumHt={-99};             itree->SetBranchAddress("sumHt",            &sumHt);
  Float_t elcharge[10]={-99};      itree->SetBranchAddress("elcharge",       elcharge);
  Float_t mucharge[10]={-99};      itree->SetBranchAddress("mucharge",       mucharge);
  Float_t coststh_rec={-99};       itree->SetBranchAddress("coststh_rec",&coststh_rec);

  Int_t nel={-99};                 itree->SetBranchAddress("nel",                &nel);
  Int_t nmu={-99};                 itree->SetBranchAddress("nmu",                &nmu);
 
  /*
  otree->Branch("popvar1",      &var1, "popvar1/F");
  otree->Branch("popvar2",      &var2, "popvar2/F");
  otree->Branch("popvar3",      &var3, "popvar3/F");
  otree->Branch("popvar4",      &var4, "popvar4/F");
  otree->Branch("popvar5",      &var5, "popvar5/F");
  otree->Branch("popvar6",      &var6, "popvar6/F");
  otree->Branch("popvar7",      &var7, "popvar7/F");
  otree->Branch("popvar8",      &var8, "popvar8/F");
  otree->Branch("popvar9",      &sphericity, "popvar9/F");
  otree->Branch("popvar10",      &var9, "popvar10/F");
  otree->Branch("popvar11",      &var10, "popvar11/F");
  otree->Branch("popvar12",      &var11, "popvar12/F");
  otree->Branch("popvar13",      &var12, "popvar13/F");
  otree->Branch("popvar14",      &var13, "popvar14/F");
  otree->Branch("popvar15",      &var14, "popvar15/F");
  otree->Branch("popvar16",      &var15, "popvar16/F");
  otree->Branch("popvar17",      &var16, "popvar17/F");
  otree->Branch("popvar18",      &var17, "popvar18/F");
  otree->Branch("popvar19",      &var18, "popvar19/F");
  

  
  Float_t twist={-99};             itree->SetBranchAddress("twist",            &twist);
  Float_t costhetastarhiggs={-99}; itree->SetBranchAddress("costhetastarhiggs",&costhetastarhiggs);
  Float_t costhetastar={-99};      itree->SetBranchAddress("costhetastar",     &costhetastar);
  Float_t coszetastar={-99};       itree->SetBranchAddress("coszetastar",      &coszetastar);
  Float_t helicity_h1={-99};       itree->SetBranchAddress("helicity_h1",      &helicity_h1);
  Float_t helicity_ljt={-99};      itree->SetBranchAddress("helicity_ljt",     &helicity_ljt);
  Float_t helicity_topjt={-99};    itree->SetBranchAddress("helicity_topjt",   &helicity_topjt);

  */

  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  THStatus* status = new THStatus(nevt);
  cout<<nevt<<" events will be processed"<<endl;
  //event loop
  for (unsigned ievt=0;ievt<nevt;ievt++) {
 
    //   if (ievt%1000 == 0 || ievt <= 10) 
    //  std::cout << "--- ... Processing event: " << ievt << std::endl;
    
    status->print_status(ievt);
    
    itree->GetEntry(ievt);

    TLorentzVector p4jets[100], p4alljets;
    TLorentzVector p4lepw,p4tophadb;
    p4alljets.SetPtEtaPhiE(0,0,0,0);
    for(int jets=0; jets<njt; jets++){
      
      double newEta = jteta[jets];
      if(abs(jteta[jets]) > 2.4){
	int signEta = ((jteta[jets]> 0.) ? +1 : -1);  


	if(abs(jteta[jets]) < 3.2)
	  newEta = 2.8 * signEta;
	else 
	  newEta = 3.5 * signEta;
      }
      
      jteta[jets]= newEta;
      
      p4jets[jets].SetPtEtaPhiE(jtpt[jets],jteta[jets],jtphi[jets],jte[jets]);
      
      p4alljets=p4alljets+p4jets[jets];
    }
    p4lepw.SetPtEtaPhiM(lepwpt,lepweta,lepwphi,lepwm);
   
    Float_t lepcharge=0;
    if(nel==0 && nmu==1){
      lepcharge=mucharge[0];
    }
    else if (nel==1 && nmu ==0 ){
      lepcharge=elcharge[0];
    }
    
    if(hbbjtidx[0]==-99) {
      mlpout = -99;
    }
    else{
      
      var1   =  (nmu==1)? mucharge[0]:elcharge[0]; //glb_Charge_Lep
      var2   =  TMath::Abs(jteta[ljtidx]);  //abs(thq_Eta_Recoil)
      var3   =  (jtcsvt[hbbjtidx[0]]>0.898)*(TMath::Abs(jteta[hbbjtidx[0]])<2.4)+(jtcsvt[hbbjtidx[1]]>0.898)*(TMath::Abs(jteta[hbbjtidx[1]])<2.4); //thq_NumBTag_Higgs
      var4   =  TMath::Log(hbbpt);    //log(thq_Pt_Higgs)
      var5   =  TMath::Log(jtpt[ljtidx]);  //log(thq_Pt_Recoil)
      var6   =  deltaR(jteta[tophadjtidx[0]],jtphi[tophadjtidx[0]],jteta[tophadjtidx[1]],jtphi[tophadjtidx[1]]); //tt_DeltaR_Light
      var7   =  TMath::Log(tophadm);   //log(tt_Mass_TopHad)
      var8   =  (jtcsvt[tophadjtidx[0]]>0.898)*(TMath::Abs(jteta[tophadjtidx[0]])<2.4)+(jtcsvt[tophadjtidx[1]]>0.898)*(TMath::Abs(jteta[tophadjtidx[1]])<2.4); //tt_NumPassBTag_Light
      

    /*

    var2   =  TMath::Log((p4alljets+p4lepw).M());  //log(glb_SqrtSHat)
    var3   =  TMath::Log(jtpt[0]);  //log(glb_Pt_J1)
    var4   =  TMath::Log(met); //log(glb_MET)  


    var7   =  coststh_rec;  //thq_Cos_LepRecoil_TH


    //  var9   =  sphericity;   //glb_Sphericity
    var9 =  TMath::Max(TMath::Abs(jteta[tophadjtidx[0]]),TMath::Abs(jteta[tophadjtidx[1]])); //tt_MaxEta_Light

    var11  =  TMath::Log(tophadm-tophadwm);  //log(tt_DMass_TopHadWHad)
    var12  =  TMath::Log(TMath::Max((p4jets[tophadjtidx[2]]+p4jets[tophadjtidx[0]]).M(),(p4jets[tophadjtidx[2]]+p4jets[tophadjtidx[1]]).M())); //log(tt_MaxMass_BTopHadLight)


    var15  =  hbbdr;  //thq_DeltaR_BJetsHiggs

    var17  =  (jtcharge[tophadjtidx[1]]+jtcharge[tophadjtidx[0]])*lepcharge; //tt_SumCharge_Light
    var18  =  jtcharge[tophadjtidx[2]]*lepcharge; //tt_Charge_BTopHad
    //var22  =  met;
    //var23  =  (jtntracks[hbbjtidx[0]]/jtpt[hbbjtidx[0]])/(jtntracks[topjtidx]/jtpt[topjtidx]);

    */

    /*
    var30  =  twist;
    var31  =  costhetastarhiggs;
    var32  =  costhetastar;
    var33  =  coszetastar;
    var34  =  helicity_h1;
    var35  =  helicity_ljt;
    var36  =  helicity_topjt;

    

    
    if(0==btagsys)
      {   
	
	var18  =  jtcsvtcorr[ljtidx][0];
	var19  =  jtcsvtcorr[hbbjtidx[0]][0];
	var20  =  jtcsvtcorr[hbbjtidx[1]][0];
	var21  =  jtcsvtcorr[topjtidx][0];
	var24  =  jtcsvtcorr[tophadjtidx[2]][0];
	var25  =  jtcsvtcorr[toplepjtidx][0];
	var26  =  jtcsvtcorr[tophadjtidx[0]][0];
	var27  =  jtcsvtcorr[tophadjtidx[1]][0];

      }
	
	
	var24 = 0+(jtcsvtcorr[ljtidx][0]>0.244 &&jtcsvtcorr[ljtidx][0]<0.679)+2*(jtcsvtcorr[ljtidx][0]>0.679 &&jtcsvtcorr[ljtidx][0]<0.898)+3*(jtcsvtcorr[ljtidx][0]>0.898);
	var25 = 0+(jtcsvtcorr[hbbjtidx[0]][0]>0.244 &&jtcsvtcorr[hbbjtidx[0]][0]<0.679)+2*(jtcsvtcorr[hbbjtidx[0]][0]>0.679 &&jtcsvtcorr[hbbjtidx[0]][0]<0.898)+3*(jtcsvtcorr[hbbjtidx[0]][0]>0.898);
	var26 = 0+(jtcsvtcorr[hbbjtidx[1]][0]>0.244 &&jtcsvtcorr[hbbjtidx[1]][0]<0.679)+2*(jtcsvtcorr[hbbjtidx[1]][0]>0.679 &&jtcsvtcorr[hbbjtidx[1]][0]<0.898)+3*(jtcsvtcorr[hbbjtidx[1]][0]>0.898);
	var27 = 0+(jtcsvtcorr[topjtidx][0]>0.244 &&jtcsvtcorr[topjtidx][0]<0.679)+2*(jtcsvtcorr[topjtidx][0]>0.679 &&jtcsvtcorr[topjtidx][0]<0.898)+3*(jtcsvtcorr[topjtidx][0]>0.898);
    
	//test=var27;
      }

	
	
    if(1==btagsys)
      {   
	var18  =  jtcsvtcorr[ljtidx][1];
	var19  =  jtcsvtcorr[hbbjtidx[0]][1];
	var20  =  jtcsvtcorr[hbbjtidx[1]][1];
	var21  =  jtcsvtcorr[topjtidx][1];
	var24  =  jtcsvtcorr[tophadjtidx[2]][1];
	var25  =  jtcsvtcorr[toplepjtidx][1];
	var26  =  jtcsvtcorr[tophadjtidx[0]][1];
	var27  =  jtcsvtcorr[tophadjtidx[1]][1];
      }
    if(2==btagsys)
      {   
	var18  =  jtcsvtcorr[ljtidx][2];
	var19  =  jtcsvtcorr[hbbjtidx[0]][2];
	var20  =  jtcsvtcorr[hbbjtidx[1]][2];
	var21  =  jtcsvtcorr[topjtidx][2];
	var24  =  jtcsvtcorr[tophadjtidx[2]][2];
	var25  =  jtcsvtcorr[toplepjtidx][2];
	var26  =  jtcsvtcorr[tophadjtidx[0]][2];
	var27  =  jtcsvtcorr[tophadjtidx[1]][2];
      }
    if(3==btagsys)
      {   
	var18  =  jtcsvtcorr[ljtidx][3];
	var19  =  jtcsvtcorr[hbbjtidx[0]][3];
	var20  =  jtcsvtcorr[hbbjtidx[1]][3];
	var21  =  jtcsvtcorr[topjtidx][3];
	var24  =  jtcsvtcorr[tophadjtidx[2]][3];
	var25  =  jtcsvtcorr[toplepjtidx][3];
	var26  =  jtcsvtcorr[tophadjtidx[0]][3];
	var27  =  jtcsvtcorr[tophadjtidx[1]][3];
      }
    if(4==btagsys)
      {   
	var18  =  jtcsvtcorr[ljtidx][4];
	var19  =  jtcsvtcorr[hbbjtidx[0]][4];
	var20  =  jtcsvtcorr[hbbjtidx[1]][4];
	var21  =  jtcsvtcorr[topjtidx][4];
	var24  =  jtcsvtcorr[tophadjtidx[2]][4];
	var25  =  jtcsvtcorr[toplepjtidx][4];
	var26  =  jtcsvtcorr[tophadjtidx[0]][4];
	var27  =  jtcsvtcorr[tophadjtidx[1]][4];
      }
      
    
    
    */
    //mlpout = reader_th->EvaluateMVA( "MLP method"  );

      mlpout = reader_th->EvaluateMVA( methodName  );

    //cout << mlpgout << endl;
    }
    otree->Fill();

  } // event loop
    
  cout << "End of event loop \n";
  
  otree->Write();
  ofile->Close();
  ifile->Close();
  cout << "Wrote file *.bdt.root \n";
  
  delete reader_th;
  delete ifile;
  delete ofile;
  
  string cmd_str = "mv -f " + opath+"/"+output + " " + opath+"/"+input;
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

