for i in  $(ls lumi_*tH_3m*.root); do
    python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal ${i}  ${i} --writeDC
done

for i in  $(ls lumi_*tH_4m*.root); do
    python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal ${i}  ${i} --writeDC
done
