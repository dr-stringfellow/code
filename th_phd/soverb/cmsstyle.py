
from ROOT import TCanvas, TColor, gStyle, TStyle, TLatex, PyConfig, gROOT

def style():
    gROOT.SetStyle("Plain")
    gStyle.SetPalette(1)
    gStyle.SetPaintTextFormat("1.4f")

    #  gStyle.SetTextSize(6)

    # For the canvas:
    gStyle.SetCanvasBorderMode(0)
    gStyle.SetCanvasColor(TColor.kWhite)
    gStyle.SetCanvasDefH(600) #Height of canvas
    gStyle.SetCanvasDefW(600) #Width of canvas
    gStyle.SetCanvasDefX(0)   #POsition on screen
    gStyle.SetCanvasDefY(0)

    # For the Pad:
    gStyle.SetPadBorderMode(0)
    # gStyle.SetPadBorderSize(Width_t size = 1)
    gStyle.SetPadColor(TColor.kWhite)
    gStyle.SetPadGridX(False)
    gStyle.SetPadGridY(False)
    gStyle.SetGridColor(0)
    gStyle.SetGridStyle(3)
    gStyle.SetGridWidth(1)

    # For the frame:
    gStyle.SetFrameBorderMode(0)
    gStyle.SetFrameBorderSize(2)
    gStyle.SetFrameFillColor(0)
    gStyle.SetFrameFillStyle(0)
    gStyle.SetFrameLineColor(1)
    gStyle.SetFrameLineStyle(1)
    gStyle.SetFrameLineWidth(1)

    # For the histo:
    # gStyle.SetHistFillColor(1)
    # gStyle.SetHistFillStyle(0)
    gStyle.SetHistLineColor(1)
    gStyle.SetHistLineStyle(0)
    gStyle.SetHistLineWidth(1)
    # gStyle.SetLegoInnerR(Float_t rad = 0.5)
    # gStyle.SetNumberContours(Int_t number = 20)

    gStyle.SetEndErrorSize(2)
    # gStyle.SetErrorMarker(20)
    # gStyle.SetErrorX(0.)

    gStyle.SetMarkerStyle(20)
    #For the fit/function:
    # gStyle.SetOptFit(0)
    # gStyle.SetFitFormat("5.4g")
    gStyle.SetFuncColor(2)
    gStyle.SetFuncStyle(1)
    gStyle.SetFuncWidth(1)
    #For the date:
    gStyle.SetOptDate(0)
    # gStyle.SetDateX(Float_t x = 0.01)
    # gStyle.SetDateY(Float_t y = 0.01)

    # For the statistics box:
    gStyle.SetOptFile(0)
    gStyle.SetOptStat("mr") # To display the mean and RMS:   SetOptStat("mr")
    gStyle.SetStatColor(TColor.kWhite)
    gStyle.SetStatFont(42)
    gStyle.SetStatFontSize(0.05)
    gStyle.SetStatTextColor(1)
    gStyle.SetStatFormat("5.4g")
    gStyle.SetStatBorderSize(1)
    gStyle.SetStatH(0.5)
    gStyle.SetStatW(0.35)
    # gStyle.SetStatStyle(Style_t style = 1001)
    # gStyle.SetStatX(Float_t x = 0)
    # gStyle.SetStatY(Float_t y = 0)
    
    # Margins:
    gStyle.SetPadTopMargin(0.05)
    gStyle.SetPadBottomMargin(0.13)
    gStyle.SetPadLeftMargin(0.13)
    gStyle.SetPadRightMargin(0.02)

    # For the Global title:
    gStyle.SetOptTitle(0)
    gStyle.SetTitleFont(42)
    gStyle.SetTitleColor(1)
    gStyle.SetTitleTextColor(1)
    gStyle.SetTitleFillColor(10)
    gStyle.SetTitleFontSize(0.05)
    # gStyle.SetTitleH(0) # Set the height of the title box
    # gStyle.SetTitleW(0) # Set the width of the title box
    # gStyle.SetTitleX(0) # Set the position of the title box
    # gStyle.SetTitleY(0.985) # Set the position of the title box
    # gStyle.SetTitleStyle(Style_t style = 1001)
    # gStyle.SetTitleBorderSize(2)
    
    # For the axis titles:
    gStyle.SetTitleColor(1, "XYZ")
    gStyle.SetTitleFont(43, "XYZ")
#    gStyle.SetTitleSize(0.06, "XYZ")
    gStyle.SetTitleSize(48, "XYZ")
    #    gStyle.SetTitleXSize(0.06) # Another way to set the size?
    #    gStyle.SetTitleYSize(0.06)
    

    gStyle.SetTitleOffset(1.1,"X");
    gStyle.SetTitleOffset(1.1,"Y");
    gStyle.SetTitleOffset(0.9,"Z");
    
    gStyle.SetLabelOffset(0.004,"X");
    gStyle.SetLabelOffset(0.015,"Y");
    gStyle.SetLabelOffset(0.015,"Z");

                



    # For the axis labels:
    gStyle.SetLabelColor(1, "XYZ")
    gStyle.SetLabelFont(43, "XYZ")
    gStyle.SetLabelSize(40, "XYZ")
    
    # For the axis:
    gStyle.SetAxisColor(1, "XYZ")
    gStyle.SetStripDecimals(True)
    gStyle.SetTickLength(0.03, "XYZ")
    gStyle.SetNdivisions(505, "YZ")
    gStyle.SetNdivisions(310, "X")
    gStyle.SetPadTickX(1)  # To get tick marks on the opposite side of the frame
    gStyle.SetPadTickY(1)

    # Change for log plots:
    gStyle.SetOptLogx(0)
    gStyle.SetOptLogy(0)
    gStyle.SetOptLogz(0)

    # Postscript options:
    gStyle.SetPaperSize(20.,20.)
    # gStyle.SetLineScalePS(Float_t scale = 3)
    # gStyle.SetLineStyleString(Int_t i, const char* text)
    # gStyle.SetHeaderPS(const char* header)
    
    gStyle.SetPadTopMargin   (0.06);
    gStyle.SetPadBottomMargin(0.16);
    gStyle.SetPadLeftMargin  (0.15);
    gStyle.SetPadRightMargin (0.04);
        
