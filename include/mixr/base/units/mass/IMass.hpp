
#ifndef __mixr_base_IMass_HPP__
#define __mixr_base_IMass_HPP__

#include "mixr/base/units/IQuantity.hpp"

#include "mixr/base/units/util/mass_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: IMass
// Description: Interface class for mass.  Base unit is Kilogram.
//------------------------------------------------------------------------------
class IMass : public IQuantity
{
   DECLARE_SUBCLASS(IMass, IQuantity)

public:
   explicit IMass();

   // conversion methods
   double getValueInGrams() const       { return getValue() * mass::KG2G;  }
   double getValueKileGrams() const     { return getValue();               }
   double getValueSlugs() const         { return getValue() * mass::KG2SL; }
};

}
}

#endif
