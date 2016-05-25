#ifndef THANALYSIS_RUNRANGEMANAGER_H
#define THANALYSIS_RUNRANGEMANAGER_H


#include <string>
#include <set>
#include <map>


class RunRangeManager
{
  //
  // construction / destruction
  //
public:
  RunRangeManager();
  RunRangeManager(const std::string& jsonFileName);
  virtual ~RunRangeManager();


  //
  // member functions
  //
public:
  void     clear();
  bool     insertLumiSection(unsigned run, unsigned lbn);
  bool     removeLumiSection(unsigned run, unsigned lbn);
  bool     readJsonFile(const std::string& fileName);
  
  unsigned nRuns() const;
  unsigned nLbns() const;
  bool     contains(unsigned run,unsigned lbn) const;
  bool     writeJsonFile(const std::string& fileName) const;
  
  
  //
  // data members
  //
private:
  typedef std::map<unsigned, std::set<unsigned> > RunRangeMap_t;

  RunRangeMap_t runRangeMap_;
  
};

#endif
