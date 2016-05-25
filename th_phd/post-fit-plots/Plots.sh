th_drawdc_x MC.config Processes.config label_3tmu.config -ipath ./ -rootfile histos-mle-withsignal.root -channel tH_3tt_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_3tt_mu.root -xtitle "NN output" -signalfac 50 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "nominal"

th_drawdc_x MC.config Processes.config label_3tel.config -ipath ./ -rootfile histos-mle-withsignal.root -channel tH_3tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_3tt_el.root -xtitle "NN output" -signalfac 50 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "nominal"

th_drawdc_x MC.config Processes.config label_4tmu.config -ipath ./ -rootfile histos-mle-withsignal.root -channel tH_4tt_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_4tt_mu.root -xtitle "NN output" -signalfac 20 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "nominal"

th_drawdc_x MC.config Processes.config label_4tel.config -ipath ./ -rootfile histos-mle-withsignal.root -channel tH_4tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_4tt_el.root -xtitle "NN output" -signalfac 20 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "nominal"


mv postfit2D_MVA_tH_4tt_mu_nominal.pdf postfit2D_MVA_tH_4tt_mu.pdf
mv postfit2D_MVA_tH_3tt_mu_nominal.pdf postfit2D_MVA_tH_3tt_mu.pdf
mv postfit2D_MVA_tH_4tt_el_nominal.pdf postfit2D_MVA_tH_4tt_el.pdf
mv postfit2D_MVA_tH_3tt_el_nominal.pdf postfit2D_MVA_tH_3tt_el.pdf
mv postfit2D_MVA_tH_4tt_mu_nominal.png postfit2D_MVA_tH_4tt_mu.png
mv postfit2D_MVA_tH_3tt_mu_nominal.png postfit2D_MVA_tH_3tt_mu.png
mv postfit2D_MVA_tH_4tt_el_nominal.png postfit2D_MVA_tH_4tt_el.png
mv postfit2D_MVA_tH_3tt_el_nominal.png postfit2D_MVA_tH_3tt_el.png


th_drawdc_x MC.config Processes.config label_2tmu.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag.root -channel tH_ttbar_cr_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_ttbar_cr_mu.root -xtitle "NN output" -signalfac 300 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag"

th_drawdc_x MC.config Processes.config label_2tel.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag.root -channel tH_ttbar_cr_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_ttbar_cr_el.root -xtitle "NN output" -signalfac 300 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag"

th_drawdc_x MC.config Processes.config label_3tmu.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag.root -channel tH_3tt_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_3tt_mu.root -xtitle "NN output" -signalfac 50 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag"

th_drawdc_x MC.config Processes.config label_3tel.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag.root -channel tH_3tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_3tt_el.root -xtitle "NN output" -signalfac 50 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag"

th_drawdc_x MC.config Processes.config label_4tmu.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag.root -channel tH_4tt_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_4tt_mu.root -xtitle "NN output" -signalfac 20 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag"

th_drawdc_x MC.config Processes.config label_4tel.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag.root -channel tH_4tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_4tt_el.root -xtitle "NN output" -signalfac 20 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag"




th_drawdc_x MC.config Processes.config label_2tmu.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag-10percent.root -channel tH_ttbar_cr_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_ttbar_cr_mu.root -xtitle "NN output" -signalfac 300 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag_10percent"

th_drawdc_x MC.config Processes.config label_2tel.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag-10percent.root -channel tH_ttbar_cr_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_ttbar_cr_el.root -xtitle "NN output" -signalfac 300 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag_10percent"

th_drawdc_x MC.config Processes.config label_3tmu.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag-10percent.root -channel tH_3tt_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_3tt_mu.root -xtitle "NN output" -signalfac 50 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag_10percent"

th_drawdc_x MC.config Processes.config label_3tel.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag-10percent.root -channel tH_3tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_3tt_el.root -xtitle "NN output" -signalfac 50 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag_10percent"

th_drawdc_x MC.config Processes.config label_4tmu.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag-10percent.root -channel tH_4tt_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_4tt_mu.root -xtitle "NN output" -signalfac 20 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag_10percent"

th_drawdc_x MC.config Processes.config label_4tel.config -ipath ./ -rootfile histos-mle-withsignal-with-2tag-10percent.root -channel tH_4tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 1 -hpath ./ -histo tH_4tt_el.root -xtitle "NN output" -signalfac 20 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.76 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix "with_2tag_10percent"


