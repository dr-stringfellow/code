#!/bin/env python

""" Plots distribution over MVA responce using histograms from a ROOT file with details created by
    TMVA.
    When using the script, do not forget to set environment for modern Python.
"""

import sys
import os
import ROOT


# Check the arguments
if len(sys.argv) != 3:
    print 'Usage: discrPlots infoFileName MLPName'
    sys.exit(1)

infoFileName = sys.argv[1]
mlpName = sys.argv[2]  # E.g., "GlobalTHReco_BFGS"


# Style settings
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetStripDecimals(False)
ROOT.gStyle.SetErrorX(0.)
ROOT.TGaxis.SetMaxDigits(3)

ROOT.gStyle.SetTitleFont(42)
ROOT.gStyle.SetTitleFontSize(0.04)  # was 0.05
ROOT.gStyle.SetTitleFont(42, "XYZ");
ROOT.gStyle.SetTitleXOffset(0.9)
ROOT.gStyle.SetTitleYOffset(1.25)
ROOT.gStyle.SetTitleSize(0.05, "XYZ")  # was 0.06
ROOT.gStyle.SetLabelFont(42, "XYZ")
ROOT.gStyle.SetLabelOffset(0.007, "XYZ")
ROOT.gStyle.SetLabelSize(0.04, "XYZ")  # was 0.05
ROOT.gStyle.SetNdivisions(506, "XYZ")
ROOT.gStyle.SetLegendFont(42)

ROOT.gStyle.SetPadTopMargin(0.13)
ROOT.gStyle.SetPadBottomMargin(0.13)
ROOT.gStyle.SetPadLeftMargin(0.16)
ROOT.gStyle.SetPadRightMargin(0.05)

ROOT.gROOT.ForceStyle()  # Force the abouve style for objects read from the file


# Open the TMVA file
infoFile = ROOT.TFile(infoFileName)
if infoFile.IsZombie():
    print 'Cannot open file "' + infoFileName + '"'
    sys.exit(1)


# Create directory to store output
outDirectory = 'fig_PAS-style/'
if not os.path.exists(outDirectory):
    os.makedirs(outDirectory)


# Create a canvas to draw
canvas = ROOT.TCanvas('', '', 1200, 1000)
canvas.SetGrid()
canvas.SetTicks()


# CMS label
cmsLabel = ROOT.TLatex(0.16, 0.89, '#scale[1.2]{#font[62]{CMS}} #font[52]{Simulation Preliminary}')
cmsLabel.SetNDC()
cmsLabel.SetTextFont(42)
cmsLabel.SetTextSize(0.05)  # was 0.06
cmsLabel.SetTextAlign(11)

energyLabel = ROOT.TLatex(0.95, 0.89, '8 TeV')
energyLabel.SetNDC()
energyLabel.SetTextFont(42)
energyLabel.SetTextSize(0.05)  # was 0.06
energyLabel.SetTextAlign(31)


# Get the ROC curve (a TH1) from the file and draw it
roc = infoFile.Get('Method_MLP/' + mlpName + '/MVA_' + mlpName + '_rejBvsS')
if not roc:
    print 'Cannot find histogram "Method_MLP/' + mlpName + '/MVA_' + mlpName + '_rejBvsS" in ' +\
     'the source file.'
    sys.exit(1)

roc.SetTitle(';Signal selection efficiency;Background rejection efficiency')
roc.SetMinimum(0.)
roc.SetMaximum(1.)
roc.SetLineWidth(2)
roc.Draw('c')

cmsLabel.Draw()
energyLabel.Draw()

canvas.Print(outDirectory + 'roc.pdf')
canvas.Print(outDirectory + 'roc.png')
canvas.Print(outDirectory + 'roc.C')


# Get histograms with MVA response and set their decoration
hSgnExam = infoFile.Get('Method_MLP/' + mlpName + '/MVA_' + mlpName + '_S')
hSgnExam.SetLineColor(ROOT.kBlue)
hSgnExam.SetFillColor(38)
hSgnExam.SetFillStyle(1001)
hSgnExam.SetLineWidth(2)

hSgnTrain = infoFile.Get('Method_MLP/' + mlpName + '/MVA_' + mlpName + '_Train_S')
hSgnTrain.SetMarkerColor(ROOT.kBlue)
hSgnTrain.SetMarkerStyle(20)
hSgnTrain.SetLineColor(ROOT.kBlue)

hBkgExam = infoFile.Get('Method_MLP/' + mlpName + '/MVA_' + mlpName + '_B')
hBkgExam.SetLineColor(ROOT.kRed)
hBkgExam.SetFillColor(ROOT.kRed)
hBkgExam.SetFillStyle(3354)
hBkgExam.SetLineWidth(2)

hBkgTrain = infoFile.Get('Method_MLP/' + mlpName + '/MVA_' + mlpName + '_Train_B')
hBkgTrain.SetMarkerColor(ROOT.kRed)
hBkgTrain.SetMarkerStyle(24)
hBkgTrain.SetLineColor(ROOT.kRed)


# Normalise the histograms to unit area
for h in [hSgnExam, hSgnTrain, hBkgExam, hBkgTrain]:
    h.Scale(1. / h.Integral(0, -1))


# Put the histrograms with MVA response into a stack
hStack = ROOT.THStack('hStack', ';MVA response;Arbitrary units')
hStack.Add(hSgnExam, 'hist')
hStack.Add(hBkgExam, 'hist')
hStack.Add(hSgnTrain, 'pe1')
hStack.Add(hBkgTrain, 'pe1')


# Create a legend
legend = ROOT.TLegend(0.18, 0.65, 0.40, 0.85)
legend.SetFillColor(ROOT.kWhite)
legend.SetTextSize(0.04)

legend.AddEntry(hSgnExam, 'sgn exam', 'f');
legend.AddEntry(hBkgExam, 'bkg exam', 'f');
legend.AddEntry(hSgnTrain, 'sgn train', 'p');
legend.AddEntry(hBkgTrain, 'bkg train', 'p');


canvas.SetGrid(0, 0)


# Draw the response
hStack.Draw('nostack')
hStack.SetMinimum(0.)
hStack.SetMaximum(0.2)  # Fine tunning

legend.Draw()
cmsLabel.Draw()
energyLabel.Draw()

canvas.Print(outDirectory + 'response.png')
canvas.Print(outDirectory + 'response.pdf')
canvas.Print(outDirectory + 'response.C')
