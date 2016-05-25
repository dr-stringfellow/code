model = higgs_datacard.build_model('tH_3tt_mu.txt', include_mc_uncertainties=True)
model_summary(model, True , True, True)
#model = frequentist.frequentize_model(model)
#exp,obs = cls_limits(model, use_data = False,cls_options = {'expected_bands':2000})
#print exp

