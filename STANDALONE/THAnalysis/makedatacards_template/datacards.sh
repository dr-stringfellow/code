for reg in 3tt; do
    for lep in mu; do
	export TH_MCDIR=/storage/a/singletophiggs/Moriond/winter14-jan28/mc/ana-nominal/
	export REGION=$reg
	export LEPTON=$lep
	export DATACARD=tH_${REGION}_${LEPTON}
	rm ${DATACARD}.txt
	rm ${DATACARD}.root
	th_makedatacards_x tH.config MC.config ${1}
        # rate systematics 
	# pdf
	th_addsftodc_x SYS_PDFGG_TT.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_PDFGG_TTH.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_PDFQG_ST.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_PDFQQ_W.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_PDFQQ_Z.config -datacard ${DATACARD}.txt
	# scale 
	th_addsftodc_x SYS_SCALETT_TT.config  -datacard ${DATACARD}.txt
        th_addsftodc_x SYS_SCALETTH_TTH.config  -datacard ${DATACARD}.txt
        th_addsftodc_x SYS_SCALETT_ST.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_SCALEVV_VV.config -datacard ${DATACARD}.txt
        th_addsftodc_x SYS_SCALEV_W.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_SCALEV_Z.config -datacard ${DATACARD}.txt
	# other
	th_addsftodc_x SYS_LUMI.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_EFFM.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_EFFE.config  -datacard ${DATACARD}.txt
        # shape systematics with reweighted samples
	th_makedatacards_x tH.config MC.config -systematic btag   -direction Up   -mcweights+ "weight_csv_hfup/weight_csv"
	th_makedatacards_x tH.config MC.config -systematic btag   -direction Down -mcweights+ "weight_csv_hfdown/weight_csv"
	th_makedatacards_x tH.config MC.config -systematic mistag -direction Up   -mcweights+ "weight_csv_lfup/weight_csv"
	th_makedatacards_x tH.config MC.config -systematic mistag -direction Down -mcweights+ "weight_csv_lfdown/weight_csv"
	th_makedatacards_x tH.config MC.config -systematic pileup -direction Up   -mcweights+ "weightPU_up/weightPU"
	th_makedatacards_x tH.config MC.config -systematic pileup -direction Down -mcweights+ "weightPU_down/weightPU"
	th_makedatacards_x tH.config MC.config -systematic toppt  -direction Up   -mcweights+ "weight_toppt"
	th_makedatacards_x tH.config MC.config -systematic toppt  -direction Down -mcweights+ "1./weight_toppt"
        # shape systematics with samples in different folders 
	th_makedatacards_x tH.config MC.config -systematic met -direction Up   -mcpath $TH_MCDIR/systematics/MET_UP/${REGION}_incl/${LEPTON}
	th_makedatacards_x tH.config MC.config -systematic met -direction Down -mcpath $TH_MCDIR/systematics/MET_DOWN/${REGION}_incl/${LEPTON}
	th_makedatacards_x tH.config MC.config -systematic jer -direction Up   -mcpath $TH_MCDIR/systematics/JER_UP/${REGION}_incl/${LEPTON}
	th_makedatacards_x tH.config MC.config -systematic jer -direction Down -mcpath $TH_MCDIR/systematics/JER_DOWN/${REGION}_incl/${LEPTON}
	th_makedatacards_x tH.config MC.config -systematic jes -direction Up   -mcpath $TH_MCDIR/systematics/JES_UP/${REGION}_incl/${LEPTON}
	th_makedatacards_x tH.config MC.config -systematic jes -direction Down -mcpath $TH_MCDIR/systematics/JES_DOWN/${REGION}_incl/${LEPTON}
        # shape systematics with different samples TODO for TTBAR, TH
        th_makedatacards_x tH-ScaleUp.config MC_SCALE_UP.config     -systematic scale -direction Up 
        th_makedatacards_x tH-ScaleDown.config MC_SCALE_DOWN.config -systematic scale -direction Down
        #th_makedatacards_x tH.config MC_MATCHING_UP.config   -systematic match -direction Up 
        #th_makedatacards_x tH.config MC_MATCHING_DOWN.config -systematic match -direction Down
        for i in TH_MCDIR REGION LEPTON DATACARD; do echo unset $i; unset $i; done
    done 
done 


