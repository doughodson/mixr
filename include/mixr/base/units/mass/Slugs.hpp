
#ifndef __mixr_base_Slugs_H__
#define __mixr_base_Slugs_H__

#include "mixr/base/units/mass/Mass.hpp"

#include "mixr/base/units/util/mass_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Slugs
// Description:  KiloGram * 0.06852176585
//------------------------------------------------------------------------------
class Slugs : public Mass
{
    DECLARE_SUBCLASS(Slugs, Mass)

public:
    Slugs();
    Slugs(const double);
    Slugs(const Mass&);

    static double convertStatic(const Mass &n)       { return n.toMass() * mass::KG2SL; }

    double toMass() const final                      { return getValue() * mass::SL2KG; }
    double fromMass(const double a) const final      { return a * mass::KG2SL; }
};

}
}

#endif
