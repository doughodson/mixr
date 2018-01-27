
#ifndef __mixr_base_KiloGrams_H__
#define __mixr_base_KiloGrams_H__

#include "mixr/base/units/mass/Mass.hpp"

#include "mixr/base/units/util/mass_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloGrams
// Description:  An instance of KiloGrams with its value equal to 1.0 is one
//               base unit for mass.
//------------------------------------------------------------------------------
class KiloGrams final: public Mass
{
    DECLARE_SUBCLASS(KiloGrams, Mass)

public:
    KiloGrams();
    KiloGrams(const double);
    KiloGrams(const Mass&);

    static double convertStatic(const Mass &n)       { return n.toMass(); }

    double toMass() const final                      { return getValue(); }
    double fromMass(const double a) const final      { return a; }
};

}
}

#endif
