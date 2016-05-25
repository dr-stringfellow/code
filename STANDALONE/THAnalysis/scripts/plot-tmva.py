#!/usr/bin/env python

'''
Make TMVA plots
'''

from RecoLuminosity.LumiDB import argparse
import math
import fnmatch
import logging
import os
import ctypes
from ROOT import TFile, TCanvas, TColor, gStyle, TStyle, TLegend, TLatex, TH1F, PyConfig, gROOT, TGraph, TGraphAsymmErrors, TMultiGraph, TEllipse, TApplication, PyROOT, TTree, TLeaf, TH1
import shutil
import sys
from array import array
import numpy as np
import string
#cmsstyle
from cmsstyle import style
from histmap import map, find


PyConfig.IgnoreCommandLineOptions = True

def divideBinContentByBinWidth(h):
    for ibin in range(1, h.GetNbinsX()+1):
        binwidth   = h.GetBinWidth(ibin)
        bincontent = h.GetBinContent(ibin)
        binerror   = h.GetBinError(ibin)
        h.SetBinContent(ibin,bincontent/binwidth);
        h.SetBinError(ibin,binerror/binwidth);

def draw_labels(labels, leginplot, res):
    for ilabel in range(0, len(labels)):
        tmp = labels[ilabel]
        pos = tmp.find(':')
        assert(pos!=-1)
        x_as_str = tmp[0:pos]; tmp = tmp[pos+1:]
        pos = tmp.find(':'); assert(pos!=-1)
        y_as_str = tmp[0:pos]; tmp = tmp[pos+1:]
        pos = tmp.find(':')
        if pos==-1 :
            font_as_str = "42"
            size_as_str = "0.05"
            text = tmp
            
        else :
            font_as_str = tmp[0:pos]; tmp = tmp[pos+1:];
            pos = tmp.find(':');
            if pos==-1 :
                size_as_str = "0.05";
                text = tmp;
                   
            else :
                size_as_str = tmp[0:pos];
                text = tmp[pos+1:];
                
        x = float(x_as_str)
        y = float(y_as_str)
        f = int(font_as_str)
        s = float(size_as_str)
        
        if not leginplot :
            scale = 800.0*0.93/1000./0.82;  x *= scale
        if res :
            y = 1.-1.33*(1.-y); s/=0.8;
        
        tex = TLatex()
        tex.SetTextSize(s);
        tex.SetTextFont(f);
        tex.SetNDC(True);
        tex.DrawLatex(x,y,text);
        

def set_legend(leg):
    leg.SetFillColor(10)
    leg.SetLineColor(10)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.05)
    leg.Draw()

def plotvariables(input, batch, toggleovertrain, labels, reco):
    if batch:
        gROOT.SetBatch()  

    style()

    height = 800
    width = 1000

    test = TTree()
    train = TTree()

    test = input.Get("TestTree")
    train = input.Get("TrainTree")

    tex=TLatex()
    tex.SetTextSize(28)
    tex.SetTextFont(43)
    tex.SetNDC(False)

    tex2=TLatex()
    tex2.SetTextSize(48)
    tex2.SetTextFont(43)
    tex2.SetNDC(False)

    regression = False

    test.GetEntry(1)
    if test.className == 'Regression' : 
        regression = True
        
    for key in test.GetListOfBranches():
        print key.GetName()
        overtrain = False
        if "class" in key.GetName():
            continue
        if "BDT" in key.GetName() or "BFGS" in key.GetName():
            overtrain = True
        elif toggleovertrain :
            continue
        
        canvas = TCanvas("c1",'can_'+key.GetName(), width, height)
        canvas.cd().SetRightMargin(0.05)
        canvas.cd().SetTopMargin(0.05)
        nbins=100000
        tmp = TH1F('tmp', 'tmp', nbins, -1000, 1000) 
        test.Project(tmp.GetName(), key.GetName(), "")
        min = -99999
        max = -99999
        threshold = 5
        countmin =0
        countmax =0
        workingdir = os.getcwd()
        if not find(key.GetName(), workingdir):
            for i in range(1,nbins+1):
                if tmp.GetBinContent(i) != 0 and min < -99998:
                    countmin += 1
                    if countmin >= threshold:
                        min = tmp.GetXaxis().GetBinLowEdge(i)
                if tmp.GetBinContent(nbins-i) != 0 and max < -99998:
                    countmax += 1
                    if countmax >= threshold:
                        max = tmp.GetXaxis().GetBinUpEdge(nbins-i)
        if min > 0:
            min = 0

        tmp.IsA().Destructor( tmp )
        Min = np.floor(min)
        max = np.ceil(max)

        result = map(key.GetName(), workingdir, min, max, 100)
        # returns [expr, min, max, nbins, xlabel]

        if result[3] < 1 :
            if result[3]==-1:
                bins = 25
                xmin = 0.0
                xmax = 4.7
                list=[]
                for i in range(0,bins-2):
                    list.append(xmin+float(i)*(2.4-xmin)/(float(bins)-2.0))
                
                list.append(2.4)
                list.append(3.2)
                list.append(xmax)
            trhisto = TH1F('trhisto'+key.GetName(), 'trhisto'+key.GetName(), bins, array('d',list))
            trhistob = TH1F('trhistob'+key.GetName(), 'trhistob'+key.GetName(), bins, array('d',list))
        else :
            trhisto = TH1F('trhisto'+key.GetName(), 'trhisto'+key.GetName(), result[3], result[1], result[2])
            trhistob = TH1F('trhistob'+key.GetName(), 'trhistob'+key.GetName(), result[3], result[1], result[2])

        histo = TH1F('histo'+key.GetName(), 'histo'+key.GetName(), result[3], result[1], result[2])
        histob = TH1F('histob'+key.GetName(), 'histob'+key.GetName(), result[3], result[1], result[2])
        histo.SetName('histo'+key.GetName())
        histob.SetName('histob'+key.GetName())
        trhisto.SetName('trhisto'+key.GetName())
        trhistob.SetName('trhistob'+key.GetName())
        if key.GetName().find(',')!=-1 or result[1]==result[2]:
            expression = key.GetName()
            if key.GetName().find('log_')!=-1:
                expression = 'exp('+expression+')'
        else :
            if key.GetName().find('log_')!=-1:
                expression = 'min(max(exp('+key.GetName()+'), '+str(result[1])+'+0.0001), ' + str(result[2]) + '-0.0001)'
            else :
                expression = 'min(max('+key.GetName()+', '+str(result[1])+'+0.0001), ' + str(result[2]) + '-0.0001)'
        selection = ''
        selectionb = ''

        print "EXPRESSION:", expression
        
        if not regression:
            selection = '(classID == 0)*weight'
            selectionb = '(classID == 1)*weight'
            test.Project(histob.GetName(), expression, selectionb)
            train.Project(trhistob.GetName(), expression, selectionb) 
        

        test.Project(histo.GetName(), expression, selection)
        train.Project(trhisto.GetName(), expression, selection) 

        if result[3] < 1 :
            divideBinContentByBinWidth(trhisto)
            if not regression:
                divideBinContentByBinWidth(trhistob)

        if trhisto.Integral()>0.0:
            trhisto.Scale(1./trhisto.Integral())
        trhisto.SetMinimum(0)
        trhisto.SetXTitle(result[4])
        trhisto.SetYTitle("norm. to unit area")
        trhisto.SetStats(0)

        #trhisto.SetFillColor(TColor.kPink)
        trhisto.SetFillStyle(3004)
        
        trhisto.SetMaximum(1.5*trhisto.GetMaximum())

        legxminot = 0.46
        legxmin = 0.65

        if reco:
            legxminot = 0.4
            legxmin = 0.55
        
        if overtrain:            
            leg1 = TLegend(0.4,0.88,0.80,0.91)
            trhisto.SetLineColor(93)
            trhisto.SetLineWidth(2)
            trhistob.SetLineWidth(2)
            trhisto.SetMarkerStyle(0)
            trhisto.SetFillColor(93)
            trhistob.SetFillStyle(1001)
            trhistob.SetFillColor(TColor.kTeal-8)
            trhistob.SetLineColor(TColor.kTeal-8)
            trhistob.SetMarkerStyle(0)
            histo.SetLineColor(99)
            histo.SetLineWidth(2)
            histo.SetMarkerStyle(0)
            histo.SetMarkerColor(99)
            histob.SetLineColor(923)
            histob.SetLineWidth(2)
            histob.SetMarkerStyle(0)
            histob.SetMarkerColor(TColor.kAzure+3)
            histob.SetLineColor(TColor.kAzure+3)
            if reco:
                leg1.AddEntry(trhisto,"#bf{#bf{Correct interpretations (train.)}}","f")
            else :
                leg1.AddEntry(trhisto,"#bf{#bf{Training signal sample}}","f")
            plotopt = 'histE'
            plotopt2 = 'PE'
        else :
            leg1 = TLegend(legxmin,0.88,0.80,0.91)
            trhistob.SetLineWidth(2)
            trhistob.SetFillColor(TColor.kTeal-8)
            trhistob.SetLineColor(TColor.kTeal-7)
            trhistob.SetMarkerStyle(0)
            trhistob.SetFillStyle(1001)
            trhisto.SetLineColor(TColor.kRed)
            trhisto.SetLineWidth(2)
            trhisto.SetMarkerStyle(0)
            trhisto.SetFillColor(TColor.kRed)
            if reco:
                leg1.AddEntry(trhisto,"#bf{Correct interpretations}","f")
            else :
                leg1.AddEntry(trhisto,"#bf{Signal}","f")
            plotopt = 'hist'
            plotopt2 = 'hist'


        if not regression:
            if trhistob.Integral() > 0:
                trhistob.Scale(1./trhistob.Integral())

        if trhistob.GetMaximum()*1.5 > trhisto.GetMaximum():
            trhisto.SetMaximum(trhistob.GetMaximum()*1.5)

        trhisto.SetNdivisions(505)
        trhisto.Draw(plotopt)
        set_legend(leg1)
        #        leg1.Draw()


        if not regression:
            trhistob.Draw(plotopt+"same")
            trhisto.Draw(plotopt+"same")

        if overtrain:
            if histo.Integral()>0.0 :
                histo.Scale(1./histo.Integral())
            histo.Draw(plotopt2+"same")
            if not regression:
                if histob.Integral()>0.0:
                    histob.Scale(1./histob.Integral())
                histob.Draw(plotopt2+"same")

        kss = -1.0
        ksb = -1.0
       
        leg2 = TLegend(0.4,0.83,0.80,0.86)
        if overtrain :
            kss = trhisto.KolmogorovTest(histo)
            if reco:
                leg2.AddEntry(histo,"#bf{#it{Correct interpretations (test)}}","l")
            else :
                leg2.AddEntry(histo,"#bf{#it{Test signal sample}}","l")

        set_legend(leg2)

        if not regression:           
            if overtrain:
                leg3 = TLegend(0.4,0.78,0.80,0.81)
                if reco:
                    leg3.AddEntry(trhistob,"#bf{#bf{Wrong interpretations (train.)}}","f");
                else :
                    leg3.AddEntry(trhistob,"#bf{#bf{Training background sample}}","f");
                ksb = trhistob.KolmogorovTest(histob)
                leg4 = TLegend(0.4,0.73,0.80,0.76)
                if reco:
                    leg4.AddEntry(histob,"#bf{#it{Wrong interpretations (test)}}","l")
                else :
                    leg4.AddEntry(histob,"#bf{#it{Test background sample}}","l")
                set_legend(leg4)
            else :
                leg3 = TLegend(legxmin,0.83,0.80,0.86)
                if reco:
                    leg3.AddEntry(trhistob,"#bf{Wrong interpretations}","f")
                else :
                    leg3.AddEntry(trhistob,"#bf{Background}","f")
            set_legend(leg3)
            
        draw_labels(labels,0,0)

        KSSBox=TLatex()
        KSSBox.SetNDC()
        KSSBox.SetTextSize(0.03)
        if overtrain:
            if reco:
                KSSBox.DrawLatex(0.815,0.965,'KS_{correct} = %i %%' % int(kss*100))
            else :
                KSSBox.DrawLatex(0.815,0.965,'KS_{signal} = %i %%' % int(kss*100))
            if not regression:
                KSBBox = TLatex()
                KSBBox.SetNDC()
                KSBBox.SetTextSize(0.03)
                if reco:
                    KSBBox.DrawLatex(0.15,0.965,'KS_{wrong} = %i %%' % int(ksb*100))
                else :
                    KSBBox.DrawLatex(0.15,0.965,'KS_{background} = %i %%' % int(ksb*100))

        canvas.RedrawAxis()
        canvas.SaveAs(input.GetName().replace('.root','')+"_"+ key.GetName() +".pdf")
        if not batch:
            wait = raw_input("Press ENTER to continue ")
        canvas.IsA().Destructor( canvas )
            
def main(input, batch, overtrain, lab, reco):
    TH1.SetDefaultSumw2()
    input = input.replace('.root','')
    inputroot = TFile("%s.root" % input , 'READ')
    if not inputroot.IsOpen():
        raise IOError("Can't open input file: %s.root" % input)
    plotvariables(inputroot, batch, overtrain, lab, reco)
    inputroot.Close()
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='Input name of root file')
    parser.add_argument('--batch', action='store_true', default=False, help='batch mode')
    parser.add_argument('--overtrain', action='store_true', default=False, help='only overtrain')
    parser.add_argument('--reco', action='store_true', default=False, help='is reconstruction')
    parser.add_argument('--labels', metavar='N', default=[], nargs='+', help='add labels in plots')
    args = parser.parse_args()
    main(args.input, args.batch, args.overtrain, args.labels, args.reco)
