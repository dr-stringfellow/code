for i in wolframh1 aplanarity lepcharge logm3; do

python combine_sys_local.py -i ${i}_bkg.dat ${i}_bkg_CSVCErr1_down.dat ${i}_bkg_CSVCErr2_down.dat ${i}_bkg_CSVHFStats1_down.dat ${i}_bkg_CSVHFStats2_down.dat ${i}_bkg_CSVHF_down.dat ${i}_bkg_CSVLFStats1_down.dat ${i}_bkg_CSVLFStats2_down.dat ${i}_bkg_CSVLF_down.dat ${i}_bkg_JER_down.dat ${i}_bkg_JES_down.dat ${i}_bkg_q2_down.dat -o ${i}_bkg_tot

done