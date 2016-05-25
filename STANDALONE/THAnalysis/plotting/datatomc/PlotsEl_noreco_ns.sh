######################################################
##################### LUMI 0 #########################
######################################################


##############################################
###########   GLOBAL #########################
##############################################

##
## JETS AND MET
##

th_datatomc_x tHel_ns.config -varname MET -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHel_ns.config -varname METphi -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 25 
th_datatomc_x tHel_ns.config -varname JetPt -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 500 -nbinsx 50
th_datatomc_x tHel_ns.config -varname Jet1Pt -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 400 -nbinsx 30 
th_datatomc_x tHel_ns.config -varname Jet2Pt -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHel_ns.config -varname Jet3Pt -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 300 -nbinsx 30
th_datatomc_x tHel_ns.config -varname JetEta -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 50
th_datatomc_x tHel_ns.config -varname Jet1Eta -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel_ns.config -varname Jet2Eta -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel_ns.config -varname Jet3Eta -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel_ns.config -varname JetPhi -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 50
th_datatomc_x tHel_ns.config -varname NJet30 -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 10.5 -nbinsx 11
th_datatomc_x tHel_ns.config -varname JetA -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHel_ns.config -varname Jet1A -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHel_ns.config -varname SumET -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1000 -nbinsx 40
th_datatomc_x tHel_ns.config -varname METsig -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 25 -nbinsx 50
th_datatomc_x tHel_ns.config -varname LightEta -varexpr "abs(jteta[etapos[0]])"  -stackmc 0 -batch 1 -xmin 0 -xmax 4.7 -nbinsx 50 -lumi 0 -suffix "lumi0" -xtitle "max. |jet #eta|"


##
## ELECTRONS
##

th_datatomc_x tHel_ns.config -varname ElE -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHel_ns.config -varname ElPt -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHel_ns.config -varname ElEta -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -2.5 -xmax 2.5 -nbinsx 30
th_datatomc_x tHel_ns.config -varname ElPhi -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHel_ns.config -varname ElIso -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 0.1 -nbinsx 36


##
## W BOSON
##

th_datatomc_x tHel_ns.config -varname WMt -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 150 -nbinsx 30
th_datatomc_x tHel_ns.config -varname WPt -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHel_ns.config -varname WEta -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5 -xmax 5 -nbinsx 30
th_datatomc_x tHel_ns.config -varname WPhi -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 30

##
## CSV
##

th_datatomc_x tHel_ns.config -varname JetCSVT -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel_ns.config -varname JetCSVT1 -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel_ns.config -varname JetCSVT2 -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel_ns.config -varname JetCSVT3 -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30

##
## OTHER
##

th_datatomc_x tHel_ns.config -varname NPV -stackmc 0 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 30.5 -nbinsx 31


