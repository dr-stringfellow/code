##
## Mu Input Vars
##
th_datatomc_x tHmu.config -varexpr popvar1 -stackmc 1 -batch 1 -nbinsx 3 -xmin -1.5 -xmax 1.5 -lumi 0 -xtitle "lepton charge" -suffix mu_lepcharge -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar2 -stackmc 1 -batch 1 -nbinsx 30 -xmin 5.5 -xmax 8 -overflow 0 -lumi 0 -xtitle "log #sqrt{#hat{s}}" -suffix mu_shat -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar3 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3 -xmax 8 -lumi 0 -xtitle "log p_{T} (1. jet)" -suffix mu_1stjtpt -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar4 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3.5 -xmax 6 -lumi 0 -xtitle "log #slash{E}_{T}" -suffix mu_met -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1    -overflow 0
th_datatomc_x tHmu.config -varexpr popvar5 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 7 -lumi 0 -xtitle "log p_{T} (Higgs)/GeV" -suffix mu_logpthiggs -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar6 -stackmc 1 -batch 1 -nbinsx 30 -xmin 2.5 -xmax 7 -lumi 0 -xtitle "log p_{T} (light jet)/GeV" -suffix mu_logptlightjet -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar7 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1 -xmax 1 -lumi 0 -xtitle "cos #theta^{*}" -suffix mu_costheta -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar8 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 4.7 -lumi 0 -xtitle "|#eta (light jet)|" -suffix mu_etalightjet -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1  -ytitle "Events/Bin width" -varname LightJetEta2
th_datatomc_x tHmu.config -varexpr popvar9 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 0.2 -lumi 0 -xtitle "sphericity" -suffix mu_sphericity -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar10 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "max(#eta(q1_{top},q2_{top}))" -suffix mu_maxetawhadjets -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar11 -stackmc 1 -batch 1 -nbinsx 30 -xmin 4.5 -xmax 6 -overflow 0 -lumi 0 -xtitle "log m (t_{had})/GeV" -suffix mu_logmthad -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar12 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3.75 -xmax 5 -overflow 0  -lumi 0 -xtitle "log(m_{t_{had}}-m_{W_{had}})" -suffix mu_logmassdiff -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar13 -stackmc 1 -batch 1 -nbinsx 30 -xmin 4 -xmax 5.5 -overflow 0 -lumi 0 -xtitle "log tt_MaxMass_BTopHadLight" -suffix mu_maxmass -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar14 -stackmc 1 -batch 1 -nbinsx 3 -xmin -0.5 -xmax 2.5 -lumi 0 -xtitle "# b tags of Higgs jets" -suffix mu_btaghiggs -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar15 -stackmc 1 -batch 1 -nbinsx 3 -xmin -0.5 -xmax 2.5 -lumi 0 -xtitle "# b tags of W_{had} jets" -suffix mu_btagwhad -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar16 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "#DeltaR (j_{1,Higgs},j_{2,Higgs})" -suffix mu_hbbdr -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar17 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "#DeltaR (W_{had} jets)" -suffix mu_whaddr -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar18 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1.5 -xmax 1.5 -lumi 0 -xtitle "#Sigma jet charge (light t_{had} jets)" -suffix mu_sumjtcharge -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHmu.config -varexpr popvar19 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1 -xmax 1 -lumi 0 -xtitle "jet charge (b_{t_{had}}) * lepton charge" -suffix mu_chargeblep -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   

#
# Reco Outputs
#
th_datatomc_x tHmu.config -varname TopRecMLPout -stackmc 1  -batch 1 -xmin -0.2 -xmax 1.2 -nbinsx 30 -overflow 0 -lumi 0 -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1 -suffix "lumi0" -xtitle "best NN output under t#bar{t} hypothesis"
th_datatomc_x tHmu_rand.config -varname TopRecMLPout -stackmc 1  -batch 1 -xmin -0.7 -xmax 1.2 -nbinsx 30 -overflow 0 -lumi 0 -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1 -suffix "lumi0" -xtitle "random NN output under t#bar{t} hypothesis"
th_datatomc_x tHmu.config -varname RecMLPout -stackmc 1  -batch 1 -xmin -0.2 -xmax 1.2 -nbinsx 30 -overflow 0 -lumi 0 -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1 -suffix "lumi0" -xtitle "best NN output under tHq hypothesis"
th_datatomc_x tHmu_rand.config -varname RecMLPout -stackmc 1  -batch 1 -xmin -0.7 -xmax 1.2 -nbinsx 30 -overflow 0 -lumi 0 -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1 -suffix "lumi0" -xtitle "random NN output under tHq hypothesis" 
##
## El Input Vars
##
th_datatomc_x tHel.config -varexpr popvar1 -stackmc 1 -batch 1 -nbinsx 3 -xmin -1.5 -xmax 1.5 -lumi 0 -xtitle "lepton charge" -suffix el_lepcharge -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar2 -stackmc 1 -batch 1 -nbinsx 30 -xmin 5.5 -xmax 8 -overflow 0 -lumi 0 -xtitle "log #sqrt{#hat{s}}" -suffix el_shat -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar3 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3 -xmax 8 -lumi 0 -xtitle "log p_{T} (1. jet)" -suffix el_1stjtpt -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar4 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3.5 -xmax 6 -lumi 0 -xtitle "log #slash{E}_{T}" -suffix el_met -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1    -overflow 0
th_datatomc_x tHel.config -varexpr popvar5 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 7 -lumi 0 -xtitle "log p_{T} (Higgs)/GeV" -suffix el_logpthiggs -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar6 -stackmc 1 -batch 1 -nbinsx 30 -xmin 2.5 -xmax 7 -lumi 0 -xtitle "log p_{T} (light jet)/GeV" -suffix el_logptlightjet -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar7 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1 -xmax 1 -lumi 0 -xtitle "cos #theta^{*}" -suffix el_costheta -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar8 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 4.7 -lumi 0 -xtitle "|#eta (light jet)|" -suffix el_etalightjet -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1  -ytitle "Events/Bin width" -varname LightJetEta2 
th_datatomc_x tHel.config -varexpr popvar9 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 0.2 -lumi 0 -xtitle "sphericity" -suffix el_sphericity -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar10 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "max(#eta(q1_{top},q2_{top}))" -suffix el_maxetawhadjets -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar11 -stackmc 1 -batch 1 -nbinsx 30 -xmin 4.5 -xmax 6 -overflow 0 -lumi 0 -xtitle "log m (t_{had})/GeV" -suffix el_logmthad -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar12 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3.75 -xmax 5 -overflow 0 -lumi 0 -xtitle "log(m_{t_{had}}-m_{W_{had}})" -suffix el_logmassdiff -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1    
th_datatomc_x tHel.config -varexpr popvar13 -stackmc 1 -batch 1 -nbinsx 30 -xmin 4 -xmax 5.5 -overflow 0 -lumi 0 -xtitle "log tt_MaxMass_BTopHadLight" -suffix el_maxmass -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar14 -stackmc 1 -batch 1 -nbinsx 3 -xmin -0.5 -xmax 2.5 -lumi 0 -xtitle "# b tags of Higgs jets" -suffix el_btaghiggs -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar15 -stackmc 1 -batch 1 -nbinsx 3 -xmin -0.5 -xmax 2.5 -lumi 0 -xtitle "# b tags of W_{had} jets" -suffix el_btagwhad -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar16 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "#DeltaR (j_{1,Higgs},j_{1,Higgs})" -suffix el_hbbdr -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar17 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "#DeltaR (W_{had} jets)" -suffix el_whaddr -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar18 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1.5 -xmax 1.5 -lumi 0 -xtitle "#Sigma jet charge (light t_{had} jets)" -suffix el_sumjtcharge -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
th_datatomc_x tHel.config -varexpr popvar19 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1 -xmax 1 -lumi 0 -xtitle "jet charge (b_{t_{had}}) * lepton charge" -suffix el_chargeblep -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   

#
# Reco Outputs
#

th_datatomc_x tHel.config -varname TopRecMLPout -stackmc 1  -batch 1 -xmin -0.2 -xmax 1.2 -nbinsx 30 -overflow 0 -lumi 0 -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1 -suffix "lumi0" -xtitle "best NN output under t#bar{t} hypothesis"
th_datatomc_x tHel_rand.config -varname TopRecMLPout -stackmc 1  -batch 1 -xmin -0.7 -xmax 1.2 -nbinsx 30 -overflow 0 -lumi 0 -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1 -suffix "lumi0" -xtitle "random NN output under t#bar{t} hypothesis"
th_datatomc_x tHel.config -varname RecMLPout -stackmc 1  -batch 1 -xmin -0.2 -xmax 1.2 -nbinsx 30 -overflow 0 -lumi 0 -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1 -suffix "lumi0" -xtitle "best NN output under tHq hypothesis"
th_datatomc_x tHel_rand.config -varname RecMLPout -stackmc 1  -batch 1 -xmin -0.7 -xmax 1.2 -nbinsx 30 -overflow 0 -lumi 0 -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1 -suffix "lumi0" -xtitle "random NN output under tHq hypothesis" 



##
## Lep Input Vars
##
#th_datatomc_x tHlep.config -varexpr popvar1 -stackmc 1 -batch 1 -nbinsx 3 -xmin -1.5 -xmax 1.5 -lumi 0 -xtitle "lepton charge" -suffix lep_lepcharge -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar2 -stackmc 1 -batch 1 -nbinsx 30 -xmin 5.5  -xmax 8 -overflow 0 -lumi 0 -xtitle "log #sqrt{#hat{s}}" -suffix lep_shat -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar3 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3 -xmax 8 -lumi 0 -xtitle "log p_{T} (1. jet)" -suffix lep_1stjtpt -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar4 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3.5 -xmax 6 -lumi 0 -xtitle "log #slash{E}_{T}" -suffix lep_met -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1    -overflow 0
#th_datatomc_x tHlep.config -varexpr popvar5 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 7 -lumi 0 -xtitle "log p_{T} (Higgs)" -suffix lep_logpthiggs -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar6 -stackmc 1 -batch 1 -nbinsx 30 -xmin 2.5 -xmax 7 -lumi 0 -xtitle "log p_{T} (light jet)" -suffix lep_logptlightjet -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar7 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1 -xmax 1 -lumi 0 -xtitle "cos #theta^{*}" -suffix lep_costheta -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar8 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "|#eta (light jet)|" -suffix lep_etalightjet -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar9 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 0.2 -lumi 0 -xtitle "sphericity" -suffix lep_sphericity -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar10 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "max(#eta(q1_{top},q2_{top}))" -suffix lep_maxetawhadjets -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar11 -stackmc 1 -batch 1 -nbinsx 30 -xmin 4.5 -xmax 6 -overflow 0 -lumi 0 -xtitle "log m (t_{had})" -suffix lep_logmthad -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar12 -stackmc 1 -batch 1 -nbinsx 30 -xmin 3.75 -xmax 5 -overflow 0 -lumi 0 -xtitle "log(m_{t_{had}}-m_{W_{had}})" -suffix lep_logmassdiff -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar13 -stackmc 1 -batch 1 -nbinsx 30 -xmin 4 -xmax 5.5 -overflow 0 -lumi 0 -xtitle "log tt_MaxMass_BTopHadLight" -suffix lep_maxmass -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar14 -stackmc 1 -batch 1 -nbinsx 3 -xmin -0.5 -xmax 2.5 -lumi 0 -xtitle "# b tags of Higgs jets" -suffix lep_btaghiggs -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar15 -stackmc 1 -batch 1 -nbinsx 3 -xmin -0.5 -xmax 2.5 -lumi 0 -xtitle "# b tags of light t_{had} jets" -suffix lep_btagwhad -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar16 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "#DeltaR (j_{1,Higgs},j_{2,Higgs})" -suffix lep_hbbdr -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar17 -stackmc 1 -batch 1 -nbinsx 30 -xmin 0 -xmax 5 -lumi 0 -xtitle "#DeltaR (t_{had} jets)" -suffix lep_whaddr -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar18 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1.5 -xmax 1.5 -lumi 0 -xtitle "#Sigma jet charge (light t_{had} jets)" -suffix lep_sumjtcharge -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
#th_datatomc_x tHlep.config -varexpr popvar19 -stackmc 1 -batch 1 -nbinsx 30 -xmin -1 -xmax 1 -lumi 0 -xtitle "jet charge (b_{t_{had}}) * lepton charge" -suffix lep_chargeblep -signal TH125-madgraph:TH-madgraph-Summer12.root -signalfac -1   
