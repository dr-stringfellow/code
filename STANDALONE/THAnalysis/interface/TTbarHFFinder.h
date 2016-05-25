#ifndef TTBARHFFINDER_H
#define TTBARHFFINDER_H

#include <TTree.h>
#include "KITAObjects.hpp"
#include <iostream>

#include <math.h>
#include <vector>
using namespace std;



class TTbarHFFinder
{
public:
  /// Constructor
  TTbarHFFinder(TTree* _itree, KITAJets* _mykitajets);
  
private:
  
  TTree* itree;
  KITAJets* mykitajets;
  
  Bool_t   isUnknown;
  Bool_t   isTwoBottom;         ///< At least two b quarks matched to different reconstructed jets
  Bool_t   isDoubleBottomJet;   ///< Same reconstructed jet matched to at least two b quarks
  Bool_t   isSingleBottom;      ///< Only one b quark matched to a reconstructed jet
  Bool_t   isTwoCharm;          ///< At least two c quarks matched to different reconstructed jets
  Bool_t   isDoubleCharmJet;    ///< Same reconstructed jet matched to at least two c quarks
  Bool_t   isSingleCharm;       ///< Only one c quark matched to a reconstructed jet
  Bool_t   isUnmatchedBottom;   ///< There are b quarks, but none is matched to a reconstructed jet
  Bool_t   isUnmatchedCharm;    ///< There are c quarks, but none is matched to a reconstructed jet
  Bool_t   isNoHF;               ///< There are no b or c quarks in the event
    
  struct HFCounter
  {
    unsigned nb, nc;
  };
  
      
    
public:
     
  /// Returns the classification decision
  Int_t GetDecision();
  Bool_t FindHF();  
private:
     
  /// A size of cone to match quarks to reconstructed jets
  double const matchConeSize;
  KITAGenParticles* mykitagenstatus2;
  KITAGenParticles* mykitagenstatus3;

  vector<KITAGenParticle> additionalPartons;
 
  vector<KITAGenParticle> ewkPartons;
  
  vector<HFCounter> partonCounts;
  HFCounter unmatchedPartonCount;
};


#endif
