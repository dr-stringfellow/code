////////////////////////////////////////////////////////////////////////////////
//
// th_metrecalculator_x
// --------------
// Recalculate MET with shifted Unclustered MET variables
//
//                              29/01/2014             Simon Fink <simon.fink@cern.ch>
////////////////////////////////////////////////////////////////////////////////

#include "../interface/CommandLine.h"
#include "../../JetMETObjects/interface/JetCorrectionUncertainty.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TLorentzVector.h>
#include <TVector.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cassert>
#include "../interface/THStatus.h"


using namespace std;

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

  vector<string> inputs     = cl.getVector<string>("inputs");
  string         datapath   = cl.getValue<string> ("datapath",               ".");
  string         treename   = cl.getValue<string> ("treename",               "t");
  string         opath      = cl.getValue<string> ("opath",                  ".");
  string         output     = cl.getValue<string> ("output",                  "");
  int            systematic = cl.getValue<int>    ("systematic",              0);
 
  if (!cl.check()) return 0;
  cl.print();

  if      (-1==systematic) cout<<"Down type systematic scaling!"<<endl;
  else if (+1==systematic) cout<<"Up type systematic scaling!"<<endl;
  else if (0==systematic) cout<<"Only recalculating met variables!"<<endl;
  else         {cout<<"Error in variable systematic: "<< systematic << endl; return 0;}

  // setup the standalone jet correction service

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
    output = output + ".met.root";
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
      if (0!=itree->FindBranch("met"))     itree->SetBranchStatus("met",       0);
      if (0!=itree->FindBranch("metx"))    itree->SetBranchStatus("metx",      0);
      if (0!=itree->FindBranch("mety"))    itree->SetBranchStatus("mety",      0);
      if (0!=itree->FindBranch("nlepw"))    itree->SetBranchStatus("nlepw",     0);
      if (0!=itree->FindBranch("lepwm"))    itree->SetBranchStatus("lepwm",     0);
      if (0!=itree->FindBranch("lepwpt"))    itree->SetBranchStatus("lepwpt",     0);
      if (0!=itree->FindBranch("lepwphi"))    itree->SetBranchStatus("lepwphi",     0);
      if (0!=itree->FindBranch("lepweta"))    itree->SetBranchStatus("lepweta",     0);
      if (0!=itree->FindBranch("lepwet"))    itree->SetBranchStatus("lepwet",     0);
      if (0!=itree->FindBranch("lepwmt"))    itree->SetBranchStatus("lepwmt",     0);
      if (0!=itree->FindBranch("lepwdphi"))    itree->SetBranchStatus("lepwdphi",     0);
      if (0!=itree->FindBranch("sphericity"))    itree->SetBranchStatus("sphericity",     0);
      if (0!=itree->FindBranch("aplanarity"))    itree->SetBranchStatus("aplanarity",     0);

      // copy the tree
      odir->cd();
      cout<<"Cloning tree in "<<input<<" ==> "<<output<<" ... "<<flush;
      TTree* otree = itree->CloneTree();
      cout<<"done.\n";

      // set branch stati valid again...!
      
      // jt:
      
      if (0!=itree->FindBranch("met"))     itree->SetBranchStatus("met",       1);
      if (0!=itree->FindBranch("metx"))    itree->SetBranchStatus("metx",      1);
      if (0!=itree->FindBranch("mety"))    itree->SetBranchStatus("mety",      1);
      if (0!=itree->FindBranch("nlepw"))    itree->SetBranchStatus("nlepw",     1);
      if (0!=itree->FindBranch("lepwm"))    itree->SetBranchStatus("lepwm",     1);
      if (0!=itree->FindBranch("lepwpt"))    itree->SetBranchStatus("lepwpt",     1);
      if (0!=itree->FindBranch("lepwphi"))    itree->SetBranchStatus("lepwphi",     1);
      if (0!=itree->FindBranch("lepweta"))    itree->SetBranchStatus("lepweta",     1);
      if (0!=itree->FindBranch("lepwet"))    itree->SetBranchStatus("lepwet",     1);
      if (0!=itree->FindBranch("lepwmt"))    itree->SetBranchStatus("lepwmt",     1);
      if (0!=itree->FindBranch("lepwdphi"))    itree->SetBranchStatus("lepwdphi",     1);
      if (0!=itree->FindBranch("sphericity"))    itree->SetBranchStatus("sphericity",     1);
      if (0!=itree->FindBranch("aplanarity"))    itree->SetBranchStatus("aplanarity",     1);
 
      // create the branches in the output tree
      
      Float_t omet;     
      Float_t ometx;    
      Float_t omety;    
  
      // met:
      
      TBranch* b_met = otree->Branch("met",      &omet,            "met/F");
      TBranch* b_metx = otree->Branch("metx",     &ometx,          "metx/F");       
      TBranch* b_mety = otree->Branch("mety",     &omety,          "mety/F");       
     
      UChar_t nlepw={-99};             TBranch* b_nlepw = otree->Branch("nlepw",         &nlepw,                   "nlepw/b");
      Float_t lepwm={-99};             TBranch* b_lepwm = otree->Branch("lepwm",         &lepwm,                   "lepwm/F");
      Float_t lepwpt={-99};            TBranch* b_lepwpt = otree->Branch("lepwpt",        &lepwpt,                 "lepwpt/F");
      Float_t lepwphi={-99};           TBranch* b_lepwphi = otree->Branch("lepwphi",       &lepwphi,               "lepwphi/F");
      Float_t lepweta={-99};           TBranch* b_lepweta = otree->Branch("lepweta",       &lepweta,               "lepweta/F");
      Float_t lepwet={-99};            TBranch* b_lepwet = otree->Branch("lepwet",        &lepwet,                 "lepwet/F");
      Float_t lepwmt={-99};            TBranch* b_lepwmt = otree->Branch("lepwmt",        &lepwmt,                 "lepwmt/F");
      Float_t lepwdphi={-99};          TBranch* b_lepwdphi = otree->Branch("lepwdphi",      &lepwdphi,             "lepwdphi/F");
      Float_t sphericity=-99;          TBranch* b_sphericity = otree->Branch("sphericity",    &sphericity,         "sphericity/F");
      Float_t aplanarity=-99;          TBranch* b_aplanarity = otree->Branch("aplanarity",    &aplanarity,         "aplanarity/F");


      // setup input branches
      Float_t metsys[2];
      Float_t metsysx[2];
      Float_t metsysy[2];
   
      itree->SetBranchAddress("metsys",      metsys);
      itree->SetBranchAddress("metsysx",    metsysx);  
      itree->SetBranchAddress("metsysy",    metsysy);  
      
      Float_t met=-99;               itree->SetBranchAddress("met",           &met);
      Float_t metx=-99;              itree->SetBranchAddress("metx",          &metx);
      Float_t mety=-99;              itree->SetBranchAddress("mety",          &mety);
      
      Int_t   nlel=0;                itree->SetBranchAddress("nlel",          &nlel);
      Float_t lelpt[110]={-99};      itree->SetBranchAddress("lelpt",         lelpt);
      Float_t leleta[110]={-99};     itree->SetBranchAddress("leleta",        leleta);
      Float_t lelphi[110]={-99};     itree->SetBranchAddress("lelphi",        lelphi);
      Float_t lele[110]={-99};       itree->SetBranchAddress("lele",          lele);

      Int_t nel={-99};               itree->SetBranchAddress("nel",            &nel);
      Float_t elpt[10]={-99};        itree->SetBranchAddress("elpt",           elpt);
      Float_t eleta[10]={-99};       itree->SetBranchAddress("eleta",         eleta);
      Float_t elphi[10]={-99};       itree->SetBranchAddress("elphi",         elphi);
      Float_t ele[10]={-99};         itree->SetBranchAddress("ele",             ele);
      Float_t elvetoid[110]={-99};   itree->SetBranchAddress("elvetoid",      elvetoid);

      Int_t   nlmu=0;                itree->SetBranchAddress("nlmu",          &nlmu);
      Float_t lmupt[110]={-99};      itree->SetBranchAddress("lmupt",         lmupt);
      Float_t lmueta[110]={-99};     itree->SetBranchAddress("lmueta",        lmueta);
      Float_t lmuphi[110]={-99};     itree->SetBranchAddress("lmuphi",        lmuphi);
      Float_t lmue[110]={-99};       itree->SetBranchAddress("lmue",          lmue);
      Float_t muvetoid[110]={-99};   itree->SetBranchAddress("muvetoid",      muvetoid);

      Int_t nmu={-99};               itree->SetBranchAddress("nmu",            &nmu);
      Float_t mupt[10]={-99};        itree->SetBranchAddress("mupt",           mupt);
      Float_t mueta[10]={-99};       itree->SetBranchAddress("mueta",         mueta);
      Float_t muphi[10]={-99};       itree->SetBranchAddress("muphi",         muphi);
      Float_t mue[10]={-99};         itree->SetBranchAddress("mue",             mue);

      Int_t   njt=-99;               itree->SetBranchAddress("njt",            &njt);
      Float_t jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           jtpt);
      Float_t jteta[40]={-99};       itree->SetBranchAddress("jteta",         jteta);
      Float_t jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         jtphi);
      Float_t jte[40]={-99};         itree->SetBranchAddress("jte",             jte);

      Int_t   nvetoel=-99;           itree->SetBranchAddress("nvetoel",              &nvetoel);
      Int_t   nvetomu=-99;           itree->SetBranchAddress("nvetomu",              &nvetomu);
     
      // loop over all events and recalculate jet pT and energy; no reordering!
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      THStatus* status = new THStatus(nevt);

      cout<<nevt<<" events will be processed"<<endl;

      for (unsigned ievt=0;ievt<nevt;ievt++) {

	status->print_status(ievt);
	itree->GetEntry(ievt);

	Int_t posnvetomu=-1;
	Int_t posnvetoel=-1;
	
	for(Int_t i=0; i<nlmu; i++)
	  { 
	    if(muvetoid[i]){
	      // Save position of hardest nvetomu
	      if(posnvetomu == -1){
		posnvetomu = i;
	      }
	    }
	  }

	for(Int_t i=0; i<nlel; i++)
	  {
	    if(elvetoid[i]){
	      if(posnvetoel == -1){
		posnvetoel = i;
	      }
	    }
	  }

	bool has_lel = nvetoel==1;
	bool has_lmu = nvetomu==1;

	TLorentzVector p4nu;
	TLorentzVector lep;
	TLorentzVector p4W;

	Int_t sysindex;
	
	if(0==systematic){
	  omet  = met;
	  ometx = metx;
	  omety = mety;
	}
	else{
	  systematic == 1 ? sysindex=0 : sysindex=1;
	  omet  = metsys[sysindex];
	  ometx = metsysx[sysindex];
	  omety = metsysy[sysindex];
	}
	
	// Extended the acceptance of this part for the QCD studies, since we ask explicitly for nveto_el == 1 && niso_el == 0                                                                           
	if (has_lmu||has_lel) {
	  if (has_lmu && has_lel){
	    if(lelpt[posnvetoel]>lmupt[posnvetomu]) lep.SetPtEtaPhiE(lelpt[posnvetoel],leleta[posnvetoel],lelphi[posnvetoel],lele[posnvetoel]);
	    else lep.SetPtEtaPhiE(lmupt[posnvetomu],lmueta[posnvetomu],lmuphi[posnvetomu],lmue[posnvetomu]);
	  }
	  else if (has_lel) lep.SetPtEtaPhiE(lelpt[posnvetoel],leleta[posnvetoel],lelphi[posnvetoel],lele[posnvetoel]);
	  else if (has_lmu) lep.SetPtEtaPhiE(lmupt[posnvetomu],lmueta[posnvetomu],lmuphi[posnvetomu],lmue[posnvetomu]);
	  
	  float nu_e  = omet;
	  float nu_px = ometx;
	  float nu_py = omety;
	  
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
	  
	  nlepw       = 1;
	  
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
	  
	  
	  //Calculate sphericity and aplanarity
	  
	  double denominator=0;
	  double numXX=0;
	  double numYY=0;
	  double numZZ=0;
	  double numXY=0;
	  double numXZ=0;
	  double numYZ=0;
	  
	  denominator+=pow(lep.P(),2) + pow(p4nu.P(),2);
	  numXX+=pow(lep.X(), 2)+pow(p4nu.X(), 2);
	  numYY+=pow(lep.Y(), 2)+pow(p4nu.Y(), 2);
	  numZZ+=pow(lep.Z(), 2)+pow(p4nu.Z(), 2);
	  numXY+=lep.X()*lep.Y()+p4nu.X()*p4nu.Y();
	  numXZ+=lep.X()*lep.Z()+p4nu.X()*p4nu.Z();
	  numYZ+=lep.Y()*lep.Z()+p4nu.Y()*p4nu.Z();
	  
	  
	  for (Int_t iJet=0; iJet<njt; ++iJet)
	    {
	      TLorentzVector Jet;
	      Jet.SetPtEtaPhiE(jtpt[iJet],jteta[iJet],jtphi[iJet],jte[iJet]);
	      denominator += pow(Jet.P(),2);
	      numXX+=pow(Jet.X(), 2);
	      numYY+=pow(Jet.Y(), 2);
	      numZZ+=pow(Jet.Z(), 2);
	      numXY+=Jet.X()*Jet.Y();
	      numXZ+=Jet.X()*Jet.Z();
	      numYZ+=Jet.Y()*Jet.Z();
	    }
	  
	  TMatrix spherTens(3, 3);
	  spherTens(0, 0) = numXX/denominator;
	  spherTens(1, 1) = numYY/denominator; //cout << numYY/denominator << endl;
	  spherTens(2, 2) = numZZ/denominator; //cout << numZZ/denominator << endl;
	  spherTens(0, 1) = numXY/denominator; //cout << numXY/denominator << endl;
	  spherTens(1, 0) = numXY/denominator; //cout << numXX/denominator << endl;
	  spherTens(0, 2) = numXZ/denominator; //cout << numXZ/denominator << endl;
	  spherTens(2, 0) = numXZ/denominator; //cout << numXX/denominator << endl;
	  spherTens(1, 2) = numYZ/denominator; //cout << numYZ/denominator << endl;
	  spherTens(2, 1) = numYZ/denominator; //cout << numXX/denominator << endl;
	  
	  
	  TVector eigenval(3);
	  
	  spherTens.EigenVectors(eigenval);
	
	  sphericity=0;
	  aplanarity=0;

	  if (eigenval(0)<=eigenval(1) && eigenval(1)<=eigenval(2))
	    {
	      sphericity = 3.0*(eigenval(0)+eigenval(1))/2.0;
	      aplanarity = 3.0*eigenval(0)/2.0;
	    }
	  if (eigenval(0)<=eigenval(2) && eigenval(2)<=eigenval(1))
	    {
	      sphericity = 3.0*(eigenval(0)+eigenval(2))/2.0;
	      aplanarity = 3.0*eigenval(0)/2.0;
	    }
	  if (eigenval(1)<=eigenval(0) && eigenval(0)<=eigenval(2))
	    {
	      sphericity = 3.0*(eigenval(1)+eigenval(0))/2.0;
	      aplanarity = 3.0*eigenval(1)/2.0;
	    }
	  if (eigenval(1)<=eigenval(2) && eigenval(2)<=eigenval(0))
	    {
	      sphericity = 3.0*(eigenval(1)+eigenval(2))/2.0;
	      aplanarity = 3.0*eigenval(1)/2.0;
	    }
	  if (eigenval(2)<=eigenval(0) && eigenval(0)<=eigenval(1))
	    {
	      sphericity = 3.0*(eigenval(2)+eigenval(0))/2.0;
	      aplanarity = 3.0*eigenval(2)/2.0;
	    }
	  if (eigenval(2)<=eigenval(1) && eigenval(1)<=eigenval(0))
	    {
	      sphericity = 3.0*(eigenval(2)+eigenval(1))/2.0;
	      aplanarity = 3.0*eigenval(2)/2.0;
	    }
	  

	  //cout <<"1:" <<sphericity << endl;
	  //sphericity=4;
	  //aplanarity = 5;
	  //cout <<"2: "<< sphericity << endl; 

	  //otree->Fill();
	  b_met->Fill();
	  b_metx->Fill();
	  b_mety->Fill();
	  b_nlepw->Fill();
	  b_lepwm->Fill();
	  b_lepwpt->Fill();
	  b_lepwphi->Fill();
	  b_lepweta->Fill();
	  b_lepwmt->Fill();
	  b_lepwet->Fill();
	  b_lepwdphi->Fill();
	  b_sphericity->Fill();
	  b_aplanarity->Fill();
	  
	}
	else
	  {
	    nlepw    = 0;
	    lepwpt   = -999.;
	    lepweta  = -999.;
	    lepwphi  = -999.;
	    lepwet   = -999.;
	    lepwmt   = -999.;
	    lepwdphi = -999.;
	    lepwm    = -999.;
	    sphericity=-999.;
	    aplanarity=-999.;
	  }
      } // for ievt<nevt
      
    } // while in keys
    
    cout<<"Save extended tree to "<<output.substr(0,output.find(".met.root"))<<".root ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv " + output + " " 
      + output.substr(0,output.find(".met.root")) + ".root";
    gSystem->Exec(cmd.c_str());
  } // for iinput

  cout<<"DONE!\n"<<endl;

  return 0;
}
