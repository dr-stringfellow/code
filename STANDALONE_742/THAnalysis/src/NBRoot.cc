////////////////////////////////////////////////////////////////////////////////
//
// NBRoot
// ---------------
//
//                                          13/12/2011 Hauke Held <held@cern.ch>
////////////////////////////////////////////////////////////////////////////////

#include "../interface/NBRoot.h"

#include "TTree.h"
#include "TCanvas.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <cstdlib>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTION / DESTRUCTION
////////////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
NBRoot::NBRoot()
{

}

//______________________________________________________________________________
NBRoot::NBRoot(TTree* tree,
	       const std::vector<std::string>& varnames) :
  tree_(tree) , 
  varnames_(varnames)
{
  init();
}

//______________________________________________________________________________
NBRoot::~NBRoot()
{

}

//______________________________________________________________________________
void NBRoot::init()
{
  assert (tree_);
  if (varnames_.size()==0) return;

  TCanvas* dummy = new TCanvas("dummy","dummy",5,5,5,5);
  dummy->cd();

  nentries_ = tree_->GetEntries();
  tree_->SetEstimate(nentries_);

  for (unsigned ivar=0;ivar<varnames_.size();ivar++) {
    tree_->Draw(varnames_[ivar].c_str());
    Double_t* arr_tmp = tree_->GetV1();
    Double_t* vals    = new Double_t[nentries_];
    for (unsigned i=0;i<nentries_;i++) vals[i] = arr_tmp[i];
    values_.push_back(vals);
  }
  assert (varnames_.size()==values_.size());
  delete dummy;
}

//______________________________________________________________________________
void NBRoot::setArray(unsigned entry,float* array)
{
  if (varnames_.size()==0) return;

  assert (values_.size());
  assert (entry<nentries_);

  for (unsigned ivar=0;ivar<values_.size();ivar++)
    array[ivar] = values_[ivar][entry];
}
