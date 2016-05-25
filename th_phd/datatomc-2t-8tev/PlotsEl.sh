######################################################
##################### LUMI 0 #########################
######################################################


##############################################
###########   GLOBAL #########################
##############################################

##
## JETS AND MET
##

th_datatomc_x tHel.config -varname MET -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHel.config -varname METphi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 25 
th_datatomc_x tHel.config -varname JetPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 500 -nbinsx 50
th_datatomc_x tHel.config -varname Jet1Pt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 400 -nbinsx 30 
th_datatomc_x tHel.config -varname Jet2Pt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHel.config -varname Jet3Pt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 300 -nbinsx 30
th_datatomc_x tHel.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 50
th_datatomc_x tHel.config -varname Jet1Eta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel.config -varname Jet2Eta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel.config -varname Jet3Eta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel.config -varname JetPhi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 50
th_datatomc_x tHel.config -varname NJet30 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.5 -xmax 10.5 -nbinsx 11
th_datatomc_x tHel.config -varname JetA -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHel.config -varname Jet1A -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHel.config -varname SumET -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 1000 -nbinsx 40
th_datatomc_x tHel.config -varname METsig -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 25 -nbinsx 50
th_datatomc_x tHel.config -varname LightEta -varexpr "abs(jteta[etapos[0]])"  -stackmc 1 -batch 1 -xmin 0 -xmax 4.7 -nbinsx 50 -lumi 0 -suffix "lumi0" -xtitle "max. |jet #eta|"


##
## ELECTRONS
##

th_datatomc_x tHel.config -varname ElE -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHel.config -varname ElPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHel.config -varname ElEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -2.5 -xmax 2.5 -nbinsx 30
th_datatomc_x tHel.config -varname ElPhi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHel.config -varname ElIso -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 0.1 -nbinsx 36


##
## W BOSON
##

th_datatomc_x tHel.config -varname WMt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 150 -nbinsx 30
th_datatomc_x tHel.config -varname WPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHel.config -varname WEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5 -xmax 5 -nbinsx 30
th_datatomc_x tHel.config -varname WPhi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 30

##
## CSV
##

th_datatomc_x tHel.config -varname JetCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname JetCSVT1 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname JetCSVT2 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname JetCSVT3 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30

##
## OTHER
##

th_datatomc_x tHel.config -varname NPV -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 30.5 -nbinsx 31


###################################################
###############   RECO DEPENDENT  #################
###################################################

##
## CSV
##

#
# HIGGS RECO
#
th_datatomc_x tHel.config -varname HbbCSVT11 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname HbbCSVT22 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname LightCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname TopCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30

th_datatomc_x tHel.config -varname RecMLPout -stackmc 1 -batch 1 -resmax 0.5 -lumi 0 -suffix "lumi0_log" -xmin -0.2. -xmax 2.0 -nbinsx 50 -logy 1 -overflow 1
th_datatomc_x tHel.config -varname RecMLPout -stackmc 1 -batch 1 -resmax 0.5 -lumi 0 -suffix "lumi0" -xmin -0.1 -xmax 1.1 -nbinsx 30 -overflow 1

#
# TOP RECO
#
th_datatomc_x tHel.config -varname TopCSVT1  -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname TopCSVT2 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname TopCSVT3 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname TopLepCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30

th_datatomc_x tHel.config -varname TopRecMLPout -stackmc 1 -batch 1 -resmax 0.5 -lumi 0 -suffix "lumi0_log" -xmin -0.2 -xmax 2.0 -nbinsx 50 -logy 1 -overflow 1
th_datatomc_x tHel.config -varname TopRecMLPout -stackmc 1 -resmax 0.5 -batch 1 -xmin -0.1 -xmax 1.1 -nbinsx 30 -overflow 1 -lumi 0 -suffix "lumi0"


##
## KINEMATICS
##

#
# HIGGS RECO
#
th_datatomc_x tHel.config -varname TopPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHel.config -varname TopM -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHel.config -varname TopEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5. -xmax 5. -nbinsx 30 

th_datatomc_x tHel.config -varname HbbM -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 50.0 -xmax 250 -nbinsx 30
th_datatomc_x tHel.config -varname HbbPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 500 -nbinsx 30
th_datatomc_x tHel.config -varname HbbDR -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 5 -nbinsx 30
th_datatomc_x tHel.config -varname HbbEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5. -xmax 5. -nbinsx 30

#
# TOP RECO
#
th_datatomc_x tHel.config -varname TopHadPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHel.config -varname TopHadM -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHel.config -varname TopHadEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5. -xmax 5. -nbinsx 30 
th_datatomc_x tHel.config -varname TopHadWDR -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 5. -nbinsx 30 -overflow 1
th_datatomc_x tHel.config -varname TopLepPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHel.config -varname TopLepM -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHel.config -varname TopLepEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5. -xmax 5. -nbinsx 30 


##
## ANGLES
##
th_datatomc_x tHel.config -varname CosThetaStarHiggs -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -nbinsx 30 
th_datatomc_x tHel.config -varname PullAngle -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0"

## 
## CLASSIFICATION
##

th_datatomc_x tHel.config -varname MLPout -stackmc 1 -batch 1 -xmax 1.3 -xmin -0.3 -lumi 0 -suffix "lumi0" -xtitle "MVA output"




######################################################
##################### LUMI 19304 #####################
######################################################


##############################################
###########   GLOBAL #########################
##############################################

##
## JETS AND MET
##

th_datatomc_x tHel.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHel.config -varname METphi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 25 
th_datatomc_x tHel.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 500 -nbinsx 50
th_datatomc_x tHel.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30 
th_datatomc_x tHel.config -varname Jet2Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHel.config -varname Jet3Pt -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 30
th_datatomc_x tHel.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 50
th_datatomc_x tHel.config -varname Jet1Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel.config -varname Jet2Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel.config -varname Jet3Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHel.config -varname JetPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 50
th_datatomc_x tHel.config -varname NJet30 -stackmc 1 -batch 1 -xmin 0.5 -xmax 10.5 -nbinsx 11
th_datatomc_x tHel.config -varname JetA -stackmc 1 -batch 1 -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHel.config -varname Jet1A -stackmc 1 -batch 1 -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHel.config -varname SumET -stackmc 1 -batch 1 -xmin 0.0 -xmax 1000 -nbinsx 40
th_datatomc_x tHel.config -varname METsig -stackmc 1 -batch 1 -xmin 0.0 -xmax 25 -nbinsx 50

##
## MUONS
##

th_datatomc_x tHel.config -varname MuE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHel.config -varname MuPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHel.config -varname MuEta -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -nbinsx 30
th_datatomc_x tHel.config -varname MuPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHel.config -varname MuIso -stackmc 1 -batch 1 -xmin 0.0 -xmax 0.12 -nbinsx 30


##
## W BOSON
##

th_datatomc_x tHel.config -varname WMt -stackmc 1 -batch 1 -xmin 0.0 -xmax 150 -nbinsx 30
th_datatomc_x tHel.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHel.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 30
th_datatomc_x tHel.config -varname WPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30

##
## CSV
##

th_datatomc_x tHel.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname JetCSVT1 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname JetCSVT2 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname JetCSVT3 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30

##
## OTHER
##

th_datatomc_x tHel.config -varname NPV -stackmc 1 -batch 1 -xmin -0.5 -xmax 30.5 -nbinsx 31


###################################################
###############   RECO DEPENDENT  #################
###################################################

##
## CSV
##

#
# HIGGS RECO
#
th_datatomc_x tHel.config -varname HbbCSVT11 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname HbbCSVT22 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname LightCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname TopCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30


th_datatomc_x tHel.config -varname RecMLPout -stackmc 1 -resmax 0.5 -batch 1 -suffix "log" -xmin -0.2 -xmax 2.0 -nbinsx 50 -logy 1 -overflow 1
th_datatomc_x tHel.config -varname RecMLPout -stackmc 1 -resmax 0.5 -batch 1 -xmin -0.2 -xmax 2.0 -nbinsx 50 -overflow 1

#
# TOP RECO
#
th_datatomc_x tHel.config -varname TopCSVT1  -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname TopCSVT2 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname TopCSVT3 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHel.config -varname TopLepCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30


th_datatomc_x tHel.config -varname TopRecMLPout -stackmc 1 -resmax 0.5 -batch 1 -suffix "log" -xmin -0.2 -xmax 2.0 -nbinsx 50 -logy 1 -overflow 1
th_datatomc_x tHel.config -varname TopRecMLPout -stackmc 1 -resmax 0.5 -batch 1 -xmin -0.2 -xmax 2.0 -nbinsx 50 -overflow 1




##
## KINEMATICS
##

#
# HIGGS RECO
#
th_datatomc_x tHel.config -varname TopPt -stackmc 1 -batch 1 -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHel.config -varname TopM -stackmc 1 -batch 1 -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHel.config -varname TopEta -stackmc 1 -batch 1 -xmin -5. -xmax 5. -nbinsx 30 

th_datatomc_x tHel.config -varname HbbM -stackmc 1 -batch 1 -xmin 50.0 -xmax 250 -nbinsx 30
th_datatomc_x tHel.config -varname HbbPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 500 -nbinsx 30
th_datatomc_x tHel.config -varname HbbDR -stackmc 1 -batch 1 -xmin 0 -xmax 5 -nbinsx 30
th_datatomc_x tHel.config -varname HbbEta -stackmc 1 -batch 1 -xmin -5. -xmax 5. -nbinsx 30

#
# TOP RECO
#
th_datatomc_x tHel.config -varname TopHadPt -stackmc 1 -batch 1 -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHel.config -varname TopHadM -stackmc 1 -batch 1 -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHel.config -varname TopHadEta -stackmc 1 -batch 1 -xmin -5. -xmax 5. -nbinsx 30 
th_datatomc_x tHel.config -varname TopHadWDR -stackmc 1 -batch 1 -xmin 0. -xmax 5. -nbinsx 30 -overflow 1
th_datatomc_x tHel.config -varname TopLepPt -stackmc 1 -batch 1 -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHel.config -varname TopLepM -stackmc 1 -batch 1 -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHel.config -varname TopLepEta -stackmc 1 -batch 1 -xmin -5. -xmax 5. -nbinsx 30 


##
## ANGLES
##
th_datatomc_x tHel.config -varname CosThetaStarHiggs -stackmc 1 -batch 1 -nbinsx 30 
th_datatomc_x tHel.config -varname PullAngle -stackmc 1 -batch 1


## 
## CLASSIFICATION
##

th_datatomc_x tHel.config -varname MLPout -stackmc 1 -batch 1 -xmax 1.7 -xmin -0.3
th_datatomc_x tHel.config -varname MLPout -stackmc 1 -batch 1 -xmax 1.1 -xmin -0.1 -nbinsx 15 -xtitle "MVA output" 
th_datatomc_x tHel.config -varname MLPout -stackmc 1 -batch 1 -xmax 1.1 -xmin -0.1 -nbinsx 15 -xtitle "MVA output" -lumi 0 -suffix "lumi0"
