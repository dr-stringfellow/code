////////////////////////////////////////////////////////////////////////////////
//
// th_pileupreweighter_x
// -----------------
//
// This program applies the pileup reweighting (gone wrong in 76cmssw processing)
//
//            03/27/2016 Simon Fink
//            
//                                                    
////////////////////////////////////////////////////////////////////////////////


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
  string         mcfile      = cl.getValue<string> ("mcfile");
  string         datafile    = cl.getValue<string> ("datafile");
  
  if (!cl.check()) return 0;
  cl.print();

  TH1F* h_data_npv;
  TH1F* h_mc_npv;
  
  string fullpath = "/portal/ekpcms6/home/sfink/thanalysis/STANDALONE/THAnalysis/";
  
  TFile mc_file((fullpath + mcfile).c_str(),"READ");
  
  cout << "Looking for file in " << (fullpath + mcfile).c_str() << endl;
  if(mc_file.IsOpen()==0) {
    cout << "No mc file found." << endl;
    throw;
  }
  mc_file.GetObject("",h_mc_npv);
  if( h_mc_npv == 0) {
    throw;
  }
  cout << "Opened MC file." << endl;

  TFile data_file((fullpath + "/data_npv.root").c_str(),"READ");
  if(data_file.IsOpen()==0){
    cout << "No data file found." << endl;
    throw;
  }
  data_file.GetObject("",h_data_npv);
  if( h_mc_npv == 0 ) {
    throw;
  }
  cout << "Opened data file." << endl;
  
  //Check for same binning
  
  Int_t nbins_mc = h_mc_npv->GetNbinsX();
  Int_t nbins_data = h_data_npv->GetNbinsX();

  assert(nbins_mc==nbins_data);
  cout << "Same binning found in both histograms." << endl;
  
  //Scale histograms 

  h_mc_npv->Scale(1.0/h_mc_npv->Integral());
  h_data_npv->Scale(1.0/h_data_npv->Integral());

  std::vector<double> weights;

  for( int bin = 0; bin <= nbins_mc; bin++){
    Float_t npv_mc = h_mc_npv->GetBinContent(bin);
    const Float_t npv_data = h_data_npv->GetBinContent(bin);
    if(npv_mc==0) npv_mc=1;
    const Float_t ratio = npv_data/npv_mc;
    weights.push_back(ratio);
    cout << "Bin " << bin << " :   MC -> "  << npv_mc << "   Data -> " << npv_data << "     ---> " << ratio << endl;
  }
  
  delete h_mc_npv;
  delete h_data_npv;

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
    output = output + ".pu.root";
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
      if (0==itree->FindBranch("npv")){ cout<<"no npv branch stored!"<<endl; return 0; }

      // don't copy branches if they already exist
      if (0!=itree->FindBranch("Weight_PU"))      itree->SetBranchStatus("Weight_PU",     0);
      if (0!=itree->FindBranch("Weight_PU_up"))      itree->SetBranchStatus("Weight_PU_up",     0);
      if (0!=itree->FindBranch("Weight_PU_down"))      itree->SetBranchStatus("Weight_PU_down",     0);
      
      // clone input tree into output directory
      odir->cd();
      TTree* otree = itree->CloneTree();
      cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;
      
      
      // create H->bb branches in output tree
       
      Float_t Weight_PU;
      Float_t Weight_PU_up;
      Float_t Weight_PU_down;
      
      TBranch* b_Weight_PU        =otree->Branch("Weight_PU",         &Weight_PU,           "Weight_PU/F");
      TBranch* b_Weight_PU_up     =otree->Branch("Weight_PU_up"      ,&Weight_PU_up,     "Weight_PU_up/F");
      TBranch* b_Weight_PU_down   =otree->Branch("Weight_PU_down",&Weight_PU_down,     "Weight_PU_down/F");
      
      // setup input branches needed to fill H->bb branches
      
      Int_t   npv;             itree->SetBranchAddress("npv",          &npv);
      
      // loop over all events and fill branches
      unsigned nevt = static_cast<unsigned>(itree->GetEntries());
      cout<<nevt<<" events will be processed"<<endl;
      
      long double SumWeights = 0;
      //      nevt = 10;

      cout << "Calculating Weights ... " << endl;
      for (unsigned ievt=0;ievt<nevt;ievt++) { //calculate weights (not yet normalized)

        itree->GetEntry(ievt);

        Weight_PU = 1.;
	if(npv>50) npv = 50;
	
	
	Weight_PU = weights.at(npv);
	SumWeights += Weight_PU;
      
      } // ievt

      
      Float_t norm = 1;
      norm = SumWeights/nevt;
      cout << "normalization : " << norm << endl;
      
      std::vector<double> normweights;
      for(unsigned i = 0; i< weights.size(); ++i){
	normweights.push_back(weights.at(i)/norm);
      }

      cout << "Normalizing ... "<< endl;
      for (unsigned ievt=0;ievt<nevt;ievt++) {

        itree->GetEntry(ievt);

        Weight_PU = 1.;
	if(npv>50) npv = 50;

	
	
	Weight_PU = normweights.at(npv);

	Weight_PU_down =  Weight_PU;
	Weight_PU_up = Weight_PU;
	
	
	b_Weight_PU->Fill();
	b_Weight_PU_up->Fill();
	b_Weight_PU_down->Fill();
	
      
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

