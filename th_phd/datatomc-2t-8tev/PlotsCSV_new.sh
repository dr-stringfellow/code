th_datatomc_x tHmu.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHel.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 
th_datatomc_x tHel.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHmu.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -cuts+ "jtpt<30 && jtpt>20" -suffix "2030" 
th_datatomc_x tHmu.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -lumi 0 -suffix "lumi0_2030" -cuts+ "jtpt<30 && jtpt>20"
th_datatomc_x tHel.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -cuts+ "jtpt<30 && jtpt>20" -suffix "2030"  
th_datatomc_x tHel.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -lumi 0 -suffix "lumi0_2030" -cuts+ "jtpt<30 && jtpt>20"
th_datatomc_x tHmu_norwgt.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30
th_datatomc_x tHmu_norwgt.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHel_norwgt.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 
th_datatomc_x tHel_norwgt.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHmu_norwgt.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -cuts+ "jtpt<30 && jtpt>20" -suffix "2030" 
th_datatomc_x tHmu_norwgt.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -lumi 0 -suffix "lumi0_2030" -cuts+ "jtpt<30 && jtpt>20"
th_datatomc_x tHel_norwgt.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -cuts+ "jtpt<30 && jtpt>20" -suffix "2030"  
th_datatomc_x tHel_norwgt.config -varname JetCSVT -stackmc 1 -batch 1 -xmin 0. -xmax 1. -nbinsx 30 -lumi 0 -suffix "lumi0_2030" -cuts+ "jtpt<30 && jtpt>20"
