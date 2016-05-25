th_datatomc_x tHlep_ct.config -xtitle "Wolfram moment" -varexpr "wolframh1" -stackmc 1 -batch 1   -xmin -0.2 -xmax 0.4 -nbinsx 20 -suffix lep_wolfram1 -down_uncfile ../systematics/mydata/wolframh1_bkg_tot_down.dat -up_uncfile ../systematics/mydata/wolframh1_bkg_tot_up.dat

th_datatomc_x tHlep_ct.config -xtitle "log m3" -varexpr "log(m3)" -stackmc 1 -batch 1   -xmin 3.5 -xmax 7.5  -nbinsx 20 -suffix lep_m3 -down_uncfile ../systematics/mydata/logm3_bkg_tot_down.dat -up_uncfile ../systematics/mydata/logm3_bkg_tot_up.dat


th_datatomc_x tHlep_ct.config -xtitle "aplanarity" -varexpr "aplanarity" -stackmc 1 -batch 1   -xmin -0.0 -xmax 0.35  -nbinsx 20 -suffix lep_aplanarity -down_uncfile ../systematics/mydata/aplanarity_bkg_tot_down.dat -up_uncfile ../systematics/mydata/aplanarity_bkg_tot_up.dat

th_datatomc_x tHlep_ct.config -xtitle "lepton charge" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -stackmc 1  -batch 1 -nbinsx 3 -xmin -1.5 -xmax 1.5 -suffix lep_lepcharge -down_uncfile ../systematics/mydata/lepcharge_bkg_tot_down.dat -up_uncfile ../systematics/mydata/lepcharge_bkg_tot_up.dat