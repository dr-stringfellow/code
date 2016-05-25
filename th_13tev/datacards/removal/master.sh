# CALCULATE LIMIT WITHOUT SYS (ALL FIXED)
rm empty.txt
echo -n "nominal," >> empty.txt
  freezing=''
  for j in $(cat syst_wobbb.txt); do 
      freezing+="$j,"; 
  done;  
  for k in $(cat bbbsyst.txt); do
      freezing+="$k,";
  done;

  echo $freezing
  echo   "combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _freezing_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" >> empty.txt;" 
  combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _freezing_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%"| awk '{print $5}' >> empty.txt; 


# FREEZE ONLY ONE SYS
  freezing=""

  echo   "combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _freezing_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" >> full.txt;"
  combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _freezing_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  | grep "Expected\ 50.0\%" | awk '{print $5}'> tmp;
  paste -d, empty.txt tmp > tmp_nominal

rm removal_effect.txt
for i in $(cat syst_wobbb.txt); do 
    echo -n "$i  " |tee -a removal_effect.txt; combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${i} | grep "Expected\ 50.0\%" |tee -a removal_effect.txt; 
done

# FREEZE ONLY BBB SYS

echo -n "bbb  " |tee -a removal_effect.txt
freezing=''
for j in $(cat bbbsyst.txt); do 
    freezing+="$j,"; 
done;  

combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _rmbbb_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" |tee -a removal_effect.txt; 


# GET NUMBERS
cat removal_effect.txt | awk '{print $6}' > tmp_removal


# FREEZE ALL BUT ONE SYS
rm onlysys_effect.txt
for i in $(cat syst_wobbb.txt); 
  do echo -n "$i  : " | tee -a onlysys_effect.txt
  freezing=''
  for j in $(cat syst_wobbb.txt); do 
      if [ "$i" != "$j" ]; then
	  freezing+="$j,"; 
      fi
  done;  

  for k in $(cat bbbsyst.txt); do
      freezing+="$k,";
  done;


  combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _freezing_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" |tee -a  onlysys_effect.txt; 
done
# FREEZE ALL BUT BBB SYS
echo -n "bbb  : " |tee -a  onlysys_effect.txt
freezing=''
for j in $(cat syst_wobbb.txt); do 
    freezing+="$j,"; 
done;  


combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _onlybbb_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" >> onlysys_effect.txt; 


#GET NUMBERS
cat onlysys_effect.txt | awk '{print $7}' > tmp_only



##########
# PASTE TOGETHER 
###########
echo "syst,only,removal" > limit.csv
cat tmp_nominal >> limit.csv
paste -d, systnames.txt tmp_only tmp_removal >>limit.csv
sed -i 's/,,//g' limit.csv
#rm tmp*
