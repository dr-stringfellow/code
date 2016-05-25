namespace beff{
/*
float ptmin[] = {30, 210, 260, 320, 400, 500};
float ptmax[] = {210,260, 320, 400, 500, 670};

size_t bins=6;
*/
/*
From ttbar for 30-200
Working Points:
CSV_L = 1.01 +/- 0.04
CSV_M = 0.97 +/- 0.04
CSV_T = 0.96 +/- 0.04
*/
/*
// Tagger: CSVL within 30 < pt < 670 GeV, abs(eta) < 2.4, x = pt
float CSVL_SFb(float x) { if(x < 210) return 1.01 ; return 1.02658*((1.+(0.0195388*x))/(1.+(0.0209145*x))); }
float CSVL_SFb_error[] = {
 0.04,
 0.0257175,
 0.026424,
 0.0264928,
 0.0315127,
 0.030734,
 0.0438259 };

// Tagger: CSVM within 30 < pt < 670 GeV, abs(eta) < 2.4, x = pt
float CSVM_SFb(float x) {  if(x < 210) return 0.97 ; return  0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));}
 float CSVM_SFb_error[] = {
 0.04,
 0.0409675,
 0.0420284,
 0.0541299,
 0.0578761,
 0.0655432 };

// Tagger: CSVT within 30 < pt < 670 GeV, abs(eta) < 2.4, x = pt
float CSVT_SFb(float x) {  if(x < 210) return 0.96 ; return   0.901615*((1.+(0.552628*x))/(1.+(0.547195*x))); }
float CSVT_SFb_error[] = {
 0.04,
 0.0653621,
 0.0712586,
 0.094589,
 0.0777011,
 0.0866563 };
 
}
*/

float ptmin[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
float ptmax[] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};
size_t bins=16;

// Tagger: CSVL within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
float CSVL_SFb(float x) { return  0.981149*((1.+(-0.000713295*x))/(1.+(-0.000703264*x)));}
float CSVL_SFb_error[] = {
 0.0484285,
 0.0126178,
 0.0120027,
 0.0141137,
 0.0145441,
 0.0131145,
 0.0168479,
 0.0160836,
 0.0126209,
 0.0136017,
 0.019182,
 0.0198805,
 0.0386531,
 0.0392831,
 0.0481008,
 0.0474291 };

// Tagger: CSVM within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
float CSVM_SFb(float x) { return  0.726981*((1.+(0.253238*x))/(1.+(0.188389*x)));}
float CSVM_SFb_error[] = {
 0.0554504,
 0.0209663,
 0.0207019,
 0.0230073,
 0.0208719,
 0.0200453,
 0.0264232,
 0.0240102,
 0.0229375,
 0.0184615,
 0.0216242,
 0.0248119,
 0.0465748,
 0.0474666,
 0.0718173,
 0.0717567 };

// Tagger: CSVT within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
float CSVT_SFb(float x) { return 0.869965*((1.+(0.0335062*x))/(1.+(0.0304598*x))); }
float CSVT_SFb_error[] = {
 0.0567059,
 0.0266907,
 0.0263491,
 0.0342831,
 0.0303327,
 0.024608,
 0.0333786,
 0.0317642,
 0.031102,
 0.0295603,
 0.0474663,
 0.0503182,
 0.0580424,
 0.0575776,
 0.0769779,
 0.0898199 };
}
