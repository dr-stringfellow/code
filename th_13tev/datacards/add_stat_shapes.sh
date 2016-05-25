for i in  $(ls tH_3t_*.root); do
    python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal ${i}  ${i} --writeDC
done

for i in  $(ls tH_4t_*.root); do
    python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal ${i}  ${i} --writeDC
done
