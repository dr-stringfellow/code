{
//=========Macro generated from canvas: canvas/canvas
//=========  (Sun Sep 27 19:43:54 2015) by ROOT version5.34/32
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
   hStack->SetTitle(";tagged(b_{thad}) [t#bar{t}];Probability density [a. u.]");
   hStack->SetMinimum(0);
   hStack->SetMaximum(1.05);
   Double_t xAxis163[3] = {-0.5, 0.5, 1.5}; 
   
   TH1F *hStack_stack_55 = new TH1F("hStack_stack_55","",2, xAxis163);
   hStack_stack_55->SetMinimum(0);
   hStack_stack_55->SetMaximum(1.05);
   hStack_stack_55->SetDirectory(0);
   hStack_stack_55->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   hStack_stack_55->SetLineColor(ci);
   hStack_stack_55->SetLineWidth(2);
   hStack_stack_55->GetXaxis()->SetTitle("tagged(b_{thad}) [t#bar{t}]");
   hStack_stack_55->GetXaxis()->SetNdivisions(508);
   hStack_stack_55->GetXaxis()->SetLabelFont(42);
   hStack_stack_55->GetXaxis()->SetLabelOffset(0.007);
   hStack_stack_55->GetXaxis()->SetTitleSize(0.05);
   hStack_stack_55->GetXaxis()->SetTitleOffset(0.9);
   hStack_stack_55->GetXaxis()->SetTitleFont(42);
   hStack_stack_55->GetYaxis()->SetTitle("Probability density [a. u.]");
   hStack_stack_55->GetYaxis()->SetNdivisions(508);
   hStack_stack_55->GetYaxis()->SetLabelFont(42);
   hStack_stack_55->GetYaxis()->SetLabelOffset(0.007);
   hStack_stack_55->GetYaxis()->SetTitleSize(0.05);
   hStack_stack_55->GetYaxis()->SetTitleOffset(1.25);
   hStack_stack_55->GetYaxis()->SetTitleFont(42);
   hStack_stack_55->GetZaxis()->SetNdivisions(508);
   hStack_stack_55->GetZaxis()->SetLabelFont(42);
   hStack_stack_55->GetZaxis()->SetLabelOffset(0.007);
   hStack_stack_55->GetZaxis()->SetTitleSize(0.05);
   hStack_stack_55->GetZaxis()->SetTitleFont(42);
   hStack->SetHistogram(hStack_stack_55);
   
   Double_t xAxis164[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFSgn473416__109 = new TH1D("PDFSgn473416__109","",2, xAxis164);
   PDFSgn473416__109->SetBinContent(2,1);
   PDFSgn473416__109->SetBinError(2,0.003312678);
   PDFSgn473416__109->SetEntries(119361);
   PDFSgn473416__109->SetDirectory(0);
   PDFSgn473416__109->SetStats(0);
   PDFSgn473416__109->SetFillColor(38);

   ci = TColor::GetColor("#0000ff");
   PDFSgn473416__109->SetLineColor(ci);
   PDFSgn473416__109->GetXaxis()->SetNdivisions(508);
   PDFSgn473416__109->GetXaxis()->SetLabelFont(42);
   PDFSgn473416__109->GetXaxis()->SetLabelOffset(0.007);
   PDFSgn473416__109->GetXaxis()->SetTitleSize(0.05);
   PDFSgn473416__109->GetXaxis()->SetTitleOffset(0.9);
   PDFSgn473416__109->GetXaxis()->SetTitleFont(42);
   PDFSgn473416__109->GetYaxis()->SetNdivisions(508);
   PDFSgn473416__109->GetYaxis()->SetLabelFont(42);
   PDFSgn473416__109->GetYaxis()->SetLabelOffset(0.007);
   PDFSgn473416__109->GetYaxis()->SetTitleSize(0.05);
   PDFSgn473416__109->GetYaxis()->SetTitleOffset(1.25);
   PDFSgn473416__109->GetYaxis()->SetTitleFont(42);
   PDFSgn473416__109->GetZaxis()->SetNdivisions(508);
   PDFSgn473416__109->GetZaxis()->SetLabelFont(42);
   PDFSgn473416__109->GetZaxis()->SetLabelOffset(0.007);
   PDFSgn473416__109->GetZaxis()->SetTitleSize(0.05);
   PDFSgn473416__109->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFSgn473416,"hist");
   Double_t xAxis165[3] = {-0.5, 0.5, 1.5}; 
   
   TH1D *PDFBkg473416__110 = new TH1D("PDFBkg473416__110","",2, xAxis165);
   PDFBkg473416__110->SetBinContent(2,1);
   PDFBkg473416__110->SetBinError(2,0.003791618);
   PDFBkg473416__110->SetEntries(105134);
   PDFBkg473416__110->SetDirectory(0);
   PDFBkg473416__110->SetStats(0);

   ci = TColor::GetColor("#ff0000");
   PDFBkg473416__110->SetFillColor(ci);
   PDFBkg473416__110->SetFillStyle(3354);

   ci = TColor::GetColor("#ff0000");
   PDFBkg473416__110->SetLineColor(ci);
   PDFBkg473416__110->GetXaxis()->SetNdivisions(508);
   PDFBkg473416__110->GetXaxis()->SetLabelFont(42);
   PDFBkg473416__110->GetXaxis()->SetLabelOffset(0.007);
   PDFBkg473416__110->GetXaxis()->SetTitleSize(0.05);
   PDFBkg473416__110->GetXaxis()->SetTitleOffset(0.9);
   PDFBkg473416__110->GetXaxis()->SetTitleFont(42);
   PDFBkg473416__110->GetYaxis()->SetNdivisions(508);
   PDFBkg473416__110->GetYaxis()->SetLabelFont(42);
   PDFBkg473416__110->GetYaxis()->SetLabelOffset(0.007);
   PDFBkg473416__110->GetYaxis()->SetTitleSize(0.05);
   PDFBkg473416__110->GetYaxis()->SetTitleOffset(1.25);
   PDFBkg473416__110->GetYaxis()->SetTitleFont(42);
   PDFBkg473416__110->GetZaxis()->SetNdivisions(508);
   PDFBkg473416__110->GetZaxis()->SetLabelFont(42);
   PDFBkg473416__110->GetZaxis()->SetLabelOffset(0.007);
   PDFBkg473416__110->GetZaxis()->SetTitleSize(0.05);
   PDFBkg473416__110->GetZaxis()->SetTitleFont(42);
   hStack->Add(PDFBkg473416,"hist");
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
   TLegendEntry *entry=leg->AddEntry("PDFSgn473416"," tHq ","f");
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
