echo $date > removal_bbb.txt
echo -n "bbb  : " >> removal_bbb.txt
freezing=''
for j in $(cat bbbsyst.txt); do 
    freezing+="$j,"; 
done;  

combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _rmbbb_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" |tee -a removal_bbb.txt; 


#combine -M Asymptotic -d higgsCombinenicholas2.MultiDimFit.mH120.root -w w -n nicholas3 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${i} | grep "Expected\ 50.0\%" >> removal_effect.txt; done
#  LocalWords:  cOmbine
