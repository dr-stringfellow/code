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
   mainPad->Range(-0.875,-0.09594886,2.875,0.8635397);
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
   hStack->SetTitle("#tagged(b_{H}) [tHq];#tagged(b_{H}) [tHq];Probability density");
   Double_t xAxis157[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1F *hStack_stack_53 = new TH1F("hStack_stack_53","#tagged(b_{H}) [tHq]",3, xAxis157);
   hStack_stack_53->SetMinimum(0);
   hStack_stack_53->SetMaximum(0.7675908);
   hStack_stack_53->SetDirectory(0);
   hStack_stack_53->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_53->SetLineColor(ci);
   hStack_stack_53->SetLineWidth(2);
   hStack_stack_53->GetXaxis()->SetTitle("#tagged(b_{H}) [tHq]");
   hStack_stack_53->GetXaxis()->SetLabelFont(42);
   hStack_stack_53->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_53->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_53->GetXaxis()->SetTitleFont(42);
   hStack_stack_53->GetYaxis()->SetTitle("Probability density");
   hStack_stack_53->GetYaxis()->SetLabelFont(42);
   hStack_stack_53->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_53->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_53->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_53->GetYaxis()->SetTitleFont(42);
   hStack_stack_53->GetZaxis()->SetLabelFont(42);
   hStack_stack_53->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_53->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_53->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_53);
   
   Double_t xAxis158[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFSgn503465__105 = new TH1D("PDFSgn503465__105","",3, xAxis158);
   PDFSgn503465__105->SetBinContent(1,0.001195409);
   PDFSgn503465__105->SetBinContent(2,0.2677657);
   PDFSgn503465__105->SetBinContent(3,0.7310389);
   PDFSgn503465__105->SetBinError(1,0.0001160639);
   PDFSgn503465__105->SetBinError(2,0.001733436);
   PDFSgn503465__105->SetBinError(3,0.00282056);
   PDFSgn503465__105->SetEntries(119361);
   PDFSgn503465__105->SetDirectory(0);
   PDFSgn503465__105->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn503465__105->SetLineColor(ci);
   PDFSgn503465__105->GetXaxis()->SetLabelFont(42);
   PDFSgn503465__105->GetXaxis()->SetLabelSize(0.035);
   PDFSgn503465__105->GetXaxis()->SetTitleSize(0.035);
   PDFSgn503465__105->GetXaxis()->SetTitleFont(42);
   PDFSgn503465__105->GetYaxis()->SetLabelFont(42);
   PDFSgn503465__105->GetYaxis()->SetLabelSize(0.035);
   PDFSgn503465__105->GetYaxis()->SetTitleSize(0.035);
   PDFSgn503465__105->GetYaxis()->SetTitleFont(42);
   PDFSgn503465__105->GetZaxis()->SetLabelFont(42);
   PDFSgn503465__105->GetZaxis()->SetLabelSize(0.035);
   PDFSgn503465__105->GetZaxis()->SetTitleSize(0.035);
   PDFSgn503465__105->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn503465,"hist");
   Double_t xAxis159[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFBkg503465__106 = new TH1D("PDFBkg503465__106","",3, xAxis159);
   PDFBkg503465__106->SetBinContent(1,0.005301356);
   PDFBkg503465__106->SetBinContent(2,0.3861006);
   PDFBkg503465__106->SetBinContent(3,0.608598);
   PDFBkg503465__106->SetBinError(1,0.0002718372);
   PDFBkg503465__106->SetBinError(2,0.002321272);
   PDFBkg503465__106->SetBinError(3,0.00298566);
   PDFBkg503465__106->SetEntries(105134);
   PDFBkg503465__106->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg503465__106->SetFillColor(ci);
   PDFBkg503465__106->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg503465__106->SetLineColor(ci);
   PDFBkg503465__106->GetXaxis()->SetLabelFont(42);
   PDFBkg503465__106->GetXaxis()->SetLabelSize(0.035);
   PDFBkg503465__106->GetXaxis()->SetTitleSize(0.035);
   PDFBkg503465__106->GetXaxis()->SetTitleFont(42);
   PDFBkg503465__106->GetYaxis()->SetLabelFont(42);
   PDFBkg503465__106->GetYaxis()->SetLabelSize(0.035);
   PDFBkg503465__106->GetYaxis()->SetTitleSize(0.035);
   PDFBkg503465__106->GetYaxis()->SetTitleFont(42);
   PDFBkg503465__106->GetZaxis()->SetLabelFont(42);
   PDFBkg503465__106->GetZaxis()->SetLabelSize(0.035);
   PDFBkg503465__106->GetZaxis()->SetTitleSize(0.035);
   PDFBkg503465__106->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg503465,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.3399628,0.9172798,0.6600372,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("#tagged(b_{H}) [tHq]");
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
   entry=leg->AddEntry("PDFSgn503465"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg503465"," bkg ","f");

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
