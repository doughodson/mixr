
#ifndef __mixr_base_SquareFeet_H__
#define __mixr_base_SquareFeet_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareFeet
// Description: Square Meters * 10.76391
//------------------------------------------------------------------------------
class SquareFeet final: public Area
{
    DECLARE_SUBCLASS(SquareFeet, Area)

public:
    SquareFeet();
    SquareFeet(const double);
    SquareFeet(const Area&);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SFT; }

    double toArea() const final                     { return getValue() * area::SFT2SM; }
    double fromArea(const double a) const final     { return a * area::SM2SFT; }
};

}
}

#endif
