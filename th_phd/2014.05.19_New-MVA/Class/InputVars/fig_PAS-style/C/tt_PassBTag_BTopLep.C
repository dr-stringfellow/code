{
//=========Macro generated from canvas: canvas/canvas
//=========  (Wed Sep 24 11:59:59 2014) by ROOT version5.34/20
   TCanvas *canvas = new TCanvas("canvas", "canvas",1,1,1200,976);
   gStyle->SetOptStat(0);
   canvas->SetHighLightColor(2);
   canvas->Range(-0.9050633,-0.1844595,1.626582,1.234459);
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
   hStack->SetTitle(";tagged(b_{tlep}) [t#bar{t}];Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(1.05);
   Double_t xAxis160[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_54 = new TH1F("hStack_stack_54","",2, xAxis160);
   hStack_stack_54->SetMinimum(0);
   hStack_stack_54->SetMaximum(1.05);
   hStack_stack_54->SetDirectory(0);
   hStack_stack_54->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_54->SetLineColor(ci);
   hStack_stack_54->SetLineWidth(2);
   hStack_stack_54->GetXaxis()->SetTitle("tagged(b_{tlep}) [t#bar{t}]");
   hStack_stack_54->GetXaxis()->SetNdivisions(508);
   hStack_stack_54->GetXaxis()->SetLabelFont(42);
   hStack_stack_54->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_54->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_54->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_54->GetXaxis()->SetTitleFont(42);
   hStack_stack_54->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_54->GetYaxis()->SetNdivisions(508);
   hStack_stack_54->GetYaxis()->SetLabelFont(42);
   hStack_stack_54->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_54->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_54->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_54->GetYaxis()->SetTitleFont(42);
   hStack_stack_54->GetZaxis()->SetNdivisions(508);
   hStack_stack_54->GetZaxis()->SetLabelFont(42);
   hStack_stack_54->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_54->GetZaxis()->SetTitleSize(0.05);
   hStack_stack_54->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_54);
   
   Double_t xAxis161[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn586708__107 = new TH1D("PDFSgn586708__107","",2, xAxis161);
   PDFSgn586708__107->SetBinContent(2,1);
   PDFSgn586708__107->SetBinError(2,0.003312678);
   PDFSgn586708__107->SetEntries(119361);
   PDFSgn586708__107->SetDirectory(0);
   PDFSgn586708__107->SetStats(0);
   PDFSgn586708__107->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn586708__107->SetLineColor(ci);
   PDFSgn586708__107->GetXaxis()->SetNdivisions(508);
   PDFSgn586708__107->GetXaxis()->SetLabelFont(42);
   PDFSgn586708__107->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn586708__107->GetXaxis()->SetTitleSize(0.05);
   PDFSgn586708__107->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn586708__107->GetXaxis()->SetTitleFont(42);
   PDFSgn586708__107->GetYaxis()->SetNdivisions(508);
   PDFSgn586708__107->GetYaxis()->SetLabelFont(42);
   PDFSgn586708__107->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn586708__107->GetYaxis()->SetTitleSize(0.05);
   PDFSgn586708__107->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn586708__107->GetYaxis()->SetTitleFont(42);
   PDFSgn586708__107->GetZaxis()->SetNdivisions(508);
   PDFSgn586708__107->GetZaxis()->SetLabelFont(42);
   PDFSgn586708__107->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn586708__107->GetZaxis()->SetTitleSize(0.05);
   PDFSgn586708__107->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn586708,"hist");
   Double_t xAxis162[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg586708__108 = new TH1D("PDFBkg586708__108","",2, xAxis162);
   PDFBkg586708__108->SetBinContent(2,1);
   PDFBkg586708__108->SetBinError(2,0.003791618);
   PDFBkg586708__108->SetEntries(105134);
   PDFBkg586708__108->SetDirectory(0);
   PDFBkg586708__108->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg586708__108->SetFillColor(ci);
   PDFBkg586708__108->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg586708__108->SetLineColor(ci);
   PDFBkg586708__108->GetXaxis()->SetNdivisions(508);
   PDFBkg586708__108->GetXaxis()->SetLabelFont(42);
   PDFBkg586708__108->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg586708__108->GetXaxis()->SetTitleSize(0.05);
   PDFBkg586708__108->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg586708__108->GetXaxis()->SetTitleFont(42);
   PDFBkg586708__108->GetYaxis()->SetNdivisions(508);
   PDFBkg586708__108->GetYaxis()->SetLabelFont(42);
   PDFBkg586708__108->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg586708__108->GetYaxis()->SetTitleSize(0.05);
   PDFBkg586708__108->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg586708__108->GetYaxis()->SetTitleFont(42);
   PDFBkg586708__108->GetZaxis()->SetNdivisions(508);
   PDFBkg586708__108->GetZaxis()->SetLabelFont(42);
   PDFBkg586708__108->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg586708__108->GetZaxis()->SetTitleSize(0.05);
   PDFBkg586708__108->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg586708,"hist");
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
   TLegendEntry *entry=leg->AddEntry("PDFSgn586708"," tHq ","f");
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
   entry=leg->AddEntry("PDFBkg586708"," bkg ","f");

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
