#include "../interface/CommandLine.h"
#include "../interface/THStatus.h"
#include "../interface/RootStyle.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TLorentzVector.h>
#include <TH1F.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <Math/VectorUtil.h>
//#include <TROOT.h>
#include <TLegend.h>


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cassert>

using namespace std;

int main(int argc,char**argv) {
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;
  
  int   file  = cl.getValue<int>    ("file",               0);
  string region = cl.getValue<string>   ("region",      "3tt_incl");
  string filename;

  if (file == 0){
    filename = "/storage/a/singletophiggs/Moriond/winter14-jan28/mc/ana-ichep/"+region+"/TH-madgraph-Summer12.root";
  }
  else if(file==1){
    filename = "/storage/a/singletophiggs/Moriond/winter14-jan28/mc/ana-ichep/3tt_incl/TTbar-madgraph-Summer12.root";
  }
  TFile* ifile = new TFile(filename.c_str(),"READ");   
  TDirectory* idir = (TDirectory*) ifile->Get("utm");
  TTree* tree = (TTree*) idir->Get("t"); 

  //  Short_t i_jtgenflv[100]={-99};        tree->SetBranchAddress("jtgenflv",           i_jtgenflv);
  Float_t i_Hdaupt[3]={-99};        tree->SetBranchAddress("Hdaupt",                   i_Hdaupt);
  Float_t i_Hdaueta[3]={-99};        tree->SetBranchAddress("Hdaueta",           i_Hdaueta);
  Float_t i_Hdauphi[3]={-99};        tree->SetBranchAddress("Hdauphi",           i_Hdauphi);
  Float_t i_Hdaum[3]={-99};        tree->SetBranchAddress("Hdaum",           i_Hdaum);

  Float_t i_btoppt={-99};        tree->SetBranchAddress("btoppt",                   &i_btoppt);
  Float_t i_btopeta={-99};        tree->SetBranchAddress("btopeta",           &i_btopeta);
  Float_t i_btopphi={-99};        tree->SetBranchAddress("btopphi",           &i_btopphi);
  Float_t i_btopm={-99};        tree->SetBranchAddress("btopm",           &i_btopm);

  Float_t i_jtpt[100];        tree->SetBranchAddress("jtpt",   i_jtpt);
  Float_t i_jteta[100];       tree->SetBranchAddress("jteta",  i_jteta);
  Float_t i_jtphi[100];       tree->SetBranchAddress("jtphi",  i_jtphi);
  Float_t i_jte[100];         tree->SetBranchAddress("jte",    i_jte);

  Int_t njt15=0;          tree->SetBranchAddress("njt15",           &njt15);

  unsigned nevt = static_cast<unsigned>(tree->GetEntries());
  THStatus* status = new THStatus(nevt);

  TH2F* h2hdau0 = new TH2F("h2hdau0","H Daughter0 pt vs eta", 40,0,550,30,-4,4);
  TH2F* h2hdau1 = new TH2F("h2hdau1","H Daughter1 pt vs eta", 40,0,550,30,-4,4);
  TH2F* h2btop = new TH2F("h2btop","B from top pt vs eta", 40,0,550,30,-4,4);
  TH2F* h2hdau = new TH2F("h2hdau","H Daughter pt vs eta", 40,0,550,30,-4,4);



  for (unsigned ievt=0;ievt<nevt;ievt++) {
    tree->GetEntry(ievt);
    status->print_status(ievt);


    TLorentzVector hdau0,hdau1,btop;
    hdau0.SetPtEtaPhiM(i_Hdaupt[0],i_Hdaueta[0],i_Hdauphi[0],i_Hdaum[0]);
    hdau1.SetPtEtaPhiM(i_Hdaupt[1],i_Hdaueta[1],i_Hdauphi[1],i_Hdaum[1]);
    btop.SetPtEtaPhiM(i_btoppt,i_btopeta,i_btopphi,i_btopm);
    
    

    float bestdRhdau0 = 999;
    float bestdRhdau1 = 999;
    float bestdRbtop = 999;
    
    int bestdRhdau0_idx = -99;
    int bestdRhdau1_idx = -99;
    int bestdRbtop_idx = -99;



    for (int ijt=0;ijt<njt15;ijt++) {
      //  if(abs(i_jtgenflv[ijt])!=5) continue;
      TLorentzVector jt;
      jt.SetPtEtaPhiE(i_jtpt[ijt],i_jteta[ijt],i_jtphi[ijt],i_jte[ijt]);
      double const dRhdau0 = ROOT::Math::VectorUtil::DeltaR(jt,hdau0);
      double const dRhdau1 = ROOT::Math::VectorUtil::DeltaR(jt,hdau1);
      double const dRbtop = ROOT::Math::VectorUtil::DeltaR(jt,btop);

      if(dRhdau0<bestdRhdau0){
	bestdRhdau0=dRhdau0;
	bestdRhdau0_idx=ijt;
      }

      if(dRhdau1<bestdRhdau1){
	bestdRhdau1=dRhdau1;
	bestdRhdau1_idx=ijt;
      }

      if(dRbtop<bestdRbtop){
	bestdRbtop=dRbtop;
	bestdRbtop_idx=ijt;
      }



    }

    if(bestdRhdau1_idx!=-99 &&bestdRhdau0_idx!=-99){
      if(i_jtpt[bestdRhdau0_idx]>i_jtpt[bestdRhdau1_idx]){
	if(bestdRhdau0<0.3) h2hdau0->Fill(i_jtpt[bestdRhdau0_idx],i_jteta[bestdRhdau0_idx]);
	if(bestdRhdau1<0.3) h2hdau1->Fill(i_jtpt[bestdRhdau1_idx],i_jteta[bestdRhdau1_idx]);
      }

      if(i_jtpt[bestdRhdau0_idx] < i_jtpt[bestdRhdau1_idx]){
	if(bestdRhdau0<0.3) h2hdau1->Fill(i_jtpt[bestdRhdau0_idx],i_jteta[bestdRhdau0_idx]);
	if(bestdRhdau1<0.3) h2hdau0->Fill(i_jtpt[bestdRhdau1_idx],i_jteta[bestdRhdau1_idx]);
      }

      if(bestdRhdau1<0.3 && bestdRhdau0<0.3) {
	  h2hdau->Fill(i_jtpt[bestdRhdau0_idx],i_jteta[bestdRhdau0_idx]);
	  h2hdau->Fill(i_jtpt[bestdRhdau1_idx],i_jteta[bestdRhdau1_idx]);
      }
      if(bestdRbtop<0.3)h2btop->Fill(i_jtpt[bestdRbtop_idx],i_jteta[bestdRbtop_idx]);   
    }
  }

  h2hdau0->SetTitle("#it{#it{p}_{T} vs #eta} - matched jet to 1. b quark from Higgs");
  h2hdau1->SetTitle("#it{#it{p}_{T} vs #eta} - matched jet to 2. b quark from Higgs");
  h2btop->SetTitle("#it{#it{p}_{T} vs #eta} - matched jet to b quark from top quark");
  h2hdau->SetTitle("#it{#it{p}_{T} vs #eta} - matched jets to b quarks");
        
  h2hdau0->SetXTitle("#it{p}_{T} (jet) [GeV]");
  h2hdau0->SetYTitle("#eta (jet)");
  h2hdau0->GetYaxis()->SetTitleOffset(1.2);
      
  h2hdau1->SetXTitle("#it{p}_{T} (jet) [GeV]");
  h2hdau1->SetYTitle("#eta (jet)");
  h2hdau1->GetYaxis()->SetTitleOffset(1.2);
      
  h2btop->SetXTitle("#it{p}_{T} (jet) [GeV]");
  h2btop->SetYTitle("#eta (jet)");
  h2btop->GetYaxis()->SetTitleOffset(1.2);

  h2hdau->SetXTitle("#it{p}_{T} (jet) [GeV]");
  h2hdau->SetYTitle("#eta (jet)");
  h2hdau->GetYaxis()->SetTitleOffset(1.2);


  TCanvas* c1 = new TCanvas("c1","c1",850,600,850,600);
  TCanvas* c2 = new TCanvas("c2","c2",850,600,850,600);
  TCanvas* c3 = new TCanvas("c3","c3",850,600,850,600);
  TCanvas* c4 = new TCanvas("c4","c4",850,600,850,600);
  TCanvas* c5 = new TCanvas("c5","c5",850,600,850,600);
  TCanvas* c6 = new TCanvas("c6","c6",850,600,850,600);
  TCanvas* c7 = new TCanvas("c7","c7",850,600,850,600);
  TCanvas* c8 = new TCanvas("c8","c8",850,600,850,600);
 
  c1->cd();

  //gStyle->SetOptStat(0);
  h2hdau0->SetStats(0);

  h2hdau0->Draw("colz");

  c2->cd();

  h2hdau1->SetStats(0);
  h2hdau1->Draw("colz");

  c3->cd();
  h2btop->SetStats(0);
  h2btop->Draw("colz");

  c4->cd();
  h2hdau->SetStats(0);
  h2hdau->Draw("colz");
  
  c5->cd();
  h2hdau->SetStats(0);
  h2hdau->SetMarkerColor(632);  
  h2hdau->SetLineColor(632);
  h2hdau->Draw("");
  h2btop->SetStats(0);
  h2btop->SetMarkerColor(600);
  h2btop->SetLineColor(600);
  h2btop->Draw("same");

   



/*
  histo->SetXTitle("#it{#it{p}_{T} (b jet) [GeV]}");
  histo->SetYTitle("#it{#eta (b jet)}");
  histo->GetYaxis()->SetTitleOffset(1.2);  

  gPad->SetRightMargin(.175);
  gPad->SetLeftMargin(.15);

  TLatex tex;
  tex.SetTextSize(0.04);
  tex.SetTextFont(42);
  tex.SetNDC(true);
  */
  
  // tex.DrawLatex(0.400,0.96,"#sqrt{#it{s}} = 8 TeV");
  //tex.DrawLatex(0.165,0.96,"pp #rightarrow tHq production");
  //tex.DrawLatex(0.5300,0.96,"aMC@NLO + Pythia, 4FS");
  
  

  //c->Update();
  string fileflag;
  if(file==0){ 
    fileflag="signal";
  }
  else if (file==1){
    fileflag="ttbar";
  }


  TLegend* leg1 = new TLegend(0.60,0.69,.88,.85);
  leg1->SetBorderSize(1);
  leg1->SetFillColor(kWhite);
  leg1->SetFillStyle(4000);//transparent
  leg1->SetTextFont(42);
  leg1->AddEntry(h2hdau, "b jets from Higgs", "L");
  leg1->AddEntry(h2btop, "b jet from Top","L");
  leg1->SetTextSize(0.04);
  leg1->SetFillColor(10);
  leg1->SetLineColor(10);
  leg1->Draw();





  string filename1 = "truth_PTvsETA_Hdau0_"+fileflag+"_"+region+".pdf";
  string filename2 = "truth_PTvsETA_Hdau1_"+fileflag+"_"+region+".pdf";
  string filename3 = "truth_PTvsETA_Btop_"+fileflag+"_"+region+".pdf";
  string filename4 = "truth_PTvsETA_Hdau_"+fileflag+"_"+region+".pdf";
  string filename5 = "truth_PTvsETA_Hdau_Btop_scatter_"+region+".pdf";
  c1->SaveAs(filename1.c_str());  
  c2->SaveAs(filename2.c_str());
  c3->SaveAs(filename3.c_str());
  c4->SaveAs(filename4.c_str());
  c5->SaveAs(filename5.c_str());
  ifile->Close();
  delete ifile;
  return 0;
}
