#!/bin/bash
for i in {0..50}; do
if [ $i -eq 0 ]; then
j=0
else
j=$((445+${i}))
k=$((${i}-1))
sed -i ''s/${k}]/${i}]/g'' /home/kfloeh/thanalysis/datacards/13tev-datacards-1/Binning.config
cat Binning.config
fi
done
