
#ifndef __mixr_FootPounds_H__
#define __mixr_FootPounds_H__

#include "mixr/base/units/energy/Energy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: FootPounds
// Description:  Joules * 0.7376
//------------------------------------------------------------------------------
class FootPounds final: public Energy
{
    DECLARE_SUBCLASS(FootPounds, Energy)

public:
    FootPounds();
    FootPounds(const double);
    FootPounds(const Energy&);

    static double convertStatic(const Energy& n)      { return n.toEnergy() * energy::J2FP; }

    double toEnergy() const final                     { return getValue() * energy::FP2J; }
    double fromEnergy(const double a) const final     { return a * energy::J2FP; }
};

}
}

#endif
