////////////////////////////////////////////////////////////////////////////////
//
// th_flavorschemematcher_x
// -----------------
//
//            04/06/2013 Benedikt Maier <bmaier@cern.ch>
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
// helper
////////////////////////////////////////////////////////////////////////////////
//-------

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
  
  //vector<string> inputs      = cl.getVector<string>("inputs");
  string         input     = cl.getValue <string>  ("input"                  );
  string         target    = cl.getValue <string>  ("target"                 );
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         ipath     = cl.getValue <string>  ("ipath",              ".");
  string         opath     = cl.getValue <string>  ("opath",              ".");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         datapath  = cl.getValue <string>  ("datapath",           "/");
  

  
  if (!cl.check()) return 0;
  cl.print();
  
  

      
  // read input file
  TFile* ifile = new TFile((ipath+"/"+input).c_str(),"READ");
  if (!ifile->IsOpen()) { cout<<"Failed to open "<<input<<endl; return 0; }
  
  // read target file
  TFile* targetfile = new TFile((ipath+"/"+target).c_str(),"READ");
  if (!targetfile->IsOpen()) { cout<<"Failed to open "<<target<<endl; return 0; }


  // create temporary output file
  size_t pos = input.find(".root");
  string output = input.substr(0,pos);
  output = output + ".fs.root";
  TFile* ofile = new TFile((opath+"/"+output).c_str(),"RECREATE");
  if (!ofile->IsOpen()) { cout<<"Failed to create "<<output<<endl; return 0; }
  
  // read input directory
  TDirectory* idir    = (TDirectory*)ifile->Get(dirname.c_str());
  cout<<input<<"/"<<dirname<<":"<<endl;
  
  // read target directory
  TDirectory* targetdir    = (TDirectory*)targetfile->Get(dirname.c_str());
  cout<<target<<"/"<<dirname<<":"<<endl;


  // create output directory
  TDirectory* odir = (TDirectory*)ofile->mkdir(dirname.c_str());
  if(0==odir){cout<<"Failed to make "<<dirname<<" in "<<output<<endl;return 0;}
  
  // open input tree
  TTree* itree = (TTree*)idir->Get(treename.c_str());
  if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<dirname<<endl;return 0;}

  // don't copy H->bb branches if they already exist
  if (0!=itree->FindBranch("sbptrwgt"))     itree->SetBranchStatus("sbptrwgt",    0);
  // don't copy H->bb branches if they already exist
  if (0!=itree->FindBranch("sbptrwgt2d"))     itree->SetBranchStatus("sbptrwgt2d",    0);
  // don't copy H->bb branches if they already exist
  if (0!=itree->FindBranch("test"))     itree->SetBranchStatus("test",    0);


  // open target tree
  TTree* targettree = (TTree*)targetdir->Get(treename.c_str());
  if(0==itree){cout<<"No "<<treename<<" in "<<output<<":"<<dirname<<endl;return 0;}

    
  
  // clone input tree into output directory
  odir->cd();
  TTree* otree = itree->CloneTree();
  cout<<output<<"/"<<dirname<<"/"<<treename<<" created (temporary!)"<<endl;
  
  
  
  // create H->bb branches in output tree
  
  Float_t sbptrwgt;
  Float_t sbptrwgt2d;
  Float_t test;

  TBranch* b_sbptrwgt        =otree->Branch("sbptrwgt",       &sbptrwgt,        "sbptrwgt/F");
  TBranch* b_sbptrwgt2d      =otree->Branch("sbptrwgt2d",     &sbptrwgt2d,      "sbptrwgt2d/F");
  TBranch* b_test      =otree->Branch("test",     &test,      "test/F");


  bool twodim = true;

  if(twodim)

    {
  //--------------
  //1D REWEIGHTING
  //--------------

  // setup input branches needed to fill H->bb branches

  // don't copy H->bb branches if they already exist
  //if (0!=itree->FindBranch("sbptrwgt"))     itree->SetBranchStatus("sbptrwgt",    0);


  Float_t sbpt;
  Float_t targetsbpt;
  
  itree->SetBranchAddress("sbpt",   &sbpt);
  targettree->SetBranchAddress("sbpt",   &targetsbpt);
  
  // loop over all events and fill H->bb branches
  unsigned nevt = static_cast<unsigned>(itree->GetEntries());

  cout<<nevt<<" events will be processed"<<endl;
  unsigned nevttarget = static_cast<unsigned>(targettree->GetEntries());

  Int_t entries[100]={0};
  for(int i=0; i<100; i++)
    entries[i]=0;
  Int_t targetentries[100]={0};
  for(int i=0; i<100; i++)
    targetentries[i]=0;
  Float_t i=0;
  Float_t j=4;
  Int_t u=0;
  


  cout << "lalala" << endl;
  


  if(input!=target)
    {
      
      
      while(j<=400)
	{
	  
	  for (unsigned ievt=0;ievt<nevt;ievt++) {
	    
	    
	    itree->GetEntry(ievt);
	    
	    if(sbpt>i && sbpt<=j)
	      {
		entries[u]++;
	      }
	    
	  } // ievt
	  cout << entries[u] << endl;
	  i=i+4;
	  j=j+4;
	  u++;
	  cout << j << endl;
	}
      
      
      
      i=0;
      j=4;
      u=0;
      
      while(j<=400)
	{
	  
	  for (unsigned ievt=0;ievt<nevttarget;ievt++) {
	    
	    
	    targettree->GetEntry(ievt);
	    
	    if(targetsbpt>i && targetsbpt<=j)
	      {
		targetentries[u]++;
	      }
	    
	  } // ievt
	  cout << targetentries[u] << endl;
	  i=i+4;
	  j=j+4;
	  u++;
	  cout << j << endl;
	}
      
      
      Float_t weights[100]={0};
      
      for(int k=0; k<100; k++)
	{
	  if(entries[k]>0)
	    weights[k]=(Float_t)targetentries[k]/(Float_t)entries[k];
	  else weights[k]=0;
	  
	  
	  cout << "Weight k: " << weights[k] << endl;
	}
      
      
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {
	
	
	i=0;
	j=4;
	u=0;
	
	sbptrwgt=1;
	
	itree->GetEntry(ievt);
	
	while(j<=400)
	  {
	    //cout << "Test" << endl;
	    
	    if(sbpt>i && sbpt<=j)
	      {
		sbptrwgt=weights[u]; 
		//cout<< "lalala " << sbptrwgt << endl;
		
	      }
	    i=i+4;
	    j=j+4;
	    u++;
	  } 
	//sbptrwgt=5;
	test=7;
	b_test          ->Fill();
	b_sbptrwgt	->Fill();
      }// ievt
      
    }
  
  if(input==target)
    {
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {
	
	itree->GetEntry(ievt);
	sbptrwgt=1;
	b_sbptrwgt	->Fill();
	
      }// ievt
      
    }
  
    }
  




  

  if(twodim)

    {


      // don't copy H->bb branches if they already exist
      if (0!=itree->FindBranch("sbptrwgt2d"))     itree->SetBranchStatus("sbptrwgt2d",    0);
  //--------------
  //2D REWEIGHTING
  //--------------

  // setup input branches needed to fill H->bb branches

  
  Float_t sbpt;
  Float_t sbeta;
  Float_t targetsbpt;
  Float_t targetsbeta;
  
  itree->SetBranchAddress("sbpt",   &sbpt);
  itree->SetBranchAddress("sbeta",   &sbeta);
  targettree->SetBranchAddress("sbpt",   &targetsbpt);
  targettree->SetBranchAddress("sbeta",   &targetsbeta);

  // loop over all events and fill H->bb branches
  unsigned nevt = static_cast<unsigned>(itree->GetEntries());

  cout<<nevt<<" events will be processed"<<endl;
  unsigned nevttarget = static_cast<unsigned>(targettree->GetEntries());

  Int_t entries[40][40]={{0},{0}};

  Int_t targetentries[40][40]={{0},{0}};

  Float_t i_pt=0;
  Float_t j_pt=10;
  Int_t u_pt=0;

  Float_t i_eta=-8;
  Float_t j_eta=-7.6;
  Int_t u_eta=0;


  


  cout << "lalala" << endl;
  


  if(input!=target)
    {
      
      
      while(j_pt<=400)
	{
	  
	  while(j_eta<=8)
	    {
	  
	      for (unsigned ievt=0;ievt<nevt;ievt++) {
		
		
		itree->GetEntry(ievt);
		
		if(sbpt>i_pt && sbpt<=j_pt && sbeta>i_eta && sbeta<=j_eta)
		  {
		    entries[u_pt][u_eta]++;
		  }
		
	      } // ievt
	      
	      i_eta=i_eta+0.4;
	      j_eta=j_eta+0.4;
	      cout << entries[u_pt][u_eta] << endl;
	      u_eta++;

	    }
	  i_eta=-8;
	  j_eta=-7.6;
	  u_eta=0;

	  
	  i_pt=i_pt+10;
	  j_pt=j_pt+10;
	  u_pt++;
	  cout << j_pt << endl;

	}
      
      
      
      i_pt=0;
      j_pt=10;
      u_pt=0;
      
      i_eta=-8;
      j_eta=-7.6;
      u_eta=0;
      
      while(j_pt<=400)
	{

	  while(j_eta<=8)
	    {
	  
	      for (unsigned ievt=0;ievt<nevttarget;ievt++) {
		
		
		targettree->GetEntry(ievt);
		
		if(targetsbpt>i_pt && targetsbpt<=j_pt && targetsbeta>i_eta && targetsbeta<=j_eta)
		  {
		    targetentries[u_pt][u_eta]++;
		  }
		
	      } // ievt
	      
	      i_eta=i_eta+0.4;
	      j_eta=j_eta+0.4;
	      u_eta++;

	    }

	  i_eta=-8;
	  j_eta=-7.6;
	  u_eta=0;
	  
	  i_pt=i_pt+10;
	  j_pt=j_pt+10;
	  u_pt++;
	  cout << j_pt << endl;
	}
      
      
      Float_t weights[40][40]={{0},{0}};
      
      for(int k=0; k<40; k++)
	{
	  for(int y=0; y<40; y++)
	    {
	      if(entries[k][y]>0)
		weights[k][y]=(Float_t)targetentries[k][y]/(Float_t)entries[k][y];
	      else weights[k][y]=0;
	      
	      
	      cout << "Weight k: " << weights[k][y] << endl;
	    }
	}
      
      
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {
	
	
	i_pt=0;
	j_pt=10;
	u_pt=0;
	
	i_eta=-8;
	j_eta=-7.6;
	u_eta=0;

	sbptrwgt2d=1;
	
	itree->GetEntry(ievt);
	
	while(j_pt<=400)
	  {
	    //cout << "Test" << endl;
	    
	    while(j_eta<=8)
	      {

		if(sbpt>i_pt && sbpt<=j_pt &&sbeta>i_eta && sbeta<=j_eta)
		  {
		    sbptrwgt2d=weights[u_pt][u_eta]; 
		    //cout<< "lalala " << sbptrwgt << endl;
		    
		  }
		
		i_eta=i_eta+0.4;
		j_eta=j_eta+0.4;
		u_eta++;
		
	      }
	    i_eta=-8;
	    j_eta=-7.6;
	    u_eta=0;
	    
	    i_pt=i_pt+10;
	    j_pt=j_pt+10;
	    u_pt++;
	  } 
	//cout << "SBPT NOW: " << sbptrwgt2d << endl;
	//sbptrwgt2d=34;
	//cout << "SBPT AFTER: " << sbptrwgt2d << endl;
	b_sbptrwgt2d	->Fill();
      }// ievt
      
    }
  
  if(input==target)
    {
      
      for (unsigned ievt=0;ievt<nevt;ievt++) {
	
	itree->GetEntry(ievt);
	sbptrwgt=1;
	sbptrwgt2d=1;
	b_sbptrwgt	->Fill();
	b_sbptrwgt2d	->Fill();
	
      }// ievt
      
    }
  
    }

 
  cout<<"Save extended tree to "<<output<<" ... "<<flush;
  ifile->Close();
  targetfile->Close();
  delete targetfile;
  delete ifile;
  ofile->Write();
  ofile->Close();
  delete ofile;
  
  //string cmd = "mv " + output + " " + input;
  //cout << cmd << endl;
  //gSystem->Exec(cmd.c_str());
  //gSystem->Exec("touch me");
  cout<<"DONE!\n"<<endl;
  



return 0;
}



