////////////////////////////////////////////////////////////////////////////////
//
// th_puremover_x
// --------------
// 
// This program was part of an old study on the impact of pileup jets; not used
//  anymore.
//
// 12/05/2014             Simon Fink <simon.fink@cern.ch>        
//                        Comment: should have everything in it to also remove jets from collection
//                                 did currently not work due to array size set by njt15
////////////////////////////////////////////////////////////////////////////////

#include "../interface/CommandLine.h"
#include "../interface/THStatus.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TRandom3.h>


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
// HELPER DEFINITIONS
////////////////////////////////////////////////////////////////////////////////
//bool largerthan (Float_t i,Float_t j) { return (i>j); }
//void fill_jtrank(const Int_t njt,Float_t* ojtpt,Int_t* ojtrank);
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
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs    = cl.getVector<string>("inputs");
  string         datapath  = cl.getValue<string> ("datapath",               ".");
  string         treename  = cl.getValue<string> ("treename",               "t");
  string         opath     = cl.getValue<string> ("opath",                   "");
  string         output    = cl.getValue<string> ("output",                  "");
  bool           isData      = cl.getValue<bool> ("isData",                  false);

  if (!cl.check()) return 0;
  cl.print();


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
    output = output + ".pur.root";
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

      //      if (0!=itree->FindBranch("jtpt"))     itree->SetBranchStatus("jtpt",       0);

      Bool_t hasOrig=false;
      if (0!=itree->FindBranch("jteta_orig")) hasOrig=true; 
      
      
      if (0!=itree->FindBranch("keepPU"))   itree->SetBranchStatus("keepPU",      0);
      if (0!=itree->FindBranch("jteta"))               itree->SetBranchStatus("jteta",0);
      

      /*      if (0!=itree->FindBranch("jtpt"))     itree->SetBranchStatus("jtpt",       0);
      if (0!=itree->FindBranch("jte"))      itree->SetBranchStatus("jte",        0);
      if (0!=itree->FindBranch("jtjer"))    itree->SetBranchStatus("jtjer",      0);
      
      if (0!=itree->FindBranch("jtphi"))               itree->SetBranchStatus("jtphi",0);
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

      */

      // copy the tree
      odir->cd();
      cout<<"Cloning tree in "<<input<<" ==> "<<output<<" ... "<<flush;
      TTree* otree = itree->CloneTree();
      cout<<"done.\n";

      // set branch stati valid again...!
      
      //if (0!=itree->FindBranch("jtpt"))     itree->SetBranchStatus("jtpt",       1);
      if (0!=itree->FindBranch("jteta"))    itree->SetBranchStatus("jteta",      1);
      

      

      /*
      if (0!=itree->FindBranch("jtpt"))     itree->SetBranchStatus("jtpt",       1);
      if (0!=itree->FindBranch("jte"))      itree->SetBranchStatus("jte",        1);
      if (0!=itree->FindBranch("jtjer"))    itree->SetBranchStatus("jtjer",      1);
      if (0!=itree->FindBranch("jteta"))               itree->SetBranchStatus("jteta",1);
      if (0!=itree->FindBranch("jtphi"))               itree->SetBranchStatus("jtphi",1);
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
      if (0!=itree->FindBranch("njt"))                 itree->SetBranchStatus("njt",1);
      if (0!=itree->FindBranch("njt15"))               itree->SetBranchStatus("njt15",1);
      */

      // create the branches in the output tree
      
      Bool_t o_keepPU[100];
      Float_t o_jteta[100];
      Float_t o_jteta_orig[100];
      /*
      Float_t o_jtpt[100];

      Float_t o_jtphi[100];
      Float_t o_jte[100];
      Float_t o_jtjer[100];

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
      */
      //Int_t o_njt  = 0;
      //Int_t o_njt15 = 0;

      // setup input branches                 
	
      TBranch* b_keepPU   = otree->Branch("keepPU",              o_keepPU,                                "keepPU[njt15]/O");	
      TBranch* b_jteta = otree->Branch("jteta",      o_jteta,      "jteta[njt15]/F");



      TBranch* b_jteta_orig;
      if(!hasOrig)  b_jteta_orig = otree->Branch("jteta_orig",      o_jteta_orig,      "jteta_orig[njt15]/F");
      /*
      TBranch* b_njt15 = otree->Branch("njt15",            &o_njt15,                            "njt15/I");
      TBranch* b_njt   = otree->Branch("njt",              &o_njt,                                "njt/I");

	TBranch* b_jtpt = otree->Branch("jtpt",       o_jtpt,        "jtpt[njt15]/F");
	TBranch* b_jte = otree->Branch("jte",        o_jte,          "jte[njt15]/F");
	
	TBranch* b_jtphi = otree->Branch("jtphi",      o_jtphi,      "jtphi[njt15]/F");
	TBranch* b_jtjer = otree->Branch("jtjer",      o_jtjer,      "jtjer[njt15]/F");

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

      */
	

      // setup input branches                                                                                                         

      Float_t i_jtpt[100];        itree->SetBranchAddress("jtpt",   i_jtpt);
      Float_t i_jteta[100];       itree->SetBranchAddress("jteta",  i_jteta);
      Float_t i_jtphi[100];       itree->SetBranchAddress("jtphi",  i_jtphi);
      Float_t i_jte[100];         itree->SetBranchAddress("jte",    i_jte);


      Float_t i_jteta_orig[100]; 
      if (hasOrig) itree->SetBranchAddress("jteta_orig", i_jteta_orig);
      
      /*      Float_t i_jtrawpt[100]={-99};        itree->SetBranchAddress("jtrawpt",           i_jtrawpt);
      Float_t i_jtcsvt[100]={-99};        itree->SetBranchAddress("jtcsvt",           i_jtcsvt);
      Float_t i_jtntracks[100]={-99};        itree->SetBranchAddress("jtntracks",           i_jtntracks);
      Float_t i_jtarea[100]={-99};        itree->SetBranchAddress("jtarea",           i_jtarea);
      Float_t i_jtjer[100]={-99};        itree->SetBranchAddress("jtjer",           i_jtjer);
      Float_t i_jtpull[100]={-99};        itree->SetBranchAddress("jtpull",           i_jtpull);
      Float_t i_jtcharge[100]={-99};        itree->SetBranchAddress("jtcharge",           i_jtcharge);
      Bool_t  i_jtid[100]={-99};        itree->SetBranchAddress("jtid",           i_jtid);
      Float_t i_jtchhadmult[100]={-99};        itree->SetBranchAddress("jtchhadmult",           i_jtchhadmult);
      Short_t i_jtgenflv[100]={-99};        itree->SetBranchAddress("jtgenflv",           i_jtgenflv);
    
      */
      Float_t i_jtgenpt[100]={-99};        itree->SetBranchAddress("jtgenpt",           i_jtgenpt);
      Float_t i_jtgeneta[100]={-99};        itree->SetBranchAddress("jtgeneta",           i_jtgeneta);
      Float_t i_jtgenphi[100]={-99};        itree->SetBranchAddress("jtgenphi",           i_jtgenphi);
      Float_t i_jtgene[100]={-99};        itree->SetBranchAddress("jtgene",           i_jtgene);
      Int_t   i_njt15=0;                     itree->SetBranchAddress("njt15",           &i_njt15);


      // setup input branches                                                                                                         
      
      // read correction histogram

      TFile fPuCorr("/storage/a/singletophiggs/th_Moriond/PUCorr/puJetCorrection.root");
      TH2D *histPUCorr = (TH2D*) fPuCorr.Get("puJetCorrection");


      TRandom3 rGen(0);
      Int_t jetcounter=0;
      Int_t jetcounter2=0;
      // loop over all events and recalculate jet pT and energy; no reordering!
      unsigned nevt= static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;

      THStatus* status = new THStatus(nevt);

      for (unsigned ievt=0;ievt<nevt;ievt++) {
	status->print_status(ievt);

	itree->GetEntry(ievt);
	
	Bool_t skipJet;
	      
	for (int ijt=0;ijt < i_njt15;ijt++) {
	  TLorentzVector p4genjet, p4jet;
	  skipJet = false; 
	  p4genjet.SetPtEtaPhiE(i_jtgenpt[ijt],i_jtgeneta[ijt],i_jtgenphi[ijt],i_jtgene[ijt]);
	  if(hasOrig) p4jet.SetPtEtaPhiE(i_jtpt[ijt],i_jteta_orig[ijt],i_jtphi[ijt],i_jte[ijt]);
	  else p4jet.SetPtEtaPhiE(i_jtpt[ijt],i_jteta[ijt],i_jtphi[ijt],i_jte[ijt]);

	  if (!( i_jtgenpt[ijt]>8 && DeltaR(p4genjet,p4jet)<0.25 ))
	    {
	      int bin = histPUCorr->FindFixBin(fabs(i_jteta[ijt]), i_jtpt[ijt]);
	      double excessRateFactor = histPUCorr->GetBinContent(bin);

	      if (rGen.Rndm() * excessRateFactor > 1.)
		skipJet = true;
	      if(excessRateFactor==0&&skipJet==1 )  cout << "Bin: " << bin << "    -    exessRate:" << excessRateFactor << "    -   skipJet:  "  << skipJet << endl;
	      
	    }
	  
	  o_keepPU[ijt]=!(skipJet);
	  if(isData) o_keepPU[ijt]=1;
	  
	  

	  o_jteta_orig[ijt] = i_jteta[ijt];
	  o_jteta[ijt] = i_jteta[ijt];

	  if(!(isData)){
	    if (abs(o_jteta[ijt]) > 2.4 && abs(o_jteta[ijt])<3.0 && abs(o_jteta[ijt])!=2.7) {
	      if(o_jteta[ijt]<0) o_jteta[ijt] = -2.7;
	      if(o_jteta[ijt]>0) o_jteta[ijt] =  2.7;
	      jetcounter2++;

	      //cout << "Changed Eta from " << o_jteta_orig[ijt] << " to " << o_jteta[ijt] << endl;
	    }
	    if (fabs(o_jteta[ijt])>=3.0 && abs(o_jteta[ijt])!=3.2) {
	      if(o_jteta[ijt]<0) o_jteta[ijt] = -3.2;
	      if(o_jteta[ijt]>0) o_jteta[ijt] =  3.2;
	      jetcounter2++;

	      //cout << "Changed Eta from " << o_jteta_orig[ijt] << " to " << o_jteta[ijt] << endl;
	    }
	  }

	  if(skipJet && !(isData)){
	    jetcounter++;
	    //cout << "############  SKipping Jet: " << ijt << " of event " << ievt << endl;
	    continue;
	  }
	  
	  
	  /*
	  if(i_jtpt[ijt]>15.) {
	    o_jtrawpt[o_njt15]        =   i_jtrawpt[ijt];
	    o_jtpt[o_njt15]           =   i_jtpt[ijt];
	    o_jteta[o_njt15]          =   i_jteta[ijt];
	    o_jtphi[o_njt15]          =   i_jtphi[ijt];
	    o_jte[o_njt15]            =   p4jet.E();
	    o_jtcsvt[o_njt15]         =   i_jtcsvt[ijt];
	    o_jtntracks[o_njt15]      =   i_jtntracks[ijt];
	    o_jtarea[o_njt15]         =   i_jtarea[ijt];
	    o_jtpull[o_njt15]         =   i_jtpull[ijt];
	    o_jtcharge[o_njt15]       =   i_jtcharge[ijt];
	    o_jtid[o_njt15]           =   i_jtid[ijt];
	    o_jtchhadmult[o_njt15]    =   i_jtchhadmult[ijt];
	    
	    o_jtgenflv[o_njt15]       =   i_jtgenflv[ijt];
	    o_jtgenpt[o_njt15]        =   i_jtgenpt[ijt];
	    o_jtgeneta[o_njt15]       =   i_jtgeneta[ijt];
	    o_jtgenphi[o_njt15]       =   i_jtgenphi[ijt];
	    o_jtgene[o_njt15]         =   i_jtgene[ijt];
	    o_jtjer[o_njt15]          =   i_jtjer[ijt];
	    
	    o_njt15++;
	  }
	  
	  if(i_jtpt[ijt]>20.) o_njt++;
	  */
	  

	} // jet loop
	

	b_keepPU->Fill();
	b_jteta -> Fill();
	if (!hasOrig) b_jteta_orig -> Fill();
/*
	b_njt->Fill();
	b_njt15->Fill();


	b_jtrawpt -> Fill();
	b_jtpt -> Fill();
	b_jteta -> Fill();
	b_jtphi -> Fill();
	b_jte -> Fill();
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
	b_jtjer-> Fill();
	*/
      } // for ievt<nevt

      cout << endl << "Marked " << jetcounter << " jets from sample for removal!" << endl;
      if(!hasOrig) cout << "Set " << jetcounter2 << " eta values to default values!" << endl;
      else cout << "Default eta values already set!" << endl;
    } // while in keys



    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv -f " + output + " " 
      + output.substr(0,output.find(".pur.root")) + ".root";
    //    string cmd2 = "cp " + output.substr(0,output.find(".pur.root")) + ".root " + "BACKUP_" + output.substr(0,output.find(".pur.root")) + ".root"; 
    //cout << endl <<  "Backupping old file to BACKUP_" << output.substr(0,output.find(".pur.root")) << ".root ..." << endl;
    //gSystem->Exec(cmd2.c_str());
     
    cout<<"Save extended tree to "<<output.substr(0,output.find(".pur.root"))<<".root ... "<<flush;
    gSystem->Exec(cmd.c_str());
  } // for iinput

  cout<<"DONE!\n"<<endl;

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// HELPER IMPLEMENTATIONS
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



//______________________________________________________________________________

/*
void fill_jtrank(const Int_t njt,Float_t* ojtpt,Int_t* ojtrank)
{
  std::vector<Float_t> vv_pts;
  std::multimap<Float_t,Int_t> mm_pts;

  for (Int_t i=0;i<njt;++i) {
    vv_pts.push_back(ojtpt[i]);
    mm_pts.insert(pair<Float_t,Int_t>(ojtpt[i],i));
  }

  std::sort (vv_pts.begin(),vv_pts.end(),largerthan);

  Int_t ijt(0);
  std::vector<Float_t>::const_iterator itvv = vv_pts.begin();
  for (;itvv!=vv_pts.end();++itvv) {
    std::multimap<Float_t,Int_t>::iterator itmm = mm_pts.find(*itvv);
    assert (itmm!=mm_pts.end());
    ojtrank[ itmm->second ] = (ijt);
    mm_pts.erase(itmm);
    ++ijt;
  }
}
*/
