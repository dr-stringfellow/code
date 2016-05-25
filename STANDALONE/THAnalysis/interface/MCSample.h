#ifndef THANALYSIS_MCSAMPLE_H
#define THANALYSIS_MCSAMPLE_H 1


#include <string>
#include <vector>
#include <map>


class MCSample;
class TTree;


//
// CLASS DEFINITION
//
class MCSample
{
  //
  // construction / destruction
  //
public:
  MCSample();
  MCSample(const std::string& name);
  virtual ~MCSample();

  
  //
  // member functions
  //
public:
  void   addTree(const std::string& filename,TTree* tree,
		 double xsec,double eff,double kfac);
  double nevents(const std::string& selection="",double lumi=0.0) const;
  double nentries(const std::string& expr,
		  const std::string& selection="",double lumi=0.0) const;
  void   project(const std::string& hname,
		 const std::string& expr,
		 const std::string& presel);
  void   projectSys(const std::string& hname, const std::string& hnameup,const std::string& hnamedown,
		    const std::string& expr,
		    const std::string& presel);
  void   projectNoWeight(const std::string& hname,
		 const std::string& expr,
		 const std::string& presel);
  void   project2d(const std::string& hname,
		   const std::string& expr,
		   const std::string& presel);
  
  std::string name()                 const { return name_; }
  unsigned    ntrees()               const { return trees_.size(); }
  std::string filename(unsigned i)   const { return filenames_[i]; }
  TTree*      tree(unsigned i)       const { return trees_[i]; }
  double      xsec(unsigned i)       const { return xsecs_[i]; }
  double      efficiency(unsigned i) const { return efficiencies_[i]; }
  double      kfactor(unsigned i)    const { return kfactors_[i]; }
  std::string weight(unsigned i)     const { return weights_[i]; }

  static bool getXsection(TTree* t,double& xsection);
  static bool getKfactor(TTree* t,double& kfactor);
  static bool getEfficiency(TTree* t,double& efficiency);
  static bool getWeight(TTree* t,double& weight);
  
  void setWeights(const std::vector<std::string>& weights);
  void setSFup(const std::vector<std::string>& sfup);
  void setSFdown(const std::vector<std::string>& sfdown);

  static std::vector<MCSample> loadSamples(const std::vector<std::string>& mcs,
					   const std::string& dirname,
					   const std::string& treename,
					   const std::string& mcpath=".",
					   const std::string& filesuffix=".root");
  static void decodeDescriptor(const std::string& descriptor,
			       std::string& name,std::string& filename,
			       double& xsec,double& eff,double& kfac);
  static unsigned printSampleInfos(const std::vector<MCSample>& mcsamples);
  static  std::string printline(unsigned length,const char& c='=');  
  
  
  //
  // member data
  //
private:
  std::string              name_;
  std::vector<std::string> filenames_;
  std::vector<TTree*>      trees_;
  std::vector<double>      xsecs_;
  std::vector<double>      efficiencies_;
  std::vector<double>      kfactors_;
  std::vector<std::string> weights_;
  std::vector<std::string> sfup_;
  std::vector<std::string> sfdown_;
};

#endif
