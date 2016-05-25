# Muon

th_datatomc_x tHmu.config -varname MuE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 8
th_datatomc_x tHmu.config -varname MuPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 8
th_datatomc_x tHmu.config -varname MuEta -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -nbinsx 8
th_datatomc_x tHmu.config -varname MuPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8
th_datatomc_x tHmu.config -varname MuIso -stackmc 1 -batch 1 -xmin 0.0 -xmax 0.12 -nbinsx 8

# Electron

th_datatomc_x tHel.config -varname ElE -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 8
th_datatomc_x tHel.config -varname ElPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 300 -nbinsx 8
th_datatomc_x tHel.config -varname ElEta -stackmc 1 -batch 1 -xmin -2.1 -xmax 2.1 -nbinsx 8
th_datatomc_x tHel.config -varname ElPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8
th_datatomc_x tHel.config -varname ElIso -stackmc 1 -batch 1 -xmin 0.0 -xmax 0.12 -nbinsx 8






# W boson

th_datatomc_x tHmu.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 8
th_datatomc_x tHmu.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 8
th_datatomc_x tHmu.config -varname WPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8
th_datatomc_x tHel.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 8
th_datatomc_x tHel.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 8
th_datatomc_x tHel.config -varname WPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8
th_datatomc_x tHlep.config -varname WPt -stackmc 1 -batch 1 -xmin 0.0 -xmax 400 -nbinsx 8
th_datatomc_x tHlep.config -varname WEta -stackmc 1 -batch 1 -xmin -5 -xmax 5 -nbinsx 8
th_datatomc_x tHlep.config -varname WPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8

# Jets

th_datatomc_x tHmu.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 8
th_datatomc_x tHmu.config -varname METphi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8 
th_datatomc_x tHmu.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 600 -nbinsx 8 -logy 1 -log_min 0.01 
th_datatomc_x tHmu.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 8 
th_datatomc_x tHmu.config -varname Jet2Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 8
th_datatomc_x tHmu.config -varname Jet3Pt -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 8
th_datatomc_x tHmu.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHmu.config -varname Jet1Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHmu.config -varname Jet2Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHmu.config -varname Jet3Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHmu.config -varname JetPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8
th_datatomc_x tHmu.config -varname NJet30 -stackmc 1 -batch 1 -xmin 3.5 -xmax 10.5 -nbinsx 7
th_datatomc_x tHel.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 8
th_datatomc_x tHel.config -varname METphi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8 
th_datatomc_x tHel.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 600 -nbinsx 8 -logy 1 -log_min 0.01 
th_datatomc_x tHel.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 8 
th_datatomc_x tHel.config -varname Jet2Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 8
th_datatomc_x tHel.config -varname Jet3Pt -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 8
th_datatomc_x tHel.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHel.config -varname Jet1Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHel.config -varname Jet2Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHel.config -varname Jet3Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHel.config -varname JetPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8
th_datatomc_x tHel.config -varname NJet30 -stackmc 1 -batch 1 -xmin 3.5 -xmax 10.5 -nbinsx 7
th_datatomc_x tHlep.config -varname MET -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 8
th_datatomc_x tHlep.config -varname METphi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8 
th_datatomc_x tHlep.config -varname JetPt -stackmc 1 -batch 1 -xmin 0 -xmax 600 -nbinsx 8 -logy 1 -log_min 0.01 
th_datatomc_x tHlep.config -varname Jet1Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 8 
th_datatomc_x tHlep.config -varname Jet2Pt -stackmc 1 -batch 1 -xmin 0 -xmax 400 -nbinsx 8
th_datatomc_x tHlep.config -varname Jet3Pt -stackmc 1 -batch 1 -xmin 0 -xmax 300 -nbinsx 8
th_datatomc_x tHlep.config -varname JetEta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHlep.config -varname Jet1Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHlep.config -varname Jet2Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHlep.config -varname Jet3Eta -stackmc 1 -batch 1 -xmin -4.7 -xmax 4.7 -nbinsx 8
th_datatomc_x tHlep.config -varname JetPhi -stackmc 1 -batch 1 -xmin -3.14 -xmax 3.14 -nbinsx 8
th_datatomc_x tHlep.config -varname NJet30 -stackmc 1 -batch 1 -xmin 3.5 -xmax 10.5 -nbinsx 7
