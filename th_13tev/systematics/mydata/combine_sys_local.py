#!/usr/bin/env python

'''
Combine systematic errors of single sources for sys plot

example:

combine_sys.py -i tH_bdtout0_bkg.dat tH_bdtout0_sig.dat tH_bdtout0*q2*.dat -o combine_q2

'''

import argparse
import math
import os
import shutil
import sys
import glob
import string



		
def main(input, output):

	print "##########################"
	print "#     combine_sys.py     #"
	print "##########################"

	print ""
	print "Input File: {:>10}".format(input)
	print "Output Path: {:>10}".format(output) 
	print ""
	
	basefile = str(input[0])

        input_sig, input_bkg = CategorizeInput(input)

        print input_sig
        print input_bkg
        
        if input_sig : 
                xvals_sig=GetRefDataFromInput(input_sig)
                xvals=xvals_sig
        if input_bkg :
                xvals_bkg=GetRefDataFromInput(input_bkg)
                xvals=xvals_bkg
        
        
        for isSig in [0,1]:
		xerr2_up = [0] * len(xvals)
		xerr2_down = [0] * len(xvals)

                if(isSig): 
                        input=input_sig
                        print "\n======================="
                        print "Starting with Signal calculations"
                        print "=======================\n"
                        if not input :
                                print "\nEmpty Signal Input\n"
                                continue

                else : 
                        input=input_bkg

                        print "\n======================="
                        print "Starting with Background calculation"
                        print "=======================\n"
                        if not input :
                                print "\nEmpty Background Input\n"
                                continue


                for sysfile in input:
                        print "\n-----------------------"
                        print "Starting with %s " % sysfile
                        print "-----------------------\n"
                        if(isSig): 
                                xvals=xvals_sig
                        else : 
                                xvals=xvals_bkg

                        upfile_vals, downfile_vals = SortVariations(sysfile,xvals)
                        
                        print "\nSorted Up Variations:"
                        print upfile_vals
                        print "\nSorted Down Variations:"
                        print downfile_vals

                

                        for index_xerr in range(0,len(upfile_vals)): 
                                xerr_up = upfile_vals[index_xerr]
                                xerr_down = downfile_vals[index_xerr]
                                x = xvals[index_xerr]
                                xerr2_up[index_xerr]=xerr2_up[index_xerr]+math.pow((xerr_up-x),2)
                                xerr2_down[index_xerr]= xerr2_down[index_xerr]+math.pow((x-xerr_down),2)
                                print "Down: %f   x: %f   Diff : %f  SquareOfDiff:%f " % (xerr_down, x, x-xerr_down, math.pow((x-xerr_down),2))
                                print "Up: %f   x: %f   Diff : %f  SquareOfDiff:%f " % (xerr_up, x, xerr_up-x, math.pow((xerr_up-x),2))
  
                        print "Quadratic sums" 
                        print "\nSummed Squared Up Variations:"
                        print xerr2_up
                        print "\nSummed Squared Down Variations:"
                        print xerr2_down
                                
                xerr_up = []
                xerr_down = []
                print "Calculating Squareroots of each bin"
                for val in xerr2_up:
                        xerr_up.append(math.sqrt(val))
                
                for val in xerr2_down:
                        xerr_down.append(math.sqrt(val))


		print "Filling final values"	
		
		for index_xerr in range(0,len(xerr_up)):
			xerr_up[index_xerr] = xerr_up[index_xerr]+xvals[index_xerr]
			xerr_down[index_xerr] = -xerr_down[index_xerr]+xvals[index_xerr]
                               
                if input :
                        print "Final Up Error "
                        print xerr_up
                        print "\nFinal Down Error " 
                        print xerr_down
			print
                        WriteCombSys(isSig, output, xerr_down, xerr_up,basefile)
             
        
        print "\nDone.\n\n"

        
def WriteCombSys(isSig, output, xerr_down, xerr_up,basefile):
        print "Writing to file."
        if isSig:
            cat = "sig" 
        else:
            cat = "bkg"
        
        f_up = open(output+'_up.dat', 'w')
        counter = 0
        f_up.write("# Bin data format: xmin, xmax, y, yerr\n")
        for line in open(basefile):
                columns = line.split(" ")
                if (len(columns) >= 2 and counter > 0):
                        f_up.write("%s %s %s %s"  % (str(columns[0]), str(columns[1]),str(xerr_up[counter-1]), str(columns[3])))
                counter = counter + 1
        f_up.close()

        f_down = open(output+'_down.dat', 'w')
        counter = 0
        print
        f_down.write("# Bin data format: xmin, xmax, y, yerr\n")
        for line in open(basefile):
                columns = line.split(" ")
                if (len(columns) >= 2 and counter > 0):
                        f_down.write("%s %s %s %s"  % (str(columns[0]), str(columns[1]),str(xerr_down[counter-1]), str(columns[3])))
                counter = counter + 1
        f_down.close()

def SortVariations(sysfile,xvals):
        '''
        takes systematic category (sysfile) and the values of the reference values (sig or bkg)
        sorts up and down variations and returns two lists with fixed up and down variations
        '''

        print "Beginning SortVariations \n"
        upfile_vals = FillUpVariations(sysfile)
        downfile_vals = FillDownVariations(sysfile)
        
        error_vals = zip(upfile_vals, downfile_vals)
        
        for i in range(0,len(error_vals)):
                print "Value # %i" % i
                print "Down: %f   x: %f    Up : %f " % (error_vals[i][1], xvals[i], error_vals[i][0])
                if(error_vals[i][0]>=xvals[i] and error_vals[i][1] >= xvals[i]):
                        print "Both variations are larger than x"
                        downfile_vals[i] = xvals[i]
                        print "Setting down error to x"
                        if(error_vals[i][0]<error_vals[i][1]):
                                print "Down error larger than up, change down to up"
                                upfile_vals[i]=error_vals[i][1]
                                        
                elif(error_vals[i][0]<=xvals[i] and error_vals[i][1] <= xvals[i]):
                        print "Both variations are smaller than x"
                        upfile_vals[i] = xvals[i]
                        print "Setting up error to x"
                        if(error_vals[i][0]<error_vals[i][1]):
                                print "up error smaller than down, change up error to down"
                                downfile_vals[i]=error_vals[i][0]
                                        
                elif(error_vals[i][0]<=xvals[i] and error_vals[i][1] >= xvals[i]):
                        upfile_vals[i],downfile_vals[i] = downfile_vals[i],upfile_vals[i]
                        print "Up error smaller than x, Down error larger than x -> changing"       
        return upfile_vals,downfile_vals


def FillUpVariations(sysfile):
        print "Beginning FillUpVariations: \n"
        upfile_vals =[]
        linecounter = 0
        for line in open(sysfile+"up.dat"):
                linecounter=linecounter+1
                if (linecounter==1):
                        continue
                columns = line.split(" ")
                if len(columns) >= 2:
                        upfile_vals.append(float(columns[2]))
        return upfile_vals

def FillDownVariations(sysfile):
        print "Beginning FillDownVariations: \n"
        downfile_vals=[]
        linecounter =0
        for line in open(sysfile+"down.dat"):
                linecounter=linecounter+1
                if (linecounter==1):
                        continue
                columns = line.split(" ")
                if len(columns) >= 2:
                        downfile_vals.append(float(columns[2]))
        return downfile_vals

def GetRefDataFromInput(input):
        print "Beginning GetRefDataFromInput : \n"
        for filename in input:
                if(string.find(filename,".dat") != -1): 
                        nom_file = filename
			                                 
        xvals = []
        xerrvals_list = []
        linecounter =0
        for line in open(nom_file):
                if(linecounter==0):
                        linecounter=linecounter+1
                        continue
                columns = line.split(" ")
                if len(columns) >= 2:
                        xvals.append(float(columns[2]))
                linecounter=linecounter+1
        input.remove(nom_file)
        return xvals


def CategorizeInput(input):
        print "Beginning CategorizeInput : \n"
        input_bkg =[]
        input_sig =[]
        
        for filename in input:
                if(string.find(filename,"sig") != -1):
                        input_sig.append(filename)
                elif(string.find(filename,"bkg") != -1):
                        input_bkg.append(filename)
                else:
                        raise IOError("Input File %s not categorizable into background or signal" % filename)
                        
        for cat_input in [input_bkg,input_sig]:
                for filename in cat_input:
                        pos = string.find(filename,"up")
                        if (pos!=-1):
                                filename_new=filename[:pos]
                                for n,i in enumerate(cat_input):
                                        if i==filename:
                                                cat_input[n]=filename_new
                                                
                                                
                                        try:
                                                cat_input.remove(filename_new+"down.dat")
                                        except:
                                                continue
                                                        
                        else:
                                pos = string.find(filename,"down")
                                if(pos==-1): 
                                        continue
                                
                                filename_new=filename[:pos]
                                for n,i in enumerate(cat_input):
                                        if i==filename:
                                                cat_input[n]=filename_new   
                                                try:
                                                        cat_input.remove(filename_new+"up.dat")
                                                except:
                                                        continue
                                
       
        return input_sig, input_bkg


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input',type=str, default = '', dest="_input", nargs='+', help='Input name of sytematic files')
    parser.add_argument('-o', '--output',type=str, default = '', dest="_output", help='Output name of combined sytematic file')
    args = parser.parse_args()
    print args._input
    print args._output
    main(args._input, args._output)
