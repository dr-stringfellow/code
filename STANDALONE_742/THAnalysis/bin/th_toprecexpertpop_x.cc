////////////////////////////////////////////////////////////////////////////////
//
//th_toprecexpertpop_x
// ------------
//
// tool to reconstuct a single top higgs event with  boosted decision trees (TMVA package) - requires nothin' but root
// fed with weight files from Andrey Popov
// 
//
//    ::21/01/2014 Simon Fink <sfink@kit.edu>::
//    ::10/11/2013 Benedikt Maier <bmaier@ekp.uni-karlsruhe.de>::
//
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
  std::cout << std::endl;
  std::cout << "==> Start TMVAClassificationApplication" << std::endl;
  
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  string         input     = cl.getValue <string>  ("input");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         ipath     = cl.getValue <string>  ("ipath",              ".");
  string         opath     = cl.getValue <string>  ("opath",              ".");
  bool           signal    = cl.getValue <bool>    ("signal",           false);
  string         bdtpath   = cl.getValue<string>   ("bdtpath"                );
  bool           takerandom = cl.getValue<bool>     ("takerandom",      false);

  
  if(!cl.check()) {};
  cl.print();
  
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
  Float_t var15=0.;
  
  /*Float_t var11=0., var12=0.;
  Float_t var13=0., var14=0.;
  Float_t var15=0., var16=0.;
  Float_t var17=0., var18=0.;


  Int_t spec1=0;
  Int_t spec2=0;
  Int_t spec3=0;
  Int_t spec4=0;

  reader_th->AddSpectator("tophadjtidx[0]", &spec1);
  reader_th->AddSpectator("tophadjtidx[1]", &spec2);
  reader_th->AddSpectator("tophadjtidx[2]", &spec3);
  reader_th->AddSpectator("toplepjtidx",    &spec4);

  */

  reader_th->AddVariable( "log(Mass_TopHad-Mass_WHad)",    &var1 );
  reader_th->AddVariable( "log(Mass_WHad)",                &var2 );
  reader_th->AddVariable( "log(Mass_BTopLepLep)",          &var3 );
  reader_th->AddVariable( "abs(Eta_TopHad)",               &var4 );
  //  reader_th->AddVariable( "PassBTag_BTopLep",              &var5 );
  //  reader_th->AddVariable( "PassBTag_BTopHad",              &var6 );
  reader_th->AddVariable( "NumBTag_Light",                 &var5 );
  reader_th->AddVariable( "DeltaR_Light",                  &var6 );
  reader_th->AddVariable( "DeltaR_BTopHadWHad" ,           &var7 );
  reader_th->AddVariable( "DeltaR_BTopLepWLep",            &var8);
  reader_th->AddVariable( "RelHt",                         &var9);
  reader_th->AddVariable( "log(Pt_TopLep)",                &var10);
  reader_th->AddVariable( "log(Pt_TopHad)",                &var11);
  reader_th->AddVariable( "Charge_BTopHad-Charge_BTopLep", &var12);
  reader_th->AddVariable( "SumCharge_Light",               &var13); 

  /*
  reader_th->AddVariable( "deltaR(jteta[tophadjtidx[2]],jtphi[tophadjtidx[2]],lepweta,lepwphi)",                                                          &var1 );
  reader_th->AddVariable( "deltaR(jteta[tophadjtidx[0]],jtphi[tophadjtidx[0]],jteta[tophadjtidx[1]],jtphi[tophadjtidx[1]])",                                            &var2 );
  reader_th->AddVariable( "log(tophadm)",       &var3 );
  reader_th->AddVariable( "log(toplepm)",                                                         &var4 );
  reader_th->AddVariable( "log(tophadwm)",                                           &var5 );
  reader_th->AddVariable( "max(abs(tophadeta),abs(toplepeta))",                                                          &var6 );
  reader_th->AddVariable( "log(min(jtpt[toplepjtidx],jtpt[tophadjtidx[2]]))",                                        &var7 );
  reader_th->AddVariable( "jtcsvt[tophadjtidx[2]]>0.679",  &var8 );
  reader_th->AddVariable( "jtcsvt[toplepjtidx]>0.679" ,                                                  &var9 );
  reader_th->AddVariable( "(topleppt+tophadpt)/sumHt", &var10);
  */

  string prefix = "TMVAClassification";
   
  // Book method(s)   
    
  TString methodName = TString("MLPBFGS") + TString(" method");
  TString weightfile_th = TString(bdtpath) +"/"+ TString("GlobalTTbarReco_BFGS_v3.weights.xml");

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
  if (0!=itree->FindBranch("top_best_recbdtout")) itree->SetBranchStatus("top_best_recbdtout",0);
  if (0!=itree->FindBranch("top_hyp_posbdt")) itree->SetBranchStatus("top_hyp_posbdt",0);
  if (0!=itree->FindBranch("top_hyp_posdR"))   itree->SetBranchStatus("top_hyp_posdR",0);
  if (0!=itree->FindBranch("tophadwm")) itree->SetBranchStatus("tophadwm",0); 
  if (0!=itree->FindBranch("tophadwpt")) itree->SetBranchStatus("tophadwpt",0);
  if (0!=itree->FindBranch("tophadwphi")) itree->SetBranchStatus("tophadwphi",0);
  if (0!=itree->FindBranch("tophadweta")) itree->SetBranchStatus("tophadweta",0);
  if (0!=itree->FindBranch("tophadm")) itree->SetBranchStatus("tophadm",0); 
  if (0!=itree->FindBranch("tophadpt")) itree->SetBranchStatus("tophadpt",0);
  if (0!=itree->FindBranch("tophadphi")) itree->SetBranchStatus("tophadphi",0);
  if (0!=itree->FindBranch("tophadeta")) itree->SetBranchStatus("tophadeta",0);
  if (0!=itree->FindBranch("tophaddr")) itree->SetBranchStatus("tophaddr",0); 
  if (0!=itree->FindBranch("tophadjtidx")) itree->SetBranchStatus("tophadjtidx",0);
  if (0!=itree->FindBranch("toplepm")) itree->SetBranchStatus("toplepm",0);   
  if (0!=itree->FindBranch("topleppt")) itree->SetBranchStatus("topleppt",0);  
  if (0!=itree->FindBranch("toplepphi")) itree->SetBranchStatus("toplepphi",0); 
  if (0!=itree->FindBranch("toplepeta")) itree->SetBranchStatus("toplepeta",0); 
  if (0!=itree->FindBranch("toplepjtidx")) itree->SetBranchStatus("toplepjtidx",0);



  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree(0);
  cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;

  if (0!=itree->FindBranch("tophadwm"))                  itree->SetBranchStatus("tophadwm",1); 
  if (0!=itree->FindBranch("tophadwpt"))                itree->SetBranchStatus("tophadwpt",1);
  if (0!=itree->FindBranch("tophadwphi"))              itree->SetBranchStatus("tophadwphi",1);
  if (0!=itree->FindBranch("tophadweta"))              itree->SetBranchStatus("tophadweta",1);
  if (0!=itree->FindBranch("tophadm"))                    itree->SetBranchStatus("tophadm",1); 
  if (0!=itree->FindBranch("tophadpt"))                  itree->SetBranchStatus("tophadpt",1);
  if (0!=itree->FindBranch("tophadphi"))                itree->SetBranchStatus("tophadphi",1);   
  if (0!=itree->FindBranch("tophadeta"))                itree->SetBranchStatus("tophadeta",1);  
  if (0!=itree->FindBranch("tophaddr"))                  itree->SetBranchStatus("tophaddr",1); 
  if (0!=itree->FindBranch("tophadjtidx"))            itree->SetBranchStatus("tophadjtidx",1);
  if (0!=itree->FindBranch("toplepm"))                    itree->SetBranchStatus("toplepm",1);
  if (0!=itree->FindBranch("topleppt"))                  itree->SetBranchStatus("topleppt",1); 
  if (0!=itree->FindBranch("toplephi"))                 itree->SetBranchStatus("toplepphi",1);
  if (0!=itree->FindBranch("toplepeta"))                itree->SetBranchStatus("toplepeta",1); 
  if (0!=itree->FindBranch("toplepjtidx"))            itree->SetBranchStatus("toplepjtidx",1);

  
  // create bdtout branches in output tree
  Float_t  top_best_recbdtout=-99; otree->Branch("top_best_recbdtout",&top_best_recbdtout, "top_best_recbdtout/F");
  Int_t  top_hyp_posdR=-99;  otree->Branch("top_hyp_posdR",&top_hyp_posdR, "top_hyp_posdR/I");
  Int_t  top_hyp_posbdt=-99;  otree->Branch("top_hyp_posbdt",&top_hyp_posbdt, "top_hyp_posbdt/I");

  Float_t tophadwm=-99;                otree->Branch("tophadwm",          &tophadwm,                     "tophadwm/F");
  Float_t tophadwpt=-99;               otree->Branch("tophadwpt",         &tophadwpt,                   "tophadwpt/F");
  Float_t tophadwphi=-99;              otree->Branch("tophadwphi",        &tophadwphi,                 "tophadwphi/F");
  Float_t tophadweta=-99;              otree->Branch("tophadweta",        &tophadweta,                 "tophadweta/F");
  Float_t tophadm=-99;                 otree->Branch("tophadm",          &tophadm,                     "tophadm/F");
  Float_t tophadpt=-99;                otree->Branch("tophadpt",         &tophadpt,                   "tophadpt/F");
  Float_t tophadphi=-99;               otree->Branch("tophadphi",        &tophadphi,                 "tophadphi/F");
  Float_t tophadeta=-99;               otree->Branch("tophadeta",        &tophadeta,                 "tophadeta/F");
  Float_t tophaddr=-99;                otree->Branch("tophaddr",         &tophaddr,                   "tophaddr/F");
  Int_t tophadjtidx[3]={-99,-99,-99};  otree->Branch("tophadjtidx",      tophadjtidx,           "tophadjtidx[3]/I");
  
  
  Float_t toplepm=-99;                otree->Branch("toplepm",          &toplepm,                     "toplepm/F");
  Float_t topleppt=-99;               otree->Branch("topleppt",         &topleppt,                   "topleppt/F");
  Float_t toplepphi=-99;              otree->Branch("toplepphi",        &toplepphi,                 "toplepphi/F");
  Float_t toplepeta=-99;              otree->Branch("toplepeta",        &toplepeta,                 "toplepeta/F");
  Int_t toplepjtidx=-99;              otree->Branch("toplepjtidx",      &toplepjtidx,             "toplepjtidx/I");
    
  
  
  //Get relevant information for creating hypothesis
  Int_t i_njt=-99;                 itree->SetBranchAddress("njt",            &i_njt);
  Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
  Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta",         i_jteta);
  Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
  Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);
  Float_t i_jtcsvt[40]={-99};      itree->SetBranchAddress("jtcsvt",       i_jtcsvt);
  Float_t i_jtcharge[40]={-99};    itree->SetBranchAddress("jtcharge",   i_jtcharge);
 
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
  //Gen Info

  Float_t i_top_tpt={-99};         
  Float_t i_top_teta={-99};        
  Float_t i_top_tphi={-99};        
  Float_t i_top_te={-99};          
  
  Float_t i_top_tbarpt={-99};      
  Float_t i_top_tbareta={-99};     
  Float_t i_top_tbarphi={-99};     
  Float_t i_top_tbare={-99};       
  
  Float_t i_top_twpt={-99};        
  Float_t i_top_tweta={-99};       
  Float_t i_top_twphi={-99};       
  Float_t i_top_twe={-99};         
  
  Float_t i_top_tbarwpt={-99};     
  Float_t i_top_tbarweta={-99};    
  Float_t i_top_tbarwphi={-99};    
  Float_t i_top_tbarwe={-99};      
  
  Float_t i_top_lepcharge={-99};   


  if(signal)
    {  
      itree->SetBranchAddress("top_tpt",             &i_top_tpt);
      itree->SetBranchAddress("top_teta",           &i_top_teta);
      itree->SetBranchAddress("top_tphi",           &i_top_tphi);
      itree->SetBranchAddress("top_te",               &i_top_te);
      itree->SetBranchAddress("top_tbarpt",       &i_top_tbarpt);
      itree->SetBranchAddress("top_tbareta",     &i_top_tbareta);
      itree->SetBranchAddress("top_tbarphi",     &i_top_tbarphi);
      itree->SetBranchAddress("top_tbare",         &i_top_tbare);
      itree->SetBranchAddress("top_twpt",           &i_top_twpt);
      itree->SetBranchAddress("top_tweta",         &i_top_tweta);
      itree->SetBranchAddress("top_twphi",         &i_top_twphi);
      itree->SetBranchAddress("top_twe",             &i_top_twe);
      itree->SetBranchAddress("top_tbarwpt",     &i_top_tbarwpt);
      itree->SetBranchAddress("top_tbarweta",   &i_top_tbarweta);
      itree->SetBranchAddress("top_tbarwphi",   &i_top_tbarwphi);
      itree->SetBranchAddress("top_tbarwe",       &i_top_tbarwe);
      itree->SetBranchAddress("top_lepcharge", &i_top_lepcharge);
    }

  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  THStatus* status = new THStatus(nevt);
  
  cout<<nevt<<" events will be processed"<<endl;
  //event loop
  for (unsigned ievt=0;ievt<nevt;ievt++) {
 
    itree->GetEntry(ievt);

    status->print_status(ievt);

    //Reconstruction - Hypotheses
    // Storing jet idxs with [topjt, higgsjt1, higgsjt2, lightjt]
    
    top_best_recbdtout = -99; //starting with a very small bdt output 
    float top_rndm_recbdtout = -99;
    Int_t best_i=-1; 
    Int_t best_j=-1;
    Int_t best_k=-1;
    Int_t best_l=-1;
    Int_t rndm_i=-1;
    Int_t rndm_j=-1;
    Int_t rndm_k=-1;
    Int_t rndm_l=-1;

    toplepjtidx    =-1;
    tophadjtidx[0] =-1;
    tophadjtidx[1] =-1;
    tophadjtidx[2] =-1;
    vector<float> vec_bdtout;
    vector<float> vec_deltaR;
    vector<int> vec_i;
    vector<int> vec_j;
    vector<int> vec_k;
    vector<int> vec_l;




    Int_t hyp_counter=0;
    
    if(i_nlepw == 1 && i_njt>=4 ){
      
      
      TLorentzVector p4jets[100];
      TLorentzVector p4toplep, p4tophad, p4whad;
      TLorentzVector p4lepw;

      for(int jets=0; jets<i_njt; jets++){
	TLorentzVector tmp;
        tmp.SetPtEtaPhiE(i_jtpt[jets],i_jteta[jets],i_jtphi[jets],i_jte[jets]);
	
	double newEta = i_jteta[jets];
        if(abs(i_jteta[jets]) > 2.4){
          int signEta = ((i_jteta[jets]> 0.) ? +1 : -1);


          if(abs(i_jteta[jets]) < 3.2)
            newEta = 2.8 * signEta;
          else
            newEta = 3.5 * signEta;
	}

	p4jets[jets].SetPtEtaPhiM(tmp.Pt(),newEta,tmp.Phi(),tmp.M());
      }
      p4lepw.SetPtEtaPhiM(i_lepwpt,i_lepweta,i_lepwphi,i_lepwm);
      bool has_wenu = (i_nel==1);
      TLorentzVector p4lepton;
      Float_t lep_charge=0;

      if (has_wenu)
	{
	  p4lepton.SetPtEtaPhiE(i_elpt[0],i_eleta[0],i_elphi[0],i_ele[0]);
	  lep_charge = i_elcharge[0];
	}
      else
	{
	  p4lepton.SetPtEtaPhiE(i_mupt[0],i_mueta[0],i_muphi[0],i_mue[0]);
	  lep_charge = i_mucharge[0];
	}
      
      if(signal){
	if(lep_charge > 0 && lep_charge==i_top_lepcharge){
	  p4toplep.SetPtEtaPhiE(i_top_tpt,i_top_teta,i_top_tphi,i_top_te);
	  p4tophad.SetPtEtaPhiE(i_top_tbarpt,i_top_tbareta,i_top_tbarphi,i_top_tbare);
	  p4whad.SetPtEtaPhiE(i_top_tbarwpt, i_top_tbarweta,i_top_tbarwphi,i_top_tbarwe);
	}
	else{
	  p4toplep.SetPtEtaPhiE(i_top_tbarpt,i_top_tbareta,i_top_tbarphi,i_top_tbare);
	  p4tophad.SetPtEtaPhiE(i_top_tpt,i_top_teta,i_top_tphi,i_top_te);
	  p4whad.SetPtEtaPhiE(i_top_twpt, i_top_tweta,i_top_twphi,i_top_twe);
	}	    
      }

      for (Int_t i=0; i<i_njt; i++){
	if( ! (i_jtcsvt[i]>0.898) && (TMath::Abs(p4jets[i].Eta())<2.4)){
          continue;
        }
	for (Int_t j=0; j<i_njt; j++){  // q1 from Whad      
	  if (j!=i){
	    for (Int_t k=0; k<i_njt; k++){  // q2 from Whad                                                                                                                                                                                
	      if ((k!=i )&&(k>j)) { 
		for (Int_t l=0; l<i_njt; l++){  // b from tophad                                                                                                                                                                           
		  if(l!=k && l!=j && l!=i) {
		    if( !((i_jtcsvt[i]>0.898) && (TMath::Abs(p4jets[i].Eta())<2.4) && (i_jtcsvt[l]>0.898) && (TMath::Abs(p4jets[l].Eta())<2.4 )))
		      {
			continue;
		      }
		    
		    hyp_counter++;
		  }}                                                                                                                                                                                                            
	      }}                                                                                                                                                                                                            
	  }}                                                                                                                                                                                                              
      }   
      
    
      if(hyp_counter!=0){

	TRandom3 random = 0;
	int randomhyp = random.Integer(hyp_counter-1);


	hyp_counter = 0;

	for (Int_t i=0; i<i_njt; i++){  // b from toplep
	  if( ! (i_jtcsvt[i]>0.898) && (TMath::Abs(p4jets[i].Eta())<2.4)){ 
	    continue;
	  }
	  TLorentzVector rectoplep = p4lepw + p4jets[i];
	
	  for (Int_t j=0; j<i_njt; j++){  // q1 from Whad
	    if (j!=i){
	      for (Int_t k=0; k<i_njt; k++){  // q2 from Whad
		if ((k!=i )&&(k>j)) {  // switching of q1 & q2 does not change Whad, so take only one
		  TLorentzVector recwhad = p4jets[j] + p4jets[k];	    
		  for (Int_t l=0; l<i_njt; l++){  // b from tophad
		    if(l!=k && l!=j && l!=i) {

		      // Skip hypothesis 
		      if( !((i_jtcsvt[i]>0.898) && (TMath::Abs(p4jets[i].Eta())<2.4) && (i_jtcsvt[l]>0.898) && (TMath::Abs(p4jets[l].Eta())<2.4 )))
			{ 
			  continue;
			}
		   
		      if(hyp_counter == randomhyp){
			rndm_i  =  i;
			rndm_j  =  j;
			rndm_k  =  k;
			rndm_l  =  l;
		      }

		  
		      TLorentzVector rectophad=p4jets[l]+recwhad;
		      TLorentzVector lep = p4lepton;
		  
		      var1   =  TMath::Log(rectophad.M()-recwhad.M()); //cout << "After 1" << endl;
		      //		  if (var1!=var1) cout << "Not A Number: " << p4jets[i].M() << " " << p4jets[j].M() << " " << p4jets[k].M() << " " << p4jets[l].M() << " " << endl;
		      var2   =  TMath::Log(recwhad.M()); //cout << "After 2" << endl;
		      var3   =  TMath::Log((p4jets[i]+lep).M()); //cout << "After 3" << endl;
		      var4   =  TMath::Abs(rectophad.Eta()); //cout << "After 4" << endl;
		      // var5   =  (i_jtcsvt[i]>0.898)*(TMath::Abs(i_jteta[i])<2.4); //cout << "After 5" << endl;
		      // var6   =  (i_jtcsvt[l]>0.898)*(TMath::Abs(i_jteta[l])<2.4); //cout << "After 6" << endl;
		      var5   =  (i_jtcsvt[j]>0.898)*(TMath::Abs(p4jets[j].Eta())<2.4)+(i_jtcsvt[k]>0.898)*(TMath::Abs(p4jets[k].Eta())<2.4); //cout << "After 7" << endl;
		      var6   =  deltaR(p4jets[j].Eta(), p4jets[j].Phi(),p4jets[k].Eta(), p4jets[k].Phi()); //cout << "After 8" << endl;
		      var7   =  deltaR(p4jets[l].Eta(),p4jets[l].Phi(),recwhad.Eta(),recwhad.Phi()); //cout << "After 9" << endl;
		      var8  =  deltaR(p4jets[i].Eta(),p4jets[i].Phi(),p4lepw.Eta(),p4lepw.Phi()); //cout << "After 10" << endl;
		      var9  =  (rectoplep.Pt()+rectophad.Pt())/(i_sumHt+lep.Pt()+i_met); //cout << "After 11 " << endl;
		      var10  =  TMath::Log(rectoplep.Pt()); //cout << "After 12" << endl;
		      var11  =  TMath::Log(rectophad.Pt()); //cout << "After 13" << endl;
		      var12  =  (i_jtcharge[l]-i_jtcharge[i])*lep_charge; //cout << "After 14" << endl; 
		      var13  =  (i_jtcharge[j]+i_jtcharge[k])*lep_charge; //cout << "After 15" << endl;
		  
		 
		      Float_t recbdtout=-99.;
		  		  
		      recbdtout = reader_th->EvaluateMVA( methodName  );   
		      vec_bdtout.push_back(recbdtout);
		      if(signal){
			Float_t DRtoplep = deltaR(rectoplep.Eta(),rectoplep.Phi(),p4toplep.Eta(),p4toplep.Phi()) + fabs(rectoplep.Pt() - p4toplep.Pt())/p4toplep.Pt();
			Float_t DRtophad = deltaR(rectophad.Eta(),rectophad.Phi(),p4tophad.Eta(),p4tophad.Phi()) + fabs(rectophad.Pt() - p4tophad.Pt())/p4tophad.Pt();
			Float_t DRwhad = deltaR(recwhad.Eta(),recwhad.Phi(),p4whad.Eta(),p4whad.Phi()) + fabs(recwhad.Pt() - p4whad.Pt())/p4whad.Pt();
			Float_t DeltaRval = DRtoplep + DRtophad + DRwhad;
			vec_deltaR.push_back(DeltaRval);
		      }
		      if(hyp_counter == randomhyp){
			top_rndm_recbdtout = recbdtout;
		      }
		      if(recbdtout > top_best_recbdtout)
			{
			  top_best_recbdtout  =  recbdtout;
			  best_i  =  i;
			  best_j  =  j;
			  best_k  =  k;
			  best_l  =  l;
			}

		      hyp_counter++;
		  
		  		  
		    }}//end light jet
		}}//end b2 from Higgs
	    }}//end b1 from Higgs
	}  // end b from top  
      
	// For double-check (has to be the same value):                  
	//      if(hyp_counter != resultfac){
	//  cout << hyp_counter << " != " << resultfac << endl;
	//  return 999;
	// }

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
	
	  top_hyp_posbdt = std::find(indices_dR.begin(), indices_dR.end(), indices_bdt.at(0)) - indices_dR.begin();
	  top_hyp_posdR = std::find(indices_bdt.begin(), indices_bdt.end(), indices_dR.at(0)) - indices_bdt.begin(); 
	}

	// !!! Take random as "best" !!                                  
	if(takerandom){
	  //Int_t n_rand = random.Integer(hyp_counter);
	  top_best_recbdtout = top_rndm_recbdtout;
	  best_i  =  rndm_i;
	  best_j  =  rndm_j;
	  best_k  =  rndm_k;
	  best_l  =  rndm_l;
	}

	//    cout << "best: " << best_recbdtout <<endl;
	TLorentzVector rectoplep  =  p4lepw + p4jets[best_i];
	TLorentzVector recwhad = p4jets[best_j] + p4jets[best_k]; 
	TLorentzVector rectophad =  recwhad + p4jets[best_l];

      
	toplepjtidx    = best_i;
	tophadjtidx[0] = best_j; // q1 from Whad
	tophadjtidx[1] = best_k; // q2 from Whad
	tophadjtidx[2] = best_l; // b from Whad
	tophadm     =    rectophad.M();
	tophadpt    =    rectophad.Pt();
	tophadphi   =    rectophad.Phi();
	tophadeta   =    rectophad.Eta();
	tophadwm    =    recwhad.M();
	tophadwpt   =    recwhad.Pt();
	tophadweta  =    recwhad.Eta();
	tophadwphi  =    recwhad.Phi();
	tophaddr    =    p4jets[best_j].DeltaR(p4jets[best_k]);
	toplepm     =    rectoplep.M();
	topleppt    =    rectoplep.Pt();
	toplepeta   =    rectoplep.Eta();
	toplepphi   =    rectoplep.Phi();
 
      }
    }    
    else{
      toplepjtidx    = -99;
      tophadjtidx[0] = -99;
      tophadjtidx[1] = -99;
      tophadjtidx[2] = -99;
      tophadm     =    -99;
      tophadpt    =    -99;
      tophadphi   =    -99;
      tophadeta   =    -99;
      tophadwm    =    -99;
      tophadwpt   =    -99;
      tophadweta  =    -99;
      tophadwphi  =    -99;
      tophaddr    =    -99;
      toplepm     =    -99;
      topleppt    =    -99;
      toplepeta   =    -99;
      toplepphi   =    -99;
      top_best_recbdtout = -99;
    }
    /* b_hbbm->Fill();
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
    b_best_recbdtout->Fill();
    */
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

