////////////////////////////////////////////////////////////////////////////////
//
// th_scalejes_x
// --------------
// scale hbb jet branches according to JES systematics
//
//                              14/11/2011             Hauke Held <held@cern.ch>
////////////////////////////////////////////////////////////////////////////////

#include "../interface/CommandLine.h"
#include "../interface/THStatus.h"
#include "../../JetMETObjects/interface/JetCorrectionUncertainty.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TLorentzVector.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cassert>

using namespace std;


struct GreaterByPt{
  float * pts;

  GreaterByPt(float * pts_): pts(pts_){}

  bool operator()(size_t i, size_t j) const{
    return pts[i] > pts[j];
  }
};

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  //5
  // evaluate command line options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs    = cl.getVector<string>("inputs");
  string         datapath  = cl.getValue<string> ("datapath",               ".");
  string         treename  = cl.getValue<string> ("treename",               "t");
  string         opath     = cl.getValue<string> ("opath",                  ".");
  string         output    = cl.getValue<string> ("output",                  "");
  int            systematic= cl.getValue<int>    ("systematic",               0);
  string         jesfile   = cl.getValue<string> ("jesfile",                 "");
  bool           bflav     = cl.getValue<bool>   ("bflav",                false);

  if (!cl.check()) return 0;
  cl.print();

  if      (-1==systematic) cout<<"Down type systematic scaling!"<<endl;
  else if (+1==systematic) cout<<"Up type systematic scaling!"<<endl;
  else               {cout<<"Error in variable systematic: "<<systematic<<endl;return 0;}

  // setup the standalone jet correction service

  JetCorrectionUncertainty* jesUnc = 
      new JetCorrectionUncertainty(jesfile); 

  //
  // loop over all input files
  //
  for (unsigned iinput=0;iinput<inputs.size();iinput++) {
  
    // open input file
    string input = inputs[iinput]; input = datapath + "/" + input;
    TFile* ifile = new TFile(input.c_str(),"READ");
    if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

    // create temporary output file
    if (output.empty()) output = input.substr(0,input.find(".root"));
    else output = output.substr(0,output.find(".root"));
    if (!opath.empty()){
      if (output.find("/")==string::npos)
	output = opath + "/" + output;
      else
	output = opath + output.substr(output.find_last_of("/"));
    }
    output = output + ".jes.root";
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

     
      // don't copy any jet or met branches if they exist
      
      // jt:
      if (0!=itree->FindBranch("jtpt"))     itree->SetBranchStatus("jtpt",       0);
      if (0!=itree->FindBranch("jte"))      itree->SetBranchStatus("jte",        0);
      if (0!=itree->FindBranch("jtjes"))    itree->SetBranchStatus("jtjes",      0);
      if (0!=itree->FindBranch("jteta"))    itree->SetBranchStatus("jteta",      0);
      if (0!=itree->FindBranch("jtphi"))    itree->SetBranchStatus("jtphi",      0);

      if (0!=itree->FindBranch("jtrawpt"))               itree->SetBranchStatus("jtrawpt",0);
      if (0!=itree->FindBranch("jtcsvt"))               itree->SetBranchStatus("jtcsvt",0);
      if (0!=itree->FindBranch("jtntracks"))               itree->SetBranchStatus("jtntracks",0);
      if (0!=itree->FindBranch("jtarea"))               itree->SetBranchStatus("jtarea",0);
      if (0!=itree->FindBranch("jtpull"))               itree->SetBranchStatus("jtpull",0);
      if (0!=itree->FindBranch("jtcharge"))               itree->SetBranchStatus("jtcharge",0);
      if (0!=itree->FindBranch("jtid"))               itree->SetBranchStatus("jtid",0);
      if (0!=itree->FindBranch("jtchhadmulti"))               itree->SetBranchStatus("jtchhadmulti",0);
      if (0!=itree->FindBranch("jtgenflv"))               itree->SetBranchStatus("jtgenflv",0);
      if (0!=itree->FindBranch("jtgenpt"))               itree->SetBranchStatus("jtgenpt",0);
      if (0!=itree->FindBranch("jtgeneta"))               itree->SetBranchStatus("jtgeneta",0);
      if (0!=itree->FindBranch("jtgenphi"))               itree->SetBranchStatus("jtgenphi",0);
      if (0!=itree->FindBranch("jtgene"))               itree->SetBranchStatus("jtgene",0);

      if (0!=itree->FindBranch("njt"))               itree->SetBranchStatus("njt",0);
      if (0!=itree->FindBranch("njt15"))               itree->SetBranchStatus("njt15",0);


      // copy the tree
      odir->cd();
      cout<<"Cloning tree in "<<input<<" ==> "<<output<<" ... "<<flush;
      TTree* otree = itree->CloneTree();
      cout<<"done.\n";

      // set branch stati valid again...!
      
      // jt:
      if (0!=itree->FindBranch("jtpt"))     itree->SetBranchStatus("jtpt",       1);
      if (0!=itree->FindBranch("jte"))      itree->SetBranchStatus("jte",        1);
      if (0!=itree->FindBranch("jteta"))    itree->SetBranchStatus("jteta",      1);
      if (0!=itree->FindBranch("jtphi"))    itree->SetBranchStatus("jtphi",      1);

      if (0!=itree->FindBranch("jtrawpt"))               itree->SetBranchStatus("jtrawpt",1);
      if (0!=itree->FindBranch("jtcsvt"))               itree->SetBranchStatus("jtcsvt",1);
      if (0!=itree->FindBranch("jtntracks"))               itree->SetBranchStatus("jtntracks",1);
      if (0!=itree->FindBranch("jtarea"))               itree->SetBranchStatus("jtarea",1);
      if (0!=itree->FindBranch("jtpull"))               itree->SetBranchStatus("jtpull",1);
      if (0!=itree->FindBranch("jtcharge"))               itree->SetBranchStatus("jtcharge",1);
      if (0!=itree->FindBranch("jtid"))               itree->SetBranchStatus("jtid",1);
      if (0!=itree->FindBranch("jtchhadmulti"))               itree->SetBranchStatus("jtchhadmulti",1);
      if (0!=itree->FindBranch("jtgenflv"))               itree->SetBranchStatus("jtgenflv",1);
      if (0!=itree->FindBranch("jtgenpt"))               itree->SetBranchStatus("jtgenpt",1);
      if (0!=itree->FindBranch("jtgeneta"))               itree->SetBranchStatus("jtgeneta",1);
      if (0!=itree->FindBranch("jtgenphi"))               itree->SetBranchStatus("jtgenphi",1);
      if (0!=itree->FindBranch("jtgene"))               itree->SetBranchStatus("jtgene",1);
      if (0!=itree->FindBranch("njt"))               itree->SetBranchStatus("njt",1);
      if (0!=itree->FindBranch("njt15"))               itree->SetBranchStatus("njt15",1);
 
      // create the branches in the output tree
      
      Float_t o_jtpt[100];
      Float_t o_jteta[100];
      Float_t o_jtphi[100];
      Float_t o_jte[100];
      Float_t o_jtjes[100];

      Float_t o_jtrawpt[100];
      Float_t o_jtcsvt[100];
      Float_t o_jtntracks[100];
      Float_t o_jtarea[100];
      Float_t o_jtpull[100];
      Float_t o_jtcharge[100];
      Bool_t  o_jtid[100];
      Float_t o_jtchhadmult[100];
      Short_t o_jtgenflv[100];
      Float_t o_jtgenpt[100];
      Float_t o_jtgeneta[100];
      Float_t o_jtgenphi[100];
      Float_t o_jtgene[100];
      Int_t  o_njt=0;
      Int_t  o_njt15=0;


      // setup input branches
      
      Float_t i_jtpt[100];        itree->SetBranchAddress("jtpt",   i_jtpt);  
      Float_t i_jteta[100];       itree->SetBranchAddress("jteta",  i_jteta);  
      Float_t i_jtphi[100];       itree->SetBranchAddress("jtphi",  i_jtphi);  
      Float_t i_jte[100];         itree->SetBranchAddress("jte",    i_jte); 
     
      Int_t njt15=0;          itree->SetBranchAddress("njt15",           &njt15);

      //itree->SetBranchAddress("jtjes",  jtjes); 

      Float_t i_jtrawpt[100]={-99};        itree->SetBranchAddress("jtrawpt",           i_jtrawpt);
      Float_t i_jtcsvt[100]={-99};        itree->SetBranchAddress("jtcsvt",           i_jtcsvt);
      Float_t i_jtntracks[100]={-99};        itree->SetBranchAddress("jtntracks",           i_jtntracks);
      Float_t i_jtarea[100]={-99};        itree->SetBranchAddress("jtarea",           i_jtarea);
      //Float_t i_jtjes[100]={-99};        itree->SetBranchAddress("jtjes",           i_jtjes);
      Float_t i_jtpull[100]={-99};        itree->SetBranchAddress("jtpull",           i_jtpull);
      Float_t i_jtcharge[100]={-99};        itree->SetBranchAddress("jtcharge",           i_jtcharge);
      Bool_t  i_jtid[100]={-99};        itree->SetBranchAddress("jtid",           i_jtid);
      Float_t i_jtchhadmult[100]={-99};        itree->SetBranchAddress("jtchhadmult",           i_jtchhadmult);
      Short_t i_jtgenflv[100]={-99};        itree->SetBranchAddress("jtgenflv",           i_jtgenflv);
      Float_t i_jtgenpt[100]={-99};        itree->SetBranchAddress("jtgenpt",           i_jtgenpt);
      Float_t i_jtgeneta[100]={-99};        itree->SetBranchAddress("jtgeneta",           i_jtgeneta);
      Float_t i_jtgenphi[100]={-99};        itree->SetBranchAddress("jtgenphi",           i_jtgenphi);
      Float_t i_jtgene[100]={-99};        itree->SetBranchAddress("jtgene",           i_jtgene);

      // setup input branches
  
      TBranch* b_jtpt = otree->Branch("jtpt",       o_jtpt,        "jtpt[njt15]/F");
      TBranch* b_jte = otree->Branch("jte",        o_jte,          "jte[njt15]/F");
      TBranch* b_jteta = otree->Branch("jteta",      o_jteta,      "jteta[njt15]/F");
      TBranch* b_jtphi = otree->Branch("jtphi",      o_jtphi,      "jtphi[njt15]/F");
      TBranch* b_jtjes = otree->Branch("jtjes",      o_jtjes,      "jtjes[njt15]/F");

      TBranch* b_jtrawpt = otree->Branch("jtrawpt",         o_jtrawpt,           "jtrawpt[njt15]/F");
      TBranch* b_jtcsvt = otree->Branch("jtcsvt",          o_jtcsvt,             "jtcsvt[njt15]/F");
      TBranch* b_jtntracks = otree->Branch("jtntracks",       o_jtntracks,       "jtntracks[njt15]/F");
      TBranch* b_jtarea = otree->Branch("jtarea",          o_jtarea,             "jtarea[njt15]/F");
      TBranch* b_jtpull = otree->Branch("jtpull",          o_jtpull,             "jtpull[njt15]/F");
      TBranch* b_jtcharge = otree->Branch("jtcharge",        o_jtcharge,         "jtcharge[njt15]/F");
      TBranch* b_jtid = otree->Branch("jtid",            o_jtid,                 "jtid[njt15]/O");
      TBranch* b_jtchhadmult = otree->Branch("jtchhadmult",     o_jtchhadmult,   "jtchhadmult[njt15]/F");
      TBranch* b_jtgenflv = otree->Branch("jtgenflv",        o_jtgenflv,         "jtgenflv[njt15]/S");
      TBranch* b_jtgenpt = otree->Branch("jtgenpt",         o_jtgenpt,           "jtgenpt[njt15]/F");
      TBranch* b_jtgeneta = otree->Branch("jtgeneta",        o_jtgeneta,         "jtgeneta[njt15]/F");
      TBranch* b_jtgenphi = otree->Branch("jtgenphi",        o_jtgenphi,         "jtgenphi[njt15]/F");
      TBranch* b_jtgene = otree->Branch("jtgene",          o_jtgene,             "jtgene[njt15]/F");
      TBranch* b_njt15 = otree->Branch("njt15",            &o_njt15,             "njt15/I");
      TBranch* b_njt   = otree->Branch("njt",              &o_njt,               "njt/I");

      // loop over all events and recalculate jet pT and energy; no reordering!
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      THStatus * status = new THStatus(nevt);

      cout<<nevt<<" events will be processed"<<endl;

      for (unsigned ievt=0;ievt<nevt;ievt++) {

	status->print_status(ievt);

	itree->GetEntry(ievt);


	Float_t rawpt_temp[140],pt_temp[140],eta_temp[140],e_temp[140], phi_temp[140],csvt_temp[140],area_temp[140],pull_temp[140],
	  ntracks_temp[140],charge_temp[140],id_temp[140],chhadmult_temp[140],
	  genflv_temp[140],genpt_temp[140],geneta_temp[140],genphi_temp[140],gene_temp[140],jes_temp[140];
	
	for (int ijt=0;ijt<njt15;ijt++) {

	  TLorentzVector jt;
	  if(bflav)
	    {
	      if(i_jtgenflv[ijt]==5)
		{
		  jesUnc->setJetPt(i_jtpt[ijt]);
		  jesUnc->setJetEta(i_jteta[ijt]);
	      
		  const Double_t ptscale = 1. + ((float)systematic*(jesUnc->getUncertainty(true)));
		  jt.SetPtEtaPhiE(ptscale*i_jtpt[ijt],i_jteta[ijt],i_jtphi[ijt],ptscale*i_jte[ijt]);
		}
	      else
		jt.SetPtEtaPhiE(i_jtpt[ijt],i_jteta[ijt],i_jtphi[ijt],i_jte[ijt]);
	    }
	  else
	    {
	      if(i_jtgenflv[ijt]!=5)
		{
                  jesUnc->setJetPt(i_jtpt[ijt]);
                  jesUnc->setJetEta(i_jteta[ijt]);

                  const Double_t ptscale = 1. + ((float)systematic*(jesUnc->getUncertainty(true)));
                  jt.SetPtEtaPhiE(ptscale*i_jtpt[ijt],i_jteta[ijt],i_jtphi[ijt],ptscale*i_jte[ijt]);
                }
	      else
		jt.SetPtEtaPhiE(i_jtpt[ijt],i_jteta[ijt],i_jtphi[ijt],i_jte[ijt]);
	    }

	  pt_temp[ijt] = jt.Pt();
	  e_temp[ijt]  = jt.E();
	  // m_temp[ijt]  = jt.M();
	  jes_temp[ijt]= jt.Pt()/i_jtpt[ijt];
	  rawpt_temp[ijt] = i_jtrawpt[ijt];

	  eta_temp[ijt] = i_jteta[ijt];
	  phi_temp[ijt] = i_jtphi[ijt];

	  csvt_temp[ijt] = i_jtcsvt[ijt];
	  ntracks_temp[ijt] = i_jtntracks[ijt];
	  area_temp[ijt] = i_jtarea[ijt];
	  pull_temp[ijt] = i_jtpull[ijt];
	  charge_temp[ijt] = i_jtcharge[ijt];
	  id_temp[ijt] = i_jtid[ijt];
	  
	  chhadmult_temp[ijt] = i_jtchhadmult[ijt];
	  
	  genflv_temp[ijt] = i_jtgenflv[ijt];
	  genpt_temp[ijt] = i_jtgenpt[ijt];
	  geneta_temp[ijt] = i_jtgeneta[ijt];
	  genphi_temp[ijt] = i_jtgenphi[ijt];
	  gene_temp[ijt] = i_jtgene[ijt];
	    
	} // for njt

        vector<size_t> indices;
	indices.resize(njt15);
        for(Int_t i=0; i<njt15; ++i){
          indices[i] = i;
	}

	sort(indices.begin(), indices.end(), GreaterByPt(pt_temp));

	//FINAL FILL FOR JETS                                                                                                         
        o_njt = 0;
        o_njt15 = 0;

	for(int i=0; i<njt15; i++)
          {
            if(pt_temp[indices[i]]>20.)
	      o_njt++;

	      o_jtrawpt[o_njt15]        =   rawpt_temp[indices[i]];
	      o_jtpt[o_njt15]           =   pt_temp[indices[i]];
	      o_jteta[o_njt15]          =   eta_temp[indices[i]];
	      o_jtphi[o_njt15]          =   phi_temp[indices[i]];
	      o_jte[o_njt15]            =   e_temp[indices[i]];
	      o_jtjes[o_njt15]          =   jes_temp[indices[i]];
	      o_jtcsvt[o_njt15]         =   csvt_temp[indices[i]];
	      o_jtntracks[o_njt15]      =   ntracks_temp[indices[i]];
	      o_jtarea[o_njt15]         =   area_temp[indices[i]];
	      o_jtpull[o_njt15]         =   pull_temp[indices[i]];
	      o_jtcharge[o_njt15]       =   charge_temp[indices[i]];
	      o_jtid[o_njt15]           =   id_temp[indices[i]];
	      o_jtchhadmult[o_njt15]    =   chhadmult_temp[indices[i]];
	      
	      o_jtgenflv[o_njt15]       =   genflv_temp[indices[i]];
	      o_jtgenpt[o_njt15]        =   genpt_temp[indices[i]];
	      o_jtgeneta[o_njt15]       =   geneta_temp[indices[i]];
	      o_jtgenphi[o_njt15]       =   genphi_temp[indices[i]];
	      o_jtgene[o_njt15]         =   gene_temp[indices[i]];

	      o_njt15++;

	  }
	
	
	// jt:
	//otree->Fill();

	b_jtrawpt -> Fill();
	b_jtpt -> Fill();
	b_jteta -> Fill();
	b_jtphi -> Fill();
	b_jte -> Fill();
	b_jtjes -> Fill();
	b_jtcsvt -> Fill();
	b_jtntracks -> Fill();
	b_jtarea -> Fill();
	b_jtpull -> Fill();
	b_jtcharge -> Fill();
	b_jtid -> Fill();
	b_jtchhadmult -> Fill();
	b_jtgenflv -> Fill();
	b_jtgenpt -> Fill();
	b_jtgeneta -> Fill();
	b_jtgenphi -> Fill();
	b_jtgene -> Fill();
	b_njt -> Fill();
	b_njt15 -> Fill();
		
      } // for ievt<nevt


    } // while in keys

    cout<<"Save extended tree to "<<output.substr(0,output.find(".jer.root"))<< " \n" << flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv " + output + " " 
      + output.substr(0,output.find(".jes.root")) + ".root";
    gSystem->Exec(cmd.c_str());
  } // for iinput

  cout<<"DONE!\n"<<endl;

  return 0;
}
