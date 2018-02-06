
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
    explicit SquareCentiMeters();

private:
   // convert square centimeters to square meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * area::SCM2SM; }
};

}
}

#endif
