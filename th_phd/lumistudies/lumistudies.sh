#!/bin/bash
rm tmp_combine_0.txt tmp_combine_12.txt
j=2198
j1=$j
arr=( $j 3000 5000 15000 30000 50000 100000 150000 300000 700000 1000000 1500000 3000000)
for i in "${arr[@]}"
do
echo $i>> tmp_0.txt
echo $i>> tmp_12.txt
sed -i ''s/${j}/${i}/g'' /home/bmaier/thanalysis/th_phd/lumistudies/Lumi.config
. script0.sh
. script12.sh
combineCards.py tH_3t=tH_3t_0.txt tH_4t=tH_4t_0.txt > tH_comb_0.txt
combineCards.py tH_3t=tH_3t_12.txt tH_4t=tH_4t_12.txt > tH_comb_12.txt
combine -M Asymptotic tH_comb_0.txt >> tmp_combine_0.txt
combine -M Asymptotic tH_comb_12.txt >> tmp_combine_12.txt
j=$i
done
sed -i ''s/${i}/${j1}/g'' /home/bmaier/thanalysis/th_phd/lumistudies/Lumi.config
python generatecsv.py
