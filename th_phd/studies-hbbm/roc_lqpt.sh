th_roc_x tH.config -batch 1 -xmin 0 -xmax 10 -nbinsx 5000 -varexpr "log(jtpt[ljtidx])" -labels   "0.2:0.96:ROC curve of reconstructed p_{T} (light jet)" "0.68:0.82:t(bl#nu)H(b#bar{b})" "0.68:0.76:3T Region" "0.68:0.70:#kappa_{t}=-1, m_{H}=125 GeV" -varname LQPt -nbinsx_iter 3000


