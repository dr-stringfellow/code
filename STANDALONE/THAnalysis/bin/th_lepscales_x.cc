////////////////////////////////////////////////////////////////////////////////
//
// th_lepscales_x
// -----------------
//
// This program applies scale factors (weights) to correct the lepton selection
//  efficiencies in the simulation.
//
//            03/16/2016 Simon Fink
//             (he valiantly stole this code from another framework)
//                                                    
////////////////////////////////////////////////////////////////////////////////


// muon scale factor systematics, may well change over time:
// (source https://twiki.cern.ch/twiki/bin/view/CMS/MuonTagAndProbe#Systematic_uncertainties)
#define MU_ID_SYS 0.01
#define MU_ISO_SYS 0.005
#define MU_TRIG_SYS 0.005

// The muon scale factor information was found here:
// https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs
//
// The electron scale factor information is from here:
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentification
// or, more specifically, from here:
// https://twiki.cern.ch/twiki/bin/view/CMS/KoPFAElectronTagAndProbe

// NOTE
// For the 13TeV run it seems one will need to rely on the information in the following two links:
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopEGM#Working_points_for_Spring15_MC_s
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffsRun2#Muon_reconstruction_identificati



#include "../interface/CommandLine.h"
#include <assert.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TEventList.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include "TGraphAsymmErrors.h"
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

TH2F* h_muon_id;
TH2F* h_muon_iso;
TH2F* h_muon_trigger;
TH2F* h_el_id;
TH2F* h_el_iso;
TH2F* h_el_reco;

void muonIDScaleFactor(float pt, float eta, float* eff, float* err);
void muonIsoScaleFactor(float pt, float eta, float* eff, float* err);
void muonTriggerScaleFactor(float pt, float eta, float* eff, float* err);
void electronIDScaleFactor(float pt, float eta, float* eff, float* err);
void electronRecoScaleFactor(float pt, float eta, float* eff, float* err);
void electronIDAndIsoScaleFactor(float eta, float pt, float* eff, float* err_up, float* err_down);
void electronTriggerScaleFactor(float eta, float pt, float* eff, float* err_up, float* err_down);

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
  string         mode_as_str = cl.getValue<string> ("mode","dijetpt");
  
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
    output = output + ".lsf.root";
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
      
      // check wether relevant branches are present
      if (0==itree->FindBranch("nlepw")){ cout<<"no W->lnu branches stored!"<<endl; return 0; }

      // don't copy branches if they already exist
      if (0!=itree->FindBranch("Weight_LSF"))      itree->SetBranchStatus("Weight_LSF",     0);
      
      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;
      
      
      // create H->bb branches in output tree
       
      Float_t Weight_LSF;
      Float_t Weight_LSF_up;
      Float_t Weight_LSF_down;
      
      TBranch* b_Weight_LSF        =otree->Branch("Weight_LSF",         &Weight_LSF,           "Weight_LSF/F");
      TBranch* b_Weight_LSF_up     =otree->Branch("Weight_LSF_up"      ,&Weight_LSF_up,     "Weight_LSF_up/F");
      TBranch* b_Weight_LSF_down   =otree->Branch("Weight_LSF_down",&Weight_LSF_down,     "Weight_LSF_down/F");
      
      // setup input branches needed to fill H->bb branches
      
      Int_t   njt;             itree->SetBranchAddress("njt",          &njt);
      Float_t jtpt[140];       itree->SetBranchAddress("jtpt",         jtpt);
      Float_t jteta[140];      itree->SetBranchAddress("jteta",       jteta);
      Int_t   nmu;             itree->SetBranchAddress("nmu",          &nmu);
      Float_t mueta[110];      itree->SetBranchAddress("mueta",       mueta);
      Float_t mupt[110];       itree->SetBranchAddress("mupt",         mupt);
      Float_t mucharge[110];   itree->SetBranchAddress("mucharge", mucharge);
      Int_t   nel;             itree->SetBranchAddress("nel",          &nel);
      Float_t eleta[110];      itree->SetBranchAddress("eleta",       eleta);
      Float_t elpt[110];       itree->SetBranchAddress("elpt",         elpt);
      Float_t elcharge[110];   itree->SetBranchAddress("elcharge", elcharge);

      // declare files and fill histograms

      string rootfile_dir = "/portal/ekpcms6/home/sfink/thanalysis/STANDALONE/THAnalysis/";
      
      TFile mu_id_file((rootfile_dir+"/MuonID_Z_RunCD_Reco76X_Feb15.root").c_str());
      h_muon_id = (TH2F *) mu_id_file.Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");

      TFile mu_iso_file((rootfile_dir + "/MuonIso_Z_RunCD_Reco76X_Feb15.root").c_str());
      h_muon_iso = (TH2F *) mu_iso_file.Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio");
  
      TFile mu_trigger_file((rootfile_dir + "/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root").c_str());
      h_muon_trigger = (TH2F*) mu_trigger_file.Get("runD_IsoMu20_OR_IsoTkMu20_HLTv4p3_PtEtaBins/pt_abseta_ratio");

      TFile el_id_file((rootfile_dir + "/ScaleFactor_GsfElectronToRECO_passingTrigWP80.txt.egamma_SF2D.root").c_str());
      h_el_id = (TH2F*) el_id_file.Get("EGamma_SF2D");

      TFile el_reco_file((rootfile_dir + "/eleRECO.txt.egamma_SF2D.root").c_str());
      h_el_reco = (TH2F*) el_reco_file.Get("EGamma_SF2D");

      
      // loop over all events and fill branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {

        itree->GetEntry(ievt);

        Weight_LSF = 1.;

        float eta = -99;
        float abseta = fabs(eta);
        float pt = -99;

        // Depends on the lepton flavour.


        // these three are to be calculated now
        float overall_eff = 0;
        float overall_abserr_up = 0;
        float overall_abserr_down = 0;

	
	
        if(nmu==1)
        {

          eta = mueta[0];
          abseta = fabs(eta);
          pt = mupt[0];

	  float id_eff, id_err;
	  muonIDScaleFactor(pt, abseta, &id_eff, &id_err);

          float iso_eff, iso_err;
          muonIsoScaleFactor(pt, abseta, &iso_eff, &iso_err);

          float trigger_eff, trigger_err;
          muonTriggerScaleFactor(pt, abseta, &trigger_eff, &trigger_err);
	  //	  cout << "Efficiencies: Id, Iso, Trig: " << id_eff << " " << iso_eff << " " << trigger_eff << endl;
          // calculate mean value of overall efficiency
          overall_eff = id_eff * iso_eff * trigger_eff;
	  assert(overall_eff != 0);

          // calculate relative errors
	  const float id_relerr = sqrt(pow(id_err / id_eff, 2) + pow(MU_ID_SYS, 2));
          const float iso_relerr = sqrt(pow(iso_err / iso_eff, 2) + pow(MU_ISO_SYS, 2));
          const float trigger_relerr = sqrt(pow(trigger_err / trigger_eff, 2) + pow(MU_TRIG_SYS, 2));

          // relative uncertainty on overall efficiency
          const float overall_relerr = sqrt( pow(id_relerr, 2.) +
					     pow(iso_relerr, 2.) + pow(trigger_relerr, 2.) );
	  
          // absolute uncertainty on overall efficiency
          overall_abserr_up = overall_abserr_down = overall_relerr * overall_eff;
        }
        else if(nel == 1) //electron
        {
          eta = eleta[0];
          abseta = fabs(eta);
          pt = elpt[0];
	  
	  float id_eff, id_err;
	  electronIDScaleFactor(pt, eta, &id_eff, &id_err);

          float reco_eff, reco_err;
          electronRecoScaleFactor(pt, eta, &reco_eff, &reco_err);

          float trigger_eff =1;
	  float trigger_err =0;
	  //          muonTriggerScaleFactor(pt, abseta, &trigger_eff, &trigger_err);
	  // cout << "Efficiencies: Id, Reco, Trig: " << id_eff << " " << reco_eff << " " << trigger_eff << endl;
          // calculate mean value of overall efficiency
          overall_eff = id_eff * reco_eff * trigger_eff;
	  assert(overall_eff != 0);

          // calculate relative errors
	  const float id_relerr = sqrt(pow(id_err / id_eff, 2) + pow(MU_ID_SYS, 2));
          const float reco_relerr = sqrt(pow(reco_err / reco_eff, 2)); // + pow(MU_ISO_SYS, 2));
          const float trigger_relerr = sqrt(pow(trigger_err / trigger_eff, 2)); // + pow(MU_TRIG_SYS, 2));

          // relative uncertainty on overall efficiency
          const float overall_relerr = sqrt( pow(id_relerr, 2.) +
					     pow(reco_relerr, 2.) + pow(trigger_relerr, 2.) );
	  
          // absolute uncertainty on overall efficiency
          overall_abserr_up = overall_abserr_down = overall_relerr * overall_eff;
          
	}
        

	

	
	Weight_LSF = overall_eff;
        
	const float low_eff  = overall_eff - overall_abserr_down; // complete down-variation
	const float high_eff = overall_eff + overall_abserr_up;   // complete up-variation

	Weight_LSF_down =  low_eff;
	Weight_LSF_up = high_eff;
	
	
	b_Weight_LSF->Fill();
	b_Weight_LSF_up->Fill();
	b_Weight_LSF_down->Fill();
	
      
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

//
//
//

// HINT This could also be done pt-dependent using other histos
void muonIDScaleFactor(float pt, float eta, float* eff, float* err)
{
  const int nbinsx = h_muon_id->GetXaxis()->GetNbins();
  const int nbinsy = h_muon_id->GetYaxis()->GetNbins();
  int xbin_index = h_muon_id->GetXaxis()->FindBin(pt);
  int ybin_index = h_muon_id->GetYaxis()->FindBin(eta);

  if(xbin_index>nbinsx) xbin_index=nbinsx;  
  if(ybin_index>nbinsy) ybin_index=nbinsy;
  if(xbin_index==0) xbin_index=1;
  if(ybin_index==0) ybin_index=1;
  const int bin_index = h_muon_id->GetBin(xbin_index,ybin_index,0); //see doc of TH1::GetBin
  *eff = h_muon_id->GetBinContent(bin_index);
  *err = h_muon_id->GetBinError(bin_index);
}

// HINT This could also be done pt-dependent using other histos
// WARNING Using scale factor for dBeta-corrected reliso <0.12
void muonIsoScaleFactor(float pt, float eta, float* eff, float* err)
{
  const int nbinsx = h_muon_iso->GetXaxis()->GetNbins();
  const int nbinsy = h_muon_iso->GetYaxis()->GetNbins();
  int xbin_index = h_muon_iso->GetXaxis()->FindBin(pt);
  int ybin_index = h_muon_iso->GetYaxis()->FindBin(eta);
  if(xbin_index>nbinsx) xbin_index=nbinsx;  
  if(ybin_index>nbinsy) ybin_index=nbinsy;
  if(xbin_index==0) xbin_index=1;
  if(ybin_index==0) ybin_index=1;

  const int bin_index = h_muon_iso->GetBin(xbin_index,ybin_index,0); //see doc of TH1::GetBin
  *eff = h_muon_iso->GetBinContent(bin_index);
  *err = h_muon_iso->GetBinError(bin_index);
}

// HINT This could also be done pt-dependent using other histos
void muonTriggerScaleFactor(float pt, float eta, float* eff, float* err)
{
  const int nbinsx = h_muon_trigger->GetXaxis()->GetNbins();
  const int nbinsy = h_muon_trigger->GetYaxis()->GetNbins();
  //  cout << "Found (" << nbinsx << "," << nbinsy << ") bins." << endl; 
  int xbin_index = h_muon_trigger->GetXaxis()->FindBin(pt);
  int ybin_index = h_muon_trigger->GetYaxis()->FindBin(eta);
  if(xbin_index>nbinsx) xbin_index=nbinsx;  
  if(ybin_index>nbinsy) ybin_index=nbinsy;
  if(xbin_index==0) xbin_index=1;
  if(ybin_index==0) ybin_index=1;


  //  cout << "Pt,Eta: " << pt << "  " << eta << endl;
  //  cout << "TriggerBins (x,y): (" << xbin_index << "," << ybin_index << ")" << endl; 
  const int bin_index = h_muon_trigger->GetBin(xbin_index,ybin_index,0); //see doc of TH1::GetBin
  *eff = h_muon_trigger->GetBinContent(bin_index);
  *err = h_muon_trigger->GetBinError(bin_index);
}

void electronIDScaleFactor(float pt, float eta, float* eff, float* err)
{
  const int nbinsx = h_el_id->GetXaxis()->GetNbins();
  const int nbinsy = h_el_id->GetYaxis()->GetNbins();
  int xbin_index = h_el_id->GetXaxis()->FindBin(eta);
  int ybin_index = h_el_id->GetYaxis()->FindBin(pt);

  if(xbin_index>nbinsx) xbin_index=nbinsx;  
  if(ybin_index>nbinsy) ybin_index=nbinsy;
  if(xbin_index==0) xbin_index=1;
  if(ybin_index==0) ybin_index=1;
  const int bin_index = h_el_id->GetBin(xbin_index,ybin_index,0); //see doc of TH1::GetBin
  //  cout << "Id:Found (" << nbinsx << "," << nbinsy << ") bins." << endl; 
  //  cout << "Eta,Pt: " << eta << "  " << pt << endl;
  //  cout << "TriggerBins (x,y): (" << xbin_index << "," << ybin_index << ")" << endl; 

  *eff = h_el_id->GetBinContent(bin_index);
  *err = h_el_id->GetBinError(bin_index);
  
  //  cout << "Eff,Err -> " << *eff << "," << *err<< endl;
}

void electronRecoScaleFactor(float pt, float eta, float* eff, float* err)
{
  const int nbinsx = h_el_reco->GetXaxis()->GetNbins();
  const int nbinsy = h_el_reco->GetYaxis()->GetNbins();
  int xbin_index = h_el_reco->GetXaxis()->FindBin(eta);
  int ybin_index = h_el_reco->GetYaxis()->FindBin(pt);
  
  if(xbin_index>nbinsx) xbin_index=nbinsx;  
  if(ybin_index>nbinsy) ybin_index=nbinsy;
  if(xbin_index==0) xbin_index=1;
  if(ybin_index==0) ybin_index=1;
  const int bin_index = h_el_reco->GetBin(xbin_index,ybin_index,0); //see doc of TH1::GetBin
  //  cout << "Reco :Found (" << nbinsx << "," << nbinsy << ") bins." << endl; 
  //  cout << "Eta,Pt: " << eta << "  " << pt << endl;
  //  cout << "TriggerBins (x,y): (" << xbin_index << "," << ybin_index << ")" << endl; 

  *eff = h_el_reco->GetBinContent(bin_index);
  *err = h_el_reco->GetBinError(bin_index);
  //  cout << "Eff,Err -> " << *eff << "," << *err<< endl;
}



void electronIDAndIsoScaleFactor(float eta, float pt, float* eff, float* err_up, float* err_down)
{
  const float abseta = fabs(eta);

  assert(pt >= 20);
  assert(abseta <= 2.5);


  if(pt < 30)
  {
    if(abseta < 0.800) {*eff = 0.953; *err_up = 0.185; *err_down = 0.007; return;} // sic. if it's an error, it's theirs
    if(abseta < 1.478) {*eff = 0.904; *err_up = 0.010; *err_down = 0.010; return;}
    if(abseta < 2.500) {*eff = 0.863; *err_up = 0.008; *err_down = 0.008; return;}
  }
  if(pt < 40)
  {
    if(abseta < 0.800) {*eff = 0.945; *err_up = 0.003; *err_down = 0.003; return;}
    if(abseta < 1.478) {*eff = 0.916; *err_up = 0.002; *err_down = 0.001; return;}
    if(abseta < 2.500) {*eff = 0.896; *err_up = 0.004; *err_down = 0.004; return;}
  }
  else if(pt < 50)
  {
    if(abseta < 0.800) {*eff = 0.948; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 1.478) {*eff = 0.959; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 2.500) {*eff = 0.944; *err_up = 0.002; *err_down = 0.005; return;}
  }
  else
  {
    if(abseta < 0.800) {*eff = 0.960; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 1.478) {*eff = 0.957; *err_up = 0.002; *err_down = 0.002; return;}
    if(abseta < 2.500) {*eff = 0.953; *err_up = 0.008; *err_down = 0.007; return;}
  }

}


void electronTriggerScaleFactor(float eta, float pt, float* eff, float* err_up, float* err_down)
{
  const float abseta = fabs(eta);

  assert(pt >= 30);
  assert(abseta <= 2.5);


  if(pt < 40)
  {
    if(abseta < 0.800) {*eff = 0.987; *err_up = 0.012; *err_down = 0.017; return;}
    if(abseta < 1.478) {*eff = 0.964; *err_up = 0.002; *err_down = 0.001; return;}
    if(abseta < 2.500) {*eff = 1.004; *err_up = 0.006; *err_down = 0.006; return;}
  }
  else if(pt < 50)
  {
    if(abseta < 0.800) {*eff = 0.997; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 1.478) {*eff = 0.980; *err_up = 0.001; *err_down = 0.001; return;}
    if(abseta < 2.500) {*eff = 1.033; *err_up = 0.007; *err_down = 0.007; return;}
  }
  else
  {
    if(abseta < 0.800) {*eff = 0.998; *err_up = 0.002; *err_down = 0.002; return;}
    if(abseta < 1.478) {*eff = 0.988; *err_up = 0.002; *err_down = 0.002; return;}
    if(abseta < 2.500) {*eff = 0.976; *err_up = 0.015; *err_down = 0.012; return;}
  }

}



