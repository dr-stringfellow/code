th_datatomc_x tHlep_ct.config -varexpr "bdtout[0]" -xtitle "BDT output" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"  -suffix "lep_bdtout0" -signalfac 300  -signal "TH125-madgraph:THQ_Hincl_13TeV-madgraph-pythia8_TuneCUETP8M1.root"  "TH125-madgraph:THW_Hincl_13TeV-madgraph-pythia8_TuneCUETP8M1.root" -down_uncfile ../13tev-yields-2t/mydata/bdtout0_tot_total_down.dat -up_uncfile ../13tev-yields-2t/mydata/bdtout0_tot_total_up.dat
th_datatomc_x tHlep_ct.config -varexpr "bdtout[0]" -xtitle "BDT output" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"  -suffix "lep_bdtout0_lumi0" -signalfac 300  -signal "TH125-madgraph:THQ_Hincl_13TeV-madgraph-pythia8_TuneCUETP8M1.root"  "TH125-madgraph:THW_Hincl_13TeV-madgraph-pythia8_TuneCUETP8M1.root" -signalfac -1 -lumi 0 
th_datatomc_x tHlep_ct.config -varexpr "best_recbdtout[0]" -xtitle "best BDT output under tHq hypothesis" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_recbdtout0"
th_datatomc_x tHlep_ct.config -varexpr "top_best_recbdtout" -xtitle "best BDT output under t#bar{t} hypothesis" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_toprecbdtout"

th_datatomc_x tHlep_ct.config -varexpr "best_recbdtout[0]" -xtitle "best BDT output under tHq hypothesis" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_recbdtout0_lumi0" -lumi 0
th_datatomc_x tHlep_ct.config -varexpr "top_best_recbdtout" -xtitle "best BDT output under t#bar{t} hypothesis" -stackmc 1 -batch 1 -xmin -0.8 -xmax 0.8 -nbinsx 16 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -suffix "lep_toprecbdtout_lumi0" -lumi 0