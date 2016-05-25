////////////////////////////////////////////////////////////////////////////////
//
// th_hypotheses_x
// -----------------
//
//            04/07/2013 Christian Boeser 
//
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TEventList.h>
#include <TLorentzVector.h>
#include <TBranch.h>
#include <TError.h>

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <map>


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
////////////////////////////////////////////////////////////////////////////////
Float_t Matchable(const TLorentzVector & gen_obj, const TLorentzVector & rec_obj);
static double DeltaR(const TLorentzVector & v1, const TLorentzVector & v2);
int count(int num, int* arr, int length);
void DumpObject(const TLorentzVector vec, string name = "variable");

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  //
  // evaluate command line options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs      = cl.getVector<string>("inputs");
  string         treename    = cl.getValue<string> ("treename",        "t");
  string         dirname     = cl.getValue<string> ("dirname",       "utm");
  bool           bestdR      = cl.getValue<bool>   ("bestdR",        false);
  bool           perfmatch   = cl.getValue<bool>   ("perfmatch",     false);


  if (!cl.check())
		return 0;
  cl.print();

  //Init random number generator
  srand(time(NULL));

  //
  // loop over all input files
  //

  for (unsigned int iinput = 0; iinput<inputs.size(); ++iinput) {
  
    // open input file
    string input = inputs[iinput]; 

    // read input file                                                                                                                                                                        
    TFile* ifile = TFile::Open((input).c_str(),"READ");
    if (!ifile->IsOpen()){
      cout << "\033[31m  > Failed to open '" << input << "'\033[0m" << endl;
      return 0;
    }else{
      cout << "Processing input file \033[33m'" << ifile->GetName() << "'\033[0m" << endl;
    }

    // read input directory
    TDirectory* idir = (TDirectory*)ifile->Get(dirname.c_str());
    if (0 == idir) {
      cout << "\033[31m  > Failed to access directory '" << dirname << "' in input file '" << input << ".root'\033[0m"\
      << endl;
      return 0;
    }



    // open input tree                                                                                                                                                                        
    TTree* itree = (TTree*)idir->Get(treename.c_str());
    if(0==itree){
      cout << "\033[31m  > No TTree named '" << treename << "' in '" << input << ":" << dirname << "'\033[0m" << endl;
      return 0;
    }

    //Get relevant information for creating hypothesis
    Int_t   i_njt=-99;               itree->SetBranchAddress("njt",            &i_njt);
    Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
    Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta_old",         i_jteta);
    Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
    Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);
    Float_t i_jtcsvt[40]={-99};      itree->SetBranchAddress("jtcsvt",       i_jtcsvt);

    Float_t i_top_tbpt={-99};         itree->SetBranchAddress("top_tbpt",    &i_top_tbpt);
    Float_t i_top_tbeta={-99};        itree->SetBranchAddress("top_tbeta",  &i_top_tbeta);
    Float_t i_top_tbphi={-99};        itree->SetBranchAddress("top_tbphi",  &i_top_tbphi);
    Float_t i_top_tbe={-99};          itree->SetBranchAddress("top_tbm",      &i_top_tbe);

    Float_t i_top_tbarbpt={-99};      itree->SetBranchAddress("top_tbarbpt",    &i_top_tbarbpt);
    Float_t i_top_tbarbeta={-99};     itree->SetBranchAddress("top_tbarbeta",  &i_top_tbarbeta);
    Float_t i_top_tbarbphi={-99};     itree->SetBranchAddress("top_tbarbphi",  &i_top_tbarbphi);
    Float_t i_top_tbarbe={-99};       itree->SetBranchAddress("top_tbarbm",      &i_top_tbarbe);

    Float_t i_top_twdaupt[2]={-99};   itree->SetBranchAddress("top_twdaupt",    i_top_twdaupt);
    Float_t i_top_twdaueta[2]={-99};  itree->SetBranchAddress("top_twdaueta",  i_top_twdaueta);
    Float_t i_top_twdauphi[2]={-99};  itree->SetBranchAddress("top_twdauphi",  i_top_twdauphi);
    Float_t i_top_twdaum[2]={-99};    itree->SetBranchAddress("top_twdaum",      i_top_twdaum);

    Float_t i_top_tbarwdaupt[2]={-99}; itree->SetBranchAddress("top_tbarwdaupt",    i_top_tbarwdaupt);
    Float_t i_top_tbarwdaueta[2]={-99};itree->SetBranchAddress("top_tbarwdaueta",  i_top_tbarwdaueta);
    Float_t i_top_tbarwdauphi[2]={-99};itree->SetBranchAddress("top_tbarwdauphi",  i_top_tbarwdauphi);
    Float_t i_top_tbarwdaum[2]={-99};  itree->SetBranchAddress("top_tbarwdaum",      i_top_tbarwdaum);

    Float_t i_top_lepcharge={-99};   itree->SetBranchAddress("top_lepcharge", &i_top_lepcharge);
    
    Int_t   i_nel={-99};             itree->SetBranchAddress("nel",            &i_nel);
    Int_t   i_nlel={-99};            itree->SetBranchAddress("nlel",          &i_nlel);
    Float_t i_elpt[10]={-99};        itree->SetBranchAddress("elpt",           i_elpt);
    Float_t i_eleta[10]={-99};       itree->SetBranchAddress("eleta",         i_eleta);
    Float_t i_elphi[10]={-99};       itree->SetBranchAddress("elphi",         i_elphi);
    Float_t i_ele[10]={-99};         itree->SetBranchAddress("ele",             i_ele);
    Float_t i_elcharge[10]={-99};         itree->SetBranchAddress("elcharge",             i_elcharge);

    Int_t   i_nmu={-99};             itree->SetBranchAddress("nmu",            &i_nmu);
    Int_t   i_nlmu={-99};            itree->SetBranchAddress("nlmu",          &i_nlmu); 
    Float_t i_mupt[10]={-99};        itree->SetBranchAddress("mupt",           i_mupt);
    Float_t i_mueta[10]={-99};       itree->SetBranchAddress("mueta",         i_mueta);
    Float_t i_muphi[10]={-99};       itree->SetBranchAddress("muphi",         i_muphi);
    Float_t i_mue[10]={-99};         itree->SetBranchAddress("mue",             i_mue);
    Float_t i_mucharge[10]={-99};       itree->SetBranchAddress("mucharge",         i_mucharge);

    Int_t   i_toplepjtidx={-99};        itree->SetBranchAddress("toplepjtidx",          &i_toplepjtidx);
    Int_t   i_tophadjtidx[3]={-99};    itree->SetBranchAddress("tophadjtidx",            i_tophadjtidx);
    

    int iSkippedEvents = 0;
    int iProcessedEvents = 0;

    unsigned int nEvt = (unsigned int)(itree->GetEntries());
    cout << "> \033[32m" << nEvt <<"\033[0m events will be processed"<<endl;
    cout << endl << " ";

    gErrorIgnoreLevel = kError;
    Int_t count_hyp = 0; // counter for all hypotheses
    //event loop

    int correct_tlep=0;
    int correct_thad=0;
    int correct_tt=0;
    int correct_wdau=0;
    int correct_w1dau=0;
    int correct_total=0;

    for (unsigned int iEvt = 0; iEvt < nEvt; ++iEvt) {
      //  > Output sign of life
      if (iEvt %250 == 0 || iEvt <= 10){
        cout << "\r  > Processing events from input tree:  (\033[33m" << iEvt << "/" << nEvt << "\033[0m)  -> \033[32m";
        cout << setprecision(4) << setiosflags(std::ios_base::left) << setfill('0') << setw(5)\
             << (iEvt*100.0)/nEvt << "\033[0m%    \b\b\b" << flush;
      }
      //'NULL' important variables (to give them a definite state)
      //Retrieve data from input Tree
      itree->GetEntry(iEvt);
      
      //Only reconstruct for n(isolated leptons)==1
     
      cout << "output !=  " << i_mucharge[0] << i_mue[0]<<i_mupt[0]<<i_elpt[0]<<i_top_tbarwdaum[0]<<i_tophadjtidx[0]<<endl;


      //Use Lorentz vectors to group variables and to avoid tapeworm names                                                         
      TLorentzVector p4jets[100];
      TLorentzVector p4topb, p4topbarb;
      TLorentzVector p4wdau1, p4wdau2;
      TLorentzVector p4lepwdau1, p4lepwdau2;

      //Set grouped variables for this event in the output tree
      int p4jetcounter = 0;
      for(int jets=0; jets<i_njt; ++jets){
	if(TMath::Abs(i_jtpt[jets]) > 40 || (TMath::Abs(i_jteta[jets])<2.4 && TMath::Abs(i_jtpt[jets]) > 30)){
	  p4jets[p4jetcounter].SetPtEtaPhiE(i_jtpt[jets], i_jteta[jets], i_jtphi[jets], i_jte[jets]);
	  p4jetcounter++;
	}
      }
      
      //Don't construct with less than 4 Jets
      if(p4jetcounter < 4) {
        iSkippedEvents++;
        continue;
      }

      //Distinct between electron and muon case to set generalized lepton variables
      bool has_wenu = (i_nel==1 && i_nlel==1 && i_nmu==0 && i_nlmu==0);
      bool has_wmunu = (i_nmu==1 && i_nlmu==1 && i_nel==0 && i_nlel==0);
      TLorentzVector p4lepton;
      Float_t lep_charge=0;
      
      
      if (has_wenu){
	p4lepton.SetPtEtaPhiE(i_elpt[0],i_eleta[0],i_elphi[0],i_ele[0]);
        lep_charge = i_elcharge[0];
      }else{
	p4lepton.SetPtEtaPhiE(i_mupt[0],i_mueta[0],i_muphi[0],i_mue[0]);
        lep_charge = i_mucharge[0];
      }
      
      if (!has_wenu &&!has_wmunu){
        iSkippedEvents++;
        cout << "\r \033[31m  > Warning - Neither an electron nor a muon was detected!\033[0m" << endl << " ";
	continue;
      }
      
      cout << "TEST " << i_top_twdaupt[1] << i_top_twdaueta[1] << i_top_twdauphi[1] << i_top_twdaum[1] << endl;
      //Fill GenInfo from Top and Anti-Top according to lepton charge                                                              
      
      if(lep_charge > 0 && lep_charge == i_top_lepcharge){
	p4topb.SetPtEtaPhiM(i_top_tbpt,i_top_tbeta,i_top_tbphi,i_top_tbe);
	p4topbarb.SetPtEtaPhiM(i_top_tbarbpt,i_top_tbarbeta,i_top_tbarbphi,i_top_tbarbe);
	p4wdau1.SetPtEtaPhiM(i_top_tbarwdaupt[0], i_top_tbarwdauphi[0], i_top_tbarwdaueta[0], i_top_tbarwdaum[0]);
	p4wdau2.SetPtEtaPhiM(i_top_tbarwdaupt[1], i_top_tbarwdauphi[1], i_top_tbarwdaueta[1], i_top_tbarwdaum[1]);
      }else if (lep_charge < 0 && lep_charge == i_top_lepcharge){
	p4topb.SetPtEtaPhiM(i_top_tbpt,i_top_tbeta,i_top_tbphi,i_top_tbe);
	p4topbarb.SetPtEtaPhiM(i_top_tbarbpt,i_top_tbarbeta,i_top_tbarbphi,i_top_tbarbe);
	p4wdau1.SetPtEtaPhiM(i_top_twdaupt[0], i_top_twdauphi[0], i_top_twdaueta[0], i_top_twdaum[0]);
	p4wdau2.SetPtEtaPhiM(i_top_twdaupt[1], i_top_twdaueta[1], i_top_twdauphi[1], i_top_twdaum[1]);
      }else{
        iSkippedEvents++;
        cout << "\r \033[31m  > Warning - Unexpected lepton charge, skipping this event!\033[0m" << endl << " ";                 
        continue;
      }
      
      /*
      DumpObject(p4topb,"p4topb");
      DumpObject(p4topbarb,"p4topbarb");
      DumpObject(p4wdau1,"p4wdau1");
      DumpObject(p4wdau2,"p4wdau2");
      */

      // Storing DeltaR and jet idxs with [topjt, higgsjt1, higgsjt2, lightjt]
      double best = numeric_limits<double>::infinity();
      vector<int> sig_i (4,-1);
      vector<int> bkg_i (4,-1);
      vector<vector<int> > bkg_config;
      Int_t best_i[4]; best_i[0] = -1; best_i[1] = -1; best_i[2] = -1; best_i[3] = -1;
      Int_t best_i_pm[4]; best_i_pm[0] = -1; best_i_pm[1] = -1; best_i_pm[2] = -1; best_i_pm[3] = -1;

      //Jet Indices

      // --
      // Check if event is matchable
      // --

      double bestdR_wdau1  = numeric_limits<double>::infinity();
      double bestdR_wdau2  = numeric_limits<double>::infinity();
      double bestdR_topb = numeric_limits<double>::infinity();
      double bestdR_topbarb = numeric_limits<double>::infinity();
      
      for(Int_t i=0; i<p4jetcounter; i++){
        if(i_jtcsvt[i]>0.89&&DeltaR(p4topb,p4jets[i])<bestdR_topb){
          bestdR_topb=DeltaR(p4topb,p4jets[i]);
          best_i_pm[0]=i;
        }
        if(i_jtcsvt[i]>0.89&&DeltaR(p4topbarb,p4jets[i])<bestdR_topbarb){
          bestdR_topbarb=DeltaR(p4topbarb,p4jets[i]);
          best_i_pm[1]=i;
        }
	if(DeltaR(p4wdau1,p4jets[i])<bestdR_wdau1){
          bestdR_wdau1=DeltaR(p4wdau1,p4jets[i]);
          best_i_pm[2]=i;
        }
	if(DeltaR(p4wdau2,p4jets[i])<bestdR_wdau2){
          bestdR_wdau2=DeltaR(p4wdau2,p4jets[i]);
          best_i_pm[3]=i;
        }
      }
      

      cout << "Best matched jets: " << best_i_pm[0]<< " " <<best_i_pm[1]<< " " <<best_i_pm[2]<< " " <<best_i_pm[3]<< " " << endl;
      cout << "Predicted  jets  : " << i_toplepjtidx << " " << i_tophadjtidx[2] << " " << i_tophadjtidx[0] << " " << i_tophadjtidx[1] << " " << endl;
     
      cout << "Best dRs: " << bestdR_topb<< "  " <<bestdR_topbarb<< "   " <<bestdR_wdau1<< "   " <<bestdR_wdau2<< "   " << endl;

      int correct_toplepbjets=0;
      int correct_tophadbjets=0;
      int correct_topbjets=0;
      int correct_wdaujets=0;

      if(lep_charge >0 ){ // topbar -> had

	if(best_i_pm[0]==i_toplepjtidx){
	  correct_toplepbjets++;
	} 
	if(best_i_pm[1]==i_tophadjtidx[2]){
	  correct_tophadbjets++;
	} 
      } 
      else{ // top -> had
	if(best_i_pm[1]==i_toplepjtidx){
	  correct_toplepbjets++;
	} 
	if(best_i_pm[0]==i_tophadjtidx[2]){
	  correct_tophadbjets++;
	}
      }

      
      if(best_i_pm[2]==i_tophadjtidx[1]||best_i_pm[2]==i_tophadjtidx[0]){
	correct_wdaujets++;
      }
      if(best_i_pm[3]==i_tophadjtidx[1]||best_i_pm[3]==i_tophadjtidx[0]){
	correct_wdaujets++;
      }
      
      cout << "correct_toplepbjets: " << correct_toplepbjets << "  correct_tophadbjets: " <<  correct_tophadbjets << "  correct_wdaujets: " << correct_wdaujets << endl;
      
      if(correct_toplepbjets==1) correct_tlep++;
      if(correct_tophadbjets==1) correct_thad++;
      if(correct_toplepbjets==1&&correct_tophadbjets==1) correct_tt++;
      if(correct_wdaujets>0) correct_w1dau++;
      if(correct_wdaujets==2) correct_wdau++;
      if(correct_wdaujets==2&&correct_toplepbjets==1&&correct_tophadbjets==1) correct_total++;
      
      
      
    }// for(iEvt < nEvt)
    
    cout << endl << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                   TopLep reconstruction efficiency: " << float(correct_tlep)/nEvt << endl;
    cout << "                   TopHad reconstruction efficiency: " << float(correct_thad)/nEvt << endl;
    cout << "               Wdaughters reconstruction efficiency: " << float(correct_wdau)/nEvt << endl;
    cout << "           One Wdaughters reconstruction efficiency: " << float(correct_w1dau)/nEvt << endl;
    cout << "                       TT reconstruction efficiency: " << float(correct_tt)/nEvt << endl;
    cout << "                    Total reconstruction efficiency: " << float(correct_total)/nEvt << endl;
    cout << "-----------------------------------------------------------------" << endl << endl;
    
    cout << "cat, eff" << endl;
    cout << "h," << float(correct_tlep)/nEvt<< endl;
    cout << "t," << float(correct_thad)/nEvt<<endl;
    cout << "l," << float(correct_wdau)/nEvt<<endl;
    cout << "th," << float(correct_tt)/nEvt<<endl;
    cout << "tot," << float(correct_total)/nEvt<<endl;
    
    ifile->Close();
    delete ifile;

    cout << "DONE!" << endl;
    
  }// for (ifile in inputs)

  return 0;
}//main(...)



////////////////////////////////////////////////////////////////////////////////
// helpers
////////////////////////////////////////////////////////////////////////////////

Float_t Matchable(const TLorentzVector & gen_obj, const TLorentzVector & rec_obj){
  Float_t DR = DeltaR(gen_obj,rec_obj);
  return DR;//+ptreltop+ptrelhiggs+ptrellight;
}

static double DeltaR(const TLorentzVector & v1, const TLorentzVector & v2) {
  Double_t DeltaR = 0.0;
  Double_t DeltaPhi = TMath::Abs(v1.Phi()-v2.Phi());
  if (DeltaPhi>TMath::Pi())
    DeltaPhi = 2*TMath::Pi() - DeltaPhi;
  Double_t DeltaEta = v1.Eta() - v2.Eta();
  DeltaR = sqrt(TMath::Power(DeltaPhi, 2) + TMath::Power(DeltaEta, 2));
  return DeltaR;
}

void DumpObject(const TLorentzVector vec, string name){
  cout << "Dumping " << name << " :" << endl;
  cout << "Pt: " << vec.Pt() << "   Eta: " << vec.Eta() << "   Phi: " << vec.Phi() << "   E: " << vec.E() << endl;
}
