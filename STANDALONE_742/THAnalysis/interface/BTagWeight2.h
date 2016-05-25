#ifndef BTAGWEIGHT2_H
#define BTAGWEIGHT2_H
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <TH2D.h>
#include <stdexcept>

using namespace std; 

class BTagWeight2
{
 public:
  struct JetInfo {
    JetInfo(float mceff,float datasf,int t=0.) : eff(mceff), sf(datasf) , tag(t){}
    float eff;
    float sf;
    int tag;
  };
  
  double CalcWeight(vector<vector<JetInfo> >jets);
  BTagWeight2(unsigned int nTaggers) : taggers(nTaggers) {}
    
    // virtual bool filter(vector<int> tags);
    template <class Filter> float weight(vector<vector<JetInfo> > jets);
 private:
    unsigned int taggers;
};

class BTag2TightFilter2
{ 
 public:
  static bool filter(int t)
    {
      return t == 2;
    }
};

class BTag3TightFilter2
{ 
 public:
  static bool filter(int t)
  {
    return t == 3;
}
};


class BTag4TightFilter2
{ 
 public:
  static bool filter(int t)
  {
    return t == 4;
}
};


double BTagWeight2::CalcWeight(vector<vector<JetInfo> >jets)
{
  // The weight will be constructed following this recipe [1]. It will be calculated as a product
  //of per-jet factors. These factors are of the order of 1, and for this reason it is fine to
  //simply multiply them instead of calculating a sum of logarithms, which is more stable in case
  //of small multipliers
  //[1] https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
  double weight = 1.;
  
  // Loop over the jets
  for (size_t i=0;i < jets.size(); i++)
    {
      // Skip jets outside the tracker acceptance
        
      // Precalculate b-tagging scale factor for the current jet
      double const sf = jets[i][0].sf;        
      
      // Update the weight
      if (jets[i][0].tag == 1)
	weight *= sf;
      else
        {
	  // Only in this case the b-tagging efficiency is needed. Calculate it
	  double const eff = jets[i][0].eff;
	    
	  if (eff < 1.)
	    weight *= (1. - sf * eff) / (1. - eff);
	  //^ The above formula does not work if eff == 1. It should be a very rear event and is
	  //possible if only the efficiencies were measured after an event selection that does not
	  //enclose the event selection applied at the moment, or if efficiencies from a wrong
	  //dataset are applied. Nevertheless, an untagged jet with eff == 1. is ignored. This is
	  //an ad-hoc solution motivated only in the case of sf == 1.
        }
    }
  
  return weight;
}



template <class Filter> float BTagWeight2::weight(vector<vector<JetInfo> >jets)
{
 unsigned int njets=jets.size();
 std::vector<unsigned int> comb(jets.size());
 for(size_t i=0;i < jets.size(); i++) comb[i]=0;
 
 unsigned int idx=0;
 unsigned int max=taggers+1; //force sorted tagging //1 << taggers;
 float pMC=0;
 float pData=0;
 if(jets.size()==0) return 0.;
 while(comb[jets.size()-1] < max)
   {
     // std::cout << std::endl << "New comb" << std::endl;
     // for(size_t i=0;i < jets.size(); i++) {std::cout << comb[i] << " ";}
     // std::cout << std::endl;

     int tags=0;
     
     float mc=1.;
     float data=1.;
     for(size_t j=0;j<njets;j++) // loop on jets
       {
	 //  std::cout << std::endl << "Jet" << j ;
	 
	 // if none tagged, take the 1-eff SF for the loosest:
	 float tagMc = 1.-jets[j][0].eff;
	 float tagData = 1.-jets[j][0].eff*jets[j][0].sf;
	 if(comb[j]> 0) //if at least one tagged take the SF for the tightest tagger
	   {
	     int k=comb[j]-1;
	     tagMc=jets[j][k].eff;
	     tagData=jets[j][k].eff*jets[j][k].sf;
	     
	     if(comb[j]< taggers) //if at least one tagged take the SF for the tightest tagger
	       {
		 int k1=comb[j];
		 tagMc*=1-jets[j][k1].eff/jets[j][k].eff;
		 tagData*=1-jets[j][k1].eff/jets[j][k].eff*jets[j][k1].sf/jets[j][k].sf;
	       }
	   }
	 
	 for(size_t k=0;k< taggers; k++ ) // loop on taggers
	   {
	     bool tagged = (comb[j] > k) ; ///((comb[j] >> k) & 0x1) == 1;
	     if(tagged) tags++;
	   }
	 
	 mc*=tagMc;       
	 data*=tagData;       
	 
       }
     if(Filter::filter(tags))
       {
	 //  std::cout << mc << " " << data << " " << data/mc << " " << pData/pMC << endl;
	 pMC+=mc;
	 pData+=data;
	 // std::cout << std::endl<< "mc, data,ratioThis,ratioTot " <<  mc << " " << data << " " << data/mc << " " << pData/pMC << endl;
       }
     while (comb[idx] == max -1  && idx+1 < jets.size()) idx++; // find first jets for which we did not already test all configs 
     // next combination
     comb[idx]++;  // test a new config for that jet
     for(size_t i=0;i<idx;i++) { comb[i]=0;  } // reset the tested configs for all previous jets
     idx=0;
   }
 if(pMC==0) return 0; 
 return pData/pMC;
}



class BTagSampleEfficiency2{
 public:
  BTagSampleEfficiency2(){}
  BTagSampleEfficiency2(const char * filename, string sample) 
    {
      _filename = filename;
      _sample =sample;
      cout << "Read in histo for sample " << sample << " ..." << endl;
      BTagEff = new TFile(filename);
      //"/storage/a/singletophiggs/Moriond/addhistos/BTagEff_2012Bravo_v1.0.root"
      histBTagEff_l = (TH2D*) BTagEff->Get(("in4_jPt30/"+sample+"_uds_T").c_str());
      if(histBTagEff_l==0) throw std::exception();
      // cout << "in4_jPt30/"+sample+"_uds_T" << endl;
      histBTagEff_g = (TH2D*) BTagEff->Get(("in4_jPt30/"+sample+"_g_T").c_str());
      if(histBTagEff_g==0) throw std::exception();
      //cout << "in4_jPt30/"+sample+"_g_T" << endl;
      histBTagEff_c = (TH2D*) BTagEff->Get(("in4_jPt30/"+sample+"_c_T").c_str());
      if(histBTagEff_c==0) throw std::exception();
      // cout << "in4_jPt30/"+sample+"_c_T" << endl;
      histBTagEff_b = (TH2D*) BTagEff->Get(("in4_jPt30/"+sample+"_b_T").c_str());
      if(histBTagEff_b==0) throw std::exception();
      //cout << "in4_jPt30/"+sample+"_b_T" << endl;
    }
  
  vector<BTagWeight2::JetInfo> jetInfo(Short_t jtgenflv, float jtpt, float jteta, float SF_CSVT, float jtcsvt)
    {
      //cout << "input " <<  jtgenflv << " " << " " << jtpt << " " << jteta << " " << SF_CSVT << endl;
      std::vector<BTagWeight2::JetInfo> jInfo;
      double m=0.;
      int tag = 0;

      if(abs(jtgenflv)==5) m = histBTagEff_b->GetBinContent(histBTagEff_b->FindFixBin(jtpt, fabs(jteta)));
      else if(abs(jtgenflv)==4) m= histBTagEff_c->GetBinContent(histBTagEff_c->FindFixBin(jtpt, fabs(jteta)));
      else if(abs(jtgenflv)==21) m= histBTagEff_g->GetBinContent(histBTagEff_g->FindFixBin(jtpt, fabs(jteta)));
      else
	m = histBTagEff_l->GetBinContent(histBTagEff_l->FindFixBin(jtpt, fabs(jteta)));

      if(jtcsvt>0.898)
	tag = 1;
     
      jInfo.push_back(BTagWeight2::JetInfo(m,SF_CSVT,tag)); 
      
      return jInfo;
    } 

  ~BTagSampleEfficiency2(){
    delete histBTagEff_l;
    delete histBTagEff_g;
    delete histBTagEff_c;
    delete histBTagEff_b;   
    delete BTagEff;
  }
  
  
  const char* _filename;
  string _sample;
  
 private:
  TFile* BTagEff;
  TH2D* histBTagEff_l;
  TH2D* histBTagEff_g;
  TH2D* histBTagEff_c;
  TH2D* histBTagEff_b;
};

#endif
