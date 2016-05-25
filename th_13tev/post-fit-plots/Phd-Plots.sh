#### ct = -1
th_plotdc_x MC.config Processes.config label_3m.config -ipath ./ -rootfile fits/mlfit_0.root -channel tH_3m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_3m_0.root -xtitle "BDT output" -signalfac 400 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -xbinmin -0.6 -xbinmax 0.6 -setmu 1 -labels+ "0.64:0.86:42:0.05:#it{Postfit}" -suffix "_0" -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -changeBinning 1 -xbinmin 3 -xbinmax 15  -blind 1

th_plotdc_x MC.config Processes.config label_4m.config -ipath ./ -rootfile fits/mlfit_0.root -channel tH_4m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_4m_0.root -xtitle "BDT output" -signalfac 80 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -xbinmin -0.6 -xbinmax 0.6 -setmu 1 -labels+ "0.64:0.86:42:0.05:#it{Postfit}" -suffix "_0"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


th_plotdc_x MC.config Processes.config label_3m.config -ipath ./ -rootfile fits/mlfit_0.root -channel tH_3m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_3m_0.root -xtitle "BDT output" -signalfac 400 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93  -prefit 1 -labels+ "0.64:0.86:42:0.05:#it{Prefit}" -suffix "_0"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


th_plotdc_x MC.config Processes.config label_4m.config -ipath ./ -rootfile fits/mlfit_0.root -channel tH_4m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_4m_0.root -xtitle "BDT output" -signalfac 80 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93  -prefit 1 -labels+ "0.64:0.86:42:0.05:#it{Prefit}" -suffix "_0"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-1,  #it{C}_{V}=1" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


#### ct = 1 SM
th_plotdc_x MC.config Processes.config label_3m.config -ipath ./ -rootfile fits/mlfit_12.root -channel tH_3m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_3m_12.root -xtitle "BDT output" -signalfac 400 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -xbinmin -0.6 -xbinmax 0.6 -setmu 1 -labels+ "0.64:0.86:42:0.05:#it{Postfit}" -suffix "_12" -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=1,  #it{C}_{V}=1"  -blind 1


th_plotdc_x MC.config Processes.config label_4m.config -ipath ./ -rootfile fits/mlfit_12.root -channel tH_4m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_4m_12.root -xtitle "BDT output" -signalfac 150 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -xbinmin -0.6 -xbinmax 0.6 -setmu 1 -labels+ "0.64:0.86:42:0.05:#it{Postfit}" -suffix "_12" -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=1,  #it{C}_{V}=1"  -blind 1


th_plotdc_x MC.config Processes.config label_3m.config -ipath ./ -rootfile fits/mlfit_12.root -channel tH_3m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_3m_12.root -xtitle "BDT output" -signalfac 400 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93  -prefit 1 -labels+ "0.64:0.86:42:0.05:#it{Prefit}" -suffix "_12"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=1,  #it{C}_{V}=1"  -blind 1


th_plotdc_x MC.config Processes.config label_4m.config -ipath ./ -rootfile fits/mlfit_12.root -channel tH_4m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_4m_12.root -xtitle "BDT output" -signalfac 150 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93  -prefit 1 -labels+ "0.64:0.86:42:0.05:#it{Prefit}" -suffix "_12" -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=1,  #it{C}_{V}=1"  -blind 1


#### ct = -1.5 
th_plotdc_x MC.config Processes.config label_3m.config -ipath ./ -rootfile fits/mlfit_19.root -channel tH_3m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_3m_19.root -xtitle "BDT output" -signalfac 250 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -xbinmin -0.6 -xbinmax 0.6 -setmu 1 -labels+ "0.64:0.86:42:0.05:#it{Postfit}" -suffix "_19"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-1.5,  #it{C}_{V}=1.5" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


th_plotdc_x MC.config Processes.config label_4m.config -ipath ./ -rootfile fits/mlfit_19.root -channel tH_4m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_4m_19.root -xtitle "BDT output" -signalfac 80 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -xbinmin -0.6 -xbinmax 0.6 -setmu 1 -labels+ "0.64:0.86:42:0.05:#it{Postfit}" -suffix "_19"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-1.5,  #it{C}_{V}=1.5" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


th_plotdc_x MC.config Processes.config label_3m.config -ipath ./ -rootfile fits/mlfit_19.root -channel tH_3m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_3m_19.root -xtitle "BDT output" -signalfac 250 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93  -prefit 1 -labels+ "0.64:0.86:42:0.05:#it{Prefit}" -suffix "_19"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-1.5,  #it{C}_{V}=1.5" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


th_plotdc_x MC.config Processes.config label_4m.config -ipath ./ -rootfile fits/mlfit_19.root -channel tH_4m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_4m_19.root -xtitle "BDT output" -signalfac 80 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93  -prefit 1 -labels+ "0.64:0.86:42:0.05:#it{Prefit}" -suffix "_19" -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-1.5,  #it{C}_{V}=1.5"  -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


#### ct = -0.5
th_plotdc_x MC.config Processes.config label_3m.config -ipath ./ -rootfile fits/mlfit_40.root -channel tH_3m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_3m_40.root -xtitle "BDT output" -signalfac 400 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -xbinmin -0.6 -xbinmax 0.6 -setmu 1 -labels+ "0.64:0.86:42:0.05:#it{Postfit}" -suffix "_40"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-0.5,  #it{C}_{V}=0.5" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


th_plotdc_x MC.config Processes.config label_4m.config -ipath ./ -rootfile fits/mlfit_40.root -channel tH_4m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_4m_40.root -xtitle "BDT output" -signalfac 200 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93 -xbinmin -0.6 -xbinmax 0.6 -setmu 1 -labels+ "0.64:0.86:42:0.05:#it{Postfit}" -suffix "_40"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-0.5,  #it{C}_{V}=0.5" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


th_plotdc_x MC.config Processes.config label_3m.config -ipath ./ -rootfile fits/mlfit_40.root -channel tH_3m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_3m_40.root -xtitle "BDT output" -signalfac 400 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93  -prefit 1 -labels+ "0.64:0.86:42:0.05:#it{Prefit}" -suffix "_40"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-0.5,  #it{C}_{V}=0.5"  -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1


th_plotdc_x MC.config Processes.config label_4m.config -ipath ./ -rootfile fits/mlfit_40.root -channel tH_4m -residuals 1 -reversemc 0 -batch 1 -blind 0 -combine 1 -hpath ./ -histo cards/tH_4m_40.root -xtitle "BDT output" -signalfac 200 -logy 0 -resmax 0.6 -poisson 1 -legh 0.15 -legx 0.775 -legw 0.25 -cwidth 1000 -cheight 800 -legsize 0.054 -legy 0.93  -prefit 1 -labels+ "0.64:0.86:42:0.05:#it{Prefit}" -suffix "_40"  -labels+  "0.175:0.74:42:0.04:#it{C}_{t}=-0.5,  #it{C}_{V}=0.5" -changeBinning 1 -xbinmin 3 -xbinmax 15 -blind 1




