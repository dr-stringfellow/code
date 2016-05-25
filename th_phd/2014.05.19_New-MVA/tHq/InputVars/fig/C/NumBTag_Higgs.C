{
//=========Macro generated from canvas: canvas/canvas
//=========  (Tue May 20 17:07:26 2014) by ROOT version5.34/09
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
   mainPad->Range(-0.875,-0.09569317,2.875,0.8612385);
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
   hStack->SetTitle("#tagged(b_{h});#tagged(b_{h});Probability density");
   Double_t xAxis64[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1F *hStack_stack_22 = new TH1F("hStack_stack_22","#tagged(b_{h})",3, xAxis64);
   hStack_stack_22->SetMinimum(0);
   hStack_stack_22->SetMaximum(0.7655453);
   hStack_stack_22->SetDirectory(0);
   hStack_stack_22->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_22->SetLineColor(ci);
   hStack_stack_22->SetLineWidth(2);
   hStack_stack_22->GetXaxis()->SetTitle("#tagged(b_{h})");
   hStack_stack_22->GetXaxis()->SetLabelFont(42);
   hStack_stack_22->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_22->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_22->GetXaxis()->SetTitleFont(42);
   hStack_stack_22->GetYaxis()->SetTitle("Probability density");
   hStack_stack_22->GetYaxis()->SetLabelFont(42);
   hStack_stack_22->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_22->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_22->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_22->GetYaxis()->SetTitleFont(42);
   hStack_stack_22->GetZaxis()->SetLabelFont(42);
   hStack_stack_22->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_22->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_22->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_22);
   
   Double_t xAxis65[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFSgn724286__43 = new TH1D("PDFSgn724286__43","",3, xAxis65);
   PDFSgn724286__43->SetBinContent(1,0.002832246);
   PDFSgn724286__43->SetBinContent(2,0.268077);
   PDFSgn724286__43->SetBinContent(3,0.7290907);
   PDFSgn724286__43->SetBinError(1,0.0002481597);
   PDFSgn724286__43->SetBinError(2,0.002321928);
   PDFSgn724286__43->SetBinError(3,0.003728913);
   PDFSgn724286__43->SetEntries(58680);
   PDFSgn724286__43->SetDirectory(0);
   PDFSgn724286__43->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn724286__43->SetLineColor(ci);
   PDFSgn724286__43->GetXaxis()->SetLabelFont(42);
   PDFSgn724286__43->GetXaxis()->SetLabelSize(0.035);
   PDFSgn724286__43->GetXaxis()->SetTitleSize(0.035);
   PDFSgn724286__43->GetXaxis()->SetTitleFont(42);
   PDFSgn724286__43->GetYaxis()->SetLabelFont(42);
   PDFSgn724286__43->GetYaxis()->SetLabelSize(0.035);
   PDFSgn724286__43->GetYaxis()->SetTitleSize(0.035);
   PDFSgn724286__43->GetYaxis()->SetTitleFont(42);
   PDFSgn724286__43->GetZaxis()->SetLabelFont(42);
   PDFSgn724286__43->GetZaxis()->SetLabelSize(0.035);
   PDFSgn724286__43->GetZaxis()->SetTitleSize(0.035);
   PDFSgn724286__43->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn724286,"hist");
   Double_t xAxis66[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFBkg724286__44 = new TH1D("PDFBkg724286__44","",3, xAxis66);
   PDFBkg724286__44->SetBinContent(1,0.07044354);
   PDFBkg724286__44->SetBinContent(2,0.4461847);
   PDFBkg724286__44->SetBinContent(3,0.4833718);
   PDFBkg724286__44->SetBinError(1,0.001180266);
   PDFBkg724286__44->SetBinError(2,0.002956183);
   PDFBkg724286__44->SetBinError(3,0.00303738);
   PDFBkg724286__44->SetEntries(58680);
   PDFBkg724286__44->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg724286__44->SetFillColor(ci);
   PDFBkg724286__44->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg724286__44->SetLineColor(ci);
   PDFBkg724286__44->GetXaxis()->SetLabelFont(42);
   PDFBkg724286__44->GetXaxis()->SetLabelSize(0.035);
   PDFBkg724286__44->GetXaxis()->SetTitleSize(0.035);
   PDFBkg724286__44->GetXaxis()->SetTitleFont(42);
   PDFBkg724286__44->GetYaxis()->SetLabelFont(42);
   PDFBkg724286__44->GetYaxis()->SetLabelSize(0.035);
   PDFBkg724286__44->GetYaxis()->SetTitleSize(0.035);
   PDFBkg724286__44->GetYaxis()->SetTitleFont(42);
   PDFBkg724286__44->GetZaxis()->SetLabelFont(42);
   PDFBkg724286__44->GetZaxis()->SetLabelSize(0.035);
   PDFBkg724286__44->GetZaxis()->SetTitleSize(0.035);
   PDFBkg724286__44->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg724286,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.389201,0.9172798,0.610799,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("#tagged(b_{h})");
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
   TLegendEntry *entry=leg->AddEntry("NULL"," tHq reco ","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFSgn724286"," correct comb. ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg724286"," wrong comb. ","f");

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
