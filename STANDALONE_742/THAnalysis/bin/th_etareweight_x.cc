////////////////////////////////////////////////////////////////////////////////////
//
// th_etareweight_x
// --------------
//
//            05/03/2014 Simon Fink <simon.fink@cern.ch>
// 
////////////////////////////////////////////////////////////////////////////////////


#include "../interface/Histogram.h"
#include "../interface/DataSample.h"
#include "../interface/MCSample.h"
#include "../interface/CommandLine.h"

#include <TMath.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TKey.h>
#include <TH1F.h>
#include "TPluginManager.h"


#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <stdexcept>

using namespace std;

//////////////////////////////////////////////////////////////////////////////// 
// helper function   
//////////////////////////////////////////////////////////////////////////////// 

string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression");


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo","*", "TStreamerInfo","RIO", "TStreamerInfo()");
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  vector<string> data      = cl.getVector<string>  ("data",                "");
  vector<string> mcs       = cl.getVector<string>  ("mcs",                 "");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");
  vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
  vector<string> datacuts  = cl.getVector<string>  ("datacuts",            "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");
  string         datalabel = cl.getValue <string>  ("datalabel",       "data");
  vector<string> mclabels  = cl.getVector<string>  ("mclabels",            "");
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",            "");
  string         opath     = cl.getValue <string>  ("opath",               "");
  string         output    = cl.getValue <string>  ("output",              "");
  string         filename   = cl.getValue <string>  ("filename",        "");
  
  if(!cl.check()) return 0;
  cl.print();
  
  if (data.empty()&&mcs.empty()){cout<<"provide -data and/or -mcs!"<<endl;return 0;}
  
    
  //
  // OPEN DATA FILE / RETRIEVE TREE
  //
  string filesuffix = "";
  DataSample datasample=DataSample::load(data,dirname,treename,datapath,filesuffix);
  if (!datasample.empty()) DataSample::printInfo(datasample);
  
  
  //
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples;
  mcsamples=MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0) MCSample::printSampleInfos(mcsamples);
    
  
  //
  // INITITALIZE HISTOGRAM DATA
  //
    
  const Int_t nbins = 50;
  const Float_t xmax = 4.7;
  const Float_t xmin = -4.7;

  Float_t binsize = (xmax-xmin)/nbins;

  string varname = "_EtaLight";
  string varexpr = "jteta";
  Histogram histogram;
  histogram.setName(varname);
  histogram.setExpr(varexpr);
  histogram.setXTitle("test");
  histogram.setYTitle("ytest");
  histogram.setNbinsx(nbins);
  histogram.setXmin(xmin);
  histogram.setXmax(xmax);
  histogram.setLogx(0);
  histogram.setLogy(0);

  //                                                                                                                                  
  // DATA                                                                                                                             
  //                                                                                                                                  
  TH1F*  hdata(0);

  //  string cuts = "";     // cuts can be used for different bins i.e. rho bins
  string data_selection;
  for(unsigned icut=0;icut<datacuts.size();icut++){
    string cut=get_cut(datacuts,icut,"expression");
    if (!data_selection.empty())  data_selection     += "&&";
    data_selection      += cut;
  }



  string selection;

  for (unsigned icut=0;icut<cuts.size();icut++) {
    string cut=get_cut(cuts,icut,"expression");
    if (!selection.empty())  selection     += "&&";
    selection       += cut;
  }

  if (data_selection.empty()) data_selection = selection;

  hdata = histogram.create("hData");
  datasample.project(hdata->GetName(),histogram.expr(),data_selection);





  //                                                                                                                        
  // MC                                                                                                                         
  //                                                        
  

  string         mcsel;
  vector<TH1F*>  mchistos;
  
  if (mcs.size()>0) {
    for (unsigned imc=0;imc<mcsamples.size();imc++) {
      MCSample     mcsample = mcsamples[imc];
      stringstream sshmcname_i; sshmcname_i<<"h"<<mcsample.name();
      mcsample.setWeights(mcweights);

      TH1F* hmc_i = histogram.create(sshmcname_i.str());
      mcsample.project(hmc_i->GetName(),histogram.expr(),selection);
      mchistos.push_back(hmc_i);
      
    }
  }
  
  cout << "mchistots.size() : " << mchistos.size() << endl;

  for (unsigned i=0;i<mchistos.size();i++) mchistos[i]->Scale(19773);  

  float mc_tot[51]={0}; 
  float data_tot[51]={0};
  float corrfac[51]={0};
  float mc_tot_before[10] = {0};
  float mc_tot_after[10] = {0};

  //Get Data entries

  for (int ibin=0;ibin<=hdata->GetNbinsX();ibin++) {
    float val_data  = hdata->GetBinContent(ibin);
    data_tot[ibin] = val_data;
    cout << hdata->GetName() << "  -> Content of bin #" << ibin << " : " << val_data << endl;
  }
  
  // Get Mc entries

  for (unsigned i=0;i<mchistos.size();i++) {
    for (int ibin=0;ibin<=mchistos[i]->GetNbinsX();ibin++) {
      float val_mc  = mchistos[i]->GetBinContent(ibin);
      mc_tot[ibin]=mc_tot[ibin]+val_mc;
      mc_tot_before[i] = mc_tot_before[i] + val_mc;
      cout << "Name " << i << " : " << mchistos[i]->GetName() << "  -> Content of bin #" << ibin << " : " << val_mc <<  "     -  MC total: " << mc_tot[ibin] << "   Total MC in this sample: " << mc_tot_before[i] <<   endl; 
    }
  }
  

  // Calculate correction factor

  for (int ibin=0;ibin<nbins;ibin++){
    if (mc_tot[ibin]!=0) {
      corrfac[ibin]=data_tot[ibin]/mc_tot[ibin];
    }
    else corrfac[ibin] = 1;
    
    cout << "Content of all mcs in bin #" << ibin << " : " << mc_tot[ibin] << endl;
    cout << "Content of all data in bin #" << ibin << " : " << data_tot[ibin] << endl; 
    cout << "===> Correction factor: " << corrfac[ibin] << endl;
    cout << "Sanity check: Corrected MC: " << mc_tot[ibin]*corrfac[ibin] << "     Difference to Data (should be 0):" << mc_tot[ibin]*corrfac[ibin]-data_tot[ibin] << endl; 
    //    cout << "MC Sum before: " << mc_tot_before << "   MC Sum after: " << mc_tot_after << "    Scaling factor: " << mc_tot_after/mc_tot_before <<  endl;
  }
  
  // Calculate sample specific scaling factor

  Float_t mc_scale_sample[10];

  for (unsigned i=0;i<mchistos.size();i++) {
    for (int ibin=0;ibin<=mchistos[i]->GetNbinsX();ibin++) {
      float val_mc  = mchistos[i]->GetBinContent(ibin);
      mc_tot_after[i] += corrfac[ibin]*val_mc;
           
    }
    cout << mchistos[i]->GetName() << "  -> MC-Content before reweighting : " << mc_tot_before[i] << endl;
    cout << mchistos[i]->GetName() << "  -> MC-Content after  reweighting : " << mc_tot_after[i] << endl;
    mc_scale_sample[i]= mc_tot_after[i]/mc_tot_before[i];
    cout << "Sample specific scaling factor: " <<  mc_scale_sample[i] << endl << "--------------------------------" << endl; 
  }

  ofstream ofile;
  if(filename == "")
    ofile.open ((varexpr+".txt").c_str());
  else
    ofile.open ((filename+".txt").c_str());
  ofile << nbins << " " << xmin << " " << xmax << endl;
  for (unsigned i=0;i<mchistos.size();i++) {
    for (int ibin=0;ibin<=mchistos[i]->GetNbinsX();ibin++) {
      ofile << corrfac[ibin]/mc_scale_sample[i] << " ";
    }
    ofile << endl;
  }

  ofile.close();



  /*
  
  // loop over all MC input files
  
  for (unsigned iinput=0;iinput<mcsamples.size();iinput++) {
    for (unsigned j=0;j<mcsamples[iinput].ntrees();j++) {
    // open input file                                                                                                                
      string input = mcsamples[iinput].filename(j);//mcs[iinput]; //input = datapath + "/" + input;                                                                  
      TFile* ifile = new TFile(input.c_str(),"READ");
      if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }
      
      // create temporary output file                                                                                                   
      size_t pos = input.find(".root");
      string output = input.substr(0,pos);
      output = output + ".rew.root";
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
	if (0==itree->FindBranch("popvar8")){ cout<<"no Popvar8 branch stored!"<<endl; return 0; }
	//if (0==itree->FindBranch("nlepz")){ cout<<"no Z->ll branches stored!"<<endl; return 0; }                                      
	
	// check wether sample is considered MC (weights to be stored)                                                                  
	if (0!=itree->FindBranch("weight_lqeta"))      itree->SetBranchStatus("weight_lqeta",     0);
	
	// clone input tree into output directory                                                                                       
	odir->cd();
	TTree* otree = itree->CloneTree();
	cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;
	
	Float_t weight_lqeta=-99;
	TBranch* b_weight_lqeta                =otree->Branch("weight_lqeta",       &weight_lqeta,    "weight_lqeta/F");
	
	
	
	Float_t popvar8=-99;      itree->SetBranchAddress("popvar8",   &popvar8);
	Float_t jteta[140]={-99};      itree->SetBranchAddress("jteta",   jteta);
	Int_t njt=-99;            itree->SetBranchAddress("njt", &njt);

	unsigned nevt = static_cast<unsigned>(itree->GetEntries());
	cout<<nevt<<" events will be processed"<<endl;
	for (unsigned ievt=0;ievt<nevt;ievt++) {
	  
	  itree->GetEntry(ievt);
	  
	  float fiveperc=nevt/20;
	  if(fiveperc==0)fiveperc=1;
	  if(TMath::Abs(ievt/fiveperc-TMath::Nint(ievt/fiveperc))<(0.5/fiveperc)){
	    ostringstream stream;
	    stream << "\r[" ;
	    for(Int_t i=0;i<TMath::Nint(ievt/fiveperc);i++){
	      if(i>20) break;
	      if(TMath::Nint(ievt/fiveperc)<6)  stream << "\033[1;31m%\033[0m";
	      else if(TMath::Nint(ievt/fiveperc)<16) stream << "\033[1;33m%\033[0m";
	      else  stream << "\033[1;32m%\033[0m";
	    }
	    for(Int_t i=0;i<20-TMath::Nint(ievt/fiveperc);i++){
	      if(i>20) break;
	      stream << "-" ;
	    }
	    stream <<  "] ";
	    
	    if(TMath::Nint(ievt/fiveperc)<6) stream << "\033[1;31m" << 5*TMath::Nint(ievt/fiveperc) << "%\033[0m";
	    else if(TMath::Nint(ievt/fiveperc)<16) stream << "\033[1;33m" << 5*TMath::Nint(ievt/fiveperc) << "%\033[0m";
	    else stream << "\033[1;32m" << 5*TMath::Nint(ievt/fiveperc) << "%\033[0m";
	    stream << " completed.";  cout << stream.str() << flush;
	  }
	  
	  weight_lqeta = -99;
	  
	  for (Int_t ijt=0; ijt<njt; ijt++){
	    //	    cout << "Number of Jets : " << njt << "    Current ijt: " << ijt << "     Current weight: " << weight_lqeta << endl;
	    for (Int_t ibin=1; ibin<nbins; ibin++){
	      if (jteta[ijt] >= (xmin+(ibin-1)*binsize) && jteta[ijt] < (xmin+ibin*binsize))  {
		if (weight_lqeta == -99) weight_lqeta = corrfac[ibin]/mc_scale_sample[iinput];
		else weight_lqeta = weight_lqeta * corrfac[ibin]/mc_scale_sample[iinput];
		//		cout << "In Bin " << ibin << " and getting weight " << corrfac[ibin]/mc_scale_sample[iinput] << "    with corrfac = " << corrfac[ibin] << "    and mc_scale_sample= " << mc_scale_sample[iinput] << endl;
	      }
	    }
	  } // njt
	  if (weight_lqeta==-99) cout << "Not any bin found!" << endl;
	  //cout << "#########################" << endl << "### FINAL WEIGHT: " << weight_lqeta << "         ########" << endl;
	  b_weight_lqeta -> Fill();
	}  // ievt
      } //idir
      
    
      cout<<"Save extended tree to "<<input<<" ... "<<flush;
    ifile->Close();
    delete ifile;
    ofile->Write();
    ofile->Close();
    delete ofile;
    string cmd = "mv -f " + output + " " + input;
    gSystem->Exec(cmd.c_str());
    cout<<"DONE!\n"<<endl;

    } // mcfile
  } // mcsample
  
  */
  return 0;
}



//______________________________________________________________________________                                                      
string get_cut(const vector<string>& cuts,unsigned icut,const string& mode)
{
  if (icut>=cuts.size()) return string("");
  string tmp(cuts[icut]);
  size_t pos;
  pos = tmp.find(':');
  if (pos==string::npos) return tmp;
  string cutlabel = tmp.substr(0,pos);
  string cutexpression= tmp.substr(pos+1);
  return (mode.find("label")==string::npos) ? cutexpression : cutlabel;
}

