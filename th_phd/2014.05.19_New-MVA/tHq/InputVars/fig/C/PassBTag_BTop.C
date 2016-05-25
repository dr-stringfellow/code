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
   mainPad->Range(-0.75,-0.1134788,1.75,1.021309);
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
   hStack->SetTitle("tagged(b_{t});tagged(b_{t});Probability density");
   Double_t xAxis67[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_23 = new TH1F("hStack_stack_23","tagged(b_{t})",2, xAxis67);
   hStack_stack_23->SetMinimum(0);
   hStack_stack_23->SetMaximum(0.9078306);
   hStack_stack_23->SetDirectory(0);
   hStack_stack_23->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   hStack_stack_23->SetLineColor(ci);
   hStack_stack_23->SetLineWidth(2);
   hStack_stack_23->GetXaxis()->SetTitle("tagged(b_{t})");
   hStack_stack_23->GetXaxis()->SetLabelFont(42);
   hStack_stack_23->GetXaxis()->SetLabelSize(0.035);
   hStack_stack_23->GetXaxis()->SetTitleSize(0.035);
   hStack_stack_23->GetXaxis()->SetTitleFont(42);
   hStack_stack_23->GetYaxis()->SetTitle("Probability density");
   hStack_stack_23->GetYaxis()->SetLabelFont(42);
   hStack_stack_23->GetYaxis()->SetLabelSize(0.035);
   hStack_stack_23->GetYaxis()->SetTitleSize(0.035);
   hStack_stack_23->GetYaxis()->SetTitleOffset(1.5);
   hStack_stack_23->GetYaxis()->SetTitleFont(42);
   hStack_stack_23->GetZaxis()->SetLabelFont(42);
   hStack_stack_23->GetZaxis()->SetLabelSize(0.035);
   hStack_stack_23->GetZaxis()->SetTitleSize(0.035);
   hStack_stack_23->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_23);
   
   Double_t xAxis68[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn209441__45 = new TH1D("PDFSgn209441__45","",2, xAxis68);
   PDFSgn209441__45->SetBinContent(1,0.1353994);
   PDFSgn209441__45->SetBinContent(2,0.8646006);
   PDFSgn209441__45->SetBinError(1,0.001651702);
   PDFSgn209441__45->SetBinError(2,0.004077941);
   PDFSgn209441__45->SetEntries(58680);
   PDFSgn209441__45->SetDirectory(0);
   PDFSgn209441__45->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn209441__45->SetLineColor(ci);
   PDFSgn209441__45->GetXaxis()->SetLabelFont(42);
   PDFSgn209441__45->GetXaxis()->SetLabelSize(0.035);
   PDFSgn209441__45->GetXaxis()->SetTitleSize(0.035);
   PDFSgn209441__45->GetXaxis()->SetTitleFont(42);
   PDFSgn209441__45->GetYaxis()->SetLabelFont(42);
   PDFSgn209441__45->GetYaxis()->SetLabelSize(0.035);
   PDFSgn209441__45->GetYaxis()->SetTitleSize(0.035);
   PDFSgn209441__45->GetYaxis()->SetTitleFont(42);
   PDFSgn209441__45->GetZaxis()->SetLabelFont(42);
   PDFSgn209441__45->GetZaxis()->SetLabelSize(0.035);
   PDFSgn209441__45->GetZaxis()->SetTitleSize(0.035);
   PDFSgn209441__45->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn209441,"hist");
   Double_t xAxis69[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg209441__46 = new TH1D("PDFBkg209441__46","",2, xAxis69);
   PDFBkg209441__46->SetBinContent(1,0.2981457);
   PDFBkg209441__46->SetBinContent(2,0.7018543);
   PDFBkg209441__46->SetBinError(1,0.002414693);
   PDFBkg209441__46->SetBinError(2,0.003677905);
   PDFBkg209441__46->SetEntries(58680);
   PDFBkg209441__46->SetDirectory(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg209441__46->SetFillColor(ci);
   PDFBkg209441__46->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg209441__46->SetLineColor(ci);
   PDFBkg209441__46->GetXaxis()->SetLabelFont(42);
   PDFBkg209441__46->GetXaxis()->SetLabelSize(0.035);
   PDFBkg209441__46->GetXaxis()->SetTitleSize(0.035);
   PDFBkg209441__46->GetXaxis()->SetTitleFont(42);
   PDFBkg209441__46->GetYaxis()->SetLabelFont(42);
   PDFBkg209441__46->GetYaxis()->SetLabelSize(0.035);
   PDFBkg209441__46->GetYaxis()->SetTitleSize(0.035);
   PDFBkg209441__46->GetYaxis()->SetTitleFont(42);
   PDFBkg209441__46->GetZaxis()->SetLabelFont(42);
   PDFBkg209441__46->GetZaxis()->SetLabelSize(0.035);
   PDFBkg209441__46->GetZaxis()->SetTitleSize(0.035);
   PDFBkg209441__46->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg209441,"hist");
   hStack->Draw("nostack");
   
   TPaveText *pt = new TPaveText(0.4026719,0.9172798,0.5973281,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("tagged(b_{t})");
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
   entry=leg->AddEntry("PDFSgn209441"," correct comb. ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("PDFBkg209441"," wrong comb. ","f");

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
