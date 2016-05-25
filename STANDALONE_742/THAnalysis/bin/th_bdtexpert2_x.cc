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
  //bool           signal  = cl.getValue <bool>    ("signal",         false);
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
  Float_t var17=0.;
  Float_t var18=0., var19=0.;
  Float_t var20=0., var21=0.;
  Float_t var22=0., var23=0.;
  Float_t var24=0.;
  Float_t var25=0., var26=0.;
  Float_t var27=0., var28=0.;
  Float_t var29=0., var30=0.;
  Float_t var31=0.;


  /*
  reader_th->AddVariable( "popvar1",                                                  &var1 );
  reader_th->AddVariable( "popvar2",                                                   &var2 );
  reader_th->AddVariable( "popvar3",                                                   &var3 );
  reader_th->AddVariable( "popvar4",                                                   &var4 );
  reader_th->AddVariable( "popvar5",                                                   &var5 );
  reader_th->AddVariable( "popvar6",                                                   &var6 );
  reader_th->AddVariable( "popvar7",                                                   &var7 );
  reader_th->AddVariable( "popvar8",                                                   &var8 );
  reader_th->AddVariable( "popvar9",                                                   &var9 );
  reader_th->AddVariable( "popvar10",                                                   &var10 );
  reader_th->AddVariable( "popvar11",                                                   &var11 );
  // reader_th->AddVariable( "popvar12",                                                   &var12 );
  // reader_th->AddVariable( "popvar13",                                                   &var13 );
  reader_th->AddVariable( "popvar14",                                                   &var14 );
  reader_th->AddVariable( "popvar15",                                                   &var15 );
  reader_th->AddVariable( "popvar16",                                                   &var16 );
  reader_th->AddVariable( "popvar17",                                                   &var17 );
  */

  
  reader_th->AddVariable( "lepcharge:=popvar1",                      &var1 );
  reader_th->AddVariable( "log(SHat):=popvar2",                      &var2 );
  reader_th->AddVariable( "jtpt1:=popvar3",                             &var3 );
  reader_th->AddVariable( "log(met):=popvar4",                          &var4);
  reader_th->AddVariable( "HiggsPt:=popvar5",                          &var5 );
  reader_th->AddVariable( "RecoilPt:=popvar6",                          &var6 );
  reader_th->AddVariable( "CosLepRecoil:=popvar7",         &var7 );
  reader_th->AddVariable( "EtaRecoil:=popvar8",         &var8 );
  reader_th->AddVariable( "sphericity:=popvar9",         &var9 );
  reader_th->AddVariable( "MaxEtaLight:=popvar10",         &var10 );
  reader_th->AddVariable( "log(ttMassTopHad):=popvar11",         &var11 );
  reader_th->AddVariable( "log(DMassTopHadWHad):=popvar12",         &var12 );
  reader_th->AddVariable( "log(MaxMassBTopHadLight):=popvar13",         &var13 );
  //  $$      reader_th->AddVariable( "NumBTagHiggs:=popvar12",         &var12 );
  // $$       reader_th->AddVariable( "NumPassBTagLight:=popvar13",         &var13 );
  reader_th->AddVariable( "DeltaRBJetsHiggs:=popvar16",         &var16 );
  reader_th->AddVariable( "DeltaRLight:=popvar17",         &var17 );
  reader_th->AddVariable( "SumChargeLight:=popvar18",         &var18 );
  reader_th->AddVariable( "ChargeBTopHad:=popvar19",         &var19 );
  reader_th->AddVariable( "jtcsvt[ljtidx]*(jtcsvt[ljtidx]>0)+(jtcsvt[ljtidx]+998.90)*(jtcsvt[ljtidx]==-999)",         &var20 );
  reader_th->AddVariable( "jtcsvt[hbbjtidx[0]]*(jtcsvt[hbbjtidx[0]]>0)+(jtcsvt[hbbjtidx[0]]+998.90)*(jtcsvt[hbbjtidx[0]]==-999)",         &var21 );
  reader_th->AddVariable( "jtcsvt[hbbjtidx[1]]*(jtcsvt[hbbjtidx[1]]>0)+(jtcsvt[hbbjtidx[1]]+998.90)*(jtcsvt[hbbjtidx[1]]==-999)",         &var22 );
  reader_th->AddVariable( "jtcsvt[topjtidx]*(jtcsvt[topjtidx]>0)+(jtcsvt[topjtidx]+998.90)*(jtcsvt[topjtidx]==-999)",         &var23 );
  reader_th->AddVariable( "jtcsvt[tophadjtidx[0]]*(jtcsvt[tophadjtidx[0]]>0)+(jtcsvt[tophadjtidx[0]]+998.90)*(jtcsvt[tophadjtidx[0]]==-999)",         &var24 );
  reader_th->AddVariable( "jtcsvt[tophadjtidx[1]]*(jtcsvt[tophadjtidx[1]]>0)+(jtcsvt[tophadjtidx[1]]+998.90)*(jtcsvt[tophadjtidx[1]]==-999)",         &var25 );
  reader_th->AddVariable( "jtcsvt[tophadjtidx[2]]*(jtcsvt[tophadjtidx[2]]>0)+(jtcsvt[tophadjtidx[2]]+998.90)*(jtcsvt[tophadjtidx[2]]==-999)",         &var26 );
  reader_th->AddVariable( "jtcsvt[toplepjtidx]*(jtcsvt[toplepjtidx]>0)+(jtcsvt[toplepjtidx]+998.90)*(jtcsvt[toplepjtidx]==-999)",         &var27 );
  reader_th->AddVariable( "DeltaRWLep:=deltaR(lepweta,lepwphi,Alt$(eleta[0],0)+Alt$(mueta[0],0),Alt$(elphi[0],0)+Alt$(muphi[0],0))",         &var28 );
  reader_th->AddVariable( "jtpull[hbbjtidx[0]]-jtpull[hbbjtidx[1]]",         &var29 );
  



  //DO NOT REMOVE THIS

  Int_t spec1=0;
  Int_t spec2=0;
  Int_t spec3=0;
  Int_t spec4=0;
  Int_t spec5=0;
  Int_t spec6=0;
  Int_t spec7=0;
  Int_t spec8=0;

  reader_th->AddSpectator("topjtidx",       &spec1);
  reader_th->AddSpectator("ljtidx",         &spec2);
  reader_th->AddSpectator("hbbjtidx[0]",    &spec3);
  reader_th->AddSpectator("hbbjtidx[1]",    &spec4);
  reader_th->AddSpectator("toplepjtidx",    &spec5);
  reader_th->AddSpectator("tophadjtidx[0]", &spec6);
  reader_th->AddSpectator("tophadjtidx[1]", &spec7);
  reader_th->AddSpectator("tophadjtidx[2]", &spec8);



  /*
  reader_th->AddVariable( "0+(jtcsvtcorr[ljtidx][0]>0.244 &&jtcsvtcorr[ljtidx][0]<0.679)+2*(jtcsvtcorr[ljtidx][0]>0.679 &&jtcsvtcorr[ljtidx][0]<0.898)+3*(jtcsvtcorr[ljtidx][0]>0.898)",                                  &var24);
  reader_th->AddVariable( "0+(jtcsvtcorr[hbbjtidx[0]][0]>0.244 &&jtcsvtcorr[hbbjtidx[0]][0]<0.679)+2*(jtcsvtcorr[hbbjtidx[0]][0]>0.679 &&jtcsvtcorr[hbbjtidx[0]][0]<0.898)+3*(jtcsvtcorr[hbbjtidx[0]][0]>0.898)",                             &var25);
  reader_th->AddVariable( "0+(jtcsvtcorr[hbbjtidx[1]][0]>0.244 &&jtcsvtcorr[hbbjtidx[1]][0]<0.679)+2*(jtcsvtcorr[hbbjtidx[1]][0]>0.679 &&jtcsvtcorr[hbbjtidx[1]][0]<0.898)+3*(jtcsvtcorr[hbbjtidx[1]][0]>0.898)" ,                            &var26);
  reader_th->AddVariable( "0+(jtcsvtcorr[topjtidx][0]>0.244 &&jtcsvtcorr[topjtidx][0]<0.679)+2*(jtcsvtcorr[topjtidx][0]>0.679 &&jtcsvtcorr[topjtidx][0]<0.898)+3*(jtcsvtcorr[topjtidx][0]>0.898)",                                &var27);
  */





  string prefix = "TMVAClassification";
   
  // Book method(s)   WH110_BDTD.weights.xml
    
  //TString methodName = TString("BDT") + TString(" method");
  TString methodName2 = TString("BDTG") + TString(" method");
  //TString weightfile_th = TString(bdtpath) +"/"+ TString("tHnominal_BDT.weights.xml");
  TString weightfile2_th = TString(bdtpath) +"/"+ TString("tHnominal_BDTG.weights.xml");
 
  //reader_th->BookMVA( methodName, weightfile_th ); 
  reader_th->BookMVA( methodName2, weightfile2_th );

  
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
  // if (0!=itree->FindBranch("bdtdout")) itree->SetBranchStatus("bdtdout",0);
  if (0!=itree->FindBranch("bdtgout")) itree->SetBranchStatus("bdtgout",0);

  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree(0);
  cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;


  // create bdtout branches in output tree
  //Float_t  bdtdout=-99; otree->Branch("bdtdout",&bdtdout, "bdtdout/F");
  Float_t  bdtgout=-99; otree->Branch("bdtgout",&bdtgout, "bdtgout/F");
  //loat_t  test=-99; otree->Branch("test",&test, "test/F");
  //Get relevant information for creating hypothesis
 
  
  Int_t hbbjtidx[3];               itree->SetBranchAddress("hbbjtidx",       hbbjtidx);
  Int_t topjtidx;                  itree->SetBranchAddress("topjtidx",      &topjtidx);
  Int_t ljtidx;                    itree->SetBranchAddress("ljtidx",          &ljtidx);
  Int_t tophadjtidx[3];            itree->SetBranchAddress("tophadjtidx", tophadjtidx);
  Int_t toplepjtidx;               itree->SetBranchAddress("toplepjtidx",&toplepjtidx);


  Float_t popvar1={-99};             itree->SetBranchAddress("popvar1",            &popvar1);
  Float_t popvar2={-99};             itree->SetBranchAddress("popvar2",            &popvar2);
  Float_t popvar3={-99};             itree->SetBranchAddress("popvar3",            &popvar3);
  Float_t popvar4={-99};             itree->SetBranchAddress("popvar4",            &popvar4);
  Float_t popvar5={-99};             itree->SetBranchAddress("popvar5",            &popvar5);
  Float_t popvar6={-99};             itree->SetBranchAddress("popvar6",            &popvar6);
  Float_t popvar7={-99};             itree->SetBranchAddress("popvar7",            &popvar7);
  Float_t popvar8={-99};             itree->SetBranchAddress("popvar8",            &popvar8);
  Float_t popvar9={-99};             itree->SetBranchAddress("popvar9",            &popvar9);
  Float_t popvar10={-99};             itree->SetBranchAddress("popvar10",            &popvar10);
  Float_t popvar11={-99};             itree->SetBranchAddress("popvar11",            &popvar11);
  Float_t popvar12={-99};             itree->SetBranchAddress("popvar12",            &popvar12);
  Float_t popvar13={-99};             itree->SetBranchAddress("popvar13",            &popvar13);
  Float_t popvar14={-99};             itree->SetBranchAddress("popvar14",            &popvar14);
  Float_t popvar15={-99};             itree->SetBranchAddress("popvar15",            &popvar15);
  Float_t popvar16={-99};             itree->SetBranchAddress("popvar16",            &popvar16);
  Float_t popvar17={-99};             itree->SetBranchAddress("popvar17",            &popvar17);
  Float_t popvar18={-99};             itree->SetBranchAddress("popvar18",            &popvar18);
  Float_t popvar19={-99};             itree->SetBranchAddress("popvar19",            &popvar19);

  Float_t jtcsvt[99]={-99};             itree->SetBranchAddress("jtcsvt",       jtcsvt);
  Float_t met={-99};             itree->SetBranchAddress("met",                      &met);
  Float_t mucharge[60]={-99};             itree->SetBranchAddress("mucharge",     mucharge);
  Float_t elcharge[60]={-99};             itree->SetBranchAddress("elcharge",     elcharge);
  Float_t lepweta={-99};             itree->SetBranchAddress("lepweta",              &lepweta);
  Float_t lepwphi={-99};             itree->SetBranchAddress("lepwphi",              &lepwphi);
  Float_t eleta[60]={-99};             itree->SetBranchAddress("eleta",            eleta);
  Float_t elphi[60]={-99};             itree->SetBranchAddress("elphi",            elphi);
  Float_t mueta[60]={-99};             itree->SetBranchAddress("mueta",            mueta);
  Float_t muphi[60]={-99};             itree->SetBranchAddress("muphi",            muphi);
  Float_t jtpull[99]={-99};             itree->SetBranchAddress("jtpull",            jtpull);
  
  Int_t nmu=-99;                       itree->SetBranchAddress("nmu",           &nmu);

  /*
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
    
  
    spec1 = topjtidx;
    spec2 = ljtidx;
    spec3 = hbbjtidx[0];
    spec4 = hbbjtidx[1];
    spec5 = toplepjtidx;
    spec6 = tophadjtidx[0];
    spec7 = tophadjtidx[1];
    spec8 = tophadjtidx[2];

    
    var1   =    popvar1;
    var2   =    popvar2;
    var3   =    popvar3;
    var4   =    popvar4;
    var5   =    popvar5;
    var6   =    popvar6;
    var7   =    popvar7;
    var8   =    popvar8;
    var9   =    popvar9;
    var10   =    popvar10;
    var11   =    popvar11;
    var12   =    popvar12;
    var13   =    popvar13;
    //   var14   =    popvar14;
    //   var15   =    popvar15;
    var16   =    popvar16;
    var17   =    popvar17;
    var18   =    popvar18;
    var19   =    popvar19;

    var20  = jtcsvt[ljtidx]*(jtcsvt[ljtidx]>0)+(jtcsvt[ljtidx]+998.90)*(jtcsvt[ljtidx]==-999);
    var21 = jtcsvt[hbbjtidx[0]]*(jtcsvt[hbbjtidx[0]]>0)+(jtcsvt[hbbjtidx[0]]+998.90)*(jtcsvt[hbbjtidx[0]]==-999);
    var22 = jtcsvt[hbbjtidx[1]]*(jtcsvt[hbbjtidx[1]]>0)+(jtcsvt[hbbjtidx[1]]+998.90)*(jtcsvt[hbbjtidx[1]]==-999);
    var23 = jtcsvt[topjtidx]*(jtcsvt[topjtidx]>0)+(jtcsvt[topjtidx]+998.90)*(jtcsvt[topjtidx]==-999);
    var24 = jtcsvt[tophadjtidx[0]]*(jtcsvt[tophadjtidx[0]]>0)+(jtcsvt[tophadjtidx[0]]+998.90)*(jtcsvt[tophadjtidx[0]]==-999);
    var25 = jtcsvt[tophadjtidx[1]]*(jtcsvt[tophadjtidx[1]]>0)+(jtcsvt[tophadjtidx[1]]+998.90)*(jtcsvt[tophadjtidx[1]]==-999);
    var26 = jtcsvt[tophadjtidx[2]]*(jtcsvt[tophadjtidx[2]]>0)+(jtcsvt[tophadjtidx[2]]+998.90)*(jtcsvt[tophadjtidx[2]]==-999);
    var27 = jtcsvt[toplepjtidx]*(jtcsvt[toplepjtidx]>0)+(jtcsvt[toplepjtidx]+998.90)*(jtcsvt[toplepjtidx]==-999);
    
    
    if(nmu==1)  {
      var28 = deltaR(lepweta,lepwphi,mueta[0],muphi[0]);
    }     
    else {
      var28 = deltaR(lepweta,lepwphi,eleta[0],elphi[0]);
    }

    var29 = jtpull[hbbjtidx[0]]-jtpull[hbbjtidx[1]];



    // bdtdout = reader_th->EvaluateMVA( "BDTD method"  );

    bdtgout = reader_th->EvaluateMVA( "BDTG method"  );

    //cout << bdtgout << endl;

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

