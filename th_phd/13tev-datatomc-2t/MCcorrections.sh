#CSV

th_datatomc_x tHlep_ct.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 20 -signalfac -1 -mcweights+ "1/Weight_CSV" -suffix wo_csv -lumi 0 -kstest 1 -ytitle "Jets/Bin"
th_datatomc_x tHlep_ct.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 20 -signalfac -1 -suffix w_csv -lumi 0 -kstest 1 -ytitle "Jets/Bin"

#Pile-up
th_datatomc_x tHlep_ct.config -varname NPV -stackmc 1 -batch 1 -xmin -0.5 -xmax 31.5 -nbinsx 32 -signalfac -1 -mcweights+ "1/Weight_PU" -suffix wo_pu -lumi 0 -kstest 1
th_datatomc_x tHlep_ct.config -varname NPV -stackmc 1 -batch 1 -xmin -0.5 -xmax 31.5 -nbinsx 32 -signalfac -1 -suffix w_pu -lumi 0 -kstest 1
