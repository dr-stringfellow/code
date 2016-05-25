#!/bin/env python

""" Plots distribution over MVA responce using histograms from a ROOT file with details created by
    TMVA.
    When using the script, do not forget to set environment for modern Python.
"""

import sys
import os
import ROOT


# Check the arguments
if len(sys.argv) != 4:
    print 'Usage: discrPlots infoFileName MLPName Title'
    sys.exit(1)

infoFileName = sys.argv[1]
mlpName = sys.argv[2]
title = sys.argv[3]


# Some style settings
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetStripDecimals(False)
ROOT.gStyle.SetErrorX(0.)


# Open the TMVA file
infoFile = ROOT.TFile(infoFileName)
if infoFile.IsZombie():
    print 'Cannot open file "' + infoFileName + '"'
    sys.exit(1)


# Create directory to store output
outDirectory = 'fig/'
if not os.path.exists(outDirectory):
    os.makedirs(outDirectory)


# Create a canvas to draw
canvas = ROOT.TCanvas('', '', 1500, 1200)
canvas.SetGrid()
canvas.SetTicks()


# Get the ROC curve (a TH1) from the file and draw it
roc = infoFile.Get('Method_MLP/' + mlpName + '/MVA_' + mlpName + '_rejBvsS')
if not roc:
    print 'Cannot find histogram "Method_MLP/' + mlpName + '/MVA_' + mlpName + '_rejBvsS" in ' +\
     'the source file.'
    sys.exit(1)

roc.SetTitle(title + ';Signal selection efficiency;Background rejection efficiency')
roc.SetMinimum(0.)
roc.SetLineWidth(2)
roc.Draw('c')

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
hStack = ROOT.THStack('hStack', title + ';MVA response')
hStack.Add(hSgnExam, 'hist')
hStack.Add(hBkgExam, 'hist')
hStack.Add(hSgnTrain, 'pe1')
hStack.Add(hBkgTrain, 'pe1')


# Create a legend
legend = ROOT.TLegend(0.70, 0.71, 0.87, 0.87)
legend.SetFillColor(ROOT.kWhite)

legend.AddEntry(hSgnExam, 'sgn exam', 'f');
legend.AddEntry(hBkgExam, 'bkg exam', 'f');
legend.AddEntry(hSgnTrain, 'sgn train', 'p');
legend.AddEntry(hBkgTrain, 'bkg train', 'p');


# Draw the response
hStack.Draw('nostack')
hStack.SetMinimum(0.)

legend.Draw()

canvas.Print(outDirectory + 'response.png')
canvas.Print(outDirectory + 'response.pdf')
canvas.Print(outDirectory + 'response.C')
