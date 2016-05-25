#include "TH1F.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TColor.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPad.h"
#include "TAxis.h"
#include "TLine.h"
#include "TLatex.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include "cmsstyle.hpp"

using namespace std;

void cls() {
  CMSStyle* cms = new CMSStyle();
  float comb_expected(-999.);
  float comb_sig1Down(-999.);
  float comb_sig1Up(-999.);
  float comb_sig2Down(-999.);
  float comb_sig2Up(-999.);
  float comb_observed(-999.);

  float comb_with_2tag_expected(-999.);
  float comb_with_2tag_sig1Down(-999.);
  float comb_with_2tag_sig1Up(-999.);
  float comb_with_2tag_sig2Down(-999.);
  float comb_with_2tag_sig2Up(-999.);
  float comb_with_2tag_observed(-999.);

  float t3_expected(-999.);
  float t3_sig1Down(-999.);
  float t3_sig1Up(-999.);
  float t3_sig2Down(-999.);
  float t3_sig2Up(-999.);
  float t3_observed(-999.);

  float t4_expected(-999.);
  float t4_sig1Down(-999.);
  float t4_sig1Up(-999.);
  float t4_sig2Down(-999.);
  float t4_sig2Up(-999.);
  float t4_observed(-999.);

  float t2_expected(-999.);
  float t2_sig1Down(-999.);
  float t2_sig1Up(-999.);
  float t2_sig2Down(-999.);
  float t2_sig2Up(-999.);
  float t2_observed(-999.);


  #include "limits.txt"
 
  // end instert
  
  float xvals[5]     = { 1,2,3,4,5};
  float grxvals[5]     = { 0.5,1.5,2.5,3.5,4.5};
  float xvalsl[5]     = { 0.5,0.5,0.5,0.5,0.5};
  float yexpected[5] = { t4_expected,
			 t3_expected,
			 comb_expected,
			 comb_with_2tag_expected,
			 t2_expected
  };
  float yobserved[5] = { t4_observed,
			 t3_observed,
			 comb_observed,
			 comb_with_2tag_observed,
			 t2_observed
  };
  float ysig1Up[5]   = { std::abs(t4_expected-t4_sig1Up),
			 std::abs(t3_expected-t3_sig1Up),
			 std::abs(comb_expected-comb_sig1Up),
			 std::abs(comb_with_2tag_expected-comb_with_2tag_sig1Up),
			 std::abs(t2_expected-t2_sig1Up)
  };
  float ysig1Down[5] = { std::abs(t4_expected-t4_sig1Down),
			 std::abs(t3_expected-t3_sig1Down),
			 std::abs(comb_expected-comb_sig1Down),
			 std::abs(comb_with_2tag_expected-comb_with_2tag_sig1Down),
			 std::abs(t2_expected-t2_sig1Down)
  };
  float ysig2Up[5]   = { std::abs(t4_expected-t4_sig2Up),
			 std::abs(t3_expected-t3_sig2Up),
			 std::abs(comb_expected-comb_sig2Up),
			 std::abs(comb_with_2tag_expected-comb_with_2tag_sig2Up),
			 std::abs(t2_expected-t2_sig2Up)
  };
  float ysig2Down[5] = { std::abs(t4_expected-t4_sig2Down),
			 std::abs(t3_expected-t3_sig2Down),
			 std::abs(comb_expected-comb_sig2Down),
			 std::abs(comb_with_2tag_expected-comb_with_2tag_sig2Down),
			 std::abs(t2_expected-t2_sig2Down)
  };
 

  //for (int i=0;i<6;i++) 

  TH1F* expected  = new TH1F("expected","expected",5,0,5);
  TH1F* expected_1sig  = new TH1F("expected_1sig","expected_1sig",5,0,5);
  TH1F* expected_2sig  = new TH1F("expected_2sig","expected_2sig",5,0,5);
  TH1F* observed  = new TH1F("observed","observed",5,0,5);

  //  for(int i=0; i<6; i++){
  //  expected->SetBinContent(i,yexpected[i]);
  //  expected_1sig->SetBinContent(i,yexpected[i]);
  //  expected_2sig->SetBinContent(i,yexpected[i]);
  //  expected_1sig->SetBinError(i,ysig1Up[i]);
  //  expected_2sig->SetBinError(i,ysig2Up[i]);
  // }

  // options
  expected->SetLineStyle(kDashed);
  expected->SetLineWidth(3);
  expected->SetMarkerStyle(0);
  expected_1sig->SetMarkerStyle(0);
  expected_2sig->SetMarkerStyle(0);
  expected_1sig->SetFillStyle(1001);
  expected_2sig->SetFillStyle(1001);
  expected_1sig->SetFillColor(kGreen);
  expected_2sig->SetFillColor(kYellow);

  //  TH1F *h = new TH1F("h","Asymm errors histogram with labels",n,0,n);
  const char *labels[5] = {"4 tag","3 tag","3 + 4 tag", "2 + 3 + 4 tag", "2 tag"};
  for (Int_t i=0;i<5;i++) {
    expected->GetXaxis()->SetBinLabel(i+1,labels[i]);
    expected->Fill(i+0.5,yexpected[i]);
    expected->SetBinError(i+1,0.001);
    observed->Fill(i+0.5,yobserved[i]);
    observed->SetBinError(i+1,0);
  }
  //h->GetXaxis()->LabelsOption("v");
  TGraphAsymmErrors* gr1sig = new TGraphAsymmErrors(5,grxvals,yexpected,xvalsl,xvalsl,ysig1Down,ysig1Up);

  gr1sig->SetMarkerStyle(0);
  //gr1sig->SetFillColor(427);
  gr1sig->SetFillColor(kGreen);
  

  TGraphAsymmErrors* gr2sig = new TGraphAsymmErrors(5,grxvals,yexpected,xvalsl,xvalsl,ysig2Down,ysig2Up);

  gr2sig->SetMarkerStyle(0);
  //gr2sig->SetFillColor(424);
  gr2sig->SetFillColor(kYellow);
  
  expected->SetMinimum(0);
  expected->SetMaximum(expected->GetMaximum()*3.0);
  expected->GetYaxis()->SetNdivisions(406);

  expected->SetTitle("");
  expected->GetYaxis()->SetTitle("95% C.L. CL_{S} limit on #sigma/#sigma_{#it{C}_{t} = -1}");
  expected->GetYaxis()->SetTitleSize(0.05);
  expected->GetYaxis()->SetTitleOffset(1.2);
  expected->SetStats(0);
  // create canvas
  

  TCanvas* c = new TCanvas("c","c",700,600,700,600);
  c->cd();
  
  

  

  observed->SetLineWidth(2);
  observed->SetMarkerStyle(8);
  observed->SetMarkerColor(kBlack);
  observed->SetMarkerSize(1);
  

  expected->Draw("AXIS");
  gr2sig->Draw("pE2");
  gr1sig->Draw("pE2same");
  expected->Draw("PEsame");

  TLine* line0 = new TLine(0.,yobserved[0],1.,yobserved[0]);
  line0->SetLineWidth(2);
  line0->SetLineColor(kBlack);
  line0->Draw();

  TLine* line1 = new TLine(1.,yobserved[1],2.,yobserved[1]);
  line1->SetLineWidth(2);
  line1->SetLineColor(kBlack);
  line1->Draw();

  TLine* line2 = new TLine(2.,yobserved[2],3.,yobserved[2]);
  line2->SetLineWidth(2);
  line2->SetLineColor(kBlack);
  line2->Draw();

  TLine* line3 = new TLine(3.,yobserved[3],4.,yobserved[3]);
  line3->SetLineWidth(2);
  line3->SetLineColor(kBlack);
  line3->Draw();

  TLine* line4 = new TLine(4.,yobserved[4],5.,yobserved[4]);
  line4->SetLineWidth(2);
  line4->SetLineColor(kBlack);
  line4->Draw();


  observed->Draw("EP same");
  
  TLine* line = new TLine(0.,1,5.,1);
  line->SetLineWidth(2);
  line->SetLineColor(kRed);
  line->Draw();

  TLatex tex;
  tex.SetTextSize(0.05);
  tex.SetTextFont(42);
  tex.SetTextColor(kRed);
  tex.SetNDC(true);
  //  tex.DrawLatex(.133,0.165,"1");
  
  TLatex tex2;
  tex2.SetTextSize(0.04);
  tex2.SetTextFont(42);
  //tex2.SetTextColor(kPink+4);
  tex2.SetNDC(true);
  

  tex2.DrawLatex(.75,0.91,"19.7 fb^{-1} (8 TeV)");
  tex2.DrawLatex(.20,0.84,"pp #rightarrow tHq ");
  tex2.DrawLatex(.20,0.79,"t #rightarrow bl#nu, H #rightarrow b#bar{b}");
  tex2.DrawLatex(.20,0.74,"#it{C}_{t} = -1, m_{H} = 125 GeV");
  //tex2.DrawLatex(.18,0.69,"CSV Medium WP");


  TLegend* leg = new TLegend(.20,.55,.56,.71);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetLineColor(0);
  leg->SetBorderSize(0);
    //Leg->SetBorderSize(3);
  leg->AddEntry(observed,"#bf{CL_{s} observed}","lp"); 
  leg->AddEntry(expected,"#bf{CL_{s} expected}","l"); 
  leg->AddEntry(gr1sig,"#bf{#pm 1#sigma CL_{s} exp.}","F");
  leg->AddEntry(gr2sig,"#bf{#pm 2#sigma CL_{s} exp.}","F");
  leg->SetTextSize(0.03);


  leg->Draw("SAME");


  gPad->SetTopMargin(14.0);

  c->RedrawAxis();
  c->SaveAs("CLs_Limit_Asym.pdf");
  return;
}

