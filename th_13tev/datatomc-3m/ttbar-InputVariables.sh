th_datatomc_x tHlep_ct.config -xtitle "log m (W_{thad})" -varexpr "log(tophadwm)" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 3 -xmax 5.5 -overflow 1  -kstest 1 -suffix lep_ttbarreco_logtophadwm -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "log (m (thad-W_{thad}))" -varexpr "log(tophadm-tophadwm)" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 3.5 -xmax 5.5 -overflow 1  -kstest 1 -suffix lep_ttbarreco_logdiffthadwhad -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "#DeltaR (thad jets)" -varexpr "tophaddr" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 3.5  -kstest 1 -suffix lep_ttbarreco_tophaddr -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "|#eta (thad)|" -varexpr "abs(tophadeta)" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 3.7 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_etatophad -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "log p_{T} (thad)" -varexpr "log(tophadpt)" -stackmc 1 -lumi 0 -batch 1  -xmin 3 -xmax 6.5  -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_logptthad -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of first top had jet" -varexpr "jtcsvt[tophadjtidx[0]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -kstest 1 -suffix lep_ttbarreco_csvwhad1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "CSV of second top had jet" -varexpr "jtcsvt[tophadjtidx[1]]" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1 -kstest 1 -suffix lep_ttbarreco_csvwhad2 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "log p_{T} (tlep)" -varexpr "log(topleppt)" -stackmc 1 -lumi 0 -batch 1  -xmin 3 -xmax 6.5  -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_logpttlep -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "rel. H_{T}" -varexpr '(tophadpt+topleppt)/(sumHt+met+(nel==1 && nmu==0)*Alt$(elpt[0],0)+(nel==0 && nmu==1)*Alt$(mupt[0],0))' -batch 1 -nbinsx 20 -xmin 0 -xmax 1  -kstest 1 -suffix lep_ttbarreco_relht -stackmc 1 -lumi 0 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "log m (t_{lep})" -varexpr "log(toplepm)" -stackmc 1 -lumi 0 -batch 1 -nbinsx 20 -xmin 4.5 -xmax 6.5 -overflow 1  -kstest 1 -suffix lep_ttbarreco_logtoplepm -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

#th_datatomc_x tHlep_ct.config -xtitle "jet charge * lepcharge toppies" -varexpr '(jtcharge[tophadjtidx[2]]-jtcharge[toplepjtidx])*(((nel==1 && nmu==0)*Alt$(elcharge[0],0))+((nel==0 && nmu==1)*Alt$(mucharge[0],0)))' -batch 1 -nbinsx 20 -xmin -1.5 -xmax 1.5  -kstest 1 -suffix lep_ttbarreco_jtchargetops -stackmc 1  -lumi 0 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

#th_datatomc_x tHlep_ct.config -xtitle "jet charge * lepcharge whad jets" -varexpr '(jtcharge[tophadjtidx[0]]-jtcharge[tophadjtidx[1]])*(((nel==1 && nmu==0)*Alt$(elcharge[0],0))+((nel==0 && nmu==1)*Alt$(mucharge[0],0)))' -batch 1 -nbinsx 20 -xmin -1.5 -xmax 1.5  -kstest 1 -suffix lep_ttbarreco_jtchargewhad -stackmc 1 -lumi 0 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"


th_datatomc_x tHlep_ct.config -xtitle "|#eta (whad)|" -varexpr "abs(tophadweta)" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 3.7 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_etatopwhad -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"


th_datatomc_x tHlep_ct.config -xtitle "|#eta (whad)0|" -varexpr "abs(jteta[tophadjtidx[0]])" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 3.7 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_etatophad0 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "|#eta (whad)1|" -varexpr "abs(jteta[tophadjtidx[1]])" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 3.7 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_etatophad1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "|#eta (whad)2|" -varexpr "abs(jteta[tophadjtidx[2]])" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 3.7 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_etatophad2 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"


th_datatomc_x tHlep_ct.config -xtitle "|#eta (whad)|" -varexpr "abs(tophadweta)" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 3.7 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_etatopwhad -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"


th_datatomc_x tHlep_ct.config -xtitle "|pt (whad)0|" -varexpr "jtpt[tophadjtidx[0]]" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 400 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_pttophad0 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "|pt (whad)1|" -varexpr "jtpt[tophadjtidx[1]]" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 400 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_pttophad1 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"

th_datatomc_x tHlep_ct.config -xtitle "|pt (whad)2|" -varexpr "jtpt[tophadjtidx[2]]" -stackmc 1 -lumi 0 -batch 1 -xmin 0 -xmax 400 -nbinsx 20 -kstest 1 -suffix lep_ttbarreco_pttophad2 -labels+ "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1"
