{
//=========Macro generated from canvas: canvas/canvas
//=========  (Mon May 19 14:43:01 2014) by ROOT version5.34/09
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
   mainPad->Range(-0.75,-0.03418005,1.75,0.3076205);
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
   hStack->SetTitle("tagged(b_{thad});tagged(b_{thad});Probability density");
   Double_t xAxis100[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_34 = new TH1F("hStack_stack_34","tagged(b_{thad})",2, xAxis100);
   hStack_stack_34->SetMinimum(0);
   hStack_stack_34->SetMaximum(0.2734404);
   hStack_stack_34->SetDirectory(0);
   hStack_stack_34->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_34->SetLineColor(ci);
   hStack_stack_34->SetLineWidth(2);
   hStack_stack_34->GetXaxis()->SetTitle("tagged(b_{thad})");
   hStack_stack_34->GetXaxis()->SetLabelFont(42);
   hStack_stack_34->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_34->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_34->GetXaxis()->SetTitleFont(42);
   hStack_stack_34->GetYaxis()->SetTitle("Probability density");
   hStack_stack_34->GetYaxis()->SetLabelFont(42);
   hStack_stack_34->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_34->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_34->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_34->GetYaxis()->SetTitleFont(42);
   hStack_stack_34->GetZaxis()->SetLabelFont(42);
   hStack_stack_34->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_34->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_34->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_34);
   
   Double_t xAxis101[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn532428__67 = new TH1D("PDFSgn532428__67","",2, xAxis101);
   PDFSgn532428__67->SetBinContent(1,0.1041629);
   PDFSgn532428__67->SetBinContent(2,0.2604194);
   PDFSgn532428__67->SetBinContent(3,0.6354177);
   PDFSgn532428__67->SetBinError(1,0.001553206);
   PDFSgn532428__67->SetBinError(2,0.002466834);
   PDFSgn532428__67->SetBinError(3,0.003864883);
   PDFSgn532428__67->SetEntries(49013);
   PDFSgn532428__67->SetDirectory(0);
   PDFSgn532428__67->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn532428__67->SetLineColor(ci);
   PDFSgn532428__67->GetXaxis()->SetLabelFont(42);
   PDFSgn532428__67->GetXaxis()->SetLabelSize(0.035);
   PDFSgn532428__67->GetXaxis()->SetTitleSize(0.035);
   PDFSgn532428__67->GetXaxis()->SetTitleFont(42);
   PDFSgn532428__67->GetYaxis()->SetLabelFont(42);
   PDFSgn532428__67->GetYaxis()->SetLabelSize(0.035);
   PDFSgn532428__67->GetYaxis()->SetTitleSize(0.035);
   PDFSgn532428__67->GetYaxis()->SetTitleFont(42);
   PDFSgn532428__67->GetZaxis()->SetLabelFont(42);
   PDFSgn532428__67->GetZaxis()->SetLabelSize(0.035);
   PDFSgn532428__67->GetZaxis()->SetTitleSize(0.035);
   PDFSgn532428__67->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn532428,"hist");
   Double_t xAxis102[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg532428__68 = new TH1D("PDFBkg532428__68","",2, xAxis102);
   PDFBkg532428__68->SetBinContent(1,0.1010373);
   PDFBkg532428__68->SetBinContent(2,0.2556695);
   PDFBkg532428__68->SetBinContent(3,0.6432933);
   PDFBkg532428__68->SetBinError(1,0.001524082);
   PDFBkg532428__68->SetBinError(2,0.00243596);
   PDFBkg532428__68->SetBinError(3,0.003895935);
   PDFBkg532428__68->SetEntries(49013);
   PDFBkg532428__68->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg532428__68->SetFillColor(ci);
   PDFBkg532428__68->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg532428__68->SetLineColor(ci);
   PDFBkg532428__68->GetXaxis()->SetLabelFont(42);
   PDFBkg532428__68->GetXaxis()->SetLabelSize(0.035);
   PDFBkg532428__68->GetXaxis()->SetTitleSize(0.035);
   PDFBkg532428__68->GetXaxis()->SetTitleFont(42);
   PDFBkg532428__68->GetYaxis()->SetLabelFont(42);
   PDFBkg532428__68->GetYaxis()->SetLabelSize(0.035);
   PDFBkg532428__68->GetYaxis()->SetTitleSize(0.035);
   PDFBkg532428__68->GetYaxis()->SetTitleFont(42);
   PDFBkg532428__68->GetZaxis()->SetLabelFont(42);
   PDFBkg532428__68->GetZaxis()->SetLabelSize(0.035);
   PDFBkg532428__68->GetZaxis()->SetTitleSize(0.035);
   PDFBkg532428__68->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg532428,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.3813043,0.9157254,0.6186957,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("tagged(b_{thad})");
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
   TLegendEntry *entry=leg->AddEntry("NULL"," t#bar{t} reco ","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFSgn532428"," correct comb. ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg532428"," wrong comb. ","f");

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
