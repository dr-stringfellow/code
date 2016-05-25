for i in {0..50};do
	cat tmp_combine_${i}.txt >> tmp.txt
done
python combine_generatecsv.py

