######################################################
##################### LUMI 0 #########################
######################################################


##############################################
###########   GLOBAL #########################
##############################################

##
## JETS AND MET
##

th_datatomc_x tHmu.config -varname MET -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHmu.config -varname METphi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 25 
th_datatomc_x tHmu.config -varname JetPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 500 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1Pt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 400 -nbinsx 30 
th_datatomc_x tHmu.config -varname Jet2Pt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet3Pt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1Eta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet2Eta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet3Eta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname JetPhi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 50
th_datatomc_x tHmu.config -varname NJet30 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 10.5 -nbinsx 11
th_datatomc_x tHmu.config -varname JetA -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1A -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHmu.config -varname SumET -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1000 -nbinsx 40
th_datatomc_x tHmu.config -varname METsig -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 25 -nbinsx 50

th_datatomc_x tHmu.config -varname LightEta -varexpr "abs(jteta[etapos[0]])"  -stackmc 1 -batch 1 -xmin 0 -xmax 4.7 -nbinsx 50 -lumi 0 -suffix "lumi0" -xtitle "max. |jet #eta|"

##
## MUONS
##

th_datatomc_x tHmu.config -varname MuE -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname MuPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname MuEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -2.1 -xmax 2.1 -nbinsx 30
th_datatomc_x tHmu.config -varname MuPhi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHmu.config -varname MuIso -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 0.12 -nbinsx 30


##
## W BOSON
##

th_datatomc_x tHmu.config -varname WMt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 150 -nbinsx 30
th_datatomc_x tHmu.config -varname WPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHmu.config -varname WEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5 -xmax 5 -nbinsx 30
th_datatomc_x tHmu.config -varname WPhi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 30

##
## CSV
##

th_datatomc_x tHmu.config -varname JetCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname JetCSVT1 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname JetCSVT2 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname JetCSVT3 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30

##
## OTHER
##

th_datatomc_x tHmu.config -varname NPV -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 30.5 -nbinsx 31




