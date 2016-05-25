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
   mainPad->Range(-0.875,-0.1049106,2.875,0.944195);
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
   hStack->SetTitle("#tagged(q_{Whad}) [t#bar{t}];#tagged(q_{Whad}) [t#bar{t}];Probability density");
   Double_t xAxis166[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1F *hStack_stack_56 = new TH1F("hStack_stack_56","#tagged(q_{Whad}) [t#bar{t}]",3, xAxis166);
   hStack_stack_56->SetMinimum(0);
   hStack_stack_56->SetMaximum(0.8392845);
   hStack_stack_56->SetDirectory(0);
   hStack_stack_56->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_56->SetLineColor(ci);
   hStack_stack_56->SetLineWidth(2);
   hStack_stack_56->GetXaxis()->SetTitle("#tagged(q_{Whad}) [t#bar{t}]");
   hStack_stack_56->GetXaxis()->SetLabelFont(42);
   hStack_stack_56->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_56->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_56->GetXaxis()->SetTitleFont(42);
   hStack_stack_56->GetYaxis()->SetTitle("Probability density");
   hStack_stack_56->GetYaxis()->SetLabelFont(42);
   hStack_stack_56->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_56->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_56->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_56->GetYaxis()->SetTitleFont(42);
   hStack_stack_56->GetZaxis()->SetLabelFont(42);
   hStack_stack_56->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_56->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_56->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_56);
   
   Double_t xAxis167[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFSgn408282__111 = new TH1D("PDFSgn408282__111","",3, xAxis167);
   PDFSgn408282__111->SetBinContent(1,0.1642646);
   PDFSgn408282__111->SetBinContent(2,0.7993185);
   PDFSgn408282__111->SetBinContent(3,0.0364169);
   PDFSgn408282__111->SetBinError(1,0.001346383);
   PDFSgn408282__111->SetBinError(2,0.002965265);
   PDFSgn408282__111->SetBinError(3,0.0006068669);
   PDFSgn408282__111->SetEntries(119361);
   PDFSgn408282__111->SetDirectory(0);
   PDFSgn408282__111->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn408282__111->SetLineColor(ci);
   PDFSgn408282__111->GetXaxis()->SetLabelFont(42);
   PDFSgn408282__111->GetXaxis()->SetLabelSize(0.035);
   PDFSgn408282__111->GetXaxis()->SetTitleSize(0.035);
   PDFSgn408282__111->GetXaxis()->SetTitleFont(42);
   PDFSgn408282__111->GetYaxis()->SetLabelFont(42);
   PDFSgn408282__111->GetYaxis()->SetLabelSize(0.035);
   PDFSgn408282__111->GetYaxis()->SetTitleSize(0.035);
   PDFSgn408282__111->GetYaxis()->SetTitleFont(42);
   PDFSgn408282__111->GetZaxis()->SetLabelFont(42);
   PDFSgn408282__111->GetZaxis()->SetLabelSize(0.035);
   PDFSgn408282__111->GetZaxis()->SetTitleSize(0.035);
   PDFSgn408282__111->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn408282,"hist");
   Double_t xAxis168[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFBkg408282__112 = new TH1D("PDFBkg408282__112","",3, xAxis168);
   PDFBkg408282__112->SetBinContent(1,0.2485578);
   PDFBkg408282__112->SetBinContent(2,0.7477212);
   PDFBkg408282__112->SetBinContent(3,0.003720973);
   PDFBkg408282__112->SetBinError(1,0.001865875);
   PDFBkg408282__112->SetBinError(2,0.003292699);
   PDFBkg408282__112->SetBinError(3,0.0002302437);
   PDFBkg408282__112->SetEntries(105134);
   PDFBkg408282__112->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg408282__112->SetFillColor(ci);
   PDFBkg408282__112->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg408282__112->SetLineColor(ci);
   PDFBkg408282__112->GetXaxis()->SetLabelFont(42);
   PDFBkg408282__112->GetXaxis()->SetLabelSize(0.035);
   PDFBkg408282__112->GetXaxis()->SetTitleSize(0.035);
   PDFBkg408282__112->GetXaxis()->SetTitleFont(42);
   PDFBkg408282__112->GetYaxis()->SetLabelFont(42);
   PDFBkg408282__112->GetYaxis()->SetLabelSize(0.035);
   PDFBkg408282__112->GetYaxis()->SetTitleSize(0.035);
   PDFBkg408282__112->GetYaxis()->SetTitleFont(42);
   PDFBkg408282__112->GetZaxis()->SetLabelFont(42);
   PDFBkg408282__112->GetZaxis()->SetLabelSize(0.035);
   PDFBkg408282__112->GetZaxis()->SetTitleSize(0.035);
   PDFBkg408282__112->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg408282,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.3353177,0.8986269,0.6646823,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("#tagged(q_{Whad}) [t#bar{t}]");
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
   entry=leg->AddEntry("PDFSgn408282"," tHq ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg408282"," bkg ","f");

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
