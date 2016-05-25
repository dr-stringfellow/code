python diffNuisances.py fits/mlfit_0.root  --all --stol 1.0 | tee diff_data.txt
python diffNuisances.py fits/mlfit_asimov_0.root  --all --stol 1.0 | tee diff_asimov.txt
