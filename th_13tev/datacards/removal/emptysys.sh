echo $date > empty.txt
echo -n "empty  : " >> empty.txt
  freezing=''
  for j in $(cat syst_wobbb.txt); do 
      freezing+="$j,"; 
  done;  
  for k in $(cat bbbsyst.txt); do
      freezing+="$k,";
  done;

  echo $freezing
  echo   "combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _fit_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" >> empty.txt;" 
          combine -M Asymptotic -d higgsCombine_0.MultiDimFit.mH120.root -w w -n _fit_0 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${freezing%,}| grep "Expected\ 50.0\%" >> empty.txt; 


#combine -M Asymptotic -d higgsCombinenicholas2.MultiDimFit.mH120.root -w w -n nicholas3 --snapshotName MultiDimFit --toysFrequentist --bypassFreq  --freezeNuisances ${i} | grep "Expected\ 50.0\%" >> removal_effect.txt; done
#  LocalWords:  cOmbine