for i in  $(ls tH_2t_*.root); do
    python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal ${i}  ${i} --writeDC
done
