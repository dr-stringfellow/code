echo $date > removal_effect.txt

for i in $(cat syst_wobbb.txt); do 
    echo -n "$i  " |tee -a removal_effect.txt; 
    combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _rm_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${i} | grep "Expected\ 50.0\%" |tee -a removal_effect.txt; 
done