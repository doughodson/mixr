
#ifndef __mixr_base_DecibelWatts_H__
#define __mixr_base_DecibelWatts_H__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

#include <cmath>

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: DecibelWatts
// Description: 10 Log(Watts)
//----------------------------------------------------------------------------
class DecibelWatts final: public Power
{
    DECLARE_SUBCLASS(DecibelWatts, Power)

public:
    DecibelWatts();
    DecibelWatts(const double);
    DecibelWatts(const Power&);

    static double convertStatic(const Power &n)      { return (10.0 * std::log10(n.toPower())); }

    double toPower() const final                     { return std::pow(10.0, (getValue()/10.0)); }
    double fromPower(const double a) const final     { return 10.0 * std::log10(a) ; }
};

}
}

#endif
