import argparse
import ROOT as r
from array import *


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

for k in f.Get("Method_MLP/GlobalTHReco_BFGS").GetListOfKeys():
    name = k.GetName()
    if name.endswith('__Signal'):
        var = name.replace('__Signal', '')
        sigs[var] = k.ReadObj()
        print name
    elif name.endswith('__Background'):
        var = name.replace('__Background', '')
        bkgs[var] = k.ReadObj()
#    else:
#       print name


assert sorted(sigs.keys()) == sorted(bkgs.keys())

c = r.TCanvas()
#c.Divide(2, 2)

#c.SaveAs(args.output[0] + "(")

while len(sigs.keys()) > 0:


    for i in range(0,  len(sigs.keys())):
        t = r.TCanvas()
        t.cd()
        
        k = sigs.keys()[0]
        s = sigs.pop(k)
        b = bkgs.pop(k)
        
        print k
        print i



            
            
        #new_bins = array('d',[-9.98740196e-01,0,1])
        
        #s_new=s.Rebin(2,"s_new",new_bins)#,new_bins)
        #b_new=b.Rebin(2,"b_new",new_bins)#,new_bins)

        if k=="Charge_BTop":
            s_new=s.Rebin(2)
            b_new=b.Rebin(2)

            s_new.GetXaxis().SetLimits(-1.,1.)
            b_new.GetXaxis().SetLimits(-1.,1.)
            s_new.GetXaxis().SetTitle("Lalala")
        if k=="log_MinPt_BHiggs_":
            s_new=s.Rebin(2)
            b_new=b.Rebin(2)

            s_new.GetXaxis().SetLimits(3.,5.90750933)
            b_new.GetXaxis().SetLimits(3,5.90750933)
            s_new.GetXaxis().SetTitle("Lalala")
        if k=="PassBTag_BTop":
            new_bins = array('d',[-0.1,0.5,1.1])
            s_new = s.Rebin(20)#,"s_new",new_bins)
            b_new = b.Rebin(20)#,"b_new",new_bins)
            s_new.GetXaxis().SetLimits(-1.,1.)
            b_new.GetXaxis().SetLimits(-1.,1.)
            s_new.GetXaxis().SetTitle("Lalala")
        if k=="NumBTag_Higgs":
            new_bins = array('d',[-1.5,-0.5,0.5,1.5])
            s_new = s.Rebin(3,"s_new",new_bins)
            b_new = b.Rebin(3,"b_new",new_bins)
            s_new.GetXaxis().SetLimits(-1.5,1.5)
            b_new.GetXaxis().SetLimits(-1.5,1.5)
            s_new.GetXaxis().SetTitle("Lalala")
     
        
        s_new.Scale(1. / s_new.Integral())
        b_new.Scale(1. / b_new.Integral())
        
        s_new.SetMaximum(1.1 * max(s_new.GetMaximum(), b_new.GetMaximum()))
        
        
        s_new.SetLineColor(r.kBlue)
        s_new.SetFillColor(r.kBlue)
        s_new.SetFillStyle(3004)
        
        b_new.SetLineColor(r.kRed)
        b_new.SetFillColor(r.kRed)
        b_new.SetFillStyle(3005)
        
        
        

        
        #s.DrawCopy()
        #b.DrawCopy("SAME")
        s_new.DrawCopy()
        b_new.DrawCopy("SAME")

        t.SaveAs(k+'_'+args.output[0])

#c.SaveAs(args.output[0] + "]")
