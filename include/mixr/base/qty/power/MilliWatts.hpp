
#ifndef __mixr_base_MilliWatts_HPP__
#define __mixr_base_MilliWatts_HPP__

#include "mixr/base/qty/power/IPower.hpp"

#include "mixr/base/qty/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: MilliWatts
// Description: Concrete class to specify power in MilliWatts
//----------------------------------------------------------------------------
class MilliWatts final: public IPower
{
   DECLARE_SUBCLASS(MilliWatts, IPower)

public:
   explicit MilliWatts();

private:
   // convert MilliWatts to Watts (our base unit)
   double convertToBaseUnit(const double x) final     { return x * power::MW2W; }
};

}
}

#endif
