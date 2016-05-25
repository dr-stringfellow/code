////////////////////////////////////////////////////////////////////////////////
//
// th_userinfo_x
// --------------
//
// Saves additional information as part of the "UserInfo" of the TTree.
// This information can be read out by later programs - and used, for example,
//  for plotting labels with the correct cross section.
//
// Usage : th_userinfo_x -inputs Filename:xsec:eff:kfac
// Example : th_userinfo_x -inputs TTbar:248:1:1
//           (for a file called TTbar.root)
//
//            26/04/2010 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/CommandLine.h"

#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TObjString.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>


using namespace std;


void clean_without_efficiencies(TTree*tree);


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char**argv)
{
  //
  // evaluate command line / configuration file options
  //
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs    = cl.getVector<string>("inputs");
  string         datapath  = cl.getValue <string>("datapath", "");
  vector<string> dirnames  = cl.getVector<string>("dirnames", "");
  string         treename  = cl.getValue <string>("treename","t");
  bool           reset     = cl.getValue <bool>  ("reset", false);
  
  if (!cl.check()) return 0;
  cl.print();
  
  
  //
  // loop over input files
  //
  unsigned wsample(0);
  for (unsigned i=0;i<inputs.size();i++)
    if (inputs[i].length()>wsample) wsample = inputs[i].length();
    wsample += datapath.length()+6;
  
  cout<<endl
  <<setiosflags(ios::left)<<setw(wsample)<<"Sample"
  <<setiosflags(ios::left)<<setw(10)<<"Directory"
  <<setiosflags(ios::right)<<setw(10)<<"Events"
  <<setiosflags(ios::right)<<setw(15)<<"Cross-Section"
  <<setiosflags(ios::right)<<setw(15)<<"Efficiency"
  <<setiosflags(ios::right)<<setw(15)<<"K-Factor"
  <<setiosflags(ios::right)<<setw(15)<<"Int. Lumi"
  <<endl;

  for (unsigned i=0;i<inputs.size();i++) {
    string sample(inputs[i]);
    double xsection(-1.0);
    double efficiency(1.0);
    double kfactor(1.0);
    size_t pos = sample.find(':');
    if (pos!=string::npos) {
      string tmp  = sample.substr(pos+1);
      sample      = sample.substr(0,pos);      
      size_t pos2 = tmp.find(':');
      stringstream ssxsec; ssxsec<<tmp.substr(0,pos2); ssxsec>>xsection;
      if (pos2!=string::npos) {
        stringstream sseff; sseff<<tmp.substr(pos2+1); sseff>>efficiency;
        tmp = tmp.substr(pos2+1);
        size_t pos3 = tmp.find(':');
        if (pos3!=string::npos) {
          stringstream sskfac; sskfac<<tmp.substr(pos3+1); sskfac>>kfactor;
        }
      }
    }

    string filename(sample);
    if (filename.find(".root")==string::npos) filename += ".root";
    if (!datapath.empty()) filename = datapath + "/" + filename;
    string file_option = (pos==string::npos&&!reset) ? "READ" : "UPDATE";
    TFile* file = new TFile(filename.c_str(),file_option.c_str());
    if (!file->IsOpen()) { cout<<"Can't open "<<filename<<endl; return 0; }

    TIter nextdir(file->GetListOfKeys());
    TKey* dirkey(0);
    while ((dirkey=(TKey*)nextdir())) {
      if (strcmp(dirkey->GetClassName(),"TDirectoryFile")!=0) continue;
      TDirectory* dir = (TDirectory*)dirkey->ReadObj();
      string  dirname = dir->GetName();
      if (dirnames.size()>0&&
        find(dirnames.begin(),dirnames.end(),dirname)==dirnames.end()) continue;
      
      TTree* tree = (TTree*)dir->Get(treename.c_str());
      if (0==tree) {
        cout<<"Can't find tree "<<treename<<" in "<<filename<<":"<<dirname<<endl;
        return 0;
      }


      if (reset) {
        tree->GetUserInfo()->Clear();
        file->Write("",TObject::kOverwrite);
      }
      else if (pos!=string::npos) {
        clean_without_efficiencies(tree);

        stringstream ssxsection;
        ssxsection<<"xsection:"<<xsection;
        tree->GetUserInfo()->Add(new TObjString(ssxsection.str().c_str()));
        if (efficiency<1.0) {
          stringstream ssefficiency;
          ssefficiency<<"efficiency:"<<efficiency;
          tree->GetUserInfo()->Add(new TObjString(ssefficiency.str().c_str()));
        }
        if (kfactor!=1.0) {
          stringstream sskfactor;
          sskfactor<<"kfactor:"<<kfactor;
          tree->GetUserInfo()->Add(new TObjString(sskfactor.str().c_str()));
        }
        file->Write("",TObject::kOverwrite);
      }

      vector<double> efficiencies;
      
      TIter next(tree->GetUserInfo());
      while (TObject* obj = next()) {
        TObjString* objstr = dynamic_cast<TObjString*>(obj); if(0==objstr)continue;
        string str = objstr->String().Data();
        size_t pos2 = str.find(':'); assert(pos2!=string::npos);
        string key(str.substr(0,pos2));
        stringstream ssvalue; ssvalue<<str.substr(pos2+1);
        double value; ssvalue>>value;
        if      (key=="efficiency") efficiencies.push_back(value);
        else if (key=="xsection")   xsection = value;
        else if (key=="kfactor")    kfactor  = value;
        else                        cout<<"Unknown key: "<<key<<endl;
      }

      double efftot(1.0);
      for (unsigned j=0;j<efficiencies.size();j++) efftot *= efficiencies[j];
      
      cout<<resetiosflags(ios::right)
      <<setiosflags(ios::left)<<setw(wsample)<<sample
      <<setiosflags(ios::left)<<setw(10)<<dirname
      <<setiosflags(ios::right)<<setw(10)<<tree->GetEntries()
      <<setiosflags(ios::right)<<setw(15)<<xsection
      <<setw(15)<<efftot
      <<" ("<<efficiencies.size()<<")"
      <<setiosflags(ios::right)<<setw(11)<<kfactor
      <<setiosflags(ios::right)<<setw(15)
      <<(tree->GetEntries()/(xsection*kfactor*efftot))
      <<endl;
    }
  }


  cout<<endl;
  
  
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

void clean_without_efficiencies(TTree* tree){

  vector<double> efficiencies_tmp;
  TIter next(tree->GetUserInfo());
  while (TObject* obj = next()) {
    TObjString* objstr = dynamic_cast<TObjString*>(obj); if(0==objstr)continue;
    string str = objstr->String().Data();
    size_t pos2 = str.find(':'); assert(pos2!=string::npos);
    string key(str.substr(0,pos2));
    stringstream ssvalue; ssvalue<<str.substr(pos2+1);
    double value; ssvalue>>value;
    if      (key=="efficiency") efficiencies_tmp.push_back(value);
  }
  tree->GetUserInfo()->Clear();
  for(UInt_t i=0; i< efficiencies_tmp.size(); i++){
    stringstream ssefficiency_tmp;
    ssefficiency_tmp<<"efficiency:"<<efficiencies_tmp.at(i);
    tree->GetUserInfo()->Add(new TObjString(ssefficiency_tmp.str().c_str()));
  }
  return;
}
