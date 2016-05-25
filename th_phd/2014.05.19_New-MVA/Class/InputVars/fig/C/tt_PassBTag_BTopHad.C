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
   hStack->SetTitle("tagged(b_{thad}) [t#bar{t}];tagged(b_{thad}) [t#bar{t}];Probability density");
   Double_t xAxis163[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_55 = new TH1F("hStack_stack_55","tagged(b_{thad}) [t#bar{t}]",2, xAxis163);
   hStack_stack_55->SetMinimum(0);
   hStack_stack_55->SetMaximum(1.05);
   hStack_stack_55->SetDirectory(0);
   hStack_stack_55->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_55->SetLineColor(ci);
   hStack_stack_55->SetLineWidth(2);
   hStack_stack_55->GetXaxis()->SetTitle("tagged(b_{thad}) [t#bar{t}]");
   hStack_stack_55->GetXaxis()->SetLabelFont(42);
   hStack_stack_55->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_55->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_55->GetXaxis()->SetTitleFont(42);
   hStack_stack_55->GetYaxis()->SetTitle("Probability density");
   hStack_stack_55->GetYaxis()->SetLabelFont(42);
   hStack_stack_55->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_55->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_55->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_55->GetYaxis()->SetTitleFont(42);
   hStack_stack_55->GetZaxis()->SetLabelFont(42);
   hStack_stack_55->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_55->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_55->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_55);
   
   Double_t xAxis164[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn473416__109 = new TH1D("PDFSgn473416__109","",2, xAxis164);
   PDFSgn473416__109->SetBinContent(2,1);
   PDFSgn473416__109->SetBinError(2,0.003312678);
   PDFSgn473416__109->SetEntries(119361);
   PDFSgn473416__109->SetDirectory(0);
   PDFSgn473416__109->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn473416__109->SetLineColor(ci);
   PDFSgn473416__109->GetXaxis()->SetLabelFont(42);
   PDFSgn473416__109->GetXaxis()->SetLabelSize(0.035);
   PDFSgn473416__109->GetXaxis()->SetTitleSize(0.035);
   PDFSgn473416__109->GetXaxis()->SetTitleFont(42);
   PDFSgn473416__109->GetYaxis()->SetLabelFont(42);
   PDFSgn473416__109->GetYaxis()->SetLabelSize(0.035);
   PDFSgn473416__109->GetYaxis()->SetTitleSize(0.035);
   PDFSgn473416__109->GetYaxis()->SetTitleFont(42);
   PDFSgn473416__109->GetZaxis()->SetLabelFont(42);
   PDFSgn473416__109->GetZaxis()->SetLabelSize(0.035);
   PDFSgn473416__109->GetZaxis()->SetTitleSize(0.035);
   PDFSgn473416__109->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn473416,"hist");
   Double_t xAxis165[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg473416__110 = new TH1D("PDFBkg473416__110","",2, xAxis165);
   PDFBkg473416__110->SetBinContent(2,1);
   PDFBkg473416__110->SetBinError(2,0.003791618);
   PDFBkg473416__110->SetEntries(105134);
   PDFBkg473416__110->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg473416__110->SetFillColor(ci);
   PDFBkg473416__110->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg473416__110->SetLineColor(ci);
   PDFBkg473416__110->GetXaxis()->SetLabelFont(42);
   PDFBkg473416__110->GetXaxis()->SetLabelSize(0.035);
   PDFBkg473416__110->GetXaxis()->SetTitleSize(0.035);
   PDFBkg473416__110->GetXaxis()->SetTitleFont(42);
   PDFBkg473416__110->GetYaxis()->SetLabelFont(42);
   PDFBkg473416__110->GetYaxis()->SetLabelSize(0.035);
   PDFBkg473416__110->GetYaxis()->SetTitleSize(0.035);
   PDFBkg473416__110->GetYaxis()->SetTitleFont(42);
   PDFBkg473416__110->GetZaxis()->SetLabelFont(42);
   PDFBkg473416__110->GetZaxis()->SetLabelSize(0.035);
   PDFBkg473416__110->GetZaxis()->SetTitleSize(0.035);
   PDFBkg473416__110->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg473416,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.3525046,0.8986269,0.6474954,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("tagged(b_{thad}) [t#bar{t}]");
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
   entry=leg->AddEntry("PDFSgn473416"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg473416"," bkg ","f");

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
