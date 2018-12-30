
#ifndef __mixr_Energy_HPP__
#define __mixr_Energy_HPP__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Energy
// Description: Abstract class for power. Base unit is a Joule.
//------------------------------------------------------------------------------
class Energy : public Quantity
{
   DECLARE_SUBCLASS(Energy, Quantity)

public:
   explicit Energy();

   // conversion methods
   double getValueInBTUs() const            { return getValue() * energy::BTU2J; }
   double getValueInCalories() const        { return getValue() * energy::C2J;   }
   double getValueInFootPoinds() const      { return getValue() * energy::FP2J;  }
   double getValueInJoules() const          { return getValue();                 }
   double getValueInKiloWattHours() const   { return getValue() * energy::KWH2J; }
};

}
}

#endif
