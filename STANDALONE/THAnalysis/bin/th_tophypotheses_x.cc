////////////////////////////////////////////////////////////////////////////////
//
// th_tophypotheses_x
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

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
////////////////////////////////////////////////////////////////////////////////
Float_t Matchable(const TLorentzVector & toplep, const TLorentzVector & tophad, const TLorentzVector &whad, const TLorentzVector & gentoplep, const TLorentzVector & gentophad, const TLorentzVector & genwhad);
static double DeltaR(const TLorentzVector & v1, const TLorentzVector & v2);

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
  if (!cl.parse(argc,argv))
    return 0;

  vector<string> inputs      = cl.getVector<string>("inputs");
  string         treename    = cl.getValue<string> ("treename",  "t");
  string         dirname    =  cl.getValue<string> ("dirname", "utm");
  bool           bestdR      = cl.getValue<bool>   ("bestdR",        false);
  bool           perfmatch   = cl.getValue<bool>   ("perfmatch",     false);
  float          workingpoint= cl.getValue<float>  ("workingpoint",  0.935);

  if (!cl.check())
    return 0;
  cl.print();

  //Init random number generator
  srand(time(NULL));

  //
  // loop over all input files
  //

  for (unsigned int iinput=0; iinput<inputs.size(); iinput++) {
  
    // open input file
    string input = inputs[iinput]; 

    // read input file                                                                                                                                                                        
    TFile* ifile = TFile::Open(input.c_str(),"READ");
    if (!ifile->IsOpen()){
      cout << "\033[31m  > Failed to open '" << input << "'\033[0m" << endl;
      return 0;
    }else{
      cout << "\033[32mProcessing input file '" << ifile->GetName() << "'\033[0m" << endl;
    }

    // create output files
    size_t pos = input.find(".root");
    string output = input.substr(0,pos);
    //    output = output + ".sig.root";

    TFile* ofile_sig = new TFile((output+"_sig.root").c_str(),"RECREATE");
    if (!ofile_sig->IsOpen()) {
        cout<<"\033[31m  > Failed to (re)create '" << (output+"_sig.root").c_str() << "'\033[0m" << endl;
        return 0;
    }

    TFile* ofile_bkg = new TFile((output+"_bkg.root").c_str(),"RECREATE");
    if (!ofile_bkg->IsOpen()) {
      cout<<"\033[31m  > Failed to (re)create '" << (output+"_bkg.root").c_str() << "'\033[0m" << endl;
      return 0;
    }

    // read specified directory from input file
    TDirectory* idir    = (TDirectory*)ifile->Get(dirname.c_str());
    if (0 == idir) {
      cout << "\033[31m  > Failed to access directory '" << dirname << "' in input file '" << input << ".root'\033[0m"\
      << endl;
      return 0;
    }

    // create same directory in the output files
    TDirectory* odir_sig = (TDirectory*)ofile_sig->mkdir(dirname.c_str());
    if(0==odir_sig){
        cout<<"\033[31m  > Failed to create directory '" << dirname << "' in '" << output << "_sig.root'\033[0m" << endl;
        return 0;
    }
    TDirectory* odir_bkg = (TDirectory*)ofile_bkg->mkdir(dirname.c_str());
    if(0 == odir_bkg){
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
    if (0!=itree->FindBranch("evt"))           itree->SetBranchStatus("evt",0);
    if (0!=itree->FindBranch("tophadwm"))      itree->SetBranchStatus("tophadwm",0);
    if (0!=itree->FindBranch("tophadwpt"))     itree->SetBranchStatus("tophadwpt",0);
    if (0!=itree->FindBranch("tophadwphi"))    itree->SetBranchStatus("tophadwphi",0);
    if (0!=itree->FindBranch("tophadweta"))    itree->SetBranchStatus("tophadweta",0);
    if (0!=itree->FindBranch("tophadm"))       itree->SetBranchStatus("tophadm",0);
    if (0!=itree->FindBranch("tophadpt"))      itree->SetBranchStatus("tophadpt",0);
    if (0!=itree->FindBranch("tophadphi"))     itree->SetBranchStatus("tophadphi",0);
    if (0!=itree->FindBranch("tophadeta"))     itree->SetBranchStatus("tophadeta",0);
    if (0!=itree->FindBranch("tophaddr"))      itree->SetBranchStatus("tophaddr",0);
    if (0!=itree->FindBranch("tophadjtidx"))   itree->SetBranchStatus("tophadjtidx",0);
    if (0!=itree->FindBranch("tophadjtidx_1")) itree->SetBranchStatus("tophadjtidx_1",0);
    if (0!=itree->FindBranch("tophadjtidx_2")) itree->SetBranchStatus("tophadjtidx_2",0);
    if (0!=itree->FindBranch("tophadjtidx_3")) itree->SetBranchStatus("tophadjtidx_3",0);
    if (0!=itree->FindBranch("toplepm"))       itree->SetBranchStatus("toplepm",0);
    if (0!=itree->FindBranch("topleppt"))      itree->SetBranchStatus("topleppt",0);
    if (0!=itree->FindBranch("toplepphi"))     itree->SetBranchStatus("toplepphi",0);
    if (0!=itree->FindBranch("toplepeta"))     itree->SetBranchStatus("toplepeta",0);
    if (0!=itree->FindBranch("toplepjtidx"))   itree->SetBranchStatus("toplepjtidx",0);
  
    //Change active directory to the output file and copy (minus the muted branches) the input tree
    odir_sig->cd();
    TTree* sigTree = itree->CloneTree(0);
    odir_bkg->cd();
    TTree* bkgTree = itree->CloneTree(0);

    cout << "\033[32m> TTree structure successfully cloned! \033[0m" << endl;

    //Create in sigTree and bkgTree reconstruction dependent information
    ULong64_t evt=-99;                  sigTree->Branch("evt",           &evt,                       "evt/l");
                                        bkgTree->Branch("evt",           &evt,                       "evt/l");
    Int_t   hyp_isbest=-1;              sigTree->Branch("hyp_isbest",    &hyp_isbest,         "hyp_isbest/I");
                                        bkgTree->Branch("hyp_isbest",    &hyp_isbest,         "hyp_isbest/I");
    Float_t hyp_weight=1.;              sigTree->Branch("hyp_weight",    &hyp_weight,         "hyp_weight/F");
                                        bkgTree->Branch("hyp_weight",    &hyp_weight,         "hyp_weight/F");
    // hyp_deltaRrel [ t+h+l , t , h , whad ];
    Float_t hyp_deltaRrel[4]={-99};     sigTree->Branch("hyp_deltaRrel",    hyp_deltaRrel,         "hyp_deltaRrel[4]/F");
                                        bkgTree->Branch("hyp_deltaRrel",    hyp_deltaRrel,         "hyp_deltaRrel[4]/F");
    Float_t tophadwm=-99;               sigTree->Branch("tophadwm",         &tophadwm,                     "tophadwm/F");
                                        bkgTree->Branch("tophadwm",         &tophadwm,                     "tophadwm/F");
    Float_t tophadwpt=-99;              sigTree->Branch("tophadwpt",        &tophadwpt,                   "tophadwpt/F");
                                        bkgTree->Branch("tophadwpt",        &tophadwpt,                   "tophadwpt/F");
    Float_t tophadwphi=-99;             sigTree->Branch("tophadwphi",       &tophadwphi,                 "tophadwphi/F");
                                        bkgTree->Branch("tophadwphi",       &tophadwphi,                 "tophadwphi/F");
    Float_t tophadweta=-99;             sigTree->Branch("tophadweta",       &tophadweta,                 "tophadweta/F");
                                        bkgTree->Branch("tophadweta",       &tophadweta,                 "tophadweta/F");
    Float_t tophadm=-99;                sigTree->Branch("tophadm",          &tophadm,                       "tophadm/F");
                                        bkgTree->Branch("tophadm",          &tophadm,                       "tophadm/F");
    Float_t tophadpt=-99;               sigTree->Branch("tophadpt",         &tophadpt,                     "tophadpt/F");
                                        bkgTree->Branch("tophadpt",         &tophadpt,                     "tophadpt/F");
    Float_t tophadphi=-99;              sigTree->Branch("tophadphi",        &tophadphi,                   "tophadphi/F");
                                        bkgTree->Branch("tophadphi",        &tophadphi,                   "tophadphi/F");
    Float_t tophadeta=-99;              sigTree->Branch("tophadeta",        &tophadeta,                   "tophadeta/F");
                                        bkgTree->Branch("tophadeta",        &tophadeta,                   "tophadeta/F");
    Float_t tophaddr=-99;               sigTree->Branch("tophaddr",         &tophaddr,                     "tophaddr/F");
                                        bkgTree->Branch("tophaddr",         &tophaddr,                     "tophaddr/F");
    Int_t tophadjtidx[3]={-99,-99,-99}; sigTree->Branch("tophadjtidx",      tophadjtidx,             "tophadjtidx[3]/I");
                                        bkgTree->Branch("tophadjtidx",      tophadjtidx,             "tophadjtidx[3]/I");
    
    Float_t toplepm=-99;                sigTree->Branch("toplepm",          &toplepm,                       "toplepm/F");
                                        bkgTree->Branch("toplepm",          &toplepm,                       "toplepm/F");
    Float_t topleppt=-99;               sigTree->Branch("topleppt",         &topleppt,                     "topleppt/F");
                                        bkgTree->Branch("topleppt",         &topleppt,                     "topleppt/F");
    Float_t toplepphi=-99;              sigTree->Branch("toplepphi",        &toplepphi,                   "toplepphi/F");
                                        bkgTree->Branch("toplepphi",        &toplepphi,                   "toplepphi/F");
    Float_t toplepeta=-99;              sigTree->Branch("toplepeta",        &toplepeta,                   "toplepeta/F");
                                        bkgTree->Branch("toplepeta",        &toplepeta,                   "toplepeta/F");
    Int_t toplepjtidx=-99;              sigTree->Branch("toplepjtidx",      &toplepjtidx,               "toplepjtidx/I");
                                        bkgTree->Branch("toplepjtidx",      &toplepjtidx,               "toplepjtidx/I");


    //Get relevant information for creating hypothesis from the input tree
    Int_t   i_njt=-99;               itree->SetBranchAddress("njt",            &i_njt);
    Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
    Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta",         i_jteta);
    Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
    Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);
    Float_t i_jtcsvt[40]={-99};      itree->SetBranchAddress("jtcsvt",       i_jtcsvt);
    Int_t   i_nlepw={-99};           itree->SetBranchAddress("nlepw",        &i_nlepw);
    Float_t i_lepwm={-99};           itree->SetBranchAddress("lepwm",        &i_lepwm);
    Float_t i_lepwpt={-99};          itree->SetBranchAddress("lepwpt",      &i_lepwpt);
    Float_t i_lepwphi={-99};         itree->SetBranchAddress("lepwphi",    &i_lepwphi);
    Float_t i_lepweta={-99};         itree->SetBranchAddress("lepweta",    &i_lepweta);

    Float_t i_top_tpt={-99};         itree->SetBranchAddress("top_tpt",    &i_top_tpt);
    Float_t i_top_teta={-99};        itree->SetBranchAddress("top_teta",  &i_top_teta);
    Float_t i_top_tphi={-99};        itree->SetBranchAddress("top_tphi",  &i_top_tphi);
    Float_t i_top_tm={-99};          itree->SetBranchAddress("top_tm",      &i_top_tm);

    Float_t i_top_tbarpt={-99};      itree->SetBranchAddress("top_tbarpt",    &i_top_tbarpt);
    Float_t i_top_tbareta={-99};     itree->SetBranchAddress("top_tbareta",  &i_top_tbareta);
    Float_t i_top_tbarphi={-99};     itree->SetBranchAddress("top_tbarphi",  &i_top_tbarphi);
    Float_t i_top_tbarm={-99};       itree->SetBranchAddress("top_tbarm",      &i_top_tbarm);

    Float_t i_top_twpt={-99};        itree->SetBranchAddress("top_twpt",    &i_top_twpt);
    Float_t i_top_tweta={-99};       itree->SetBranchAddress("top_tweta",  &i_top_tweta);
    Float_t i_top_twphi={-99};       itree->SetBranchAddress("top_twphi",  &i_top_twphi);
    Float_t i_top_twm={-99};         itree->SetBranchAddress("top_twm",      &i_top_twm);

    Float_t i_top_tbpt={-99};        itree->SetBranchAddress("top_tbpt",    &i_top_tbpt);
    Float_t i_top_tbeta={-99};       itree->SetBranchAddress("top_tbeta",  &i_top_tbeta);
    Float_t i_top_tbphi={-99};       itree->SetBranchAddress("top_tbphi",  &i_top_tbphi);
    Float_t i_top_tbm={-99};         itree->SetBranchAddress("top_tbm",      &i_top_tbm);

    Float_t i_top_tbarwpt={-99};     itree->SetBranchAddress("top_tbarwpt",    &i_top_tbarwpt);
    Float_t i_top_tbarweta={-99};    itree->SetBranchAddress("top_tbarweta",  &i_top_tbarweta);
    Float_t i_top_tbarwphi={-99};    itree->SetBranchAddress("top_tbarwphi",  &i_top_tbarwphi);
    Float_t i_top_tbarwm={-99};      itree->SetBranchAddress("top_tbarwm",      &i_top_tbarwm);

    Float_t i_top_tbarbpt={-99};     itree->SetBranchAddress("top_tbarbpt",    &i_top_tbarbpt);
    Float_t i_top_tbarbeta={-99};    itree->SetBranchAddress("top_tbarbeta",  &i_top_tbarbeta);
    Float_t i_top_tbarbphi={-99};    itree->SetBranchAddress("top_tbarbphi",  &i_top_tbarbphi);
    Float_t i_top_tbarbm={-99};      itree->SetBranchAddress("top_tbarbm",      &i_top_tbarbm);

    Float_t i_top_twdaupt[10]={-99}; itree->SetBranchAddress("top_twdaupt",    i_top_twdaupt);
    Float_t i_top_twdaueta[10]={-99};itree->SetBranchAddress("top_twdaueta",  i_top_twdaueta);
    Float_t i_top_twdauphi[10]={-99};itree->SetBranchAddress("top_twdauphi",  i_top_twdauphi);
    Float_t i_top_twdaum[10]={-99};  itree->SetBranchAddress("top_twdaum",      i_top_twdaum);

    Float_t i_top_tbarwdaupt[10]={-99}; itree->SetBranchAddress("top_tbarwdaupt",    i_top_tbarwdaupt);
    Float_t i_top_tbarwdaueta[10]={-99};itree->SetBranchAddress("top_tbarwdaueta",  i_top_tbarwdaueta);
    Float_t i_top_tbarwdauphi[10]={-99};itree->SetBranchAddress("top_tbarwdauphi",  i_top_tbarwdauphi);
    Float_t i_top_tbarwdaum[10]={-99};  itree->SetBranchAddress("top_tbarwdaum",      i_top_tbarwdaum);
    
    Float_t i_top_lepcharge={-99};   itree->SetBranchAddress("top_lepcharge", &i_top_lepcharge);
    
    Int_t   i_nel={-99};             itree->SetBranchAddress("nel",            &i_nel);
    Float_t i_elpt[10]={-99};        itree->SetBranchAddress("elpt",           i_elpt);
    Float_t i_eleta[10]={-99};       itree->SetBranchAddress("eleta",         i_eleta);
    Float_t i_elphi[10]={-99};       itree->SetBranchAddress("elphi",         i_elphi);
    Float_t i_ele[10]={-99};         itree->SetBranchAddress("ele",             i_ele);
    Float_t i_elcharge[10]={-99};    itree->SetBranchAddress("elcharge",   i_elcharge);

    Int_t   i_nmu={-99};             itree->SetBranchAddress("nmu",            &i_nmu);
    Float_t i_mupt[10]={-99};        itree->SetBranchAddress("mupt",           i_mupt);
    Float_t i_mueta[10]={-99};       itree->SetBranchAddress("mueta",         i_mueta);
    Float_t i_muphi[10]={-99};       itree->SetBranchAddress("muphi",         i_muphi);
    Float_t i_mue[10]={-99};         itree->SetBranchAddress("mue",             i_mue);
    Float_t i_mucharge[10]={-99};    itree->SetBranchAddress("mucharge",   i_mucharge);


    int iSkippedEvents = 0;
    int iProcessedEvents = 0;

    unsigned int nEvt = (unsigned int)(itree->GetEntries());
    cout << "> \033[32m" << nEvt <<"\033[0m events will be processed"<<endl;
    cout << endl << " ";

    //Loop over all events (from input Tree)
    Int_t count_hyp = 0; // counter for all hypotheses
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

      // Is this correct? Only reconstruct for n(isolated leptons)==1 ...
      if(i_nlepw != 1){
	      iSkippedEvents++;
	      continue;
      }
      //... and more than 4 Jets
      if(i_njt < 4) {
	      iSkippedEvents++;
	      continue;
      }
      //Use Lorentz vectors to group variables and to avoid tapeworm names
      TLorentzVector p4jets[100];
      Float_t jetcsvt[100];
      TLorentzVector p4toplep, p4tophad, p4whad;
      TLorentzVector p4toplepb, p4tophadb;
      TLorentzVector p4wdau1, p4wdau2;
      TLorentzVector p4lepw;

      //Set grouped variables for this event in the output tree
      int p4jetcounter = 0;
      std::map<int, int> jtidxmap;
      jtidxmap.insert(std::pair<int,int>(-1,-1));
      for(int jets=0; jets<i_njt; jets++){
        if ((TMath::Abs(i_jtpt[jets]) > 30 && TMath::Abs(i_jteta[jets]) < 2.4) || TMath::Abs(i_jtpt[jets]) > 40   ) {
          p4jets[p4jetcounter].SetPtEtaPhiE(i_jtpt[jets], i_jteta[jets], i_jtphi[jets], i_jte[jets]);
	  jetcsvt[p4jetcounter]=i_jtcsvt[jets];
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
      bool has_wenu = i_nel==1;
      bool has_wmunu = i_nmu==1;
      TLorentzVector p4lepton;
      Float_t lep_charge=0;

      if (has_wenu){
        p4lepton.SetPtEtaPhiE(i_elpt[0],i_eleta[0],i_elphi[0],i_ele[0]);
        lep_charge = i_elcharge[0];
      }else{
        p4lepton.SetPtEtaPhiE(i_mupt[0],i_mueta[0],i_muphi[0],i_mue[0]);
        lep_charge = i_mucharge[0];
      }


      //Fill GenInfo from Top and Anti-Top according to lepton charge
      if(lep_charge > 0 && lep_charge == i_top_lepcharge){
        p4toplep.SetPtEtaPhiM(i_top_tpt,i_top_teta,i_top_tphi,i_top_tm);
        p4tophad.SetPtEtaPhiM(i_top_tbarpt,i_top_tbareta,i_top_tbarphi,i_top_tbarm);
        p4toplepb.SetPtEtaPhiM(i_top_tbpt,i_top_tbeta,i_top_tbphi,i_top_tbm);
        p4tophadb.SetPtEtaPhiM(i_top_tbarbpt,i_top_tbarbeta,i_top_tbarbphi,i_top_tbarbm);
        p4whad.SetPtEtaPhiM(i_top_tbarwpt, i_top_tbarweta,i_top_tbarwphi,i_top_tbarwm);
        p4wdau1.SetPtEtaPhiM(i_top_tbarwdaupt[0], i_top_tbarwdaueta[0], i_top_tbarwdauphi[0], i_top_tbarwdaum[0]);
        p4wdau2.SetPtEtaPhiM(i_top_tbarwdaupt[1], i_top_tbarwdaueta[1], i_top_tbarwdauphi[1], i_top_tbarwdaum[1]);
      }else if (lep_charge < 0 && lep_charge == i_top_lepcharge){
        p4toplep.SetPtEtaPhiM(i_top_tbarpt,i_top_tbareta,i_top_tbarphi,i_top_tbarm);
        p4tophad.SetPtEtaPhiM(i_top_tpt,i_top_teta,i_top_tphi,i_top_tm);
        p4toplepb.SetPtEtaPhiM(i_top_tbarbpt,i_top_tbarbeta,i_top_tbarbphi,i_top_tbarbm);
        p4tophadb.SetPtEtaPhiM(i_top_tbpt,i_top_tbeta,i_top_tbphi,i_top_tbm);
        p4whad.SetPtEtaPhiM(i_top_twpt, i_top_tweta,i_top_twphi,i_top_twm);
        p4wdau1.SetPtEtaPhiM(i_top_twdaupt[0], i_top_twdaueta[0], i_top_twdauphi[0], i_top_twdaum[0]);
        p4wdau2.SetPtEtaPhiM(i_top_twdaupt[1], i_top_twdaueta[1], i_top_twdauphi[1], i_top_twdaum[1]);
      }else{
        iSkippedEvents++;
        //cout << "\r \033[31m  > Warning - Unexpected lepton charge, skipping this event!\033[0m" << endl << " ";
        continue;
      }
      //If it is neither a muon nor an electron -> increase skip counter
      if (!has_wenu &&!has_wmunu){
        iSkippedEvents++;
        // cout << "\r \033[31m  > Warning - Neither an electron nor a muon was detected!\033[0m" << endl << " ";
        continue;
      }
      
      // Storing DeltaR and jet indices with [toplepjt, tophadq1, tophadq2, tophadbjt]
      double best = numeric_limits<double>::infinity();
      vector<int> sig_i (4,-1);
      vector<int> bkg_i (4,-1);
      vector<vector<int> > bkg_config;
      toplepjtidx=-1;
      tophadjtidx[0]=-1;
      tophadjtidx[1]=-1;
      tophadjtidx[2]=-1;

      // --
      // Check if event is matchable
      // --
      double bestdR_wdau1  = numeric_limits<double>::infinity();
      double bestdR_wdau2  = numeric_limits<double>::infinity();
      double bestdR_toplep = numeric_limits<double>::infinity();
      double bestdR_tophad = numeric_limits<double>::infinity();
      
      Bool_t JetIsCentral[p4jetcounter];
      Bool_t  JetIsTagged[p4jetcounter];
      Bool_t  JetIsInReco[p4jetcounter];
      

      // Find best hypothesis perfect match
      Int_t best_i_pm[4]; best_i_pm[0] = -1; best_i_pm[1] = -1; best_i_pm[2] = -1; best_i_pm[3] = -1;
      for(Int_t i=0; i<p4jetcounter; i++){
	JetIsCentral[i] = TMath::Abs(p4jets[i].Eta())<2.4 ? true : false;
	JetIsTagged[i]  = (jetcsvt[i]>workingpoint)       ? true : false;
	JetIsInReco[i]  = ( (!JetIsCentral[i] && p4jets[i].Pt()>40) || JetIsCentral[i] ) ? true :false;

	if (!JetIsInReco[i]) cout << "\r \033[31m  > Warning - Jet not in reco, altough all jets should be!\033[0m" << endl << " ";

	if(JetIsCentral[i] && DeltaR(p4toplepb,p4jets[i])<bestdR_toplep && JetIsTagged[i]){
          bestdR_toplep=DeltaR(p4toplepb,p4jets[i]);
          best_i_pm[0]=i;
        }
        
        if(JetIsInReco[i] && DeltaR(p4wdau1,p4jets[i])<bestdR_wdau1){
          bestdR_wdau1=DeltaR(p4wdau1,p4jets[i]);
          best_i_pm[1]=i;
        }
        if(JetIsInReco[i] && DeltaR(p4wdau2,p4jets[i])<bestdR_wdau2){
          bestdR_wdau2=DeltaR(p4wdau2,p4jets[i]);
          best_i_pm[2]=i;
        }   
        if(JetIsCentral[i] && DeltaR(p4tophad,p4jets[i])<bestdR_tophad && JetIsTagged[i]){
          bestdR_tophad=DeltaR(p4tophadb,p4jets[i]);
          best_i_pm[3]=i;
        }
      }

      // Check, if all have a dR distance <0.3 and are uniquely matched
      Int_t matchable = 1;
      matchable = (bestdR_toplep<0.3)*(bestdR_wdau1<0.3)*(bestdR_tophad<0.3)*(bestdR_wdau2<0.3) \
	          *(count(best_i_pm,best_i_pm+4,best_i_pm[0])==1)*(count(best_i_pm,best_i_pm+4,best_i_pm[1])==1) \
                  *(count(best_i_pm,best_i_pm+4,best_i_pm[2])==1)*(count(best_i_pm,best_i_pm+4,best_i_pm[3])==1);
      //If matchable is already ZERO, event is skipped due to bad jets, not bad reconstruction
      // -> increment iSkippedEvents
      if (matchable == 0){
        iSkippedEvents++;
        continue;
      }

      count_hyp--; // -1 for signal hypothesis

      // Loop over all jets i,j,k,l and get the minimal DeltaR between two of those jets
      // i -> jet from bottom quark of the leptonic top quark decay branch
      // j -> jet from quark 1 of the hadronic W boson decay branch
      // k -> jet from quark 2 of the hadronic W boson decay branch
      // l -> jet from bottom quark of the hadronic top quark decay branch
      for (Int_t i = 0; i < p4jetcounter; ++i){  // b from toplep
        if (!JetIsCentral[i] || !JetIsTagged[i])
          continue;
        TLorentzVector rectoplep = p4lepw + p4jets[i];
        for (Int_t j = 0; j < p4jetcounter; ++j){  // q1 from Whad
	  if (!JetIsInReco[j])
	    continue;
          if (j!=i){
            for (Int_t k=0; k < p4jetcounter; ++k){  // q2 from Whad
	      if (!JetIsInReco[k])
		continue;
	      
	      if ((k!=i )&&(k>j)) {  // switching of q1 & q2 does not change Whad, so take only one
                TLorentzVector recwhad = p4jets[j] + p4jets[k];
                for (Int_t l=0; l < p4jetcounter; ++l){  // b from tophad
                  if (!JetIsCentral[l] || !JetIsTagged[l])
                    continue;

                  if( l!=k && l!=j && l!=i) {
                    count_hyp++;
                    TLorentzVector rectophad;
                    rectophad = recwhad + p4jets[l];
                    Float_t DRtoplep = DeltaR(rectoplep,p4toplep) + fabs(rectoplep.Pt() - p4toplep.Pt())/p4toplep.Pt();
                    Float_t DRtophad = DeltaR(rectophad,p4tophad) + fabs(rectophad.Pt() - p4tophad.Pt())/p4tophad.Pt();
                    Float_t DRwhad = DeltaR(recwhad,p4whad) + fabs(recwhad.Pt() - p4whad.Pt())/p4whad.Pt();
                    Float_t DeltaRval = DRtoplep + DRtophad + DRwhad;
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
                }//end thophad b jet
              }
            }//end q2 from Tophad
          }
        }//end q1 from Tophad
      }// end b from toplep

      //To avoid training bias due to a vast amount of background (= wrong jet to parton assignement) events compared
      //to signal events, for each "signal" one possible "background" permutation will be chosen
      //Get ONE random entry from the background list
      if (bkg_config.size() > 0 && sig_i.at(0) != -1)
        bkg_i = bkg_config.at(rand() % bkg_config.size());
      else
        matchable = 0;

      Int_t i, j, k, l;

      if (matchable) {
        iProcessedEvents++;
        //Process sig_i event:
        i = sig_i.at(0);
        j = sig_i.at(1);
        k = sig_i.at(2);
        l = sig_i.at(3);
        TLorentzVector rectoplep = p4lepw + p4jets[best_i_pm[0]];
        TLorentzVector recwhad = p4jets[best_i_pm[1]] + p4jets[best_i_pm[2]];
        TLorentzVector rectophad = recwhad + p4jets[best_i_pm[3]];
        hyp_isbest = 1;
        hyp_weight = 1.0;
        Float_t DRtoplep = DeltaR(rectoplep, p4toplep) + fabs(rectoplep.Pt() - p4toplep.Pt()) / p4toplep.Pt();
        Float_t DRtophad = DeltaR(rectophad, p4tophad) + fabs(rectophad.Pt() - p4tophad.Pt()) / p4tophad.Pt();
        Float_t DRwhad = DeltaR(recwhad, p4whad) + fabs(recwhad.Pt() - p4whad.Pt()) / p4whad.Pt();
        hyp_deltaRrel[0] = DRtoplep + DRtophad + DRwhad;
        hyp_deltaRrel[1] = DRtoplep;
        hyp_deltaRrel[2] = DRtophad;
        hyp_deltaRrel[3] = DRwhad;
	tophadwm   = (recwhad.M() > 0) ? recwhad.M() : -1;
			    
	//	tophadwm = recwhad.M();
        tophadwpt = recwhad.Pt();
        tophadweta = recwhad.Eta();
        tophadwphi = recwhad.Phi();
        
	//tophadm = rectophad.M();
	tophadm   = (rectophad.M() > 0) ? rectophad.M() : -1;
	
        tophadpt = rectophad.Pt();
        tophadphi = rectophad.Phi();
        tophadeta = rectophad.Eta();
        tophaddr = p4jets[j].DeltaR(p4jets[k]);
        tophadjtidx[0] = jtidxmap[j];
        tophadjtidx[1] = jtidxmap[k];
        tophadjtidx[2] = jtidxmap[l];
	toplepm   = (rectoplep.M() > 0) ? rectoplep.M() : -1;

	//        toplepm = rectoplep.M();
        topleppt = rectoplep.Pt();
        toplepphi = rectoplep.Phi();
        toplepeta = rectoplep.Eta();
        toplepjtidx = jtidxmap[i];
        //Fill the signal tree with the values calculated
        sigTree->Fill();

        //Process bkg_i event:
        i = bkg_i.at(0);
        j = bkg_i.at(1);
        k = bkg_i.at(2);
        l = bkg_i.at(3);
        rectoplep = p4lepw + p4jets[i];
        recwhad = p4jets[j] + p4jets[k];
        rectophad = recwhad + p4jets[l];
        hyp_isbest = 0;
        hyp_weight = 1.0;
        DRtoplep = DeltaR(rectoplep, p4toplep) + fabs(rectoplep.Pt() - p4toplep.Pt()) / p4toplep.Pt();
        DRtophad = DeltaR(rectophad, p4tophad) + fabs(rectophad.Pt() - p4tophad.Pt()) / p4tophad.Pt();
        DRwhad = DeltaR(recwhad, p4whad) + fabs(recwhad.Pt() - p4whad.Pt()) / p4whad.Pt();
        hyp_deltaRrel[0] = DRtoplep + DRtophad + DRwhad;
        hyp_deltaRrel[1] = DRtoplep;
        hyp_deltaRrel[2] = DRtophad;
        hyp_deltaRrel[3] = DRwhad;
	tophadwm   = (recwhad.M() > 0) ? recwhad.M() : -1;
	//        tophadwm = recwhad.M();
        tophadwpt = recwhad.Pt();
        tophadweta = recwhad.Eta();
        tophadwphi = recwhad.Phi();
	tophadm   = (rectophad.M() > 0) ? rectophad.M() : -1;
	//        tophadm = rectophad.M();
        tophadpt = rectophad.Pt();
        tophadphi = rectophad.Phi();
        tophadeta = rectophad.Eta();
        tophaddr = p4jets[j].DeltaR(p4jets[k]);
        tophadjtidx[0] = jtidxmap[j];
x        tophadjtidx[1] = jtidxmap[k];
        tophadjtidx[2] = jtidxmap[l];
	toplepm   = (rectoplep.M() > 0) ? rectoplep.M() : -1;
	//        toplepm = rectoplep.M();
        topleppt = rectoplep.Pt();
        toplepphi = rectoplep.Phi();
        toplepeta = rectoplep.Eta();
        toplepjtidx = jtidxmap[i];
        //Fill the background tree with the values calculated
        bkgTree->Fill();
      }
      //Match jets in the output tree with jets from the input tree and permutate these matchings.
      //Calculate with every (output) permutation the properties of the resulting parent partons.
      //Further on all wrong permutations are the "backgound", the single right permutation is the "signal"
      /* Deprecated
      for (Int_t i = 0; i<i_njt; ++i){  // b from toplep
	      TLorentzVector rectoplep = p4lepw + p4jets[i];
	      for (Int_t j = 0; j<i_njt; ++j){  // q1 from Tophad
          if (j!=i){
            for (Int_t k = 0; k<i_njt; ++k){  // q2 from Tophad
              if ((k!=i )&&(k>j)) {  // switching of q1 & q2 does not change Tophad, so take only one
                TLorentzVector recwhad = p4jets[j] + p4jets[k];
                for (Int_t l = 0; l<i_njt; ++l){  // top had b jet
                  if(l!=k && l!=j && l!=i) {
                    TLorentzVector rectophad = recwhad + p4jets[l];
                    if(best_i[0]==i && best_i[1]==j && best_i[2]==k && best_i[3]==l){
                      hyp_isbest = 1;
                      //iSkippedEvents++;
                      hyp_weight = 1.0;
                    }else if(best_i[0]==-1 || best_i[1]==-1 || best_i[2]==-1 ||best_i[3]==-1){
                      hyp_isbest = -99;
                    }else{
                      hyp_isbest = 0;
                      //attention: fill correct weight (e.g. 1.0/count_hyp)
                      hyp_weight = 1.0;
                    }
                    Float_t DRtoplep = DeltaR(rectoplep,p4toplep) + fabs(rectoplep.Pt() - p4toplep.Pt())/p4toplep.Pt();
                    Float_t DRtophad = DeltaR(rectophad,p4tophad) + fabs(rectophad.Pt() - p4tophad.Pt())/p4tophad.Pt();
                    Float_t DRwhad = DeltaR(recwhad,p4whad) + fabs(recwhad.Pt() - p4whad.Pt())/p4whad.Pt();

                    hyp_deltaRrel[0]  = DRtoplep + DRtophad + DRwhad;
                    hyp_deltaRrel[1]  = DRtoplep;
                    hyp_deltaRrel[2]  = DRtophad;
                    hyp_deltaRrel[3]  = DRwhad;
                    tophadwm          = recwhad.M();
                    tophadwpt         = recwhad.Pt();
                    tophadweta        = recwhad.Eta();
                    tophadwphi        = recwhad.Phi();
                    tophadm           = rectophad.M();
                    tophadpt          = rectophad.Pt();
                    tophadphi         = rectophad.Phi();
                    tophadeta         = rectophad.Eta();
                    tophaddr          = p4jets[j].DeltaR(p4jets[k]);
                    tophadjtidx[0]    = j;
                    tophadjtidx[1]    = k;
                    tophadjtidx[2]    = l;
                    toplepm           = rectoplep.M();
                    topleppt          = rectoplep.Pt();
                    toplepphi         = rectoplep.Phi();
                    toplepeta         = rectoplep.Eta();
                    toplepjtidx       = i;
                    //Fill the output tree with the values of this permutation
                    otree->Fill();
                  }
                }//end top had b jet
              }
            }//end q2 from Tophad
          }
        }//end q1 from Tophad
      }// end b from toplep
     */

    }
    
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
    
  }// ifile
  

  return 0;
}



////////////////////////////////////////////////////////////////////////////////
// helpers
////////////////////////////////////////////////////////////////////////////////

Float_t Matchable(const TLorentzVector & toplep, const TLorentzVector & tophad, const TLorentzVector & whad, const TLorentzVector & gentoplep, const TLorentzVector & gentophad, const TLorentzVector & genwhad){
  Float_t DRtoplep = DeltaR(toplep,gentoplep); 
  Float_t DRtophad = DeltaR(tophad,gentophad);
  Float_t DRwhad = DeltaR(whad,genwhad); 
  return (DRtoplep<0.5)&&(DRtophad<0.5)&&(DRwhad<0.5);//+ptreltoplep+ptreltophad+ptrelwhad;
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
