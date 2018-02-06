
#ifndef __mixr_base_DecibelSquareMeters_H__
#define __mixr_base_DecibelSquareMeters_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"
#include <cmath>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: DecibelSquareMeters
// Description: 10 * Log(Square Meters)
//------------------------------------------------------------------------------
class DecibelSquareMeters final: public Area
{
    DECLARE_SUBCLASS(DecibelSquareMeters, Area)

public:
    explicit DecibelSquareMeters();

private:
   // convert decibel square meters to square meters (our base unit)
   double convertToBaseUnit(const double x) final    { return std::pow(10.0, x/10.0); }
};

}
}

#endif
