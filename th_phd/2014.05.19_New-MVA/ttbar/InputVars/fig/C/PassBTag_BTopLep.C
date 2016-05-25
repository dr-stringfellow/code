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
   hStack->SetTitle("tagged(b_{tlep});tagged(b_{tlep});Probability density");
   Double_t xAxis97[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_33 = new TH1F("hStack_stack_33","tagged(b_{tlep})",2, xAxis97);
   hStack_stack_33->SetMinimum(0);
   hStack_stack_33->SetMaximum(0.2734404);
   hStack_stack_33->SetDirectory(0);
   hStack_stack_33->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_33->SetLineColor(ci);
   hStack_stack_33->SetLineWidth(2);
   hStack_stack_33->GetXaxis()->SetTitle("tagged(b_{tlep})");
   hStack_stack_33->GetXaxis()->SetLabelFont(42);
   hStack_stack_33->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_33->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_33->GetXaxis()->SetTitleFont(42);
   hStack_stack_33->GetYaxis()->SetTitle("Probability density");
   hStack_stack_33->GetYaxis()->SetLabelFont(42);
   hStack_stack_33->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_33->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_33->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_33->GetYaxis()->SetTitleFont(42);
   hStack_stack_33->GetZaxis()->SetLabelFont(42);
   hStack_stack_33->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_33->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_33->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_33);
   
   Double_t xAxis98[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn194872__65 = new TH1D("PDFSgn194872__65","",2, xAxis98);
   PDFSgn194872__65->SetBinContent(1,0.1041629);
   PDFSgn194872__65->SetBinContent(2,0.2604194);
   PDFSgn194872__65->SetBinContent(3,0.6354177);
   PDFSgn194872__65->SetBinError(1,0.001553206);
   PDFSgn194872__65->SetBinError(2,0.002466834);
   PDFSgn194872__65->SetBinError(3,0.003864883);
   PDFSgn194872__65->SetEntries(49013);
   PDFSgn194872__65->SetDirectory(0);
   PDFSgn194872__65->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn194872__65->SetLineColor(ci);
   PDFSgn194872__65->GetXaxis()->SetLabelFont(42);
   PDFSgn194872__65->GetXaxis()->SetLabelSize(0.035);
   PDFSgn194872__65->GetXaxis()->SetTitleSize(0.035);
   PDFSgn194872__65->GetXaxis()->SetTitleFont(42);
   PDFSgn194872__65->GetYaxis()->SetLabelFont(42);
   PDFSgn194872__65->GetYaxis()->SetLabelSize(0.035);
   PDFSgn194872__65->GetYaxis()->SetTitleSize(0.035);
   PDFSgn194872__65->GetYaxis()->SetTitleFont(42);
   PDFSgn194872__65->GetZaxis()->SetLabelFont(42);
   PDFSgn194872__65->GetZaxis()->SetLabelSize(0.035);
   PDFSgn194872__65->GetZaxis()->SetTitleSize(0.035);
   PDFSgn194872__65->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn194872,"hist");
   Double_t xAxis99[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg194872__66 = new TH1D("PDFBkg194872__66","",2, xAxis99);
   PDFBkg194872__66->SetBinContent(1,0.1010373);
   PDFBkg194872__66->SetBinContent(2,0.2556695);
   PDFBkg194872__66->SetBinContent(3,0.6432933);
   PDFBkg194872__66->SetBinError(1,0.001524082);
   PDFBkg194872__66->SetBinError(2,0.00243596);
   PDFBkg194872__66->SetBinError(3,0.003895935);
   PDFBkg194872__66->SetEntries(49013);
   PDFBkg194872__66->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg194872__66->SetFillColor(ci);
   PDFBkg194872__66->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg194872__66->SetLineColor(ci);
   PDFBkg194872__66->GetXaxis()->SetLabelFont(42);
   PDFBkg194872__66->GetXaxis()->SetLabelSize(0.035);
   PDFBkg194872__66->GetXaxis()->SetTitleSize(0.035);
   PDFBkg194872__66->GetXaxis()->SetTitleFont(42);
   PDFBkg194872__66->GetYaxis()->SetLabelFont(42);
   PDFBkg194872__66->GetYaxis()->SetLabelSize(0.035);
   PDFBkg194872__66->GetYaxis()->SetTitleSize(0.035);
   PDFBkg194872__66->GetYaxis()->SetTitleFont(42);
   PDFBkg194872__66->GetZaxis()->SetLabelFont(42);
   PDFBkg194872__66->GetZaxis()->SetLabelSize(0.035);
   PDFBkg194872__66->GetZaxis()->SetTitleSize(0.035);
   PDFBkg194872__66->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg194872,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.3850204,0.9079534,0.6149796,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("tagged(b_{tlep})");
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
   entry=leg->AddEntry("PDFSgn194872"," correct comb. ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg194872"," wrong comb. ","f");

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
