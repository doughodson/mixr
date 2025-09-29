
#ifndef __mixr_base_DecibelMilliWatts_HPP__
#define __mixr_base_DecibelMilliWatts_HPP__

#include "mixr/base/qty/power/IPower.hpp"

#include "mixr/base/qty/util/power_utils.hpp"

#include <cmath>

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: DecibelMilliWatts
// Description: Concrete class to specify power in Decibel MilliWatts
//----------------------------------------------------------------------------
class DecibelMilliWatts final: public IPower
{
   DECLARE_SUBCLASS(DecibelMilliWatts, IPower)

public:
   explicit DecibelMilliWatts();

private:
   // convert Decibel MilliWatts to Watts (our base unit)
   double convertToBaseUnit(const double x) final     { return power::MW2W * std::pow(10.0, x/10.0); }
};

}
}

#endif
