model = higgs_datacard.build_model('tH_comb.txt', include_mc_uncertainties=True)
options = Options()
options.set('minimizer', 'strategy', 'newton_vanilla')
exp = asymptotic_cls_limits(model, use_data = False,  options = options)
print exp
#model_summary(model, True, True, True)
