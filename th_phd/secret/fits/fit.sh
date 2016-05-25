for i in {0..50}; do 
echo $i
combine -M MaxLikelihoodFit  ../tH_comb_$i.txt --saveNLL --saveNormalizations --saveWithUncertainties
python mlfitNormsToText.py -u mlfit.root | tee mlfit_$i.txt
done