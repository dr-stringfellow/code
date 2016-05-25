lalala 


    export TH_MCDIR=/storage/jbod/bmaier/13tev
    export REGION=3t
    export DATACARD=tH_${REGION}_5
    
    rm ${DATACARD}.txt
    rm ${DATACARD}.root 
    

    th_makedatacards_x tH.config MC.config ${1} -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16
    #
    # rate systematics 
    #

    echo ${DATACARD}
    # lumi and ttbar heavy flavor
    th_addsftodc_x SYS_LUMI.config  -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_LEPEFF.config  -datacard ${DATACARD}.txt
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
    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHq -direction Up   -mcweights+ "THQ125-madgraph:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHq -direction Down -mcweights+ "THQ125-madgraph:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHW -direction Up   -mcweights+ "THW125-madgraph:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHW -direction Down -mcweights+ "THW125-madgraph:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttbb -direction Up   -mcweights+ "TTbar-powheg_bb:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttbb -direction Down -mcweights+ "TTbar-powheg_bb:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt2b -direction Up   -mcweights+ "TTbar-powheg_2b:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt2b -direction Down -mcweights+ "TTbar-powheg_2b:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt1b -direction Up   -mcweights+ "TTbar-powheg_b:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt1b -direction Down -mcweights+ "TTbar-powheg_b:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttcc -direction Up   -mcweights+ "TTbar-powheg_cc:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttcc -direction Down -mcweights+ "TTbar-powheg_cc:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttlf -direction Up   -mcweights+ "TTbar-powheg_lf:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttlf -direction Down -mcweights+ "TTbar-powheg_lf:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 


    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttH -direction Up   -mcweights+ "TTH-powheg:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 
 
    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttH -direction Down -mcweights+ "TTH-powheg:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_Wj  -direction Up   -mcweights+ "Wjets:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_Wj  -direction Down -mcweights+ "Wjets:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 


 # CSV

    th_makedatacards_x tH.config MC.config -systematic CSVCErr1  -direction Up -mcweights+ "Weight_CSVCErr1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVCErr1  -direction Down -mcweights+ "Weight_CSVCErr1down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVCErr2  -direction Up -mcweights+ "Weight_CSVCErr2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVCErr2  -direction Down -mcweights+ "Weight_CSVCErr2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVHF  -direction Up -mcweights+ "Weight_CSVHFup" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVHF  -direction Down -mcweights+ "Weight_CSVHFdown" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats1  -direction Up -mcweights+ "Weight_CSVHFStats1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats1  -direction Down -mcweights+ "Weight_CSVHFStats1down"  -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats2  -direction Up -mcweights+ "Weight_CSVHFStats2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats2  -direction Down -mcweights+ "Weight_CSVHFStats2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVLF  -direction Up -mcweights+ "Weight_CSVLFup" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVLF  -direction Down -mcweights+ "Weight_CSVLFdown" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats1  -direction Up -mcweights+ "Weight_CSVLFStats1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats1  -direction Down -mcweights+ "Weight_CSVLFStats1down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats2  -direction Up -mcweights+ "Weight_CSVLFStats2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats2  -direction Down -mcweights+ "Weight_CSVLFStats2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 16 

    # JES

    th_makedatacards_x tH.config MC.config -systematic CMS_jes  -direction Down -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -mcpath ${TH_MCDIR}/systematics/JESDOWN/3t -varexpr "bdtout[5]" -nbinsx 16 
    th_makedatacards_x tH.config MC.config -systematic CMS_jes  -direction Up -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -mcpath ${TH_MCDIR}/systematics/JESUP/3t -varexpr "bdtout[5]" -nbinsx 16 

    # JER

    th_makedatacards_x tH.config MC.config -systematic CMS_jer  -direction Down -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -mcpath ${TH_MCDIR}/systematics/JERDOWN/3t -varexpr "bdtout[5]" -nbinsx 16 
    th_makedatacards_x tH.config MC.config -systematic CMS_jer  -direction Up -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -mcpath ${TH_MCDIR}/systematics/JERUP/3t -varexpr "bdtout[5]" -nbinsx 16 

    for k in TH_MCDIR REGION DATACARD; do echo unset $k; unset $k; done
lalala 


    export TH_MCDIR=/storage/jbod/bmaier/13tev
    export REGION=4t
    export DATACARD=tH_${REGION}_5
    
    rm ${DATACARD}.txt
    rm ${DATACARD}.root 
    

    th_makedatacards_x tH.config MC.config ${1} -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8
    #
    # rate systematics 
    #

    echo ${DATACARD}
    # lumi and ttbar heavy flavor
    th_addsftodc_x SYS_LUMI.config  -datacard ${DATACARD}.txt
    th_addsftodc_x SYS_LEPEFF.config  -datacard ${DATACARD}.txt
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
    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHq -direction Up   -mcweights+ "THQ125-madgraph:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHq -direction Down -mcweights+ "THQ125-madgraph:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHW -direction Up   -mcweights+ "THW125-madgraph:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tHW -direction Down -mcweights+ "THW125-madgraph:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttbb -direction Up   -mcweights+ "TTbar-powheg_bb:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttbb -direction Down -mcweights+ "TTbar-powheg_bb:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt2b -direction Up   -mcweights+ "TTbar-powheg_2b:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt2b -direction Down -mcweights+ "TTbar-powheg_2b:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt1b -direction Up   -mcweights+ "TTbar-powheg_b:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_tt1b -direction Down -mcweights+ "TTbar-powheg_b:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttcc -direction Up   -mcweights+ "TTbar-powheg_cc:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttcc -direction Down -mcweights+ "TTbar-powheg_cc:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttlf -direction Up   -mcweights+ "TTbar-powheg_lf:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttlf -direction Down -mcweights+ "TTbar-powheg_lf:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 


    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttH -direction Up   -mcweights+ "TTH-powheg:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 
 
    th_makedatacards_x tH.config MC.config -systematic Q2scale_ttH -direction Down -mcweights+ "TTH-powheg:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_Wj  -direction Up   -mcweights+ "Wjets:weights_syst[4]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic Q2scale_Wj  -direction Down -mcweights+ "Wjets:weights_syst[8]/weights_syst[0]" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 


 # CSV

    th_makedatacards_x tH.config MC.config -systematic CSVCErr1  -direction Up -mcweights+ "Weight_CSVCErr1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVCErr1  -direction Down -mcweights+ "Weight_CSVCErr1down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVCErr2  -direction Up -mcweights+ "Weight_CSVCErr2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVCErr2  -direction Down -mcweights+ "Weight_CSVCErr2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVHF  -direction Up -mcweights+ "Weight_CSVHFup" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVHF  -direction Down -mcweights+ "Weight_CSVHFdown" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats1  -direction Up -mcweights+ "Weight_CSVHFStats1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats1  -direction Down -mcweights+ "Weight_CSVHFStats1down"  -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats2  -direction Up -mcweights+ "Weight_CSVHFStats2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVHFStats2  -direction Down -mcweights+ "Weight_CSVHFStats2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVLF  -direction Up -mcweights+ "Weight_CSVLFup" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVLF  -direction Down -mcweights+ "Weight_CSVLFdown" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats1  -direction Up -mcweights+ "Weight_CSVLFStats1up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats1  -direction Down -mcweights+ "Weight_CSVLFStats1down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats2  -direction Up -mcweights+ "Weight_CSVLFStats2up" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    th_makedatacards_x tH.config MC.config -systematic CSVLFStats2  -direction Down -mcweights+ "Weight_CSVLFStats2down" -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -varexpr "bdtout[5]" -nbinsx 8 

    # JES

    th_makedatacards_x tH.config MC.config -systematic CMS_jes  -direction Down -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -mcpath ${TH_MCDIR}/systematics/JESDOWN/4t -varexpr "bdtout[5]" -nbinsx 8 
    th_makedatacards_x tH.config MC.config -systematic CMS_jes  -direction Up -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -mcpath ${TH_MCDIR}/systematics/JESUP/4t -varexpr "bdtout[5]" -nbinsx 8 

    # JER

    th_makedatacards_x tH.config MC.config -systematic CMS_jer  -direction Down -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -mcpath ${TH_MCDIR}/systematics/JERDOWN/4t -varexpr "bdtout[5]" -nbinsx 8 
    th_makedatacards_x tH.config MC.config -systematic CMS_jer  -direction Up -datacard ${DATACARD}.txt -rootfile ${DATACARD}.root -mcweights+ "THQ125-madgraph:weights_syst[450]/weights_syst[0]" "THW125-madgraph:weights_syst[450]/weights_syst[0]" "THQ125-madgraph:0.9963659371" "THW125-madgraph:1.0040079588" "THQ125-madgraph:1" "THW125-madgraph:1" "TTH-powheg:1" -mcpath ${TH_MCDIR}/systematics/JERUP/4t -varexpr "bdtout[5]" -nbinsx 8 

    for k in TH_MCDIR REGION DATACARD; do echo unset $k; unset $k; done
