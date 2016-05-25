# Muon
th_datatomc_x tHlep_ct.config -varname MuE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname MuEta -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -nbinsx 30

# W boson

th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 30

# Lepton
th_datatomc_x tHlep_ct.config -varexpr "(nel==1 && nmu==0)*Alt$(elpt[0],0)+(nel==0 && nmu==1)*Alt$(mupt[0],0)" -stackmc 1 -batch 1 -xmin 0 -xmax 200 -nbinsx 25 -suffix "leppt" -xtitle "lepton p_{T} [GeV]"
th_datatomc_x tHlep_ct.config -varexpr "(nel==1 && nmu==0)*Alt$(eleta[0],0)+(nel==0 && nmu==1)*Alt$(mueta[0],0)" -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -nbinsx 25 -suffix "lepeta" -xtitle "lepton #eta"

# Jets

th_datatomc_x tHlep_ct.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 25
th_datatomc_x tHlep_ct.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 700 -nbinsx 30 -logy 1 -log_min 10 -cuts+ "jtpt>20" -ytitle "Jets/Bin"
th_datatomc_x tHlep_ct.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30  
th_datatomc_x tHlep_ct.config -varname Jet2Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname Jet3Pt -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 30
th_datatomc_x tHlep_ct.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 -cuts+ "jtpt>20" -ytitle "Jets/Bin"
th_datatomc_x tHlep_ct.config -varname Jet1Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 -cuts+ "jtpt[0]>20"
th_datatomc_x tHlep_ct.config -varname Jet2Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 -cuts+ "jtpt[1]>20"
th_datatomc_x tHlep_ct.config -varname Jet3Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 -cuts+ "jtpt[2]>20"
th_datatomc_x tHlep_ct.config -varname NJet30 -stackmc 1 -batch 1 -xmin 3.5 -xmax 10.5 -nbinsx 7


#
# Lumi 0
#


th_datatomc_x tHlep_ct.config -varname MuE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname MuPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 30 -lumi 0  -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname MuEta -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -nbinsx 30 -lumi 0 -suffix "lumi0"

# W boson

th_datatomc_x tHlep_ct.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 30 -lumi 0 -suffix "lumi0"

# Lepton
th_datatomc_x tHlep_ct.config -varexpr "(nel==1 && nmu==0)*Alt$(elpt[0],0)+(nel==0 && nmu==1)*Alt$(mupt[0],0)" -stackmc 1 -batch 1 -xmin 0 -xmax 200 -lumi 0 -nbinsx 25 -suffix "leppt_lumi0" -xtitle "lepton p_{T} [GeV]"
th_datatomc_x tHlep_ct.config -varexpr "(nel==1 && nmu==0)*Alt$(eleta[0],0)+(nel==0 && nmu==1)*Alt$(mueta[0],0)" -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -lumi 0 -nbinsx 25 -suffix "lepeta_lumi0" -xtitle "lepton #eta"

# Jets

th_datatomc_x tHlep_ct.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 25 -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 700 -nbinsx 30 -logy 1 -log_min 10 -cuts+ "jtpt>30" -ytitle "Jets/Bin" -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname Jet2Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname Jet3Pt -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 30 -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 -cuts+ "jtpt>30" -ytitle "Jets/Bin" -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varexpr "abs(jteta)" -stackmc 1 -batch 1 -xmin 0 -xmax 4.7 -nbinsx 20 -cuts+ "jtpt>30" -ytitle "Jets/Bin" -lumi 0 -suffix "_absjteta_lumi0" 
th_datatomc_x tHlep_ct.config -varname Jet1Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 -cuts+ "jtpt[0]>20" -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname Jet2Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 -cuts+ "jtpt[1]>20" -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname Jet3Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 30 -cuts+ "jtpt[2]>20" -lumi 0 -suffix "lumi0"
th_datatomc_x tHlep_ct.config -varname NJet30 -stackmc 1 -batch 1 -xmin 3.5 -xmax 10.5 -nbinsx 7 -lumi 0 -suffix "lumi0"
