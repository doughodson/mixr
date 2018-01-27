
#ifndef __mixr_Joules_H__
#define __mixr_Joules_H__

#include "mixr/base/units/energy/Energy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Joules
// Description: An instance of Joules with its value equal to 1.0 is one
//              base unit for energy.
//------------------------------------------------------------------------------
class Joules final: public Energy
{
    DECLARE_SUBCLASS(Joules, Energy)

public:
    Joules();
    Joules(const double);
    Joules(const Energy&);

    static double convertStatic(const Energy& n)       { return n.toEnergy(); }

    double toEnergy() const final                      { return getValue(); }
    double fromEnergy(const double a) const final      { return a; }
};

}
}

#endif
