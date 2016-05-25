th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 20  -mcweights+ "1/(Weight_CSV)" -suffix "noWeightCSV"
th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 20  -mcweights "(Weight_CSV)" -suffix "WeightCSV"
th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 20  -mcweights+ "1/(Weight_PU)" -suffix "noWeightPU"
th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 20  -mcweights "(Weight_CSV)" -suffix "WeightPU"
