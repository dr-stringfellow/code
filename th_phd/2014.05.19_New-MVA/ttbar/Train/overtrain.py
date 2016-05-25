import argparse
import ROOT as r
from ROOT import gPad, TLegend

r.gROOT.SetBatch()

parser = argparse.ArgumentParser(description='Plot TMVA training and testing.')
parser.add_argument('file', metavar='ROOTFILE', type=str, nargs=1,
        help='a TMVA output file to plot')
parser.add_argument('output', metavar='OUTFILE', type=str, nargs=1,
        help='the PDF file to produce')
args = parser.parse_args()

f = r.TFile(args.file[0])

sigs = {}
bkgs = {}

for k in f.Get("Method_MLP/GlobalTTbarReco_BFGS").GetListOfKeys():
    name = k.GetName()
    if name.endswith('__Signal'):
        var = name.replace('__Signal', '')
        sigs[var] = k.ReadObj()
    elif name.endswith('__Background'):
        var = name.replace('__Background', '')
        bkgs[var] = k.ReadObj()
    else:
        print name

assert sorted(sigs.keys()) == sorted(bkgs.keys())

c = r.TCanvas("canvas","canvas", 900, 800)

c.Divide(1,1)
c.cd(1)

gPad.SetBottomMargin(0.13)
gPad.SetLeftMargin(0.13)
gPad.SetTopMargin(0.13)

train_s = f.Get("Method_MLP/GlobalTTbarReco_BFGS/MVA_GlobalTTbarReco_BFGS_Train_S")
train_b = f.Get("Method_MLP/GlobalTTbarReco_BFGS/MVA_GlobalTTbarReco_BFGS_Train_B")

train_s.SetMaximum(1.1 * max(train_s.GetMaximum(), train_b.GetMaximum()))

train_s.SetLineColor(r.kRed)
train_s.SetLineWidth(2)
train_s.SetFillColor(r.kRed)
train_s.SetFillStyle(3004)
train_s.SetTitle("")
train_s.GetXaxis().SetTitle("NN output")
train_s.GetXaxis().SetTitleOffset(1.2)
#train_s.GetXaxis().SetLabelOffset(0.3)
train_s.GetXaxis().SetTitleSize(0.05)
train_s.GetXaxis().SetLabelSize(0.04)
train_s.GetYaxis().SetTitle("Norm. to unit area")
train_s.GetYaxis().SetTitleOffset(1.2)
#train_s.GetYaxis().SetLabelOffset(0.3)
train_s.GetYaxis().SetTitleSize(0.05)
train_s.GetYaxis().SetLabelSize(0.04)

train_s.SetStats(0)

train_s.Draw("HIST")

train_b.SetLineColor(r.kBlue)
train_b.SetLineWidth(2)
train_b.SetFillColor(r.kBlue)
train_b.SetFillStyle(0)
train_b.Draw("HIST SAME")

test_s = f.Get("Method_MLP/GlobalTTbarReco_BFGS/MVA_GlobalTTbarReco_BFGS_S")
test_b = f.Get("Method_MLP/GlobalTTbarReco_BFGS/MVA_GlobalTTbarReco_BFGS_B")

test_b.SetLineColor(r.kBlue+1)
test_b.SetLineColor(r.kBlue+1)
test_b.SetMarkerColor(r.kBlue+1)
test_b.SetMarkerSize(r.kBlue+1)
test_b.SetLineWidth(2)

test_s.SetLineColor(r.kRed+1)
test_s.SetLineColor(r.kRed+1)
test_s.SetMarkerColor(r.kRed+1)
test_s.SetMarkerSize(r.kRed+1)
test_s.SetLineWidth(2)

test_s.Draw("SAME")
test_b.Draw("SAME")

l = TLegend(0.15,0.89,0.45,0.99)
l.SetFillStyle(0)
l.SetLineColor(10)
l.SetBorderSize(0)
l.SetTextAlign(11)
l.SetTextFont(42)
l.SetTextSize(0.045)

l.AddEntry(train_s,"correct train","f")
l.AddEntry(train_b,"wrong train","f")
l.Draw()

l2 = TLegend(0.5,0.89,0.9,0.99)
l2.SetFillStyle(0)
l2.SetLineColor(10)
l2.SetBorderSize(0)
l2.SetTextAlign(11)
l2.SetTextFont(42)
l2.SetTextSize(0.045)

l2.AddEntry(test_s,"correct test","lep")
l2.AddEntry(test_b,"wrong test","lep")
l2.Draw()

c.SaveAs(args.output[0])

