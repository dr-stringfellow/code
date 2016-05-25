#combine -M HybridNew --frequentist --testStat LHC tH_comb2.txt -H ProfileLikelihood --fork 4 -t-1 | tee all2.log
combine tH_comb2.txt -M HybridNew --frequentist  -s 123 --saveToys --saveHybridResult
