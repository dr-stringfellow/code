##########
# LUMI 0 #
##########

th_datatomc_x tHlep_ct.config -varname MET -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHlep_ct.config -varname JetPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 350 -nbinsx 50
th_datatomc_x tHlep_ct.config -varname Jet1Pt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0 -xmax 350 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname JetEta2 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0"  -cuts+ "(jteta[]<2.4||(jtpt[]>40))"


th_datatomc_x tHlep_ct.config -varname NJet30 -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.5 -xmax 10.5 -nbinsx 10

th_datatomc_x tHlep_ct.config -varname ElPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname ElEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -2.5 -xmax 2.5 -nbinsx 30


th_datatomc_x tHlep_ct.config -varname MuPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -2.5 -xmax 2.5 -nbinsx 30


th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname WEta -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -5 -xmax 5 -nbinsx 30


th_datatomc_x tHlep_ct.config -varname NPV -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 30.5 -nbinsx 31

th_datatomc_x tHlep_ct.config -varname JetCSVT -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin 0. -xmax 1. -nbinsx 30


#################
#    RESULTS    #
#################

th_datatomc_x tHlep_ct.config -xtitle "BDT output" -varexpr "bdtout[0]" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 30 -suffix bdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"  -printint 1 -lumi 0 &

th_datatomc_x tHlep_ct.config -xtitle "tH reconstruction output" -varexpr "best_recbdtout[0]" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 30 -suffix recbdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"  -lumi 0&

th_datatomc_x tHlep_ct.config -xtitle "ttbar reconstruction output" -varexpr "top_best_recbdtout[0]" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.7 -nbinsx 30 -suffix toprecbdtout -lumi 0&


###############
# CLASS INPUT #
###############

th_datatomc_x tHlep_ct.config -varname LightJetEta2 -stackmc 1 -lumi 0 -batch 1  -xtitle "|#eta (light jet)|" -suffix lep_etalightjet -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "log p_{T} (light jet)" -varexpr "log(jtpt[ljtidx[0]])" -stackmc 1 -lumi 0 -batch 1  -xmin 2.5 -xmax 7  -nbinsx 20 -suffix lep_logptlightjet -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -varexpr "log(hbbpt[0])" -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 7 -xtitle "log p_{T} (Higgs)" -suffix lep_logpthiggs -stackmc 1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -varexpr "tophaddr" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 5 -xtitle "#DeltaR (W_{had} jets)" -suffix lep_whaddr -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -varexpr "log(tophadm)" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 4.5 -xmax 7 -overflow 1 -xtitle "log m (t_{had})" -suffix lep_logmthad -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of first Higgs jet" -varexpr "jtcsvt[hbbjtidx[0][0]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -suffix lep_csvhbb1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of second Higgs jet" -varexpr "jtcsvt[hbbjtidx[1][0]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -suffix lep_csvhbb2 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of first W_{had} jet" -varexpr "jtcsvt[tophadjtidx[0]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -suffix lep_csvwhad1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of second W_{had} jet" -varexpr "jtcsvt[tophadjtidx[1]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -suffix lep_csvwhad2 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "aplanarity" -varexpr "aplanarity" -stackmc 1 -lumi 0 -lumi 0 -batch 1   -xmin -0.0 -xmax 0.35  -nbinsx 20 -suffix lep_aplanarity -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "log m3" -varexpr "log(m3)" -stackmc 1 -lumi 0 -lumi 0 -batch 1   -xmin 3.5 -xmax 7.5  -nbinsx 20 -suffix lep_m3 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "Wolfram moment" -varexpr "wolframh1" -stackmc 1 -lumi 0 -lumi 0 -batch 1   -xmin -0.2 -xmax 0.4 -nbinsx 20 -suffix lep_wolfram1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "lepton charge" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -stackmc 1 -lumi 0 -batch 1 -nbinsx 3 -xmin -1.5 -xmax 1.5 -suffix lep_lepcharge -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -varexpr "jtcostheta_l[topjtidx[0]]" -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -1 -xmax 1. -nbinsx 30 -suffix "jtcostheta_l1" -xtitle "Cos #theta(#ell ,t_{had})" -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

