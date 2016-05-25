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
   mainPad->Range(-1.875,-0.08927559,1.875,0.8034803);
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
   hStack->SetTitle("Q(l);Q(l);Probability density");
   Double_t xAxis151[4] = {-1.5, -0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_51 = new TH1F("hStack_stack_51","Q(l)",3, xAxis151);
   hStack_stack_51->SetMinimum(0);
   hStack_stack_51->SetMaximum(0.7142047);
   hStack_stack_51->SetDirectory(0);
   hStack_stack_51->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_51->SetLineColor(ci);
   hStack_stack_51->SetLineWidth(2);
   hStack_stack_51->GetXaxis()->SetTitle("Q(l)");
   hStack_stack_51->GetXaxis()->SetLabelFont(42);
   hStack_stack_51->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_51->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_51->GetXaxis()->SetTitleFont(42);
   hStack_stack_51->GetYaxis()->SetTitle("Probability density");
   hStack_stack_51->GetYaxis()->SetLabelFont(42);
   hStack_stack_51->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_51->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_51->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_51->GetYaxis()->SetTitleFont(42);
   hStack_stack_51->GetZaxis()->SetLabelFont(42);
   hStack_stack_51->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_51->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_51->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_51);
   
   Double_t xAxis152[4] = {-1.5, -0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn248626__101 = new TH1D("PDFSgn248626__101","",3, xAxis152);
   PDFSgn248626__101->SetBinContent(1,0.3198051);
   PDFSgn248626__101->SetBinContent(3,0.6801949);
   PDFSgn248626__101->SetBinError(1,0.001872355);
   PDFSgn248626__101->SetBinError(3,0.002732786);
   PDFSgn248626__101->SetEntries(119361);
   PDFSgn248626__101->SetDirectory(0);
   PDFSgn248626__101->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn248626__101->SetLineColor(ci);
   PDFSgn248626__101->GetXaxis()->SetLabelFont(42);
   PDFSgn248626__101->GetXaxis()->SetLabelSize(0.035);
   PDFSgn248626__101->GetXaxis()->SetTitleSize(0.035);
   PDFSgn248626__101->GetXaxis()->SetTitleFont(42);
   PDFSgn248626__101->GetYaxis()->SetLabelFont(42);
   PDFSgn248626__101->GetYaxis()->SetLabelSize(0.035);
   PDFSgn248626__101->GetYaxis()->SetTitleSize(0.035);
   PDFSgn248626__101->GetYaxis()->SetTitleFont(42);
   PDFSgn248626__101->GetZaxis()->SetLabelFont(42);
   PDFSgn248626__101->GetZaxis()->SetLabelSize(0.035);
   PDFSgn248626__101->GetZaxis()->SetTitleSize(0.035);
   PDFSgn248626__101->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn248626,"hist");
   Double_t xAxis153[4] = {-1.5, -0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg248626__102 = new TH1D("PDFBkg248626__102","",3, xAxis153);
   PDFBkg248626__102->SetBinContent(1,0.501485);
   PDFBkg248626__102->SetBinContent(3,0.498515);
   PDFBkg248626__102->SetBinError(1,0.002685594);
   PDFBkg248626__102->SetBinError(3,0.002676556);
   PDFBkg248626__102->SetEntries(105134);
   PDFBkg248626__102->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg248626__102->SetFillColor(ci);
   PDFBkg248626__102->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg248626__102->SetLineColor(ci);
   PDFBkg248626__102->GetXaxis()->SetLabelFont(42);
   PDFBkg248626__102->GetXaxis()->SetLabelSize(0.035);
   PDFBkg248626__102->GetXaxis()->SetTitleSize(0.035);
   PDFBkg248626__102->GetXaxis()->SetTitleFont(42);
   PDFBkg248626__102->GetYaxis()->SetLabelFont(42);
   PDFBkg248626__102->GetYaxis()->SetLabelSize(0.035);
   PDFBkg248626__102->GetYaxis()->SetTitleSize(0.035);
   PDFBkg248626__102->GetYaxis()->SetTitleFont(42);
   PDFBkg248626__102->GetZaxis()->SetLabelFont(42);
   PDFBkg248626__102->GetZaxis()->SetLabelSize(0.035);
   PDFBkg248626__102->GetZaxis()->SetTitleSize(0.035);
   PDFBkg248626__102->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg248626,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.4588777,0.9343782,0.5411223,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Q(l)");
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
   entry=leg->AddEntry("PDFSgn248626"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg248626"," bkg ","f");

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
