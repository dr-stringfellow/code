////////////////////////////////////////////////////////////////////////////////
//
// th_addsftodc_x
// -----------------
//
//                                    12/01/2012 Hauke Held <held@cern.ch>
//                                    02/09/2013 Benedikt Maier <bmaier@cern.ch>
////////////////////////////////////////////////////////////////////////////////

#include "../interface/CommandLine.h"
#include "../interface/RootStyle.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <istream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <cfloat>
#include <fstream>
#include <iterator>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////////////////////
string get_scale(const vector<string>& colors,const string&sample);

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  // additions for datacard handling
  string         datacard  = cl.getValue <string>  ("datacard","datacard.txt");
  vector<string> channels  = cl.getVector<string>  ("channels",        "Wmnu");
  vector<string> processes = cl.getVector<string>  ("processes",           "");
  string         systematic= cl.getValue <string>  ("systematic",     "ERROR");
  string         type      = cl.getValue <string>  ("type",             "lnN");
  vector<string> scale     = cl.getVector<string>  ("scale",            "1.0");

  //dummys

  
  if(!cl.check()) return 0;
  cl.print();
  
  //
  // UPDATE DATACARD
  //

  ifstream idcfile (datacard.c_str()); assert (idcfile.is_open());

  vector<unsigned> idefs;
  vector<string>   lines;
  while (idcfile.good()) {
    string line;
    getline (idcfile,line);
    lines.push_back(line);
    //    cout<<line<<endl;
    if (line.find("bin")==0) idefs.push_back(lines.size()-1);
  }
  //cout<<idefs.size()<<endl;
  assert (idefs.size()==2);
  //cout<<lines[idefs[1]]<<endl;
  //cout<<lines[idefs[1]+1]<<endl;

  stringstream line1(lines[idefs[1]]);
  stringstream line2(lines[idefs[1]+1]);
  istream_iterator<string> it1(line1);
  istream_iterator<string> it2(line2);
  istream_iterator<string> end;
  vector<string> bin(it1,end);
  vector<string> process(it2,end);
  assert (bin.size()==process.size());

  //  for (unsigned i=0;i<bin.size();i++) cout<<bin[i]<<" "<<process[i]<<endl;
  idcfile.close();


  vector<string> hits;
  for (unsigned i=0;i<channels.size();i++)
    for (unsigned j=0;j<processes.size();j++) {
      hits.push_back(string(channels[i]+"___UND___"+processes[j]));
    }

  int ws(30),wp(18);
  cout<<"Datacard "<<datacard<<" is being appended... ";
  ofstream dcfile;
  dcfile.open(datacard.c_str(),ofstream::app); assert (dcfile.is_open());
  stringstream ss; ss<<left;
  ss<<setw(ws-7)<<systematic<<setw(10)<<type;
  for (unsigned i=1;i<process.size();i++) {
    bool doScale = false;
    string entry = bin[i]+"___UND___"+process[i];

    for (unsigned k=0;k<hits.size()&&!doScale;k++)
      if (entry.c_str()==hits[k]) doScale = true;

    if (!doScale) ss<<setw(wp)<<"-";
    else {
      string p_scale=get_scale(scale,process[i]);
      ss<<setw(wp)<<p_scale;
    }
  } 
  ss<<endl;
  dcfile<<ss.str().c_str();
  dcfile.close();
  cout<<"done."<<endl;



  return 0;
}


//
// helper functions
//_____________________________________________________________

string get_scale(const vector<string>& weights,const string& name){
  for (vector<string>::const_iterator it=weights.begin();it!=weights.end();++it) {
    string weight(*it);
    size_t pos = weight.find(":");
    if (pos==string::npos) return weight;
    else if (weight.substr(0,pos)==name) return weight.substr(pos+1);
  }
  return "-";
}

