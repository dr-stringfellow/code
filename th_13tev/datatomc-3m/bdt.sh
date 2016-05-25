th_datatomc_x tHlep_ct.config -varexpr "best_recbdtout[0]" -xtitle "best BDT output under tHq hypothesis" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_recbdtout0"
th_datatomc_x tHlep_ct.config -varexpr "top_best_recbdtout" -xtitle "best BDT output under t#bar{t} hypothesis" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_toprecbdtout"

