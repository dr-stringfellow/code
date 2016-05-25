## PU reweighting

th_datatomc_x tHlep.config -varname NPV -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 30.5 -nbinsx 31 signal TH125-madgraph:TH-madgraph-Summer12_mu.root -signalfac -1 -suffix w_pu -kstest 1
th_datatomc_x tHlep.config -varname NPV -stackmc 1 -batch 1 -lumi 0 -suffix "lumi0" -xmin -0.5 -xmax 30.5 -nbinsx 31 signal TH125-madgraph:TH-madgraph-Summer12_mu.root -signalfac -1 -mcweights "weight_btag" "weight_toppt" "weight_lsf" -suffix wo_pu -kstest 1

## Top pt reweighting

th_datatomc_x tHlep.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 30 signal TH125-madgraph:TH-madgraph-Summer12_mu.root -signalfac -1 -suffix w_toppt -kstest 1
th_datatomc_x tHlep.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 30 signal TH125-madgraph:TH-madgraph-Summer12_mu.root -signalfac -1 -mcweights "weight_btag" "weight_lsf" "weightPU" -suffix wo_toppt -kstest 1

## Simple btag weights

th_datatomc_x tHlep.config -varexpr "(jtcsvt>0.898)*(abs(jteta)<2.4)" -cuts+ "abs(jteta)<2.4" -stackmc 1 -batch 1 -xmin -0.5 -xmax 1.5 -nbinsx 2 signal TH125-madgraph:TH-madgraph-Summer12_mu.root -signalfac -1 -suffix w_btag -kstest 1 -binlabels "jets failing CSVT" "jets passing CSVT" -resmax 0.1 -ytitle "Jets/Bin"
th_datatomc_x tHlep.config -varexpr "(jtcsvt>0.898)*(abs(jteta)<2.4)" -cuts+ "abs(jteta)<2.4" -stackmc 1 -batch 1 -xmin -0.5 -xmax 1.5 -nbinsx 2 signal TH125-madgraph:TH-madgraph-Summer12_mu.root -signalfac -1 -mcweights "weightPU" "weight_lsf" "weight_toppt" -suffix wo_btag -kstest 1 -binlabels "jets failing CSVT" "jets passing CSVT" -resmax 0.1 -ytitle "Jets/Bin"

## Lepton scale factors

th_datatomc_x tHlep.config -varexpr "lepeta[0]"  -stackmc 1 -batch 1 -xmin -2.5 -xmax 2.5 -nbinsx 25 signal TH125-madgraph:TH-madgraph-Summer12_mu.root -signalfac -1 -suffix w_lep -kstest 1 -data "Muon.root" "Electron.root" -xtitle "lepton #eta"
th_datatomc_x tHlep.config -varexpr "lepeta[0]"  -stackmc 1 -batch 1 -xmin -2.5 -xmax 2.5 -nbinsx 25 signal TH125-madgraph:TH-madgraph-Summer12_mu.root -signalfac -1 -suffix wo_lep -kstest 1 -mcweights "weightPU" "weight_toppt" "weight_btag" -data "Muon.root" "Electron.root" -xtitle "lepton #eta"

## Jet eta binning

th_datatomcsys_x tHlep_q2.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 50 -signal TH125-madgraph:TH-madgraph-Summer12_el.root -signalfac -1 -usesyssamples 1  -kstest 0 -ytitle "Jets/Bin"
th_datatomcsys_x tHlep_q2.config -varname JetEta2 -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 50 -signal TH125-madgraph:TH-madgraph-Summer12_el.root -signalfac -1 -usesyssamples 1  -kstest 0 -ytitle "Jets/Bin"

## CSV reweighting

th_datatomc_x tHlep.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -signalfac -1 -suffix wo_csv -ytitle  "Jets/Bin"
th_datatomc_x tHlep.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -signalfac -1 -mcweights+ "weight_csv/weight_btag" -suffix w_csv -ytitle "Jets/Bin"
