
#ifndef __mixr_base_DecibelWatts_H__
#define __mixr_base_DecibelWatts_H__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

#include <cmath>

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: DecibelWatts
// Description: Concrete class to specify power in Decibel Watts
//----------------------------------------------------------------------------
class DecibelWatts final: public Power
{
   DECLARE_SUBCLASS(DecibelWatts, Power)

public:
   explicit DecibelWatts();

private:
   // convert Decibel Watts to Watts (our base unit)
   double convertToBaseUnit(const double x) final     { return std::pow(10.0, x / 10.0); }
};

}
}

#endif
