////////////////////////////////////////////////////////////////////////////////
//
//th_recexpert_x
// ------------
//
// tool to reconstuct a single top higgs event with  boosted decision trees (TMVA package) - requires nothin' but root
//
// 
//
//    
//    ::05/14/2012 Benedikt Maier <bmaier@ekp.uni-karlsruhe.de>::
////////////////////////////////////////////////////////////////////////////////

//#include "SchieferD/AnaUtils/interface/MCSample.h"

#include "../interface/CommandLine.h"
#include "../interface/Math.h"

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
  bool           signal  = cl.getValue <bool>    ("signal",         false);
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
  Float_t var11=0.;

  /*Float_t var11=0., var12=0.;
  Float_t var13=0., var14=0.;
  Float_t var15=0., var16=0.;
  Float_t var17=0., var18=0.;
  */

  Int_t spec1=0;
  Int_t spec2=0;
  Int_t spec3=0;
  Int_t spec4=0;

  reader_th->AddSpectator("topjtidx", &spec1);
  reader_th->AddSpectator("ljtidx", &spec2);
  reader_th->AddSpectator("hbbjtidx[0]", &spec3);
  reader_th->AddSpectator("hbbjtidx[1]", &spec4);
  reader_th->AddVariable( "log(hbbm)",                                                          &var1 );
  reader_th->AddVariable( "abs(jteta[ljtidx])",                                            &var2 );
  reader_th->AddVariable( "(jtcsvt[hbbjtidx[0]]>0.679)+(jtcsvt[hbbjtidx[1]]>0.679)",       &var3 );
  reader_th->AddVariable( "hbbdr",                                                         &var4 );
  reader_th->AddVariable( "(toppt+hbbpt)/sumHt",                                           &var5 );
  reader_th->AddVariable( "log(topm)",                                                          &var6 );
  reader_th->AddVariable( "jtcsvt[topjtidx]>0.679",                                        &var7 );
  reader_th->AddVariable( "log(min(min(jtpt[hbbjtidx[0]],jtpt[hbbjtidx[1]]),jtpt[topjtidx]))",  &var8 );
  reader_th->AddVariable( "abs(hbbeta)" ,                                                  &var9 );
  reader_th->AddVariable( "deltaR(jteta[ljtidx],jtphi[ljtidx],jteta[topjtidx],jtphi[topjtidx])" ,                                                  &var10 );
  reader_th->AddVariable( "deltaleptop:=(nel==1 && nmu==0)*(Alt$(eleta[0],0)-topeta)+(nmu==1 && nel==0)*(Alt$(mueta[0],0)-topeta)", &var11);
  

  string prefix = "TMVAClassification";
   
  // Book method(s)   WH110_BDTD.weights.xml
    
  TString methodName = TString("BDT") + TString(" method");
  TString weightfile_th = TString(bdtpath) +"/"+ TString("BDTreco_expert_BDT.weights.xml");

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
  
  // don't copy bdtout branch if it already exists
  if (0!=itree->FindBranch("best_recbdtout")) itree->SetBranchStatus("best_recbdtout",0);
  if (0!=itree->FindBranch("hyp_posbdt")) itree->SetBranchStatus("hyp_posbdt",0);
    if (0!=itree->FindBranch("hyp_posdR")) itree->SetBranchStatus("hyp_posdR",0);
  if (0!=itree->FindBranch("hbbm"))                     itree->SetBranchStatus("hbbm",0); 
  if (0!=itree->FindBranch("hbbpt"))                   itree->SetBranchStatus("hbbpt",0);
  if (0!=itree->FindBranch("hbbphi"))                 itree->SetBranchStatus("hbbphi",0);
  if (0!=itree->FindBranch("hbbeta"))                 itree->SetBranchStatus("hbbeta",0);
  if (0!=itree->FindBranch("hbbdr"))                   itree->SetBranchStatus("hbbdr",0); 
  if (0!=itree->FindBranch("hbbjtidx"))             itree->SetBranchStatus("hbbjtidx",0);
  if (0!=itree->FindBranch("topm"))                     itree->SetBranchStatus("topm",0);   
  if (0!=itree->FindBranch("toppt"))                   itree->SetBranchStatus("toppt",0);  
  if (0!=itree->FindBranch("topphi"))                 itree->SetBranchStatus("topphi",0); 
  if (0!=itree->FindBranch("topeta"))                 itree->SetBranchStatus("topeta",0); 
  if (0!=itree->FindBranch("topjtidx"))             itree->SetBranchStatus("topjtidx",0);
  if (0!=itree->FindBranch("ljtidx"))                 itree->SetBranchStatus("ljtidx",0);
  if (0!=itree->FindBranch("costwlt2_rec"))     itree->SetBranchStatus("costwlt2_rec",0);  

  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree(0);
  cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;

  if (0!=itree->FindBranch("hbbm"))                  itree->SetBranchStatus("hbbm",1); 
  if (0!=itree->FindBranch("hbbpt"))                itree->SetBranchStatus("hbbpt",1);
  if (0!=itree->FindBranch("hbbphi"))              itree->SetBranchStatus("hbbphi",1);
  if (0!=itree->FindBranch("hbbeta"))              itree->SetBranchStatus("hbbeta",1);
  if (0!=itree->FindBranch("hbbdr"))                itree->SetBranchStatus("hbbdr",1); 
  if (0!=itree->FindBranch("hbbjtidx"))          itree->SetBranchStatus("hbbjtidx",1);
  if (0!=itree->FindBranch("topm"))                  itree->SetBranchStatus("topm",1);   
  if (0!=itree->FindBranch("toppt"))                itree->SetBranchStatus("toppt",1);  
  if (0!=itree->FindBranch("topphi"))              itree->SetBranchStatus("topphi",1); 
  if (0!=itree->FindBranch("topeta"))              itree->SetBranchStatus("topeta",1); 
  if (0!=itree->FindBranch("topjtidx"))          itree->SetBranchStatus("topjtidx",1);
  if (0!=itree->FindBranch("ljtidx"))              itree->SetBranchStatus("ljtidx",1);
  if (0!=itree->FindBranch("costwlt2_rec"))  itree->SetBranchStatus("costwlt2_rec",1);  

  // create bdtout branches in output tree
  Float_t  best_recbdtout=-99; otree->Branch("best_recbdtout",&best_recbdtout, "best_recbdtout/F");
  Int_t  hyp_posdR=-99;  otree->Branch("hyp_posdR",&hyp_posdR, "hyp_posdR/I");
  Int_t  hyp_posbdt=-99;  otree->Branch("hyp_posbdt",&hyp_posbdt, "hyp_posbdt/I");

  Float_t hbbm=-99;                 otree->Branch("hbbm",          &hbbm,                     "hbbm/F");
  Float_t hbbpt=-99;                otree->Branch("hbbpt",         &hbbpt,                   "hbbpt/F");
  Float_t hbbphi=-99;               otree->Branch("hbbphi",        &hbbphi,                 "hbbphi/F");
  Float_t hbbeta=-99;               otree->Branch("hbbeta",        &hbbeta,                 "hbbeta/F");
  Float_t hbbdr=-99;                otree->Branch("hbbdr",         &hbbdr,                   "hbbdr/F");
  Int_t hbbjtidx[3]={-99,-99,-99};  otree->Branch("hbbjtidx",      hbbjtidx,           "hbbjtidx[3]/I");
  Float_t topm=-99;                 otree->Branch("topm",          &topm,                     "topm/F");
  Float_t toppt=-99;                otree->Branch("toppt",         &toppt,                   "toppt/F");
  Float_t topphi=-99;               otree->Branch("topphi",        &topphi,                 "topphi/F");
  Float_t topeta=-99;               otree->Branch("topeta",        &topeta,                 "topeta/F");
  Int_t topjtidx=-99;               otree->Branch("topjtidx",      &topjtidx,             "topjtidx/I");
  Int_t ljtidx=-99;                 otree->Branch("ljtidx",        &ljtidx,                 "ljtidx/I");
  Float_t costwlt2_rec=-99;         otree->Branch("costwlt2_rec",  &costwlt2_rec,     "costwlt2_rec/F");
  
  //Get relevant information for creating hypothesis
  Int_t i_njt=-99;                 itree->SetBranchAddress("njt",            &i_njt);
  Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
  Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta",         i_jteta);
  Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
  Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);
  Float_t i_jtcsvt[40]={-99};      itree->SetBranchAddress("jtcsvt",       i_jtcsvt);
  
  UChar_t i_nlepw={-99};           itree->SetBranchAddress("nlepw",        &i_nlepw);
  Float_t i_lepwm={-99};           itree->SetBranchAddress("lepwm",        &i_lepwm);
  Float_t i_lepwpt={-99};          itree->SetBranchAddress("lepwpt",      &i_lepwpt);
  Float_t i_lepwphi={-99};         itree->SetBranchAddress("lepwphi",    &i_lepwphi);
  Float_t i_lepweta={-99};         itree->SetBranchAddress("lepweta",    &i_lepweta);
  
  Int_t i_nel={-99};               itree->SetBranchAddress("nel",            &i_nel);
  Float_t i_elpt[10]={-99};        itree->SetBranchAddress("elpt",           i_elpt);
  Float_t i_eleta[10]={-99};       itree->SetBranchAddress("eleta",         i_eleta);
  Float_t i_elphi[10]={-99};       itree->SetBranchAddress("elphi",         i_elphi);
  Float_t i_ele[10]={-99};         itree->SetBranchAddress("ele",             i_ele);
    
  Int_t i_nmu={-99};               itree->SetBranchAddress("nmu",            &i_nmu);
  Float_t i_mupt[10]={-99};        itree->SetBranchAddress("mupt",           i_mupt);
  Float_t i_mueta[10]={-99};       itree->SetBranchAddress("mueta",         i_mueta);
  Float_t i_muphi[10]={-99};       itree->SetBranchAddress("muphi",         i_muphi);
  Float_t i_mue[10]={-99};         itree->SetBranchAddress("mue",             i_mue);
  
  Float_t i_sumHt={-99};           itree->SetBranchAddress("sumHt",        &i_sumHt);

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
  cout<<nevt<<" events will be processed"<<endl;
  //event loop
  for (unsigned ievt=0;ievt<nevt;ievt++) {
 
    // if (ievt%1000 == 0 || ievt <= 10) 
    //  std::cout << "--- ... Processing event: " << ievt << std::endl;
 
    int test=int(nevt/20);
    if(test==0)test=1;
    if(ievt%test==0){
      ostringstream stream;
      stream << "\r[" ;
      for(int i=0;i<ievt/test;i++){
	if(i>20) break;
	stream << "%";
      }

      for(int i=0;i<(20-(ievt/test));i++){
	if(20-(ievt/test)>20) break;
	stream << "-" ;
      }
      stream <<  "] ";
      stream << ievt/test*5;
      stream << "% completed.";
      cout << stream.str() << flush;
      //std::cout.flush();
    }
   
    itree->GetEntry(ievt);

    //Reconstruction - Hypotheses
    // Storing jet idxs with [topjt, higgsjt1, higgsjt2, lightjt]
    
    best_recbdtout = -99; //starting with a very small bdt output 
    Int_t best_i=-1; 
    Int_t best_j=-1;
    Int_t best_k=-1;
    Int_t best_l=-1;
    ljtidx=-1;
    topjtidx=-1;
    hbbjtidx[0]=-1;
    hbbjtidx[1]=-1;
    vector<float> vec_bdtout;
    vector<float> vec_deltaR;
    Int_t hyp_counter=0;

    if(i_nlepw == 1 && i_njt>=4 ){
      
      
      TLorentzVector p4jets[100];
      TLorentzVector p4top, p4higgs, p4light;
      TLorentzVector p4lepw;
      for(int jets=0; jets<i_njt; jets++){
	p4jets[jets].SetPtEtaPhiE(i_jtpt[jets],i_jteta[jets],i_jtphi[jets],i_jte[jets]);
      }
      p4lepw.SetPtEtaPhiM(i_lepwpt,i_lepweta,i_lepwphi,i_lepwm);
      
      bool has_wenu = (i_nel==1);
      TLorentzVector p4lepton;
      
      if (has_wenu)
	p4lepton.SetPtEtaPhiE(i_elpt[0],i_eleta[0],i_elphi[0],i_ele[0]);
      else
	p4lepton.SetPtEtaPhiE(i_mupt[0],i_mueta[0],i_muphi[0],i_mue[0]);

      if(signal){
	p4higgs.SetPtEtaPhiM(i_Hpt,i_Heta,i_Hphi,i_Hm);
	p4top.SetPtEtaPhiM(i_tpt,i_teta,i_tphi,i_tm);
	p4light.SetPtEtaPhiM(i_lqpt, i_lqeta,i_lqphi,i_lqm);
      }

      for (Int_t i=0; i<i_njt; i++){  // b from top
	TLorentzVector rectop = p4lepw + p4jets[i];
	for (Int_t j=0; j<i_njt; j++){  // b1 from Higgs
	  if (j!=i){
	  for (Int_t k=0; k<i_njt; k++){  // b2 from Higgs
	    if ((k!=i )&&(k>j)) {  // switching of b1 & b2 does not change Higgs, so take only one
	      TLorentzVector rechiggs = p4jets[j] + p4jets[k];	    
	      for (Int_t l=0; l<i_njt; l++){  // light jet
		if(l!=k && l!=j && l!=i) {
		  hyp_counter++;
		  TLorentzVector reclight=p4jets[l];		    
		  //Fixme:
		  TLorentzVector lep = p4lepton;
		  TLorentzVector boost_wcm = p4lepw;
		  TVector3 boost3_wcm = boost_wcm.BoostVector();
		  TLorentzVector top_helper = rectop;
		  Float_t cos_tmp=-99;
		  lep.Boost(-boost3_wcm);		    
		  top_helper.Boost(-boost3_wcm);
		  cos_tmp = lep.Vect()*top_helper.Vect()/(lep.Vect().Mag()*top_helper.Vect().Mag());

		  var1   =  TMath::Log(rechiggs.M());
		  var2   =  TMath::Abs(i_jteta[l]);
		  var3   =  (i_jtcsvt[j]>0.679)+(i_jtcsvt[k]>0.679);
		  //var4   =  (i_jtcsvt[l]>0.679);
	
		  var4   =  p4jets[j].DeltaR(p4jets[k]);
		  var5   =  (rechiggs.Pt()+rectop.Pt())/i_sumHt;
		  var6   =  TMath::Log(rectop.M());
		  var7   =  (i_jtcsvt[i]>0.679);
		  var8   =  TMath::Log(TMath::Min(TMath::Min(i_jtpt[j],i_jtpt[k]),i_jtpt[i]));
		  var9   =  TMath::Abs(rechiggs.Eta());
		  //var10  =  deltaR(i_jteta[i],i_jtphi[i],i_lepweta,i_lepwphi);
		  //var11  =  TMath::Abs(rectop.Eta());
		  var10  =  deltaR(reclight.Eta(),reclight.Phi(),i_jteta[i],i_jtphi[i]);
		  var11  =  (has_wenu) ? (i_eleta[0]-rectop.Eta()):(i_mueta[0]-rectop.Eta());

		  Float_t recbdtout=-99.;
		  recbdtout = reader_th->EvaluateMVA( "BDT method"  );   
		  vec_bdtout.push_back(recbdtout);
		  if(signal){
		    Float_t DRtop = deltaR(rectop.Eta(),rectop.Phi(),p4top.Eta(),p4top.Phi()) + fabs(rectop.Pt() - p4top.Pt())/p4top.Pt();
                    Float_t DRhiggs = deltaR(rechiggs.Eta(),rechiggs.Phi(),p4higgs.Eta(),p4higgs.Phi()) + fabs(rechiggs.Pt() - p4higgs.Pt())/p4higgs.Pt();
                    Float_t DRlight = deltaR(reclight.Eta(),reclight.Phi(),p4light.Eta(),p4light.Phi()) + fabs(reclight.Pt() - p4light.Pt())/p4light.Pt();
                    Float_t DeltaRval = DRtop + DRhiggs + DRlight;
		    vec_deltaR.push_back(DeltaRval);
		  }
		  
		  if(recbdtout > best_recbdtout)
		    {
		      best_recbdtout  =  recbdtout;
		      best_i  =  i;
		      best_j  =  j;
		      best_k  =  k;
		      best_l  =  l;
		    }
		}}//end light jet
	    }}//end b2 from Higgs
	  }}//end b1 from Higgs
      }  // end b from top  
      
      // Fill in which position max(BDT) and min(deltaR) are
      if(signal){
	vec_bdtout.resize(hyp_counter);
	vec_deltaR.resize(hyp_counter);
	vector<size_t> indices_bdt;
	vector<size_t> indices_dR;
	indices_bdt.resize(hyp_counter);
	indices_dR.resize(hyp_counter);
	for(Int_t i=0; i<hyp_counter; ++i){
	  indices_bdt[i] = i;
	  indices_dR[i] = i;
	}
	
	sort(indices_bdt.begin(),indices_bdt.end(),GreaterByVal(vec_bdtout));
	sort(indices_dR.begin(),indices_dR.end(),SmallerByVal(vec_deltaR));
	
	hyp_posbdt = std::find(indices_dR.begin(), indices_dR.end(), indices_bdt.at(0)) - indices_dR.begin();
	hyp_posdR = std::find(indices_bdt.begin(), indices_bdt.end(), indices_dR.at(0)) - indices_bdt.begin(); 
      }

      
      //    cout << "best: " << best_recbdtout <<endl;
      TLorentzVector rectop    =  p4lepw + p4jets[best_i];
      TLorentzVector rechiggs  =  p4jets[best_j] + p4jets[best_k];
      TLorentzVector reclight  =  p4jets[best_l];
      
      ljtidx      =    best_l;
      hbbm        =    rechiggs.M();
      hbbpt       =    rechiggs.Pt();
      hbbphi      =    rechiggs.Phi();
      hbbeta      =    rechiggs.Eta();
      hbbdr       =    p4jets[best_j].DeltaR(p4jets[best_k]);
      hbbjtidx[0] =    best_j;
      hbbjtidx[1] =    best_k;
      topm        =    rectop.M();
      toppt       =    rectop.Pt();
      topphi      =    rectop.Phi();
      topeta      =    rectop.Eta();
      topjtidx    =    best_i;
      
      TLorentzVector lep = p4lepton;
      TLorentzVector boost_wcm = p4lepw;
      TVector3 boost3_wcm = boost_wcm.BoostVector();
      TLorentzVector top_helper = rectop;
    
      lep = p4lepton;
      lep.Boost(-boost3_wcm);		    
      top_helper.Boost(-boost3_wcm);
      costwlt2_rec = lep.Vect()*top_helper.Vect()/(lep.Vect().Mag()*top_helper.Vect().Mag());
      
    }    
    else{
      ljtidx      =  -99;
      hbbm        =   -99;
      hbbpt       =   -99;
      hbbphi      =   -99; 
      hbbeta      =   -99;
      hbbdr       =   -99;
      hbbjtidx[0] =   -99;
      hbbjtidx[1] =   -99;
      topm        =   -99;
      toppt       =   -99;
      topphi      =   -99;
      topeta      =   -99;
      topjtidx    =   -99;
      costwlt2_rec =  -99;
      best_recbdtout = -99;
    }
    /*b_hbbm->Fill();
    b_hbbpt->Fill();
    b_hbbphi->Fill();
    b_hbbeta->Fill();
    b_hbbdr->Fill();
    b_hbbjtidx->Fill();
    b_topm->Fill();
    b_toppt->Fill();
    b_topphi->Fill();
    b_topeta->Fill();
    b_topjtidx->Fill();
    b_ljtidx->Fill();
    b_costwlt2_rec->Fill();
    b_best_recbdtout->Fill();*/
    
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
  
  string cmd_str = "mv " + opath+"/"+output + " " + opath+"/"+input;
  gSystem->Exec(cmd_str.c_str());
  
  cout << "Override original file... !" << endl;
  cout << "==> TMVAClassificationApplication is done! Hell Yeah!" << endl;

  return 0;

  //  std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;
    
 
}


