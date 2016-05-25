////////////////////////////////////////////////////////////////////////////////
//
// th_addvariables_x
// -----------------
//
//            01/10/2013 Benedikt Maier <bmaier@cern.ch>
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
bool EleVetoID_2012(float pt, float eta, float relIso);
bool MuVetoID_2012(float pt, float eta, float relIso);

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
      if (0!=itree->FindBranch("lepwmt"))             itree->SetBranchStatus("lepwmt",          0);
      if (0!=itree->FindBranch("lepwm"))              itree->SetBranchStatus("lepwm",           0);
      if (0!=itree->FindBranch("lepwpt"))             itree->SetBranchStatus("lepwpt",          0);
      if (0!=itree->FindBranch("lepwphi"))            itree->SetBranchStatus("lepwphi",         0);
      if (0!=itree->FindBranch("lepweta"))            itree->SetBranchStatus("lepweta",         0);
      if (0!=itree->FindBranch("lepwet"))             itree->SetBranchStatus("lepwet",          0);
      if (0!=itree->FindBranch("lepwdphi"))           itree->SetBranchStatus("lepwdphi",        0);

      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;

      // if cleaning is requested, nothing further!
      if (clean) continue;
      
      // create H->bb branches in output tree
      Float_t lepwm={-99};    TBranch* b_1 = otree->Branch("lepwm",         &lepwm,                   "lepwm/F");
      Float_t lepwpt={-99};   TBranch* b_2 = otree->Branch("lepwpt",        &lepwpt,                 "lepwpt/F");
      Float_t lepwphi={-99};  TBranch* b_3 = otree->Branch("lepwphi",       &lepwphi,               "lepwphi/F");
      Float_t lepweta={-99};  TBranch* b_4 = otree->Branch("lepweta",       &lepweta,               "lepweta/F");
      Float_t lepwet={-99};   TBranch* b_5 = otree->Branch("lepwet",        &lepwet,                 "lepwet/F");
      Float_t lepwmt={-99};   TBranch* b_6 = otree->Branch("lepwmt",        &lepwmt,                 "lepwmt/F");
      Float_t lepwdphi={-99}; TBranch* b_7 = otree->Branch("lepwdphi",      &lepwdphi,             "lepwdphi/F");


      // Set Branch Addresses
      Int_t   nvetoel={-99};  itree->SetBranchAddress("nvetoel",   &nvetoel);
      Int_t   nvetomu={-99};  itree->SetBranchAddress("nvetomu",   &nvetomu);

      Int_t   nlel;                    itree->SetBranchAddress("nlel",          &nlel);
      Float_t lelpt[110]={-99};        itree->SetBranchAddress("lelpt",         lelpt);
      Float_t leleta[110]={-99};       itree->SetBranchAddress("leleta",        leleta);
      Float_t lelphi[110]={-99};       itree->SetBranchAddress("lelphi",        lelphi);
      Float_t leliso[110]={-99};       itree->SetBranchAddress("leliso",        leliso);
      Float_t leliso2[110]={-99};      itree->SetBranchAddress("leliso2",       leliso2);
      Float_t lele[110]={-99};         itree->SetBranchAddress("lele",          lele);
      Float_t lelcharge[110]={-99};    itree->SetBranchAddress("lelcharge",     lelcharge);
      Float_t lelmvaid[110]={-99};     itree->SetBranchAddress("lelmvaid",      lelmvaid);
      Float_t  lelconv[110]={-99};     itree->SetBranchAddress("lelconv",       lelconv);
      
      Int_t   nlmu;                    itree->SetBranchAddress("nlmu",          &nlmu);
      Float_t lmupt[110]={-99};        itree->SetBranchAddress("lmupt",         lmupt);
      Float_t lmueta[110]={-99};       itree->SetBranchAddress("lmueta",        lmueta);
      Float_t lmuphi[110]={-99};       itree->SetBranchAddress("lmuphi",        lmuphi);
      Float_t lmuiso[110]={-99};       itree->SetBranchAddress("lmuiso",        lmuiso);
      Float_t lmuiso_dBeta[110]={-99}; itree->SetBranchAddress("lmuiso_dBeta",  lmuiso_dBeta);
      Float_t lmue[110]={-99};         itree->SetBranchAddress("lmue",          lmue);
      Float_t lmucharge[110]={-99};    itree->SetBranchAddress("lmucharge",     lmucharge);
      Float_t musigid[110]={-99};      itree->SetBranchAddress("musigid",       musigid);

      Float_t met={-99};               itree->SetBranchAddress("met",           &met);
      Float_t uncorrmet={-99};         itree->SetBranchAddress("uncorrmet",     &uncorrmet);
      Float_t metphi={-99};            itree->SetBranchAddress("metphi",        &metphi);
      Float_t uncorrmetphi={-99};      itree->SetBranchAddress("uncorrmetphi",  &uncorrmetphi);
      Float_t meteta={-99};            itree->SetBranchAddress("meteta",        &meteta);
      Float_t metx={-99};              itree->SetBranchAddress("metx",          &metx);
      Float_t mety={-99};              itree->SetBranchAddress("mety",          &mety);
      Float_t metsig=-99;              itree->SetBranchAddress("metsig",        &metsig);
      Float_t metsumet=-99;            itree->SetBranchAddress("metsumet",      &metsumet);
      
      // loop over all events and fill H->bb branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {

	itree->GetEntry(ievt);

	Int_t posnvetomu = -1;
	Int_t posnvetoel = -1;

	for(Int_t i=0; i<nlmu; i++){
	  if(MuVetoID_2012(lmupt[i],lmueta[i],lmuiso_dBeta[i])){
	    if(posnvetomu == -1)
	      posnvetomu = i;
	  }
	}

	for(Int_t i=0; i<nlel; i++){
	  if(EleVetoID_2012(lelpt[i],leleta[i],leliso[i])){
	    if(posnvetoel == -1)
	      posnvetoel = i;
	  }
	}
	
	bool has_lel = nvetoel==1;
	bool has_lmu = nvetomu==1;
	TLorentzVector p4nu;
	TLorentzVector lep;
	TLorentzVector p4W;
	
	
	// Extended the acceptance of this part for the QCD studies, since we ask explicitly for nveto_el == 1 && niso_el == 0                                                            
	if (has_lmu||has_lel) {
	  if (has_lmu && has_lel){
	    if(lelpt[posnvetoel]>lmupt[posnvetomu]) lep.SetPtEtaPhiE(lelpt[posnvetoel],leleta[posnvetoel],lelphi[posnvetoel],lele[posnvetoel]);
	    else lep.SetPtEtaPhiE(lmupt[posnvetomu],lmueta[posnvetomu],lmuphi[posnvetomu],lmue[posnvetomu]);
	  }
	  else if (has_lel) lep.SetPtEtaPhiE(lelpt[posnvetoel],leleta[posnvetoel],lelphi[posnvetoel],lele[posnvetoel]);
	  else if (has_lmu) lep.SetPtEtaPhiE(lmupt[posnvetomu],lmueta[posnvetomu],lmuphi[posnvetomu],lmue[posnvetomu]);
	  
	  float nu_e  = met;
	  float nu_px = metx;
	  float nu_py = mety;

	  float lep_e  = lep.E();
	  float lep_pt = lep.Pt();
	  float lep_px = lep.Px();
	  float lep_py = lep.Py();
	  float lep_pz = lep.Pz();

	  float mw  = 80.43;

	  float pt  = std::sqrt((lep_px+nu_px)*(lep_px+nu_px)+
				(lep_py+nu_py)*(lep_py+nu_py));

	  float mtsq= ((lep_pt+nu_e)*(lep_pt+nu_e) -
		       (lep_px+nu_px)*(lep_px+nu_px) -
		       (lep_py+nu_py)*(lep_py+nu_py));

	  float mt  = (mtsq>0.0) ? std::sqrt(mtsq) : 0.0;

	  float A, B, Csq, C, S1, S2;
	  float scf(1.0);

	  if (mt < mw) {
	    A = mw*mw/2.0;
	  }
	  else {
	    A = mt*mt/2.0;
	    float k = nu_e*lep_pt - nu_px*lep_px - nu_py*lep_py;
	    if (k <0.0001) k = 0.0001;
	    scf = 0.5*(mw*mw)/k;
	    nu_px *= scf;
	    nu_py *= scf;
	    nu_e  = sqrt(nu_px*nu_px + nu_py*nu_py);
	  }

	  B  = nu_px*lep_px + nu_py*lep_py;
	  Csq= 1.0 + nu_e*nu_e*(lep_pz*lep_pz-lep_e*lep_e)/(A+B)/(A+B);
	  C  = (Csq>0.0) ? std::sqrt(Csq) : 0.0;
	  S1 = (-(A+B)*lep_pz + (A+B)*lep_e*C)/(lep_pz*lep_pz-lep_e*lep_e);
	  S2 = (-(A+B)*lep_pz - (A+B)*lep_e*C)/(lep_pz*lep_pz-lep_e*lep_e);

	  float nu_pz = (std::abs(S1) < std::abs(S2)) ? S1 : S2;

	  TVector3 nu;   nu.SetXYZ(nu_px,nu_py,nu_pz);
	  TVector3 wlnu; wlnu.SetXYZ(nu_px+lep_px,nu_py+lep_py,nu_pz+lep_pz);

	  lepwpt   = pt;
	  lepweta  = wlnu.Eta();
	  lepwphi  = wlnu.Phi();
	  lepwet   = nu_e + lep_pt;
	  lepwmt   = mt;
	  lepwdphi = lep.Vect().DeltaPhi(nu);

	  float nupt=std::sqrt((nu_px)*(nu_px)+
			       (nu_py)*(nu_py));
	  float nueta=nu.Eta();
	  float nuphi=nu.Phi();

	  p4nu.SetPtEtaPhiM(nupt,nueta,nuphi,0);
	  p4W=lep+p4nu;

	  //p4W.SetPtEtaPhiM(lepwpt,lepweta,lepwphi,80.43);                                                                                                                               

	  lepwm=p4W.M();
	}
	else
	  {
	    lepwpt   = -999.;
	    lepweta  = -999.;
	    lepwphi  = -999.;
	    lepwet   = -999.;
	    lepwmt   = -999.;
	    lepwdphi = -999.;
	    lepwm    = -999.;
	  }
	b_1 ->Fill();
	b_2 ->Fill();
	b_3 ->Fill();
	b_4 ->Fill();
	b_5 ->Fill();	
	b_6 ->Fill();
	b_7 ->Fill();
	
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



//______________________________________________________________________________
double deltaPhi(double phi1, double phi2)
{
  double result = phi1 - phi2;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}


bool EleVetoID_2012(float pt, float eta, float relIso) {
  Float_t eleptmin=20;
  Float_t eleetamax=2.5;
  Float_t relisomax=0.15;

  if(fabs(pt)<=eleptmin) return false;
  if(fabs(eta)>=eleetamax) return false;
  if(relIso >= relisomax) return false;
  return true;
}

bool MuVetoID_2012(float pt, float eta, float relIso)
{
  Float_t muptmin=10.;
  Float_t muetamax=2.5;
  Float_t murelisomax=0.2;

  if(fabs(pt)<=muptmin) return false;
  if(fabs(eta)>=muetamax) return false;
  if(relIso >= murelisomax) return false;

  return true;
}
