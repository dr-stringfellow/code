////////////////////////////////////////////////////////////////////////////////
//
// th_debug_x
// -----------------
//
//            29/01/2014 Christian Boeser <christian.boser@cern.ch>
//                                                    
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"

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
  
  string    input       = cl.getValue<string> ("input");
  string    treename    = cl.getValue<string> ("treename",  "t");
  string    dirname     = cl.getValue<string> ("dirname", "utm");
  vector<int>  debugevent  = cl.getVector<int>   ("events", "");
  int debugrun   = cl.getValue<int>   ("run",  -99);
  int debuglumi  = cl.getValue<int>   ("lumi", -99);
  int skipevents = cl.getValue<int>   ("skipevents", 0);

  if (!cl.check()) return 0;
  cl.print();

  // open input file
  TFile* ifile = new TFile(input.c_str(),"READ");
  if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }
  
  // open input tree
  TTree* itree = (TTree*)ifile->Get((dirname+"/"+treename).c_str());
  if(0==itree){cout<<"No "<<treename<<" in "<<input<<":"<<dirname<<endl;return 0;}
  
  Int_t   njt=0;                   itree->SetBranchAddress("njt",           &njt);
  Float_t jtpt[140]={-99};         itree->SetBranchAddress("jtpt",          jtpt);
  Float_t jtrawpt[140]={-99};      itree->SetBranchAddress("jtrawpt",       jtrawpt);
  Float_t jteta[140]={-99};        itree->SetBranchAddress("jteta",         jteta);
  Float_t jtphi[140]={-99};        itree->SetBranchAddress("jtphi",         jtphi);
  Float_t jte[140]={-99};          itree->SetBranchAddress("jte",           jte);
  Float_t sumHt = -99;             itree->SetBranchAddress("sumHt",         &sumHt);
  Short_t jtgenflv[140]={-99};     itree->SetBranchAddress("jtgenflv",      jtgenflv);
  Float_t jtgenpt[140]={-99};      itree->SetBranchAddress("jtgenpt",       jtgenpt);
  Float_t jtgeneta[140]={-99};     itree->SetBranchAddress("jtgeneta",      jtgeneta);
  Float_t jtgenphi[140]={-99};     itree->SetBranchAddress("jtgenphi",      jtgenphi);
  Float_t jtgene[140]={-99};       itree->SetBranchAddress("jtgene",        jtgene);
  Float_t jtcsvt[140]={-99};       itree->SetBranchAddress("jtcsvt",        jtcsvt);
  Float_t jtcsvtcorr[140][5];      itree->SetBranchAddress("jtcsvtcorr",    jtcsvtcorr);
  Float_t jtcsvtcorr4p[140][4];    itree->SetBranchAddress("jtcsvtcorr4p",  jtcsvtcorr4p);
  Float_t jtntracks[140]={-99};    itree->SetBranchAddress("jtntracks",     jtntracks);
  Float_t jtarea[140]={-99};       itree->SetBranchAddress("jtarea",        jtarea);
  Float_t jtpull[140]={-99};       itree->SetBranchAddress("jtpull",        jtpull);
  Float_t jtcharge[140]={-99};     itree->SetBranchAddress("jtcharge",      jtcharge);
  Bool_t  jtid[140]={0};           itree->SetBranchAddress("jtid",          jtid);
  Float_t jtchhadmult[140]={-99};  itree->SetBranchAddress("jtchhadmult",   jtchhadmult);
  Float_t jtcsvsf_l[140]={-99};    itree->SetBranchAddress("jtcsvsf_l",     jtcsvsf_l);
  Float_t jtcsvsf_m[140]={-99};    itree->SetBranchAddress("jtcsvsf_m",     jtcsvsf_m);
  Float_t jtcsvsf_t[140]={-99};    itree->SetBranchAddress("jtcsvsf_t",     jtcsvsf_t);
  Float_t jtcsvsf_lerr[140]={-99}; itree->SetBranchAddress("jtcsvsf_lerr",  jtcsvsf_lerr);
  Float_t jtcsvsf_merr[140]={-99}; itree->SetBranchAddress("jtcsvsf_merr",  jtcsvsf_merr);
  Float_t jtcsvsf_terr[140]={-99}; itree->SetBranchAddress("jtcsvsf_terr",  jtcsvsf_terr);
  
  //LEPTONS: FIRST LOOSE THEN ISOLATED.                                                                                                                                               
  
  Int_t   nlmu;                    itree->SetBranchAddress("nlmu",          &nlmu);
  Float_t lmupt[110]={-99};        itree->SetBranchAddress("lmupt",         lmupt);
  Float_t lmueta[110]={-99};       itree->SetBranchAddress("lmueta",        lmueta);
  Float_t lmuphi[110]={-99};       itree->SetBranchAddress("lmuphi",        lmuphi);
  Float_t lmuiso[110]={-99};       itree->SetBranchAddress("lmuiso",        lmuiso);
  Float_t lmuiso_dBeta[110]={-99}; itree->SetBranchAddress("lmuiso_dBeta",  lmuiso_dBeta);
  Float_t lmue[110]={-99};         itree->SetBranchAddress("lmue",          lmue);
  Float_t lmucharge[110]={-99};    itree->SetBranchAddress("lmucharge",     lmucharge);
  Float_t musigid[110]={-99};      itree->SetBranchAddress("musigid",       musigid);
  Float_t muvetoid[110]={-99};     itree->SetBranchAddress("muvetoid",      muvetoid);
  Int_t   nvetomu={-99};           itree->SetBranchAddress("nvetomu",  &nvetomu);

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

  Int_t   nvetoel={-99};           itree->SetBranchAddress("nvetoel",       &nvetoel);
  Int_t   nel={-99};               itree->SetBranchAddress("nel",           &nel);
  Float_t elpt[110]={-99};         itree->SetBranchAddress("elpt",          elpt);
  Float_t eleta[110]={-99};        itree->SetBranchAddress("eleta",         eleta);
  Float_t elphi[110]={-99};        itree->SetBranchAddress("elphi",         elphi);
  Float_t eliso[110]={-99};        itree->SetBranchAddress("eliso",         eliso);
  Float_t ele[110]={-99};          itree->SetBranchAddress("ele",           ele);
  Float_t elcharge[110]={-99};     itree->SetBranchAddress("elcharge",      elcharge);
  Float_t elmvaid[110]={-99};      itree->SetBranchAddress("elmvaid",       elmvaid);
  Float_t elsigid[110]={-99};      itree->SetBranchAddress("elsigid",       elsigid);
  Float_t elvetoid[110]={-99};     itree->SetBranchAddress("elvetoid",      elvetoid);
  //MET + W BOSON                                                                                                                                                                     

  Float_t met={-99};               itree->SetBranchAddress("met",           &met);
  Float_t uncorrmet={-99};         itree->SetBranchAddress("uncorrmet",     &uncorrmet);
  Float_t metphi={-99};            itree->SetBranchAddress("metphi",        &metphi);
  Float_t uncorrmetphi={-99};      itree->SetBranchAddress("uncorrmetphi",  &uncorrmetphi);
  Float_t meteta={-99};            itree->SetBranchAddress("meteta",        &meteta);
  Float_t metx={-99};              itree->SetBranchAddress("metx",          &metx);
  Float_t mety={-99};              itree->SetBranchAddress("mety",          &mety);
  Float_t metsig=-99;              itree->SetBranchAddress("metsig",        &metsig);
  Float_t metsumet=-99;            itree->SetBranchAddress("metsumet",      &metsumet);
  // unclustered met shift [0]=up, [1]=down:                                                                                                                                          
  Float_t metsysx[2]={-99,-99};    itree->SetBranchAddress("metsysx",       metsysx);
  Float_t metsysy[2]={-99,-99};    itree->SetBranchAddress("metsysy",       metsysy);
  Float_t metsys[2]={-99,-99};     itree->SetBranchAddress("metsys",        metsys);

  UChar_t nlepw={-99};             itree->SetBranchAddress("nlepw",         &nlepw);
  Float_t lepwm={-99};             itree->SetBranchAddress("lepwm",         &lepwm);
  Float_t lepwpt={-99};            itree->SetBranchAddress("lepwpt",        &lepwpt);
  Float_t lepwphi={-99};           itree->SetBranchAddress("lepwphi",       &lepwphi);
  Float_t lepweta={-99};           itree->SetBranchAddress("lepweta",       &lepweta);
  Float_t lepwet={-99};            itree->SetBranchAddress("lepwet",        &lepwet);
  Float_t lepwmt={-99};            itree->SetBranchAddress("lepwmt",        &lepwmt);
  Float_t lepwdphi={-99};          itree->SetBranchAddress("lepwdphi",      &lepwdphi);
  Float_t sphericity=-99;          itree->SetBranchAddress("sphericity",    &sphericity);
  Float_t aplanarity=-99;          itree->SetBranchAddress("aplanarity",    &aplanarity);

  Float_t MassDeviation={-99};     itree->SetBranchAddress("MassDeviation", &MassDeviation);

  Float_t hbbm={-99};              itree->SetBranchAddress("hbbm",          &hbbm);
  Float_t hbbpt={-99};             itree->SetBranchAddress("hbbpt",         &hbbpt);
  Float_t hbbphi={-99};            itree->SetBranchAddress("hbbphi",        &hbbphi);
  Float_t hbbeta={-99};            itree->SetBranchAddress("hbbeta",        &hbbeta);
  Float_t hbbdr={-99};             itree->SetBranchAddress("hbbdr",         &hbbdr);
  Int_t   hbbjtidx[3]={-99};       itree->SetBranchAddress("hbbjtidx",      hbbjtidx);


  Float_t topm={-99};              itree->SetBranchAddress("topm",          &topm);
  Float_t toppt={-99};             itree->SetBranchAddress("toppt",         &toppt);
  Float_t topphi={-99};            itree->SetBranchAddress("topphi",        &topphi);
  Float_t topeta={-99};            itree->SetBranchAddress("topeta",        &topeta);
  Int_t   topjtidx={-99};          itree->SetBranchAddress("topjtidx",      &topjtidx);

  Float_t costst={-99};            itree->SetBranchAddress("costst",        &costst);
  Float_t coststh={-99};           itree->SetBranchAddress("coststh",       &coststh);

  Bool_t  hlt_isomu24_eta2p1=0;     itree->SetBranchAddress("hlt_isomu24_eta2p1",&hlt_isomu24_eta2p1);

  Bool_t  hlt_isomu24=0;            itree->SetBranchAddress("hlt_isomu24",       &hlt_isomu24);

  Bool_t  hlt_ele27_wp80=0;         itree->SetBranchAddress("hlt_ele27_wp80",    &hlt_ele27_wp80);

  Float_t weightTrig=1;            itree->SetBranchAddress("weightTrig",    &weightTrig);
  Float_t weightPU=1;              itree->SetBranchAddress("weightPU",      &weightPU);
  Float_t weightPU_up=1;           itree->SetBranchAddress("weightPU_up",   &weightPU_up);
  Float_t weightPU_down=1;         itree->SetBranchAddress("weightPU_down", &weightPU_down);

  // MISCALLENEOUS                                                                                                                                                                   

  UInt_t   run=-99;              itree->SetBranchAddress("run",           &run);
  ULong64_t evt=-99;             itree->SetBranchAddress("evt",           &evt);
  UInt_t  lbn=-99;               itree->SetBranchAddress("lbn",           &lbn);
  Float_t poissonmean=1;           itree->SetBranchAddress("poissonmean",   &poissonmean);
  UChar_t npv=-99;               itree->SetBranchAddress("npv",            &npv);
  //For HBHE, Scraping, OptionalMETFilters                                                                                                                                            
  Bool_t passpresel=1;             itree->SetBranchAddress("passpresel",    &passpresel);
  
  cout << "Debugging";
  if(debugrun != -99) {cout << " in run "<< debugrun;}
  if(debuglumi != -99) {cout << " in lbn " << debuglumi;}
  cout << " the events: " << endl;
  for(UInt_t deb=0; deb<debugevent.size(); deb++){
    cout << debugevent.at(deb) << endl;
  }
  
  cout << endl;
  unsigned nevt = static_cast<unsigned>(itree->GetEntries());
  cout<<nevt<<" events will be processed"<<endl;
  
  for (unsigned ievt=0;ievt<nevt;ievt++) {
    if(ievt==0) ievt += skipevents;
    if(ievt < 10 || ievt%10000==0)
      cout << "Processing event " << ievt << endl;
    itree->GetEntry(ievt);
    bool debevt = true;
    if(!debugevent.empty()){
      debevt = false;
      if(debugrun == -99 || run == debugrun){
	if(debuglumi == -99 || lbn == debuglumi){       
	  for(UInt_t deb=0; deb<debugevent.size(); deb++){
	    if(evt == debugevent.at(deb) || debugevent.at(deb)< 0){
	      debevt = true;
	      cout << "----------------------------------------------------------" << endl;
	      cout << "Debugging Event " << evt << ", Run " << run << ", LumiSection " << lbn << endl;
	    }
	  }
	}
      }
    }
    else continue;
    
    if(!debevt) continue;
    int test =  static_cast<unsigned>(npv);

    if(!debugevent.empty()){
      cout << "passEventFilter " <<  passpresel << " : NPV " << test << endl;
      cout << "hlt_isomu24_eta2p1 " <<  hlt_isomu24_eta2p1 << endl << endl;
      
      cout << "MET : " << uncorrmet << " : phi : " << uncorrmetphi << endl << endl;
      cout << "MET type-0, type 1 and phi corrected : " <<  met << " : phi : " << metphi << endl << endl;
      for(int ijt=0; ijt<njt; ijt++){
	cout << "jet " << ijt << " : pt " <<  jtpt[ijt] << " : eta " << jteta[ijt] << " : phi " << jtphi[ijt]  << " : CSV " << jtcsvt[ijt] << " : genpt " << jtgenpt[ijt] << endl;
      }
      cout << endl;
      for(Int_t imu=0; imu<nlmu; imu++){
	cout << "muon " << imu << " : pt " <<  lmupt[imu] << " : eta " << lmueta[imu] << " : phi " << lmuphi[imu] << endl;
	cout << "       : reliso " << lmuiso_dBeta[imu] << " : ID " << musigid[imu] << " : vetoID " << muvetoid[imu] << endl;
      }
      cout << endl;
      for(Int_t iel=0; iel<nlel; iel++){
	cout << "elec " << iel << " : pt " <<  lelpt[iel] << " : eta " << leleta[iel] << " : phi " << lelphi[iel] <<  endl;
	cout << "       : reliso " << leliso[iel] <<  " : ID " << elsigid[iel] <<  " : vetoID " <<  elvetoid[iel] << " : MVA " << lelmvaid[iel] << endl;
      }
      cout << endl;     
    }
  }

  return 0;

}
    

