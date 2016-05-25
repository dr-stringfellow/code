////////////////////////////////////////////////////////////////////////////////
//
// th_addvariables_x
// -----------------
//
//            01/10/2013 Benedikt Maier <bmaier@cern.ch>
//                                                    
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"
#include "../src/Math.h"
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
  bool           clean       = cl.getValue<bool>   ("clean",   false);

  if (!cl.check()) return 0;
  cl.print();

  
  //
  // loop over all input files
  //
  for (unsigned iinput=0;iinput<inputs.size();iinput++) {
  
    // open input file
    string input = inputs[iinput]; 
    TFile* ifile = new TFile(input.c_str(),"READ");
    if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

    // create temporary output file
    size_t pos = input.find(".root");
    string output = input.substr(0,pos);
    output = output + ".var.root";
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
      


      // don't copy branches if they already exist
      if (0!=itree->FindBranch("jtgenpt_2"))      itree->SetBranchStatus("jtgenpt_2",     0);
      if (0!=itree->FindBranch("jtgeneta_2"))      itree->SetBranchStatus("jtgeneta_2",     0);
      if (0!=itree->FindBranch("jtgenphi_2"))      itree->SetBranchStatus("jtgenphi_2",     0);
      if (0!=itree->FindBranch("btopidx"))      itree->SetBranchStatus("btopidx",     0);
      if (0!=itree->FindBranch("sbidx"))      itree->SetBranchStatus("sbidx",     0);
      if (0!=itree->FindBranch("lqjtidx"))      itree->SetBranchStatus("lqjtidx",     0);
 
      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;

      // create branches in output tree
      
      Int_t o_ngenjt_2=0;
      Float_t o_jtgenpt_2[100];
      Float_t o_jtgeneta_2[100];
      Float_t o_jtgenphi_2[100];
      Int_t o_jtgenconst_2[100];
      Int_t o_btopjtidx;
      Int_t o_sbjtidx;
      Int_t o_lqjtidx;

      TBranch* b_ngenjt_2             =otree->Branch("ngenjt_2",        &o_ngenjt_2,             "ngenjt_2/I");
      TBranch* b_jtgenpt_2            =otree->Branch("jtgenpt_2",       o_jtgenpt_2,             "jtgenpt_2[ngenjt_2]/F");
      TBranch* b_jtgeneta_2           =otree->Branch("jtgeneta_2",      o_jtgeneta_2,            "jtgeneta_2[ngenjt_2]/F");
      TBranch* b_jtgenphi_2           =otree->Branch("jtgenphi_2",      o_jtgenphi_2,            "jtgenphi_2[ngenjt_2]/F");
      TBranch* b_jtgenconst_2         =otree->Branch("jtgenconst_2",    o_jtgenconst_2,          "jtgenconst_2[ngenjt_2]/I");
      TBranch* b_btopjtidx            =otree->Branch("btopjtidx",       &o_btopjtidx,            "btopjtidx/I");
      TBranch* b_sbjtidx              =otree->Branch("sbjtidx",         &o_sbjtidx,              "sbjtidx/I");
      TBranch* b_lqjtidx              =otree->Branch("lqjtidx",         &o_lqjtidx,              "lqjtidx/I");

     
      // setup input branches needed to fill H->bb branches
      
      
      // parton quantities      
      Float_t i_btopphi;               itree->SetBranchAddress("btopphi",           &i_btopphi);
      Float_t i_btopeta;               itree->SetBranchAddress("btopeta",           &i_btopeta); 
      Float_t i_sbphi;                 itree->SetBranchAddress("sbphi",             &i_sbphi);
      Float_t i_sbeta;                 itree->SetBranchAddress("sbeta",             &i_sbeta);
      Float_t i_lqphi;                 itree->SetBranchAddress("lqphi",             &i_lqphi);
      Float_t i_lqeta;                 itree->SetBranchAddress("lqeta",             &i_lqeta);

      // jet quantities
      Int_t   i_ngenjt;                itree->SetBranchAddress("ngenjt",        &i_ngenjt);
      Float_t i_jtgenpt[140];          itree->SetBranchAddress("jtgenpt",        i_jtgenpt);
      Float_t i_jtgenphi[140];         itree->SetBranchAddress("jtgenphi",       i_jtgenphi);
      Float_t i_jtgeneta[140];         itree->SetBranchAddress("jtgeneta",       i_jtgeneta);
      Int_t i_jtgenconst[140];         itree->SetBranchAddress("jtgenconst",     i_jtgenconst);

            
      // loop over all events and fill H->bb branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {

	itree->GetEntry(ievt);
	
	
	o_ngenjt_2=0;

	o_btopjtidx=-99;
	o_sbjtidx=-99;
	o_lqjtidx=-99;

	Float_t deltaR_btop=0.3;
	Float_t deltaR_sb=0.3;
	Float_t deltaR_lq=0.3;

	
	for(Int_t i = 0; i<i_ngenjt; i++)
	  {

	    if(i_jtgenpt[i]>10 && TMath::Abs(i_jtgeneta[i])<4.7 && (deltaR(i_jtgeneta[i],i_jtgenphi[i],i_btopeta,i_btopphi)>0.3 || deltaR(i_jtgeneta[i],i_jtgenphi[i],i_sbeta,i_sbphi)>0.3 || deltaR(i_jtgeneta[i],i_jtgenphi[i],i_lqeta,i_lqphi)>0.3))
	      {
		o_jtgenpt_2[o_ngenjt_2]=i_jtgenpt[i];
		o_jtgeneta_2[o_ngenjt_2]=i_jtgeneta[i];
		o_jtgenphi_2[o_ngenjt_2]=i_jtgenphi[i];
		o_jtgenconst_2[o_ngenjt_2]=i_jtgenconst[i];
		o_ngenjt_2++;
	      }

	    if(i_jtgenpt[i]>10 && TMath::Abs(i_jtgeneta[i])<4.7 && (deltaR(i_jtgeneta[i],i_jtgenphi[i],i_btopeta,i_btopphi)<deltaR_btop))
	      {
		o_btopjtidx=i;
		deltaR_btop=deltaR(i_jtgeneta[i],i_jtgenphi[i],i_btopeta,i_btopphi);
	      }

            if(i_jtgenpt[i]>10 && TMath::Abs(i_jtgeneta[i])<4.7 && (deltaR(i_jtgeneta[i],i_jtgenphi[i],i_sbeta,i_sbphi)<deltaR_sb))
	      {
		o_sbjtidx=i;
		deltaR_sb=deltaR(i_jtgeneta[i],i_jtgenphi[i],i_sbeta,i_sbphi);
	      }

	    if(i_jtgenpt[i]>10 && TMath::Abs(i_jtgeneta[i])<4.7 && (deltaR(i_jtgeneta[i],i_jtgenphi[i],i_lqeta,i_lqphi)<deltaR_lq))
	      {
		o_lqjtidx=i;
		deltaR_lq=deltaR(i_jtgeneta[i],i_jtgenphi[i],i_lqeta,i_lqphi);
	      }
	    
	  }

		
	
	b_ngenjt_2            ->Fill();
	b_jtgenpt_2           ->Fill();
	b_jtgeneta_2          ->Fill();
	b_jtgenphi_2          ->Fill();
	b_jtgenconst_2        ->Fill();
	b_btopjtidx           ->Fill();
	b_sbjtidx             ->Fill();
	b_lqjtidx             ->Fill();


      } // ievt

    } // idir
    
    cout<<"Save extended tree to "<<input<<" ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv -f " + output + " " + input;
    gSystem->Exec(cmd.c_str());
    cout<<"DONE!\n"<<endl;
    
  } // ifile
  

  return 0;
}




