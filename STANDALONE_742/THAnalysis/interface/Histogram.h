#ifndef THANALYSIS_HISTOGRAM_H
#define THANALYSIS_HISTOGRAM_H 1

#include "TH1F.h"


#include <string>
#include <vector>
#include <map>


class Histogram;


//
// CLASS DEFINITION
//
class Histogram
{
  //
  // construction / destruction
  //
public:
  Histogram();
  Histogram(const std::string &name,
	    const std::string& title,
	    const std::string& expr,
	    unsigned nbinsx,
	    double   xmin,
	    double   xmax,
	    bool     logx=false,
	    bool     logy=false);
  Histogram(const std::string &name,
	    const std::string& title,
	    const std::string& expr,
	    const std::string& selection,
	    unsigned nbinsx,
	    double   xmin,
	    double   xmax,
	    bool     logx=false,
	    bool     logy=false);

  Histogram(const std::string &name,
	    const std::string& title,
	    const std::string& expr,
	    std::vector<double> binsx,
	    bool     logx=false,
	    bool     logy=false);
  Histogram(const std::string &name,
	    const std::string& title,
	    const std::string& expr,
	    const std::string& selection,
	    std::vector<double> binsx,
	    bool     logx=false,
	    bool     logy=false);


  //
  // member functions
  //
public:
  TH1F* create(const std::string& prefix);
  
  void setName(const std::string& name) { name_=name; }
  void setXYTitle(const std::string& title);
  void setXTitle(const std::string& xtitle) { xtitle_=xtitle; }
  void setYTitle(const std::string& ytitle) { ytitle_=ytitle; }
  void setExpr(const std::string& expr) { expr_= expr; }
  void setSelection(const std::string& selection) { selection_=selection; }
  void setNbinsx(unsigned nbinsx) { nbinsx_=nbinsx; }
  void setBinsx(const std::vector<double>& binsx) { binsx_=binsx; }
  void setXmin(double xmin) { xmin_=xmin; }
  void setXmax(double xmax) { xmax_=xmax; }
  void setLogx(bool logx) { logx_=logx; }
  void setLogy(bool logy) { logy_=logy; }

  std::string name()      const { return name_; }
  std::string title()     const { return ";"+xtitle_+";"+ytitle_; }
  std::string xtitle()    const { return xtitle_; }
  std::string ytitle()    const { return ytitle_; }
  std::string expr()      const { return expr_; }
  std::string selection() const { return selection_; }
  unsigned    nbinsx()    const { return (binsx_.size()>0)?binsx_.size()-1:nbinsx_;}
  double      xmin()      const { return (binsx_.size()>0)?binsx_.front():xmin_; }
  double      xmax()      const { return (binsx_.size()>0)?binsx_.back() :xmax_; }
  bool        logx()      const { return logx_; }
  bool        logy()      const { return logy_; }
  
  bool        variableBinWidth() const { return binsx_.size()>0; }

  static void divideBinContentByBinWidth(TH1F* h);
  static bool hasVariableBinWidth(TH1* h);
  static std::map<std::string,Histogram> getHistogramMap();


  //
  // member data
  //
private:
  std::string         name_;
  std::string         xtitle_;
  std::string         ytitle_;
  std::string         expr_;
  std::string         selection_;
  unsigned            nbinsx_;
  std::vector<double> binsx_;
  double              xmin_;
  double              xmax_;
  bool                logx_;
  bool                logy_;

};

#endif
