TARGETDIR=${PWD}
COPYDIR=/storage/a/singletophiggs/Moriond/winter14-jan07


mkdir -p mc/ana-nominal/3tt_incl
mkdir -p mc/ana-nominal/ttbar_cr_incl


mkdir -p data/ana-nominal/3tt_incl
mkdir -p data/ana-nominal/ttbar_cr_incl

mkdir -p mc/ana-nominal/systematics/JES_UP/3tt_incl
mkdir -p mc/ana-nominal/systematics/JES_UP/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/JES_DOWN/3tt_incl
mkdir -p mc/ana-nominal/systematics/JES_DOWN/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/JER_UP/3tt_incl
mkdir -p mc/ana-nominal/systematics/JER_UP/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/JER_DOWN/3tt_incl
mkdir -p mc/ana-nominal/systematics/JER_DOWN/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/BTAG_UP/3tt_incl
mkdir -p mc/ana-nominal/systematics/BTAG_UP/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/BTAG_DOWN/3tt_incl
mkdir -p mc/ana-nominal/systematics/BTAG_DOWN/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/MISS_UP/3tt_incl
mkdir -p mc/ana-nominal/systematics/MISS_UP/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/MISS_DOWN/3tt_incl
mkdir -p mc/ana-nominal/systematics/MISS_DOWN/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/MET_UP/3tt_incl
mkdir -p mc/ana-nominal/systematics/MET_UP/ttbar_cr_incl
mkdir -p mc/ana-nominal/systematics/MET_DOWN/3tt_incl
mkdir -p mc/ana-nominal/systematics/MET_DOWN/ttbar_cr_incl

cd $COPYDIR
for i in $(find . -iname "*config"); do echo "cp $i $TARGETDIR/$i"; done > tmpcopy.sh
for i in $(find . -iname "*txt"); do echo "cp $i $TARGETDIR/$i"; done >> tmpcopy.sh
. tmpcopy.sh
rm tmpcopy.sh
cd $TARGETDIR

####################
##       MC       ##
####################

#for i in $(find -iname "*sh"); do "cp $COPYDIR/$i $TARGETDIR/$i"; done

cd $TARGETDIR/mc

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ana-nominal.config -input $i -output ana-nominal/$i"; done > skim_ana_nominal.sh


### ana-nominal

cd ana-nominal

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl.config -input $i -output 3tt_incl/$i"; done > skim_3tt_incl.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl.config -input $i -output ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl.sh

for i in $(ls $TARGETDIR/mc | grep .root); 
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

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_csvreweight_x -inputs $i"; done > csvreweight.sh

## ana-nominal/3tt_incl

cd ./3tt_incl


for i in $(ls $TARGETDIR/mc | grep .root);
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


for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_csvreweight_x -inputs $i"; done > csvreweight.sh

for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > nnexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > nntopexpert.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i"; done > classification.sh

## ana-nominal/ttbar_cr_incl

cd ../ttbar_cr_incl

for i in $(ls $TARGETDIR/mc | grep .root);
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

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_csvreweight_x -inputs $i"; done > csvreweight.sh

for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > nnexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > nntopexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i"; done > classification.sh

## ana-nominal/systematics

cd ../systematics

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_scalejes_x -inputs ../$i -opath ./JES_UP  -jesfile $(find -iname "*MC*AK5PF*.txt")  -systematic  1"; done > jes_up.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_scalejes_x -inputs ../$i -opath ./JES_DOWN -jesfile $(find -iname "*MC*AK5PF*.txt") -systematic -1"; done > jes_down.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_scalejer_x -inputs ../$i -opath ./JER_UP   -systematic  1"; done > jer_up.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_scalejer_x -inputs ../$i -opath ./JER_DOWN -systematic -1"; done > jer_down.sh

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_metrecalculator_x -inputs ../$i -opath ./MET_UP -systematic 1"; done > met_up.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_metrecalculator_x -inputs ../$i -opath ./MET_DOWN -systematic -1"; done > met_down.sh



## ana-nominal/systematics/JER_UP

cd ./JER_UP


for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i"; done > 3tt_incl/classification.sh

cp ../../*incl.config .
cp 3tt_incl/*sh ttbar_cr_incl/.

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl.config -input $i -output 3tt_incl/$i"; done > skim_3tt_incl.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl.config -input $i -output ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl.sh


cd ..

## ana-nominal/systematics/JES_UP                                                                                                                                                                                                                                   

cd ./JES_UP


for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh



for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i"; done > 3tt_incl/classification.sh

cp ../../*incl.config .
cp 3tt_incl/*sh ttbar_cr_incl/.

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl.config -input $i -output 3tt_incl/$i"; done > skim_3tt_incl.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl.config -input $i -output ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl.sh

cd ..

## ana-nominal/systematics/JER_DOWN                                                                                                                                                                                                                                       

cd ./JER_DOWN


for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i"; done > 3tt_incl/classification.sh

cp ../../*incl.config .
cp 3tt_incl/*sh ttbar_cr_incl/.

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl.config -input $i -output 3tt_incl/$i"; done > skim_3tt_incl.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl.config -input $i -output ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl.sh

cd ..

## ana-nominal/systematics/JES_DOWN                                                                                                                                                                                                                                       

cd ./JES_DOWN


for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i"; done > 3tt_incl/classification.sh

cp ../../*incl.config .
cp 3tt_incl/*sh ttbar_cr_incl/.

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl.config -input $i -output 3tt_incl/$i"; done > skim_3tt_incl.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl.config -input $i -output ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl.sh

cd ..

## ana-nominal

cd ..

cat 3tt_incl.config  | sed -e 's/jtcsvt/jtcsvtcorr[][1]/g' > 3tt_incl_BTAG_UP.config
cat 3tt_incl.config  | sed -e 's/jtcsvt/jtcsvtcorr[][2]/g' > 3tt_incl_BTAG_DOWN.config
cat 3tt_incl.config  | sed -e 's/jtcsvt/jtcsvtcorr[][3]/g' > 3tt_incl_MISS_UP.config     ## FIXME HARD!!   is [] possible???
cat 3tt_incl.config  | sed -e 's/jtcsvt/jtcsvtcorr[][4]/g' > 3tt_incl_MISS_DOWN.config

cat ttbar_cr_incl.config  | sed -e 's/jtcsvt/jtcsvtcorr[][1]/g' > ttbar_cr_incl_BTAG_UP.config
cat ttbar_cr_incl.config  | sed -e 's/jtcsvt/jtcsvtcorr[][2]/g' > ttbar_cr_incl_BTAG_DOWN.config
cat ttbar_cr_incl.config  | sed -e 's/jtcsvt/jtcsvtcorr[][3]/g' > ttbar_cr_incl_MISS_UP.config ## FIXME HARD!!   is [] possible???                                                                                                                                       
cat ttbar_cr_incl.config  | sed -e 's/jtcsvt/jtcsvtcorr[][4]/g' > ttbar_cr_incl_MISS_DOWN.config

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl_BTAG_UP.config -input $i -output systematics/BTAG_UP/3tt_incl/$i"; done > skim_3tt_incl_BTAG_UP.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl_BTAG_UP.config -input $i -output systematics/BTAG_UP/ ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl_BTAG_UP.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl_BTAG_DOWN.config -input $i -output systematics/BTAG_DOWN/3tt_incl/$i"; done > skim_3tt_incl_BTAG_DOWN.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl_BTAG_DOWN.config -input $i -output systematics/BTAG_DOWN/ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl_BTAG_DOWN.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl_MISS_UP.config -input $i -output systematics/MISS_UP/3tt_incl/$i"; done > skim_3tt_incl_MISS_UP.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl_MISS_UP.config -input $i -output systematics/MISS_UP/ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl_MISS_UP.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl_MISS_DOWN.config -input $i -output systematics/MISS_DOWN/3tt_incl/$i"; done > skim_3tt_incl_MISS_DOWN.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl_MISS_DOWN.config -input $i -output systematics/MISS_DOWN/ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl_MISS_DOWN.sh

cd systematics

###  ana-nominal/systematics/BTAG_DOWN

cd BTAG_UP

for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i  -btagsys 1";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i -btagsys 1"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i  -btagsys 1"; done > 3tt_incl/classification.sh

cp 3tt_incl/*sh ttbar_cr_incl/.

cd ..


cd BTAG_DOWN

for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i  -btagsys 2";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i -btagsys 2 "; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i  -btagsys 2"; done > 3tt_incl/classification.sh

cp 3tt_incl/*sh ttbar_cr_incl/.

cd ..


cd MISS_UP


for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i  -btagsys 3";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i -btagsys 3"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i  -btagsys 3"; done > 3tt_incl/classification.sh

cp 3tt_incl/*sh ttbar_cr_incl/.

cd ..


cd MISS_DOWN

for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i  -btagsys 4";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i -btagsys 4"; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i  -btagsys 4"; done > 3tt_incl/classification.sh

cp 3tt_incl/*sh ttbar_cr_incl/.

cd ..

cd MET_UP


for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i ";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i "; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i"; done > 3tt_incl/classification.sh

cp ../../*incl.config .
cp 3tt_incl/*sh ttbar_cr_incl/.

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl.config -input $i -output 3tt_incl/$i"; done > skim_3tt_incl.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl.config -input $i -output ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl.sh
 
cd ..


cd MET_DOWN
 

for i in $(ls $TARGETDIR/mc | grep .root);
  do echo -ne "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input $i  ";
  if [[ "$i" == TH*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nnexpert.sh

for i in $(ls $TARGETDIR/mc | grep .root); 
  do echo -ne "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input $i "; 
  if [[ "$i" == *SemiLept*.root ]]; then
      echo -ne " -signal 1";
  fi;
  echo;
done > 3tt_incl/nntopexpert.sh


for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input $i"; done > 3tt_incl/classification.sh

cp ../../*incl.config .
cp 3tt_incl/*sh ttbar_cr_incl/.

for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x 3tt_incl.config -input $i -output 3tt_incl/$i"; done > skim_3tt_incl.sh
for i in $(ls $TARGETDIR/mc | grep .root); do echo "th_skim_x ttbar_cr_incl.config -input $i -output ttbar_cr_incl/$i"; done > skim_ttbar_cr_incl.sh


cd ..



echo "Folder mc successfully setup!"

####################                                                                                                                
##      DATA      ##                                                                                                                
#################### 

cd $TARGETDIR/data

for i in $(find -maxdepth 1 ! -iname "*json.root" -name "*root"); do echo "th_skim_x skim_json.config -input $i -output ${i%.root}_json.root "; done > skim_json.sh

for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_skim_x ana-nominal.config -input ${i%.root}_json.root -output ana-nominal/${i%.root}_json.root"; done > skim_ana_nominal.sh



cd ana-nominal

for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_skim_x 3tt_incl.config -input ${i%.root}_json.root -output 3tt_incl/${i%.root}_json.root"; done > skim_3tt_incl.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_skim_x ttbar_cr_incl.config -input ${i%.root}_json.root -output ttbar_cr_incl/${i%.root}_json.root"; done > skim_ttbar_cr_incl.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_toppt_x -inputs ${i%.root}_json.root"; done > topptreweight.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_csvreweight_x -inputs ${i%.root}_json.root"; done > csvreweight.sh

cd ./3tt_incl

for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_toppt_x -inputs ${i%.root}_json.root"; done > topptreweight.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_csvreweight_x -inputs ${i%.root}_json.root"; done > csvreweight.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input ${i%.root}_json.root"; done > nnexpert.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights/ -input ${i%.root}_json.root"; done > nntopexpert.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input ${i%.root}_json.root"; done > classification.sh

cd ../ttbar_cr_incl

for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_toppt_x -inputs ${i%.root}_json.root"; done > topptreweight.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_csvreweight_x -inputs ${i%.root}_json.root"; done > csvreweight.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_recexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-reco-popov/nn_reco/weights/ -input ${i%.root}_json.root"; done > nnexpert.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_toprecexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-top-reco-popov/nn_reco/weights -input ${i%.root}_json.root"; done > nntopexpert.sh
for i in $(ls $TARGETDIR/data | grep .root | grep -v json); do echo "th_bdtexpertpop_x -bdtpath /storage/a/singletophiggs/th_Moriond/nn-class-popov/nn_class/weights -input ${i%.root}_json.root"; done > classification.sh

cd $TARGETDIR

echo "Folder data successfully setup!"

echo "-------------------------------"

echo "     READY TO BE ANALYZED!     "

echo "-------------------------------"