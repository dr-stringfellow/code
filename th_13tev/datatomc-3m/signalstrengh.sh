#th_datatomc_x tHlep_ct_unsplit.config -xtitle "BDT output" -varexpr "bdtout[0]" -stackmc 1 -batch 1 -xmin -1 -xmax 1 -nbinsx 20 -suffix bdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -lumi 2150 -mcweights+ "TH125-madgraph-13tev:0" -printint 1 -pseudodata 1 -signal "" -output "bdtout0"
#th_datatomc_x tHlep_ct_unsplit.config -xtitle "BDT output" -varexpr "bdtout[0]" -stackmc 1 -batch 1 -xmin -1 -xmax 1 -nbinsx 20 -suffix bdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -lumi 2150 -mcweights+ "TH125-madgraph-13tev:30" -printint 1 -pseudodata 1 -signal "" -output "bdtout30"
#th_datatomc_x tHlep_ct_unsplit.config -xtitle "BDT output" -varexpr "bdtout[0]" -stackmc 1 -batch 1 -xmin -1 -xmax 1 -nbinsx 20 -suffix bdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -lumi 2150 -mcweights+ "TH125-madgraph-13tev:75" -printint 1 -pseudodata 1 -signal "" -output "bdtout75"
#th_datatomc_x tHlep_ct_unsplit.config -xtitle "BDT output" -varexpr "bdtout[0]" -stackmc 1 -batch 1 -xmin -1 -xmax 1 -nbinsx 20 -suffix bdtout -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -lumi 2150 -mcweights+ "TH125-madgraph-13tev:9" -printint 1 -pseudodata 1 -signal "" -output "bdtout9"


th_datatomc_x tHlep_ct_unsplit.config -varexpr "bdtout[0]" -xtitle "BDT output" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -datacuts "bdtout[0]<0" -suffix "lep_bdtout0" -down_uncfile bdtout0_tot_total_down.dat -up_uncfile bdtout0_tot_total_up.dat -mcweights+ "TH125-madgraph-13tev:0" -printint 1 -pseudodata 1 -signalfac -1 -output "bdtout0"


th_datatomc_x tHlep_ct_unsplit.config -varexpr "bdtout[0]" -xtitle "BDT output" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -datacuts "bdtout[0]<0" -suffix "lep_bdtout0" -down_uncfile bdtout0_tot_total_down.dat -up_uncfile bdtout0_tot_total_up.dat -mcweights+ "TH125-madgraph-13tev:30" -printint 1 -pseudodata 1 -signalfac -1 -output "bdtout30"


th_datatomc_x tHlep_ct_unsplit.config -varexpr "bdtout[0]" -xtitle "BDT output" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -datacuts "bdtout[0]<0" -suffix "lep_bdtout0" -down_uncfile bdtout0_tot_total_down.dat -up_uncfile bdtout0_tot_total_up.dat -mcweights+ "TH125-madgraph-13tev:75" -printint 1 -pseudodata 1 -signalfac -1 -output "bdtout75"


th_datatomc_x tHlep_ct_unsplit.config -varexpr "bdtout[0]" -xtitle "BDT output" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -datacuts "bdtout[0]<0" -suffix "lep_bdtout0" -down_uncfile bdtout0_tot_total_down.dat -up_uncfile bdtout0_tot_total_up.dat -mcweights+ "TH125-madgraph-13tev:9" -printint 1 -pseudodata 1 -signalfac -1 -output "bdtout9"
