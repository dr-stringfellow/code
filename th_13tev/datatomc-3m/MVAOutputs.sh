th_datatomc_x tHlep_ct.config -xtitle "BDT output" -varexpr "bdtout[0]" -stackmc 1 -batch 1 -xmin -0.4 -xmax 0.4 -nbinsx 30 -suffix bdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"  -printint 1 -kstest 1&
th_datatomc_x tHlep_ct.config -xtitle "tH reconstruction output" -varexpr "best_recbdtout[0]" -stackmc 1 -batch 1 -xmin -0.4 -xmax 0.4 -nbinsx 30 -suffix recbdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -kstest 1&

th_datatomc_x tHlep_ct.config -xtitle "ttbar reconstruction output" -varexpr "top_best_recbdtout[0]" -stackmc 1 -batch 1 -xmin -0.4 -xmax 0.4 -nbinsx 30 -suffix toprecbdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -kstest 1 -lumi 0&

 th_datatomc_x tHlep_ct.config -xtitle "ttbar reconstruction output" -varexpr "top_rand_recbdtout" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.4 -nbinsx 30 -suffix toprerandcbdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -kstest 1 -lumi 0&
