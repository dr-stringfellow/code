#!/usr/bin/env python

'''
Make TMVA plots
'''

import math
import os
import ctypes
from ROOT import TFile, TCanvas, TColor, gStyle, TStyle, TLegend, TLatex, TH1F, PyConfig, gROOT, TGraph, TGraphAsymmErrors, TMultiGraph, TEllipse, TApplication, PyROOT, TTree, TLeaf, TH1, RooHistError, Double
import shutil
import sys
from array import array
import numpy as np
import string
#cmsstyle


print "lirumlarum"
a = 20
print a
RooHistError
y = 1
ym = Double(0)
yp = Double(0)
nSigma = 1.0
RooHistError.instance().getPoissonInterval(y, ym, yp,nSigma)
yerrplus = yp - y
yerrminus = y - ym
print yerrplus,yerrminus
