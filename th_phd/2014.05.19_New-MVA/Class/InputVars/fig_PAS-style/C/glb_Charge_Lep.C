{
//=========Macro generated from canvas: canvas/canvas
//=========  (Wed Sep 24 11:59:58 2014) by ROOT version5.34/20
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,976);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(-2.107595,-0.1254684,1.689873,0.8396731);
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
   hStack->SetTitle(";Q(l);Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(0.7142047);
   Double_t xAxis151[4] = {-1.5, -0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_51 = new TH1F("hStack_stack_51","",3, xAxis151);
   hStack_stack_51->SetMinimum(0);
   hStack_stack_51->SetMaximum(0.7142047);
   hStack_stack_51->SetDirectory(0);
   hStack_stack_51->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_51->SetLineColor(ci);
   hStack_stack_51->SetLineWidth(2);
   hStack_stack_51->GetXaxis()->SetTitle("Q(l)");
   hStack_stack_51->GetXaxis()->SetNdivisions(508);
   hStack_stack_51->GetXaxis()->SetLabelFont(42);
   hStack_stack_51->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_51->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_51->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_51->GetXaxis()->SetTitleFont(42);
   hStack_stack_51->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_51->GetYaxis()->SetNdivisions(508);
   hStack_stack_51->GetYaxis()->SetLabelFont(42);
   hStack_stack_51->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_51->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_51->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_51->GetYaxis()->SetTitleFont(42);
   hStack_stack_51->GetZaxis()->SetNdivisions(508);
   hStack_stack_51->GetZaxis()->SetLabelFont(42);
   hStack_stack_51->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_51->GetZaxis()->SetTitleSize(0.05);
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
   PDFSgn248626__101->SetStats(0);
   PDFSgn248626__101->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn248626__101->SetLineColor(ci);
   PDFSgn248626__101->GetXaxis()->SetNdivisions(508);
   PDFSgn248626__101->GetXaxis()->SetLabelFont(42);
   PDFSgn248626__101->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn248626__101->GetXaxis()->SetTitleSize(0.05);
   PDFSgn248626__101->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn248626__101->GetXaxis()->SetTitleFont(42);
   PDFSgn248626__101->GetYaxis()->SetNdivisions(508);
   PDFSgn248626__101->GetYaxis()->SetLabelFont(42);
   PDFSgn248626__101->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn248626__101->GetYaxis()->SetTitleSize(0.05);
   PDFSgn248626__101->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn248626__101->GetYaxis()->SetTitleFont(42);
   PDFSgn248626__101->GetZaxis()->SetNdivisions(508);
   PDFSgn248626__101->GetZaxis()->SetLabelFont(42);
   PDFSgn248626__101->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn248626__101->GetZaxis()->SetTitleSize(0.05);
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
   PDFBkg248626__102->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg248626__102->SetFillColor(ci);
   PDFBkg248626__102->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg248626__102->SetLineColor(ci);
   PDFBkg248626__102->GetXaxis()->SetNdivisions(508);
   PDFBkg248626__102->GetXaxis()->SetLabelFont(42);
   PDFBkg248626__102->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg248626__102->GetXaxis()->SetTitleSize(0.05);
   PDFBkg248626__102->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg248626__102->GetXaxis()->SetTitleFont(42);
   PDFBkg248626__102->GetYaxis()->SetNdivisions(508);
   PDFBkg248626__102->GetYaxis()->SetLabelFont(42);
   PDFBkg248626__102->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg248626__102->GetYaxis()->SetTitleSize(0.05);
   PDFBkg248626__102->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg248626__102->GetYaxis()->SetTitleFont(42);
   PDFBkg248626__102->GetZaxis()->SetNdivisions(508);
   PDFBkg248626__102->GetZaxis()->SetLabelFont(42);
   PDFBkg248626__102->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg248626__102->GetZaxis()->SetTitleSize(0.05);
   PDFBkg248626__102->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg248626,"hist");
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
   
   TLegend *leg = new TLegend(0.19,0.72,0.34,0.84,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("PDFSgn248626"," tHq ","f");
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
   entry->SetTextFont(42);
   leg->Draw();
   canvas->Modified();
   canvas->cd();
   canvas->SetSelected(canvas);
}
