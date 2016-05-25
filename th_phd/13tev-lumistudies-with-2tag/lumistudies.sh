#!/bin/bash
j=2198
j1=$j
arr=( $j 3000 5000 15000 30000 50000 100000 150000 300000 1000000 3000000)
for i in "${arr[@]}"
do
echo 
echo $i>> tmp_combine_0_$i.txt
echo $i>> tmp_combine_12_$i.txt
sed -i ''s/${j}/${i}/g'' /home/bmaier/thanalysis/th_phd/13tev-lumistudies-with-2tag/Lumi.config
. script0.sh
. script12.sh

python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal tH_2t_0.root  tH_2t_0.root --writeDC
python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal tH_2t_12.root  tH_2t_12.root --writeDC

combineCards.py tH_2t=tH_2t_0.txt  tH_3t=tH_3t_0.txt  tH_4t=tH_4t_0.txt > tH_comb_0.txt
combineCards.py tH_2t=tH_2t_12.txt tH_3t=tH_3t_12.txt tH_4t=tH_4t_12.txt > tH_comb_12.txt

combine -M Asymptotic -t -1 tH_comb_0.txt >> tmp_combine_0_$i.txt
combine -M Asymptotic -t -1 tH_comb_12.txt >> tmp_combine_12_$i.txt
j=$i
done
sed -i ''s/${i}/${j1}/g'' /home/bmaier/thanalysis/th_phd/13tev-lumistudies/Lumi.config
python generatecsv.py
