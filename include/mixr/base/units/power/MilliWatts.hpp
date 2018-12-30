
#ifndef __mixr_base_MilliWatts_HPP__
#define __mixr_base_MilliWatts_HPP__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: MilliWatts
// Description: Concrete class to specify power in MilliWatts
//----------------------------------------------------------------------------
class MilliWatts final: public Power
{
   DECLARE_SUBCLASS(MilliWatts, Power)

public:
   explicit MilliWatts();

private:
   // convert MilliWatts to Watts (our base unit)
   double convertToBaseUnit(const double x) final     { return x * power::MW2W; }
};

}
}

#endif
