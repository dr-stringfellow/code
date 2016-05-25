echo $date > full.txt
echo -n "nominal  : " >> full.txt
  echo   "combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _fit_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq | grep "Expected\ 50.0\%" >> full.txt;" 
          combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _fit_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq | grep "Expected\ 50.0\%" >> full.txt; 


#combine -M Asymptotic -d higgsCombinenicholas2.MultiDimFit.mH120.root -w w -n nicholas3 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${i} | grep "Expected\ 50.0\%" >> removal_effect.txt; done
#  LocalWords:  cOmbine
