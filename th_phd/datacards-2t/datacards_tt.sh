for reg in ttbar_cr; do
    for lep in el mu; do
	export TH_MCDIR=/storage/a/singletophiggs/Moriond/winter14-jan28/mc/ana-ichep/
	export REGION=$reg
	export LEPTON=$lep
	export DATACARD=tH_ttbar_cr_${LEPTON}
	echo "${DATACARD}"
	rm ${DATACARD}.txt
	rm ${DATACARD}.root
	th_makedatacards_x tH.config MC.config ${1}
#	mv tH_ttbar_cr_el.root ${DATACARD}.root
#	mv tH_ttbar_cr_el.txt ${DATACARD}.txt
        # rate systematics 
	# pdf
	th_addsftodc_x SYS_PDFGG.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_PDFQG.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_PDFQQ.config -datacard ${DATACARD}.txt
	# scale 
	th_addsftodc_x SYS_SCALETT.config  -datacard ${DATACARD}.txt
        th_addsftodc_x SYS_SCALETTH.config  -datacard ${DATACARD}.txt
        th_addsftodc_x SYS_SCALEV.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_SCALEVV.config -datacard ${DATACARD}.txt
	# other
	th_addsftodc_x SYS_LUMI.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_EFFM.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_EFFE.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_TTLF.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_TTBB.config  -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_TTB.config   -datacard ${DATACARD}.txt
	th_addsftodc_x SYS_TTCC.config  -datacard ${DATACARD}.txt
        # shape systematics with reweighted samples
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_fake_b -direction Up   -mcweights+ "weight_btag_LFup/weight_btag"
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_fake_b -direction Down -mcweights+ "weight_btag_LFdown/weight_btag"
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_eff_b -direction Up   -mcweights+ "weight_btag_HFup/weight_btag"
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_eff_b -direction Down -mcweights+ "weight_btag_HFdown/weight_btag"
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_pu -direction Up   -mcweights+ "weightPU_up/weightPU"
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_pu -direction Down -mcweights+ "weightPU_down/weightPU"
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_topPtcorr -direction Up   -mcweights+ "weight_toppt"
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_topPtcorr -direction Down -mcweights+ "1./weight_toppt"
        # shape systematics with samples in different folders 
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_met -direction Up   -mcpath $TH_MCDIR/systematics/MET_UP/${REGION}_incl/${LEPTON}/eval
	th_makedatacards_x tH.config MC.config -systematic CMS_tHq_met -direction Down -mcpath $TH_MCDIR/systematics/MET_DOWN/${REGION}_incl/${LEPTON}/eval
	th_makedatacards_x tH.config MC.config -systematic CMS_res_j -direction Up   -mcpath $TH_MCDIR/systematics/JER_UP/${REGION}_incl/${LEPTON}/eval
	th_makedatacards_x tH.config MC.config -systematic CMS_res_j -direction Down -mcpath $TH_MCDIR/systematics/JER_DOWN/${REGION}_incl/${LEPTON}/eval
#	th_makedatacards_x tH.config MC.config -systematic CMS_res_b -direction Up   -mcpath $TH_MCDIR/systematics/JER_B_UP/${REGION}_incl/${LEPTON}/eval
#	th_makedatacards_x tH.config MC.config -systematic CMS_res_b -direction Down -mcpath $TH_MCDIR/systematics/JER_B_DOWN/${REGION}_incl/${LEPTON}/eval
#	th_makedatacards_x tH.config MC.config -systematic CMS_scale_j -direction Up   -mcpath $TH_MCDIR/systematics/JES_UP/${REGION}_incl/${LEPTON}/eval
#	th_makedatacards_x tH.config MC.config -systematic CMS_scale_j -direction Down -mcpath $TH_MCDIR/systematics/JES_DOWN/${REGION}_incl/${LEPTON}/eval
#	th_makedatacards_x tH.config MC.config -systematic CMS_scale_b -direction Up   -mcpath $TH_MCDIR/systematics/JES_B_UP/${REGION}_incl/${LEPTON}/eval
#	th_makedatacards_x tH.config MC.config -systematic CMS_scale_b -direction Down -mcpath $TH_MCDIR/systematics/JES_B_DOWN/${REGION}_incl/${LEPTON}/eval

	# 17 different JES systematics
	
	th_makedatacards_x tH.config MC.config -systematic CMS_scale0_j -direction Up   -mcpath $TH_MCDIR/systematics/JES0_UP/${REGION}_incl/${LEPTON}/eval
	th_makedatacards_x tH.config MC.config -systematic CMS_scale0_j -direction Down -mcpath $TH_MCDIR/systematics/JES0_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale1_j -direction Up   -mcpath $TH_MCDIR/systematics/JES1_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale1_j -direction Down -mcpath $TH_MCDIR/systematics/JES1_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale2_j -direction Up   -mcpath $TH_MCDIR/systematics/JES2_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale2_j -direction Down -mcpath $TH_MCDIR/systematics/JES2_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale3_j -direction Up   -mcpath $TH_MCDIR/systematics/JES3_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale3_j -direction Down -mcpath $TH_MCDIR/systematics/JES3_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale4_j -direction Up   -mcpath $TH_MCDIR/systematics/JES4_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale4_j -direction Down -mcpath $TH_MCDIR/systematics/JES4_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale5_j -direction Up   -mcpath $TH_MCDIR/systematics/JES5_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale5_j -direction Down -mcpath $TH_MCDIR/systematics/JES5_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale6_j -direction Up   -mcpath $TH_MCDIR/systematics/JES6_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale6_j -direction Down -mcpath $TH_MCDIR/systematics/JES6_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale7_j -direction Up   -mcpath $TH_MCDIR/systematics/JES7_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale7_j -direction Down -mcpath $TH_MCDIR/systematics/JES7_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale8_j -direction Up   -mcpath $TH_MCDIR/systematics/JES8_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale8_j -direction Down -mcpath $TH_MCDIR/systematics/JES8_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale9_j -direction Up   -mcpath $TH_MCDIR/systematics/JES9_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale9_j -direction Down -mcpath $TH_MCDIR/systematics/JES9_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale10_j -direction Up   -mcpath $TH_MCDIR/systematics/JES10_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale10_j -direction Down -mcpath $TH_MCDIR/systematics/JES10_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale11_j -direction Up   -mcpath $TH_MCDIR/systematics/JES11_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale11_j -direction Down -mcpath $TH_MCDIR/systematics/JES11_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale12_j -direction Up   -mcpath $TH_MCDIR/systematics/JES12_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale12_j -direction Down -mcpath $TH_MCDIR/systematics/JES12_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale13_j -direction Up   -mcpath $TH_MCDIR/systematics/JES13_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale13_j -direction Down -mcpath $TH_MCDIR/systematics/JES13_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale14_j -direction Up   -mcpath $TH_MCDIR/systematics/JES14_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale14_j -direction Down -mcpath $TH_MCDIR/systematics/JES14_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale15_j -direction Up   -mcpath $TH_MCDIR/systematics/JES15_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale15_j -direction Down -mcpath $TH_MCDIR/systematics/JES15_DOWN/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale16_j -direction Up   -mcpath $TH_MCDIR/systematics/JES16_UP/${REGION}_incl/${LEPTON}/eval
        th_makedatacards_x tH.config MC.config -systematic CMS_scale16_j -direction Down -mcpath $TH_MCDIR/systematics/JES16_DOWN/${REGION}_incl/${LEPTON}/eval
        


        # shape systematics with different samples TODO for TTBAR, TH
        th_makedatacards_x tH-ScaleUp.config MC.config         -systematic Q2scale_tH   -direction Up 
        th_makedatacards_x tH-ScaleDown.config MC.config       -systematic Q2scale_tH   -direction Down
	th_makedatacards_x tH.config MC_SCALE_UP_ttbb.config   -systematic Q2scale_tt2b -direction Up 
        th_makedatacards_x tH.config MC_SCALE_DOWN_ttbb.config -systematic Q2scale_tt2b -direction Down
	th_makedatacards_x tH.config MC_SCALE_UP_ttb.config    -systematic Q2scale_tt1b -direction Up 
        th_makedatacards_x tH.config MC_SCALE_DOWN_ttb.config  -systematic Q2scale_tt1b -direction Down
	th_makedatacards_x tH.config MC_SCALE_UP_ttcc.config   -systematic Q2scale_ttcc -direction Up 
        th_makedatacards_x tH.config MC_SCALE_DOWN_ttcc.config -systematic Q2scale_ttcc -direction Down
	th_makedatacards_x tH.config MC_SCALE_UP_ttlf.config   -systematic Q2scale_ttlf -direction Up 
        th_makedatacards_x tH.config MC_SCALE_DOWN_ttlf.config -systematic Q2scale_ttlf -direction Down
        th_makedatacards_x tH.config MC_MATCHING_UP_ttbb.config     -systematic Matching_tt2b -direction Up 
        th_makedatacards_x tH.config MC_MATCHING_DOWN_ttbb.config   -systematic Matching_tt2b -direction Down
	th_makedatacards_x tH.config MC_MATCHING_UP_ttb.config      -systematic Matching_tt1b -direction Up 
        th_makedatacards_x tH.config MC_MATCHING_DOWN_ttb.config    -systematic Matching_tt1b -direction Down
	th_makedatacards_x tH.config MC_MATCHING_UP_ttcc.config     -systematic Matching_ttcc -direction Up 
        th_makedatacards_x tH.config MC_MATCHING_DOWN_ttcc.config   -systematic Matching_ttcc -direction Down
	th_makedatacards_x tH.config MC_MATCHING_UP_ttlf.config     -systematic Matching_ttlf -direction Up 
        th_makedatacards_x tH.config MC_MATCHING_DOWN_ttlf.config   -systematic Matching_ttlf -direction Down
        for i in TH_MCDIR REGION LEPTON DATACARD; do echo unset $i; unset $i; done
    done 
done 


