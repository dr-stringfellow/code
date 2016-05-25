combine -M MaxLikelihoodFit tH_comb.txt --seed -1 -n asy
#python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py  -A mlfitasy.root | grep tHq_tt | grep -v bin
#python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py  -aA mlfitasy.root -f latex ${2} > full_outcome_mc.tex
python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -A mlfitasy.root  --vtol 1.0 --stol 0.99 --vtol2 2.0 --stol2 0.99 -f text ${2} >> outcome.tex
#python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -A mlfitasy.root  --vtol 99. --stol 0.50 --vtol2 99. --stol2 0.99 -f text ${2} #>> outcome.tex
