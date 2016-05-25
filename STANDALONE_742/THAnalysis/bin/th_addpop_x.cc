////////////////////////////////////////////////////////////////////////////////
//
// th_addpop_x
// -----------------
//
//            04/14/2014 Nils Faltermann <nils.faltermann@cern.ch>
//                                                    
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"
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

double deltaPhi(double phi1, double phi2);
double deltaR(double eta1, double phi1, double eta2, double phi2);


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
  string         datapath    = cl.getValue<string> ("datapath",  "/");
  bool           clean       = cl.getValue<bool>   ("clean",   false);

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

      // check wether sample is considered MC (weights to be stored)
      bool isMC = itree->FindBranch("npv");
      if (isMC) cout<<"Treating as MC"<<endl; else cout<<"Treating as DATA"<<endl;


      // don't copy H->bb branches if they already exist
      if (0!=itree->FindBranch("popvar1"))           itree->SetBranchStatus("popvar1",          0);
      if (0!=itree->FindBranch("popvar2"))           itree->SetBranchStatus("popvar2",          0);
      if (0!=itree->FindBranch("popvar3"))           itree->SetBranchStatus("popvar3",          0);
      if (0!=itree->FindBranch("popvar4"))           itree->SetBranchStatus("popvar4",          0);
      if (0!=itree->FindBranch("popvar5"))           itree->SetBranchStatus("popvar5",          0);
      if (0!=itree->FindBranch("popvar6"))           itree->SetBranchStatus("popvar6",          0);
      if (0!=itree->FindBranch("popvar7"))           itree->SetBranchStatus("popvar7",          0);
      if (0!=itree->FindBranch("popvar8"))           itree->SetBranchStatus("popvar8",          0);
      if (0!=itree->FindBranch("popvar9"))           itree->SetBranchStatus("popvar9",          0);
      if (0!=itree->FindBranch("popvar10"))          itree->SetBranchStatus("popvar10",         0);
      if (0!=itree->FindBranch("popvar11"))          itree->SetBranchStatus("popvar11",         0);
      if (0!=itree->FindBranch("popvar12"))          itree->SetBranchStatus("popvar12",         0);
      if (0!=itree->FindBranch("popvar13"))          itree->SetBranchStatus("popvar13",         0);
      if (0!=itree->FindBranch("popvar14"))          itree->SetBranchStatus("popvar14",         0);
      if (0!=itree->FindBranch("popvar15"))          itree->SetBranchStatus("popvar15",         0);
      if (0!=itree->FindBranch("popvar16"))          itree->SetBranchStatus("popvar16",         0);
      if (0!=itree->FindBranch("popvar17"))          itree->SetBranchStatus("popvar17",         0);
      if (0!=itree->FindBranch("popvar18"))          itree->SetBranchStatus("popvar18",         0);
      if (0!=itree->FindBranch("popvar19"))          itree->SetBranchStatus("popvar19",         0);
 
      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;

      // if cleaning is requested, nothing further!
      if (clean) continue;
      
      // create H->bb branches in output tree
      
      Float_t popvar1=0,popvar2=0,popvar3=0,popvar4=0,popvar5=0,
	popvar6=0,popvar7=0,popvar8=0,popvar9=0,popvar10=0,
	popvar11=0,popvar12=0,popvar13=0,popvar14=0,popvar15=0,
	popvar16=0,popvar17=0,popvar18=0,popvar19=0;

      TBranch* b_popvar1             =otree->Branch("popvar1",           &popvar1,                     "popvar1/F");
      TBranch* b_popvar2             =otree->Branch("popvar2",           &popvar2,                     "popvar2/F");
      TBranch* b_popvar3             =otree->Branch("popvar3",           &popvar3,                     "popvar3/F");
      TBranch* b_popvar4             =otree->Branch("popvar4",           &popvar4,                     "popvar4/F");
      TBranch* b_popvar5             =otree->Branch("popvar5",           &popvar5,                     "popvar5/F");
      TBranch* b_popvar6             =otree->Branch("popvar6",           &popvar6,                     "popvar6/F");
      TBranch* b_popvar7             =otree->Branch("popvar7",           &popvar7,                     "popvar7/F");
      TBranch* b_popvar8             =otree->Branch("popvar8",           &popvar8,                     "popvar8/F");
      TBranch* b_popvar9             =otree->Branch("popvar9",           &popvar9,                     "popvar9/F");
      TBranch* b_popvar10            =otree->Branch("popvar10",          &popvar10,                   "popvar10/F");
      TBranch* b_popvar11            =otree->Branch("popvar11",          &popvar11,                   "popvar11/F");
      TBranch* b_popvar12            =otree->Branch("popvar12",          &popvar12,                   "popvar12/F");
      TBranch* b_popvar13            =otree->Branch("popvar13",          &popvar13,                   "popvar13/F");
      TBranch* b_popvar14            =otree->Branch("popvar14",          &popvar14,                   "popvar14/F");
      TBranch* b_popvar15            =otree->Branch("popvar15",          &popvar15,                   "popvar15/F");
      TBranch* b_popvar16            =otree->Branch("popvar16",          &popvar16,                   "popvar16/F");
      TBranch* b_popvar17            =otree->Branch("popvar17",          &popvar17,                   "popvar17/F");
      TBranch* b_popvar18            =otree->Branch("popvar18",          &popvar18,                   "popvar18/F");
      TBranch* b_popvar19            =otree->Branch("popvar19",          &popvar19,                   "popvar19/F");

      // setup input branches needed to fill H->bb branches

      Float_t lepwphi={-99};           itree->SetBranchAddress("lepwphi",         &lepwphi);
      Float_t lepweta={-99};           itree->SetBranchAddress("lepweta",         &lepweta);
      Float_t lepwpt={-99};            itree->SetBranchAddress("lepwpt",           &lepwpt);
      Float_t lepwm={-99};             itree->SetBranchAddress("lepwm",             &lepwm);
      Float_t hbbpt={-99};             itree->SetBranchAddress("hbbpt",             &hbbpt);

      Float_t jtcsvtcorr[40][5];       itree->SetBranchAddress("jtcsvtcorr",    jtcsvtcorr);
      Float_t jtcsvt[140];             itree->SetBranchAddress("jtcsvt",            jtcsvt);
      
      Int_t njt={-99};                 itree->SetBranchAddress("njt",                 &njt);
      
      Float_t jtpt[40]={-99};          itree->SetBranchAddress("jtpt",                jtpt);
      Float_t jteta[40]={-99};         itree->SetBranchAddress("jteta",              jteta);
      Float_t jtphi[40]={-99};         itree->SetBranchAddress("jtphi",              jtphi);
      Float_t jte[40]={-99};           itree->SetBranchAddress("jte",                  jte);
      
      Int_t hbbjtidx[3];               itree->SetBranchAddress("hbbjtidx",        hbbjtidx);
      Int_t topjtidx;                  itree->SetBranchAddress("topjtidx",       &topjtidx);
      Int_t ljtidx;                    itree->SetBranchAddress("ljtidx",           &ljtidx);
      
      Int_t nel={-99};                 itree->SetBranchAddress("nel",                 &nel);
      
      Int_t nmu={-99};                 itree->SetBranchAddress("nmu",                 &nmu);
      
      Float_t elcharge[10]={-99};      itree->SetBranchAddress("elcharge",        elcharge);
      Float_t mucharge[10]={-99};      itree->SetBranchAddress("mucharge",        mucharge);
      Float_t met=-99;                 itree->SetBranchAddress("met",                 &met);
      Float_t coststh_rec={-99};       itree->SetBranchAddress("coststh_rec", &coststh_rec);
      Float_t sphericity=-99;          itree->SetBranchAddress("sphericity",   &sphericity);
      Int_t tophadjtidx[3];            itree->SetBranchAddress("tophadjtidx",  tophadjtidx);
      Float_t hbbdr=-99;               itree->SetBranchAddress("hbbdr",             &hbbdr);
      Float_t jtcharge[40]={-99};      itree->SetBranchAddress("jtcharge",        jtcharge);
      Float_t tophadwm={-99};          itree->SetBranchAddress("tophadwm",       &tophadwm);
      Float_t tophadm={-99};           itree->SetBranchAddress("tophadm",         &tophadm);
      
       
      // loop over all events and fill H->bb branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {

	itree->GetEntry(ievt);

	TLorentzVector p4jets[100], p4alljets;
	TLorentzVector p4lepw,p4tophadb;
	p4alljets.SetPtEtaPhiE(0,0,0,0);
	for(int jets=0; jets<njt; jets++){
	  p4jets[jets].SetPtEtaPhiE(jtpt[jets],jteta[jets],jtphi[jets],jte[jets]);
	  p4alljets=p4alljets+p4jets[jets];
	}
	p4lepw.SetPtEtaPhiM(lepwpt,lepweta,lepwphi,lepwm);

	Float_t lepcharge=0;
	if(nel==0 && nmu==1){
	  lepcharge=mucharge[0];
	}
	else if (nel==1 && nmu ==0 ){
	  lepcharge=elcharge[0];
	}

	//glb_Charge_Lep
	popvar1   =  (nmu==1)? mucharge[0]:elcharge[0]; 
	
	//log(glb_SqrtSHat)
	popvar2   =  TMath::Log((p4alljets+p4lepw).M()); 
	
	//log(glb_Pt_J1) 
	popvar3   =  TMath::Log(jtpt[0]); 
	
	//log(glb_MET)
	popvar4   =  TMath::Log(met);
	
	//log(thq_Pt_Higgs)
	popvar5   =  TMath::Log(hbbpt);   
	
	//log(thq_Pt_Recoil)
	popvar6   =  TMath::Log(jtpt[ljtidx]); 
	
	//thq_Cos_LepRecoil_TH
	popvar7   =  coststh_rec;
	
	//abs(thq_Eta_Recoil)
	popvar8   =  TMath::Abs(jteta[ljtidx]); 
	
	//glb_Sphericity
	popvar9   =  sphericity; 
	
	//tt_MaxEta_Light
	popvar10  =  TMath::Max(TMath::Abs(jteta[tophadjtidx[0]]),TMath::Abs(jteta[tophadjtidx[1]])); 
	
	//log(tt_Mass_TopHad)
	popvar11  =  TMath::Log(tophadm);
	
	//log(tt_DMass_TopHadWHad)
	popvar12  =  TMath::Log(tophadm-tophadwm);
	
	//log(tt_MaxMass_BTopHadLight)
	popvar13  =  TMath::Log(TMath::Max((p4jets[tophadjtidx[2]]+p4jets[tophadjtidx[0]]).M(),(p4jets[tophadjtidx[2]]+p4jets[tophadjtidx[1]]).M()));  
	
	//thq_NumBTag_Higgs 
	popvar14  =  (jtcsvt[hbbjtidx[0]]>0.898)*(TMath::Abs(jteta[hbbjtidx[0]])<2.4)+(jtcsvt[hbbjtidx[1]]>0.898)*(TMath::Abs(jteta[hbbjtidx[1]])<2.4);
	
	//tt_NumPassBTag_Light
	popvar15  =  (jtcsvt[tophadjtidx[0]]>0.898)*(TMath::Abs(jteta[tophadjtidx[0]])<2.4)+(jtcsvt[tophadjtidx[1]]>0.898)*(TMath::Abs(jteta[tophadjtidx[1]])<2.4); 
	
	//thq_DeltaR_BJetsHiggs
	popvar16  =  hbbdr;
	
	//tt_DeltaR_Light
	popvar17  =  deltaR(jteta[tophadjtidx[0]],jtphi[tophadjtidx[0]],jteta[tophadjtidx[1]],jtphi[tophadjtidx[1]]);  
	
	//tt_SumCharge_Light
	popvar18  =  (jtcharge[tophadjtidx[1]]+jtcharge[tophadjtidx[0]])*lepcharge;  
	
	//tt_Charge_BTopHad 
	popvar19  =  jtcharge[tophadjtidx[2]]*lepcharge;

	b_popvar1             ->Fill();
	b_popvar2             ->Fill();
	b_popvar3             ->Fill();
	b_popvar4             ->Fill();
	b_popvar5             ->Fill();
	b_popvar6             ->Fill();
	b_popvar7             ->Fill();
	b_popvar8             ->Fill();
	b_popvar9             ->Fill();
	b_popvar10            ->Fill();
	b_popvar11            ->Fill();
	b_popvar12            ->Fill();
	b_popvar13            ->Fill();
	b_popvar14            ->Fill();
	b_popvar15            ->Fill();
	b_popvar16            ->Fill();
	b_popvar17            ->Fill();
	b_popvar18            ->Fill();
	b_popvar19            ->Fill();

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

