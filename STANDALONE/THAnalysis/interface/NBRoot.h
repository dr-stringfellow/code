#ifndef HELD_NBANALYSIS_NBROOT_H
#define HELD_NBANALYSIS_NBROOT_H 1

#include <TTree.h>

#include <string>
#include <vector>

class NBRoot;

//
// class definition
//

class NBRoot
{

  //
  // construction / destruction
  //
public:
  NBRoot();
  NBRoot(TTree* tree,
	 const std::vector<std::string>& varnames);
  virtual ~NBRoot();

  //
  // member functions
  //
public:
  void setTree(TTree* tree)                        {tree_    =tree;}
  void setVars(std::vector<std::string>& varnames) {varnames_=varnames;}
  void setArray(unsigned entry,float* array);

private:
  void init();

  //
  // member data
  //
private:
  TTree*                   tree_;
  std::vector<Double_t*>   values_;
  std::vector<std::string> varnames_;
  unsigned                 nentries_;
};

#endif
