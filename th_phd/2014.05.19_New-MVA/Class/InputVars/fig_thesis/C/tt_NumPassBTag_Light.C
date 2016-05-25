{
//=========Macro generated from canvas: canvas/canvas
//=========  (Sun Sep 27 19:43:54 2015) by ROOT version5.34/32
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,976);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(-1.107595,-0.1474419,2.689873,0.9867263);
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
   hStack->SetTitle(";#tagged(q_{Whad}) [t#bar{t}];Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(0.8392845);
   Double_t xAxis166[4] = {-0.5, 0.5, 1.5, 2.5}; 
   
   TH1F *hStack_stack_56 = new TH1F("hStack_stack_56","",3, xAxis166);
   hStack_stack_56->SetMinimum(0);
   hStack_stack_56->SetMaximum(0.8392845);
   hStack_stack_56->SetDirectory(0);
   hStack_stack_56->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_56->SetLineColor(ci);
   hStack_stack_56->SetLineWidth(2);
   hStack_stack_56->GetXaxis()->SetTitle("#tagged(q_{Whad}) [t#bar{t}]");
   hStack_stack_56->GetXaxis()->SetNdivisions(508);
   hStack_stack_56->GetXaxis()->SetLabelFont(42);
   hStack_stack_56->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_56->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_56->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_56->GetXaxis()->SetTitleFont(42);
   hStack_stack_56->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_56->GetYaxis()->SetNdivisions(508);
   hStack_stack_56->GetYaxis()->SetLabelFont(42);
   hStack_stack_56->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_56->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_56->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_56->GetYaxis()->SetTitleFont(42);
   hStack_stack_56->GetZaxis()->SetNdivisions(508);
   hStack_stack_56->GetZaxis()->SetLabelFont(42);
   hStack_stack_56->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_56->GetZaxis()->SetTitleSize(0.05);
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
   PDFSgn408282__111->SetStats(0);
   PDFSgn408282__111->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn408282__111->SetLineColor(ci);
   PDFSgn408282__111->GetXaxis()->SetNdivisions(508);
   PDFSgn408282__111->GetXaxis()->SetLabelFont(42);
   PDFSgn408282__111->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn408282__111->GetXaxis()->SetTitleSize(0.05);
   PDFSgn408282__111->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn408282__111->GetXaxis()->SetTitleFont(42);
   PDFSgn408282__111->GetYaxis()->SetNdivisions(508);
   PDFSgn408282__111->GetYaxis()->SetLabelFont(42);
   PDFSgn408282__111->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn408282__111->GetYaxis()->SetTitleSize(0.05);
   PDFSgn408282__111->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn408282__111->GetYaxis()->SetTitleFont(42);
   PDFSgn408282__111->GetZaxis()->SetNdivisions(508);
   PDFSgn408282__111->GetZaxis()->SetLabelFont(42);
   PDFSgn408282__111->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn408282__111->GetZaxis()->SetTitleSize(0.05);
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
   PDFBkg408282__112->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg408282__112->SetFillColor(ci);
   PDFBkg408282__112->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg408282__112->SetLineColor(ci);
   PDFBkg408282__112->GetXaxis()->SetNdivisions(508);
   PDFBkg408282__112->GetXaxis()->SetLabelFont(42);
   PDFBkg408282__112->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg408282__112->GetXaxis()->SetTitleSize(0.05);
   PDFBkg408282__112->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg408282__112->GetXaxis()->SetTitleFont(42);
   PDFBkg408282__112->GetYaxis()->SetNdivisions(508);
   PDFBkg408282__112->GetYaxis()->SetLabelFont(42);
   PDFBkg408282__112->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg408282__112->GetYaxis()->SetTitleSize(0.05);
   PDFBkg408282__112->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg408282__112->GetYaxis()->SetTitleFont(42);
   PDFBkg408282__112->GetZaxis()->SetNdivisions(508);
   PDFBkg408282__112->GetZaxis()->SetLabelFont(42);
   PDFBkg408282__112->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg408282__112->GetZaxis()->SetTitleSize(0.05);
   PDFBkg408282__112->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg408282,"hist");
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
   
   TLegend *leg = new TLegend(0.77,0.72,0.92,0.84,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("PDFSgn408282"," tHq ","f");
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
   entry->SetTextFont(42);
   leg->Draw();
      tex = new TLatex(0.96,0.13,"Underflow: #color[4]{0%} / #color[2]{0%},  Overflow: #color[4]{0%} / #color[2]{0%}");
tex->SetNDC();
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetTextAngle(90);
   tex->SetLineWidth(2);
   tex->Draw();
   canvas->Modified();
   canvas->cd();
   canvas->SetSelected(canvas);
}
