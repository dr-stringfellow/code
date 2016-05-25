{
//=========Macro generated from canvas: canvas/canvas
//=========  (Wed Sep 24 11:55:57 2014) by ROOT version5.34/20
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,976);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(-0.9050633,-0.1594837,1.626582,1.067314);
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
   hStack->SetTitle(";tagged(b_{t});Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(0.9078306);
   Double_t xAxis67[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_23 = new TH1F("hStack_stack_23","",2, xAxis67);
   hStack_stack_23->SetMinimum(0);
   hStack_stack_23->SetMaximum(0.9078306);
   hStack_stack_23->SetDirectory(0);
   hStack_stack_23->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_23->SetLineColor(ci);
   hStack_stack_23->SetLineWidth(2);
   hStack_stack_23->GetXaxis()->SetTitle("tagged(b_{t})");
   hStack_stack_23->GetXaxis()->SetNdivisions(508);
   hStack_stack_23->GetXaxis()->SetLabelFont(42);
   hStack_stack_23->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_23->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_23->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_23->GetXaxis()->SetTitleFont(42);
   hStack_stack_23->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_23->GetYaxis()->SetNdivisions(508);
   hStack_stack_23->GetYaxis()->SetLabelFont(42);
   hStack_stack_23->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_23->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_23->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_23->GetYaxis()->SetTitleFont(42);
   hStack_stack_23->GetZaxis()->SetNdivisions(508);
   hStack_stack_23->GetZaxis()->SetLabelFont(42);
   hStack_stack_23->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_23->GetZaxis()->SetTitleSize(0.05);
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
   PDFSgn209441__45->SetStats(0);
   PDFSgn209441__45->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn209441__45->SetLineColor(ci);
   PDFSgn209441__45->GetXaxis()->SetNdivisions(508);
   PDFSgn209441__45->GetXaxis()->SetLabelFont(42);
   PDFSgn209441__45->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn209441__45->GetXaxis()->SetTitleSize(0.05);
   PDFSgn209441__45->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn209441__45->GetXaxis()->SetTitleFont(42);
   PDFSgn209441__45->GetYaxis()->SetNdivisions(508);
   PDFSgn209441__45->GetYaxis()->SetLabelFont(42);
   PDFSgn209441__45->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn209441__45->GetYaxis()->SetTitleSize(0.05);
   PDFSgn209441__45->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn209441__45->GetYaxis()->SetTitleFont(42);
   PDFSgn209441__45->GetZaxis()->SetNdivisions(508);
   PDFSgn209441__45->GetZaxis()->SetLabelFont(42);
   PDFSgn209441__45->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn209441__45->GetZaxis()->SetTitleSize(0.05);
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
   PDFBkg209441__46->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg209441__46->SetFillColor(ci);
   PDFBkg209441__46->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg209441__46->SetLineColor(ci);
   PDFBkg209441__46->GetXaxis()->SetNdivisions(508);
   PDFBkg209441__46->GetXaxis()->SetLabelFont(42);
   PDFBkg209441__46->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg209441__46->GetXaxis()->SetTitleSize(0.05);
   PDFBkg209441__46->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg209441__46->GetXaxis()->SetTitleFont(42);
   PDFBkg209441__46->GetYaxis()->SetNdivisions(508);
   PDFBkg209441__46->GetYaxis()->SetLabelFont(42);
   PDFBkg209441__46->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg209441__46->GetYaxis()->SetTitleSize(0.05);
   PDFBkg209441__46->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg209441__46->GetYaxis()->SetTitleFont(42);
   PDFBkg209441__46->GetZaxis()->SetNdivisions(508);
   PDFBkg209441__46->GetZaxis()->SetLabelFont(42);
   PDFBkg209441__46->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg209441__46->GetZaxis()->SetTitleSize(0.05);
   PDFBkg209441__46->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg209441,"hist");
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
   
   TLegend *leg = new TLegend(0.19,0.67,0.38,0.84,NULL,"brNDC");
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
   entry=leg->AddEntry("PDFSgn209441"," correct ","f");
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
   entry=leg->AddEntry("PDFBkg209441"," wrong ","f");

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
