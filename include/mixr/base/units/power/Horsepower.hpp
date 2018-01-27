
#ifndef __mixr_base_Horsepower_H__
#define __mixr_base_Horsepower_H__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: Horsepower
// Description: Watts * 1341
//----------------------------------------------------------------------------
class Horsepower final: public Power
{
    DECLARE_SUBCLASS(Horsepower, Power)

public:
    Horsepower();
    Horsepower(const double);
    Horsepower(const Power&);

    static double convertStatic(const Power &n)      { return n.toPower() * power::W2HP; }

    double toPower() const final                     { return getValue() * power::HP2W; }
    double fromPower(const double a) const final     { return a * power::W2HP; }
};

}
}

#endif
