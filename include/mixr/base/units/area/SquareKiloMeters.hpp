
#ifndef __mixr_base_SquareKiloMeters_H__
#define __mixr_base_SquareKiloMeters_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareKiloMeters
// Description: Square Meters * 0.000001
//------------------------------------------------------------------------------
class SquareKiloMeters final: public Area
{
    DECLARE_SUBCLASS(SquareKiloMeters, Area)

public:
    SquareKiloMeters();
    SquareKiloMeters(const double);
    SquareKiloMeters(const Area&);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SKM; }

    double toArea() const final                     { return getValue() * area::SKM2SM; }
    double fromArea(const double a) const final     { return a * area::SM2SKM; }
};

}
}

#endif
