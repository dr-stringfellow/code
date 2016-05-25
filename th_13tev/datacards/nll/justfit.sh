for i in {0..50}; do
echo $i>>ML.txt
echo $i
combine -M MaxLikelihoodFit --expectSignal=1 cards/tH_comb_${i}.txt --justFit --rMin=1 --rMax=1 |tee tmp.txt
grep nll tmp.txt
grep nll tmp.txt>>ML.txt
done
