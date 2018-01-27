
#ifndef __mixr_base_SquareMilliMeters_H__
#define __mixr_base_SquareMilliMeters_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareMilliMeters
// Description: Square Meters * 1000000.0
//------------------------------------------------------------------------------
class SquareMilliMeters final: public Area
{
    DECLARE_SUBCLASS(SquareMilliMeters, Area)

public:
    SquareMilliMeters();
    SquareMilliMeters(const double);
    SquareMilliMeters(const Area&);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SMM; }

    double toArea() const final                     { return getValue() * area::SMM2SM; }
    double fromArea(const double a) const final     { return a * area::SM2SMM; }
};

}
}

#endif
