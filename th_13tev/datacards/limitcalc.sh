for i in {0..50}; do
    echo Caluculating Limit $i:
#	combineCards.py tH_3m=cards/tH_3m_${i}.txt tH_4m=cards/tH_4m_${i}.txt > cards/tH_comb_${i}.txt
	echo $i > limits/tmp_combine_${i}.txt
#	if [ $i -eq 44 ];then
#		combine --verbose 9 -t -1 -M Asymptotic tH_comb_44.txt >> tmp_combine_44.txt
#	else
	combine -M Asymptotic  cards/tH_comb_${i}.txt >> limits/tmp_combine_${i}.txt
#	fi
#	python /home/kfloeh/cmssw/CMSSW_7_4_2/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py mlfit.root --format latex --all --stol 1.0 | tee diff_${i}.txt
#	sed -i.bak '/_bin/d' diff_${i}.txt
done
for i in {0..50};do
	cat limits/tmp_combine_${i}.txt >> limits/tmp.txt
#	rm tmp_combine_${i}.txt
done
#python combine_generatecsv.py
#rm tmp.txt
