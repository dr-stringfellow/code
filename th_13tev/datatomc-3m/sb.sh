##########
# LUMI 0 #
##########th_datatomc_x tH-sb.config -xtitle "CSV of first Higgs jet" -varexpr "jtcostheta_j2[1]" -stackmc 1 -batch 1 -nbinsx 20 -xmin 0 -xmax 1 -overflow 1

$th_datatomc_x tH-sb.config -varexpr "jtcostheta_l[0]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_l0"  -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_l[1]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25  -suffix "ns_jtcostheta_l1" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_l[2]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_l2" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_l[3]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_l3" -data "" &
$
$
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_cm[0]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_cm0" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_cm[1]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_cm1" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_cm[2]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_cm2" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_cm[3]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_cm3" -data "" &
$
$
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j1[0]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j10" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j1[1]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j11" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j1[2]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j12" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j1[3]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j13" -data "" &
$
$
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j2[0]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j20" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j2[1]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j21" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j2[2]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j22" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j2[3]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j23" -data "" &
$
$
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j3[0]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j30" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j3[1]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j31" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j3[2]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j32" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j3[3]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j33" -data "" &
$
$
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j4[0]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j40" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j4[1]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j41" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j4[2]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j42" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_j4[3]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j43" -data "" &
$
$
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_cm[topjtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_cm_top" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_cm[hbbjtidx[0][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_cm_hbb0" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_cm[hbbjtidx[1][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_cm_hbb1" -data "" &
$th_datatomc_x tH-sb.config -varexpr "jtcostheta_cm[ljtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_cm_ljt" -data "" &


th_datatomc_x tH-sb.config -varexpr "jtcostheta_l[topjtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_l_top" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_l[hbbjtidx[0][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_l_hbb0" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_l[hbbjtidx[1][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_l_hbb1" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_l[ljtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_l_ljt" -data "" 

th_datatomc_x tH-sb.config -varexpr "jtcostheta_j1[topjtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j1_top" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j1[hbbjtidx[0][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j1_hbb0" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j1[hbbjtidx[1][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j1_hbb1" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j1[ljtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j1_ljt" -data "" 


th_datatomc_x tH-sb.config -varexpr "jtcostheta_j2[topjtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j2_top" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j2[hbbjtidx[0][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j2_hbb0" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j2[hbbjtidx[1][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j2_hbb1" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j2[ljtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j2_ljt" -data "" 

th_datatomc_x tH-sb.config -varexpr "jtcostheta_j3[topjtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j3_top" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j3[hbbjtidx[0][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j3_hbb0" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j3[hbbjtidx[1][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j3_hbb1" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j3[ljtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j3_ljt" -data "" 

th_datatomc_x tH-sb.config -varexpr "jtcostheta_j4[topjtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j4_top" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j4[hbbjtidx[0][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j4_hbb0" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j4[hbbjtidx[1][0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j4_hbb1" -data "" 
th_datatomc_x tH-sb.config -varexpr "jtcostheta_j4[ljtidx[0]]"   -batch  1 -xmin -1 -xmax 1 -nbinsx 25 -suffix "ns_jtcostheta_j4_ljt" -data "" 
