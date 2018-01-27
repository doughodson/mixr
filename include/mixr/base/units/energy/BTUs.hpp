
#ifndef __mixr_BTUs_H__
#define __mixr_BTUs_H__

#include "mixr/base/units/energy/Energy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: BTUs
// Description:  Joules * 9.478 x 10 -04
//------------------------------------------------------------------------------
class BTUs final: public Energy
{
    DECLARE_SUBCLASS(BTUs, Energy)

public:
    BTUs();
    BTUs(const double);
    BTUs(const Energy&);

    static double convertStatic(const Energy& n)     { return n.toEnergy() * energy::J2BTU; }

    double toEnergy() const final                    { return getValue() * energy::BTU2J; }
    double fromEnergy(const double a) const final    { return a * energy::J2BTU; }
};

}
}

#endif
