#!/usr/bin/env python

'''

Produce S/B plot post-fit

'''

from RecoLuminosity.LumiDB import argparse
import math
import fnmatch
import logging
import os
import ctypes
import ROOT
from ROOT import TFile, TAttMarker, TCanvas, TColor, gStyle, TStyle, TLegend, TLatex, TH1F, PyConfig, gROOT, TGraph, TGraphAsymmErrors, TMultiGraph, TEllipse, TApplication, PyROOT, THStack, TPad, gPad, RooHistError
import shutil
import sys
from array import array
import numpy as np
from cmsstyle import style

PyConfig.IgnoreCommandLineOptions = True

def plot(inputfit, output, inputdata, bins, mcunc_on_bck):
    gStyle.SetOptStat(0)
    gStyle.SetPalette(1)
    gStyle.SetErrorX(0);
    
    style()

    i_hist_sig = []
    i_hist_bkg = []
    i_hist_data = []
    reg_list = []

    for i in inputdata:
        print i.GetName()
        reg_list.append(i.GetName().replace('.root','').replace('tH_',''))
                        
    print reg_list

    for keys in inputfit.GetListOfKeys():
        #ifile.GetListOfKeys().Print()
        if '__tH' in keys.GetName():
            print "To signal", keys.GetName()
            i_hist_sig.append(keys.GetName())
        elif '__data_obs__' in keys.GetName():
            print "To data", keys.GetName()
            i_hist_data.append(keys.GetName())
        else :
            print "To background", keys.GetName()
            i_hist_bkg.append(keys.GetName())

    sb = []
    s = []
    b = []
    d = []
    err = []
    errb = []
    
    for reg in reg_list:
        for i in inputdata:
            if reg in i.GetName():
                data = i
        datahist=data.Get(data.GetName().replace('.root','__data_obs__nominal'))
        print "Integral", datahist.Integral()
        
        for i in range(1,16):
            s_tmp = 0
            b_tmp = 0
            d_tmp = 0
            err_tmp = 0
            errb_tmp = 0
            for j in i_hist_sig:
                h_tmp = inputfit.Get(j)
                if reg in inputfit.Get(j).GetName():
                    s_tmp = s_tmp + h_tmp.GetBinContent(i)
                    err_tmp = math.sqrt(err_tmp*err_tmp + h_tmp.GetBinError(i)*h_tmp.GetBinError(i))
            for j in i_hist_bkg:
                h_tmp = inputfit.Get(j)
                if reg in inputfit.Get(j).GetName():
                    b_tmp = b_tmp + h_tmp.GetBinContent(i)
                    err_tmp = math.sqrt(err_tmp*err_tmp + h_tmp.GetBinError(i)*h_tmp.GetBinError(i))
                    errb_tmp = math.sqrt(errb_tmp*errb_tmp + h_tmp.GetBinError(i)*h_tmp.GetBinError(i))
            d_tmp = datahist.GetBinContent(i)
            sb.append(math.log(s_tmp/b_tmp))
            err.append(err_tmp)
            errb.append(errb_tmp)
            s.append(s_tmp)
            b.append(b_tmp)
            d.append(d_tmp)

    print sb
    print s
    print b
    print d
    xmin = sb[0]
    xmax = sb[0]
    for i in range(0,len(sb)):
        if xmin > sb[i] and d[i] > 0 and b[i] > 0:
            xmin = sb[i]
        if xmax < sb[i] and d[i] > 0 and b[i] > 0:
            xmax = sb[i] + 0.1

    if bins > 0 :
        nbinsx = bins
    else :
        nbinsx = 10
        
    print xmin
    print xmax
    
    h_s = TH1F("h_s","h_s", nbinsx, xmin, xmax)
    h_b = TH1F("h_b","h_b", nbinsx, xmin, xmax)
    h_d = TH1F("h_d","h_d", nbinsx, xmin, xmax)
    h_err = TH1F("h_err","h_err", nbinsx, xmin, xmax)

    for i in range(0,len(sb)):
        h_s.SetBinContent(h_s.FindBin(sb[i]),h_s.GetBinContent(h_s.FindBin(sb[i]))+s[i])
        h_b.SetBinContent(h_b.FindBin(sb[i]),h_b.GetBinContent(h_b.FindBin(sb[i]))+b[i])
        h_d.SetBinContent(h_d.FindBin(sb[i]),h_d.GetBinContent(h_d.FindBin(sb[i]))+d[i])
        if mcunc_on_bck:
            h_err.SetBinContent(h_err.FindBin(sb[i]),h_err.GetBinContent(h_err.FindBin(sb[i]))+b[i])
            h_err.SetBinError(h_err.FindBin(sb[i]),math.sqrt(h_err.GetBinError(h_err.FindBin(sb[i]))*h_err.GetBinError(h_err.FindBin(sb[i]))+errb[i]*errb[i]))
        else :
            h_err.SetBinContent(h_err.FindBin(sb[i]),h_err.GetBinContent(h_err.FindBin(sb[i]))+b[i]+s[i])
            h_err.SetBinError(h_err.FindBin(sb[i]),math.sqrt(h_err.GetBinError(h_err.FindBin(sb[i]))*h_err.GetBinError(h_err.FindBin(sb[i]))+err[i]*err[i]))
    for i in range(1,h_err.GetNbinsX()+1):
        print i, "err",h_err.GetBinContent(i)
        print "errerr",h_err.GetBinError(i)


    c = TCanvas("canvas","canvas", 800, 800)
    #c.SetLogy()
    
    #mainPad = TPad()
    #mainPad = gPad

    #
    #PREPARE RESIDUALS
    #
    topMargin    = c.GetTopMargin()
    bottomMargin = c.GetBottomMargin()
    leftMargin   = c.GetLeftMargin()
    rightMargin  = c.GetRightMargin()
    c.Divide(1,2,0.0,0.0)
    
    c.GetPad(1).SetFillColor(0)
    c.GetPad(2).SetFillColor(0)
    
    c.GetPad(1).SetPad(0.0,0.25,1.0,1.0)
    c.GetPad(2).SetPad(0.0,0.0,1.0,0.25)
    
    c.cd(1)
    gPad.SetLogy()
    gPad.SetTopMargin(topMargin/0.75)
    gPad.SetLeftMargin(leftMargin)
    gPad.SetRightMargin(rightMargin)
    
    c.cd(2)
    gPad.SetBottomMargin(0.480)
    gPad.SetLeftMargin(leftMargin)
    gPad.SetRightMargin(rightMargin)
    gPad.SetFillStyle(0)

 #   c.cd(1)


#    gPad.SetRightMargin(0.18)
#    gPad.SetTopMargin(0.07)
    
    h_s.SetLineColor(2)
    h_b.SetLineColor(13)
    h_d.SetLineColor(1)
    h_s.SetFillColor(2)
    h_b.SetFillColor(13)
    h_d.SetMarkerStyle(20)
    h_d.SetMarkerSize(1.2)


    hs = THStack("hs","")
    hs.Add(h_b)

    if not mcunc_on_bck :
        hs.Add(h_s)

            
    hs.SetMinimum(0.02)
    maximum=hs.GetMaximum()
    hs.SetMaximum(13*maximum)

    c.cd(1)
    hres = h_d.Clone()
    
    mchistos = [ h_s, h_b] 
    hres.Reset()
    
    hres.GetYaxis().CenterTitle()
    #hres.GetYaxis().SetTitleOffset(139)
    hres.SetStats(0)
    hres.GetYaxis().SetTitleSize(24)
    hres.GetYaxis().SetTitleOffset(2.2)
    hres.GetYaxis().SetLabelSize(24)
    hres.GetYaxis().SetNdivisions(505)
    hres.GetXaxis().SetTitleSize(42)
    hres.GetXaxis().SetTickLength(hres.GetXaxis().GetTickLength() * 3.0 )
    hres.SetYTitle("#frac{Data-Bkg.}{Bkg.}")
    hres.SetXTitle("log_{10}(S/B)") 
    hres.GetXaxis().SetTitleOffset(144)


    resdata = TGraphAsymmErrors(0)
    resdata.Set(0)
    
    for ibin in range(1, hres.GetNbinsX()+1): 
        ndata  = h_d.GetBinContent(ibin)
        nmc    = hs.GetStack().Last().GetBinContent(ibin)
        edata  = h_d.GetBinError(ibin)
        if nmc>0 :
            bin  = (ndata-nmc)/nmc
            err  = edata/nmc
            if ndata!=0 :
                hres.SetBinContent(ibin,bin)
                hres.SetBinError(ibin,err)
            else :
                hres.SetBinContent(ibin,-999)
                hres.SetBinError(ibin,0)

            y=ndata
            nSigma = 1.
            ym = ROOT.Double(0)
            yp = ROOT.Double(0)
            RooHistError.instance().getPoissonInterval(int(y),ym,yp,nSigma)
            yerrplus = yp - y
            yerrminus = y - ym
            xbin = hres.GetBinCenter(ibin)
            resdata.SetPoint(ibin-1,xbin,bin)
            resdata.SetPointEYlow(ibin-1,(yerrminus/nmc))
            resdata.SetPointEYhigh(ibin-1,(yerrplus/nmc))
        else : 
            hres.SetBinContent(ibin,1.0);
            hres.SetBinError(ibin,1.0);
        print hres.GetBinContent(ibin),
    mymax=max(abs(hres.GetMinimum()),
              abs(hres.GetMaximum()))
    mymax *=1.05; mymax = min(49.9,max)
    resmax = 1.2
    if resmax>0. :
        mymax = resmax
    hres.SetMinimum(-1.0*mymax)
    hres.SetMaximum(1.2*mymax)
    hres.SetMarkerStyle(20)
    hres.SetMarkerSize(0.5)
    hres.SetTitleOffset(4)
       
    herr2 =  h_s.Clone("herr")
    herr2.Reset() 

    if mcunc_on_bck:
        mchistos = [ h_b] 
    
    
    
    for i in range(0,len(mchistos)):
        for ibin in range(1,mchistos[i].GetNbinsX()+1) :
            valA  = herr2.GetBinContent(ibin)
            evalA = herr2.GetBinError(ibin)
            valB  = mchistos[i].GetBinContent(ibin)
            evalB = mchistos[i].GetBinError(ibin)
            herr2.SetBinContent(ibin,(valA+valB))
            herr2.SetBinError(ibin,math.sqrt(evalA*evalA+evalB*evalB))
            print 'Here1a ' + str(herr2.GetBinError(ibin))
            print 'Here1b ' + str(herr2.GetBinContent(ibin))
       
            if (i==len(mchistos)-1) and (herr2.GetBinContent(ibin)):
                herr2.SetBinContent(ibin,0.)
 #               herr2.SetBinError(ibin,(math.sqrt(evalA*evalA+evalB*evalB)/(valA+valB)))
                herr2.SetBinError(ibin,(math.sqrt(evalA*evalA+evalB*evalB)/hs.GetStack().Last().GetBinContent(ibin)))
                print 'Here2 ' + str(herr2.GetBinError(ibin))
     
                
    herr2.SetFillColor(TColor.kMagenta-9)
    herr2.SetFillStyle(3001)
    herr2.SetMarkerStyle(1)
    

    c.cd(2)

    hres.SetMarkerSize(0)
    gPad.SetGridy()
    hres.Draw("AXIS")
    hres.Draw("AXIG same")
    herr2.Draw("error sameE2")
    resdata.SetMarkerStyle(20)
    resdata.SetMarkerSize(0.5)
    gStyle.SetEndErrorSize(6)
    resdata.Draw("P0")
    resdata.Draw("||")
    

    kstest = h_b.KolmogorovTest(h_d)
    chi2 = h_b.Chi2Test(h_d)
    print kstest

    # KS +chi2 test
    
    label_kstest = TLatex()
    label_kstest.SetNDC(1)
    label_kstest.SetTextAlign(12)
    label_kstest.SetTextSize(0.10)
    label_kstest.SetTextFont(42)
    label_kstest.DrawLatex(0.18,0.875,"KS test = %3.2f    #chi^{2} = %3.2f" % (kstest,chi2))


    # Stat.+syst. label

    l_res = TLegend(0.48,0.81,0.63,0.94)
    l_res.SetTextSize(0.10)
    l_res.SetFillColor(0)
    l_res.SetFillStyle(0)
    l_res.SetLineColor(10) 
    l_res.SetBorderSize(0)

    l_res.AddEntry(herr2,"#bf{Bkg. stat.+sys.}","F")
    l_res.Draw()

    c.cd(1)
      
    h_err.SetFillColor(TColor.kBlack)
    h_err.SetFillStyle(3245)
    h_err.SetMarkerStyle(1)
    # l.AddEntry(herr,"MC stat.","F")
    if mcunc_on_bck :
        hs.Add(h_s)
       
    hs.Draw()
    hs.GetXaxis().SetTitle("log_{10}(S/B)")
    hs.GetYaxis().SetTitle("Events/Bin")
    hs.GetYaxis().SetTitleSize(44)
    hs.GetXaxis().SetTitleSize(42)
#
#
# UNCOMMENT TO PUT ERRORS ON MCSTACK!! (IF YOU WANT :) )
#
#
#    h_err.Draw("error E2same")
    hs.GetXaxis().SetTitleOffset(1.2)
    hs.GetYaxis().SetTitleOffset(1.3)
    
    #print size
    stats =TLatex()
    stats.SetNDC(1)
    stats.SetTextAlign(12)
    stats.SetTextSize(0.06)
    stats.SetTextFont(42)
    
    stats.DrawLatex(0.69,0.960,"20 fb^{-1} (8 TeV)")
    stats.DrawLatex(0.30,0.955,"#it{Preliminary}")
    stats.DrawLatex(0.18,0.86,"3+4 tag regions (e+#mu)")
    stats.DrawLatex(0.18,0.79,"m_{H}=125 GeV, C_{t}=-1")
    
    stats2 =TLatex()
    stats2.SetNDC(1)
    stats2.SetTextAlign(12)
    stats2.SetTextSize(0.07)
    stats2.SetTextFont(42)
    stats2.DrawLatex(0.18,0.965,"#bf{CMS}")
    #c.Update()


    gdata = TGraphAsymmErrors(h_d)
    for i  in range(0,gdata.GetN()):
        y=gdata.GetY()[i]
        nSigma = 1.
        ym = ROOT.Double(0)
        yp = ROOT.Double(0)
        RooHistError.instance().getPoissonInterval(int(y),ym,yp,nSigma)
        yerrplus = yp - y
        yerrminus = y - ym
        gdata.SetPointEYlow(i,yerrminus)
        gdata.SetPointEYhigh(i,yerrplus)
        gdata.SetPointEXlow(i,0)
        gdata.SetPointEXhigh(i,0)

        
    gdata.Draw("PZ");

    l = TLegend(0.705,0.69,0.87,0.89)
    l.SetFillStyle(0)
    l.SetLineColor(10)
    l.SetBorderSize(0)
    l.SetTextAlign(11)
    l.SetTextFont(42)
    l.SetTextSize(0.055)
    
    l.AddEntry(h_d,"Data","lp")
    #l.AddEntry(h_s,"tHq, y#scale[0.55]{t}=-1","f")
    l.AddEntry(h_s,"tHq(b#bar{b})","f")
    l.AddEntry(h_b,"All bkgs.","f")
    #l.AddEntry(h_err,"Stat.+sys.","f")
    l.Draw()



    if mcunc_on_bck:
        output = output + '_bck'
    c.SaveAs(output+".pdf")
    wait = input("PRESS ENTER TO CONTINUE.")  

            
def main(inputfilename, output, datas, bins, back):
    inputroot = TFile("%s.root" % inputfilename , 'READ')
    datafiles = []
    if not inputroot:
        raise IOError("Can't open input file: %s.root" % inputfilename)
    for dataname in datas:
        print "Appending file",dataname
        datafiles.append( TFile("%s.root" % dataname , 'READ'))
        
    plot(inputroot, output, datafiles, bins, back)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='Input name of root file, without *.root')
    parser.add_argument('--data', metavar='N', nargs='+', help='Input name of data, without *.root')
    parser.add_argument('--back', action='store_true', default=False, help='MC unc on background only?')
    parser.add_argument('--DCsignal', action='store_true', default=False, help='Take signal from prefit?')
    parser.add_argument('--nbinsx', type=int, default=-1, help='Number of bins')
    parser.add_argument('--output', default="tmp", help='Output name')
    args = parser.parse_args()
    main(args.input, args.output, args.data, args.nbinsx, args.back)



