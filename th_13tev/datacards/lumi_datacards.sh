    i=$1
    reg=$2
    
    kfactor_tHq=$(sed -n $((${i}+1))'{p;q;}' tHq_kfactors.txt)
    kfactor_tHW=$(sed -n $((${i}+1))'{p;q;}' tHW_kfactors.txt)
    branching=$(sed -n $((${i}+1))'{p;q;}' Branchingfractions.txt)

    if [ $i -eq 0 ]; then
        j=0
        else
        j=$((445+${i}))
    fi


    export TH_MCDIR=/storage/jbod/sfink/th_samples/13tev/mc_mar16/
    export REGION=$reg
    DATACARD=lumi_${3}_tH_${REGION}_${i}
    
    rm ${DATACARD}.txt
    rm ${DATACARD}.root 
    

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"
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

    #QCDScale for SingleTop?
    th_addsftodc_x SYS_SCALEST.config  -datacard ${DATACARD}.txt


    
    #
    # shape systematics
    #

    # PU Unc.
    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic pu -direction Up  -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "1/Weight_PU" "(Weight_PU-1)*2+1"  "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic pu -direction Down   -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "1/Weight_PU" "(Weight_PU-1)/2+1" "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    # JES

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CMS_jes  -direction Down -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -mcpath ${TH_MCDIR}/systematics/JESDOWN/${reg}/naf -varexpr "bdtout[${i}]"
    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CMS_jes  -direction Up -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -mcpath ${TH_MCDIR}/systematics/JESUP/${reg}/naf -varexpr "bdtout[${i}]"

    # JER

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CMS_jer  -direction Down -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -mcpath ${TH_MCDIR}/systematics/JERDOWN/${reg}/naf -varexpr "bdtout[${i}]"
    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CMS_jer  -direction Up -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -mcpath ${TH_MCDIR}/systematics/JERUP/${reg}/naf -varexpr "bdtout[${i}]"


    # Q2 Scale
    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_tHq -direction Up   -mcweights+ "THQ125-madgraph-13tev:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_tHq -direction Down -mcweights+ "THQ125-madgraph-13tev:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_tHW -direction Up   -mcweights+ "THW125-madgraph-13tev:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_tHW -direction Down -mcweights+ "THW125-madgraph-13tev:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttbb -direction Up   -mcweights+ "TTbar-powheg_bb:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttbb -direction Down -mcweights+ "TTbar-powheg_bb:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_tt2b -direction Up   -mcweights+ "TTbar-powheg_2b:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_tt2b -direction Down -mcweights+ "TTbar-powheg_2b:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_tt1b -direction Up   -mcweights+ "TTbar-powheg_b:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_tt1b -direction Down -mcweights+ "TTbar-powheg_b:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttcc -direction Up   -mcweights+ "TTbar-powheg_cc:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttcc -direction Down -mcweights+ "TTbar-powheg_cc:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttlf -direction Up   -mcweights+ "TTbar-powheg_lf:weights_syst[4]/weights_sy st[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttlf -direction Down -mcweights+ "TTbar-powheg_lf:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"


    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttH -direction Up   -mcweights+ "TTH-powheg:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttH -direction Down -mcweights+ "TTH-powheg:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_Wj  -direction Up   -mcweights+ "Wjets:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_Wj  -direction Down -mcweights+ "Wjets:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttW  -direction Up   -mcweights+ "ttW:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_ttW  -direction Down -mcweights+ "ttW:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_VV  -direction Up   -mcweights+ "Diboson:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic Q2scale_VV  -direction Down -mcweights+ "Diboson:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"


 # CSV

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVCErr1  -direction Up -mcweights+ "Weight_CSVCErr1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVCErr1  -direction Down -mcweights+ "Weight_CSVCErr1down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVCErr2  -direction Up -mcweights+ "Weight_CSVCErr2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVCErr2  -direction Down -mcweights+ "Weight_CSVCErr2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVHF  -direction Up -mcweights+ "Weight_CSVHFup" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVHF  -direction Down -mcweights+ "Weight_CSVHFdown" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVHFStats1  -direction Up -mcweights+ "Weight_CSVHFStats1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVHFStats1  -direction Down -mcweights+ "Weight_CSVHFStats1down"  -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVHFStats2  -direction Up -mcweights+ "Weight_CSVHFStats2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVHFStats2  -direction Down -mcweights+ "Weight_CSVHFStats2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVLF  -direction Up -mcweights+ "Weight_CSVLFup" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVLF  -direction Down -mcweights+ "Weight_CSVLFdown" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVLFStats1  -direction Up -mcweights+ "Weight_CSVLFStats1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVLFStats1  -direction Down -mcweights+ "Weight_CSVLFStats1down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVLFStats2  -direction Up -mcweights+ "Weight_CSVLFStats2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"

    th_makedatacards_x tH.config MC.config -lumi ${3} -blind 1  -cuts+ "bdtout[${i}] > -10" -systematic CSVLFStats2  -direction Down -mcweights+ "Weight_CSVLFStats2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THW125-madgraph-13tev:weights_syst[${j}]/weights_syst[0]" "THQ125-madgraph-13tev:${kfactor_tHq}" "THW125-madgraph-13tev:${kfactor_tHW}" "THQ125-madgraph-13tev:${branching}" "THW125-madgraph-13tev:${branching}" "TTH-powheg:${branching}" -varexpr "bdtout[${i}]"


    for k in TH_MCDIR REGION DATACARD; do echo unset $k; unset $k; done
