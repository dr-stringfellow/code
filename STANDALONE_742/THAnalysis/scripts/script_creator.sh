## th_addvariables_x

for i in $(ls *root); do echo th_addvariables_x -inputs $i; done > addvariables.sh

## th_csvreweight_x

for i in $(ls *root); do echo th_csvreweight_x -inputs $i; done > csvreweight.sh

## th_lepscales_x

for i in $(ls *root); do echo th_lepscales_x -inputs $i; done > lepscalefactors.sh

## th_toppt_x

for i in $(ls * | grep .root);
  do echo -ne "th_toppt_x -inputs $i";
  if [[ "$i" == *FullLept*.root ]]; then
      echo -ne " -isFull 1";
  fi;
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -isSemi 1";
  fi;
  if [[ "$i" == TTbar-madgraph-Summer12.root ]]; then
      echo -ne " -isIncl 1";
  fi;
  echo;
done > topptreweight.sh


## th_toprecexpertpop_x

for i in $(ls * | grep .root);
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i";
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > nntopexpert.sh


## th_recexpertpop_x

for i in $(ls * | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i";
  if [[ "$i" == TH-*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > nnexpert.sh


## th_bdtexpertpop_x

for i in $(ls * | grep .root);
  do echo -ne "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i";
  echo;
done > classification.sh

for i in $(ls * | grep .root);
  do echo -ne "th_bdtexpert2_x -bdtpath /portal/ekpcms5/home/sfink/thanalysis/th_Moriond/bdt-class-csv/th_popvar_fancy_150/weights -input $i";
  echo;
done > bdtg-class.sh


## th_addtransform_x (mlpout)

export PWD=$(pwd)

for i in $(ls * | grep .root);
  do echo -ne "th_addtransform_x -bdtpath /portal/ekpcms5/home/sfink/thanalysis/th_Moriond/mva-";
  if [[ "$PWD" == *4tt_incl* ]]; then
      echo -ne "4tt";
  else 
      echo -ne "3tt";
  fi
  if [[ "$PWD" == */mu ]]; then
      echo -ne "-binning-mu -input $i -bdtout mlpout";
  else
      echo -ne "-binning-el -input $i -bdtout mlpout";
  fi
  echo;
done > transform.sh


## th_addtransform_x (bdtgout)

for i in $(ls * | grep .root);
  do echo -ne "th_addtransform_x -bdtpath /portal/ekpcms5/home/sfink/thanalysis/th_Moriond/mva-";
  if [[ "$PWD" == *4tt_incl* ]]; then
      echo -ne "4tt";
  else 
      echo -ne "3tt";
  fi
  if [[ "$PWD" == */mu ]]; then
      echo -ne "-binning-mu -input $i -bdtout bdtgout";
  else
      echo -ne "-binning-el -input $i -bdtout bdtgout";
  fi
  echo;
done > transform_bdt.sh



