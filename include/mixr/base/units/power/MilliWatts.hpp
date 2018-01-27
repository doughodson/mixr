
#ifndef __mixr_base_MilliWatts_H__
#define __mixr_base_MilliWatts_H__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: MilliWatts
// Description: Watts * 0.01
//----------------------------------------------------------------------------
class MilliWatts final: public Power
{
    DECLARE_SUBCLASS(MilliWatts, Power)

public:
    MilliWatts();
    MilliWatts(const double);
    MilliWatts(const Power&);

    static double convertStatic(const Power &n)      { return n.toPower() * power::W2MW; }

    double toPower() const final                     { return getValue() * power::MW2W; }
    double fromPower(const double a) const final     { return a * power::W2MW; }
};

}
}

#endif
