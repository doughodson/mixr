
#ifndef __mixr_base_Mass_H__
#define __mixr_base_Mass_H__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/mass_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Mass
// Description: Abstract class for mass.  Base unit is Kilogram.
//------------------------------------------------------------------------------
class Mass : public Quantity
{
   DECLARE_SUBCLASS(Mass, Quantity)

public:
   explicit Mass();

   // conversion methods
   double getValueInGrams() const       { return getValue() * mass::KG2G;  }
   double getValueKileGrams() const     { return getValue();               }
   double getValueSlugs() const         { return getValue() * mass::KG2SL; }
};

}
}

#endif
