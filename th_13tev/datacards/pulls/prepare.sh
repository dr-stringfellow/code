###Script for preparing NuisanceDiff for plotting

#1. Removing MC stat nuisances - nobody needs them!
sed -i.bak '/_bin/d' diff_data.txt

sed -i 's/*//g'  diff_data.txt
sed -e 's/,//g'  diff_data.txt  > tmp.txt
sed -e 's/!//g'  tmp.txt  > tmp1.txt

#2. Replacing all spaces by a tab
awk -v OFS="\t" '$1=$1' tmp1.txt > tmp2.txt

#3. Cutting to relevant columns (s+b fit)
cut  -f1,4-5 tmp2.txt > tmp3.txt

#4. Delete commas
#5. Replace tabs with commas
sed -e 's/\t/,/g' tmp3.txt >tmp4.txt

# Move and delete
mv tmp4.txt pulls.csv
rm tmp*.txt

sed -i.bak '/_bin/d' diff_asimov.txt

sed -i 's/*//g'  diff_asimov.txt
sed -e 's/,//g'  diff_asimov.txt  > tmp.txt
sed -e 's/!//g'  tmp.txt  > tmp1.txt

#2. Replacing all spaces by a tab
awk -v OFS="\t" '$1=$1' tmp1.txt > tmp2.txt

#3. Cutting to relevant columns (s+b fit)
cut  -f4-5 tmp2.txt > tmp3.txt
sed -i 's/s+b/sb-pre/g' tmp3.txt
sed -i 's/fit/fit-pre/g' tmp3.txt

paste pulls.csv tmp3.txt >tmp4.txt

#4. Delete commas
#5. Replace tabs with commas
sed -e 's/\t/,/g' tmp4.txt > tmp5.txt

# Move and delete
mv tmp5.txt pulls.csv
rm tmp*.txt