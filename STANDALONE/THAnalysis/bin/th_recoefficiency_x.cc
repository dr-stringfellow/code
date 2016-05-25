////////////////////////////////////////////////////////////////////////////////
//
// th_hypotheses_x
// -----------------
//
//            04/07/2013 Christian Boeser 
//
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TEventList.h>
#include <TLorentzVector.h>
#include <TBranch.h>
#include <TError.h>

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <map>


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
////////////////////////////////////////////////////////////////////////////////
Float_t Matchable(const TLorentzVector & gen_obj, const TLorentzVector & rec_obj);
Float_t GetChiSquared(const TLorentzVector & top, const TLorentzVector & higgs);
static double DeltaR(const TLorentzVector & v1, const TLorentzVector & v2);
int count(int num, int* arr, int length);


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
  string         treename    = cl.getValue<string> ("treename",        "t");
  string         dirname     = cl.getValue<string> ("dirname",       "utm");
  bool           bestdR      = cl.getValue<bool>   ("bestdR",        false);
  bool           perfmatch   = cl.getValue<bool>   ("perfmatch",     false);


  if (!cl.check())
		return 0;
  cl.print();

  //Init random number generator
  srand(time(NULL));

  //
  // loop over all input files
  //

  for (unsigned int iinput = 0; iinput<inputs.size(); ++iinput) {
  
    // open input file
    string input = inputs[iinput]; 

    // read input file                                                                                                                                                                        
    TFile* ifile = TFile::Open((input).c_str(),"READ");
    if (!ifile->IsOpen()){
      cout << "\033[31m  > Failed to open '" << input << "'\033[0m" << endl;
      return 0;
    }else{
      cout << "Processing input file \033[33m'" << ifile->GetName() << "'\033[0m" << endl;
    }

    // read input directory
    TDirectory* idir = (TDirectory*)ifile->Get(dirname.c_str());
    if (0 == idir) {
      cout << "\033[31m  > Failed to access directory '" << dirname << "' in input file '" << input << ".root'\033[0m"\
      << endl;
      return 0;
    }



    // open input tree                                                                                                                                                                        
    TTree* itree = (TTree*)idir->Get(treename.c_str());
    if(0==itree){
      cout << "\033[31m  > No TTree named '" << treename << "' in '" << input << ":" << dirname << "'\033[0m" << endl;
      return 0;
    }

    //Get relevant information for creating hypothesis
    Int_t   i_njt=-99;               itree->SetBranchAddress("njt",            &i_njt);
    Float_t i_jtpt[40]={-99};        itree->SetBranchAddress("jtpt",           i_jtpt);
    Float_t i_jteta[40]={-99};       itree->SetBranchAddress("jteta_old",         i_jteta);
    Float_t i_jtphi[40]={-99};       itree->SetBranchAddress("jtphi",         i_jtphi);
    Float_t i_jte[40]={-99};         itree->SetBranchAddress("jte",             i_jte);
    Float_t i_lepwm={-99};           itree->SetBranchAddress("lepwm",        &i_lepwm);
    Float_t i_lepwpt={-99};          itree->SetBranchAddress("lepwpt",      &i_lepwpt);
    Float_t i_lepwphi={-99};         itree->SetBranchAddress("lepwphi",    &i_lepwphi);
    Float_t i_lepweta={-99};         itree->SetBranchAddress("lepweta",    &i_lepweta);

    Float_t i_Hpt={-99};             itree->SetBranchAddress("Hpt",            &i_Hpt);
    Float_t i_Heta={-99};            itree->SetBranchAddress("Heta",          &i_Heta);
    Float_t i_Hphi={-99};            itree->SetBranchAddress("Hphi",          &i_Hphi);
    Float_t i_Hm={-99};              itree->SetBranchAddress("Hm",              &i_Hm);

    Float_t i_Hdaupt[2]={-99};       itree->SetBranchAddress("Hdaupt",       i_Hdaupt);
    Float_t i_Hdaueta[2]={-99};      itree->SetBranchAddress("Hdaueta",     i_Hdaueta);
    Float_t i_Hdauphi[2]={-99};      itree->SetBranchAddress("Hdauphi",     i_Hdauphi);
    Float_t i_Hdaum[2]={-99};        itree->SetBranchAddress("Hdaum",         i_Hdaum);

    Float_t i_btoppt={-99};          itree->SetBranchAddress("btoppt",      &i_btoppt);
    Float_t i_btopeta={-99};         itree->SetBranchAddress("btopeta",    &i_btopeta);
    Float_t i_btopphi={-99};         itree->SetBranchAddress("btopphi",    &i_btopphi);
    Float_t i_btopm={-99};           itree->SetBranchAddress("btopm",        &i_btopm);

    Float_t i_sbpt={-99};            itree->SetBranchAddress("sbpt",          &i_sbpt);
    Float_t i_sbeta={-99};           itree->SetBranchAddress("sbeta",        &i_sbeta);
    Float_t i_sbphi={-99};           itree->SetBranchAddress("sbphi",        &i_sbphi);
    Float_t i_sbm={-99};             itree->SetBranchAddress("sbm",            &i_sbm);

    Float_t i_tpt={-99};             itree->SetBranchAddress("tpt",            &i_tpt);
    Float_t i_teta={-99};            itree->SetBranchAddress("teta",          &i_teta);
    Float_t i_tphi={-99};            itree->SetBranchAddress("tphi",          &i_tphi);
    Float_t i_tm={-99};              itree->SetBranchAddress("tm",              &i_tm);

    Float_t i_lqpt={-99};            itree->SetBranchAddress("lqpt",          &i_lqpt);
    Float_t i_lqeta={-99};           itree->SetBranchAddress("lqeta",        &i_lqeta);
    Float_t i_lqphi={-99};           itree->SetBranchAddress("lqphi",        &i_lqphi);
    Float_t i_lqm={-99};             itree->SetBranchAddress("lqm",            &i_lqm);
    
    Int_t   i_nel={-99};             itree->SetBranchAddress("nel",            &i_nel);
    Int_t   i_nlel={-99};            itree->SetBranchAddress("nlel",          &i_nlel);
    Float_t i_elpt[10]={-99};        itree->SetBranchAddress("elpt",           i_elpt);
    Float_t i_eleta[10]={-99};       itree->SetBranchAddress("eleta",         i_eleta);
    Float_t i_elphi[10]={-99};       itree->SetBranchAddress("elphi",         i_elphi);
    Float_t i_ele[10]={-99};         itree->SetBranchAddress("ele",             i_ele);


    Int_t   i_nmu={-99};             itree->SetBranchAddress("nmu",            &i_nmu);
    Int_t   i_nlmu={-99};            itree->SetBranchAddress("nlmu",          &i_nlmu); 
    Float_t i_mupt[10]={-99};        itree->SetBranchAddress("mupt",           i_mupt);
    Float_t i_mueta[10]={-99};       itree->SetBranchAddress("mueta",         i_mueta);
    Float_t i_muphi[10]={-99};       itree->SetBranchAddress("muphi",         i_muphi);
    Float_t i_mue[10]={-99};         itree->SetBranchAddress("mue",             i_mue);

    Int_t i_hbbjtidx[3][51];           itree->SetBranchAddress("hbbjtidx",       i_hbbjtidx);
    Int_t i_topjtidx[51];              itree->SetBranchAddress("topjtidx",       i_topjtidx);
    Int_t i_ljtidx[51];                itree->SetBranchAddress("ljtidx",           i_ljtidx);
    Int_t i_tophadjtidx[3];            itree->SetBranchAddress("tophadjtidx", i_tophadjtidx);
    Int_t i_toplepjtidx;               itree->SetBranchAddress("toplepjtidx",&i_toplepjtidx);

    

    int iSkippedEvents = 0;
    int iProcessedEvents = 0;

    unsigned int nEvt = (unsigned int)(itree->GetEntries());
    cout << "> \033[32m" << nEvt <<"\033[0m events will be processed"<<endl;
    cout << endl << " ";

    gErrorIgnoreLevel = kError;
    Int_t count_hyp = 0; // counter for all hypotheses
    //event loop

    int correct_higgs=0;
    int correct_th=0;
    int correct_total=0;
    int correct_l=0;
    int correct_t=0;
    int correct_higgs_04=0;
    int correct_th_04=0;
    int correct_total_04=0;
    int correct_l_04=0;
    int correct_t_04=0;
    int correct_higgs_matchable=0;
    int correct_th_matchable=0;
    int correct_total_matchable=0;
    int correct_l_matchable=0;
    int correct_t_matchable=0;
        

    int correctchi_higgs=0;
    int correctchi_th=0;
    int correctchi_total=0;
    int correctchi_l=0;
    int correctchi_t=0;
    int correctchi_higgs_04=0;
    int correctchi_th_04=0;
    int correctchi_total_04=0;
    int correctchi_l_04=0;
    int correctchi_t_04=0;
    int correctchi_higgs_matchable=0;
    int correctchi_th_matchable=0;
    int correctchi_total_matchable=0;
    int correctchi_l_matchable=0;
    int correctchi_t_matchable=0;



    bool isMatchableEvent = false;
    int total_matchable_events = 0;

    for (unsigned int iEvt = 0; iEvt < nEvt; ++iEvt) {
      //  > Output sign of life
      if (iEvt %250 == 0 || iEvt <= 10){
        cout << "\r  > Processing events from input tree:  (\033[33m" << iEvt << "/" << nEvt << "\033[0m)  -> \033[32m";
        cout << setprecision(4) << setiosflags(std::ios_base::left) << setfill('0') << setw(5)\
             << (iEvt*100.0)/nEvt << "\033[0m%    \b\b\b" << flush;
      }
      //'NULL' important variables (to give them a definite state)
      //Retrieve data from input Tree
      itree->GetEntry(iEvt);

      //Only reconstruct for n(isolated leptons)==1
      isMatchableEvent = false;
      
      //Use Lorentz vectors to group variables and to avoid tapeworm names
      TLorentzVector p4jets[100];
      TLorentzVector p4top, p4higgs, p4light;
      TLorentzVector p4lepw;
      TLorentzVector p4btop;
      TLorentzVector p4sb;
      TLorentzVector p4hdau1, p4hdau2;

      TLorentzVector p4recotop;
      TLorentzVector p4recoh;

      Int_t bestchi_i=-1;
      Int_t bestchi_j=-1;
      Int_t bestchi_k=-1;
      Int_t bestchi_l=-1;
      Float_t chi;
      Float_t bestchi = 9999;

      //Set grouped variables for this event in the output tree
      int p4jetcounter = 0;
      std::map<int, int> jtidxmap;
      jtidxmap.insert(std::pair<int,int>(-1,-1));
      for(int jets=0; jets<i_njt; ++jets){
        //Cut out jets with less then pt < 30GeV
        if (TMath::Abs(i_jtpt[jets]) > 40 || (TMath::Abs(i_jteta[jets])<2.4 && TMath::Abs(i_jtpt[jets]) > 30) ) {
          p4jets[p4jetcounter].SetPtEtaPhiE(i_jtpt[jets], i_jteta[jets], i_jtphi[jets], i_jte[jets]);
          jtidxmap[p4jetcounter] = jets;
          p4jetcounter++;
        }
      }
      
      //Don't construct with less than 4 Jets
      if(p4jetcounter < 4) {
        iSkippedEvents++;
        continue;
      }
      p4lepw.SetPtEtaPhiM(i_lepwpt,i_lepweta,i_lepwphi,i_lepwm);

      //Distinct between electron and muon case to set generalized lepton variables
      bool has_wenu = (i_nel==1 && i_nlel==1 && i_nmu==0 && i_nlmu==0);
      bool has_wmunu = (i_nmu==1 && i_nlmu==1 && i_nel==0 && i_nlel==0);
      TLorentzVector p4lepton;
      
      
      if (has_wenu){
	p4lepton.SetPtEtaPhiE(i_elpt[0],i_eleta[0],i_elphi[0],i_ele[0]);
      }else{
	p4lepton.SetPtEtaPhiE(i_mupt[0],i_mueta[0],i_muphi[0],i_mue[0]);
      }
      
      if (!has_wenu &&!has_wmunu){
        iSkippedEvents++;
        cout << "\r \033[31m  > Warning - Neither an electron nor a muon was detected!\033[0m" << endl << " ";
	continue;
      }

      // gen particles
      p4higgs.SetPtEtaPhiM(i_Hpt,i_Heta,i_Hphi,i_Hm);
      p4top.SetPtEtaPhiM(i_tpt,i_teta,i_tphi,i_tm);
      p4light.SetPtEtaPhiM(i_lqpt, i_lqeta,i_lqphi,i_lqm);
      p4btop.SetPtEtaPhiM(i_btoppt,i_btopeta,i_btopphi,i_btopm);
      p4sb.SetPtEtaPhiM(i_sbpt,i_sbeta,i_sbphi,i_sbm);
      p4hdau1.SetPtEtaPhiM(i_Hdaupt[0],i_Hdaueta[0],i_Hdauphi[0],i_Hdaum[0]);
      p4hdau2.SetPtEtaPhiM(i_Hdaupt[1],i_Hdaueta[1],i_Hdauphi[1],i_Hdaum[1]);

      
      for (Int_t i=0; i<p4jetcounter; i++){  // b from top                                                              
	if (TMath::Abs(p4jets[i].Eta()) > 2.4)
          continue;
        p4recotop = p4lepw + p4jets[i];

        for (Int_t j=0; j<p4jetcounter; j++){  // b1 from Higgs                                                                     
          if (j!=i){
            for (Int_t k=0; k<p4jetcounter; k++){  // b2 from Higgs                                                                 
              if ((k!=i )&&(k>j)) {  // switching of b1 & b2 does not change Higgs, so take only one                                
                if (TMath::Abs(p4jets[j].Eta()) > 2.4 || TMath::Abs(p4jets[k].Eta()) > 2.4)
                  continue;
                p4recoh = p4jets[j] + p4jets[k];
		
                for (Int_t l=0; l<p4jetcounter; l++){  // light jet                                          
		  float ljeta = abs(p4jets[l].Eta());
                  if(l!=k && l!=j && l!=i && ljeta > abs(p4jets[i].Eta()) && ljeta > abs(p4jets[j].Eta()) && ljeta > abs(p4jets[k].Eta()) ){
                    TLorentzVector reclight;
                    reclight=p4jets[l];
                    chi = GetChiSquared(p4recotop, p4recoh)- ljeta;
		    //cout << "(" <<i << "," <<j << "," <<k <<")" << endl;
		    if(chi < bestchi){
		      bestchi_i = i;
		      bestchi_j = j;
		      bestchi_k = k;
		      bestchi_l = l;
		      bestchi = chi;
		    }
		    
                    
                  }
                }//end light jet                                                                                                    
              }
            }//end b2 from Higgs                                                                                                    
          }
        }//end b1 from Higgs                                                                                                        
      }  // end b from top     
      
      ///      cout << "Best chi found for " << bestchi_i << "," << bestchi_j << "," << bestchi_k << " -> " << bestchi << endl;
      
      // Storing DeltaR and jet idxs with [topjt, higgsjt1, higgsjt2, lightjt]
      double best = numeric_limits<double>::infinity();
      vector<int> sig_i (4,-1);
      vector<int> bkg_i (4,-1);
      vector<vector<int> > bkg_config;
      Int_t best_i[4]; best_i[0] = -1; best_i[1] = -1; best_i[2] = -1; best_i[3] = -1;
      Int_t best_i_pm[4]; best_i_pm[0] = -1; best_i_pm[1] = -1; best_i_pm[2] = -1; best_i_pm[3] = -1;

      //Jet Indices

      // --
      // Check if event is matchable
      // --
      Float_t bestdR_hdau1=99;
      Float_t bestdR_hdau2=99;
      Float_t bestdR_sb=99;
      Float_t bestdR_btop=99;
      Float_t bestdR_light=99;
      
      // Find best hypothesis perfect match
      for(Int_t i=0; i<p4jetcounter; i++){
	if(TMath::Abs(p4jets[i].Eta())<2.4&&Matchable(p4hdau1,p4jets[i])<bestdR_hdau1){
	  bestdR_hdau1=Matchable(p4hdau1,p4jets[i]);
	  best_i_pm[0]=i;
	}
	if(TMath::Abs(p4jets[i].Eta())<2.4&&Matchable(p4hdau2,p4jets[i])<bestdR_hdau2){
	  bestdR_hdau2=Matchable(p4hdau2,p4jets[i]);
	  best_i_pm[1]=i;
	}
	if(TMath::Abs(p4jets[i].Eta())<2.4&&Matchable(p4btop,p4jets[i])<bestdR_btop){
	  bestdR_btop=Matchable(p4btop,p4jets[i]);
	  best_i_pm[2]=i;
	}
	if(Matchable(p4light,p4jets[i])<bestdR_light){
	  bestdR_light=Matchable(p4light,p4jets[i]);
	  best_i_pm[3]=i;
	}
      }
      
      int correct_hjets=0;
      int correct_tjet=0;
      int correct_ljet=0;


      int correctchi_hjets=0;
      int correctchi_tjet=0;
      int correctchi_ljet=0;


      if(bestdR_light < 0.4 && bestdR_btop < 0.4&& bestdR_hdau2 < 0.4 && bestdR_hdau1 <0.4){
	total_matchable_events++;
	isMatchableEvent = true;
      }

      
      if(best_i_pm[0]==i_hbbjtidx[0][0]||best_i_pm[0]==i_hbbjtidx[1][0]){
	correct_hjets++;
      }
      if(best_i_pm[1]==i_hbbjtidx[0][0]||best_i_pm[1]==i_hbbjtidx[1][0]){
	correct_hjets++;
      }      
      if(best_i_pm[2]==i_topjtidx[0]){
	correct_tjet++;
      }
      if(best_i_pm[3]==i_ljtidx[0]){
	correct_ljet++;
      }

      if(bestchi_k==i_hbbjtidx[0][0]||bestchi_k==i_hbbjtidx[1][0]){
	correctchi_hjets++;
      }
      if(bestchi_j==i_hbbjtidx[0][0]||bestchi_j==i_hbbjtidx[1][0]){
	correctchi_hjets++;
      }      
      if(bestchi_i==i_topjtidx[0]){
	correctchi_tjet++;
      }
      if(bestchi_l=i_ljtidx[0]){
	correctchi_ljet++;
      }
      


      if(correctchi_hjets==2) correctchi_higgs++;
      if(correctchi_tjet==1) correctchi_t++;
      if(correctchi_hjets==2&&correctchi_tjet==1) correctchi_th++;
      if(correctchi_hjets==2&&correctchi_tjet==1&&correctchi_ljet==1) correctchi_total++;
      if(correctchi_ljet==1) correctchi_l++;


      if(correctchi_hjets==2 && isMatchableEvent) correctchi_higgs_matchable++;
      if(correctchi_tjet==1 && isMatchableEvent) correctchi_t_matchable++;
      if(correctchi_hjets==2&&correctchi_tjet==1&& isMatchableEvent) correctchi_th_matchable++;
      if(correctchi_hjets==2&&correctchi_tjet==1&&correctchi_ljet==1&& isMatchableEvent) correctchi_total_matchable++;
      if(correctchi_ljet==1&& isMatchableEvent) correctchi_l_matchable++;

      if(correctchi_hjets==2 && bestdR_hdau1 < 0.4 && bestdR_hdau2 < 0.4) correctchi_higgs_04++;
      if(correctchi_tjet==1 && bestdR_btop < 0.4) correctchi_t_04++;
      if(correctchi_hjets==2&&correctchi_tjet==1  && bestdR_hdau1 < 0.4 && bestdR_hdau2 < 0.4 && bestdR_btop <0.4) correctchi_th_04++;
      if(correctchi_hjets==2&&correctchi_tjet==1&&correctchi_ljet==1  && bestdR_hdau1 < 0.4 && bestdR_hdau2 < 0.4 && bestdR_btop <0.4 && bestdR_light < 0.4) correctchi_total_04++;
      if(correctchi_ljet==1 && bestdR_light < 0.4) correctchi_l_04++;





      if(correct_hjets==2) correct_higgs++;
      if(correct_tjet==1) correct_t++;
      if(correct_hjets==2&&correct_tjet==1) correct_th++;
      if(correct_hjets==2&&correct_tjet==1&&correct_ljet==1) correct_total++;
      if(correct_ljet==1) correct_l++;


      if(correct_hjets==2 && isMatchableEvent) correct_higgs_matchable++;
      if(correct_tjet==1 && isMatchableEvent) correct_t_matchable++;
      if(correct_hjets==2&&correct_tjet==1&& isMatchableEvent) correct_th_matchable++;
      if(correct_hjets==2&&correct_tjet==1&&correct_ljet==1&& isMatchableEvent) correct_total_matchable++;
      if(correct_ljet==1&& isMatchableEvent) correct_l_matchable++;

      


      if(correct_hjets==2 && bestdR_hdau1 < 0.4 && bestdR_hdau2 < 0.4) correct_higgs_04++;
      if(correct_tjet==1 && bestdR_btop < 0.4) correct_t_04++;
      if(correct_hjets==2&&correct_tjet==1  && bestdR_hdau1 < 0.4 && bestdR_hdau2 < 0.4 && bestdR_btop <0.4) correct_th_04++;
      if(correct_hjets==2&&correct_tjet==1&&correct_ljet==1  && bestdR_hdau1 < 0.4 && bestdR_hdau2 < 0.4 && bestdR_btop <0.4 && bestdR_light < 0.4) correct_total_04++;
      if(correct_ljet==1 && bestdR_light < 0.4) correct_l_04++;


      
     
      
    }// for(iEvt < nEvt)

    cout << endl << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "----------------------TYPE I RECO (closest jets)-----------------" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                    Higgs reconstruction efficiency: " << float(correct_higgs)/nEvt << endl;
    cout << "                      Top reconstruction efficiency: " << float(correct_t)/nEvt << endl;
    cout << "              Forward Jet reconstruction efficiency: " << float(correct_l)/nEvt << endl;
    cout << "                 TopHiggs reconstruction efficiency: " << float(correct_th)/nEvt << endl;
    cout << "                    Total reconstruction efficiency: " << float(correct_total)/nEvt << endl;
    cout << "-----------------------------------------------------------------" << endl << endl;
    
    cout << endl << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "----------------------TYPE II RECO (only <0.4 jets matched)------" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                    Higgs reconstruction efficiency: " << float(correct_higgs_04)/nEvt << endl;
    cout << "                      Top reconstruction efficiency: " << float(correct_t_04)/nEvt << endl;
    cout << "              Forward Jet reconstruction efficiency: " << float(correct_l_04)/nEvt << endl;
    cout << "                 TopHiggs reconstruction efficiency: " << float(correct_th_04)/nEvt << endl;
    cout << "                    Total reconstruction efficiency: " << float(correct_total_04)/nEvt << endl;
    cout << "-----------------------------------------------------------------" << endl << endl;


    cout << endl << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "----------------------TYPE III RECO (only matchable events)------" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                    Higgs reconstruction efficiency: " << float(correct_higgs_matchable)/total_matchable_events << endl;
    cout << "                      Top reconstruction efficiency: " << float(correct_t_matchable)/total_matchable_events << endl;
    cout << "              Forward Jet reconstruction efficiency: " << float(correct_l_matchable)/total_matchable_events << endl;
    cout << "                 TopHiggs reconstruction efficiency: " << float(correct_th_matchable)/total_matchable_events << endl;
    cout << "                    Total reconstruction efficiency: " << float(correct_total_matchable)/total_matchable_events << endl;
    cout << "-----------------------------------------------------------------" << endl << endl;




    cout << endl << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "---------------CHI2---TYPE I RECO (closest jets)-----------------" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                    Higgs reconstruction efficiency: " << float(correctchi_higgs)/nEvt << endl;
    cout << "                      Top reconstruction efficiency: " << float(correctchi_t)/nEvt << endl;
    cout << "              Forward Jet reconstruction efficiency: " << float(correctchi_l)/nEvt << endl;
    cout << "                 TopHiggs reconstruction efficiency: " << float(correctchi_th)/nEvt << endl;
    cout << "                    Total reconstruction efficiency: " << float(correctchi_total)/nEvt << endl;
    cout << "-----------------------------------------------------------------" << endl << endl;
    
    cout << endl << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "---------------CHI2---TYPE II RECO (only <0.4 jets matched)------" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                    Higgs reconstruction efficiency: " << float(correctchi_higgs_04)/nEvt << endl;
    cout << "                      Top reconstruction efficiency: " << float(correctchi_t_04)/nEvt << endl;
    cout << "              Forward Jet reconstruction efficiency: " << float(correctchi_l_04)/nEvt << endl;
    cout << "                 TopHiggs reconstruction efficiency: " << float(correctchi_th_04)/nEvt << endl;
    cout << "                    Total reconstruction efficiency: " << float(correctchi_total_04)/nEvt << endl;
    cout << "-----------------------------------------------------------------" << endl << endl;


    cout << endl << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "--------------CHI2----TYPE III RECO (only matchable events)------" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                    Higgs reconstruction efficiency: " << float(correctchi_higgs_matchable)/total_matchable_events << endl;
    cout << "                      Top reconstruction efficiency: " << float(correctchi_t_matchable)/total_matchable_events << endl;
    cout << "              Forward Jet reconstruction efficiency: " << float(correctchi_l_matchable)/total_matchable_events << endl;
    cout << "                 TopHiggs reconstruction efficiency: " << float(correctchi_th_matchable)/total_matchable_events << endl;
    cout << "                    Total reconstruction efficiency: " << float(correctchi_total_matchable)/total_matchable_events << endl;
    cout << "-----------------------------------------------------------------" << endl << endl;





    cout << "t1,t2,t3,chi1,chi2,chi3" << endl;
    cout << "h," << float(correct_higgs)/nEvt << "," << float(correct_higgs_04)/nEvt << "," << float(correct_higgs_matchable)/total_matchable_events ;
    cout << "," << float(correctchi_higgs)/nEvt << "," << float(correctchi_higgs_04)/nEvt << "," << float(correctchi_higgs_matchable)/total_matchable_events << endl;


    cout << "t," << float(correct_t)/nEvt << "," << float(correct_t_04)/nEvt << "," << float(correct_t_matchable)/total_matchable_events;
    cout << "," << float(correctchi_t)/nEvt << "," << float(correctchi_t_04)/nEvt << "," << float(correctchi_t_matchable)/total_matchable_events <<endl;

    cout << "l," << float(correct_l)/nEvt << "," << float(correct_l_04)/nEvt << "," << float(correct_l_matchable)/total_matchable_events ;
    cout << "," << float(correctchi_l)/nEvt << "," << float(correctchi_l_04)/nEvt << "," << float(correctchi_l_matchable)/total_matchable_events <<endl;

    cout << "th," << float(correct_th)/nEvt << "," << float(correct_th_04)/nEvt << "," << float(correct_th_matchable)/total_matchable_events;
    cout << "," << float(correctchi_th)/nEvt << "," << float(correctchi_th_04)/nEvt << "," << float(correctchi_th_matchable)/total_matchable_events <<endl;

    cout << "tot," << float(correct_total)/nEvt << "," << float(correct_total_04)/nEvt << "," << float(correct_total_matchable)/total_matchable_events;
    cout << "," << float(correctchi_total)/nEvt << "," << float(correctchi_total_04)/nEvt << "," << float(correctchi_total_matchable)/total_matchable_events <<endl;
    
    ifile->Close();
    delete ifile;

  }// for (ifile in inputs)

  return 0;
}//main(...)



////////////////////////////////////////////////////////////////////////////////
// helpers
////////////////////////////////////////////////////////////////////////////////

Float_t Matchable(const TLorentzVector & gen_obj, const TLorentzVector & rec_obj){
  Float_t DR = DeltaR(gen_obj,rec_obj);
  return DR;//+ptreltop+ptrelhiggs+ptrellight;
}

Float_t GetChiSquared(const TLorentzVector & top, const TLorentzVector & higgs){
  Float_t topm = top.M();
  Float_t higgsm = higgs.M();
  

  
  Float_t chi1 = pow(abs(topm - 173.0),2)/pow(30,2);
  Float_t chi2 = pow(abs(higgsm - 125.0),2)/pow(15,2);

  //  cout << "Calculating Chi2 with " << topm << " " << higgsm << " ---> " << chi1+chi2 << endl;
  return chi1+chi2;
}

static double DeltaR(const TLorentzVector & v1, const TLorentzVector & v2) {
  Double_t DeltaR = 0.0;
  Double_t DeltaPhi = TMath::Abs(v1.Phi()-v2.Phi());
  if (DeltaPhi>TMath::Pi())
    DeltaPhi = 2*TMath::Pi() - DeltaPhi;
  Double_t DeltaEta = v1.Eta() - v2.Eta();
  DeltaR = sqrt(TMath::Power(DeltaPhi, 2) + TMath::Power(DeltaEta, 2));
  return DeltaR;
}

