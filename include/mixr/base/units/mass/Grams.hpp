
#ifndef __mixr_base_Grams_H__
#define __mixr_base_Grams_H__

#include "mixr/base/units/mass/Mass.hpp"

#include "mixr/base/units/util/mass_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Grams
// Description:  KiloGrams * 1000
//------------------------------------------------------------------------------
class Grams : public Mass
{
    DECLARE_SUBCLASS(Grams, Mass)

public:
    Grams();
    Grams(const double);
    Grams(const Mass&);

    static double convertStatic(const Mass &n)       { return n.toMass() * mass::KG2G; }

    double toMass() const final                      { return getValue() * mass::G2KG; }
    double fromMass(const double a) const final      { return a * mass::KG2G; }
};

}
}

#endif
