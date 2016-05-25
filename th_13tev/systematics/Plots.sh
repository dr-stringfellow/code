#
# BDT OUTPUT
#


## Q2

th_mctomc_with_dat_x Q2.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1

## JES

th_mctomc_with_dat_x JES.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1

## JES

th_mctomc_with_dat_x JER.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1

## CSV

th_mctomc_with_dat_x CSVCErr1.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1
th_mctomc_with_dat_x CSVCErr2.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1
th_mctomc_with_dat_x CSVHFStats1.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1
th_mctomc_with_dat_x CSVHFStats2.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1
th_mctomc_with_dat_x CSVHF.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1
th_mctomc_with_dat_x CSVLFStats1.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1
th_mctomc_with_dat_x CSVLFStats2.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1
th_mctomc_with_dat_x CSVLF.config -varexpr "bdtout[0]" -xmax 1 -xmin -1 -nbinsx 18  -xtitle "BDT output"  -output plots/1_bdtout0 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "bdtout0"  -overflow 1 -underflow 1


#
# INPUT VARIABLES
#

## Wolfram

th_mctomc_with_dat_x Q2.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x JES.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x JER.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVCErr1.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVCErr2.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHFStats1.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHFStats2.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHF.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLFStats1.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLFStats2.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLF.config -xtitle "Wolfram moment" -varexpr "wolframh1" -xmin -0.2 -xmax 0.4 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "wolframh1"  -overflow 1 -underflow 1



## aplanarity

th_mctomc_with_dat_x Q2.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x JES.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x JER.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVCErr1.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVCErr2.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHFStats1.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHFStats2.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHF.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLFStats1.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLFStats2.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLF.config -xtitle "Wolfram moment" -varexpr "aplanarity" -xmin -0.0 -xmax 0.35 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "aplanarity"  -overflow 1 -underflow 1




## log m3

th_mctomc_with_dat_x Q2.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x JES.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x JER.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVCErr1.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVCErr2.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHFStats1.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHFStats2.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHF.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLFStats1.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLFStats2.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLF.config -xtitle "Wolfram moment" -varexpr "log(m3)" -xmin 3.5 -xmax 7.5 -nbinsx 20 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "logm3"  -overflow 1 -underflow 1




## lepcharge

th_mctomc_with_dat_x Q2.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x JES.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x JER.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVCErr1.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVCErr2.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHFStats1.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHFStats2.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVHF.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLFStats1.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLFStats2.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1

th_mctomc_with_dat_x CSVLF.config -xtitle "Wolfram moment" -varexpr "(nel==1 && nmu==0)*Alt\$(elcharge[0],0)+(nel==0 && nmu==1)*Alt\$(mucharge[0],0)" -nbinsx 3 -xmin -1.5 -xmax 1.5 -output plots/lep_wolfram1 -normalized 0 -resmax 1.08 -ytitle "d#sigma/dp_T(t) [pb/bin] "  -datname "lepcharge"  -overflow 1 -underflow 1


