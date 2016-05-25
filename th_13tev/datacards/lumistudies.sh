#!/bin/bash
j=2671.4
j1=$j
arr=( $j 5000 15000 30000 50000 100000 150000 300000 1000000 3000000)
for i in "${arr[@]}"
do
echo $i |tee -a tmp.txt
sed -i ''s/${j}/${i}/g'' /home/sfink/thanalysis/th_13phd/datacards/Lumi.config
./datacards.sh 0 3m 
./datacards.sh 0 4m
combineCards.py tH_3m=tH_3m_0.txt tH_4m=tH_4m_0.txt > tH_comb_${i}.txt

#python changedatacards.py 
#/home/kfloeh/theta/utils2/theta-auto.py asympt_comb.py >> tmp.txt
#j=$i
done
sed -i ''s/${i}/${j1}/g'' /home/sfink/thanalysis/th_13phd/datacards/Lumi.config
#python generatecsv.py
