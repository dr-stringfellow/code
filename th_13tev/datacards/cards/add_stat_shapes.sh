for i in  $(ls tH_3m_*.root); do
    python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal ${i}  ${i} --writeDC
done

for i in  $(ls tH_4m_*.root); do
    python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal ${i}  ${i} --writeDC
done
