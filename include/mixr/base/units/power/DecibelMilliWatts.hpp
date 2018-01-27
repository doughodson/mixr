
#ifndef __mixr_base_DecibelMilliWatts_H__
#define __mixr_base_DecibelMilliWatts_H__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

#include <cmath>

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: DecibelMilliWatts
// Description: 10 Log(Watts * 1000)
//----------------------------------------------------------------------------
class DecibelMilliWatts final: public Power
{
    DECLARE_SUBCLASS(DecibelMilliWatts, Power)

public:
    DecibelMilliWatts();
    DecibelMilliWatts(const double value);
    DecibelMilliWatts(const Power& org);

    static double convertStatic(const Power &n)      { return (10.0 * std::log10(n.toPower() * power::W2MW)); }

    double toPower() const final                     { return power::MW2W * std::pow(10.0, getValue()/10.0); }
    double fromPower(const double a) const final     { return (10.0 * std::log10(a * power::W2MW)); }
};

}
}

#endif
