th_overtrain_x -input ./bdt_reco/bdt_recoBDTreco.root -batch 1 -varexpr "BDT" -xmin "-1" -xmax 1 -nbinsx 40
th_overtrain_x -input ./bdt_reco/bdt_recoBDTreco.root -batch 1 -varexpr "log_hbbm_" -xmin "2" -xmax 8 -nbinsx 100 -xtitle "HbbM" -reso 1
th_overtrain_x -input ./bdt_reco/bdt_recoBDTreco.root -batch 1 -varexpr "topm" -xmin 50 -xmax 300 -nbinsx 100 -xtitle "TopM" -reso 1
