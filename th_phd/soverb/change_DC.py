#!/usr/bin/env python

'''

Make datacards blind

'''

from RecoLuminosity.LumiDB import argparse
import fnmatch
import logging
import os
import ROOT
import shutil
import sys

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch()  

def blind(input, inputroot, inputtxt, outputroot, outputtxt):

    blind_data = inputroot.Get("%s__data_obs__nominal" % input)
    blindstring = "%1.0f" % blind_data.Integral()
    print blindstring
    blindstring.replace("\.0",'')
    print blind_data.GetName()
    blind_data.Reset()
   
    keys = []
    for key in inputroot.GetListOfKeys():
        # Prevent double counting of histograms
        if key.GetName() in keys :
            continue
        elif isinstance(inputroot.Get(key.GetName()), ROOT.TH1) :
            keys.append(key.GetName())
        # Keep track of stuff we find in this directory
        directories = []
        histos = []
        name = key.GetName()
        classname = key.GetClassName()
        if classname.startswith('TDirectory'):
            if not name in directories :
                directories.append(name)
        elif isinstance(inputroot.Get(name), ROOT.TH1):
            if not name in histos :
                histos.append(name)
        # Copy all histograms from input -> output directory
        for histo in histos:
            th1 = inputroot.Get(histo)
            th1.SetName(histo)
            is_data = False
            is_nominal = False
            is_signal = False 
            path = inputroot.GetPath().split(':')[1]
            full_path = os.path.join(path, histo)
            # A version without a leading slash
            rel_path = full_path if full_path[0] != '/' else full_path[1:]
            if fnmatch.fnmatch(full_path, "*data*") :
                log.debug("... but it is data!")
                is_data = True
            if fnmatch.fnmatch(full_path, "*__tH__*") :
                is_signal = True
            if fnmatch.fnmatch(full_path, "*__nominal*") :
                is_nominal = True
            if not is_data: 
                outputroot.cd()
                th1.Write()
            if not is_data and is_nominal :
                blind_data.Add(th1)

    outputroot.cd()
    blind_data.Write()
    newstring = "%1.2f" % blind_data.Integral()
    for line in inputtxt:
        line = line.replace("  %s  " % blindstring,"  %s  " % newstring)
        outputtxt.write(line)

            
def main(inputfilename):
    inputroot = ROOT.TFile("%s.root" % inputfilename , 'READ')
    inputtxt = open("%s.txt" % inputfilename , 'r')
    if not inputroot:
        raise IOError("Can't open input file: %s.root" % inputfilename)
    if not inputtxt:
        raise IOError("Can't open input file: %s.txt" % inputfilename)
    outputroot = ROOT.TFile("%s.bk.root" % inputfilename , 'RECREATE')
    outputtxt = open("%s.bk.txt" % inputfilename , 'w')
    if not outputroot:
        raise IOError("Can't open output file: %s" % outputfilename)
    if not outputtxt:
        raise IOError("Can't open output file: %s" % outputfilename)
    blind(inputfilename, inputroot, inputtxt, outputroot, outputtxt)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='Input name of root and txt file')
    args = parser.parse_args()
    main(args.input)
    shutil.move("%s.bk.root" % args.input, "%s.root" % args.input)
    shutil.move("%s.bk.txt" % args.input, "%s.txt" % args.input)

