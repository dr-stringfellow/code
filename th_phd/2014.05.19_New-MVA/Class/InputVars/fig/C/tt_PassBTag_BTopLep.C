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
   mainPad->Range(-0.75,-0.13125,1.75,1.18125);
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
   hStack->SetTitle("tagged(b_{tlep}) [t#bar{t}];tagged(b_{tlep}) [t#bar{t}];Probability density");
   Double_t xAxis160[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_54 = new TH1F("hStack_stack_54","tagged(b_{tlep}) [t#bar{t}]",2, xAxis160);
   hStack_stack_54->SetMinimum(0);
   hStack_stack_54->SetMaximum(1.05);
   hStack_stack_54->SetDirectory(0);
   hStack_stack_54->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_54->SetLineColor(ci);
   hStack_stack_54->SetLineWidth(2);
   hStack_stack_54->GetXaxis()->SetTitle("tagged(b_{tlep}) [t#bar{t}]");
   hStack_stack_54->GetXaxis()->SetLabelFont(42);
   hStack_stack_54->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_54->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_54->GetXaxis()->SetTitleFont(42);
   hStack_stack_54->GetYaxis()->SetTitle("Probability density");
   hStack_stack_54->GetYaxis()->SetLabelFont(42);
   hStack_stack_54->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_54->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_54->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_54->GetYaxis()->SetTitleFont(42);
   hStack_stack_54->GetZaxis()->SetLabelFont(42);
   hStack_stack_54->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_54->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_54->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_54);
   
   Double_t xAxis161[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn586708__107 = new TH1D("PDFSgn586708__107","",2, xAxis161);
   PDFSgn586708__107->SetBinContent(2,1);
   PDFSgn586708__107->SetBinError(2,0.003312678);
   PDFSgn586708__107->SetEntries(119361);
   PDFSgn586708__107->SetDirectory(0);
   PDFSgn586708__107->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn586708__107->SetLineColor(ci);
   PDFSgn586708__107->GetXaxis()->SetLabelFont(42);
   PDFSgn586708__107->GetXaxis()->SetLabelSize(0.035);
   PDFSgn586708__107->GetXaxis()->SetTitleSize(0.035);
   PDFSgn586708__107->GetXaxis()->SetTitleFont(42);
   PDFSgn586708__107->GetYaxis()->SetLabelFont(42);
   PDFSgn586708__107->GetYaxis()->SetLabelSize(0.035);
   PDFSgn586708__107->GetYaxis()->SetTitleSize(0.035);
   PDFSgn586708__107->GetYaxis()->SetTitleFont(42);
   PDFSgn586708__107->GetZaxis()->SetLabelFont(42);
   PDFSgn586708__107->GetZaxis()->SetLabelSize(0.035);
   PDFSgn586708__107->GetZaxis()->SetTitleSize(0.035);
   PDFSgn586708__107->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn586708,"hist");
   Double_t xAxis162[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg586708__108 = new TH1D("PDFBkg586708__108","",2, xAxis162);
   PDFBkg586708__108->SetBinContent(2,1);
   PDFBkg586708__108->SetBinError(2,0.003791618);
   PDFBkg586708__108->SetEntries(105134);
   PDFBkg586708__108->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg586708__108->SetFillColor(ci);
   PDFBkg586708__108->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg586708__108->SetLineColor(ci);
   PDFBkg586708__108->GetXaxis()->SetLabelFont(42);
   PDFBkg586708__108->GetXaxis()->SetLabelSize(0.035);
   PDFBkg586708__108->GetXaxis()->SetTitleSize(0.035);
   PDFBkg586708__108->GetXaxis()->SetTitleFont(42);
   PDFBkg586708__108->GetYaxis()->SetLabelFont(42);
   PDFBkg586708__108->GetYaxis()->SetLabelSize(0.035);
   PDFBkg586708__108->GetYaxis()->SetTitleSize(0.035);
   PDFBkg586708__108->GetYaxis()->SetTitleFont(42);
   PDFBkg586708__108->GetZaxis()->SetLabelFont(42);
   PDFBkg586708__108->GetZaxis()->SetLabelSize(0.035);
   PDFBkg586708__108->GetZaxis()->SetTitleSize(0.035);
   PDFBkg586708__108->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg586708,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.3562207,0.8908549,0.6437793,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("tagged(b_{tlep}) [t#bar{t}]");
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
   entry=leg->AddEntry("PDFSgn586708"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg586708"," bkg ","f");

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
