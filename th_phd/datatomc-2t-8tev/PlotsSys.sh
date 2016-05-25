#
# JES SYSTEMATICS
#

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JES syst." -suffix "jes"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>20 && jtpt<25" -labels+ "0.55:0.89:jet p_{T} #in (20,25) GeV/c" -suffix "jes_2025"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>25 && jtpt<30" -labels+ "0.55:0.89:jet p_{T} #in (25,30) GeV/c" -suffix "jes_2530"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>30 && jtpt<40" -labels+ "0.55:0.89:jet p_{T} #in (30,40) GeV/c" -suffix "jes_3040"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>40 && jtpt<50" -labels+ "0.55:0.89:jet p_{T} #in (40,50) GeV/c" -suffix "jes_4050"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JES syst."  -cuts+ "jtpt>50 && jtpt<75" -labels+ "0.55:0.89:jet p_{T} #in (50,75) GeV/c" -suffix "jes_5075"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>75 " -labels+ "0.55:0.89:jet p_{T} #in (75,#infty) GeV/c" -suffix "jes_75"



#
# JES SYSTEMATICS - LOOSE PU ID
#

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JES syst."  -labels+ "0.55:0.96:loose cut-based PU ID" -suffix "jespu"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>20 && jtpt<25" -labels+ "0.55:0.89:jet p_{T} #in (20,25) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jespu_2025"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>25 && jtpt<30" -labels+ "0.55:0.89:jet p_{T} #in (25,30) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jespu_2530"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>30 && jtpt<40" -labels+ "0.55:0.89:jet p_{T} #in (30,40) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jespu_3040"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>40 && jtpt<50" -labels+ "0.55:0.89:jet p_{T} #in (40,50) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jespu_4050"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JES syst."  -cuts+ "jtpt>50 && jtpt<75" -labels+ "0.55:0.89:jet p_{T} #in (50,75) GeV/c" "0.55:0.96:loose cut-based PU ID"  -suffix "jespu_5075"

th_datatomcsys_x tHmu_jes.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JES syst." -cuts+ "jtpt>75 " -labels+ "0.55:0.89:jet p_{T} #in (75,#infty) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jespu_75"



#
# JER SYSTEMATICS
#

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JER syst." -suffix "jer"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>20 && jtpt<25" -labels+ "0.55:0.89:jet p_{T} #in (20,25) GeV/c" -suffix "jer_2025"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>25 && jtpt<30" -labels+ "0.55:0.89:jet p_{T} #in (25,30) GeV/c" -suffix "jer_2530"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>30 && jtpt<40" -labels+ "0.55:0.89:jet p_{T} #in (30,40) GeV/c" -suffix "jer_3040"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>40 && jtpt<50" -labels+ "0.55:0.89:jet p_{T} #in (40,50) GeV/c" -suffix "jer_4050"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JER syst."  -cuts+ "jtpt>50 && jtpt<75" -labels+ "0.55:0.89:jet p_{T} #in (50,75) GeV/c" -suffix "jer_5075"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>75 " -labels+ "0.55:0.89:jet p_{T} #in (75,#infty) GeV/c" -suffix "jer_75"



#
# JER SYSTEMATICS - LOOSE PU ID
#

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JER syst."  -labels+ "0.55:0.96:loose cut-based PU ID" -suffix "jerpu"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>20 && jtpt<25" -labels+ "0.55:0.89:jet p_{T} #in (20,25) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jerpu_2025"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>25 && jtpt<30" -labels+ "0.55:0.89:jet p_{T} #in (25,30) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jerpu_2530"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>30 && jtpt<40" -labels+ "0.55:0.89:jet p_{T} #in (30,40) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jerpu_3040"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>40 && jtpt<50" -labels+ "0.55:0.89:jet p_{T} #in (40,50) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jerpu_4050"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JER syst."  -cuts+ "jtpt>50 && jtpt<75" -labels+ "0.55:0.89:jet p_{T} #in (50,75) GeV/c" "0.55:0.96:loose cut-based PU ID"  -suffix "jerpu_5075"

th_datatomcsys_x tHmu_jer.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "JER syst." -cuts+ "jtpt>75 " -labels+ "0.55:0.89:jet p_{T} #in (75,#infty) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "jerpu_75"



#
# Q2 SYSTEMATICS
#

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "Q2 syst." -suffix "q2"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>20 && jtpt<25" -labels+ "0.55:0.89:jet p_{T} #in (20,25) GeV/c" -suffix "q2_2025"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>25 && jtpt<30" -labels+ "0.55:0.89:jet p_{T} #in (25,30) GeV/c" -suffix "q2_2530"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>30 && jtpt<40" -labels+ "0.55:0.89:jet p_{T} #in (30,40) GeV/c" -suffix "q2_3040"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>40 && jtpt<50" -labels+ "0.55:0.89:jet p_{T} #in (40,50) GeV/c" -suffix "q2_4050"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "Q2 syst."  -cuts+ "jtpt>50 && jtpt<75" -labels+ "0.55:0.89:jet p_{T} #in (50,75) GeV/c" -suffix "q2_5075"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>75 " -labels+ "0.55:0.89:jet p_{T} #in (75,#infty) GeV/c" -suffix "q2_75"



#
# Q2 SYSTEMATICS - LOOSE PU ID
#

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "Q2 syst."  -labels+ "0.55:0.96:loose cut-based PU ID" -suffix "q2pu"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>20 && jtpt<25" -labels+ "0.55:0.89:jet p_{T} #in (20,25) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "q2pu_2025"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>25 && jtpt<30" -labels+ "0.55:0.89:jet p_{T} #in (25,30) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "q2pu_2530"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>30 && jtpt<40" -labels+ "0.55:0.89:jet p_{T} #in (30,40) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "q2pu_3040"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>40 && jtpt<50" -labels+ "0.55:0.89:jet p_{T} #in (40,50) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "q2pu_4050"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "Q2 syst."  -cuts+ "jtpt>50 && jtpt<75" -labels+ "0.55:0.89:jet p_{T} #in (50,75) GeV/c" "0.55:0.96:loose cut-based PU ID"  -suffix "q2pu_5075"

th_datatomcsys_x tHmu_q2.config -varname JetEta -stackmc 1 -batch 1 -lumi 0 -xmin -4.7 -xmax 4.7 -nbinsx 40 -cuts+ "jtpt[]>20 && jtpucutidl[]>0" -usesyssamples 1 -systype "Q2 syst." -cuts+ "jtpt>75 " -labels+ "0.55:0.89:jet p_{T} #in (75,#infty) GeV/c" "0.55:0.96:loose cut-based PU ID" -suffix "q2pu_75"


