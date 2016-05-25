/**
 * A set of functions to convert between user and NDC coordinates. Before using any of them, one
 * must call Update for the pad.
 */

#pragma once

#include <TPad.h>


namespace undc
{

/// Converts NDC x into user x
double UtoX(TPad const &pad, double u)
{
    double const xRange = pad.GetX2() - pad.GetX1();
    return u * xRange + pad.GetX1();
}


/// Converts NDC y into user y
double VtoY(TPad const &pad, double v)
{
    double const yRange = pad.GetY2() - pad.GetY1();
    return v * yRange + pad.GetY1();
}


/// Converts user x into NDC x
double XtoU(TPad const &pad, double x)
{
    double const xRange = pad.GetX2() - pad.GetX1();
    return (x - pad.GetX1()) / xRange;
}


/// Converts user y into NDC y
double YtoV(TPad const &pad, double y)
{
    double const yRange = pad.GetY2() - pad.GetY1();
    return (y - pad.GetY1()) / yRange;
}

};  // end of namespace undc