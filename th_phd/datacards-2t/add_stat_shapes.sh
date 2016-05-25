for i in tH_3tt_mu tH_3tt_el tH_4tt_mu tH_4tt_el; do
    python  add_stat_shapes.py --prefix='' --normalize --filter=*nominal ${i}.root  ${i}.root --writeDC
done
