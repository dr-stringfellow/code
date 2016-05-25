{
//=========Macro generated from canvas: canvas/canvas
//=========  (Sun Sep 27 19:43:54 2015) by ROOT version5.34/32
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,976);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(-0.9050633,-0.1767993,1.626582,1.183196);
   canvas->SetFillColor(0);
   canvas->SetBorderMode(0);
   canvas->SetBorderSize(2);
   canvas->SetTickx(1);
   canvas->SetTicky(1);
   canvas->SetLeftMargin(0.16);
   canvas->SetRightMargin(0.05);
   canvas->SetTopMargin(0.13);
   canvas->SetBottomMargin(0.13);
   canvas->SetFrameBorderMode(0);
   canvas->SetFrameBorderMode(0);
   
   THStack *hStack = new THStack();
   hStack->SetName("hStack");
   hStack->SetTitle(";tagged(b_{t}) [tHq];Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(1.006396);
   Double_t xAxis154[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_52 = new TH1F("hStack_stack_52","",2, xAxis154);
   hStack_stack_52->SetMinimum(0);
   hStack_stack_52->SetMaximum(1.006396);
   hStack_stack_52->SetDirectory(0);
   hStack_stack_52->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_52->SetLineColor(ci);
   hStack_stack_52->SetLineWidth(2);
   hStack_stack_52->GetXaxis()->SetTitle("tagged(b_{t}) [tHq]");
   hStack_stack_52->GetXaxis()->SetNdivisions(508);
   hStack_stack_52->GetXaxis()->SetLabelFont(42);
   hStack_stack_52->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_52->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_52->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_52->GetXaxis()->SetTitleFont(42);
   hStack_stack_52->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_52->GetYaxis()->SetNdivisions(508);
   hStack_stack_52->GetYaxis()->SetLabelFont(42);
   hStack_stack_52->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_52->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_52->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_52->GetYaxis()->SetTitleFont(42);
   hStack_stack_52->GetZaxis()->SetNdivisions(508);
   hStack_stack_52->GetZaxis()->SetLabelFont(42);
   hStack_stack_52->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_52->GetZaxis()->SetTitleSize(0.05);
   hStack_stack_52->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_52);
   
   Double_t xAxis155[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn648522__103 = new TH1D("PDFSgn648522__103","",2, xAxis155);
   PDFSgn648522__103->SetBinContent(1,0.07447275);
   PDFSgn648522__103->SetBinContent(2,0.9255273);
   PDFSgn648522__103->SetBinError(1,0.0009146187);
   PDFSgn648522__103->SetBinError(2,0.003183914);
   PDFSgn648522__103->SetEntries(119361);
   PDFSgn648522__103->SetDirectory(0);
   PDFSgn648522__103->SetStats(0);
   PDFSgn648522__103->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn648522__103->SetLineColor(ci);
   PDFSgn648522__103->GetXaxis()->SetNdivisions(508);
   PDFSgn648522__103->GetXaxis()->SetLabelFont(42);
   PDFSgn648522__103->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn648522__103->GetXaxis()->SetTitleSize(0.05);
   PDFSgn648522__103->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn648522__103->GetXaxis()->SetTitleFont(42);
   PDFSgn648522__103->GetYaxis()->SetNdivisions(508);
   PDFSgn648522__103->GetYaxis()->SetLabelFont(42);
   PDFSgn648522__103->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn648522__103->GetYaxis()->SetTitleSize(0.05);
   PDFSgn648522__103->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn648522__103->GetYaxis()->SetTitleFont(42);
   PDFSgn648522__103->GetZaxis()->SetNdivisions(508);
   PDFSgn648522__103->GetZaxis()->SetLabelFont(42);
   PDFSgn648522__103->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn648522__103->GetZaxis()->SetTitleSize(0.05);
   PDFSgn648522__103->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn648522,"hist");
   Double_t xAxis156[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg648522__104 = new TH1D("PDFBkg648522__104","",2, xAxis156);
   PDFBkg648522__104->SetBinContent(1,0.04152739);
   PDFBkg648522__104->SetBinContent(2,0.9584726);
   PDFBkg648522__104->SetBinError(1,0.0007788231);
   PDFBkg648522__104->SetBinError(2,0.003710768);
   PDFBkg648522__104->SetEntries(105134);
   PDFBkg648522__104->SetDirectory(0);
   PDFBkg648522__104->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg648522__104->SetFillColor(ci);
   PDFBkg648522__104->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg648522__104->SetLineColor(ci);
   PDFBkg648522__104->GetXaxis()->SetNdivisions(508);
   PDFBkg648522__104->GetXaxis()->SetLabelFont(42);
   PDFBkg648522__104->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg648522__104->GetXaxis()->SetTitleSize(0.05);
   PDFBkg648522__104->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg648522__104->GetXaxis()->SetTitleFont(42);
   PDFBkg648522__104->GetYaxis()->SetNdivisions(508);
   PDFBkg648522__104->GetYaxis()->SetLabelFont(42);
   PDFBkg648522__104->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg648522__104->GetYaxis()->SetTitleSize(0.05);
   PDFBkg648522__104->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg648522__104->GetYaxis()->SetTitleFont(42);
   PDFBkg648522__104->GetZaxis()->SetNdivisions(508);
   PDFBkg648522__104->GetZaxis()->SetLabelFont(42);
   PDFBkg648522__104->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg648522__104->GetZaxis()->SetTitleSize(0.05);
   PDFBkg648522__104->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg648522,"hist");
   hStack->Draw("nostack");
   TLatex *   tex = new TLatex(0.2,0.89,"#scale[1.2]{#font[62]{CMS}} #font[52]{Simulation Preliminary}");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.95,0.89,"8 TeV");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->Draw();
   
   TLegend *leg = new TLegend(0.19,0.72,0.34,0.84,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("PDFSgn648522"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFBkg648522"," bkg ","f");

   ci = TColor::GetColor("#ff0000");
   entry->SetFillColor(ci);
   entry->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
      tex = new TLatex(0.96,0.13,"Underflow: #color[4]{0%} / #color[2]{0%},  Overflow: #color[4]{0%} / #color[2]{0%}");
tex->SetNDC();
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetTextAngle(90);
   tex->SetLineWidth(2);
   tex->Draw();
   canvas->Modified();
   canvas->cd();
   canvas->SetSelected(canvas);
}
