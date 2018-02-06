
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
    explicit SquareMilliMeters();

private:
   // convert square millimeters to square meters (our base unit)
   double convertToBaseUnit(const double x) final   { return x * area::SMM2SM; }
};

}
}

#endif
