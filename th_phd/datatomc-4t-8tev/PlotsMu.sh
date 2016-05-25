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
th_datatomc_x tHmu.config -varname NJet30 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.5 -xmax 10.5 -nbinsx 11
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



## 
## CLASSIFICATION
##

th_datatomc_x tHmu.config -varname MLPout -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmax 1.7 -xmin -0.3


###################################################
###############   RECO DEPENDENT  #################
###################################################

##
## CSV
##

#
# HIGGS RECO
#
th_datatomc_x tHmu.config -varname HbbCSVT11 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname HbbCSVT22 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname LightCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname TopCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30

th_datatomc_x tHmu.config -varname RecMLPout -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0_log" -xmin -0.2. -xmax 2.0 -nbinsx 50 -logy 1 -overflow 1
th_datatomc_x tHmu.config -varname RecMLPout -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.2. -xmax 2.0 -nbinsx 50 -overflow 1

#
# TOP RECO
#
th_datatomc_x tHmu.config -varname TopCSVT1  -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname TopCSVT2 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname TopCSVT3 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname TopLepCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30


th_datatomc_x tHmu.config -varname TopRecMLPout -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0_log" -xmin -0.2 -xmax 2.0 -nbinsx 50 -logy 1 -overflow 1
th_datatomc_x tHmu.config -varname TopRecMLPout -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.2 -xmax 2.0 -nbinsx 50 -overflow 1


##
## KINEMATICS
##

#
# HIGGS RECO
#
th_datatomc_x tHmu.config -varname TopPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHmu.config -varname TopM -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHmu.config -varname TopEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5. -xmax 5. -nbinsx 30 

th_datatomc_x tHmu.config -varname HbbM -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 50.0 -xmax 250 -nbinsx 30
th_datatomc_x tHmu.config -varname HbbPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 500 -nbinsx 30
th_datatomc_x tHmu.config -varname HbbDR -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 5 -nbinsx 30
th_datatomc_x tHmu.config -varname HbbEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5. -xmax 5. -nbinsx 30

th_datatomc_x tHmu.config -varname LightEta -varexpr "jteta[ljtidx]"  -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 30 

#
# TOP RECO
#
th_datatomc_x tHmu.config -varname TopHadPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHmu.config -varname TopHadM -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHmu.config -varname TopHadEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5. -xmax 5. -nbinsx 30 
th_datatomc_x tHmu.config -varname TopHadWDR -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 5. -nbinsx 30 -overflow 1
th_datatomc_x tHmu.config -varname TopLepPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHmu.config -varname TopLepM -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHmu.config -varname TopLepEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5. -xmax 5. -nbinsx 30 


##
## ANGLES
##
th_datatomc_x tHmu.config -varname CosThetaStarHiggs -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -nbinsx 30 
th_datatomc_x tHmu.config -varname PullAngle -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0"

## 
## CLASSIFICATION
##

th_datatomc_x tHmu.config -varname MLPout -stackmc 1 -batch 1 -xmax 1.7 -xmin -0.3 -lumi 0 -suffix "lumi0"



######################################################
##################### LUMI 19304 #####################
######################################################


##############################################
###########   GLOBAL #########################
##############################################

##
## JETS AND MET
##

th_datatomc_x tHmu.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHmu.config -varname METphi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 25 
th_datatomc_x tHmu.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 500 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30 
th_datatomc_x tHmu.config -varname Jet2Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet3Pt -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet2Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet3Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname JetPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 50
th_datatomc_x tHmu.config -varname NJet30 -stackmc 1 -batch 1 -xmin 0.5 -xmax 10.5 -nbinsx 11
th_datatomc_x tHmu.config -varname JetA -stackmc 1 -batch 1 -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1A -stackmc 1 -batch 1 -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHmu.config -varname SumET -stackmc 1 -batch 1 -xmin 0.0 -xmax 1000 -nbinsx 40
th_datatomc_x tHmu.config -varname METsig -stackmc 1 -batch 1 -xmin 0.0 -xmax 25 -nbinsx 50

##
## MUONS
##

th_datatomc_x tHmu.config -varname MuE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname MuPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname MuEta -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -nbinsx 30
th_datatomc_x tHmu.config -varname MuPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHmu.config -varname MuIso -stackmc 1 -batch 1 -xmin 0.0 -xmax 0.12 -nbinsx 30


##
## W BOSON
##

th_datatomc_x tHmu.config -varname WMt -stackmc 1 -batch 1 -xmin 0.0 -xmax 150 -nbinsx 30
th_datatomc_x tHmu.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHmu.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 30
th_datatomc_x tHmu.config -varname WPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30

##
## CSV
##

th_datatomc_x tHmu.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname JetCSVT1 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname JetCSVT2 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname JetCSVT3 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30

##
## OTHER
##

th_datatomc_x tHmu.config -varname NPV -stackmc 1 -batch 1 -xmin -0.5 -xmax 30.5 -nbinsx 31


###################################################
###############   RECO DEPENDENT  #################
###################################################

##
## CSV
##

#
# HIGGS RECO
#
th_datatomc_x tHmu.config -varname HbbCSVT11 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname HbbCSVT22 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname LightCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname TopCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30


th_datatomc_x tHmu.config -varname RecMLPout -stackmc 1 -batch 1 -suffix "log" -xmin -0.2 -xmax 2.0 -nbinsx 50 -logy 1 -overflow 1
th_datatomc_x tHmu.config -varname RecMLPout -stackmc 1 -batch 1 -suffix "log" -xmin -0.2 -xmax 2.0 -nbinsx 50 -overflow 1

th_datatomc_x tHmu.config -varname LightEta -varexpr "jteta[ljtidx]"  -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 

#
# TOP RECO
#
th_datatomc_x tHmu.config -varname TopCSVT1  -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname TopCSVT2 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname TopCSVT3 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname TopLepCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30

th_datatomc_x tHmu.config -varname TopRecMLPout -stackmc 1 -batch 1 -suffix "log" -xmin -0.2 -xmax 2.0 -nbinsx 50 -logy 1 -overflow 1
th_datatomc_x tHmu.config -varname TopRecMLPout -stackmc 1 -batch 1 -suffix "log" -xmin -0.2 -xmax 2.0 -nbinsx 50 -overflow 1


##
## KINEMATICS
##

#
# HIGGS RECO
#
th_datatomc_x tHmu.config -varname TopPt -stackmc 1 -batch 1 -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHmu.config -varname TopM -stackmc 1 -batch 1 -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHmu.config -varname TopEta -stackmc 1 -batch 1 -xmin -5. -xmax 5. -nbinsx 30 

th_datatomc_x tHmu.config -varname HbbM -stackmc 1 -batch 1 -xmin 50.0 -xmax 250 -nbinsx 30
th_datatomc_x tHmu.config -varname HbbPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 500 -nbinsx 30
th_datatomc_x tHmu.config -varname HbbDR -stackmc 1 -batch 1 -xmin 0 -xmax 5 -nbinsx 30
th_datatomc_x tHmu.config -varname HbbEta -stackmc 1 -batch 1 -xmin -5. -xmax 5. -nbinsx 30

#
# TOP RECO
#
th_datatomc_x tHmu.config -varname TopHadPt -stackmc 1 -batch 1 -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHmu.config -varname TopHadM -stackmc 1 -batch 1 -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHmu.config -varname TopHadEta -stackmc 1 -batch 1 -xmin -5. -xmax 5. -nbinsx 30 
th_datatomc_x tHmu.config -varname TopHadWDR -stackmc 1 -batch 1 -xmin 0. -xmax 5. -nbinsx 30 -overflow 1
th_datatomc_x tHmu.config -varname TopLepPt -stackmc 1 -batch 1 -xmin 0. -xmax 400. -nbinsx 30
th_datatomc_x tHmu.config -varname TopLepM -stackmc 1 -batch 1 -xmin 50. -xmax 300. -nbinsx 30 -overflow 1
th_datatomc_x tHmu.config -varname TopLepEta -stackmc 1 -batch 1 -xmin -5. -xmax 5. -nbinsx 30 


##
## ANGLES
##
th_datatomc_x tHmu.config -varname CosThetaStarHiggs -stackmc 1 -batch 1 -nbinsx 30 
th_datatomc_x tHmu.config -varname PullAngle -stackmc 1 -batch 1


## 
## CLASSIFICATION
##

th_datatomc_x tHmu.config -varname MLPout -stackmc 1 -batch 1 -xmax 1.7 -xmin -0.3 -signal TH125-madgraph:TH-madgraph-Summer12.root -logy 1 -datacuts "mlpout<0.7" -overflow 1 -xtitle "MVA output"

th_datatomc_x tHmu.config -varname transMLPout -stackmc 1 -batch 1 -xmax 1 -xmin -1 -signal TH125-madgraph:TH-madgraph-Summer12.root -datacuts "mlpouttrans<0.067" -signalfac 20 -nbinsx 15 -xtitle "MVA output"

th_datatomc_x tHmu.config -varname MLPout -stackmc 1 -batch 1 -xmax 1.1 -xmin -0.1 -signal TH125-madgraph:TH-madgraph-Summer12.root -datacuts "mlpout<0.54" -signalfac 20 -nbinsx 15 -xtitle "MVA output" -suffix "orig" -overflow 1 -underflow 1
