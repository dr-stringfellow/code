/* This file is provided for inclusion by the submodules (TreeRunner, etc.) which require declarations
   of the KITA objects.*/

#ifndef KITAOBJECTS_HPP
#define KITAOBJECTS_HPP

#include "KITA/KITAObjects/interface/KITA4Vector.h"
#include "KITA/KITAObjects/interface/KITAElectron.h"
#include "KITA/KITAObjects/interface/KITAConversionInfo.h"
#include "KITA/KITAObjects/interface/KITAGenInfo.h"
#include "KITA/KITAObjects/interface/KITAGenJet.h"
#include "KITA/KITAObjects/interface/KITAGenMet.h"
#include "KITA/KITAObjects/interface/KITAGenParticle.h"
#include "KITA/KITAObjects/interface/KITAGenTopEvent.h"
#include "KITA/KITAObjects/interface/KITAGenSingleTopEvent.h"
#include "KITA/KITAObjects/interface/KITAGenFlavor.h"
#include "KITA/KITAObjects/interface/KITAHlt.h"
#include "KITA/KITAObjects/interface/KITAJet.h"
#include "KITA/KITAObjects/interface/KITAPrimaryVertex.h"
#include "KITA/KITAObjects/interface/KITAMet.h"
#include "KITA/KITAObjects/interface/KITAMisc.h"
#include "KITA/KITAObjects/interface/KITAMuon.h"
#include "KITA/KITAObjects/interface/KITAPFMet.h"
#include "KITA/KITAObjects/interface/KITAJetWithSubjets.h"
#include "KITA/KITAObjects/interface/KITATopJet.h"
#include "KITA/KITAObjects/interface/KITADataCleaning.h"

//4-vector addition:
template<typename T>
KITA4Vector operator+(const T & l, const T & r){
    typedef typename T::Scalar scalar;
    KITA4Vector left(l);
    KITA4Vector right(r);
    scalar data_l[4];
    scalar data_r[4];
    left.GetCoordinates(data_l);
    right.GetCoordinates(data_r);
    for(int i=0; i<4; ++i){
        data_l[i] += data_r[i];
    }
    T result;
    result.SetCoordinates(data_l);
    return result;
}

inline KITA4Vector operator*(const KITA4Vector & rhs, double s){
  KITA4Vector result(rhs.x()*s, rhs.y()*s, rhs.z()*s, rhs.E()*s);
  return result;
}

inline KITA4Vector operator*(double s, const KITA4Vector & rhs){
  return rhs * s;
}

#endif
