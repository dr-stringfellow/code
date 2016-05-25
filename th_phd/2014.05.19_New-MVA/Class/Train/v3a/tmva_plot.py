import argparse
import ROOT as r

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

for k in f.Get("Method_MLP/THvsBkg_Global_BFGS").GetListOfKeys():
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

c = r.TCanvas()
c.Divide(2, 2)
c.cd(1)

f.Get("Method_MLP/THvsBkg_Global_BFGS/MVA_THvsBkg_Global_BFGS_rejBvsS").Draw()

c.cd(2)

train_s = f.Get("Method_MLP/THvsBkg_Global_BFGS/MVA_THvsBkg_Global_BFGS_Train_S")
train_b = f.Get("Method_MLP/THvsBkg_Global_BFGS/MVA_THvsBkg_Global_BFGS_Train_B")

train_s.SetMaximum(1.1 * max(train_s.GetMaximum(), train_b.GetMaximum()))

train_s.SetLineColor(r.kBlue)
train_s.SetFillColor(r.kBlue)
train_s.SetFillStyle(3004)
train_s.Draw("HIST")

train_b.SetLineColor(r.kRed)
train_b.SetFillColor(r.kRed)
train_b.SetFillStyle(3005)
train_b.Draw("HIST SAME")

test_s = f.Get("Method_MLP/THvsBkg_Global_BFGS/MVA_THvsBkg_Global_BFGS_S")
test_b = f.Get("Method_MLP/THvsBkg_Global_BFGS/MVA_THvsBkg_Global_BFGS_B")

test_s.Draw("SAME")
test_b.Draw("SAME")

c.cd(3)

f.Get("CorrelationMatrixS").Draw("COLZ TEXT")

c.cd(4)

f.Get("CorrelationMatrixB").Draw("COLZ TEXT")

c.SaveAs(args.output[0])

