
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
    explicit SquareKiloMeters();

private:
   // convert square kilometers to square meters (our base unit)
   double convertToBaseUnit(const double x) final   { return x * area::SKM2SM; }
};

}
}

#endif
