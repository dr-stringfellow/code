#!/bin/bash
j=2215
j1=$j
arr=( $j 3000 5000 15000 30000 50000 100000 150000 300000 1000000 3000000)
for i in "${arr[@]}"
do
echo $i>> tmp.txt
sed -i ''s/${j}/${i}/g'' /home/kfloeh/thanalysis/datacards/13tev-datacards-0/Lumi.config
./blind.sh
./combine_txt.sh
python changedatacards.py 
/home/kfloeh/theta/utils2/theta-auto.py asympt_comb.py >> tmp.txt
j=$i
done
sed -i ''s/${i}/${j1}/g'' /home/kfloeh/thanalysis/datacards/13tev-datacards-0/Lumi.config
python generatecsv.py
