#ifndef MCWEIGHTERS_H
#define MCWEIGHTERS_H

#include <boost/property_tree/ptree.hpp>
#include "TH3.h"
#include <TFile.h>

// this file defines some Processors used to (re)weight mc events. They are usually run at the very beginning
// of an analysis path
// just apply a given constant factor to each event
// reweighting based on the poisson_mean
//
// note that this only works with pileup histograms (both for MC and data) which
// * start at 0
// * have the same binwidth (after rebinning)
// one of the histograms (either data or MC) might be larger than the other, but then the information of the smaller one
// is truncated (effectively setting all of the "extra" bins missing to 0.0).
//
// For data, use the procedure described at
// https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData
// For MC, make a histogram of the rec.genInfo->pileup_intime for all events before the selection!

class OneDPileupReweighter{
public:
    OneDPileupReweighter(TH1 * data_poisson_means, TH1* mc_poisson_means);
    double getPileupWeight();
    ~OneDPileupReweighter(){}
private:
    double binwidth;
    std::vector<double> pileup_weights;
};


// 3D reweighting based on actual in-time pileup, oot before and oot after pileup.
// requires properly normalized, cubic TH3D which directly contains the event weight (x=oot before, y = intime, z = oot after).
// To generate the 3D matrix for the input, see
// misc/weight3d.cc
// be sure to use the pileup scenario which corresponds to the MC and data you are using. Also note that
// this does not handle known issues with the random seed initialization.

class ThreeDPileupReweighter{
	public:
		ThreeDPileupReweighter(std::string,std::string);
		double getPileupWeight();
		~ThreeDPileupReweighter(){
		  delete pileup_w;
		  histfile->Close();
		}

 private:
		TH3D* pileup_w;	
		TH3D* pileup_w_systup;	
		TH3D* pileup_w_systdown;	
		int weight_pileup_i_up;
		int weight_pileup_i_down;
		int max_npu;
		TFile* histfile;
		
};


// use sample-info.cfg to get the cross section weight factor.
// Also allows to use a pileup root file to reweight according to the number of
// true pileup events (if not given, no pileup reweighting will be done).
//
// SampleInfoWeighter can be used together with GridControlSample:
// * the 'FILE_NAMES' environment variable for the default location of 'sample-info.cfg'
// * DATASETPATH environment variable is used per default to locate the relevant sample  in sample-info.cfg
// both of that can be overriden by passing non-empty strings to the constructor.
//
// The sample-info.cfg file is in the boost property_tree info format which is documented here:
//  http://www.boost.org/doc/libs/1_42_0/doc/html/boost_propertytree/parsers.html#boost_propertytree.parsers.info_parser
//
// a minimal, commented example is here:
/*
; there should be one entry for real data, specifying the estimated lumi and pileup:
DATA {
    lumi 1.0 ; in 1/pb
    pileup pileup.root ; will search for a histogram named "pileup" in this file and use it for 1D in-time reweighting
}

; there should be one entry per MC sample. Selecting the right one is done via the DATASETPATH environment
; variable (or constructor argument) which is matched against the 'datasetpath' setting given here in this file.
; Note that the first matching entry will be used (this is important if using wildcard with similar dataset names).
ttbar {  ; arbitrary, but unique name
   datasetpath MC_MadGraph_TTbar*   ; dataset path for matching
   nevents 1286491   ; number of events before all cuts
   xsec 157.5 ; cross section before all cuts
   ; exactly one of those should be given:
   pileup pileup.root:pileup:10 ; true pileup for 1D in-time / 3D pileup reweighting. If not given, this pileup reweighting is not done.
}
*/
//
// The 'pileup' setting controls the poisson-mean-based pileup-reweighting ("truth"-based). The general notation of this option is:
// <rootfile name>:<histogram name>:<rebin factor>
// where <rebin factor> defaults to 1 if not given. Example:
// data_pileup.root:pileup:25

class SampleInfoWeighter{
 private:
  double xs_weight;
  OneDPileupReweighter* pileup_weights_1D;
  ThreeDPileupReweighter* pileup_weights_3D;
  bool apply_pileup, apply_xsec;
  int weight_onepb_i, weight_pileup_i;
  
 public:
  // If path_to_sampleinfo_cfg is empty, it is looked up at the directory one above the first
  //    filename in the environment variable FILE_NAMES
  // If datasetpath is empty, the environment variable DATASETPATH (usually set by grid-control) is used
  explicit SampleInfoWeighter(const std::string & path_to_sampleinfo_cfg = "", const std::string & datasetpath = "", const std::string & systematic_shift = "");
  
  void setApplyPileupWeight(bool apply_pileup_weight){
        apply_pileup = apply_pileup_weight;
  }
  void setApplyXSecWeight(bool apply_xsec_weight){
    apply_xsec = apply_xsec_weight;
  }
  double getWeights1D();
  double getWeights3D();
};

#endif

