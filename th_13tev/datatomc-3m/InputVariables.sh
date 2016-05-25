#th_datatomc_x tHlep_ct.config -xtitle "|#eta (light jet)|" -varexpr "abs(jteta[ljtidx[0]])" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 4.7 -nbinsx 20 -suffix lep_etalightjet -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"
th_datatomc_x tHlep_ct.config -varname LightJetEta2 -stackmc 1 -lumi 0 -batch 1  -xtitle "|#eta (light jet)|" -suffix lep_etalightjet -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"


th_datatomc_x tHlep_ct.config -xtitle "log p_{T} (light jet)" -varexpr "log(jtpt[ljtidx[0]])" -stackmc 1 -lumi 0 -batch 1  -xmin 2.5 -xmax 7  -nbinsx 20 -suffix lep_logptlightjet -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "lepton charge" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -stackmc 1 -lumi 0 -batch 1 -nbinsx 3 -xmin -1.5 -xmax 1.5 -suffix lep_lepcharge -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -varexpr "log(hbbpt[0])" -batch 1 -nbinsx 20 -xmin 0 -xmax 7 -xtitle "log p_{T} (Higgs)" -suffix lep_logpthiggs -stackmc 1 -lumi 0 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -varexpr "tophaddr" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 5 -xtitle "#DeltaR (W_{had} jets)" -suffix lep_whaddr -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -varexpr "log(tophadm)" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 4.5 -xmax 6 -overflow 1 -xtitle "log m (t_{had})" -suffix lep_logmthad -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of first Higgs jet" -varexpr "jtcsvt[hbbjtidx[0][0]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -suffix lep_csvhbb1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of second Higgs jet" -varexpr "jtcsvt[hbbjtidx[1][0]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -suffix lep_csvhbb2 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of first W_{had} jet" -varexpr "jtcsvt[tophadjtidx[0]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -suffix lep_csvwhad1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of second W_{had} jet" -varexpr "jtcsvt[tophadjtidx[1]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -suffix lep_csvwhad2 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"


th_datatomc_x tHlep_ct.config -xtitle "Cos #theta(lepton ,t_{had})" -varexpr "jtcostheta_l[topjtidx[0]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin -1 -xmax 1 -overflow 1 -suffix lep_costhetalt -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"
