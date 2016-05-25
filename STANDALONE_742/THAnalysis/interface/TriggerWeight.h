#ifndef TRIGGERWEIGHT_H
#define TRIGGERWEIGHT_H


#include <TH1F.h>
#include <TF1.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

class TriggerWeight
{
public:  
  TriggerWeight()
  {
 
    tscaleEleID2012Awp80     = openFile("/portal/ekpcms5/home/sfink/analysis/STANDALONE/THAnalysis/triggerRootFiles/EleRecoId.ScaleFactor.wp80.2012ABC.root"); 
    tscaleSingleEle2012Awp80 = openFile("/portal/ekpcms5/home/sfink/analysis/STANDALONE/THAnalysis/triggerRootFiles/SingleEle.TrigEff.wp80.2012ABC.root");
    tscaleIDmu               = openFile("/portal/ekpcms5/home/sfink/analysis/STANDALONE/THAnalysis/triggerRootFiles/MuRecoId.ScaleFactor.2012ABC.root");  
    tscaleSingleMuon2012A    = openFile("/portal/ekpcms5/home/sfink/analysis/STANDALONE/THAnalysis/triggerRootFiles/SingleMu24OR40.TrigEff.2012ABC.root");
  
    //   if(tscaleHLTmu == 0 || tscaleIDmu == 0) 
    //    {
    //	std::cout << "ERROR: cannot load Muon Trigger efficiencies" << std::endl;
    //    } 


  }
 
  static TTree* openFile(TString name)
  {
    // return 0;
    TFile* hltMuFile = new TFile(name,"READ");

    if (hltMuFile->IsZombie()) std::cout << "Error Opening " << name << " " << std::endl;

    if(hltMuFile)   return (TTree*) hltMuFile->Get("tree");
    else return 0;
   }  
  
  static  std::pair<float,float> efficiencyFromPtEta(float pt1, float eta1, TTree *t)
  {
    //     std::cout << "here " << t << " pt1 "  << pt1 << " eta1 " << eta1 <<  std::endl;
    float s1 = 1.,err=1.;
    std::pair<float,float> r(s1,err);
    if(!t) return r;
    float ptMin,ptMax,etaMin,etaMax,scale,error;
    int count = 0;
    t->SetBranchAddress("ptMin",&ptMin);
    t->SetBranchAddress("ptMax",&ptMax);
    t->SetBranchAddress("etaMin",&etaMin);
    t->SetBranchAddress("etaMax",&etaMax);
    t->SetBranchAddress("scale",&scale);
    t->SetBranchAddress("error",&error);
    float lastPtBin = 200;
      //  for(int jentry = 0; jentry < t->GetEntries(); jentry++)
//	  {
//	  t->GetEntry(jentry);
	//  if(ptMax >= lastPtBin) lastPtBin =ptMax;
	//  }
    for(int jentry = 0; jentry < t->GetEntries(); jentry++)
      {
        t->GetEntry(jentry);
        if(ptMax==lastPtBin) ptMax=1e99;
        if((pt1 > ptMin) && (pt1 < ptMax) && (eta1 > etaMin) && (eta1 < etaMax))
          {
            s1 = scale;
            err=error;
            count++;
          }
      }
    
    if(count == 0)
      {
        return r;
      }
    
    r.first=s1;
    r.second = err;
    return (r);
  } 
  
  
  float scaleMuID(float pt1, float eta1){    return efficiencyFromPtEta(pt1,eta1,tscaleIDmu).first;  }
  double eleId2012Awp80( float pt, float eta){    return efficiencyFromPtEta(pt,eta,tscaleEleID2012Awp80).first;}
  float singleEle2012Awp80( float pt, float eta){    return efficiencyFromPtEta(pt,eta,tscaleSingleEle2012Awp80).first;}
  float singleMuon2012A( float pt, float eta){    return efficiencyFromPtEta(pt,eta,tscaleSingleMuon2012A).first;}
  
private:

  TTree * tscaleSingleEle2012Awp80;
  TTree * tscaleSingleMuon2012A;
  TTree * tscaleEleID2012Awp80;
  TTree * tscaleIDmu;
};

#endif
