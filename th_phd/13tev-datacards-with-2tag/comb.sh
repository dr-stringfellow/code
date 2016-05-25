for i in {0..50}; do
    combineCards.py tH_2t=tH_2t_${i}.txt tH_3t=tH_3t_${i}.txt tH_4t=tH_4t_${i}.txt > tH_comb_${i}.txt
done