
#ifndef __mixr_base_Watts_H__
#define __mixr_base_Watts_H__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: Watts
// Description: An instance of KiloWatts with its value equal to 1.0 is one base
// unit for power.
//----------------------------------------------------------------------------
class Watts final: public Power
{
    DECLARE_SUBCLASS(Watts, Power)

public:
    Watts();
    Watts(const double);
    Watts(const Power&);

    static double convertStatic(const Power &n)      { return n.toPower(); }

    double toPower() const final                     { return getValue(); }
    double fromPower(const double a) const final     { return a; }
};

}
}

#endif
