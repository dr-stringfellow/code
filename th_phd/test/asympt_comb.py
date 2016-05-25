options = Options()
options.set('minimizer', 'strategy', 'newton_vanilla')
model = higgs_datacard.build_model('tH_comb.txt', include_mc_uncertainties=True)
exp,obs = asymptotic_cls_limits(model, use_data = False, options = options)
print exp
res = mle(model, input = 'data', n=1, with_covariance=True, options=options,signal_prior='fix:1')
#print res
model_summary(model)
fitresults = {}
signal = 'tHq'
for i in res['tHq']:
        if '__' not in i:
                fitresults[i] = [res[signal][i][0][0], res[signal][i][0][1]]

f = open('fitresults/nominal.txt','w')
for key in sorted(fitresults.keys()):
        line = '%s %f %f\n' % (key, fitresults[key][0], fitresults[key][1])
#        print line,
        f.write(line)
f.close()
parameter_values = {}
for p in model.get_parameters([]):
  parameter_values[p] = res['tHq'][p][0][0]
#print model.get_parameters([])
