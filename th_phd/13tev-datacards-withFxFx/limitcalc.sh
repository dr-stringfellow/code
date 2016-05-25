for i in {0..50}; do
	#combineCards.py tH_3t=tH_3t_${i}.txt tH_4t=tH_4t_${i}.txt > tH_comb_${i}.txt
	echo $i >> tmp_combine_${i}.txt
	if [ $i -eq 44 ];then
		combine --verbose 9 -t -1 -M Asymptotic tH_comb_44.txt >> tmp_combine_44.txt
	else
		combine -M Asymptotic -t -1 tH_comb_${i}.txt >> tmp_combine_${i}.txt
	fi
#	python /home/kfloeh/cmssw/CMSSW_7_4_2/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py mlfit.root --format latex --all --stol 1.0 | tee diff_${i}.txt
#	sed -i.bak '/_bin/d' diff_${i}.txt
done
for i in {0..50};do
	cat tmp_combine_${i}.txt >> tmp.txt
#	rm tmp_combine_${i}.txt
done
python combine_generatecsv.py
#rm tmp.txt
