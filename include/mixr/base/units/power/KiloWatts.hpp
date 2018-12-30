
#ifndef __mixr_base_KiloWatts_HPP__
#define __mixr_base_KiloWatts_HPP__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: KiloWatts
// Description: Concrete class to specify power in KiloWatts
//----------------------------------------------------------------------------
class KiloWatts final: public Power
{
   DECLARE_SUBCLASS(KiloWatts, Power)

public:
   explicit KiloWatts();

private:
   // convert KiloWatts to Watts (our base unit)
   double convertToBaseUnit(const double x) final     { return x * power::KW2W; }
};

}
}

#endif
