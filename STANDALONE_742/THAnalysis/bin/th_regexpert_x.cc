////////////////////////////////////////////////////////////////////////////////
//
// th_regexpert_x
// ------------
//
// tool to train boosted decision trees (TMVA package) - requires nothin' but root
// this version has been trimmed to only work with MCs
// 
//
//    ::21/08/2012 Christian Boeser
////////////////////////////////////////////////////////////////////////////////


#include "../interface/MCSample.h"
//#include "SchieferD/AnaUtils/src/Weights.h"
#include "../interface/CommandLine.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <map>

#include "TH1F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TKey.h"
#include "TControlBar.h"
#include "TSystem.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TObjString.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"
#include "TPluginManager.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace std;


int main(int argc,char**argv)
{
  // This loads the library
  
  TMVA::Tools::Instance();
  
  
  // Default MVA methods to be trained + tested
  std::map<std::string,int> Use;


  Use["BDT"]             = 1;
  Use["BDTG"]            = 0;


   
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  string         input      = cl.getValue <string>  ("input");
  string         dirname    =  cl.getValue<string> ("dirname", "utm");
  string         treename   = cl.getValue <string>  ("treename",        "t");
  string         ipath      = cl.getValue <string>  ("ipath",              ".");
  string         opath      = cl.getValue <string>  ("opath",              ".");
  string         bdtpath    = cl.getValue <string>  ("bdtpath",            ".");
  int            btagsys    = cl.getValue <int>     ("btagsys",              0);


  if(!cl.check()) {};
  cl.print();
  
  assert (std::abs(btagsys)<5);
  
  if      (0==btagsys) cout<<"Nominal mode"<<endl;
  else if (1==btagsys) cout<<"Systematic BTAG UP"<<endl;
  else if (2==btagsys) cout<<"Systematic BTAG DOWN"<<endl;
  else if (3==btagsys) cout<<"Systematic MISTAG UP"<<endl;
  else if (4==btagsys) cout<<"Systematic MISTAG DOWN"<<endl;

   std::cout << "==> Start TMVARegressionApplication" << std::endl;

   // --- Create the Reader object

   TMVA::Reader *reader0 = new TMVA::Reader( "!Color:!Silent" );
   //   TMVA::Reader *reader1 = new TMVA::Reader( "!Color:!Silent" );
   //   TMVA::Reader *reader2 = new TMVA::Reader( "!Color:!Silent" );

   // Create a set of variables and declare them to the reader                                    
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   Float_t var1=0.,var2=0.,var3=0.,var4=0.,var5=0.,var6=0.,var7=0.,var8=0.;

   reader0->AddVariable( "jtpt", &var1);
   reader0->AddVariable( "jteta", &var2);
   reader0->AddVariable( "jtrawpt", &var3);
   reader0->AddVariable( "jte", &var4);
   reader0->AddVariable( "jtntracks", &var5);
   reader0->AddVariable( "jtarea", &var6);
   reader0->AddVariable( "jtcharge", &var7);
   reader0->AddVariable( "jtchhadmult", &var8);


   // --- Book the MVA methods

      TString prefix = "TMVARegression";

   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = it->first + " method";
	 TString weightfile = bdtpath + "/TMVARegression_BDT.weights.xml";
	 // TString weightfile1 = bdtpath + "/TMVARegression_sjf_BDT.weights1.xml";
	 //TString weightfile2 = bdtpath + "/TMVARegression_sjf_BDT.weights2.xml";
         
	 //	 TString weightfile0 = bdtpath + "/" + prefix + "0_" + TString(it->first) + ".weights.xml";
         //TString weightfile1 = bdtpath + "/"  + prefix + "1_" + TString(it->first) + ".weights.xml";
         //TString weightfile2 = bdtpath + "/"  + prefix + "2_" + TString(it->first) + ".weights.xml";

         reader0->BookMVA( methodName, weightfile ); 
      }
   }


   // Prepare input tree (this must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //   

   // read input file                                         
   cout << (ipath+"/"+input).c_str() << endl;
   TFile* ifile = new TFile((ipath+"/"+input).c_str(),"READ");
   if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }

   // create temporary output file                                                                                                                     
   size_t pos = input.find(".root");
   string output= input.substr(0,pos);
   output = output + ".reg.root";
   TFile* ofile = new TFile((opath+"/"+output).c_str(),"RECREATE");
   if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }


   // read input directory                                                                                                                                                                   
   TDirectory* idir    = (TDirectory*)ifile->Get(dirname.c_str());
   cout<<input<<"/"<<dirname<<":"<<endl;
    
   // create output directory signal                                                                                                                                                     
   TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
   if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}
    


   // open input tree                                                                                                                                  
   TTree* itree = (TTree*)idir->Get(treename.c_str());
   if(0==itree){cout<<"No "<<treename<<" in " << output <<":" << endl;return 0;}


   
   // Prepare the tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop   //


   // if corrfacs in the tree, sample already corrected! -> Abort

   if (0!=itree->FindBranch("jtcorrfac")){
     cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n!!! SAMPLE ALREADY CORRECTED !!!\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
     return 0;
   }
   
   
   if (0!=itree->FindBranch("jtpt")) itree->SetBranchStatus("jtpt",0);
   if (0!=itree->FindBranch("jte"))  itree->SetBranchStatus("jte",0);
   if (0!=itree->FindBranch("hbbm")) itree->SetBranchStatus("hbbm",0);
   //if (0!=itree->FindBranch("hbbe"))  itree->SetBranchStatus("hbbe",0);
   if (0!=itree->FindBranch("hbbpt"))  itree->SetBranchStatus("hbbpt",0);
   if (0!=itree->FindBranch("topm")) itree->SetBranchStatus("topm",0);
   //if (0!=itree->FindBranch("tope"))  itree->SetBranchStatus("tope",0);
   if (0!=itree->FindBranch("toppt"))  itree->SetBranchStatus("toppt",0);
   
   
   odir->cd();
   TTree* otree = itree->CloneTree(0);

   
   itree->SetBranchStatus("jtpt",1);
   itree->SetBranchStatus("jte",1);
   itree->SetBranchStatus("hbbm",1);
   //itree->SetBranchStatus("hbbe",1);
   itree->SetBranchStatus("hbbpt",1);
   itree->SetBranchStatus("topm",1);
   itree->SetBranchStatus("toppt",1);
   //itree->SetBranchStatus("tope",1);
   

   Int_t njt; itree->SetBranchAddress("njt", &njt);
   Int_t hbbjtidx[3]; itree->SetBranchAddress("hbbjtidx", hbbjtidx);
   Int_t topjtidx; itree->SetBranchAddress("topjtidx", &topjtidx);
   Float_t i_jtpt[140]; itree->SetBranchAddress("jtpt", i_jtpt);
   Float_t i_jteta[140]; itree->SetBranchAddress("jteta", i_jteta);
   Float_t i_jtrawpt[140]; itree->SetBranchAddress("jtrawpt", i_jtrawpt);
   Float_t i_jte[140]; itree->SetBranchAddress("jte", i_jte);
   Float_t i_jtntracks[140]; itree->SetBranchAddress("jtntracks", i_jtntracks);
   Float_t i_jtcharge[140]; itree->SetBranchAddress("jtcharge", i_jtcharge);
   Float_t i_jtarea[140]; itree->SetBranchAddress("jtarea", i_jtarea);
   Float_t i_jtchhadmult[140]; itree->SetBranchAddress("jtchhadmult", i_jtchhadmult);

   Float_t i_jtphi[140]; itree->SetBranchAddress("jtphi", i_jtphi);
   Float_t i_lepweta; itree->SetBranchAddress("lepweta", &i_lepweta);
   Float_t i_lepwpt; itree->SetBranchAddress("lepwpt", &i_lepwpt);
   Float_t i_lepwphi; itree->SetBranchAddress("lepwphi", &i_lepwphi);
   Float_t i_lepwm; itree->SetBranchAddress("lepwm", &i_lepwm);

   // Create branches in the output tree

   Int_t idx=3;

   Float_t o_jtpt[140];       TBranch* b_jtpt      = otree->Branch("jtpt", o_jtpt, "jtpt[njt]/F");
   Float_t o_jte[140];        TBranch* b_jte       = otree->Branch("jte", o_jte, "jte[njt]/F");
   Float_t o_jtcorrfac[3]={0.,0.,0.};    TBranch* b_jtcorrfac = otree->Branch("jtcorrfac", o_jtcorrfac, "jtcorrfac[3]/F"); 
   Float_t o_hbbpt;           TBranch* b_hbbpt     = otree->Branch("hbbpt", &o_hbbpt, "hbbpt/F");
   Float_t o_hbbm;            TBranch* b_hbbm      = otree->Branch("hbbm", &o_hbbm, "hbbm/F");
   Float_t o_hbbe;            TBranch* b_hbbe      = otree->Branch("hbbe", &o_hbbe, "hbbe/F");
   Float_t o_toppt;           TBranch* b_toppt     = otree->Branch("toppt", &o_toppt, "toppt/F");
   Float_t o_topm;            TBranch* b_topm      = otree->Branch("topm", &o_topm, "topm/F");
   Float_t o_tope;            TBranch* b_tope      = otree->Branch("tope", &o_tope, "tope/F");
   
   
   TLorentzVector hjt1c,hjt2c,tjtc,higgsc,lepw,topc;

   std::cout << "--- Processing: " << itree->GetEntries() << " events" << std::endl;

   TStopwatch sw;
   sw.Start();

   for (Long64_t ievt=0; ievt < itree->GetEntries();ievt++) {

      if (ievt%1000 == 0) {
	std::cout << "--- ... Processing event: " << ievt << std::endl;
      }

      itree->GetEntry(ievt);

      for(int i =0; i<140; i++)
	{
	  o_jtpt[i]=i_jtpt[i];
	  o_jte[i]=i_jte[i];
	}

      var1 = i_jtpt[hbbjtidx[0]];
      var2 = i_jteta[hbbjtidx[0]];
      var3 = i_jtrawpt[hbbjtidx[0]];
      var4 = i_jte[hbbjtidx[0]];
      var5 = i_jtntracks[hbbjtidx[0]];
      var6 = i_jtarea[hbbjtidx[0]];
      var7 = i_jtcharge[hbbjtidx[0]];
      var8 = i_jtchhadmult[hbbjtidx[0]];
      
      
      o_jtcorrfac[0] = (reader0->EvaluateRegression("BDT method"))[0];
      
      var1 = i_jtpt[hbbjtidx[1]];
      var2 = i_jteta[hbbjtidx[1]];
      var3 = i_jtrawpt[hbbjtidx[1]];
      var4 = i_jte[hbbjtidx[1]];
      var5 = i_jtntracks[hbbjtidx[1]];
      var6 = i_jtarea[hbbjtidx[1]];
      var7 = i_jtcharge[hbbjtidx[1]];
      var8 = i_jtchhadmult[hbbjtidx[1]];
      
      o_jtcorrfac[1] = (reader0->EvaluateRegression("BDT method"))[0];
      

      var1 = i_jtpt[topjtidx];
      var2 = i_jteta[topjtidx];
      var3 = i_jtrawpt[topjtidx];
      var4 = i_jte[topjtidx];
      var5 = i_jtntracks[topjtidx];
      var6 = i_jtarea[topjtidx];
      var7 = i_jtcharge[topjtidx];
      var8 = i_jtchhadmult[topjtidx];
      
      o_jtcorrfac[2] = (reader0->EvaluateRegression("BDT method"))[0];


      o_jtpt[hbbjtidx[0]] = o_jtcorrfac[0]*i_jtpt[hbbjtidx[0]];
      o_jtpt[hbbjtidx[1]] = o_jtcorrfac[1]*i_jtpt[hbbjtidx[1]];
      o_jtpt[topjtidx] = o_jtcorrfac[2]*i_jtpt[topjtidx];
      o_jte[hbbjtidx[0]] = o_jtcorrfac[0]*i_jte[hbbjtidx[0]];
      o_jte[hbbjtidx[1]] = o_jtcorrfac[1]*i_jte[hbbjtidx[1]];
      o_jte[topjtidx] = o_jtcorrfac[2]*i_jte[topjtidx];
      
      
      
      hjt1c.SetPtEtaPhiE(o_jtpt[hbbjtidx[0]],i_jteta[hbbjtidx[0]],i_jtphi[hbbjtidx[0]],o_jte[hbbjtidx[0]]);
      hjt2c.SetPtEtaPhiE(o_jtpt[hbbjtidx[1]],i_jteta[hbbjtidx[1]],i_jtphi[hbbjtidx[1]],o_jte[hbbjtidx[1]]);
      topc.SetPtEtaPhiE(o_jtpt[topjtidx],i_jteta[topjtidx],i_jtphi[topjtidx],o_jte[topjtidx]);
      
      higgsc = hjt1c+hjt2c;
      lepw.SetPtEtaPhiM(i_lepwpt,i_lepweta,i_lepwphi,i_lepwm);
      topc = lepw+topc;

      TLorentzVector beforejt1;
      TLorentzVector beforejt2; 
      TLorentzVector beforehiggs; 
      beforejt1.SetPtEtaPhiE(i_jtpt[hbbjtidx[0]],i_jteta[hbbjtidx[0]],i_jtphi[hbbjtidx[0]],i_jte[hbbjtidx[0]]);
      beforejt2.SetPtEtaPhiE(i_jtpt[hbbjtidx[1]],i_jteta[hbbjtidx[1]],i_jtphi[hbbjtidx[1]],i_jte[hbbjtidx[1]]);
      beforehiggs = beforejt1+beforejt2;

      //cout << "Higgs Masse vorher: " << beforehiggs.M() << endl;
      //cout << "Higgs Masse nachhr: " << higgsc.M() << endl;
      
      o_hbbm = higgsc.M();
      o_hbbpt = higgsc.Pt();
      o_hbbe = higgsc.E();


      

      o_topm = topc.M();
      o_toppt = topc.Pt();
      o_tope = topc.E();
 
      //b_fathFilterJets_pt->Fill();
      //b_fathFilterJets_e->Fill();
      //b_fathFilterJets_corrfac->Fill();
      //b_FatH->Fill();
      otree->Fill();

   }


   delete reader0;
   //   delete reader1;
   //   delete reader2;
   sw.Stop();
   cout << "Wrote " << otree->GetEntries() << " in new tree\n";
   std::cout << "--- End of event loop: "; sw.Print();

   // --- Write histograms



  // create bdtout branches in output tree

   
   otree->Write();
   ofile->Close();
   ifile->Close();

   std::cout << "--- Created root file: \"" << ofile->GetName() 
	     << "\" containing the MVA output histograms" << std::endl;
     
   delete ifile;
   delete ofile;

   TString cmd_str = "mv " + opath + "/" + output + " " + opath + "/" + input;
   gSystem->Exec(cmd_str);
   std::cout << "==> TMVARegressionApplication is done!" << std::endl << std::endl;   

   return 0;
}
