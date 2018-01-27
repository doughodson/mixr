
#ifndef __mixr_base_KiloWatts_H__
#define __mixr_base_KiloWatts_H__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: KiloWatts
// Description: Watts * 0.001
//----------------------------------------------------------------------------
class KiloWatts final: public Power
{
   DECLARE_SUBCLASS(KiloWatts, Power)

public:
    KiloWatts();
    KiloWatts(const double);
    KiloWatts(const Power&);

    static double convertStatic(const Power &n)      { return n.toPower() * power::W2KW; }

    double toPower() const final                     { return getValue() * power::KW2W; }
    double fromPower(const double a) const final     { return a * power::W2KW; }
};

}
}

#endif
