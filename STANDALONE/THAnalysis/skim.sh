for i in `seq 2 4`;
do
th_skim_x -cuts "(genevt_ttxid%100)==0" -input "/storage/a/singletophiggs/13tev/mc_nov16/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8.root" -output "/storage/jbod/kfloeh/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8_lf.root"
th_skim_x -cuts "(genevt_ttxid%100)==51" -input "/storage/a/singletophiggs/13tev/mc_nov16/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8.root" -output "/storage/jbod/kfloeh/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8_b.root"
th_skim_x -cuts "(genevt_ttxid%100)==52" -input "/storage/a/singletophiggs/13tev/mc_nov16/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8.root" -output "/storage/jbod/kfloeh/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8_2b.root"
th_skim_x -cuts "(genevt_ttxid%100)==53||(genevt_ttxid%100)==54||(genevt_ttxid%100)==55" -input "/storage/a/singletophiggs/13tev/mc_nov16/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8.root" -output "/storage/jbod/kfloeh/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8_bb.root"
th_skim_x -cuts "(genevt_ttxid%100)==41||(genevt_ttxid%100)==42||(genevt_ttxid%100)==43||(genevt_ttxid%100)==44||(genevt_ttxid%100)==45" -input "/storage/a/singletophiggs/13tev/mc_nov16/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8.root" -output "/storage/jbod/kfloeh/${i}t/TT_TuneCUETP8M1_13TeV-powheg-pythia8_cc.root"
done
