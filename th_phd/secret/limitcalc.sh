echo "0" > tmp.txt
combineCards.py tH_3t=tH_3t_0.txt tH_4t=tH_4t_0.txt > tH_comb.txt
combine -M Asymptotic -t -1 tH_comb.txt >> tmp.txt
python combine_generatecsv.py
#rm tmp.txt
