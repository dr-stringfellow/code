for i in 5000 7500 10000 15000 30000 50000 100000 150000 300000 1000000 3000000; do
for j in 0 12; do
combineCards.py tH_3m=lumi_${i}_tH_3m_${j}.txt tH_4m=lumi_${i}_tH_4m_${j}.txt > lumi_${i}_tH_comb_${j}.txt
done
done