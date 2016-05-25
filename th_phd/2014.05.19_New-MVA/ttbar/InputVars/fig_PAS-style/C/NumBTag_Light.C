{
//=========Macro generated from canvas: canvas/canvas
//=========  (Sun Sep 27 19:41:22 2015) by ROOT version5.34/32
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,976);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(-1.107595,-0.1301439,2.689873,0.8709629);
   canvas->SetFillColor(0);
   canvas->SetBorderMode(0);
   canvas->SetBorderSize(2);
   canvas->SetTickx(1);
   canvas->SetTicky(1);
   canvas->SetLeftMargin(0.16);
   canvas->SetRightMargin(0.05);
   canvas->SetTopMargin(0.13);
   canvas->SetBottomMargin(0.13);
   canvas->SetFrameBorderMode(0);
   canvas->SetFrameBorderMode(0);
   
   THStack *hStack = new THStack();
   hStack->SetName("hStack");
   hStack->SetTitle(";#tagged(q_{thad});Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(0.740819);
   Double_t xAxis97[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1F *hStack_stack_33 = new TH1F("hStack_stack_33","",3, xAxis97);
   hStack_stack_33->SetMinimum(0);
   hStack_stack_33->SetMaximum(0.740819);
   hStack_stack_33->SetDirectory(0);
   hStack_stack_33->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_33->SetLineColor(ci);
   hStack_stack_33->SetLineWidth(2);
   hStack_stack_33->GetXaxis()->SetTitle("#tagged(q_{thad})");
   hStack_stack_33->GetXaxis()->SetNdivisions(508);
   hStack_stack_33->GetXaxis()->SetLabelFont(42);
   hStack_stack_33->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_33->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_33->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_33->GetXaxis()->SetTitleFont(42);
   hStack_stack_33->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_33->GetYaxis()->SetNdivisions(508);
   hStack_stack_33->GetYaxis()->SetLabelFont(42);
   hStack_stack_33->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_33->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_33->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_33->GetYaxis()->SetTitleFont(42);
   hStack_stack_33->GetZaxis()->SetNdivisions(508);
   hStack_stack_33->GetZaxis()->SetLabelFont(42);
   hStack_stack_33->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_33->GetZaxis()->SetTitleSize(0.05);
   hStack_stack_33->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_33);
   
   Double_t xAxis98[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFSgn194872__65 = new TH1D("PDFSgn194872__65","",3, xAxis98);
   PDFSgn194872__65->SetBinContent(1,0.293244);
   PDFSgn194872__65->SetBinContent(2,0.7055419);
   PDFSgn194872__65->SetBinContent(3,0.001214081);
   PDFSgn194872__65->SetBinError(1,0.002627559);
   PDFSgn194872__65->SetBinError(2,0.004062136);
   PDFSgn194872__65->SetBinError(3,0.0001732864);
   PDFSgn194872__65->SetEntries(49013);
   PDFSgn194872__65->SetDirectory(0);
   PDFSgn194872__65->SetStats(0);
   PDFSgn194872__65->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn194872__65->SetLineColor(ci);
   PDFSgn194872__65->GetXaxis()->SetNdivisions(508);
   PDFSgn194872__65->GetXaxis()->SetLabelFont(42);
   PDFSgn194872__65->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn194872__65->GetXaxis()->SetTitleSize(0.05);
   PDFSgn194872__65->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn194872__65->GetXaxis()->SetTitleFont(42);
   PDFSgn194872__65->GetYaxis()->SetNdivisions(508);
   PDFSgn194872__65->GetYaxis()->SetLabelFont(42);
   PDFSgn194872__65->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn194872__65->GetYaxis()->SetTitleSize(0.05);
   PDFSgn194872__65->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn194872__65->GetYaxis()->SetTitleFont(42);
   PDFSgn194872__65->GetZaxis()->SetNdivisions(508);
   PDFSgn194872__65->GetZaxis()->SetLabelFont(42);
   PDFSgn194872__65->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn194872__65->GetZaxis()->SetTitleSize(0.05);
   PDFSgn194872__65->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn194872,"hist");
   Double_t xAxis99[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1D *PDFBkg194872__66 = new TH1D("PDFBkg194872__66","",3, xAxis99);
   PDFBkg194872__66->SetBinContent(1,0.3716352);
   PDFBkg194872__66->SetBinContent(2,0.6253119);
   PDFBkg194872__66->SetBinContent(3,0.003052942);
   PDFBkg194872__66->SetBinError(1,0.002941906);
   PDFBkg194872__66->SetBinError(2,0.003835251);
   PDFBkg194872__66->SetBinError(3,0.0002666034);
   PDFBkg194872__66->SetEntries(49013);
   PDFBkg194872__66->SetDirectory(0);
   PDFBkg194872__66->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg194872__66->SetFillColor(ci);
   PDFBkg194872__66->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg194872__66->SetLineColor(ci);
   PDFBkg194872__66->GetXaxis()->SetNdivisions(508);
   PDFBkg194872__66->GetXaxis()->SetLabelFont(42);
   PDFBkg194872__66->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg194872__66->GetXaxis()->SetTitleSize(0.05);
   PDFBkg194872__66->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg194872__66->GetXaxis()->SetTitleFont(42);
   PDFBkg194872__66->GetYaxis()->SetNdivisions(508);
   PDFBkg194872__66->GetYaxis()->SetLabelFont(42);
   PDFBkg194872__66->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg194872__66->GetYaxis()->SetTitleSize(0.05);
   PDFBkg194872__66->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg194872__66->GetYaxis()->SetTitleFont(42);
   PDFBkg194872__66->GetZaxis()->SetNdivisions(508);
   PDFBkg194872__66->GetZaxis()->SetLabelFont(42);
   PDFBkg194872__66->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg194872__66->GetZaxis()->SetTitleSize(0.05);
   PDFBkg194872__66->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg194872,"hist");
   hStack->Draw("nostack");
   TLatex *   tex = new TLatex(0.2,0.89,"#scale[1.2]{#font[62]{CMS}} #font[52]{Simulation Preliminary}");
tex->SetNDC();
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.95,0.89,"8 TeV");
tex->SetNDC();
   tex->SetTextAlign(31);
   tex->SetTextFont(42);
   tex->SetLineWidth(2);
   tex->Draw();
   
   TLegend *leg = new TLegend(0.73,0.67,0.92,0.84,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("NULL","Interpretation","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFSgn194872"," correct ","f");
   entry->SetFillColor(38);
   entry->SetFillStyle(1001);

   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("PDFBkg194872"," wrong ","f");

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
   entry->SetTextFont(42);
   leg->Draw();
   canvas->Modified();
   canvas->cd();
   canvas->SetSelected(canvas);
}
