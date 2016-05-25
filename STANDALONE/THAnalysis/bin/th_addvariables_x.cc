////////////////////////////////////////////////////////////////////////////////
//
// th_addvariables_x
// -----------------
//
//            01/10/2013 Benedikt Maier <bmaier@cern.ch>
//                                                    
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"
#include "../interface/Math.h"
//#include "SchieferD/HiggsAnalysis/interface/VHBBMultiThresholdEfficiency.h"
//#include "../src/Weights.cc"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TEventList.h>
#include <TLorentzVector.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
////////////////////////////////////////////////////////////////////////////////
//-------



//Sorting
struct GreaterByCSV{
    float * csv;
    
    GreaterByCSV(float * csv_): csv(csv_){}
    
    bool operator()(size_t i, size_t j) const{
        return csv[i] > csv[j];
       }
   };

struct GreaterByEta{
    float * eta;
    
    GreaterByEta(float * eta_): eta(eta_){}
    
    bool operator()(size_t i, size_t j) const{
        return eta[i] > eta[j];
       }
   };

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
  string         treename    = cl.getValue<string> ("treename",  "t");
  //string         datapath    = cl.getValue<string> ("datapath",  "/");
  //string         mode_as_str = cl.getValue<string> ("mode","dijetpt");
  bool           clean       = cl.getValue<bool>   ("clean",   false);
  Int_t          isMu        = cl.getValue<int>    ("isMu",    -1);
  //bool           isSemi      = cl.getValue<bool>   ("isSemi",  false);

  if (!cl.check()) return 0;
  cl.print();

  //
  // decode mode
  //
  
  
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
    output = output + ".var.root";
    TFile* ofile = new TFile(output.c_str(),"RECREATE");
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
      
      // check whether relevant branches are present
      if (0==itree->FindBranch("nlepw")){ cout<<"no W->lnu branches stored!"<<endl; return 0; }
      //if (0==itree->FindBranch("nlepz")){ cout<<"no Z->ll branches stored!"<<endl; return 0; }

      // check whether sample is considered MC (weights to be stored)
      bool isMC = itree->FindBranch("npv");
      if (isMC) cout<<"Treating as MC"<<endl; else cout<<"Treating as DATA"<<endl;


      // don't copy H->bb branches if they already exist
      if (0!=itree->FindBranch("costhetastar"))      itree->SetBranchStatus("costhetastar",     0);
      if (0!=itree->FindBranch("costhetastarhiggs")) itree->SetBranchStatus("costhetastarhiggs",0);
      if (0!=itree->FindBranch("coszetastar"))       itree->SetBranchStatus("coszetastar",      0);
      if (0!=itree->FindBranch("twist"))             itree->SetBranchStatus("twist",            0);
      if (0!=itree->FindBranch("helicity_h1"))       itree->SetBranchStatus("helicity_h1",      0);
      if (0!=itree->FindBranch("helicity_h2"))       itree->SetBranchStatus("helicity_h2",      0);
      if (0!=itree->FindBranch("helicity_ljt"))      itree->SetBranchStatus("helicity_ljt",     0);
      if (0!=itree->FindBranch("helicity_topjt"))    itree->SetBranchStatus("helicity_topjt",   0);
      if (0!=itree->FindBranch("taggerpos"))         itree->SetBranchStatus("taggerpos",        0);
      if (0!=itree->FindBranch("taggerpos_new"))     itree->SetBranchStatus("taggerpos_new",    0);
      if (0!=itree->FindBranch("flavor_separator"))     itree->SetBranchStatus("flavor_separator",    0);
      if (0!=itree->FindBranch("tophadjtidx"))     itree->SetBranchStatus("tophadjtidx",    0);
      if (0!=itree->FindBranch("toplepjtidx"))     itree->SetBranchStatus("toplepjtidx",    0);
      if (0!=itree->FindBranch("etapos"))            itree->SetBranchStatus("etapos",           0);
      if (0!=itree->FindBranch("jtcsvt"))            itree->SetBranchStatus("jtcsvt",           0);
      if (0!=itree->FindBranch("njt"))             itree->SetBranchStatus("njt",            0);
      if (0!=itree->FindBranch("ismu"))              itree->SetBranchStatus("ismu",             0);


 
      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;

      itree->SetBranchStatus("jtcsvt",    1);
      itree->SetBranchStatus("njt",       1);


      // if cleaning is requested, nothing further!
      if (clean) continue;
      
      // create H->bb branches in output tree
       
      Float_t costhetastar;
      Float_t costhetastarhiggs;
      Float_t coszetastar;
      Float_t twist;
      Float_t helicity_h1;
      Float_t helicity_h2;
      Float_t helicity_ljt;
      Float_t helicity_topjt;
     Float_t delta_R_1jet2jet;
      Float_t dR_reco_not_tjets;
      Float_t jtcsvt[140];
      Int_t   taggerpos[140];
      Int_t   taggerpos_new[140];
      Int_t   etapos[140];
      Int_t njt=0;
      Int_t ismu=-1;
      Int_t flavor_separator=-999; // number jets and 3rd and 4th highest b-tag -> variable for ttbar separation based on flavor
      Int_t tophadjtidx[3];
      Int_t toplepjtidx;
      
      TBranch* b_njt                 =otree->Branch("njt",       &njt,    "njt/I");
      TBranch* b_flavor_separator                 =otree->Branch("flavor_separator",       &flavor_separator,    "flavor_separator/I");
     TBranch* b_delta_R_1jet2jet        =otree->Branch("delta_R_1jet2jet",      &delta_R_1jet2jet,           "delta_R_1jet2jet/F");
      TBranch* b_dR_reco_not_tjets        =otree->Branch("dR_reco_not_tjets",      &dR_reco_not_tjets,           "dR_reco_not_tjets/F");
      TBranch* b_toplepjtidx       =otree->Branch("toplepjtidx",     &toplepjtidx,     "toplepjtidx/I");
      /*
      TBranch* b_costhetastar        =otree->Branch("costhetastar",      &costhetastar,           "costhetastar/F");
      TBranch* b_costhetastarhiggs   =otree->Branch("costhetastarhiggs", &costhetastarhiggs, "costhetastarhiggs/F");
      TBranch* b_coszetastar         =otree->Branch("coszetastar",       &coszetastar,             "coszetastar/F");
      TBranch* b_twist               =otree->Branch("twist",             &twist,                         "twist/F");
      TBranch* b_helicity_h1         =otree->Branch("helicity_h1",       &helicity_h1,             "helicity_h1/F");
      TBranch* b_helicity_h2         =otree->Branch("helicity_h2",       &helicity_h2,             "helicity_h2/F");
      TBranch* b_helicity_ljt        =otree->Branch("helicity_ljt",      &helicity_ljt,           "helicity_ljt/F");
      TBranch* b_helicity_topjt      =otree->Branch("helicity_topjt",    &helicity_topjt,       "helicity_topjt/F");
      */
      TBranch* b_taggerpos           =otree->Branch("taggerpos",         taggerpos,             "taggerpos[140]/I");
      TBranch* b_taggerpos_new       =otree->Branch("taggerpos_new",     taggerpos_new,     "taggerpos_new[140]/I");
      TBranch* b_tophadjtidx       =otree->Branch("tophadjtidx",     tophadjtidx,     "tophadjtidx[3]/I");
      TBranch* b_etapos              =otree->Branch("etapos",            etapos,                   "etapos[140]/I");
      TBranch* b_jtcsvt              =otree->Branch("jtcsvt",            jtcsvt,                 "jtcsvt[njt]/F");
      
      TBranch* b_ismu                =otree->Branch("ismu",       &ismu,    "ismu/I");

      // setup input branches needed to fill H->bb branches
      
      Int_t nlepw;
      Float_t lepwphi;
      Float_t lepweta;
      Float_t lepwpt;
      Float_t lepwm;
      
      Float_t topphi;
      Float_t topeta;
      Float_t toppt;
      Float_t topm;
      
      Float_t hbbm;
      Float_t hbbpt;
      Float_t hbbeta;
      Float_t hbbphi;
     
      
   

      

      //Float_t jtcsvtcorr[40][5];       itree->SetBranchAddress("jtcsvtcorr", jtcsvtcorr);
      Float_t i_jtcsvt[140];           itree->SetBranchAddress("jtcsvt",       i_jtcsvt);     
      UInt_t i_njt;                   itree->SetBranchAddress("njt",        &i_njt);
     
      
      Float_t jtpt[40]={-99};          itree->SetBranchAddress("jtpt",             jtpt);
      Float_t jteta[40]={-99};         itree->SetBranchAddress("jteta",           jteta);
      Float_t jtphi[40]={-99};         itree->SetBranchAddress("jtphi",           jtphi);
      Float_t jte[40]={-99};           itree->SetBranchAddress("jte",               jte);
      
      Int_t hbbjtidx[3];               itree->SetBranchAddress("hbbjtidx",     hbbjtidx);
      Int_t topjtidx;                  itree->SetBranchAddress("topjtidx",    &topjtidx);
      //Int_t ljtidx;                    itree->SetBranchAddress("ljtidx",        &ljtidx);
      
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
      

      itree->SetBranchAddress("nlepw",   &nlepw);
      itree->SetBranchAddress("lepwphi",  &lepwphi);
      itree->SetBranchAddress("lepweta",  &lepweta);
      itree->SetBranchAddress("lepwpt",  &lepwpt);
      itree->SetBranchAddress("lepwm",  &lepwm);
      itree->SetBranchAddress("topphi",  &topphi);
      itree->SetBranchAddress("topeta",  &topeta);
      itree->SetBranchAddress("toppt",  &toppt);
      itree->SetBranchAddress("topm",  &topm);
      itree->SetBranchAddress("hbbm",  &hbbm);
      itree->SetBranchAddress("hbbpt",  &hbbpt);
      itree->SetBranchAddress("hbbeta",  &hbbeta);
      itree->SetBranchAddress("hbbphi",  &hbbphi);
      
    
      
      
       
      // loop over all events and fill H->bb branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      Float_t csv_temp[140];
      Float_t csv_new_temp[140];
      Float_t eta_temp[140];
      
      for (int ievt=0;ievt<nevt;ievt++) {
	if(ievt%10000==0)
	  cout << "Processing event #" << ievt << endl;
	
	itree->GetEntry(ievt);
	


	cout << "njt: " << i_njt << endl;


	for(int i=0; i<i_njt; i++)
	  {
	    //	    cout << "test: " << i << endl;
	    if(TMath::Abs(jteta[i])<2.4 && i_jtcsvt[i]>0.){
	      csv_temp[i]=-999; // jtcsvtcorr[i][0]; FIXME
	      csv_new_temp[i]=i_jtcsvt[i];
	    }
	    else if(TMath::Abs(jteta[i])<2.4 && i_jtcsvt[i]<0.){
	      csv_temp[i]=-999;
	      csv_new_temp[i]=-999;
	    }
	    else {
	      csv_temp[i]=-999;
	      csv_new_temp[i]=-999;
	    }
	    eta_temp[i]=TMath::Abs(jteta[i]);
	  }



	for(int i=0; i<i_njt; i++)
	  {
	    jtcsvt[i]=csv_new_temp[i];
	    
	    //if(TMath::Abs(jteta[i])>2.4)
	      //cout << "Uncorr: " << i_jtcsvt[i] << " temp: " << csv_new_temp[i] << " final: " << jtcsvt[i] << endl;
	      //else cout  << "Uncorr: " << i_jtcsvt[i] << " temp: " << csv_new_temp[i] << " final: " << jtcsvt[i] << endl;
	  }

	vector<size_t> taggerindices;
	vector<size_t> taggerindices_new;
	vector<size_t> etaindices;
	taggerindices.resize(i_njt);
	taggerindices_new.resize(i_njt);
	etaindices.resize(i_njt);
	
	for(Int_t i=0; i<i_njt; ++i){
	  taggerindices[i] = i;
	  taggerindices_new[i] = i;
	  etaindices[i] = i;
	}


	sort(taggerindices.begin(), taggerindices.end(), GreaterByCSV(csv_temp));
	sort(taggerindices_new.begin(), taggerindices_new.end(), GreaterByCSV(csv_new_temp));
	sort(etaindices.begin(), etaindices.end(), GreaterByEta(eta_temp));
	
        for(int i=0; i<140; i++)
        {
          if(i<i_njt)
          {
            taggerpos[i]=taggerindices[i];
            taggerpos_new[i]=taggerindices_new[i];
            etapos[i]=etaindices[i];
          }
          else
          {
            taggerpos[i]=-1;
            taggerpos_new[i]=-1;
            etapos[i]=-1;
          }
        }

	njt=i_njt;
	
	ismu = isMu;
	
	
	//Delta R between 1st highest jet and 2nd highest jet (pT)
	delta_R_1jet2jet = deltaR(jteta[0], jtphi[0], jteta[1], jtphi[1]);
        
        int njt_category;
        
        if(njt<=5) njt_category = 0;
        else if(njt<=7) njt_category = 1;
        else njt_category = 2;
        
        int tag3_category;
        
        if(jtcsvt[taggerpos_new[2]]<=0.3) tag3_category = 0;
        else if(jtcsvt[taggerpos_new[2]]<=0.7) tag3_category = 1;
        else tag3_category = 2;
        
        int tag4_category;
        
        if(jtcsvt[taggerpos_new[3]]<=0.3) tag4_category = 0;
        else if(jtcsvt[taggerpos_new[3]]<=0.6) tag4_category = 1;
        else tag4_category = 2;
        
        flavor_separator = njt_category + 3*tag3_category + 9*tag4_category;
        
        
//         if(njt<=5)
        dR_reco_not_tjets = -8.;
//         else if(
        
        
	
	b_jtcsvt              ->Fill();
	b_njt                 ->Fill();
	b_ismu                ->Fill();
	b_delta_R_1jet2jet    ->Fill();
        b_dR_reco_not_tjets   ->Fill();
	/*
	b_helicity_h1         ->Fill();
	b_helicity_h2         ->Fill();
	b_helicity_ljt        ->Fill();
	b_helicity_topjt      ->Fill();
	b_twist               ->Fill();
	b_coszetastar         ->Fill();
	b_costhetastar        ->Fill();
	b_costhetastarhiggs   ->Fill();
	*/
	
	b_taggerpos           ->Fill();
	b_taggerpos_new       ->Fill();
	b_etapos              ->Fill();
        b_flavor_separator    ->Fill();
        b_tophadjtidx         ->Fill();
        b_toplepjtidx         ->Fill();

      } // ievt

    } // idir
    
    cout<<"Save extended tree to "<<output<<" ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
//    string cmd = "mv -f " + output + " " + input;
//    gSystem->Exec(cmd.c_str());
    cout<<"DONE!\n"<<endl;
    
  } // ifile
  

  return 0;
}


