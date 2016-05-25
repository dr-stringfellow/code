th_drawdc_x MC.config inverted_Processes.config label_3tmu.config -ipath ./ -rootfile mlfit.root -channel tH_3tt_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo tH_3tt_mu.root -xtitle "NN output" -signalfac 50 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix inv

th_drawdc_x MC.config inverted_Processes.config label_3tel.config -ipath ./ -rootfile mlfit.root -channel tH_3tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo tH_3tt_el.root -xtitle "NN output" -signalfac 50 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix inv

th_drawdc_x MC.config inverted_Processes.config label_4tmu.config -ipath ./ -rootfile mlfit.root -channel tH_4tt_mu -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo tH_4tt_mu.root -xtitle "NN output" -signalfac 20 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix inv

th_drawdc_x MC.config inverted_Processes.config label_4tel.config -ipath ./ -rootfile mlfit.root -channel tH_4tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo tH_4tt_el.root -xtitle "NN output" -signalfac 20 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix inv



#th_drawdc_x MC.config inverted_Processes.config label_4tel.config -ipath ./ -rootfile mlfit.root -channel tH_4tt_el -residuals 1 -reversemc 0 -batch 1 -blind 0 -theta 0 -hpath ./ -histo tH_4tt_el.root -xtitle "NN output" -signalfac 1 -logy 0 -resmax 1.2 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -suffix inv -combine 1