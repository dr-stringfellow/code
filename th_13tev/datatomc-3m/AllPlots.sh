##########
# LUMI 0 #
##########

th_datatomc_x tHlep_ct.config -varname MET -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHlep_ct.config -varname JetPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 500 -nbinsx 50
th_datatomc_x tHlep_ct.config -varname Jet1Pt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -4.7 -xmax 4.7 -nbinsx 50
th_datatomc_x tHlep_ct.config -varname NJet30 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.5 -xmax 10.5 -nbinsx 10
th_datatomc_x tHlep_ct.config -varname LightEta -varexpr "abs(jteta[etapos[0]])"  -stackmc 1 -batch 1 -xmin 0 -xmax 4.7 -nbinsx 50 -lumi 0 -suffix "lumi0" -xtitle "max. |jet #eta|"

th_datatomc_x tHlep_ct.config -varname ElPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -2.5 -xmax 2.5 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElE -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElPhi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElIso -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 0.1 -nbinsx 36


th_datatomc_x tHlep_ct.config -varname MuPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -2.5 -xmax 2.5 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuE -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuPhi -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuIso -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 0.1 -nbinsx 36


th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname WEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5 -xmax 5 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname WMt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 150 -nbinsx 30


th_datatomc_x tHlep_ct.config -varname JetCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHlep_ct.config -varname JetCSVT1 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30


th_datatomc_x tHlep_ct.config -varname NPV -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 30.5 -nbinsx 31

#############                                                                                                                   
# LUMI 1978 #                                                                                                                     
#############



th_datatomc_x tHlep_ct.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHlep_ct.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 500 -nbinsx 50
th_datatomc_x tHlep_ct.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 50
th_datatomc_x tHlep_ct.config -varname NJet30 -stackmc 1 -batch 1 -xmin 0.5 -xmax 10.5 -nbinsx 10
th_datatomc_x tHlep_ct.config -varname LightEta -varexpr "abs(jteta[etapos[0]])"  -stackmc 1 -batch 1 -xmin 0 -xmax 4.7 -nbinsx 50 -lumi 0 -suffix "lumi0" -xtitle "max. |jet #eta|"

th_datatomc_x tHlep_ct.config -varname ElPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElEta -stackmc 1 -batch 1 -xmin -2.5 -xmax 2.5 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElIso -stackmc 1 -batch 1 -xmin 0.0 -xmax 0.1 -nbinsx 36

th_datatomc_x tHlep_ct.config -varname MuPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuEta -stackmc 1 -batch 1 -xmin -2.5 -xmax 2.5 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuIso -stackmc 1 -batch 1 -xmin 0.0 -xmax 0.1 -nbinsx 36


th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname WMt -stackmc 1 -batch 1 -xmin 0.0 -xmax 150 -nbinsx 30


th_datatomc_x tHlep_ct.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHlep_ct.config -varname JetCSVT1 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30


th_datatomc_x tHlep_ct.config -varname NPV -stackmc 1 -batch 1 -xmin -0.5 -xmax 30.5 -nbinsx 31

th_datatomc_x tHlep_ct.config -varname JetEta2 -stackmc 1 -batch 1
 




####################
# UNWEIGHTED PLOTS #
####################

th_datatomc_x tHlep_ct.config -varname TopLepPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30 
th_datatomc_x tHlep_ct.config -varname TopHadPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30 
th_datatomc_x tHlep_ct.config -varname TopPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 400. -nbinsx 30 



th_datatomc_x tHlep_ct.config -varname NPV -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 30.5 -nbinsx 31 -mcweights+ "1/Weight_PU" -suffix+ "noWeight"


th_datatomc_x tHlep_ct.config -varname ElPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30  -mcweights+ "1/Weight_LSF" -suffix+ "noWeight"
th_datatomc_x tHlep_ct.config -varname ElPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30  -mcweights+ "1/Weight_LSF" -suffix "noWeight"

th_datatomc_x tHlep_ct.config -varname MuPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30  -mcweights+ "1/Weight_LSF" -suffix+ "noWeight"
th_datatomc_x tHlep_ct.config -varname MuPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30  -mcweights+ "1/Weight_LSF" -suffix "noWeight"


#################
#    RESULTS    #
#################


th_datatomc_x tHlep_ct.config -xtitle "BDT output" -varexpr "bdtout[0]" -stackmc 1 -batch 1 -xmin -1 -xmax 1 -nbinsx 20 -suffix bdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -lumi 0 -down_uncfile bdtout0_tot_total_down.dat -up_uncfile bdtout0_tot_total_up.dat -datacuts "bdtout[0]<0."

th_datatomc_x tHlep_ct.config -xtitle "tH reconstruction output" -varexpr "best_recbdtout[0]" -stackmc 1 -batch 1 -xmin -1 -xmax 1 -nbinsx 20 -suffix recbdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -lumi 0

th_datatomc_x tHlep_ct.config -xtitle "ttbar reconstruction output" -varexpr "top_best_recbdtout[0]" -stackmc 1 -batch 1 -xmin -1 -xmax 1 -nbinsx 20 -suffix toprecbdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -lumi 0

