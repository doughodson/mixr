
#ifndef __mixr_base_SquareCentiMeters_H__
#define __mixr_base_SquareCentiMeters_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareCentiMeters
// Description: Square Meters * 10000.0
//------------------------------------------------------------------------------
class SquareCentiMeters final: public Area
{
    DECLARE_SUBCLASS(SquareCentiMeters, Area)

public:
    SquareCentiMeters();
    SquareCentiMeters(const double);
    SquareCentiMeters(const Area&);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SCM; }

    double toArea() const final                     { return getValue() * area::SCM2SM; }
    double fromArea(const double a) const final     { return a * area::SM2SCM; }
};

}
}

#endif
