{
//=========Macro generated from canvas: canvas/canvas
//=========  (Tue May 20 17:25:42 2014) by ROOT version5.34/09
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,776);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(0,0,1,1);
   canvas->SetFillColor(0);
   canvas->SetBorderMode(0);
   canvas->SetBorderSize(2);
   canvas->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: mainPad
   TPad *mainPad = new TPad("mainPad", "",0,0,0.9,1);
   mainPad->Draw();
   mainPad->cd();
   mainPad->Range(-0.75,-0.1257995,1.75,1.132196);
   mainPad->SetFillColor(0);
   mainPad->SetBorderMode(0);
   mainPad->SetBorderSize(2);
   mainPad->SetGridx();
   mainPad->SetGridy();
   mainPad->SetTickx(1);
   mainPad->SetTicky(1);
   mainPad->SetFrameBorderMode(0);
   mainPad->SetFrameBorderMode(0);
   
   THStack *hStack = new THStack();
   hStack->SetName("hStack");
   hStack->SetTitle("tagged(b_{t}) [tHq];tagged(b_{t}) [tHq];Probability density");
   Double_t xAxis154[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_52 = new TH1F("hStack_stack_52","tagged(b_{t}) [tHq]",2, xAxis154);
   hStack_stack_52->SetMinimum(0);
   hStack_stack_52->SetMaximum(1.006396);
   hStack_stack_52->SetDirectory(0);
   hStack_stack_52->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_52->SetLineColor(ci);
   hStack_stack_52->SetLineWidth(2);
   hStack_stack_52->GetXaxis()->SetTitle("tagged(b_{t}) [tHq]");
   hStack_stack_52->GetXaxis()->SetLabelFont(42);
   hStack_stack_52->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_52->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_52->GetXaxis()->SetTitleFont(42);
   hStack_stack_52->GetYaxis()->SetTitle("Probability density");
   hStack_stack_52->GetYaxis()->SetLabelFont(42);
   hStack_stack_52->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_52->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_52->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_52->GetYaxis()->SetTitleFont(42);
   hStack_stack_52->GetZaxis()->SetLabelFont(42);
   hStack_stack_52->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_52->GetZaxis()->SetTitleSize(0.035);
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
   PDFSgn648522__103->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn648522__103->SetLineColor(ci);
   PDFSgn648522__103->GetXaxis()->SetLabelFont(42);
   PDFSgn648522__103->GetXaxis()->SetLabelSize(0.035);
   PDFSgn648522__103->GetXaxis()->SetTitleSize(0.035);
   PDFSgn648522__103->GetXaxis()->SetTitleFont(42);
   PDFSgn648522__103->GetYaxis()->SetLabelFont(42);
   PDFSgn648522__103->GetYaxis()->SetLabelSize(0.035);
   PDFSgn648522__103->GetYaxis()->SetTitleSize(0.035);
   PDFSgn648522__103->GetYaxis()->SetTitleFont(42);
   PDFSgn648522__103->GetZaxis()->SetLabelFont(42);
   PDFSgn648522__103->GetZaxis()->SetLabelSize(0.035);
   PDFSgn648522__103->GetZaxis()->SetTitleSize(0.035);
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

   ci = TColor::GetColor("#ff0000");
   PDFBkg648522__104->SetFillColor(ci);
   PDFBkg648522__104->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg648522__104->SetLineColor(ci);
   PDFBkg648522__104->GetXaxis()->SetLabelFont(42);
   PDFBkg648522__104->GetXaxis()->SetLabelSize(0.035);
   PDFBkg648522__104->GetXaxis()->SetTitleSize(0.035);
   PDFBkg648522__104->GetXaxis()->SetTitleFont(42);
   PDFBkg648522__104->GetYaxis()->SetLabelFont(42);
   PDFBkg648522__104->GetYaxis()->SetLabelSize(0.035);
   PDFBkg648522__104->GetYaxis()->SetTitleSize(0.035);
   PDFBkg648522__104->GetYaxis()->SetTitleFont(42);
   PDFBkg648522__104->GetZaxis()->SetLabelFont(42);
   PDFBkg648522__104->GetZaxis()->SetLabelSize(0.035);
   PDFBkg648522__104->GetZaxis()->SetTitleSize(0.035);
   PDFBkg648522__104->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg648522,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.3548272,0.9172798,0.6451728,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("tagged(b_{t}) [tHq]");
   pt->Draw();
   mainPad->Modified();
   canvas->cd();
   
   TLegend *leg = new TLegend(0.83,0.7,0.98,0.85,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("NULL"," sgn vs bkg ","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFSgn648522"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
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
   leg->Draw();
   canvas->Modified();
   canvas->cd();
   canvas->SetSelected(canvas);
}
