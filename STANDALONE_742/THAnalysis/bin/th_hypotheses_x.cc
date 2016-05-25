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

    // create output files
    size_t pos = input.find(".root");
    string output = input.substr(0,pos);
    //    output = output + ".sig.root";

    TFile* ofile_sig = new TFile((output+"_sig.root").c_str(),"RECREATE");
    if (!ofile_sig->IsOpen()) {
      cout<<"\033[31m  > Failed to recreate '" << (output+"_sig.root").c_str() << "'\033[0m" << endl;
      return 0;
    }

    TFile* ofile_bkg = new TFile((output+"_bkg.root").c_str(),"RECREATE");
    if (!ofile_bkg->IsOpen()) {
      cout<<"\033[31m  > Failed to recreate '" << (output+"_bkg.root").c_str() << "'\033[0m" << endl;
      return 0;
    }

    // read input directory
    TDirectory* idir = (TDirectory*)ifile->Get(dirname.c_str());
    if (0 == idir) {
      cout << "\033[31m  > Failed to access directory '" << dirname << "' in input file '" << input << ".root'\033[0m"\
      << endl;
      return 0;
    }


    // create output directory signal                                                                                                                                                     
    TDirectory* odir_sig = (TDirectory*)ofile_sig->mkdir(dirname.c_str());
    if(0==odir_sig){
      cout<<"\033[31m  > Failed to create directory '" << dirname << "' in '" << output << "_sig.root'\033[0m" << endl;
      return 0;
    }
    TDirectory* odir_bkg = (TDirectory*)ofile_bkg->mkdir(dirname.c_str());
    if(0==odir_bkg){
      cout<<"\033[31m  > Failed to create directory '" << dirname << "' in '" << output << "_bkg.root'\033[0m" << endl;
      return 0;
    }

    // open input tree                                                                                                                                                                        
    TTree* itree = (TTree*)idir->Get(treename.c_str());
    if(0==itree){
      cout << "\033[31m  > No TTree named '" << treename << "' in '" << input << ":" << dirname << "'\033[0m" << endl;
      return 0;
    }

    //Don't copy reconstruction dependent information -> mute relevant branches
    if (0!=itree->FindBranch("evt")) itree->SetBranchStatus("evt",0); 
    if (0!=itree->FindBranch("hbbm")) itree->SetBranchStatus("hbbm",0); 
    if (0!=itree->FindBranch("hbbpt")) itree->SetBranchStatus("hbbpt",0);
    if (0!=itree->FindBranch("hbbphi")) itree->SetBranchStatus("hbbphi",0);
    if (0!=itree->FindBranch("hbbeta")) itree->SetBranchStatus("hbbeta",0);
    if (0!=itree->FindBranch("hbbdr")) itree->SetBranchStatus("hbbdr",0); 
    if (0!=itree->FindBranch("hbbjtidx")) itree->SetBranchStatus("hbbjtidx",0);
    if (0!=itree->FindBranch("topm")) itree->SetBranchStatus("topm",0);   
    if (0!=itree->FindBranch("toppt")) itree->SetBranchStatus("toppt",0);  
    if (0!=itree->FindBranch("topphi")) itree->SetBranchStatus("topphi",0); 
    if (0!=itree->FindBranch("topeta")) itree->SetBranchStatus("topeta",0); 
    if (0!=itree->FindBranch("topjtidx")) itree->SetBranchStatus("topjtidx",0);
    if (0!=itree->FindBranch("ljtidx")) itree->SetBranchStatus("ljtidx",0);
    if (0!=itree->FindBranch("costwlt_rec")) itree->SetBranchStatus("costwlt_rec",0);
    if (0!=itree->FindBranch("costwlt2_rec")) itree->SetBranchStatus("costwlt2_rec",0);  
    if (0!=itree->FindBranch("costhetastarhiggs")) itree->SetBranchStatus("costhetastarhiggs",0);  
    
    if (0!=itree->FindBranch("matchable")) itree->SetBranchStatus("matchable",0);
  
    
    odir_sig->cd();
    TTree* sigTree = itree->CloneTree(0);
    odir_bkg->cd();
    TTree* bkgTree = itree->CloneTree(0);

    cout << "> TTree structure successfully cloned!" << endl;

    //Create in sigTree and bkgTree reconstruction dependent information
    // nEvent broken in signal MC => Popov?
    ULong64_t evt=-99;                sigTree->Branch("evt",           &evt,                       "evt/l");
                                      bkgTree->Branch("evt",           &evt,                       "evt/l");
    Int_t   hyp_isbest=-1;            sigTree->Branch("hyp_isbest",    &hyp_isbest,         "hyp_isbest/I");
                                      bkgTree->Branch("hyp_isbest",    &hyp_isbest,         "hyp_isbest/I");
    Float_t hyp_weight=1.;            sigTree->Branch("hyp_weight",    &hyp_weight,         "hyp_weight/F");
                                      bkgTree->Branch("hyp_weight",    &hyp_weight,         "hyp_weight/F");
    // hyp_deltaRrel [ t+h+l , t , h , light ];
    Float_t hyp_deltaRrel[4]={-99};   sigTree->Branch("hyp_deltaRrel", hyp_deltaRrel, "hyp_deltaRrel[4]/F");
                                      bkgTree->Branch("hyp_deltaRrel", hyp_deltaRrel, "hyp_deltaRrel[4]/F");
    Float_t hbbm=-99;                 sigTree->Branch("hbbm",          &hbbm,                     "hbbm/F");
                                      bkgTree->Branch("hbbm",          &hbbm,                     "hbbm/F");
    Float_t hbbpt=-99;                sigTree->Branch("hbbpt",         &hbbpt,                   "hbbpt/F");
                                      bkgTree->Branch("hbbpt",         &hbbpt,                   "hbbpt/F");
    Float_t hbbphi=-99;               sigTree->Branch("hbbphi",        &hbbphi,                 "hbbphi/F");
                                      bkgTree->Branch("hbbphi",        &hbbphi,                 "hbbphi/F");
    Float_t hbbeta=-99;               sigTree->Branch("hbbeta",        &hbbeta,                 "hbbeta/F");
                                      bkgTree->Branch("hbbeta",        &hbbeta,                 "hbbeta/F");
    Float_t hbbdr=-99;                sigTree->Branch("hbbdr",         &hbbdr,                   "hbbdr/F");
                                      bkgTree->Branch("hbbdr",         &hbbdr,                   "hbbdr/F");
    Int_t hbbjtidx[3]={-99,-99,-99};  sigTree->Branch("hbbjtidx",      hbbjtidx,           "hbbjtidx[3]/I");
                                      bkgTree->Branch("hbbjtidx",      hbbjtidx,           "hbbjtidx[3]/I");
    Float_t topm=-99;                 sigTree->Branch("topm",          &topm,                     "topm/F");
                                      bkgTree->Branch("topm",          &topm,                     "topm/F");
    Float_t toppt=-99;                sigTree->Branch("toppt",         &toppt,                   "toppt/F");
                                      bkgTree->Branch("toppt",         &toppt,                   "toppt/F");
    Float_t topphi=-99;               sigTree->Branch("topphi",        &topphi,                 "topphi/F");
                                      bkgTree->Branch("topphi",        &topphi,                 "topphi/F");
    Float_t topeta=-99;               sigTree->Branch("topeta",        &topeta,                 "topeta/F");
                                      bkgTree->Branch("topeta",        &topeta,                 "topeta/F");
    Int_t topjtidx=-99;               sigTree->Branch("topjtidx",      &topjtidx,             "topjtidx/I");
                                      bkgTree->Branch("topjtidx",      &topjtidx,             "topjtidx/I");
    Int_t ljtidx=-99;                 sigTree->Branch("ljtidx",        &ljtidx,                 "ljtidx/I");
                                      bkgTree->Branch("ljtidx",        &ljtidx,                 "ljtidx/I");
    Float_t costwlt_rec=-99;          sigTree->Branch("costwlt_rec",   &costwlt_rec,       "costwlt_rec/F");
                                      bkgTree->Branch("costwlt_rec",   &costwlt_rec,       "costwlt_rec/F");
    Float_t costwlt2_rec=-99;         sigTree->Branch("costwlt2_rec",  &costwlt2_rec,     "costwlt2_rec/F");
                                      bkgTree->Branch("costwlt2_rec",  &costwlt2_rec,     "costwlt2_rec/F");
    Int_t matchable =-1;              sigTree->Branch("matchable",     &matchable,           "matchable/I");
                                      bkgTree->Branch("matchable",     &matchable,           "matchable/I");
    Float_t costhetastarhiggs = -99;  sigTree->Branch("costhetastarhiggs", &costhetastarhiggs,  "costhetastarhiggs/F");
                                      bkgTree->Branch("costhetastarhiggs", &costhetastarhiggs,  "costhetastarhiggs/F");
    Float_t btoplepm = -99;           sigTree->Branch("btoplepm", &btoplepm,  "btoplepm/F");
                                      bkgTree->Branch("btoplepm", &btoplepm,  "btoplepm/F");

    //Get relevant information for creating hypothesis
    Int_t   i_njt=-99;               itree->SetBranchAddress("njt",            &i_njt);
    Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
    Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta",         i_jteta);
    Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
    Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);
    Float_t i_jtcsvt[40]={-99};      itree->SetBranchAddress("jtcsvt",       i_jtcsvt);
    Int_t i_nlepw={-99};             itree->SetBranchAddress("nlepw",        &i_nlepw);
    Float_t i_lepwm={-99};           itree->SetBranchAddress("lepwm",        &i_lepwm);
    Float_t i_lepwpt={-99};          itree->SetBranchAddress("lepwpt",      &i_lepwpt);
    Float_t i_lepwphi={-99};         itree->SetBranchAddress("lepwphi",    &i_lepwphi);
    Float_t i_lepweta={-99};         itree->SetBranchAddress("lepweta",    &i_lepweta);

    Float_t i_Hpt={-99};             itree->SetBranchAddress("Hpt",            &i_Hpt);
    Float_t i_Heta={-99};            itree->SetBranchAddress("Heta",          &i_Heta);
    Float_t i_Hphi={-99};            itree->SetBranchAddress("Hphi",          &i_Hphi);
    Float_t i_Hm={-99};              itree->SetBranchAddress("Hm",              &i_Hm);

    Float_t i_Hdaupt[2]={-99};       itree->SetBranchAddress("Hdaupt",       i_Hdaupt);
    Float_t i_Hdaueta[2]={-99};      itree->SetBranchAddress("Hdaueta",     i_Hdaueta);
    Float_t i_Hdauphi[2]={-99};      itree->SetBranchAddress("Hdauphi",     i_Hdauphi);
    Float_t i_Hdaum[2]={-99};        itree->SetBranchAddress("Hdaum",         i_Hdaum);

    Float_t i_btoppt={-99};          itree->SetBranchAddress("btoppt",      &i_btoppt);
    Float_t i_btopeta={-99};         itree->SetBranchAddress("btopeta",    &i_btopeta);
    Float_t i_btopphi={-99};         itree->SetBranchAddress("btopphi",    &i_btopphi);
    Float_t i_btopm={-99};           itree->SetBranchAddress("btopm",        &i_btopm);

    Float_t i_sbpt={-99};            itree->SetBranchAddress("sbpt",          &i_sbpt);
    Float_t i_sbeta={-99};           itree->SetBranchAddress("sbeta",        &i_sbeta);
    Float_t i_sbphi={-99};           itree->SetBranchAddress("sbphi",        &i_sbphi);
    Float_t i_sbm={-99};             itree->SetBranchAddress("sbm",            &i_sbm);

    Float_t i_tpt={-99};             itree->SetBranchAddress("tpt",            &i_tpt);
    Float_t i_teta={-99};            itree->SetBranchAddress("teta",          &i_teta);
    Float_t i_tphi={-99};            itree->SetBranchAddress("tphi",          &i_tphi);
    Float_t i_tm={-99};              itree->SetBranchAddress("tm",              &i_tm);

    Float_t i_lqpt={-99};            itree->SetBranchAddress("lqpt",          &i_lqpt);
    Float_t i_lqeta={-99};           itree->SetBranchAddress("lqeta",        &i_lqeta);
    Float_t i_lqphi={-99};           itree->SetBranchAddress("lqphi",        &i_lqphi);
    Float_t i_lqm={-99};             itree->SetBranchAddress("lqm",            &i_lqm);
    
    Int_t   i_nel={-99};             itree->SetBranchAddress("nel",            &i_nel);
    Int_t   i_nlel={-99};            itree->SetBranchAddress("nlel",          &i_nlel);
    Float_t i_elpt[10]={-99};        itree->SetBranchAddress("elpt",           i_elpt);
    Float_t i_eleta[10]={-99};       itree->SetBranchAddress("eleta",         i_eleta);
    Float_t i_elphi[10]={-99};       itree->SetBranchAddress("elphi",         i_elphi);
    Float_t i_ele[10]={-99};         itree->SetBranchAddress("ele",             i_ele);


    Int_t   i_nmu={-99};             itree->SetBranchAddress("nmu",            &i_nmu);
    Int_t   i_nlmu={-99};            itree->SetBranchAddress("nlmu",          &i_nlmu); 
    Float_t i_mupt[10]={-99};        itree->SetBranchAddress("mupt",           i_mupt);
    Float_t i_mueta[10]={-99};       itree->SetBranchAddress("mueta",         i_mueta);
    Float_t i_muphi[10]={-99};       itree->SetBranchAddress("muphi",         i_muphi);
    Float_t i_mue[10]={-99};         itree->SetBranchAddress("mue",             i_mue);

    


    int iSkippedEvents = 0;
    int iProcessedEvents = 0;

    unsigned int nEvt = (unsigned int)(itree->GetEntries());
    cout << "> \033[32m" << nEvt <<"\033[0m events will be processed"<<endl;
    cout << endl << " ";

    gErrorIgnoreLevel = kError;
    Int_t count_hyp = 0; // counter for all hypotheses
    //event loop
    for (unsigned int iEvt = 0; iEvt < nEvt; ++iEvt) {
      //  > Output sign of life
      if (iEvt %250 == 0 || iEvt <= 10){
        cout << "\r  > Processing events from input tree:  (\033[33m" << iEvt << "/" << nEvt << "\033[0m)  -> \033[32m";
        cout << setprecision(4) << setiosflags(std::ios_base::left) << setfill('0') << setw(5)\
             << (iEvt*100.0)/nEvt << "\033[0m%    \b\b\b" << flush;
      }
      //'NULL' important variables (to give them a definite state)
      hyp_isbest=-99;
      evt = iEvt;
      //Retrieve data from input Tree
      itree->GetEntry(iEvt);

      //Only reconstruct for n(isolated leptons)==1
      if(i_nlepw != 1){
        iSkippedEvents++;
        continue;
      }

      //Use Lorentz vectors to group variables and to avoid tapeworm names
      TLorentzVector p4jets[100];
      Float_t jetcsvt[100];
      TLorentzVector p4top, p4higgs, p4light;
      TLorentzVector p4lepw;
      TLorentzVector p4btop;
      TLorentzVector p4sb;
      TLorentzVector p4hdau1, p4hdau2;

      //Set grouped variables for this event in the output tree
      int p4jetcounter = 0;
      std::map<int, int> jtidxmap;
      jtidxmap.insert(std::pair<int,int>(-1,-1));
      for(int jets=0; jets<i_njt; ++jets){
        //Cut out jets with less then pt < 30GeV
        if (TMath::Abs(i_jtpt[jets]) > 30) {
          p4jets[p4jetcounter].SetPtEtaPhiE(i_jtpt[jets], i_jteta[jets], i_jtphi[jets], i_jte[jets]);
	  jetcsvt[p4jetcounter] = i_jtcsvt[jets];
          jtidxmap[p4jetcounter] = jets;
          p4jetcounter++;
        }
      }
      //Don't construct with less than 4 Jets
      if(p4jetcounter < 4) {
        iSkippedEvents++;
        continue;
      }
      p4lepw.SetPtEtaPhiM(i_lepwpt,i_lepweta,i_lepwphi,i_lepwm);

      //Distinct between electron and muon case to set generalized lepton variables
      bool has_wenu = (i_nel==1 && i_nlel==1 && i_nmu==0 && i_nlmu==0);
      bool has_wmunu = (i_nmu==1 && i_nlmu==1 && i_nel==0 && i_nlel==0);
      TLorentzVector p4lepton;

      if (has_wenu){
	      p4lepton.SetPtEtaPhiE(i_elpt[0],i_eleta[0],i_elphi[0],i_ele[0]);
	    }else{
	      p4lepton.SetPtEtaPhiE(i_mupt[0],i_mueta[0],i_muphi[0],i_mue[0]);
	    }
      if (!has_wenu &&!has_wmunu){
        iSkippedEvents++;
        //cout << "\r \033[31m  > Warning - Neither an electron nor a muon was detected!\033[0m" << endl << " ";
	      continue;
	    }

      // gen particles
      p4higgs.SetPtEtaPhiM(i_Hpt,i_Heta,i_Hphi,i_Hm);
      p4top.SetPtEtaPhiM(i_tpt,i_teta,i_tphi,i_tm);
      p4light.SetPtEtaPhiM(i_lqpt, i_lqeta,i_lqphi,i_lqm);
      p4btop.SetPtEtaPhiM(i_btoppt,i_btopeta,i_btopphi,i_btopm);
      p4sb.SetPtEtaPhiM(i_sbpt,i_sbeta,i_sbphi,i_sbm);
      p4hdau1.SetPtEtaPhiM(i_Hdaupt[0],i_Hdaueta[0],i_Hdauphi[0],i_Hdaum[0]);
      p4hdau2.SetPtEtaPhiM(i_Hdaupt[1],i_Hdaueta[1],i_Hdauphi[1],i_Hdaum[1]);

      // Storing DeltaR and jet idxs with [topjt, higgsjt1, higgsjt2, lightjt]
      double best = numeric_limits<double>::infinity();
      vector<int> sig_i (4,-1);
      vector<int> bkg_i (4,-1);
      vector<vector<int> > bkg_config;
      Int_t best_i[4]; best_i[0] = -1; best_i[1] = -1; best_i[2] = -1; best_i[3] = -1;
      Int_t best_i_pm[4]; best_i_pm[0] = -1; best_i_pm[1] = -1; best_i_pm[2] = -1; best_i_pm[3] = -1;

      //Jet Indices
      ljtidx=-1;
      topjtidx=-1;
      hbbjtidx[0]=-1;
      hbbjtidx[1]=-1;

      // --
      // Check if event is matchable
      // --
      Float_t bestdR_hdau1=99;
      Float_t bestdR_hdau2=99;
      Float_t bestdR_sb=99;
      Float_t bestdR_btop=99;
      Float_t bestdR_light=99;
      
      // Find best hypothesis perfect match
      for(Int_t i=0; i<p4jetcounter; i++){
	      if(TMath::Abs(p4jets[i].Eta())<2.4&&Matchable(p4hdau1,p4jets[i])<bestdR_hdau1){
	        bestdR_hdau1=Matchable(p4hdau1,p4jets[i]);
	        best_i_pm[0]=i;
	      }
	      if(TMath::Abs(p4jets[i].Eta())<2.4&&Matchable(p4hdau2,p4jets[i])<bestdR_hdau2){
	        bestdR_hdau2=Matchable(p4hdau2,p4jets[i]);
	        best_i_pm[1]=i;
	      }
	      if(TMath::Abs(p4jets[i].Eta())<2.4&&Matchable(p4btop,p4jets[i])<bestdR_btop){
	        bestdR_btop=Matchable(p4btop,p4jets[i]);
	        best_i_pm[2]=i;
	      }
	      if(jetcsvt[i]<0.941&&Matchable(p4light,p4jets[i])<bestdR_light){
	        bestdR_light=Matchable(p4light,p4jets[i]);
	        best_i_pm[3]=i;
	      }
	    }

      // Check, if all have a dR distance <0.3 and are uniquely matched
      matchable = (bestdR_hdau1<0.3)*(bestdR_hdau2<0.3)*(bestdR_btop<0.3)*(bestdR_light<0.3)\
                  *(count(best_i_pm,best_i_pm+4,best_i_pm[0])==1)*(count(best_i_pm,best_i_pm+4,best_i_pm[1])==1)\
                  *(count(best_i_pm,best_i_pm+4,best_i_pm[2])==1)*(count(best_i_pm,best_i_pm+4,best_i_pm[3])==1);
      count_hyp--; // -1 for signal hypothesis

      for (Int_t i=0; i<p4jetcounter; i++){  // b from top
        if (TMath::Abs(p4jets[i].Eta()) > 2.4)
          continue;
        TLorentzVector rectop = p4lepw + p4jets[i];

        for (Int_t j=0; j<p4jetcounter; j++){  // b1 from Higgs
          if (j!=i){
            for (Int_t k=0; k<p4jetcounter; k++){  // b2 from Higgs
              if ((k!=i )&&(k>j)) {  // switching of b1 & b2 does not change Higgs, so take only one
                if (TMath::Abs(p4jets[j].Eta()) > 2.4 || TMath::Abs(p4jets[k].Eta()) > 2.4)
                  continue;
                TLorentzVector rechiggs = p4jets[j] + p4jets[k];
                for (Int_t l=0; l<p4jetcounter; l++){  // light jet
		  if (jetcsvt[l]>0.941)
		    continue;
                  if(l!=k && l!=j && l!=i){
                    count_hyp++;
                    TLorentzVector reclight;
                    reclight=p4jets[l];
                    Float_t DRtop = DeltaR(rectop,p4top) + fabs(rectop.Pt() - p4top.Pt())/p4top.Pt();
                    Float_t DRhiggs = DeltaR(rechiggs,p4higgs) + fabs(rechiggs.Pt() - p4higgs.Pt())/p4higgs.Pt();
                    Float_t DRlight = DeltaR(reclight,p4light) + fabs(reclight.Pt() - p4light.Pt())/p4light.Pt();
                    Float_t DeltaRval = DRtop + DRhiggs + DRlight;
                    if(DeltaRval < best){
                      if (sig_i.at(0) != -1) {
                        bkg_config.push_back(sig_i);
                      }
                      best = DeltaRval;
                      sig_i.at(0) = i;
                      sig_i.at(1) = j;
                      sig_i.at(2) = k;
                      sig_i.at(3) = l;
                    }else{
                      bkg_i.at(0) = i;
                      bkg_i.at(1) = j;
                      bkg_i.at(2) = k;
                      bkg_i.at(3) = l;
                      bkg_config.push_back(bkg_i);
                    }
                  }
                }//end light jet
              }
            }//end b2 from Higgs
          }
        }//end b1 from Higgs
      }  // end b from top 

      //To avoid training bias due to a vast amount of background (= wrong jet to parton assignement) events compared
      //to signal events, for each "signal" one possible "background" permutation will be chosen

      //Get ONE random entry from the background list
      if (bkg_config.size() > 0 && sig_i.at(0) != -1)
        bkg_i = bkg_config.at(rand() % bkg_config.size());
      else
        matchable = 0;

      Int_t i, j, k, l;
      //Process sig_i event:
      if (matchable) {
        iProcessedEvents++;
        i = best_i_pm[2];
        j = best_i_pm[0];
        k = best_i_pm[1];
        l = best_i_pm[3];
        TLorentzVector rectop = p4lepw + p4jets[best_i_pm[2]];
        TLorentzVector rechiggs = p4jets[best_i_pm[0]] + p4jets[best_i_pm[1]];
        TLorentzVector reclight = p4jets[best_i_pm[3]];
        hyp_isbest = 1;
        hyp_weight = 1.0;
        Float_t DRtop = DeltaR(rectop, p4top) + fabs(rectop.Pt() - p4top.Pt()) / p4top.Pt();
        Float_t DRhiggs = DeltaR(rechiggs, p4higgs) + fabs(rechiggs.Pt() - p4higgs.Pt()) / p4higgs.Pt();
        Float_t DRlight = DeltaR(reclight, p4light) + fabs(reclight.Pt() - p4light.Pt()) / p4light.Pt();
        hyp_deltaRrel[0] = DRtop + DRhiggs + DRlight;
        hyp_deltaRrel[1] = DRtop;
        hyp_deltaRrel[2] = DRhiggs;
        hyp_deltaRrel[3] = DRlight;
        ljtidx = jtidxmap[l];
        hbbm = rechiggs.M();
        hbbpt = rechiggs.Pt();
        hbbphi = rechiggs.Phi();
        hbbeta = rechiggs.Eta();
        hbbdr = p4jets[j].DeltaR(p4jets[k]);
        hbbjtidx[0] = jtidxmap[j];
        hbbjtidx[1] = jtidxmap[k];
        topm = rectop.M();
        toppt = rectop.Pt();
        topphi = rectop.Phi();
        topeta = rectop.Eta();
        topjtidx = jtidxmap[i];
        sigTree->Fill();


        //Process bkg_i event:
        i = bkg_i.at(0);
        j = bkg_i.at(1);
        k = bkg_i.at(2);
        l = bkg_i.at(3);
        rectop = p4lepw + p4jets[i];
        rechiggs = p4jets[j] + p4jets[k];
        reclight = p4jets[l];
        hyp_isbest = 0;
        hyp_weight = 1.0;
        DRtop = DeltaR(rectop, p4top) + fabs(rectop.Pt() - p4top.Pt()) / p4top.Pt();
        DRhiggs = DeltaR(rechiggs, p4higgs) + fabs(rechiggs.Pt() - p4higgs.Pt()) / p4higgs.Pt();
        DRlight = DeltaR(reclight, p4light) + fabs(reclight.Pt() - p4light.Pt()) / p4light.Pt();
        hyp_deltaRrel[0] = DRtop + DRhiggs + DRlight;
        hyp_deltaRrel[1] = DRtop;
        hyp_deltaRrel[2] = DRhiggs;
        hyp_deltaRrel[3] = DRlight;
        ljtidx = jtidxmap[l];
        hbbm = rechiggs.M();
        hbbpt = rechiggs.Pt();
        hbbphi = rechiggs.Phi();
        hbbeta = rechiggs.Eta();
        hbbdr = p4jets[j].DeltaR(p4jets[k]);
        hbbjtidx[0] = jtidxmap[j];
        hbbjtidx[1] = jtidxmap[k];
        topm = rectop.M();
        toppt = rectop.Pt();
        topphi = rectop.Phi();
        topeta = rectop.Eta();
        topjtidx = jtidxmap[i];
        bkgTree->Fill();
      }
    }// for(iEvt < nEvt)

    cout << endl << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "Skipped events due to wrong lepton/jet count: " << iSkippedEvents << endl;
    cout << "                 Number of events after cuts: " << nEvt - iSkippedEvents << endl;
    cout << "         Number of actually processed events: " << iProcessedEvents << endl;
    cout << "  Number of possible background permutations: " << count_hyp << endl;

    ifile->Close();
    delete ifile;
    cout << "Save signal tree as \033[33m'" << output << "_sig.root'\033[0m " << flush;
    ofile_sig->Write();
    ofile_sig->Close();
    delete ofile_sig;
    cout << "and background tree as \033[33m'" << output << "_bkg.root'\033[0m ... " << flush;
    ofile_bkg->Write();
    ofile_bkg->Close();
    delete ofile_bkg;

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
