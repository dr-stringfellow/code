for i in {0..50}; do
    if [ $i -eq 0 ]; then
	j=0
	else
	j=$((445+${i}))
	fi
    for reg in 3t 4t; do


#    if [ "$reg" = "3t" ]; then
#    bins=12
#else
#    bins=8
#fi

    kfactor_tHq=$(sed -n $((${i}+1))'{p;q;}' tHq_kfactors.txt)
    kfactor_tHW=$(sed -n $((${i}+1))'{p;q;}' tHW_kfactors.txt) 
    branching=$(sed -n $((${i}+1))'{p;q;}' Branchingfractions.txt)    
    


echo '

    export TH_MCDIR=/storage/jbod/bmaier/13tev
    export REGION='$reg'
    export DATACARD=tH_${REGION}_'${i}'
    
    rm ${DATACARD}.txt
    rm ${DATACARD}.root 
    

    th_makedatacards_x tH.config MC.config ${1} -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"
    #
    # rate systematics 
    #

    echo ${DATACARD}
    # lumi and ttbar heavy flavor
    th_addsftodc_x SYS_LUMI.config  -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_LEPEFF.config  -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_TTBB.config  -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_TT2B.config   -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_TT1B.config   -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_TTCC.config  -datacard ${DATACARD}.txt
    # pdf
    th_addsftodc_x SYS_PDFGG.config  -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_PDFQG.config  -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_PDFQQ.config -datacard ${DATACARD}.txt
    # scale for diboson and single top -- other samples have systematic weights
    th_addsftodc_x SYS_SCALEVV.config -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_SCALEST.config -datacard ${DATACARD}.txt
    
    #
    # shape systematics
    #


    # Q2 Scale
    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHq -direction Up   -mcweights+ "THQ125-madgraph:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" -varexpr "bdtout['${i}'] "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHq -direction Down -mcweights+ "THQ125-madgraph:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHW -direction Up   -mcweights+ "THW125-madgraph:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHW -direction Down -mcweights+ "THW125-madgraph:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttbb -direction Up   -mcweights+ "TTbar-powheg_bb:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttbb -direction Down -mcweights+ "TTbar-powheg_bb:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt2b -direction Up   -mcweights+ "TTbar-powheg_2b:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt2b -direction Down -mcweights+ "TTbar-powheg_2b:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt1b -direction Up   -mcweights+ "TTbar-powheg_b:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt1b -direction Down -mcweights+ "TTbar-powheg_b:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttcc -direction Up   -mcweights+ "TTbar-powheg_cc:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttcc -direction Down -mcweights+ "TTbar-powheg_cc:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttlf -direction Up   -mcweights+ "TTbar-powheg_lf:weights_syst[4]/weights_sy st[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttlf -direction Down -mcweights+ "TTbar-powheg_lf:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"


    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttH -direction Up   -mcweights+ "TTH-powheg:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttH -direction Down -mcweights+ "TTH-powheg:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_Wj  -direction Up   -mcweights+ "Wjets:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic Q2scale_Wj  -direction Down -mcweights+ "Wjets:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"


 # CSV

    th_makedatacards_x tH.config MC.config -systematic CSVCErr1  -direction Up -mcweights+ "Weight_CSVCErr1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVCErr1  -direction Down -mcweights+ "Weight_CSVCErr1down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVCErr2  -direction Up -mcweights+ "Weight_CSVCErr2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVCErr2  -direction Down -mcweights+ "Weight_CSVCErr2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVHF  -direction Up -mcweights+ "Weight_CSVHFup" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVHF  -direction Down -mcweights+ "Weight_CSVHFdown" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats1  -direction Up -mcweights+ "Weight_CSVHFStats1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats1  -direction Down -mcweights+ "Weight_CSVHFStats1down"  -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats2  -direction Up -mcweights+ "Weight_CSVHFStats2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats2  -direction Down -mcweights+ "Weight_CSVHFStats2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVLF  -direction Up -mcweights+ "Weight_CSVLFup" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVLF  -direction Down -mcweights+ "Weight_CSVLFdown" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats1  -direction Up -mcweights+ "Weight_CSVLFStats1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats1  -direction Down -mcweights+ "Weight_CSVLFStats1down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats2  -direction Up -mcweights+ "Weight_CSVLFStats2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats2  -direction Down -mcweights+ "Weight_CSVLFStats2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -varexpr "bdtout['${i}']"

    # JES

    th_makedatacards_x tH.config MC.config -systematic CMS_jes  -direction Down -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -mcpath '${TH_MCDIR}'/systematics/JESDOWN/'${reg}' -varexpr "bdtout['${i}']"
    th_makedatacards_x tH.config MC.config -systematic CMS_jes  -direction Up -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -mcpath '${TH_MCDIR}'/systematics/JESUP/'${reg}' -varexpr "bdtout['${i}']"

    # JER

    th_makedatacards_x tH.config MC.config -systematic CMS_jer  -direction Down -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -mcpath '${TH_MCDIR}'/systematics/JERDOWN/'${reg}' -varexpr "bdtout['${i}']"
    th_makedatacards_x tH.config MC.config -systematic CMS_jer  -direction Up -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst['${j}']/weights_syst[0]" "THW125-madgraph:weights_syst['${j}']/weights_syst[0]" "THQ125-madgraph:'${kfactor_tHq}'" "THW125-madgraph:'${kfactor_tHW}'" "THQ125-madgraph:'${branching}'" "THW125-madgraph:'${branching}'" "TTH-powheg:'${branching}'" -mcpath '${TH_MCDIR}'/systematics/JERUP/'${reg}' -varexpr "bdtout['${i}']"

    for k in TH_MCDIR REGION DATACARD; do echo unset $k; unset $k; done' 
done  > script${i}.sh
done

