////////////////////////////////////////////////////////////////////////////////
//
// RunRangeManager
// ---------------
//
//            28/09/2010 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/RunRangeManager.h"


#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


////////////////////////////////////////////////////////////////////////////////
// construction / destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
RunRangeManager::RunRangeManager()
{

}


//______________________________________________________________________________
RunRangeManager::RunRangeManager(const string& jsonFileName)
{
  if (!readJsonFile(jsonFileName))
    cerr<<"RunRangeManager WARNING: construction from "<<jsonFileName<<" failed!"
	<<endl;
}


//______________________________________________________________________________
RunRangeManager::~RunRangeManager()
{

}


////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void RunRangeManager::clear()
{
  runRangeMap_.clear();
}


//______________________________________________________________________________
bool RunRangeManager::insertLumiSection(unsigned run, unsigned lbn)
{
  RunRangeMap_t::iterator itRun = runRangeMap_.find(run);
  if (itRun==runRangeMap_.end()) {
    set<unsigned> lbns;
    lbns.insert(lbn);
    runRangeMap_[run] = lbns;
  }
  else {
    itRun->second.insert(lbn);
  }
  return true;
}


//______________________________________________________________________________
bool RunRangeManager::removeLumiSection(unsigned run, unsigned lbn)
{
  RunRangeMap_t::iterator itRun = runRangeMap_.find(run);
  if (itRun==runRangeMap_.end()) return false;
  itRun->second.erase(lbn);
  return true;
}


//______________________________________________________________________________
bool RunRangeManager::readJsonFile(const string& fileName)
{
  ifstream fin(fileName.c_str(), ios::in);
  if (!fin.is_open()) {
    cerr<<"Can't open JSON file "<<fileName<<endl;
    return false;
  }

  clear();
  
  string s; unsigned run(0), lbnmin(0), lbnmax(0);
  while (fin>>s) {
    if (0==run) {
      stringstream ss;
      ss<<s.substr(s.find_first_of('"')+1,s.find_last_of('"')-1);
      ss>>run;
      assert(runRangeMap_.find(run)==runRangeMap_.end());
      set<unsigned> lbns;
      runRangeMap_[run] = lbns;
    }
    else {
      if (0==lbnmin) {
	lbnmax=0;
	stringstream ss;
	ss<<s.substr(s.find_last_of('[')+1,s.find(',')-1);
	ss>>lbnmin;
      }
      else if (0==lbnmax) {
	assert(lbnmin>0);
	stringstream ss;
	ss<<s.substr(0,s.find_first_of('[')-1);
	ss>>lbnmax;
	for (unsigned lbn=lbnmin;lbn<=lbnmax;lbn++) runRangeMap_[run].insert(lbn);
	lbnmin=0;
	if (s.find("]]")!=string::npos) run=0;
      }
    }
  }
  
  fin.close();
  
  return true;
}


//______________________________________________________________________________
unsigned RunRangeManager::nRuns() const
{
  return runRangeMap_.size();
}


//______________________________________________________________________________
unsigned RunRangeManager::nLbns() const
{
  unsigned result(0);
  RunRangeMap_t::const_iterator itRun=runRangeMap_.begin();
  for (;itRun!=runRangeMap_.end();++itRun) result += itRun->second.size();
  return result;
}


//______________________________________________________________________________  
bool RunRangeManager::contains(unsigned run,unsigned lbn) const
{
  RunRangeMap_t::const_iterator itRun = runRangeMap_.find(run);
  if (itRun==runRangeMap_.end()) return false;
  return (itRun->second.find(lbn)!=itRun->second.end());
}


//______________________________________________________________________________
bool RunRangeManager::writeJsonFile(const string& fileName) const
{
  ofstream fout(fileName.c_str());
  if (!fout.is_open()) {
    cerr<<"RunRangeManager::writeJsonFile() ERROR: can't create "<<fileName<<endl;
    return false;
  }
  
  fout<<"{"<<endl;
  
  RunRangeMap_t::const_iterator itRun = runRangeMap_.begin();
  for (;itRun!=runRangeMap_.end();++itRun) {
  
    if (itRun!=runRangeMap_.begin()) fout<<","<<endl;
    
    unsigned run = itRun->first;
    fout<<"\""<<run<<"\": [";
    
    bool first(true); unsigned lbnmin(0),lbnmax(0);
    set<unsigned>::const_iterator itLbn=itRun->second.begin();
    for (;itLbn!=itRun->second.end();++itLbn) {
      
      unsigned lbn(*itLbn);

      if (lbnmin==0) {
	lbnmin = lbn;
	lbnmax = lbnmin;
      }
      else if (lbnmax==lbn-1) {
	lbnmax++;
      }
      else {
	if (first) first = false; else fout<<", ";
	fout<<"["<<lbnmin<<","<<lbnmax<<"]";
	lbnmin = 0; lbnmax = 0;
      }
      
    } // itLbn
    
    if (lbnmin!=0) {
      if (!first) fout<<", ";
      fout<<"["<<lbnmin<<","<<lbnmax<<"]";
    }
    
    fout<<"]";
  
  } // itRun

  fout<<endl<<"}"<<endl;
  fout.close();

  return true;
}
