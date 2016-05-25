////////////////////////////////////////////////////////////////////////////////
//
// th_csvreweight_x
// -----------------
//
//            10/11/2013 Benedikt Maier <benedikt.maier@cern.ch>
//                                                    
////////////////////////////////////////////////////////////////////////////////

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TEventList.h>
#include <TLorentzVector.h>
#include <TH1.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "../interface/BTagSF.h"
#include "../interface/CommandLine.h"
#include "../interface/BTagWeight2.h"

using namespace std;

// helpers
//______________________________________________________________________________

float getCSVweight(int iSysHF,int iSysC, int iSysLF, Int_t njt, Short_t jtgenflv[], Float_t jtpt[], Float_t jteta[], Float_t jtcsvt[]);
void getBTagWeight(float& btag2, float& btag3, float& btag4, float& btagweight, Int_t njt, Short_t jtgenflv[], Float_t jtpt[], Float_t jteta[], Float_t sft[], Float_t csvt[]);
void fillScaleFactors(float pt, float eta, int flv, Float_t& csvt, int var=0);
string get_name(string sample);

TH1D* h_csv_wgt_hf[9][5];
TH1D* c_csv_wgt_hf[9][5];
TH1D* h_csv_wgt_lf[9][3][3];

BTagSampleEfficiency2 * btagEff2; 

///////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main(int argc,char**argv)
{
  //
  // evaluate command line options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs      = cl.getVector<string>("inputs");
  string         treename    = cl.getValue<string> ("treename",  "t");
  string         datapath    = cl.getValue<string> ("datapath",  "/");
  bool           clean       = cl.getValue<bool>   ("clean",   false);
  string         sample      = cl.getValue<string> ("sample",     "");
  //int            iSysHF      = cl.getValue<int>    ("iSysHF",      0); // Btag up/down: 3/4   Mistag up/down: 0/0 
  //int            iSysC       = cl.getValue<int>    ("iSysC",       0); // Btag up/down: 0/0   Mistag up/down: 0/0
  //int            iSysLF      = cl.getValue<int>    ("iSysLF",      0); // Btag up/down: 0/0   Mistag up/down: 3/4

  if (!cl.check()) return 0;
  cl.print();

  
  //
  // loop over all input files
  //
  for (unsigned iinput=0;iinput<inputs.size();iinput++) {
  
    // open input file
    string input = inputs[iinput]; //input = datapath + "/" + input;
    TFile* ifile = new TFile(input.c_str(),"READ");
    if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

    // create temporary output file
    size_t pos = input.find(".root");
    string output = input.substr(0,pos);
    output = output + ".csv.root";
    TFile* ofile = new TFile(output.c_str(),"CREATE");
    if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
    
    // loop over input directories
    TIter next(ifile->GetListOfKeys());
    TKey* key(0);
    while ((key=(TKey*)next())) {

      if (strcmp(key->GetClassName(),"TDirectoryFile")!=0) continue;
      
      // read input directory
      TDirectory* idir    = (TDirectory*)key->ReadObj();
      string      dirname = idir->GetName();
      cout<<input<<"/"<<dirname<<":"<<endl;
      
      // create output directory
      TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
      if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}
      
      // open input tree
      TTree* itree = (TTree*)idir->Get(treename.c_str());
      if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<dirname<<endl;return 0;}
      
      // check wether relevant branches are present
      if (0==itree->FindBranch("nlepw")){ cout<<"no W->lnu branches stored!"<<endl; return 0; }
      

      // don't copy branches if they already exist
      
      if (0!=itree->FindBranch("weight_csv"))             itree->SetBranchStatus("weight_csv",            0);
      if (0!=itree->FindBranch("weight_csv_jesup"))       itree->SetBranchStatus("weight_csv_jesup",      0);
      if (0!=itree->FindBranch("weight_csv_jesdown"))     itree->SetBranchStatus("weight_csv_jesdown",    0);

      if (0!=itree->FindBranch("weight_btag"))            itree->SetBranchStatus("weight_btag",           0);
      if (0!=itree->FindBranch("weight_btag_up"))         itree->SetBranchStatus("weight_btag_up",        0);
      if (0!=itree->FindBranch("weight_btag_down"))       itree->SetBranchStatus("weight_btag_down",      0);
      if (0!=itree->FindBranch("weight_btag_LFup"))         itree->SetBranchStatus("weight_btag_LFup",        0);
      if (0!=itree->FindBranch("weight_btag_LFdown"))       itree->SetBranchStatus("weight_btag_LFdown",      0);
      if (0!=itree->FindBranch("weight_btag_HFup"))         itree->SetBranchStatus("weight_btag_HFup",        0);
      if (0!=itree->FindBranch("weight_btag_HFdown"))       itree->SetBranchStatus("weight_btag_HFdown",      0);
     
      if (0!=itree->FindBranch("weight_csv_hfup"))        itree->SetBranchStatus("weight_csv_hfup",       0);
      if (0!=itree->FindBranch("weight_csv_hfdown"))      itree->SetBranchStatus("weight_csv_hfdown",     0);
      if (0!=itree->FindBranch("weight_csv_lfstat1up"))   itree->SetBranchStatus("weight_csv_lfstat1up",  0);
      if (0!=itree->FindBranch("weight_csv_lfstat1down")) itree->SetBranchStatus("weight_csv_lfstat1down",0);
      if (0!=itree->FindBranch("weight_csv_lfstat2up"))   itree->SetBranchStatus("weight_csv_lfstat2up",  0);
      if (0!=itree->FindBranch("weight_csv_lfstat2down")) itree->SetBranchStatus("weight_csv_lfstat2down",0);
      
      if (0!=itree->FindBranch("weight_csv_lfup"))        itree->SetBranchStatus("weight_csv_lfup",       0);
      if (0!=itree->FindBranch("weight_csv_lfdown"))      itree->SetBranchStatus("weight_csv_lfdown",     0);
      if (0!=itree->FindBranch("weight_csv_hfstat1up"))   itree->SetBranchStatus("weight_csv_hfstat1up",  0);
      if (0!=itree->FindBranch("weight_csv_hfstat1down")) itree->SetBranchStatus("weight_csv_hfstat1down",0);
      if (0!=itree->FindBranch("weight_csv_hfstat2up"))   itree->SetBranchStatus("weight_csv_hfstat2up",  0);
      if (0!=itree->FindBranch("weight_csv_hfstat2down")) itree->SetBranchStatus("weight_csv_hfstat2down",0);

      if (0!=itree->FindBranch("btag2MSF"))               itree->SetBranchStatus("btag2MSF",              0);
      if (0!=itree->FindBranch("btag3MSF"))               itree->SetBranchStatus("btag3MSF",              0);
      if (0!=itree->FindBranch("btag4MSF"))               itree->SetBranchStatus("btag4MSF",              0);
      if (0!=itree->FindBranch("btag2TSF"))               itree->SetBranchStatus("btag2TSF",              0);
      if (0!=itree->FindBranch("btag3TSF"))               itree->SetBranchStatus("btag3TSF",              0);
      if (0!=itree->FindBranch("btag4TSF"))               itree->SetBranchStatus("btag4TSF",              0);
      if (0!=itree->FindBranch("btag2TSF_up"))            itree->SetBranchStatus("btag2TSF_up",           0);
      if (0!=itree->FindBranch("btag3TSF_up"))            itree->SetBranchStatus("btag3TSF_up",           0);
      if (0!=itree->FindBranch("btag4TSF_up"))            itree->SetBranchStatus("btag4TSF_up",           0);
      if (0!=itree->FindBranch("btag2TSF_down"))          itree->SetBranchStatus("btag2TSF_down",         0);
      if (0!=itree->FindBranch("btag3TSF_down"))          itree->SetBranchStatus("btag3TSF_down",         0);
      if (0!=itree->FindBranch("btag4TSF_down"))          itree->SetBranchStatus("btag4TSF_down",         0);

      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;


      // if cleaning is requested, nothing further!
      if (clean) continue;
      
      // create branches in output tree
       
      Float_t weight_csv;
      Float_t weight_csv_jesup;
      Float_t weight_csv_jesdown;
      Float_t weight_csv_hfup;
      Float_t weight_csv_hfdown;
      Float_t weight_csv_lfstat1up;
      Float_t weight_csv_lfstat1down;
      Float_t weight_csv_lfstat2up;
      Float_t weight_csv_lfstat2down;
      Float_t weight_csv_lfup;
      Float_t weight_csv_lfdown;
      Float_t weight_csv_hfstat1up;
      Float_t weight_csv_hfstat1down;
      Float_t weight_csv_hfstat2up;
      Float_t weight_csv_hfstat2down;


      TBranch* b_weight_csv             =otree->Branch("weight_csv",            &weight_csv,                          "weight_csv/F");
      TBranch* b_weight_csv_jesup       =otree->Branch("weight_csv_jesup",      &weight_csv_jesup,              "weight_csv_jesup/F");
      TBranch* b_weight_csv_jesdown     =otree->Branch("weight_csv_jesdown",    &weight_csv_jesdown,          "weight_csv_jesdown/F");
      
      TBranch* b_weight_csv_hfup        =otree->Branch("weight_csv_hfup",       &weight_csv_hfup,                "weight_csv_hfup/F");
      TBranch* b_weight_csv_hfdown      =otree->Branch("weight_csv_hfdown",     &weight_csv_hfdown,            "weight_csv_hfdown/F");
      TBranch* b_weight_csv_lfstat1up   =otree->Branch("weight_csv_lfstat1up",  &weight_csv_lfstat1up,      "weight_csv_lfstat1up/F");
      TBranch* b_weight_csv_lfstat1down =otree->Branch("weight_csv_lfstat1down",&weight_csv_lfstat1down,  "weight_csv_lfstat1down/F");
      TBranch* b_weight_csv_lfstat2up   =otree->Branch("weight_csv_lfstat2up",  &weight_csv_lfstat2up,      "weight_csv_lfstat2up/F");
      TBranch* b_weight_csv_lfstat2down =otree->Branch("weight_csv_lfstat2down",&weight_csv_lfstat2down,  "weight_csv_lfstat2down/F");
      
      TBranch* b_weight_csv_lfup        =otree->Branch("weight_csv_lfup",       &weight_csv_lfup,                "weight_csv_lfup/F");
      TBranch* b_weight_csv_lfdown      =otree->Branch("weight_csv_lfdown",     &weight_csv_lfdown,            "weight_csv_lfdown/F");
      TBranch* b_weight_csv_hfstat1up   =otree->Branch("weight_csv_hfstat1up",  &weight_csv_hfstat1up,      "weight_csv_hfstat1up/F");
      TBranch* b_weight_csv_hfstat1down =otree->Branch("weight_csv_hfstat1down",&weight_csv_hfstat1down,  "weight_csv_hfstat1down/F");
      TBranch* b_weight_csv_hfstat2up   =otree->Branch("weight_csv_hfstat2up",  &weight_csv_hfstat2up,      "weight_csv_hfstat2up/F");
      TBranch* b_weight_csv_hfstat2down =otree->Branch("weight_csv_hfstat2down",&weight_csv_hfstat2down,  "weight_csv_hfstat2down/F");

      float weight_btag=1.;        TBranch* b_weight_btag =        otree->Branch("weight_btag",        &weight_btag,         "weight_btag/F"       );
      float weight_btag_up=1.;     TBranch* b_weight_btag_up =     otree->Branch("weight_btag_up",     &weight_btag_up,      "weight_btag_up/F"    );
      float weight_btag_down=1.;   TBranch* b_weight_btag_down =   otree->Branch("weight_btag_down",   &weight_btag_down,    "weight_btag_down/F"  );
      float weight_btag_LFup=1.;   TBranch* b_weight_btag_LFup =   otree->Branch("weight_btag_LFup",   &weight_btag_LFup,    "weight_btag_LFup/F"  );
      float weight_btag_LFdown=1.; TBranch* b_weight_btag_LFdown = otree->Branch("weight_btag_LFdown", &weight_btag_LFdown,  "weight_btag_LFdown/F");
      float weight_btag_HFup=1.;   TBranch* b_weight_btag_HFup =   otree->Branch("weight_btag_HFup",   &weight_btag_HFup,    "weight_btag_HFup/F"  );
      float weight_btag_HFdown=1.; TBranch* b_weight_btag_HFdown = otree->Branch("weight_btag_HFdown", &weight_btag_HFdown,  "weight_btag_HFdown/F");

      float btag2TSF=1.;  TBranch* b_btag2TSF =   otree->Branch("btag2TSF",      &btag2TSF,        "btag2TSF/F");
      float btag3TSF=1.;  TBranch* b_btag3TSF =   otree->Branch("btag3TSF",      &btag3TSF,        "btag3TSF/F");
      float btag4TSF=1.;  TBranch* b_btag4TSF =   otree->Branch("btag4TSF",      &btag4TSF,        "btag4TSF/F");

      float btag2TSF_up=1.;  TBranch* b_btag2TSF_up =   otree->Branch("btag2TSF_up",      &btag2TSF_up,        "btag2TSF_up/F");
      float btag3TSF_up=1.;  TBranch* b_btag3TSF_up =   otree->Branch("btag3TSF_up",      &btag3TSF_up,        "btag3TSF_up/F");
      float btag4TSF_up=1.;  TBranch* b_btag4TSF_up =   otree->Branch("btag4TSF_up",      &btag4TSF_up,        "btag4TSF_up/F");
      
      float btag2TSF_down=1.;  TBranch* b_btag2TSF_down =   otree->Branch("btag2TSF_down",      &btag2TSF_down,        "btag2TSF_down/F");
      float btag3TSF_down=1.;  TBranch* b_btag3TSF_down =   otree->Branch("btag3TSF_down",      &btag3TSF_down,        "btag3TSF_down/F");
      float btag4TSF_down=1.;  TBranch* b_btag4TSF_down =   otree->Branch("btag4TSF_down",      &btag4TSF_down,        "btag4TSF_down/F");

      // setup input branches needed to fill H->bb branches
      Int_t   njt;             itree->SetBranchAddress("njt",          &njt);
      ULong64_t evt;           itree->SetBranchAddress("evt",          &evt);
      Float_t jtpt[140];       itree->SetBranchAddress("jtpt",         jtpt);
      Float_t jteta[140];      itree->SetBranchAddress("jteta",       jteta);
      Short_t jtgenflv[140];   itree->SetBranchAddress("jtgenflv", jtgenflv);
      Float_t jtcsvt[140];     itree->SetBranchAddress("jtcsvt",     jtcsvt);

      // declare files and fill histograms

      TFile* f_CSVwgt_HF = new TFile ("/portal/ekpcms5/home/bmaier/thanalysis/STANDALONE/THAnalysis/csv_rwt_hf_IT.root");
      TFile* f_CSVwgt_LF = new TFile ("/portal/ekpcms5/home/bmaier/thanalysis/STANDALONE/THAnalysis/csv_rwt_lf_IT.root");

      for( int iSys=0; iSys<9; iSys++ ) {
	TString syst_csv_suffix_hf = "final";
	TString syst_csv_suffix_c = "final";
	TString syst_csv_suffix_lf = "final";
	
	switch( iSys ){
	case 0:
	  // this is the nominal case
	  break;
	case 1:
	  // JESUp
	  syst_csv_suffix_hf = "final_JESUp"; 
	  syst_csv_suffix_lf = "final_JESUp";
	  syst_csv_suffix_c  = "final_cErr1Up";
	  break;
	case 2:
	  // JESDown
	  syst_csv_suffix_hf = "final_JESDown"; 
	  syst_csv_suffix_lf = "final_JESDown";
	  syst_csv_suffix_c  = "final_cErr1Down";
	  break;
	case 3:
	  // purity up
	  syst_csv_suffix_hf = "final_LFUp"; 
	  syst_csv_suffix_lf = "final_HFUp";
	  syst_csv_suffix_c  = "final_cErr2Up";
	  break;
	case 4:
	  // purity down
	  syst_csv_suffix_hf = "final_LFDown"; 
	  syst_csv_suffix_lf = "final_HFDown";
	  syst_csv_suffix_c  = "final_cErr2Down";
	  break;
	case 5:
	  // stats1 up
	  syst_csv_suffix_hf = "final_Stats1Up"; 
	  syst_csv_suffix_lf = "final_Stats1Up";
	  break;
	case 6:
	  // stats1 down
	  syst_csv_suffix_hf = "final_Stats1Down"; 
	  syst_csv_suffix_lf = "final_Stats1Down";
	  break;
	case 7:
	  // stats2 up
	  syst_csv_suffix_hf = "final_Stats2Up"; 
	  syst_csv_suffix_lf = "final_Stats2Up";
	  break;
	case 8:
	  // stats2 down
	  syst_csv_suffix_hf = "final_Stats2Down"; 
	  syst_csv_suffix_lf = "final_Stats2Down";
	  break;
	}
	
	for( int iPt=0; iPt<5; iPt++ ) h_csv_wgt_hf[iSys][iPt] = (TH1D*)f_CSVwgt_HF->Get( Form("csv_ratio_Pt%i_Eta0_%s",iPt,syst_csv_suffix_hf.Data()) );
	   	 
	if( iSys<5 ){
	  for( int iPt=0; iPt<5; iPt++ ) c_csv_wgt_hf[iSys][iPt] = (TH1D*)f_CSVwgt_HF->Get( Form("c_csv_ratio_Pt%i_Eta0_%s",iPt,syst_csv_suffix_c.Data()) );
	}
	
	for( int iPt=0; iPt<3; iPt++ ){
	  for( int iEta=0; iEta<3; iEta++ )h_csv_wgt_lf[iSys][iPt][iEta] = (TH1D*)f_CSVwgt_LF->Get( Form("csv_ratio_Pt%i_Eta%i_%s",iPt,iEta,syst_csv_suffix_lf.Data()) );
	}
      }
      
      string name;
      if(!sample.empty())
	name = sample;
      else
	name = get_name(input);
      
      btagEff2 = new BTagSampleEfficiency2( "/storage/a/singletophiggs/Moriond/addhistos/BTagEff_2012Bravo_v1.0.root",  name);

      // loop over all events and fill H->bb branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {

	itree->GetEntry(ievt);
	

	float sfcsvt[njt];
	float sfcsvt_up[njt];
	float sfcsvt_down[njt];
	float sfcsvt_LFup[njt];
	float sfcsvt_LFdown[njt];
	float sfcsvt_HFup[njt];
	float sfcsvt_HFdown[njt];

	for(int i=0; i< njt; i++){
	  sfcsvt[i]=0.0;
	  sfcsvt_up[i]=0.0;
	  sfcsvt_down[i]=0.0;
	  fillScaleFactors(jtpt[i],jteta[i],jtgenflv[i], sfcsvt[i], 0);
	  fillScaleFactors(jtpt[i],jteta[i],jtgenflv[i], sfcsvt_LFup[i], 2);
	  fillScaleFactors(jtpt[i],jteta[i],jtgenflv[i], sfcsvt_LFdown[i], -2);
	  fillScaleFactors(jtpt[i],jteta[i],jtgenflv[i], sfcsvt_HFup[i], 1);
	  fillScaleFactors(jtpt[i],jteta[i],jtgenflv[i], sfcsvt_HFdown[i], -1);
	  fillScaleFactors(jtpt[i],jteta[i],jtgenflv[i], sfcsvt_up[i], 3);
	  fillScaleFactors(jtpt[i],jteta[i],jtgenflv[i], sfcsvt_down[i], -3);
	  //	  cout << jtgenflv[i] << " : " <<  sfcsvt_down[i] << " " <<  sfcsvt_LFdown[i] << " " << sfcsvt_HFdown[i] << " " <<  sfcsvt[i] <<  " " << sfcsvt_HFup[i] <<  " " << sfcsvt_LFup[i]  <<  " " << sfcsvt_up[i] << endl;
	}

	float dummy1, dummy2, dummy3;
	getBTagWeight( btag2TSF,      btag3TSF ,      btag4TSF,      weight_btag,       njt, jtgenflv, jtpt, jteta, sfcsvt,      jtcsvt);
	getBTagWeight( btag2TSF_up,   btag3TSF_up ,   btag4TSF_up,   weight_btag_up,    njt, jtgenflv, jtpt, jteta, sfcsvt_up,   jtcsvt);
	getBTagWeight( btag2TSF_down, btag3TSF_down , btag4TSF_down, weight_btag_down,  njt, jtgenflv, jtpt, jteta, sfcsvt_down, jtcsvt);
	getBTagWeight( dummy1, dummy2 , dummy3, weight_btag_LFup,    njt, jtgenflv, jtpt, jteta, sfcsvt_LFup,   jtcsvt);
	getBTagWeight( dummy1, dummy2 , dummy3, weight_btag_LFdown,  njt, jtgenflv, jtpt, jteta, sfcsvt_LFdown, jtcsvt);
	getBTagWeight( dummy1, dummy2 , dummy3, weight_btag_HFup,    njt, jtgenflv, jtpt, jteta, sfcsvt_HFup,   jtcsvt);
	getBTagWeight( dummy1, dummy2 , dummy3, weight_btag_HFdown,  njt, jtgenflv, jtpt, jteta, sfcsvt_HFdown, jtcsvt);
	//	cout << " :: " <<  weight_btag_down << " " <<  weight_btag_LFdown << " " << weight_btag_HFdown << " " <<  weight_btag <<  " " << weight_btag_HFup <<  " " << weight_btag_LFup  <<  " " << weight_btag_up << endl;
	


	weight_csv              = 1;
	weight_csv_jesup        = 1;
	weight_csv_jesdown      = 1;
	weight_csv_hfup         = 1;
	weight_csv_hfdown       = 1;
	weight_csv_lfstat1up    = 1;
	weight_csv_lfstat1down  = 1;
	weight_csv_lfstat2up    = 1;
	weight_csv_lfstat2down  = 1;

	weight_csv_lfup         = 1;
	weight_csv_lfdown       = 1;
	weight_csv_hfstat1up    = 1;
	weight_csv_hfstat1down  = 1;
	weight_csv_hfstat2up    = 1;
	weight_csv_hfstat2down  = 1;


	//for( int iSys=0; iSys<9; iSys++ ) {


	//weight_csv              = csvWgtlf*csvWgtC*csvWgthf;


	weight_csv              = getCSVweight(0,0,0,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_jesup        = getCSVweight(1,1,1,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_jesdown      = getCSVweight(2,2,2,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_hfup         = getCSVweight(0,0,3,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_hfdown       = getCSVweight(0,0,4,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_lfstat1up    = getCSVweight(0,0,5,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_lfstat1down  = getCSVweight(0,0,6,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_lfstat2up    = getCSVweight(0,0,7,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_lfstat2down  = getCSVweight(0,0,8,njt,jtgenflv,jtpt,jteta,jtcsvt);	
	weight_csv_lfup         = getCSVweight(3,0,0,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_lfdown       = getCSVweight(4,0,0,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_hfstat1up    = getCSVweight(5,0,0,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_hfstat1down  = getCSVweight(6,0,0,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_hfstat2up    = getCSVweight(7,0,0,njt,jtgenflv,jtpt,jteta,jtcsvt);
	weight_csv_hfstat2down  = getCSVweight(8,0,0,njt,jtgenflv,jtpt,jteta,jtcsvt);	
	

	/*weight_csv_jesup        = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_jesdown      = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_hfup         = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_hfdown       = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_lfstat1up    = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_lfstat1down  = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_lfstat2up    = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_lfstat2down  = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_lfup         = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_lfdown       = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_hfstat1up    = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_hfstat1down  = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_hfstat2up    = csvWgtlf*csvWgtC*csvWgthf;
	weight_csv_hfstat2down  = csvWgtlf*csvWgtC*csvWgthf;
	*/
	b_btag2TSF                 ->Fill();
	b_btag3TSF                 ->Fill();
	b_btag4TSF                 ->Fill();

	b_btag2TSF_up              ->Fill();
	b_btag3TSF_up              ->Fill();
	b_btag4TSF_up              ->Fill();

	b_btag2TSF_down            ->Fill();
	b_btag3TSF_down            ->Fill();
	b_btag4TSF_down            ->Fill();

	b_weight_csv               ->Fill();
	b_weight_csv_jesup         ->Fill();
	b_weight_csv_jesdown       ->Fill();
	b_weight_csv_hfup          ->Fill();
	b_weight_csv_hfdown        ->Fill();
	b_weight_csv_lfstat1up     ->Fill();
	b_weight_csv_lfstat1down   ->Fill();
	b_weight_csv_lfstat2up     ->Fill();
	b_weight_csv_lfstat2down   ->Fill();
	b_weight_csv_lfup          ->Fill();
	b_weight_csv_lfdown        ->Fill();
	b_weight_csv_hfstat1up     ->Fill();
	b_weight_csv_hfstat1down   ->Fill();
	b_weight_csv_hfstat2up     ->Fill();
	b_weight_csv_hfstat2down   ->Fill();
	b_weight_btag              ->Fill();
	b_weight_btag_up           ->Fill();
	b_weight_btag_down         ->Fill();
	b_weight_btag_LFup           ->Fill();
	b_weight_btag_LFdown         ->Fill();
	b_weight_btag_HFup           ->Fill();
	b_weight_btag_HFdown         ->Fill();
	
      } // ievt
      
    } // idir
    
    cout<<"Save extended tree to "<<input<<" ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv " + output + " " + input;
    gSystem->Exec(cmd.c_str());
    cout<<"DONE!\n"<<endl;
    
  } // ifile
  

  return 0;
}

  //
  //
  //

float getCSVweight(int iSysHF,int iSysC, int iSysLF, Int_t njt, Short_t jtgenflv[], Float_t jtpt[], Float_t jteta[], Float_t jtcsvt[])
{
  double csvWgthf = 1.;
  double csvWgtC  = 1.;
  double csvWgtlf = 1.;
  
  for(int i=0; i<njt; i++)
    {
      //cout << "Pt: "<< jtpt[i] <<" Eta: " << jteta[i]<< endl;
      
      int iPt = -1; int iEta = -1;
      if (jtpt[i] >=30.0 && jtpt[i]<40) iPt = 0;
      else if (jtpt[i] >=40 && jtpt[i]<60) iPt = 1;
      else if (jtpt[i] >=60 && jtpt[i]<100) iPt = 2;
      else if (jtpt[i] >=100 && jtpt[i]<160) iPt = 3;
      else if (jtpt[i] >=160 && jtpt[i]<10000) iPt = 4;
      
      
      if (TMath::Abs(jteta[i]) >=0 &&  TMath::Abs(jteta[i])<0.8) iEta = 0;
      else if ( TMath::Abs(jteta[i])>=0.8 && TMath::Abs(jteta[i])<1.6) iEta = 1;
      else if ( TMath::Abs(jteta[i])>=1.6 && TMath::Abs(jteta[i])<2.4) iEta = 2;
      else
	iEta=-1;
      
      
      //if (iPt < 0 || iEta < 0) std::cout << "Error, couldn't find Pt, Eta bins for this b-flavor jet, jetPt = " << jtpt[i] << ", jetAbsEta = " << TMath::Abs(jteta[i]) << std::endl;
      
      
      if(iEta>-1 && iPt>-1){
	if (abs(jtgenflv[i]) == 5 ){
	  int useCSVBin = (jtcsvt[i]>=0.) ? h_csv_wgt_hf[iSysHF][iPt]->FindBin(jtcsvt[i]) : 1;
	  double iCSVWgtHF = h_csv_wgt_hf[iSysHF][iPt]->GetBinContent(useCSVBin);
	  if( iCSVWgtHF!=0 ) csvWgthf *= iCSVWgtHF;
	}
	else if( abs(jtgenflv[i]) == 4 ){
	  int useCSVBin = (jtcsvt[i]>=0.) ? c_csv_wgt_hf[iSysC][iPt]->FindBin(jtcsvt[i]) : 1;
	  double iCSVWgtC = c_csv_wgt_hf[iSysC][iPt]->GetBinContent(useCSVBin);
	  if( iCSVWgtC!=0 ) csvWgtC *= iCSVWgtC;
	}
	else {
	  if (iPt >=2) iPt=2;       /// [30-40], [40-60] and [60-10000] only 3 Pt bins for lf
	  int useCSVBin = (jtcsvt[i]>=0.) ? h_csv_wgt_lf[iSysLF][iPt][iEta]->FindBin(jtcsvt[i]) : 1;
	  double iCSVWgtLF = h_csv_wgt_lf[iSysLF][iPt][iEta]->GetBinContent(useCSVBin);
	  //cout << "jtgenflv " << jtgenflv[i] << endl;
	  //cout << "useCSVBin " << useCSVBin << endl;
	  csvWgtlf *= iCSVWgtLF;
	  //cout << "iCSVWgtLF " << iCSVWgtLF << endl;
	  
	}
      }
      else
	{
	  csvWgtlf *= 1;
	  csvWgtC  *= 1;
	  csvWgthf *= 1;
	}
    } // jt loop
  return csvWgtlf*csvWgtC*csvWgthf;
}

//_____________________________________________________ 
void getBTagWeight(float& btag2, float& btag3, float& btag4, float& btagweight, Int_t njt, Short_t jtgenflv[], Float_t jtpt[], Float_t jteta[], Float_t sft[], Float_t csvt[])
{
  std::vector<std::vector<BTagWeight2::JetInfo> > btagJetInfos;
  
  double weight_btag=1.0;
  for(int i=0; i<njt; i++)
    {
      if(fabs(jteta[i])<2.4)
	btagJetInfos.push_back(btagEff2->jetInfo(jtgenflv[i], jtpt[i], jteta[i], sft[i], csvt[i]));
    }
  
  BTagWeight2 btag(1);
  
  weight_btag = btag.CalcWeight(btagJetInfos); 
  btag2 =  (njt<11) ? btag.weight<BTag2TightFilter2>(btagJetInfos) : 1.0;
  btag3 =  (njt<11) ? btag.weight<BTag3TightFilter2>(btagJetInfos) : 1.0;
  btag4 =  (njt<11) ? btag.weight<BTag4TightFilter2>(btagJetInfos) : 1.0;
  //    cout << btag2 << " "<< btag3 << " "<<btag4 << endl;
  btagweight = weight_btag;

  return;
}


//_____________________________________________________ 
void fillScaleFactors(float pt, float eta, int flv, Float_t& csvt, int var){

  double uncFactor = 1.;
  if(abs(eta)<2.4){
    if(TMath::Abs(flv) == 4 or TMath::Abs(flv) == 5){
      if (pt < 20.)
	{
	  pt = 20.;
	  uncFactor *= 2;
	}
      else if (pt > 800.)
	{
	  pt = 800.;
	  uncFactor *= 2;
	}
      
      double const nominalSF = GetSFTagCSVT(pt);
      if (var== 0 || abs(var) == 2 ){
	csvt = nominalSF;
	return;
      }
      
      if (abs(flv)  == 4)
	uncFactor *= 2;
      
      double const uncertainty = GetSFTagUncCSVT(pt);
      
      if(var == 1 || var == 3){
	csvt = nominalSF + uncertainty * uncFactor;
	return;
      }
      if(var == -1 || var ==-3){
	csvt = nominalSF - uncertainty * uncFactor;
	return;
      }
      cout << "ERROR!!! \n" << endl;
      return;
      
    }
    // MISTAG
    else{
      if (pt < 20.){
	pt = 20.;
	uncFactor *= 2;
      }
      else if (pt > 1000.)
	{
	  pt = 1000.;
	  uncFactor *= 2;
	}
      double const nominalSF = GetSFMistagCSVTMean(pt, eta);
      if (var == 0 || abs(var) == 1){
	csvt = nominalSF;
      return;
      }
      
      if (var == 2 || var == 3)
	{
	  double const upSF = GetSFMistagCSVTMax(pt, eta);
	  csvt = nominalSF + (upSF - nominalSF) * uncFactor;
	  return;
	}
      if(var == -2 || var == -3)      
	{
	  double const downSF = GetSFMistagCSVTMin(pt, eta);
	  csvt = nominalSF - (nominalSF - downSF) * uncFactor;
	  return;
	}
      
      cout << "ERROR!!! \n" << endl;
      return;
    }
  }
  else {
    csvt = 1.0;
    return;
  }
}


string get_name(string sample)
{
  string name=sample;
  if(name.find("Jets-madgraph")!=string::npos)return "Wjets";
  else if(name.find("pythia6-Summer12")!=string::npos)return "Wjets";
  else if(name.find("s-powheg")!=string::npos)return "t-schan";
  else if(name.find("t-powheg")!=string::npos)return "t-tchan";
  else if(name.find("tW-powheg")!=string::npos)return "t-tWchan";
  else if(name.find("TH-SM-madgraph")!=string::npos)return "tHq-sm";
  else if(name.find("TH-madgraph")!=string::npos)return "tHq-nc";
  else if(name.find("TTH")!=string::npos)return "ttH";
  else if(name.find("FullLept")!=string::npos)return "ttbar-dilep";
  else if(name.find("SemiLept")!=string::npos)return "ttbar-semilep";
  else if(name.find("TTbar-madgraph")!=string::npos)return "ttbar-inc";
  else return "ttbar-inc";
}

