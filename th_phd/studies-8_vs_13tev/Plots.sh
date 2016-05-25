th_mctomc_with_dat_normalized_x com.config -varexpr "tpt" -nbinsx 30 -xmin 0 -xmax 600 -datname "toppt" -output pdf_8_vs_13tev/tH_8_vs_13tev_MC_toppt_ns.pdf  
th_mctomc_with_dat_normalized_x com.config -varexpr "Hpt" -nbinsx 30 -xmin 0 -xmax 600 -datname "Hpt" -output pdf_8_vs_13tev/tH_8_vs_13tev_MC_Higgspt_ns.pdf  

th_mctomc_with_dat_normalized_x com.config -varexpr "abs(teta)" -nbinsx 30 -xmin 0 -xmax 6 -datname "topeta" -output pdf_8_vs_13tev/tH_8_vs_13tev_MC_topeta_ns.pdf  
th_mctomc_with_dat_normalized_x com.config -varexpr "abs(Heta)" -nbinsx 30 -xmin 0 -xmax 6 -datname "Heta" -output pdf_8_vs_13tev/tH_8_vs_13tev_MC_Higgseta_ns.pdf  

th_mctomc_with_dat_normalized_x com.config -varexpr "abs(ty)" -nbinsx 30 -xmin 0 -xmax 3.8 -datname "topy" -output pdf_8_vs_13tev/tH_8_vs_13tev_MC_topeta_ns.pdf  
th_mctomc_with_dat_normalized_x com.config -varexpr "abs(Hy)" -nbinsx 30 -xmin 0 -xmax 3.8 -datname "Hy" -output pdf_8_vs_13tev/tH_8_vs_13tev_MC_Higgseta_ns.pdf  

th_mctomc_with_dat_normalized_x com.config -varexpr "Sum\$(jtpt>30)" -nbinsx 12 -xmin -0.5 -xmax 11.5 -datname "njt" -output pdf_8_vs_13tev/tH_8_vs_13tev_MC_njt_ns.pdf -mcweights+ "13tev:(abs(Hdauid[0])==5)"
th_mctomc_with_dat_normalized_x com_ttbar.config -varexpr "Sum\$(jtpt>30)" -nbinsx 12 -xmin -0.5 -xmax 11.5 -datname "njt" -output pdf_8_vs_13tev/tH_8_vs_13tev_MC_njt_ns.pdf -mcweights+ "8tev:(nlepw==1)"
