#ifndef THANALYSIS_DATASAMPLE_H
#define THANALYSIS_DATASAMPLE_H 1


#include <string>
#include <vector>


class DataSample;
class TTree;


//
// CLASS DEFINITION
//
class DataSample
{
  //
  // construction / destruction
  //
public:
  DataSample();
  virtual ~DataSample();

  
  //
  // member functions
  //
public:
  bool   empty() { return trees_.size()==0; }
  void   addTree(const std::string& filename,TTree* t);
  void   addTree(const std::string& filename,TTree* t,const std::string& selection);
  void   addTree(const std::string& filename,TTree* t,const std::string& selection,
		 float prescale);
  double nevents(const std::string& selection) const;
  void   project(const std::string& hname,
		 const std::string& expr,
		 const std::string& presel);
  void   project2d(const std::string& hname,
		   const std::string& expr,
		   const std::string& presel);
  void   projectMM(const std::string& hname,
		   const std::string& expr,
		   const std::string& loosesel,
		   const std::string& tightcut,
		   std::string        eps_sig,
		   std::string        eps_qcd);
  
  unsigned    ntrees()              const { return trees_.size(); }
  
  std::string filename(unsigned i)  const { return filenames_[i]; }
  TTree*      tree(unsigned i)      const { return trees_[i]; }
  std::string selection(unsigned i) const { return selections_[i]; }
  float       prescale(unsigned i)  const { return prescales_[i]; }

  static DataSample load(const std::vector<std::string>& data,
			 const std::string&              dirname,
			 const std::string&              treename,
			 const std::string&              datapath=".",
			 const std::string&              filesuffix=".root");
 static DataSample loadsingle(const std::string        & data,
			 const std::string&              dirname,
			 const std::string&              treename,
			 const std::string&              datapath=".",
			 const std::string&              filesuffix=".root");
  static void decodeDescriptor(const std::string& descriptor,
			       std::string&       filename,
			       std::string&       selection,
			       float&             prescale);
  static void printInfo(const DataSample& datasample);
  static std::string printline(unsigned length,const char& c='=');
  
  
  //
  // class members
  //
private:
  std::vector<std::string> filenames_;
  std::vector<TTree*>      trees_;
  std::vector<std::string> selections_;
  std::vector<float>       prescales_;

};


#endif
