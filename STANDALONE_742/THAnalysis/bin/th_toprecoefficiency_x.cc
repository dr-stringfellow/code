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

    Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
    Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta",         i_jteta);
    Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
    Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);

    Float_t i_top_twdaupt[10]={-99}; itree->SetBranchAddress("top_twdaupt",    i_top_twdaupt);
    Float_t i_top_twdaueta[10]={-99};itree->SetBranchAddress("top_twdaueta",  i_top_twdaueta);
    Float_t i_top_twdauphi[10]={-99};itree->SetBranchAddress("top_twdauphi",  i_top_twdauphi);
    Float_t i_top_twdaue[10]={-99};  itree->SetBranchAddress("top_twdaue",      i_top_twdaue);

    Float_t i_top_tbarwdaupt[10]={-99}; itree->SetBranchAddress("top_tbarwdaupt",    i_top_tbarwdaupt);
    Float_t i_top_tbarwdaueta[10]={-99};itree->SetBranchAddress("top_tbarwdaueta",  i_top_tbarwdaueta);
    Float_t i_top_tbarwdauphi[10]={-99};itree->SetBranchAddress("top_tbarwdauphi",  i_top_tbarwdauphi);
    Float_t i_top_tbarwdaue[10]={-99};  itree->SetBranchAddress("top_tbarwdaue",      i_top_tbarwdaue);

    Float_t i_top_lepcharge={-99};   itree->SetBranchAddress("top_lepcharge", &i_top_lepcharge);
    Int_t   i_tophadjtidx[3]={-99};    itree->SetBranchAddress("hbbjtidx",            i_tophadjtidx);
    Int_t   i_nel={-99};             itree->SetBranchAddress("nel",            &i_nel);

    
    Int_t   i_nmu={-99};             itree->SetBranchAddress("nmu",            &i_nmu);


    Float_t i_elcharge[10]={-99};         itree->SetBranchAddress("elcharge",             i_elcharge);
    Float_t i_mucharge[10]={-99};         itree->SetBranchAddress("mucharge",             i_mucharge);

    unsigned int nEvt = (unsigned int)(itree->GetEntries());
    cout << "> \033[32m" << nEvt <<"\033[0m events will be processed"<<endl;
    cout << endl << " ";



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



      bool has_wenu = (i_nel==1 && i_nmu==0 );                                                                                                                                      
      bool has_wmunu = (i_nmu==1 && i_nel==0);
     
      Float_t lep_charge=0;


      if (has_wenu){
	lep_charge = i_elcharge[0];
	cout << "here "<< i_elcharge[0] <<endl;
	
      }
      else
        lep_charge = i_mucharge[0];
      

      cout << "lepton charge: " << lep_charge << endl;

      //Use Lorentz vectors to group variables and to avoid tapeworm names                                                         
      TLorentzVector p4wdaujet1, p4wdaujet2;
      TLorentzVector p4wdau1, p4wdau2;
      
      //Set grouped variables for this event in the output tree
      p4wdaujet1.SetPtEtaPhiE(i_jtpt[i_tophadjtidx[1]], i_jteta[i_tophadjtidx[1]], i_jtphi[i_tophadjtidx[1]], i_jte[i_tophadjtidx[1]]);
      p4wdaujet2.SetPtEtaPhiE(i_jtpt[i_tophadjtidx[2]], i_jteta[i_tophadjtidx[2]], i_jtphi[i_tophadjtidx[2]], i_jte[i_tophadjtidx[2]]);
      
      //Fill GenInfo from Top and Anti-Top according to lepton charge                                                              
      if(lep_charge > 0 && lep_charge == i_top_lepcharge){
	p4wdau1.SetPtEtaPhiM(i_top_tbarwdaupt[0], i_top_tbarwdauphi[0], i_top_tbarwdaueta[0], 0);
	p4wdau2.SetPtEtaPhiM(i_top_tbarwdaupt[1], i_top_tbarwdauphi[1], i_top_tbarwdaueta[1], 0);
      
      }else if (lep_charge < 0 && lep_charge == i_top_lepcharge){
	p4wdau1.SetPtEtaPhiM(i_top_twdaupt[0], i_top_twdauphi[0], i_top_twdaueta[0], 0);
	p4wdau2.SetPtEtaPhiM(i_top_twdaupt[1], i_top_twdauphi[1], i_top_twdaueta[1], 0);
      }else{
	cout << "\r \033[31m  > Warning - Unexpected lepton charge, skipping this event!\033[0m" << endl << " ";                 
        continue;
      }
      

      if (DeltaR(p4wdaujet1,p4wdau1)<DeltaR(p4wdaujet1,p4wdau2)){
	cout << "DR1: " << DeltaR(p4wdaujet1,p4wdau1) << endl; 
	cout << "DR2: " << DeltaR(p4wdaujet2,p4wdau2) << endl; 
      }
      else{
	cout << "DR1: " << DeltaR(p4wdaujet1,p4wdau2) << endl;
        cout << "DR2: " << DeltaR(p4wdaujet2,p4wdau1) << endl;
      }

      
    }// for(iEvt < nEvt)
 
    ifile->Close();
    delete ifile;

    cout << "DONE!" << endl;
    
  }// for (ifile in inputs)

  return 0;
}//main(...)



////////////////////////////////////////////////////////////////////////////////
// helpers
////////////////////////////////////////////////////////////////////////////////


static double DeltaR(const TLorentzVector & v1, const TLorentzVector & v2) {
  Double_t DeltaR = 0.0;
  Double_t DeltaPhi = TMath::Abs(v1.Phi()-v2.Phi());
  if (DeltaPhi>TMath::Pi())
    DeltaPhi = 2*TMath::Pi() - DeltaPhi;
  Double_t DeltaEta = v1.Eta() - v2.Eta();
  DeltaR = sqrt(TMath::Power(DeltaPhi, 2) + TMath::Power(DeltaEta, 2));
  return DeltaR;
}

