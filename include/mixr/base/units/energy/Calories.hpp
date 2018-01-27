
#ifndef __mixr_Calories_H__
#define __mixr_Calories_H__

#include "mixr/base/units/energy/Energy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Calories
// Description:  Joules * 0.2388888888888888889
//------------------------------------------------------------------------------
class Calories final: public Energy
{
    DECLARE_SUBCLASS(Calories, Energy)

public:
    Calories();
    Calories(const double);
    Calories(const Energy&);

    static double convertStatic(const Energy& n)       { return n.toEnergy() * energy::J2C; }

    double toEnergy() const final                      { return getValue() * energy::C2J; }
    double fromEnergy(const double a) const final      { return a * energy::J2C; }
};

}
}

#endif
