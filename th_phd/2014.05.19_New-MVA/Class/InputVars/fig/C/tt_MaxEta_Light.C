{
//=========Macro generated from canvas: canvas/canvas
//=========  (Tue May 20 17:25:43 2014) by ROOT version5.34/09
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
   mainPad->Range(-0.5875,-0.08305633,5.2875,0.7475069);
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
   hStack->SetTitle("max |#eta(q_{Whad})| [t#bar{t}];max |#eta(q_{Whad})| [t#bar{t}];Probability density");
   Double_t xAxis172[15] = {0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 3.2, 4.7}; 
   
   TH1F *hStack_stack_58 = new TH1F("hStack_stack_58","max |#eta(q_{Whad})| [t#bar{t}]",14, xAxis172);
   hStack_stack_58->SetMinimum(0);
   hStack_stack_58->SetMaximum(0.6644506);
   hStack_stack_58->SetDirectory(0);
   hStack_stack_58->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_58->SetLineColor(ci);
   hStack_stack_58->SetLineWidth(2);
   hStack_stack_58->GetXaxis()->SetTitle("max |#eta(q_{Whad})| [t#bar{t}]");
   hStack_stack_58->GetXaxis()->SetLabelFont(42);
   hStack_stack_58->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_58->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_58->GetXaxis()->SetTitleFont(42);
   hStack_stack_58->GetYaxis()->SetTitle("Probability density");
   hStack_stack_58->GetYaxis()->SetLabelFont(42);
   hStack_stack_58->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_58->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_58->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_58->GetYaxis()->SetTitleFont(42);
   hStack_stack_58->GetZaxis()->SetLabelFont(42);
   hStack_stack_58->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_58->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_58->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_58);
   
   Double_t xAxis173[15] = {0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 3.2, 4.7}; 
   
   TH1D *PDFSgn412924__115 = new TH1D("PDFSgn412924__115","",14, xAxis173);
   PDFSgn412924__115->SetBinContent(1,0.06399963);
   PDFSgn412924__115->SetBinContent(2,0.2001357);
   PDFSgn412924__115->SetBinContent(3,0.323796);
   PDFSgn412924__115->SetBinContent(4,0.4200947);
   PDFSgn412924__115->SetBinContent(5,0.4720923);
   PDFSgn412924__115->SetBinContent(6,0.4872879);
   PDFSgn412924__115->SetBinContent(7,0.4614874);
   PDFSgn412924__115->SetBinContent(8,0.4364736);
   PDFSgn412924__115->SetBinContent(9,0.3920159);
   PDFSgn412924__115->SetBinContent(10,0.3335181);
   PDFSgn412924__115->SetBinContent(11,0.3116488);
   PDFSgn412924__115->SetBinContent(12,0.2844908);
   PDFSgn412924__115->SetBinContent(13,0.1330999);
   PDFSgn412924__115->SetBinContent(14,0.03740796);
   PDFSgn412924__115->SetBinError(1,0.001883393);
   PDFSgn412924__115->SetBinError(2,0.003337691);
   PDFSgn412924__115->SetBinError(3,0.004228362);
   PDFSgn412924__115->SetBinError(4,0.004816116);
   PDFSgn412924__115->SetBinError(5,0.005095761);
   PDFSgn412924__115->SetBinError(6,0.005209766);
   PDFSgn412924__115->SetBinError(7,0.005028771);
   PDFSgn412924__115->SetBinError(8,0.004908568);
   PDFSgn412924__115->SetBinError(9,0.004638008);
   PDFSgn412924__115->SetBinError(10,0.004288526);
   PDFSgn412924__115->SetBinError(11,0.004126719);
   PDFSgn412924__115->SetBinError(12,0.003937106);
   PDFSgn412924__115->SetBinError(13,0.001337499);
   PDFSgn412924__115->SetBinError(14,0.0005170474);
   PDFSgn412924__115->SetEntries(119361);
   PDFSgn412924__115->SetDirectory(0);
   PDFSgn412924__115->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn412924__115->SetLineColor(ci);
   PDFSgn412924__115->GetXaxis()->SetLabelFont(42);
   PDFSgn412924__115->GetXaxis()->SetLabelSize(0.035);
   PDFSgn412924__115->GetXaxis()->SetTitleSize(0.035);
   PDFSgn412924__115->GetXaxis()->SetTitleFont(42);
   PDFSgn412924__115->GetYaxis()->SetLabelFont(42);
   PDFSgn412924__115->GetYaxis()->SetLabelSize(0.035);
   PDFSgn412924__115->GetYaxis()->SetTitleSize(0.035);
   PDFSgn412924__115->GetYaxis()->SetTitleFont(42);
   PDFSgn412924__115->GetZaxis()->SetLabelFont(42);
   PDFSgn412924__115->GetZaxis()->SetLabelSize(0.035);
   PDFSgn412924__115->GetZaxis()->SetTitleSize(0.035);
   PDFSgn412924__115->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn412924,"hist");
   Double_t xAxis174[15] = {0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 3.2, 4.7}; 
   
   TH1D *PDFBkg412924__116 = new TH1D("PDFBkg412924__116","",14, xAxis174);
   PDFBkg412924__116->SetBinContent(1,0.09138783);
   PDFBkg412924__116->SetBinContent(2,0.2804387);
   PDFBkg412924__116->SetBinContent(3,0.4783861);
   PDFBkg412924__116->SetBinContent(4,0.589697);
   PDFBkg412924__116->SetBinContent(5,0.6328101);
   PDFBkg412924__116->SetBinContent(6,0.5895558);
   PDFBkg412924__116->SetBinContent(7,0.54612);
   PDFBkg412924__116->SetBinContent(8,0.4951421);
   PDFBkg412924__116->SetBinContent(9,0.3951732);
   PDFBkg412924__116->SetBinContent(10,0.3062249);
   PDFBkg412924__116->SetBinContent(11,0.2510422);
   PDFBkg412924__116->SetBinContent(12,0.1877019);
   PDFBkg412924__116->SetBinContent(13,0.03200023);
   PDFBkg412924__116->SetBinContent(14,0.003775903);
   PDFBkg412924__116->SetBinError(1,0.002506304);
   PDFBkg412924__116->SetBinError(2,0.004403743);
   PDFBkg412924__116->SetBinError(3,0.005764454);
   PDFBkg412924__116->SetBinError(4,0.00642316);
   PDFBkg412924__116->SetBinError(5,0.00670353);
   PDFBkg412924__116->SetBinError(6,0.006458196);
   PDFBkg412924__116->SetBinError(7,0.006276059);
   PDFBkg412924__116->SetBinError(8,0.005987323);
   PDFBkg412924__116->SetBinError(9,0.00537234);
   PDFBkg412924__116->SetBinError(10,0.00473777);
   PDFBkg412924__116->SetBinError(11,0.004378759);
   PDFBkg412924__116->SetBinError(12,0.003799095);
   PDFBkg412924__116->SetBinError(13,0.0007891413);
   PDFBkg412924__116->SetBinError(14,0.0002102755);
   PDFBkg412924__116->SetEntries(105134);
   PDFBkg412924__116->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg412924__116->SetFillColor(ci);
   PDFBkg412924__116->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg412924__116->SetLineColor(ci);
   PDFBkg412924__116->GetXaxis()->SetLabelFont(42);
   PDFBkg412924__116->GetXaxis()->SetLabelSize(0.035);
   PDFBkg412924__116->GetXaxis()->SetTitleSize(0.035);
   PDFBkg412924__116->GetXaxis()->SetTitleFont(42);
   PDFBkg412924__116->GetYaxis()->SetLabelFont(42);
   PDFBkg412924__116->GetYaxis()->SetLabelSize(0.035);
   PDFBkg412924__116->GetYaxis()->SetTitleSize(0.035);
   PDFBkg412924__116->GetYaxis()->SetTitleFont(42);
   PDFBkg412924__116->GetZaxis()->SetLabelFont(42);
   PDFBkg412924__116->GetZaxis()->SetLabelSize(0.035);
   PDFBkg412924__116->GetZaxis()->SetTitleSize(0.035);
   PDFBkg412924__116->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg412924,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.344608,0.8986269,0.655392,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("max |#eta(q_{Whad})| [t#bar{t}]");
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
   entry=leg->AddEntry("PDFSgn412924"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg412924"," bkg ","f");

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
