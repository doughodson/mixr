
#ifndef __mixr_KiloWattHours_H__
#define __mixr_KiloWattHours_H__

#include "mixr/base/units/energy/Energy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloWattHours
// Description: Joules * 0.000000277778
//------------------------------------------------------------------------------
class KiloWattHours final: public Energy
{
    DECLARE_SUBCLASS(KiloWattHours, Energy)

public:
    KiloWattHours();
    KiloWattHours(const double);
    KiloWattHours(const Energy&);

    static double convertStatic(const Energy& n)     { return n.toEnergy() * energy::J2KWH; }

    double toEnergy() const final                    { return getValue() * energy::KWH2J; }
    double fromEnergy(const double a) const final    { return a * energy::J2KWH; }
};

}
}

#endif
