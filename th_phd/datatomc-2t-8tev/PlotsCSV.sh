##
## JETS AND MET
##

th_datatomc_x tHmu.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHmu.config -varname METphi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 25 
th_datatomc_x tHmu.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 500 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30 
th_datatomc_x tHmu.config -varname Jet2Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet3Pt -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet2Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname Jet3Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30
th_datatomc_x tHmu.config -varname JetPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 50
th_datatomc_x tHmu.config -varname NJet30 -stackmc 1 -batch 1 -xmin 0.5 -xmax 8.5 -nbinsx 9
th_datatomc_x tHmu.config -varname JetA -stackmc 1 -batch 1 -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHmu.config -varname Jet1A -stackmc 1 -batch 1 -xmin 0.0 -xmax 1.5 -nbinsx 50
th_datatomc_x tHmu.config -varname SumET -stackmc 1 -batch 1 -xmin 0.0 -xmax 1000 -nbinsx 40
th_datatomc_x tHmu.config -varname METsig -stackmc 1 -batch 1 -xmin 0.0 -xmax 25 -nbinsx 50

##
## MUONS
##

th_datatomc_x tHmu.config -varname MuE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname MuPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname MuEta -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -nbinsx 30
th_datatomc_x tHmu.config -varname MuPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHmu.config -varname MuIso -stackmc 1 -batch 1 -xmin 0.0 -xmax 0.12 -nbinsx 30

##
## ELECTRONS
##

th_datatomc_x tHmu.config -varname ElE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname ElPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHmu.config -varname ElEta -stackmc 1 -batch 1 -xmin -2.5 -xmax 2.5 -nbinsx 30
th_datatomc_x tHmu.config -varname ElPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30
th_datatomc_x tHmu.config -varname ElIso -stackmc 1 -batch 1 -xmin 0.0 -xmax 0.1 -nbinsx 36

##
## W BOSON
##

th_datatomc_x tHmu.config -varname WMt -stackmc 1 -batch 1 -xmin 0.0 -xmax 150 -nbinsx 30
th_datatomc_x tHmu.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHmu.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 30
th_datatomc_x tHmu.config -varname WPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 30


##
## OTHER
##

th_datatomc_x tHmu.config -varname NPV -stackmc 1 -batch 1 -xmin -0.5 -xmax 30.5 -nbinsx 31



##
## TOP PT REWEIGHTING (Sample weights obtained by comparing the integrals without any cuts)
##

th_datatomc_x tHmu.config -varname TopPt -stackmc 1 -batch 1 -xmax 400 -nbinsx 40 -overflow 1 -kstest 1
th_datatomc_x tHmu.config -varname TopLepPt -stackmc 1 -batch 1 -xmax 400 -nbinsx 40 -overflow 1 -kstest 1
th_datatomc_x tHmu.config -varname TopHadPt -stackmc 1 -batch 1 -xmax 400 -nbinsx 40 -overflow 1 -kstest 1
th_datatomc_x tHmu.config -varname TopM -stackmc 1 -batch 1 -xmin 50. -xmax 300. -nbinsx 30 -overflow 1 -kstest 1



th_datatomc_x tHmu.config -varname HbbCSVT11 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -kstest 1
th_datatomc_x tHmu.config -varname HbbCSVT22 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -kstest 1
th_datatomc_x tHmu.config -varname LightCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -kstest 1
th_datatomc_x tHmu.config -varname TopCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -kstest 1

th_datatomc_x tHmu.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -kstest 1
th_datatomc_x tHmu.config -varname JetCSVT1 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -kstest 1
th_datatomc_x tHmu.config -varname JetCSVT2 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -kstest 1
th_datatomc_x tHmu.config -varname JetCSVT3 -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -kstest 1






th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<30&&jtpt[]>20)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<30&&jtpt[]>20)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<30&&jtpt[]>20)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:20 < p_{T} < 30 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT2030_log" -logy 1 -cuts+ "jtpt[]>20 &&jtpt[]<30 && abs(jteta)<2.4"
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<30&&jtpt[]>20)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<30&&jtpt[]>20)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<30&&jtpt[]>20)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:20 < p_{T} < 30 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT2030" -cuts+ "jtpt[]>20 &&jtpt[]<30 && abs(jteta)<2.4"

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<40&&jtpt[]>30)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<40&&jtpt[]>30)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<40&&jtpt[]>30)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:30 < p_{T} < 40 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT3040_log" -logy 1 -cuts+ "jtpt[]>30 &&jtpt[]<40&& abs(jteta)<2.4 "
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt<40&&jtpt>30)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<40&&jtpt[]>30)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<40&&jtpt[]>30)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:30 < p_{T} < 40 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT3040" -cuts+ "jtpt[]>30 &&jtpt[]<40&& abs(jteta)<2.4  "

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<60&&jtpt[]>40)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<60&&jtpt[]>40)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<60&&jtpt[]>40)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:40 < p_{T} < 60 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 1 -output "tHmu_JetCSVT4060_log" -logy 1 -cuts+ "jtpt[]>40 &&jtpt[]<60 && abs(jteta)<2.4"
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt<60&&jtpt>40)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<60&&jtpt[]>40)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<60&&jtpt[]>40)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:40 < p_{T} < 60 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT4060" -cuts+ "jtpt[]>40 &&jtpt[]<60 && abs(jteta)<2.4"

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<100&&jtpt[]>60)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<100&&jtpt[]>60)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<100&&jtpt[]>60)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:60 < p_{T} < 100 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT60100_log" -logy 1  -cuts+ "jtpt[]>60 &&jtpt[]<100&& abs(jteta)<2.4 "
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt<100&&jtpt>60)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<100&&jtpt[]>60)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<100&&jtpt[]>60)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:60 < p_{T} < 100 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT60100"  -cuts+ "jtpt[]>60 &&jtpt[]<100&& abs(jteta)<2.4 "

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<160&&jtpt[]>100)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<160&&jtpt[]>100)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<160&&jtpt[]>100)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:100 < p_{T} < 160 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT100160_log" -logy 1  -cuts+ "jtpt[]>100 &&jtpt[]<160&& abs(jteta)<2.4 "
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt<160&&jtpt>100)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<160&&jtpt[]>100)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<160&&jtpt[]>100)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:100 < p_{T} < 160 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT100160"  -cuts+ "jtpt[]>100 &&jtpt[]<160 && abs(jteta)<2.4"

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]>160)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]>160)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]>160)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:p_{T} > 160 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT160_log" -logy 1 -cuts+ "jtpt[]>160 && abs(jteta)<2.4"
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt>160)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]>160)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]>160)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:p_{T} > 160 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT160" -cuts+ "jtpt[]>160 && abs(jteta)<2.4"





 th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:0 < |#eta| < 0.8" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta0to08_log" -cuts "abs(jteta[])<0.8 &&abs(jteta[])>0" -logy 1
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)"  -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:0 < |#eta| < 0.8" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta0to08" -cuts "abs(jteta[])<0.8 &&abs(jteta[])>0"

 th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:0.8 < |#eta| < 1.6" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta08to16_log" -cuts "abs(jteta[])<1.6 &&abs(jteta[])>0.8" -logy 1
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:0.8 < |#eta| < 1.6" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta08to16" -cuts "abs(jteta[])<1.6 &&abs(jteta[])>0.8"

 th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:1.6 < |#eta| < 2.4" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta16to24_log" -cuts "abs(jteta[])<2.4 &&abs(jteta[])>1.6" -logy 1
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:1.6 < |#eta| < 2.4" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta16to24" -cuts "abs(jteta[])<2.4 &&abs(jteta[])>1.6"

 th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:2.4 < |#eta| < 4.7" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta24to47_log" -cuts "abs(jteta[])<4.7 &&abs(jteta[])>2.4" -logy 1
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin -0.15 -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:2.41 < |#eta| < 4.7" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta24to47" -cuts "abs(jteta[])<4.7 &&abs(jteta[])>2.4"




##### 0 as left bound


th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<30&&jtpt[]>20)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<30&&jtpt[]>20)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<30&&jtpt[]>20)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:20 < p_{T} < 30 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT2030_log" -logy 1 -cuts+ "jtpt[]>20 &&jtpt[]<30 && abs(jteta)<2.4"
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<30&&jtpt[]>20)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<30&&jtpt[]>20)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<30&&jtpt[]>20)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:20 < p_{T} < 30 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT2030" -cuts+ "jtpt[]>20 &&jtpt[]<30 && abs(jteta)<2.4"

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<40&&jtpt[]>30)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<40&&jtpt[]>30)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<40&&jtpt[]>30)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:30 < p_{T} < 40 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT3040_log" -logy 1 -cuts+ "jtpt[]>30 &&jtpt[]<40&& abs(jteta)<2.4 "
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt<40&&jtpt>30)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<40&&jtpt[]>30)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<40&&jtpt[]>30)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:30 < p_{T} < 40 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT3040" -cuts+ "jtpt[]>30 &&jtpt[]<40&& abs(jteta)<2.4  "

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<60&&jtpt[]>40)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<60&&jtpt[]>40)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<60&&jtpt[]>40)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:40 < p_{T} < 60 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 1 -output "tHmu_JetCSVT4060_log" -logy 1 -cuts+ "jtpt[]>40 &&jtpt[]<60 && abs(jteta)<2.4"
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt<60&&jtpt>40)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<60&&jtpt[]>40)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<60&&jtpt[]>40)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:40 < p_{T} < 60 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT4060" -cuts+ "jtpt[]>40 &&jtpt[]<60 && abs(jteta)<2.4"

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<100&&jtpt[]>60)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<100&&jtpt[]>60)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<100&&jtpt[]>60)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:60 < p_{T} < 100 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT60100_log" -logy 1  -cuts+ "jtpt[]>60 &&jtpt[]<100&& abs(jteta)<2.4 "
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt<100&&jtpt>60)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<100&&jtpt[]>60)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<100&&jtpt[]>60)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:60 < p_{T} < 100 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT60100"  -cuts+ "jtpt[]>60 &&jtpt[]<100&& abs(jteta)<2.4 "

th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt[]<160&&jtpt[]>100)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<160&&jtpt[]>100)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<160&&jtpt[]>100)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:100 < p_{T} < 160 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT100160_log" -logy 1  -cuts+ "jtpt[]>100 &&jtpt[]<160&& abs(jteta)<2.4 "
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt<160&&jtpt>100)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]<160&&jtpt[]>100)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]<160&&jtpt[]>100)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:100 < p_{T} < 160 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT100160"  -cuts+ "jtpt[]>100 &&jtpt[]<160 && abs(jteta)<2.4"


th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt>160)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]>160)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]>160)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:p_{T} > 160 GeV" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVT160" -cuts+ "jtpt[]>160 && abs(jteta)<2.4"
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtpt>160)*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)*(jtpt[]>160)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)*(jtpt[]>160)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:p_{T} > 160 GeV" -ytitle "jets" -xtitle "jet CSV" -logy 1 -kstest 0 -output "tHmu_JetCSVT160_log" -cuts+ "jtpt[]>160 && abs(jteta)<2.4"




 th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 40 -labels+ "0.55:0.84:0 < |#eta| < 0.8" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta0to08_log" -cuts "abs(jteta[])<0.8 &&abs(jteta[])>0" -logy 1
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)"  -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:0 < |#eta| < 0.8" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta0to08" -cuts "abs(jteta[])<0.8 &&abs(jteta[])>0"

 th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:0.8 < |#eta| < 1.6" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta08to16_log" -cuts "abs(jteta[])<1.6 &&abs(jteta[])>0.8" -logy 1
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:0.8 < |#eta| < 1.6" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta08to16" -cuts "abs(jteta[])<1.6 &&abs(jteta[])>0.8"

 th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:1.6 < |#eta| < 2.4" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta16to24_log" -cuts "abs(jteta[])<2.4 &&abs(jteta[])>1.6" -logy 1
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:1.6 < |#eta| < 2.4" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta16to24" -cuts "abs(jteta[])<2.4 &&abs(jteta[])>1.6"

 th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:2.4 < |#eta| < 4.7" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta24to47_log" -cuts "abs(jteta[])<4.7 &&abs(jteta[])>2.4" -logy 1
th_datatomc_x tHmu.config -varexpr "jtcsvt[]*(jtcsvt[]>0)+(jtcsvt[]+9.90)*(jtcsvt[]==-10)+(jtcsvt[]+0.95)*(jtcsvt[]==-1)" -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -labels+ "0.55:0.84:2.41 < |#eta| < 4.7" -ytitle "jets" -xtitle "jet CSV" -kstest 0 -output "tHmu_JetCSVTEta24to47" -cuts "abs(jteta[])<4.7 &&abs(jteta[])>2.4"


