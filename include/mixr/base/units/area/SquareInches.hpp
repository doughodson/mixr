
#ifndef __mixr_base_SquareInches_H__
#define __mixr_base_SquareInches_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareInches
// Description: Square Meters * 1550.0030399
//------------------------------------------------------------------------------
class SquareInches final: public Area
{
    DECLARE_SUBCLASS(SquareInches, Area)

public:
    SquareInches();
    SquareInches(const double);
    SquareInches(const Area&);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SIN; }

    double toArea() const final                     { return getValue() * area::SIN2SM; }
    double fromArea(const double a) const final     { return a * area::SM2SIN; }
};

}
}

#endif
